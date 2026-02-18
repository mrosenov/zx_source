#include "facbasemanager.h"
#include "startfacbase.hpp"
#include "stopfacbase.hpp"
#include "mapuser.h"
#include "gdeliveryserver.hpp"
#include "createfacbase_re.hpp"
#include "factionmanager.h"
#include "facmallchange.hpp"
#include "facmallsync.hpp"

namespace GNET
{
void FacBaseInfo::CheckStatusChange()
{
	int now = Timer::GetTime();
	if (status_timeout && now > status_timeout)
	{
		Log::log(LOG_ERR, "faction %d base status %d timeout", fid, status);
		if (status == STATUS_ALLOCATING)
		{
			//一般情况 这个状态应该都能恢复。此处关闭自恢复 防止一个基地在gs上加载多个实例的情况
			/*
			Reset();
			SetStatus(STATUS_INIT);
			*/
		}
		else if (status == STATUS_CLOSING)
		{
			/*
			Reset();
			SetStatus(STATUS_INIT);
			*/
		}
		else if (status == STATUS_DISCONNECT)
		{
			Reset();
			SetStatus(STATUS_INIT);
		}
	}
}

void FacBaseInfo::SetStatus(int st)
{
	status = st;
	if (status == STATUS_INIT)
		status_timeout = 0;
	else if (status == STATUS_ALLOCATING)
		status_timeout = Timer::GetTime() + SWITCH_TIMEOUT;
	else if (status == STATUS_ALLOCATE)
		status_timeout = 0;
	else if (status == STATUS_CLOSING)
		status_timeout = Timer::GetTime() + CLOSING_TIME;
	else if (status == STATUS_DISCONNECT)
		status_timeout = Timer::GetTime() + DISCONNECT_TIMEOUT;
}

void FacBaseInfo::TryStart(int gs, int roleid)
{
	gsid = gs;
	noti_roleid = roleid;
	StartFacBase start(fid);
	GProviderServer::GetInstance()->DispatchProtocol(gs, start);
	SetStatus(STATUS_ALLOCATING);
	LOG_TRACE("faction %d try start base, gs %d", fid, gs);
}

void FacBaseInfo::OnStart(int retcode, int faction, int gs, int in, const std::set<int> & mall)
{
	if (fid != faction || gsid != gs)
	{
		Log::log(LOG_ERR, "faction base onstart, fid %d:%d gs %d:%d not match, ret %d", fid, faction, gsid, gs, retcode);
		return;
	}
	if (status != STATUS_ALLOCATING)
	{
		Log::log(LOG_ERR, "faction base onstart, fid %d status %d invalid, ret %d", fid, status, retcode);
		return;
	}
	if (noti_roleid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(noti_roleid);
		if (pinfo)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, CreateFacBase_Re(ERR_SUCCESS, noti_roleid, pinfo->localsid, retcode==ERR_SUCCESS));
		noti_roleid = 0;
	}
	if (retcode != ERR_SUCCESS)
	{
		SetStatus(STATUS_INIT);
		Reset();
		return;
	}
	SetStatus(STATUS_ALLOCATE);
	index = in;
	mall_indexes = mall;
	FactionManager::Instance()->OnBaseStart(fid);

	GProviderServer::GetInstance()->BroadcastProtocol(FacMallChange(fid, mall));
}

int FacBaseInfo::GetIndex(int & gs, int & in)
{
	if (!IsAllocated())
		return ERR_FAC_BASE_ENTER_NOTOPEN; 
	gs = gsid;
	in = index;
	return ERR_SUCCESS;
}

void FacBaseInfo::OnDisconnect(int gs)
{
	if (gs != gsid)
		return;
	if (IsFree())
		return;
	Reset();
	SetStatus(STATUS_DISCONNECT);
	FactionManager::Instance()->OnBaseStop(fid);
}

void FacBaseInfo::OnMallChange(const std::set<int> & mall)
{
	mall_indexes = mall;
	GProviderServer::GetInstance()->BroadcastProtocol(FacMallChange(fid, mall));
}

