#include "roleid"
#include "circleid"
#include "log.h"
#include "circlemanager.h"
#include "dbcircleget.hrp"
#include "gamedbclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "mapuser.h"
#include "maplinkserver.h"
#include "syncplayerfaction.hpp"
#include "dbcirclesyncarg"
#include "dbcirclesync.hrp"
#include "gcirclechat.hpp"
#include "dbcircleappoint.hrp"
#include "dbcirclejoin.hrp"
#include "updaterolecircle.hpp"
#include "gcirclememberdb"
#include "circlenotifylink.hpp"

namespace GNET
{
/////////////////////////////////// CircleInfo  //////////////////////////////////////
bool CircleInfo::SyncMember( unsigned int roleid,unsigned char level, unsigned char occupation)
{
	std::vector<GCircleMemberDB>::iterator itm = FindMember(roleid);
        if(itm!=cinfo.member.end() && (itm->level != level || itm->occupation != occupation))
        {
	        itm->level = level;	
		itm->occupation = occupation;
	        dirty = true;
		return true;
        }
	return false;
}
CircleRoleContainer::iterator CircleInfo::FindOnlineRole(unsigned int roleid)
{
/*	for(CircleRoleContainer::iterator it=online_list.begin(),ie=online_list.end();it!=ie;++it)
	{
		if(it->crole.roleid==roleid)
		{	
			return it;
		}
	}
	return online_list.end();
*/
	return online_list.find(roleid);
}

std::vector<GCircleMemberDB>::iterator CircleInfo::FindMember(unsigned int roleid)
{
	for(std::vector<GCircleMemberDB>::iterator it=cinfo.member.begin(),ie=cinfo.member.end();it!=ie;++it)
	{
		if(it->roleid==roleid)
		{	
			return it;
		}
	}
	return cinfo.member.end();
}

	
bool CircleInfo::Update(bool isbatch)
{
        UserContainer& container = UserContainer::GetInstance();
        //  Thread::RWLock::RDScoped l(container.GetLocker());
        PlayerInfo * pinfo;
        for(CircleRoleContainer::iterator it=online_list.begin(),ie=online_list.end();it!=ie;++it)
	{
	 	pinfo = container.FindRole(it->second.crole.roleid);
                if (pinfo)
                {
			SyncMember(it->second.crole.roleid,pinfo->level,pinfo->occupation);// modify 'dirty'
			if(it->second.crole.level != pinfo->level)
                               	it->second.crole.level = pinfo->level;
                        if(it->second.crole.occupation != pinfo->occupation)
                                it->second.crole.occupation = pinfo->occupation;
                        
               }
        }

	if(dirty)
	{
		DBCircleSyncArg arg;
        	arg.circleid = cinfo.circleid;
		dirty = false;
		cinfo.tasksavetime = Timer::GetTime();
		arg.info = cinfo;
		DBCircleSync* rpc = (DBCircleSync*) Rpc::Call( RPC_DBCIRCLESYNC,arg);
		rpc->isbatch = isbatch;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		updatetime = Timer::GetTime();
		LOG_TRACE("CircleInfo::Update :sync circle=%d", cinfo.circleid);
		return true;
	}

	updatetime = Timer::GetTime();
	return false;
}
/*
void CircleInfo::Broadcast(Protocol& data)
{
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	access = true;
        for(CircleRoleContainer::iterator it=online_list.begin(),ie=online_list.end();it!=ie;++it)
        {
                data.localsid = it->localsid;
                dsm->Send(it->linksid,data);
        }
}
*/
void CircleInfo::AddOnline(const GCircleMemberDB& crole,int index, int gameid,int linksid,int localsid )
{
	if(gameid<=0)
	{
		LOG_TRACE("CircleInfo::AddOnline ,gameid invalid: roleid=%d gameid=%d ", crole.roleid, gameid);
		return;
	}
	
	CircleMember mem(crole);
	mem.SetOnline(index,gameid,linksid,localsid);
/*
	online_list.push_back(mem);
*/
	CircleRoleContainer::iterator it=online_list.find(crole.roleid);
	if(it!=online_list.end())
	{
		it->second=mem;
	}
	else
	{
		IncOnline();
		online_list.insert(pair<unsigned int,CircleMember>(crole.roleid,mem));
	}

//	LOG_TRACE("CircleInfo::AddOnline: index=%d gameid= %d ", online_list.back().index, online_list.back().gameid);
	return;
	
	
}

void CircleInfo::RemoveOnline(unsigned int roleid)
{
/*
	CircleRoleContainer& list = online_list;
	CircleRoleContainer::iterator it= FindOnlineRole(roleid);
	if(it!=list.end())
	{
//		LOG_TRACE("CircleInfo::RemoveOnline: index=%d gameid= %d linksid=%d, localsid=%d", it->index, \
//				it->gameid, it->linksid, it->localsid);

				DecOnline();
		list.erase(it);
	}
*/
	if(online_list.erase(roleid)==1)
		DecOnline();
	else
		LOG_TRACE("CircleInfo::RemoveOnline,role is not online: circleid=%d, roleid=%d",cinfo.circleid , roleid);
	
}



bool CircleInfo::IsSilent()
{
//	LOG_TRACE("CircleManager: issilent circleid= %d online=%d, access=%d", cinfo.circleid, online, access);
	return !online && !access;
}

void CircleInfo::SendMessage(unsigned int msgid, int data1, int data2)
{
	GCircleChat msg;
	msg.src = msgid;
	msg.channel = GP_CHAT_SYSTEM;
	Marshal::OctetsStream data;
	data<<(int)data1<<(int)data2;
	msg.msg = data;
	for(CircleRoleContainer::iterator it=online_list.begin(),ie=online_list.end();it!=ie;++it)
	{
		msg.localsid = it->second.localsid;
		GDeliveryServer::GetInstance()->Send(it->second.linksid, msg);
	}
}
int CircleInfo::GetDefaultTaskpoint()
{
	int grad_count=0;
	for(std::vector<GCircleMemberDB>::iterator it=cinfo.member.begin(),ie=cinfo.member.end();it!=ie;++it)
	{
		if(it->graduate_time>0)
		{	
			grad_count++;
		}
	}

	return PER_ROLE_POINT*grad_count;
}
const static unsigned int CirclePointThreshold[9]={500,450,400,350,300,250,200,150,100};
bool CircleInfo::CheckPointThreshold(unsigned int delta)
{
	unsigned int threshold = CirclePointThreshold[cinfo.circlegrade-1];
	if(cinfo.taskpoint<threshold  && (cinfo.taskpoint+delta)>=threshold)
		return true;
	else
		return false;
}
////////////////////////////////// CircleManager ///////////////////////////////////
int CircleManager::zone_off;
bool CircleManager::circle_open=false;

CircleManager::~CircleManager()
{
	for(CircleMap::iterator it=circlemap.begin(),ie=circlemap.end();it!=ie;++it)
		delete it->second;
	circlemap.clear();
}

bool CircleManager::Initialize()
{
	IntervalTimer::Attach(this,500000/IntervalTimer::Resolution());
	return true;
}

CircleInfo * CircleManager::Find(unsigned int circleid)
{
	if (circleid == 0)
		return NULL;
	CircleMap::iterator it = circlemap.find(circleid);
	if (it != circlemap.end())
		return it->second;
	return NULL;
}
void CircleManager::ResetTaskpoint()
{
	int now=Timer::GetTime();
	Log::formatlog("CircleManager","ResetTaskpoint, time=%d",now%86400 - zone_off);
	for(CircleMap::iterator it = circlemap.begin(),ie=circlemap.end();it!=ie;++it)
	{
		unsigned int i = it->second->GetDefaultTaskpoint();
		if(i != it->second->cinfo.taskpoint)
		{	
			it->second->cinfo.taskpoint = i;
			it->second->dirty = true;
		}
	}
	return;
}
bool CircleManager::AddPoint(unsigned int circleid, unsigned int delta)
{
	CircleMap::iterator it = circlemap.find(circleid);
	if(it==circlemap.end())
	{
		Log::formatlog("CircleManager","Addpoint, circle not found,circleid=%d",circleid);
		return false;
	}
	if(it->second->CheckPointThreshold(delta))
	{
		CircleNotifyLink msg;
		msg.msgtype = CIRCLE_POINT_OVER_THRESHOLD;
		msg.roleid = 0;
		msg.circleid = circleid;
		it->second->Broadcast(msg);
		DEBUG_PRINT("CircleManager::Addpoint, point over threshold,circleid=%d,point=%d",circleid,\
				it->second->cinfo.taskpoint);
	}
	it->second->cinfo.taskpoint += delta;
	it->second->dirty = true;
	DEBUG_PRINT("CircleManager::Addpoint, delta=%d,point=%d",delta,it->second->cinfo.taskpoint);
	return true;
}
bool CircleManager::GetPoint(unsigned int circleid,unsigned int& circlepoint) // not used
{
	CircleMap::iterator it = circlemap.find(circleid);
	if(it==circlemap.end())
	{
		//log
		return false;
	}
	circlepoint = it->second->cinfo.taskpoint;
	//it->second->cinfo.taskpoint=0;
	//it->second->dirty = true;
	return true;
}
bool CircleManager::GetGradBonus(unsigned int circleid, unsigned int roleid, int64_t & bonus, bool clear)
{
	CircleMap::iterator it = circlemap.find(circleid);
	if(it==circlemap.end())
	{
		LOG_TRACE("CircleManager::GetGradBonus circle not found, circleid= %d roleid=%d", circleid, roleid);
		return false;
	}
	std::vector<GCircleMemberDB>::iterator itm = it->second->FindMember(roleid);
	if(itm == it->second->cinfo.member.end())
	{
		Log::log( LOG_ERR,"CircleManager::GetGradBonus role not in circle, circleid= %d roleid=%d", circleid, roleid);
		return false;
	}
	DEBUG_PRINT("CircleManager::GetGradBonus, circleid= %d roleid=%d,bonus_count=%d", circleid, roleid,itm->grad_bonus_count);
	if(itm->grad_bonus_count == 0)
		return false;
	bonus = CalcGradBonus(itm->grad_bonus_count,it->second->cinfo.circlegrade);
	if(clear==true)
	{
		itm->grad_bonus_count = 0;
		it->second->dirty = true;
	}
	
	return true;
}
bool CircleManager::Update()
{
	CircleMap::iterator it = circlemap.upper_bound(cursor);
	if(it==circlemap.end())
	{
		cursor = 0;
		return true;
	}
	cursor = it->second->cinfo.circleid;
	
	time_t now = Timer::GetTime();
	if(now - it->second->updatetime > UPDATE_INTERVAL)
	{
		it->second->Update();
		if(it->second->IsSilent())
		{
			delete it->second;
			circlemap.erase(it);
			DEBUG_PRINT("CircleManager: remove timeout circle, circleid=%d, mapsize=%d",cursor, circlemap.size());
			return true;
		}
		it->second->access = false;
	}
	return true;
}

void CircleManager::OnLoad(GCircleDB& circledata,unsigned int roleid)
{
//	DEBUG_PRINT("CircleManager::OnLoad:roleid=%d ,circleid=%d", roleid,circledata.circleid);
	CircleMap::iterator ic = circlemap.find(circledata.circleid);
	if (ic == circlemap.end())
	{
		CircleInfo * pcircle = new CircleInfo(circledata);
		pair<CircleMap::iterator,bool> tmp = circlemap.insert(std::make_pair(circledata.circleid, pcircle));
		ic = tmp.first;
		pcircle->access = true;
	}
	else		
		DEBUG_PRINT("circle found while load,circleid=%d",circledata.circleid);

	int now = Timer::GetTime();
	int savetime = ic->second->cinfo.tasksavetime;
//	if ( (now - savetime) >86400 || (now%86400) < (savetime%86400)) //day change
	if( (now+zone_off)/86400 != (savetime+zone_off)/86400 )
	{
		ic->second->cinfo.taskpoint=ic->second->GetDefaultTaskpoint();
		ic->second->dirty = true;
	}

	std::vector<GCircleMemberDB>::iterator it = ic->second->FindMember(roleid);
	std::vector<GCircleMemberDB> & membervec = ic->second->cinfo.member;
	if(it == membervec.end())
	{
		Log::log( LOG_ERR,"role not in circle while load,circleid=%d,roleid=%d",circledata.circleid,roleid);
		return;
	}
	int index = ic->second->MemberIndex(it);
		
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);//pinfo->ingame is still false
        if(pinfo)
        {
                ic->second->AddOnline(*it,index,pinfo->gameid,pinfo->linksid,pinfo->localsid);
	}
	else
		Log::log( LOG_ERR,"role not found while load,circleid=%d,roleid=%d",circledata.circleid,roleid);
	return;
}

void CircleManager::OnAsyncData(CircleAsyncData* arg, CircleAsyncData* res)
{
	if(IsCircleOpen()==false)
		return;
	TaskCircleGroupAsyncData request;
	request.Unmarshal(arg->data.begin(), arg->data.size());
	DEBUG_PRINT("CircleAsyncData: circleid=%d", request.GetData()->nCircleGroupId);

	CircleMap::iterator ic = circlemap.find(request.GetData()->nCircleGroupId);
	if (ic == circlemap.end())
		return;
	int grade = ic->second->cinfo.circlegrade;
	
	TaskCircleGroupAsyncData response(
		ic->second->cinfo.circleid,
		request.GetData()->nTask,
		GradeMinLv(grade), //minlevel
		GradeMaxLv(grade), //maxlevel
		ic->second->cinfo.taskpoint,
		ic->second->online,
		ic->second->cinfo.member.size()	
	);
	res->roleid = arg->roleid;
	res->result = 0;
	res->data.replace(response.GetBuffer(), response.GetSize());
}

void CircleManager::OnLevelUp(int roleid, int circleid, unsigned int level, int reborn_cnt)
{
	if(IsCircleOpen()==false)
		return;
	if(level >160)
	{
		Log::log( LOG_ERR,"CircleManager::OnLevelUp: roleid=%d, level=%d ,level error", roleid,level);
		return;
	}	
	if( reborn_cnt>0 )
	{
		if(circleid != 0)//leave last grade
		{
			InitJoinCircle(NO_GRADE,circleid,roleid,LEAVE_CIRCLE);
		}
		else
			return;	
	}
	if(circleid == 0 )// haven't join circle before
	{
		if(level >= CIRCLE_MIN_LEVEL)
	//	if(isNewGradeLevel(level) || OverLastGrade(level))
			InitJoinCircle(GetGrade(level),0,roleid,JOIN_CIRCLE);
		return;
	}
	
	CircleMap::iterator ic = circlemap.find(circleid);
	if (ic == circlemap.end())
	{
		DEBUG_PRINT("CircleManager::OnLevelUp: roleid=%d, circleid=%d ,circle not in cache", roleid,circleid);
		return;
	}
	if(GetGrade(level)> ic->second->cinfo.circlegrade)
	{
		InitJoinCircle(GetGrade(level),circleid,roleid,LEAVE_AND_JOIN_CIRCLE);
		return;
	}

	//if(ic->second->cinfo.master_rid==0 && isCircleMasterLevel(level))
	//	InitAppoint(roleid,circleid,CIRCLE_MASTER);
	return;
}