void FacBaseInfo::OnFactionDel()
{
	if (IsFree())
		return;
	StopFacBase stop(fid, index);
	GProviderServer::GetInstance()->DispatchProtocol(gsid, stop);
	LOG_TRACE("faction %d stop base, gs %d index %d", fid, gsid, index);

	SetStatus(STATUS_CLOSING);
	GProviderServer::GetInstance()->BroadcastProtocol(FacMallChange(fid, std::set<int>()));
}

bool FacBaseInfo::OnStop(int retcode, int faction, int gs, int in)
{
	if (fid != faction || gsid != gs)
	{
		Log::log(LOG_ERR, "faction base onstop, fid %d:%d gs %d:%d not match, ret %d", fid, faction, gsid, gs, retcode);
		return false;
	}
	if (status != STATUS_CLOSING)
	{
		Log::log(LOG_ERR, "faction base onstop, fid %d status %d invalid, ret %d", fid, status, retcode);
		return false;
	}
	if (retcode == ERR_SUCCESS)
	{
		Reset();
		SetStatus(STATUS_INIT);
	}
	//如果 retcode 不成功 基地将被锁定为 CLOSING 状态 便于发现错误
	return retcode == ERR_SUCCESS;
}

//////////////////////////////////
void FacBaseManager::OnActTopUpdate(const GTopTable & weektop)
{
	//服务器启动时 Load 排行榜数据触发
	//周二周排行榜更新时不会触发
	if (load_top)
		return;
//	fac_top.clear();
	std::vector<GTopItem>::const_iterator it, ite = weektop.items.end();	
	int i = 0;
	for (it = weektop.items.begin(); it != ite && i < CARE_TOP_NUM; ++it, i++)
	{
//		fac_top.insert(it->id);
	//	if ((int)base_map.size() < total_num)
		base_map.insert(std::make_pair(it->id, FacBaseInfo(it->id)));
		//榜上的帮派先把坑儿占上 此处认为 gs 注册的基地数量一定会 >= CARE_TOP_NUM
		//否则即便上榜的帮派 已经被插入到 base_map里面 也有可能在 startfacbase 的时候 gs 返回失败
	}
	load_top = true;
	Log::formatlog("facbasemgr", "OnActTopUpdate base_map size %d",  base_map.size());
}

FacBaseInfo * FacBaseManager::FindBase(int faction)
{
	BaseMap::iterator it = base_map.find(faction);
	if (it == base_map.end())
		return NULL;
	return &(it->second);
}

void FacBaseManager::RegisterServer(int gs_id, int count)
{
	if (count <= 0)
		return;
	LineNumber::iterator it = line_num.find(gs_id);
	if (it != line_num.end())
	{
		Log::log(LOG_ERR, "facbase register repeat, gs %d count %d", gs_id, count);
		return;
	}
	line_num.insert(std::make_pair(gs_id, FBaseLineNumber(count)));
	//line_num[gs_id] = FBaseLineNumber(count);
	total_num += count;
	LOG_TRACE("facbase gs %d register total_num %d", gs_id, total_num);
}

bool FacBaseManager::TryAllocGS(int & gs)
{
	gs = -1;
	int max_free_num = 0;
	LineNumber::iterator it, ite = line_num.end();
	for (it = line_num.begin(); it != ite; ++it)
	{	
		int free_num = it->second.GetFree();
		if (free_num > max_free_num)
		{
			gs = it->first;
			max_free_num = free_num;
		}
	}
	if (max_free_num > 0)
	{
		it = line_num.find(gs);
		return it != line_num.end() && it->second.Alloc();
	}
	else
		return false; 
}