void CircleManager::InitAppoint(unsigned int roleid,unsigned int circleid,CircleTitle title)
{
	DEBUG_PRINT("CircleManager::InitAppoint: roleid=%d, circleid=%d ", roleid,circleid);
        CircleMap::iterator ic = circlemap.find(circleid);
	if (ic == circlemap.end())
	{
		Log::formatlog("CircleManager","InitAppoint: roleid=%d, circleid=%d ,circle not in cache", roleid,circleid);
		return;
	}
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
	if(pinfo == NULL || pinfo->circleinfo_valid == false)
	{
		Log::formatlog("CircleManager","InitAppoint: roleid=%d,role circle history not in cache", roleid);
		return;
	}	
	if(title==CIRCLE_MASTER && ic->second->MasterCanChange())
	{
		DBCircleAppoint* rpc=(DBCircleAppoint*) Rpc::Call(RPC_DBCIRCLEAPPOINT, \
			DBCircleAppointArg(circleid,roleid,title));	
		rpc->roleid = roleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);	
	}
	return;
}

void CircleManager::OnAppoint(unsigned int roleid, unsigned int circleid, CircleTitle title, unsigned char titlemask)
{
	DEBUG_PRINT("CircleManager::OnAppoint: roleid=%d, circleid=%d ", roleid,circleid);
	CircleMap::iterator ic = circlemap.find(circleid);
        if(ic!=circlemap.end())
        	ic->second->OnAppoint(roleid, title);
       	else
		Log::log( LOG_ERR,"CircleManager::OnAppoint: roleid=%d, circleid=%d ,circle not in cache", roleid,circleid);
       	
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
        if(pinfo && pinfo->circleinfo_valid)
        {
		pinfo->SetCircleTitlemask(titlemask);

		//notify gs and client
		NotifyGSNewCircle(roleid,circleid,titlemask,pinfo->gameid);

		CircleInfo * pcircle = Find(circleid);
		if (NULL != pcircle)
		{
			CircleNotifyLink msg;
			msg.msgtype = CIRCLE_APPOINT_MASTER;
			msg.roleid = roleid;
			msg.circleid = circleid;
			pcircle->Broadcast(msg);
		}
	}
	else
		Log::log( LOG_ERR,"CircleManager::OnAppoint: roleid=%d,role circle history not in cache", roleid);
	return;
}
const static unsigned int BonusArray[9]={1652,21154,67514,178494,687917,1453541,2317861,6710749,17401379}; 
int64_t CircleManager::CalcGradBonus(unsigned char count,unsigned char classgrade)
{
	return BonusArray[classgrade-1]*count;
}

void CircleManager::InitJoinCircle(int newgrade, int oldcircleid, int roleid, Jointype type)
{
	DEBUG_PRINT("CircleManager::InitJoinCircle:roleid=%d ", roleid);
	GRoleInfo *info = RoleInfoCache::Instance().Get(roleid);	
	if(info == NULL)
	{
		Log::formatlog("CircleManager","InitJoinCircle:roleid=%d ,roleinfocache not found", roleid);
		return;
	}
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
	if(pinfo == NULL || pinfo->circleinfo_valid == false)
	{
		Log::formatlog("CircleManager","InitJoinCircle: roleid=%d,role circle history not in cache", roleid);
		return;
	}
	if(pinfo->circlejoin_processing == true)
	{
	//	LOG_TRACE("CircleManager::InitJoinCircle: roleid=%d,db processing, drop request", roleid);
		return;
	}
	pinfo->circlejoin_processing = true;

	DBCircleJoin* rpc=(DBCircleJoin*) Rpc::Call(RPC_DBCIRCLEJOIN, \
			DBCircleJoinArg(newgrade,oldcircleid,roleid,info->level,info->occupation,info->reborn_cnt,type));
	rpc->roleid = roleid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return;
}
void CircleManager::SyncRoleCircleHis(unsigned int roleid,Jointype type,GCircleHistory& circlehisnode,int gradtime)
{
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
	if(pinfo)
	{
           	if(!pinfo->circleinfo_valid)
        	{
                        DBCircleHisGet * rpc = (DBCircleHisGet*) Rpc::Call(RPC_DBCIRCLEHISGET, RoleId(roleid));
			GameDBClient::GetInstance()->SendProtocol(rpc);
			Log::formatlog("CircleManager","SyncRoleCircleHis:roleid=%u DBCircleHistory request", roleid);
                       	return;
               	}
		switch(type)
		{
//			case LEAVE_CIRCLE:
//				pinfo->SetCircleGradTime(gradtime);
//				break;
			case JOIN_CIRCLE:
				pinfo->AddCircleHis(circlehisnode);
				break;
			case LEAVE_CIRCLE:	
			case LEAVE_AND_JOIN_CIRCLE:
				pinfo->SetCircleGradTime(gradtime);
				pinfo->AddCircleHis(circlehisnode);
				break;
		}
	}
}