int FacBaseManager::TryStartBase(int faction, int noti_roleid)
{
	LOG_TRACE("try start facbase, fid=%d", faction);
	FacBaseInfo * base = FindBase(faction);
	if (base != NULL)
	{
		if (base->IsAllocated())
		{
			LOG_TRACE("facbase is already open, fid=%d", faction);
			FactionManager::Instance()->OnBaseStart(faction);
			return ERR_SUCCESS;
		}
		if (!base->IsFree())
		{
			LOG_TRACE("facbase try start status %d invalid, fid=%d", base->GetStatus(), faction);
			return ERR_FAC_BASE_START_STATUS;
		}
		int gs = 0;
		if (!TryAllocGS(gs))
		{
			Log::log(LOG_ERR, "try start facbase, alloc faild, fid=%d", faction);
			return ERR_FAC_BASE_START_FULL;
		}
		base->TryStart(gs, noti_roleid);
		return ERR_SUCCESS;
	}
	if ((int)base_map.size() >= total_num)
	{
		LOG_TRACE("try start base, but alreay full, size=%d, limit %d fid=%d", base_map.size(), total_num, faction);
		return ERR_FAC_BASE_START_FULL;
	}
	int gs = 0;
	if (!TryAllocGS(gs))
	{
		Log::log(LOG_ERR, "try alloc facbase, alloc faild, fid=%d", faction);
		return ERR_FAC_BASE_START_FULL;
	}
	base_map.insert(std::make_pair(faction, FacBaseInfo(faction)));
	base = FindBase(faction);
	if (base == NULL)
		return ERR_FAC_BASE_START_STATUS;
	base->TryStart(gs, noti_roleid);
	return ERR_SUCCESS;
}

void FacBaseManager::OnBaseStart(int retcode, int fid, int gsid, int index, const std::set<int> & mall_indexes)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
	{
		Log::log(LOG_ERR, "faction %d base start, ret %d, but not load", fid, retcode);
		return;
	}
	if (retcode != ERR_SUCCESS)
	{
		LineNumber::iterator it = line_num.find(gsid);
		if (it != line_num.end())
			it->second.Free();
	}
	base->OnStart(retcode, fid, gsid, index, mall_indexes);
}

void FacBaseManager::OnDisconnect(int gs)
{
	LineNumber::iterator it = line_num.find(gs);
	if (it == line_num.end())
		return;
	total_num -= it->second.GetMax();
	if (total_num < 0)
		total_num = 0;
	line_num.erase(it); //it 删除
	Log::log(LOG_ERR, "facbase gs %d disconnect, total_num %d", gs, total_num);
	BaseMap::iterator bit, bite = base_map.end();
	for (bit = base_map.begin(); bit != bite; bit++)
		bit->second.OnDisconnect(gs);
}

int FacBaseManager::GetBaseIndex(int fid, int & gs, int & index)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
		return ERR_FAC_BASE_ENTER_NOTOPEN;
	return base->GetIndex(gs, index);
}

/*
int FacBaseManager::EnterBase(int fid, int & gs, int & index)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
		return ERR_FAC_BASE_ENTER_NOTOPEN;
	return base->GetIndex(gs, index);
}

int FacBaseManager::OnBasePropChange(int fid, int & base_gs, int & base_index)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
		return ERR_FAC_BASE_ENTER_NOTOPEN;
	return base->GetIndex(base_gs, base_index);
}

int FacBaseManager::OnBaseBuildingProgress(int fid, int & base_gs, int & base_index)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
		return ERR_FAC_BASE_ENTER_NOTOPEN;
	return base->GetIndex(base_gs, base_index);
}
*/
void FacBaseManager::OnMallChange(int fid, const std::set<int> & mall)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
	{
		Log::log(LOG_ERR, "faction %d mall change, but not load", fid);
		return;
	}
	return base->OnMallChange(mall);
}

void FacBaseManager::OnGSConnect(unsigned int sid)
{
	FacMallSync sync;
	BaseMap::const_iterator it, ite = base_map.end();
	for (it = base_map.begin(); it != ite; ++it)
		sync.malls[it->first] = it->second.GetMall();
	if (!sync.malls.empty())
		GProviderServer::GetInstance()->Send(sid, sync);
}

void FacBaseManager::OnFactionDel(int fid)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
		return;
	base->OnFactionDel();
}

void FacBaseManager::OnBaseStop(int retcode, int fid, int gsid, int index)
{
	FacBaseInfo * base = FindBase(fid);
	if (base == NULL)
	{
		Log::log(LOG_ERR, "faction %d base stop, ret %d, but not load", fid, retcode);
		return;
	}
	if (retcode != ERR_SUCCESS)
	{
		Log::log(LOG_ERR, "faction %d base stop, err %d", fid, retcode);
		return;
	}
	if (base->OnStop(retcode, fid, gsid, index))
	{
		LineNumber::iterator it = line_num.find(gsid);
		if (it != line_num.end())
			it->second.Free();
	}
}

};