void CircleManager::OnLogin(unsigned int roleid,int circleid, int gameid,int linksid,int localsid)// ,unsigned int level)
{
	if(IsCircleOpen()==false)
		return;
	if (!circleid)
	{
	//	if(OverLastGrade(level))
	//	join circle
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if(pinfo)
			pinfo->circleinfo_valid=true;//no circle history
		return;
	}
	DEBUG_PRINT("CircleManager::OnLogin: roleid=%d,circle=%d,gameid=%d,linksid=%d,localsid=%d",
			roleid,circleid,gameid,linksid,localsid);
	DBCircleHisGet * rpc = (DBCircleHisGet*) Rpc::Call(RPC_DBCIRCLEHISGET, RoleId(roleid));
	GameDBClient::GetInstance()->SendProtocol(rpc);
	
	CircleMap::iterator ic = circlemap.find(circleid);
	if (ic == circlemap.end())
	{
		DEBUG_PRINT("CircleManager::OnLogin:circleid=%d not found", circleid);
		DBCircleGet * rpc = (DBCircleGet*) Rpc::Call(RPC_DBCIRCLEGET, CircleID(circleid));
		rpc->roleid = roleid;//save locally
//		rpc->circleid = circleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);	
		return;
	}

	std::vector<GCircleMemberDB> & membervec = ic->second->cinfo.member;
		
	std::vector<GCircleMemberDB>::iterator itm = ic->second->FindMember(roleid);
	if(itm!=membervec.end())
	{
		int index = ic->second->MemberIndex(itm);
		ic->second->AddOnline(*itm,index,gameid,linksid,localsid);
	}
}

void CircleManager::OnLogout(unsigned int roleid, int circleid, int level, int occupation)
{
	if(IsCircleOpen()==false)
		return;
	if(circleid == 0)
		return;
	DEBUG_PRINT("CircleManager::OnLogout: roleid=%d,circle=%d", roleid,circleid);
	CircleMap::iterator ic = circlemap.find(circleid);
        if(ic==circlemap.end())
	{
		Log::log( LOG_ERR,"CircleManager::OnLogout,circle not found: roleid=%d,circle=%d", roleid,circleid);
		return;
	}
	//sync db
	ic->second->SyncMember( roleid,level,occupation);	
	ic->second->RemoveOnline(roleid);
	return;
}
void CircleManager::NotifyClient(CircleMsgType type,unsigned int roleid, unsigned int circleid, unsigned int localsid,\
	       unsigned int linksid, unsigned int name, unsigned char grade)
{
//	DEBUG_PRINT("CircleManager::NotifyClient: roleid=%d,circle=%d", roleid,circleid);
	CircleNotifyLink msg(type,roleid,circleid,localsid,name,grade);
	GDeliveryServer::GetInstance()->Send(linksid,msg);
}
void CircleManager::NotifyGSNewCircle(unsigned int roleid, unsigned int circleid, unsigned char titlemask, int gsid)
{
	DEBUG_PRINT("CircleManager::NotifyGS: roleid=%d,circle=%d,gsid=%d", roleid,circleid,gsid);
	UpdateRoleCircle msg(roleid,circleid,titlemask);
	GProviderServer::GetInstance()->DispatchProtocol( gsid, msg );
}
void CircleManager::OnJoin(int circleid, const GCircleMemberDB& crole)
{
//	LOG_TRACE("CircleManager::OnJoin:circleid=%d ,roleid=%d", circleid,crole.roleid);
	CircleMap::iterator ic = circlemap.find(circleid);
	if (ic == circlemap.end())
	{
		DEBUG_PRINT("CircleManager::OnJoin:circleid=%d not found", circleid);
		DBCircleGet * rpc = (DBCircleGet*) Rpc::Call(RPC_DBCIRCLEGET, CircleID(circleid));
		rpc->roleid = crole.roleid;//save locally
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return;
	}
	std::vector<GCircleMemberDB> & membervec = ic->second->cinfo.member;	
//      	std::vector<GCircleMemberDB>::iterator itm = ic->second->FindMember(crole.roleid);
        int index;
//	if(itm==membervec.end())		       
//	{
		membervec.push_back(crole);
		index = membervec.size()-1;
//	}
//	else
//	{
//		index = ic->second->MemberIndex(itm);
//	}
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(crole.roleid);
	if(pinfo)
	{
		ic->second->AddOnline(crole,index,pinfo->gameid,pinfo->linksid,pinfo->localsid);
	}
	return;
}

void CircleManager::OnLeave(int circleid, unsigned int roleid, int time)
{
	DEBUG_PRINT("CircleManager::OnLeave:circleid=%d ,roleid=%d", circleid,roleid);
	CircleMap::iterator ic = circlemap.find(circleid);
        if(ic==circlemap.end())
	{
		Log::log( LOG_ERR,"CircleManager::OnLeave:circleid=%d not found", circleid);
		return;
	}
	std::vector<GCircleMemberDB> & membervec = ic->second->cinfo.member;
        std::vector<GCircleMemberDB>::iterator itm = ic->second->FindMember(roleid);
	if(itm!=membervec.end())
	{
		itm->graduate_time = time;//write to db already
		for(std::vector<GCircleMemberDB>::iterator it=membervec.begin(),ie=membervec.end();it!=ie;++it )
			it->grad_bonus_count++;
		ic->second->cinfo.taskpoint += CircleInfo::PER_ROLE_POINT;	
		ic->second->dirty = true;
	}
	
	ic->second->RemoveOnline(roleid);
	
        return;
}

void CircleManager::OnDeleteRole(unsigned int circleid, unsigned int roleid)
{
	if(IsCircleOpen()==false)
		return;
	DEBUG_PRINT("CircleManager::OnDeleteRole:circleid=%d ,roleid=%d", circleid,roleid);
	CircleMap::iterator ic = circlemap.find(circleid);
        if(ic==circlemap.end())
                return;

	std::vector<GCircleMemberDB> & membervec = ic->second->cinfo.member;
        std::vector<GCircleMemberDB>::iterator itm = ic->second->FindMember(roleid);
	int index=0;
	if(itm!=membervec.end())
	{
		ic->second->dirty = true;
  		index = ic->second->MemberIndex(itm);
		membervec.erase(itm);
		ic->second->RemoveOnline(roleid);
		if(roleid == ic->second->cinfo.master_rid)
			ic->second->cinfo.master_rid = 0;
		CircleRoleContainer& list = ic->second->online_list;
		for(CircleRoleContainer::iterator it=list.begin(),ie=list.end();it!=ie;++it)
		{
			if(it->second.index > index)
			{	
				it->second.index--;
			}

		}
	}
    
        return;
}

void CircleManager::GetFullInfo(unsigned int circleid, unsigned int roleid, CircleList_Re &list)
{
	DEBUG_PRINT("CircleManager::GetFullInfo circle=%d",circleid);
	CircleMap::iterator ic = circlemap.find(circleid);
	if (ic == circlemap.end())
		return;
	
	list.circleid = circleid;
	std::vector<CircleMemberInfo> member;
	member.reserve(ic->second->cinfo.member.size());

	std::vector< GCircleMemberDB >& membercache = ic->second->cinfo.member;	
	std::vector< GCircleMemberDB >::iterator it= membercache.begin(), ie = membercache.end();
	for(; it!=ie;++it)
	{
		member.push_back(CircleMemberInfo(it->roleid,it->name,it->level,it->reborn_cnt,it->occupation, \
				it->gender,it->jointime,it->graduate_time));
	}
	CircleRoleContainer & online_list = ic->second->online_list;
	for(CircleRoleContainer::iterator itl=online_list.begin(),iel=online_list.end();itl!=iel;++itl)
	{
		if(itl->second.index > -1 && itl->second.index < static_cast<int>(member.size()))
			member[itl->second.index].lineid = itl->second.gameid;
	//	else
	//		LOG_TRACE("set online member, circleid=%d, index=%d,gameid=%d", circleid, itl->index, itl->gameid);
	}
	list.members.swap(member);
	list.online_num = online_list.size();
	if(ic->second->online != list.online_num)
		LOG_TRACE("circle online num unsync, circleid=%d,online_list.size=%d,online=%d",circleid, list.online_num, ic->second->online);
	list.createtime = ic->second->cinfo.createtime;
	if(GetGradBonus(circleid,roleid,list.grad_bonus,false) == false)
		list.grad_bonus=0;
	list.circlepoint = ic->second->cinfo.taskpoint;
		
        PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
        if (pinfo && pinfo->circleinfo_valid)
	{
		list.history = pinfo->circlehistory;
	}	
	return;
}
void CircleManager::GetBaseInfo(IntVector& idlist, std::vector<GCircleBaseInfo> & infolist)
{
	CircleMap::iterator ic;
	for(IntVector::iterator it=idlist.begin(), ie=idlist.end(); it!=ie;++it)
	{
		ic = circlemap.find(*it);
		if (ic == circlemap.end())
			continue;
		GCircleDB &circle = ic->second->cinfo;
		infolist.push_back(GCircleBaseInfo(*it, circle.circlename,circle.circlegrade,circle.master_rid));
	}
//	LOG_TRACE("CircleManager::GetBaseInfo number=%d",infolist.size());
	return;
}

void CircleManager::OnRolenameChange(unsigned int circleid, int roleid, const Octets & newname)
{
	CircleMap::iterator it = circlemap.find(circleid);
	if (it == circlemap.end())
		return;
	std::vector<GCircleMemberDB>::iterator itm = it->second->FindMember(roleid);
	if (itm != it->second->cinfo.member.end())
	{
		itm->name = newname;
		it->second->dirty = true;//目前不必要 因为 DBCircleSync 不会覆盖name
	}
}
/*
bool CircleManager::IsMember(int circleid, unsigned int roleid)
{
	CircleMap::iterator ic = circlemap.find(circleid);
	if(ic==circlemap.end())
		return false;
	std::list<CircleRole>& roles = ic->second->list;
	std::list<CircleRole>::iterator it = roles.begin(), ie = roles.end();
	for (; it != ie; ++it)
	{
		if(it->role.rid==roleid)
			return true;
	}
	return false;
}*/

void CircleSaver::DoSave()
{
	if (!still_not_sync_all)
		return;
	CircleManager::CircleMap & circlemap = CircleManager::Instance()->circlemap;
	for(CircleManager::CircleMap::iterator it = circlemap.upper_bound(index);it != circlemap.end();++it)
	{
		bool isbatch = true;
		if(it->second->Update(isbatch))
		{
			index = it->first;
			return;
		}
	}
	still_not_sync_all = false;
}

};

