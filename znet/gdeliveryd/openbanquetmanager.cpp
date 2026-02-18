#include "openbanquetmanager.h"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "battlestart.hpp"
#include "gproviderserver.hpp"
#include "openbanquetjoin_re.hpp"
#include "battlemanager.h"
#include "sendbattleenter.hpp"
#include "playerchangegs.hpp"
#include "gopenbanquetclose.hpp"
#include "openbanquetdrag_re.hpp"
#include "crossbattlemanager.hpp"
#include "centraldeliveryserver.hpp"
#include "gopenbanquetroledel.hpp"
#include "openbanquetnotify_re.hpp"
#include "battleenter_re.hpp"

namespace GNET
{


// 场景是否满了
bool OpenBanquetInfo::IsRolesFull()
{
	//LOG_TRACE("wait_queue=%d,attack_map=%d,attack_wait=%d,defend_map=%d, defend_wait=%d,disconnect=%d", wait_queue.size(), attack_map.size(), attack_wait.size(), defend_map.size(), defend_wait.size(), disconnect_map.size());
	if(attack_map.size()+attack_wait.size() >= EOBI_MAX_HALF_ROLES_NUM && defend_map.size()+defend_wait.size() >= EOBI_MAX_HALF_ROLES_NUM)
	{
		return true;
	}
	return false;
}

// 战场人数
int OpenBanquetInfo::GetRolesCount()
{
	LOG_TRACE("OpenBanquetManager::GetRolesCount attack_map.size=%d, defend.size=%d", attack_map.size(), defend_map.size());
	return attack_map.size() + defend_map.size();
}

// 角色是否在战场中
bool OpenBanquetInfo::FindRole(int roleid, char& faction)
{
	TRolesMap::iterator attack_it = attack_map.find(roleid);
	if(attack_it != attack_map.end())
	{
		faction = EOBF_ATTACKER;
		return true;
	}

	TRolesMap::iterator defend_it = defend_map.find(roleid);
	if(defend_it != defend_map.end())
	{
		faction = EOBF_DEFENDER;
		return true;
	}

	return false;
	
}

// 设置断线保护时间
void OpenBanquetInfo::SetTimeOut(int roleid, char faction,  time_t time)
{
	SOpenBanquetRoleInfo sinfo;
	sinfo.roleid = roleid;
	sinfo.faction = faction;
	disconnect_map.insert(std::make_pair(time+EOBI_MAX_DISCONNET_TIME, sinfo));
	return;
}

void OpenBanquetInfo::RemoveTimeOut(int roleid)
{
	TDisMap::iterator it = disconnect_map.begin();
	
	//LOG_TRACE("OpenBanquetInfo::RemoveTimeOut, disconnect_map.size=%d", disconnect_map.size());
	while(it != disconnect_map.end())
	{
		if(it->second.roleid == roleid)
		{
			LOG_TRACE("OpenBanquetInfo::RemoveTimeOut disconnect_map erase roleid=%d, disconnect_map.size=%d", roleid, disconnect_map.size());
			disconnect_map.erase(it++);
			return;
		}
		else
		{
			++it;
		}
	}
	
	//LOG_TRACE("OpenBanquetInfo::RemoveTimeOut end, it->first=%d, time=%d, roleid=%d", it->first, time, it->second.roleid);
}

void OpenBanquetInfo::UpdateTimeOut(time_t time)
{
	TDisMap::iterator it = disconnect_map.begin();
	
	//LOG_TRACE("OpenBanquetInfo::UpdateTimeOut, disconnect_map.size=%d", disconnect_map.size());
	while(it != disconnect_map.end())
	{
		LOG_TRACE("OpenBanquetInfo::UpdateTimeOut start, it->first=%d, time=%d, disconnect_map.size=%d", it->first, time, disconnect_map.size());
		if(it->first > time)
		{
			break;
		}
		
		SendRoleDelMsg(it->second.roleid, it->second.faction);
		LOG_TRACE("OpenBanquetInfo::UpdateTimeOut end, it->first=%d, time=%d, roleid=%d", it->first, time, it->second.roleid);
		BattleMapClear(it->second.roleid);	
		disconnect_map.erase(it++);
	}
}

// 得到空余的相关信息 free_camp:空闲的阵营,roles_num:预进入人数，return: 是否可以进入
bool OpenBanquetInfo::CheckFreeInfo(int& free_faction, int roles_num)
{
	int free_num = 0;
	if(attack_map.size() >= defend_map.size())
	{
		// 如果攻击方多，则返回防守方
		free_faction = EOBF_DEFENDER;
		free_num = EOBI_MAX_HALF_ROLES_NUM - defend_map.size();
	}
	else
	{
		free_faction = EOBF_ATTACKER;
		free_num = EOBI_MAX_HALF_ROLES_NUM - attack_map.size();
	}

	LOG_TRACE("OpenBanquetInfo::CheckFreeInfo, attack_map=%d,defend_map=%d,free_faction=%d,free_num=%d,roles_num=%d", attack_map.size(),defend_map.size(),free_faction,free_num,roles_num);	
	if(free_num >= roles_num)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 放入排队队列
bool OpenBanquetInfo::PushWaitQueue(TRolesVec& roles, int gsid)
{
	TRolesVec _troles;
	TRolesVec::iterator it;
	for(it=roles.begin(); it!=roles.end(); ++it)
	{
		int roleid = *it;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if ( pinfo && pinfo->user && pinfo->ingame )
		{
			int role_reborn;
			GRoleInfo *prole = RoleInfoCache::Instance().Get(roleid);
			if (prole == NULL)
			{
				role_reborn = 0;
			}
			else
			{
				role_reborn = prole->reborn_cnt;
			}

			if(!OpenBanquetManager::GetInstance()->CheckJoinPolicy(pinfo->level, role_reborn))
			{
				OpenBanquetManager::GetInstance()->SendNotifyMsg(roleid, ERR_OPB_LEVEL_LIMIT);
				LOG_TRACE("OpenBanquetInfo::PutWaitQueue roleid=%d CheckJoinPolicy fail", roleid);
				continue;	
			}

			if(!OpenBanquetManager::GetInstance()->IsPerdayLimit(roleid, 1))
			{
				OpenBanquetManager::GetInstance()->SendNotifyMsg(roleid, ERR_OPB_ENTER_MAXTIMES);
				LOG_TRACE("OpenBanquetInfo::PutWaitQueue roleid=%d IsPerdayLimit fail", roleid);
				continue;
			}

			SOpenBanquetRoleInfo* pRoleInfo = GetRoleInfo(roleid);
			if(NULL != pRoleInfo)
			{
   				OpenBanquetManager::GetInstance()->SendNotifyMsg(roleid, ERR_OPB_HAS_QUEUE); 
				LOG_TRACE("OpenBanquetInfo::PutWaitQueue roleid=%d has join", roleid);
				continue;
			}

			//LOG_TRACE("OpenBanquetInfo::PutWaitQueue roleid=%d, pinfo->user.src_zoneid=%d", roleid, pinfo->user->src_zoneid);
			SOpenBanquetRoleInfo roleinfo;
			roleinfo.old_gs = gsid;
			roleinfo.name = pinfo->name;
			roleinfo.src_zoneid = pinfo->user->src_zoneid;
			roleinfo.status = EOBRS_QUEUE;

			InsertRoleInfo(roleid, roleinfo);
			CrossBattleManager::GetInstance()->JoinTeam(OPENBANQUET_BATTLE, OPENBANQUET_BATTLE, id.gs_id, id.map_tag, roleid, end_time, 0, pinfo->user->src_zoneid);
		
			OpenBanquetManager::GetInstance()->InsertRolesServer(roleid, id);
			_troles.push_back(roleid);
		}

	}
	if(_troles.size() != 0)
	{
		wait_queue.push_back(_troles);	
		// 告诉客户端排队
		SendJoinMsg(_troles, S2C_OPENBANQUET_QUEUE);

		//LOG_TRACE("OpenBanquetInfo::PutWaitQueue wait_queue.size=%d, roles.size=%d", wait_queue.size(), _troles.size()); 
	}

	//if(IsRolesFull())
	//{
		// 告诉客户端排队
		//SendJoinMsg(_troles, id, S2C_OPENBANQUET_QUEUE);
	//}
	
	return true;

}

// pop排队队列
bool OpenBanquetInfo::PopWaitQueue()
{
	//LOG_TRACE("OpenBanquetInfo::PopWaitQueue wait_queue.size=%d", wait_queue.size()); 
	
	if(!wait_queue.empty())
	{
		wait_queue.pop_front();	
		LOG_TRACE("OpenBanquetInfo::PopWaitQueue wait_queue.size=%d", wait_queue.size()); 
	}
	
	return true;
}


// 玩家放入等待进入队列
bool OpenBanquetInfo::PutWaitMap(int free_faction, TRolesVec& roles, time_t now)
{
	LOG_TRACE("OpenBanquetInfo::PutWaitMap start free_faction=%d, roles.size=%d, attack_wait=%d, defend_wait=%d", free_faction, roles.size(), attack_wait.size(), defend_wait.size());

	if(free_faction == EOBF_ATTACKER)
	{
		TRolesVec::iterator it;
		for(it=roles.begin(); it!=roles.end();++it)
		{
			int roleid = *it;
			attack_wait.insert(std::make_pair(roleid, now + EOBI_MAX_WAITING));
			SetRoleStatus(roleid, EOBRS_WAITING);
			SendJoinWaitMsg(roles, free_faction);
		}
	}

	if(free_faction == EOBF_DEFENDER)
	{
		TRolesVec::iterator it;
		for(it=roles.begin(); it!=roles.end();++it)
		{
			int roleid = *it;
			defend_wait.insert(std::make_pair(roleid, now + EOBI_MAX_WAITING));
			SetRoleStatus(roleid, EOBRS_WAITING);
			SendJoinWaitMsg(roles, free_faction);
		}

	}
	return true;
}

// 服务器主动清除等待进入倒计时到时间的
void OpenBanquetInfo::WaitMapClear(time_t now)
{
	//LOG_TRACE("OpenBanquetManager::WaitMapClear(now) start attack_wait=%d, defend_wait=%d", attack_wait.size(), defend_wait.size());

	TTimeMap::iterator attack_it;
	for(attack_it=attack_wait.begin(); attack_it!=attack_wait.end();)
	{
		time_t end_time = attack_it->second;
		
		if(end_time < now)
		{
			LOG_TRACE("OpenBanquetInfo::WaitMapClear attack_wait erase attack_wait=%d, defend_wait=%d, end_time=%d, now=%d", attack_wait.size(), defend_wait.size(), end_time, now);
			ClearRoleInfo(attack_it->first);
			EraseCrossBattle(attack_it->first);
			OpenBanquetManager::GetInstance()->ClearRolesServer(attack_it->first);
			attack_wait.erase(attack_it++);
		}
		else
		{
			attack_it++;
		}
	}

	TTimeMap::iterator defend_it;
	for(defend_it=defend_wait.begin(); defend_it!=defend_wait.end();)
	{
		time_t end_time = defend_it->second;

		if(end_time < now)
		{
			LOG_TRACE("OpenBanquetInfo::WaitMapClear defend_wait erase attack_wait=%d, defend_wait=%d, end_time=%d, now=%d", attack_wait.size(), defend_wait.size(), end_time, now);
			ClearRoleInfo(defend_it->first);
			EraseCrossBattle(defend_it->first);
			OpenBanquetManager::GetInstance()->ClearRolesServer(defend_it->first);
			defend_wait.erase(defend_it++);
		}
		else
		{
			defend_it++;
		}
	}
	//LOG_TRACE("OpenBanquetInfo::WaitMapClear(now) end attack_wait=%d, defend_wait=%d", attack_wait.size(), defend_wait.size());

}

void OpenBanquetInfo::EraseCrossBattle(int roleid)
{
	// 删除原服
	BattleEnter_Re res;
	res.roleid = roleid;
	res.battle_type = OPENBANQUET_BATTLE;
	res.retcode = ERR_BATTLE_ENTERCROSSBATTLE;
	int zoneid = CrossBattleManager::GetInstance()->GetSrcZoneID(roleid);
	CentralDeliveryServer::GetInstance()->DispatchProtocol(zoneid, res);
	
	// 删除跨服
	CrossBattleManager::GetInstance()->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
}

// 战场内玩家删除
void OpenBanquetInfo::BattleMapClear(int roleid)
{
	QueueCancel(roleid, 0);// 进入取消
	QueueCancel(roleid, 1);// 排队取消
	attack_map.erase(roleid);	
	defend_map.erase(roleid);
	ClearRoleInfo(roleid);

	EraseCrossBattle(roleid);
	OpenBanquetManager::GetInstance()->ClearRolesServer(roleid);
}

// 离开战场
void OpenBanquetInfo::Leave(int roleid)
{
	//LOG_TRACE("OpenBanquetInfo::Leave start roleid=%d, attack_map=%d, defend_map=%d", roleid, attack_map.size(), defend_map.size());

	SendChangeGsMsg(roleid);
	BattleMapClear(roleid);

	LOG_TRACE("OpenBanquetInfo::Leave role=%d, attack_map=%d, defend_map=%d", roleid, attack_map.size(), defend_map.size());
}

// 得到加入的阵营
bool OpenBanquetInfo::GetEnterFaction(int roleid, char& tfaction)
{
	bool ret = false;
	TTimeMap::iterator attack_it = attack_wait.find(roleid);
	if(attack_it!=attack_wait.end())
	{
		tfaction = EOBF_ATTACKER;
		ret = true;
	}
	
	TTimeMap::iterator defend_it = defend_wait.find(roleid);
	if(defend_it!=defend_wait.end())
	{
		tfaction = EOBF_DEFENDER;
		ret = true;
	}
	LOG_TRACE("OpenBanquetInfo::GetEnterFaction end attack_map=%d, defend_map=%d, attack_wait=%d, defend_wait=%d, faction=%d", attack_map.size(), defend_map.size(), attack_wait.size(), defend_wait.size(), tfaction);

	return ret;
}

// 进入战场
bool OpenBanquetInfo::Enter(int roleid)
{
	bool ret = false;
	TTimeMap::iterator attack_it = attack_wait.find(roleid);
	if(attack_it!=attack_wait.end())
	{
		attack_wait.erase(attack_it);
		attack_map.insert(roleid);
		SetRoleStatus(roleid, EOBRS_FIGHTING);
		ret = true;

		LOG_TRACE("OpenBanquetInfo::Enter roleid=%d, attack_wait=%d, defend_wait=%d, attack_map=%d, defend_map=%d", roleid, attack_wait.size(), defend_wait.size(), attack_map.size(), defend_map.size());
	}
	
	TTimeMap::iterator defend_it = defend_wait.find(roleid);
	if(defend_it!=defend_wait.end())
	{
		defend_wait.erase(defend_it);
		defend_map.insert(roleid);
		SetRoleStatus(roleid, EOBRS_FIGHTING);
		ret = true;

		LOG_TRACE("OpenBanquetInfo::Enter roleid=%d, attack_wait=%d, defend_wait=%d, attack_map=%d, defend_map=%d", roleid, attack_wait.size(), defend_wait.size(), attack_map.size(), defend_map.size());
	}
	
	return ret;
}

void OpenBanquetInfo::QueueCancel(int roleid, char isqueue)
{
	LOG_TRACE("OpenBanquetInfo::QueueCancel start roleid=%d, isqueue=%d, wait_queue=%d", roleid, isqueue, wait_queue.size());
	
	OpenBanquetManager::GetInstance()->ClearRolesServer(roleid);

	if(isqueue == 0)// 进入取消
	{
		attack_wait.erase(roleid);
		defend_wait.erase(roleid);
		ClearRoleInfo(roleid);
	}

	if(isqueue == 1) // 排队取消
	{
		TWaitQueue::iterator it;
		for(it=wait_queue.begin(); it!=wait_queue.end(); ++it)
		{

			TRolesVec& cancel_roles = *it;

			TRolesVec::iterator _it;
			for(_it=cancel_roles.begin(); _it!=cancel_roles.end(); ++_it)
			{
				if(*_it == roleid)
				{	
					cancel_roles.erase(_it);
					ClearRoleInfo(roleid);

					if(cancel_roles.size() == 0)
					{
						wait_queue.erase(it);
						LOG_TRACE("OpenBanquetInfo::QueueCancel erase roleid=%d, wait_queue=%d", roleid, wait_queue.size());
					}
					return;
				}
			}
		}
	}

}

// 排队人数
int OpenBanquetInfo::GetQueueCount()
{
	int count = 0;
	TWaitQueue::iterator it;
	for(it=wait_queue.begin(); it!=wait_queue.end(); ++it)
	{
		TRolesVec& roles = *it;
		count += roles.size();
	}
	return count;
}

int OpenBanquetInfo::GetQueuePos(int roleid)
{
	int count = 0;
	TWaitQueue::iterator it;
	for(it=wait_queue.begin(); it!=wait_queue.end(); ++it)
	{
		TRolesVec& roles = *it;
		TRolesVec::iterator _it;
		for(_it=roles.begin(); _it!=roles.end(); ++_it)
		{
			if(*_it != roleid)
			{	
				++count;
			}
			else
			{
				return ++count;
			}
		}

	}
	return count;
}

bool OpenBanquetInfo::GetRoleField(std::vector<GOpenBanquetRoleInfo>& attack_field, std::vector<GOpenBanquetRoleInfo>& defend_field)
{
	TRolesMap::iterator attack_it = attack_map.begin();
	for(; attack_it!=attack_map.end(); ++attack_it)
	{
		int roleid = *attack_it;
		SOpenBanquetRoleInfo* pRoleInfo = GetRoleInfo(roleid);
		if(NULL == pRoleInfo)
		{
			LOG_TRACE("OpenBanquetInfo::GetRoleField attack_map=%d, roleid=%d not find roleinfo", attack_map.size(), roleid);
			continue;
		}

		GOpenBanquetRoleInfo ginfo;
		ginfo.roleid = roleid;
		ginfo.name = pRoleInfo->name;
		attack_field.push_back(ginfo);
		//LOG_TRACE("OpenBanquetInfo::GetRoleField  attack_map=%d, roleid=%d, name.size=%d", attack_map.size(), roleid, ginfo.name.size());
	}

	TRolesMap::iterator defend_it = defend_map.begin();
	for(; defend_it!=defend_map.end(); ++defend_it)
	{
		int roleid = *defend_it;
		SOpenBanquetRoleInfo* pRoleInfo = GetRoleInfo(roleid);
		if(NULL == pRoleInfo)
		{
			LOG_TRACE("OpenBanquetInfo::GetRoleField  defend_map=%d roleid=%d not find roleinfo", defend_map.size(), roleid);
			continue;
		}

		GOpenBanquetRoleInfo ginfo;
		ginfo.roleid = roleid;
		ginfo.name = pRoleInfo->name;
		attack_field.push_back(ginfo);
		//LOG_TRACE("OpenBanquetInfo::GetRoleField  defend_map=%d, roleid=%d, name.size=%d", defend_map.size(), roleid, ginfo.name.size());

	}

	return true;
}

// 发送进入排队队列消息
void OpenBanquetInfo::SendJoinWaitMsg(TRolesVec& roles, int free_faction)
{
	TRolesVec::iterator it;
	for(it=roles.begin(); it!=roles.end(); ++it)
	{
		int roleid = *it;
	
		OpenBanquetDrag_Re drag_msg;
		CrossBattleManager *manager = CrossBattleManager::GetInstance();
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL == pinfo )// 跨服战场开启但是报名者不在跨服，通知原服
		{
			drag_msg.roleid = roleid;
			drag_msg.gs_id = id.gs_id;
			drag_msg.map_id = id.map_tag;
			drag_msg.localsid = 0;
			drag_msg.iscross = true;

  			int src_zoneid = manager->GetSrcZoneID(roleid);
			LOG_TRACE("OpenBanquetInfo::SendJoinWaitMsg roleid=%d, wait_queue=%d, src_zoneid=%d", roleid, wait_queue.size(), src_zoneid);

  			if(src_zoneid != 0)
			{
				manager->InsertFightingTag(id.gs_id, roleid, id.map_tag, free_faction, end_time, OPENBANQUET_BATTLE);
				manager->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
				CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, drag_msg);
			}
			
		}
		else if(pinfo != NULL && pinfo->ingame) // 报名者在报名的GS上
		{
			drag_msg.roleid = roleid;
			drag_msg.gs_id = id.gs_id;
			drag_msg.map_id = id.map_tag;
			drag_msg.localsid = pinfo->localsid;
			drag_msg.iscross = false;

			LOG_TRACE("OpenBanquetInfo::SendJoinWaitMsg send to client, roleid=%d, wait_queue=%d, localsid=%d", roleid, wait_queue.size(), pinfo->localsid);
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, drag_msg);
		}
		else
		{
			LOG_TRACE("OpenBanquetInfo::SendJoinWaitMsg roleid=%d is not in game", roleid);
			manager->EraseCrossBattle(roleid, OPENBANQUET_BATTLE);
		}
	}
}

// 发送排队返回消息
void OpenBanquetInfo::SendJoinMsg(int roleid, int jointype)
{
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if ( NULL == pinfo )
	{
		LOG_TRACE("OpenBanquetInfo::SendJoinMsg roleid=%d, type=%d is not online", roleid, jointype);
		return;
	}
	
	if(!OpenBanquetManager::GetInstance()->IsPerdayLimit(roleid, 1))
	{
		LOG_TRACE("OpenBanquetInfo::SendJoinMsg roleid=%d, type=%d IsPerdayLimit is false", roleid, jointype);
		return;
	}

	LOG_TRACE("OpenBanquetInfo::SendJoinMsg send to client, roleid=%d, type=%d, wait_queue=%d, localsid=%d", roleid, jointype, wait_queue.size(), pinfo->localsid);
	GDeliveryServer::GetInstance()->Send(pinfo->linksid, OpenBanquetJoin_Re(roleid, jointype, id.gs_id, id.map_tag, GetQueuePos(roleid), pinfo->localsid));

}

// 发送排队返回消息
void OpenBanquetInfo::SendJoinMsg(TRolesVec& roles, int jointype)
{
	TRolesVec::iterator it;
	for(it=roles.begin(); it!=roles.end(); ++it)
	{
		int roleid = *it;
		SendJoinMsg(roleid, jointype);	
	}
}

//返回报名点
void OpenBanquetInfo::SendChangeGsMsg(int roleid)
{
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
	if ( NULL == pinfo )
	{
		LOG_TRACE("OpenBanquetInfo::SendChangeGsMsg roleid=%d is not online", roleid);
		return;
	}

	SOpenBanquetRoleInfo* pRoleInfo = GetRoleInfo(roleid);
	if(NULL == pRoleInfo)
	{
		LOG_TRACE("OpenBanquetInfo::SendChangeGsMsg roleid=%d not find roleinfo", roleid);
		return;
	}

	int old_gs = pRoleInfo->old_gs;
	int reason = ENTER_OPENBANQUT_REASON;
	GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, old_gs, 615, reason, -398, 175, 138));

}

// gs流水席角色删除消息
void OpenBanquetInfo::SendRoleDelMsg(int roleid, char faction)
{
	LOG_TRACE("OpenBanquetInfo::SendRoleDelMsg send to gs GOpenBanquetRoleDel, roleid=%d, faction=%d, map_tag=%d", roleid, faction, id.map_tag);
	GProviderServer::GetInstance()->Send(sid, GOpenBanquetRoleDel(id.map_tag, roleid, faction));

}

bool OpenBanquetManager::CheckJoinPolicy(int role_level, int role_reborn)
{
	//LOG_TRACE("OpenBanquetManager::CheckJoinPolicy level=%d, reborn=%d", role_level, role_reborn);
	if (role_level < BATTLE_LVL_NEED || role_reborn < 1)	
		return false;
	return true;
}

void OpenBanquetManager::StartAnnounce()
{
	LOG_TRACE("OpenBanquetManager::StartAnnounce Start Announce");
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_TERRI_START;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

bool OpenBanquetManager::Initialize()
{
	struct tm dt;
	time_t now = GetTime();
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	t_base = mktime(&dt);
	//LOG_TRACE("OpenBanquetManager::Initialize  t_base %s, tbase %d", ctime(&t_base), t_base);

	srand(GetTime());
	UpdateTime();
	t_status_update = GetTime() + STATUS_UPDATE_INTERVAL;
	IntervalTimer::Attach( this, UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());

	//LOG_TRACE("OpenBanquetManager::Initialize  t_base %s, tbase %d", ctime(&t_base), t_base);
	return true;
}

bool OpenBanquetManager::RegisterServer(int sid, int gs_id, const std::vector<BattleFieldInfo>& info)
{
	LOG_TRACE( "OpenBanquetManager::RegisterServer sid=%d, gs_id=%d, info.size=%d", sid, gs_id, info.size());
	std::vector<BattleFieldInfo>::const_iterator it;
	for(it=info.begin(); it!=info.end();++it)
	{
		SOpenBanquetID uid;
		uid.gs_id = gs_id;
		uid.map_tag = it->tag;

		ServerMap::const_iterator _git = servers.find(uid);
		if (_git != servers.end())
		{
			servers.erase(uid);
			Log::log(LOG_ERR, "OpenBanquetManager::OpenBanquetServerRegister duplicate gs_id=%d,map_tag=%d", uid.gs_id, uid.map_tag);
		}
		Log::formatlog("OpenBanquetManager::RegisterServer", "gs_id=%d, map_tag=%d, map_id=%d", gs_id, it->tag, it->map_id);
		servers.insert(std::make_pair(uid, OpenBanquetInfo(uid, sid, *it)));
	}
	return true;
}

int  OpenBanquetManager::GetQueuePos(int roleid, int gs_id, int map_tag)
{
	OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(gs_id, map_tag);
	if(!pOpenBanquetInfo)
	{
		Log::log(LOG_ERR,"OpenBanquetManager::GetQueuePos, but pOpenBanquetTag is NULL! gs_id=%d, map_tag=%d", gs_id, map_tag);
		return 0;
	}
	
	return pOpenBanquetInfo->GetQueuePos(roleid);
}

bool OpenBanquetManager::GetRoleField(int roleid, int gs_id, int map_tag, int localsid, std::vector<GOpenBanquetRoleInfo>& attack_field, std::vector<GOpenBanquetRoleInfo>& defend_field)
{
	OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(gs_id, map_tag);
	if(!pOpenBanquetInfo)
	{
		Log::log(LOG_ERR,"OpenBanquetManager::GetRoleField, but pOpenBanquetTag is NULL! gs_id=%d, map_tag=%d", gs_id, map_tag);
		return false;
	}
	
	return pOpenBanquetInfo->GetRoleField(attack_field, defend_field);
}

time_t OpenBanquetManager::GetTime()
{
	time_t now = Timer::GetTime();
	return now + t_forged;
}

void OpenBanquetManager::SetForgedTime(time_t forge)
{               
	t_forged = forge;
	time_t now = GetTime();
	t_status_update = now + STATUS_UPDATE_INTERVAL;
} 

void OpenBanquetManager::SetStartTime(time_t start)
{
	t_start = start;
}

time_t OpenBanquetManager::UpdateTime()
{
	time_t now = GetTime();
	//time_t base = t_base;
	//LOG_TRACE("OpenBanquetManager::UpdateTime forget=%d now=%s, now=%d, t_base=%s, t_base=%d, now-t_base=%d", t_forged, ctime(&now), now, ctime(&t_base), t_base, now-t_base);
	if(now - t_base > 86400) // 一天一次
	{
		t_base += 86400;
		LOG_TRACE("OpenBanquetManager::UpdateTime now - t_base > 86400 t_base=%s, t_base=%d", ctime(&t_base), t_base);
	}		

	return now;
}

void OpenBanquetManager::UpdateStatus(time_t now)
{
	//LOG_TRACE("OpenBanquetManager::UpdateStatus exec");
	ServerMap::iterator it = servers.begin();
	for(; it != servers.end(); ++it)
	{
		if(now >= BattleBeginTime() && now < BattleEndTime())
		{
			//LOG_TRACE("OpenBanquetManager::UpdateStatus statue==open");
			it->second.status |= OBS_OPEN;
			it->second.status &=(~OBS_WAITING);
		}
		else
		{
			it->second.status &= (~OBS_OPEN);
		}

		/*
		time_t _tmp_endtime = BattleEndTime();
		time_t _tmp_checkclose = BattleCloseCheck();
		LOG_TRACE("OpenBanquetManager::UpdateStatus status=%x Delay now=%s,closetime=%s,checkclose=%s,now=%d,enttime=%d,checkclose=%d", 
					it->second.status, ctime(&now), ctime(&_tmp_endtime),ctime(&_tmp_checkclose),now,_tmp_endtime,_tmp_checkclose);
		*/
		if(!(it->second.status&OBS_CLOSE) && it->second.status&OBS_OPEN && now >= BattleCloseCheck())
		{
			if(it->second.GetRolesCount() >= CLOSE_CHECK_ROLE_SIZE)
			{
				// 战场延长
				t_close_check += BATTLE_DELEY_TIME;
				t_close += BATTLE_DELEY_TIME;
				
				time_t endtime = BattleEndTime();
				LOG_TRACE("OpenBanquetManager::UpdateStatus status Delay gs_id=%d, map_tag=%d, now=%d, now=%s, closetime=%d, closetime=%s, map_id=%d", 
					it->second.id.gs_id, it->second.id.map_tag, now, ctime(&now), BattleEndTime(), ctime(&endtime), it->second.map_id);
			}
			else
			{
				LOG_TRACE("OpenBanquetManager::UpdateStatus status change close gs_id=%d, map_tag=%d, time=%d, time=%s, map_id=%d",
					it->second.id.gs_id, it->second.id.map_tag, now, ctime(&now), it->second.map_id);
				it->second.status |= OBS_CLOSE;
			}
		}
		if(it->second.status&OBS_WAITING)
		{

		}
		else if(it->second.status&OBS_CANCEL)
		{
			LOG_TRACE("OpenBanquetManager::UpdateStatus Cancel gs_id=%d, map_tag=%d, time=%d, time=%s, map_id=%d", 
					it->second.id.gs_id, it->second.id.map_tag, now, ctime(&now), it->second.map_id);
			
			it->second.Reset();
			ReSet();

			it->second.status |= OBS_WAITING;
		}
		else if((it->second.status&OBS_CLOSE && now >=BattleEndTime()) || now >= BidFinalEndTime())
		{
			GOpenBanquetClose close(0, 0);
			close.gs_id = it->second.id.gs_id;
			close.map_id = it->second.id.map_tag;
			LOG_TRACE("OpenBanquetManager::UpdateStatus status is OBS_CLOSE gs_id=%d, map_tag=%d, end_time=%d, sid=%d",
					it->second.id.gs_id, close.map_id, it->second.sid);
			
			GProviderServer::GetInstance()->Send(it->second.sid, close);
			
			it->second.status |= OBS_CANCEL;
			it->second.status &= (~OBS_FIGHTING);
			Log::formatlog("OpenBanquetManager::UpdateStatus","gs_id=%d:map_tag=%d:time=%d:attcksize=%d:defendsize=%d:map_id=%d", 
					it->second.id.gs_id, it->second.id.map_tag, now, it->second.attack_map.size(), it->second.defend_map.size(), it->second.map_id);

		}
		else if(it->second.status&OBS_SENDSTART)
		{
			//LOG_TRACE("UpdateStatus gs_id=%d, map_tag=%d, time=%d send start!!!!", it->second.id.gs_id, it->second.id.map_tag, now);
			//OpenBanquetManager::GetInstance()->OnOpenBanquetStart(it->second.id.gs_id, it->second.id.map_tag,0);
			if(it->second.timeout && it->second.timeout <= now)
			{
				LOG_TRACE("OpenBanquetManager::UpdateStatus status is OBS_SENDSTART timeout, cancel gs_id=%d,map_tag=%d", it->second.id.gs_id, it->second.id.map_tag);
				it->second.status &= (~OBS_SENDSTART);
				it->second.status |= OBS_CANCEL;
				it->second.timeout = 0;
			}
		}
		else if(it->second.status&OBS_FIGHTING)
		{
			//LOG_TRACE("UpdateStatus gs_id=%d, map_tag=%d, map_id=%d fighting!!!!", it->second.id.gs_id, it->second.id.map_tag, it->second.map_id);
		}
		else if(it->second.status&OBS_OPEN)
		{
			it->second.timeout = now + SENDSTART_TIMEOUT;
			
			BattleStart start(0, 0, 0);
			start.end_time = now + it->second.last_time;
			start.tag = it->second.id.map_tag;
			start.map_id = it->second.map_id;
			start.battle_type = GetFieldType();
			LOG_TRACE("OpenBanquetManager::UpdateStatus status is OBS_OPEN gs_id=%d, map_tag=%d, last_time=%d, end_time=%d, sid=%d",
					it->second.id.gs_id, start.tag, it->second.last_time, start.end_time, it->second.sid);
			
			GProviderServer::GetInstance()->Send(it->second.sid, start);
			it->second.end_time = now + it->second.last_time;

			it->second.status |= OBS_SENDSTART;
		}
	}
}

void OpenBanquetManager::UpdateOpenBanquet(time_t now)
{
	ServerMap::iterator it = servers.begin();
	for(; it != servers.end(); ++it)
	{
		if(!(it->second.status & OBS_FIGHTING))
		{
			//LOG_TRACE("OpenBanquetManager::UpdateOpenBanquet status=%x not is OBS_FIGHTING!", it->second.status);
			continue;
		}
		
		// 清除到时间的
		it->second.WaitMapClear(now);

		// 清除断线到时的
		it->second.UpdateTimeOut(now);

		// 检测排队状态及拉人
		if(!it->second.IsRolesFull() && !it->second.wait_queue.empty())
		{
			// 战场还可以进人
			TRolesVec front_roles = it->second.wait_queue.front();
			//LOG_TRACE("OpenBanquetManager::UpdateOpenBanquet wait_queue.size=%d front_roles.size=%d", it->second.wait_queue.size(), front_roles.size());

			if(front_roles.size() == 0)
			{
				it->second.PopWaitQueue();
				// 队列里没玩家
				LOG_TRACE("OpenBanquetManager::UpdateOpenBanquet front_roles.size() == 0");
				break;
			}
			
			int free_faction;
			if(!it->second.CheckFreeInfo(free_faction, front_roles.size()))
			{
				// 战场无位置
				//LOG_TRACE("OpenBanquetManager::UpdateOpenBanquet CheckFreeInfo is fail");
				break;
			}

			if(it->second.PutWaitMap(free_faction, front_roles, now))
			{
				// 成功
				LOG_TRACE("OpenBanquetManager::UpdateOpenBanquet PutWaitMap free_faction=%d, front_roles.size=%d", free_faction, front_roles.size());
				it->second.PopWaitQueue();
			}
		}
	}
}

bool OpenBanquetManager::Update()
{
	time_t now = UpdateTime();
	
	// UpdateStatus更新频率要低
	if(t_status_update < now)
	{
		LOG_TRACE("UpdateTime forget=%d,now=%d,t_base=%d,now-t_base=%d,%s", t_forged, now, t_base, now-t_base, ctime(&now));

		UpdateStatus(now);
		t_status_update = now + STATUS_UPDATE_INTERVAL;
	}

	UpdateOpenBanquet(now);
	
	return true;
}

void OpenBanquetManager::SendNotifyMsg(int roleid, int result)
{
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (pinfo)
	{
		GDeliveryServer::GetInstance()->Send(pinfo->linksid, OpenBanquetNotify_Re(roleid, result, pinfo->localsid));
	}
}

int OpenBanquetManager::OnJoin(TRolesVec& roles, char is_team, int gsid)
{
	//LOG_TRACE("OpenBanquetManager::OnJoin exec");

	if(roles.size() <= 0 || roles.size() > 6)
	{
		 Log::log(LOG_ERR,"OpenBanquetManager::OnJoin roles.size==0!");
		 return ERR_OPB_TEAM_INFO_FAIL;
	}

	int leader_roleid = roles[0];
	OpenBanquetInfo* pOpenBanquetInfo = FindFreeServer();
	if(!pOpenBanquetInfo)
	{
		SendNotifyMsg(leader_roleid, ERR_OPB_MAP_NOTEXIST);
		Log::log(LOG_INFO,"OpenBanquetManager::OnJoin leader=%d pOpenBanquetInfo is NULL!", leader_roleid);
		return ERR_OPB_MAP_NOTEXIST;
	}
	
	if(!(pOpenBanquetInfo->status & OBS_FIGHTING))
	{
		SendNotifyMsg(leader_roleid, ERR_OPB_NOT_OPEN);
		Log::log(LOG_INFO,"OpenBanquetManager::OnJoin leader=%d pOpenBanquetInfo status not is OBS_FIGHTING!", leader_roleid);
		return ERR_OPB_NOT_OPEN;
	}
	
	pOpenBanquetInfo->PushWaitQueue(roles, gsid);	
	return ERR_SUCCESS;
}

// 换线操作
void OpenBanquetManager::OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map)
{
	LOG_TRACE("OpenBanquetManager::OnChangeGS Leave ChangeGS old gs(%d):tag(%d) new gs(%d):tag(%d) role %d retcode %d", old_gs, old_map, new_gs, new_map, roleid, retcode);
	
	if (IsOpenBanquetServer(old_gs, old_map))// 从流水席战场换线
	{
		ServerMap::iterator it = servers.begin();
		for(; it != servers.end(); ++it)
		{
			if (it->second.id.map_tag == old_map)
			{
				if (retcode == ERR_CHGS_SUCCESS)
				{
					LOG_TRACE("OpenBanquetManager::OnChangeGS gs_id=%d,map_tag=%d attack erase role %d", it->second.id.gs_id, it->second.id.map_tag, roleid);
					it->second.BattleMapClear(roleid);
				}
				break;
			}
		}
	}
}

void OpenBanquetManager::OnLogout(int roleid, int gsid, int map)
{

	SOpenBanquetID* pOpenBanquetID = FindRolesServer(roleid);    
	if(NULL == pOpenBanquetID)
	{
		LOG_TRACE("OpenBanquetManager::OnLogout roleid=%d FindRolesServer is NULL");
		return;
	}

	LOG_TRACE("OpenBanquetManager::OnLogout role %d Logout gs %d map %d", roleid, pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
	if (IsOpenBanquetServer(pOpenBanquetID->gs_id, pOpenBanquetID->map_tag))
	{
		OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
		if(!pOpenBanquetInfo)
		{
			LOG_TRACE("OpenBanquetManager::OnLogout but pOpenBanquetInfo is NULL! gs_id=%d, map_tag=%d", pOpenBanquetID->gs_id, pOpenBanquetID->map_tag);
			return;
		}
		

		LOG_TRACE("OpenBanquetManager::OnLogout gs_id=%d,map_tag=%d erase role %d", pOpenBanquetID->gs_id, pOpenBanquetID->map_tag, roleid);
		pOpenBanquetInfo->BattleMapClear(roleid);
	}
}

void OpenBanquetManager::OnRealEnter(int roleid, int new_gs, int new_map)
{
	LOG_TRACE("OpenBanquetManager OnRealEnter gs_id=%d:map_tag=%d",  new_gs, new_map);

	if (IsOpenBanquetServer(new_gs, new_map))
	{
		OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(new_gs, new_map);
		if(!pOpenBanquetInfo)
		{
			LOG_TRACE("OpenBanquetManager::OnRealEnter but pOpenBanquetInfo is NULL! gs_id=%d, map_tag=%d", new_gs, new_map);
			return;
		}
		
		if(pOpenBanquetInfo->GetRoleStatus(roleid) == EOBRS_ACCEPT)
		{
			LOG_TRACE("OpenBanquetManager::OnRealEnter gs_id=%d, map_tag=%d, roleid=%d", new_gs, new_map, roleid);
			pOpenBanquetInfo->Enter(roleid);
			InsertPerdayLimit(roleid);
		}
	}
}

void OpenBanquetManager::OnQueue(int roleid, int new_gs, int new_map)
{
	LOG_TRACE("OpenBanquetManager OnQueue gs_id=%d:map_tag=%d", new_gs, new_map);

	if (IsOpenBanquetServer(new_gs, new_map))
	{
		OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(new_gs, new_map);
		if(!pOpenBanquetInfo)
		{
			LOG_TRACE("OpenBanquetManager OnQueue but pOpenBanquetInfo is NULL! gs_id=%d, map_tag=%d", new_gs, new_map);
			return;
		}
		
		if(pOpenBanquetInfo->GetRoleStatus(roleid) == EOBRS_QUEUE)
		{
			LOG_TRACE("OpenBanquetManager OnQueue queue gs_id=%d, map_tag=%d, roleid=%d", new_gs, new_map, roleid);
			pOpenBanquetInfo->SendJoinMsg(roleid, S2C_OPENBANQUET_QUEUE);
		}
	}
}

void OpenBanquetManager::LogOpenBanquetList()
{
	ServerMap::iterator it = servers.begin();
	for(; it != servers.end(); ++it)
	{
		Log::formatlog("OpenBanquetManager::LogOpenBanquetList", "gs_id=%d,map_tag=%d", it->second.id.gs_id, it->second.id.map_tag);
	}
}

int OpenBanquetManager::OnOpenBanquetEnd(int gs_id, int map_tag, int result)
{
	SOpenBanquetID id;
	id.gs_id = gs_id;
	id.map_tag = map_tag;

	time_t now = GetTime();
	ServerMap::iterator it = servers.begin();
	ServerMap::iterator ite = servers.end();
	for(; it != ite; ++it)
	{
		if(it->second.id==id)
			break;
	}

	if (it == ite)
	{
		Log::log(LOG_ERR, "OpenBanquetManager::OnOpenBanquetEnd can not find gs_id=%d,map_tag=%d", id.gs_id, id.map_tag);
		return ERR_OPB_MAP_NOTEXIST;
	}
	
	if (!(it->second.status & OBS_FIGHTING))
	{
		Log::log(LOG_ERR, "OpenBanquetManager::OnOpenBanquetEnd status %x never start", it->second.status);
		return ERR_OPB_NOT_OPEN;
	}
	
	Log::formatlog("OpenBanquetManager::OnOpenBanquetEnd","gs_id=%d:map_tag=%d:time=%d:result=%d:attcksize=%d:defendsize=%d:map_id=%d", 
			 id.gs_id, id.map_tag, now, result, it->second.attack_map.size(), it->second.defend_map.size(), it->second.map_id);

	it->second.status &= (~OBS_FIGHTING);
	it->second.Reset();
	LogOpenBanquetList();

	return ERR_SUCCESS;
}

int OpenBanquetManager::OnOpenBanquetStart(int gs_id, int map_tag, int retcode)
{
	SOpenBanquetID id;
	id.gs_id = gs_id;
	id.map_tag = map_tag;

	ServerMap::iterator it = servers.begin();
	ServerMap::iterator ie = servers.end();
	for(; it != ie; ++it)
	{
		if (id == it->second.id)
			break;
	}
	if (it == ie)
	{
		Log::log(LOG_ERR, "OnOpenBanquetStart can not find gs_id=%d,map_tag=%d", id.gs_id, id.map_tag);
		return ERR_OPB_MAP_NOTEXIST;
	}
	if (!(it->second.status & OBS_SENDSTART))
	{
		Log::log(LOG_ERR, "OnOpenBanquetStart gs_id=%d,map_tag=%d status %x invalid", id.gs_id, id.map_tag, it->second.status);
		return ERR_OPB_NOT_OPEN;
	}

	it->second.status &= (~OBS_SENDSTART);
	if(retcode != ERR_SUCCESS)
	{
		it->second.status |= OBS_CANCEL;
		Log::log(LOG_ERR,"OnOpenBanquetStart failed (%d), cancel gs_id=%d,map_tag=%d", retcode, it->second.id.gs_id, it->second.id.map_tag);
		return ERR_OPB_NOT_OPEN;
	}
	it->second.status |= OBS_FIGHTING;
	Log::formatlog("OpenBanquetManager::OnOpenBanquetStart","gs_id=%d:map_tag=%d:time=%d:map_id=%d", it->second.id.gs_id, it->second.id.map_tag, GetTime(),it->second.map_id);

	return ERR_SUCCESS;
}

//流水席战场gs掉线
void OpenBanquetManager::OnDisconnect(int gsid)
{       
	LOG_TRACE("OpenBanquet gs %d disconnected", gsid);
	for (ServerMap::iterator it=servers.begin(),ite=servers.end(); it!=ite;)
	{
		if (it->second.id.gs_id == gsid)
		{
			servers.erase(it++);
		}
		else
			++it;
	}
	Log::formatlog("OpenBanquetManager::OnDisconnect","gs_id=%d", gsid);
}

int OpenBanquetManager::OnBattleLeave(int roleid, int gs_id, int map_tag)
{
	SOpenBanquetID id;
	id.gs_id = gs_id;
	id.map_tag = map_tag;

	ServerMap::iterator it = servers.find(id);
	if (it == servers.end())
	{
		LOG_TRACE("OpenBanquetManager::OnBattleLeave not find  gs_id=%d, map_tag=%d", gs_id, map_tag);
		return ERR_BATTLE_MAP_NOTEXIST;
	}
	
	it->second.Leave(roleid);
	return ERR_SUCCESS;
}

int OpenBanquetManager::OnTryEnter(int roleid, int gs_id, int map_tag, int & endtime, char &battle_type, char iscross, bool b_gm)
{
	LOG_TRACE("OpenBanquetManager::OnBattleEnter role=%d, gs_id=%d, map_tag=%d, servers.size=%d", roleid, gs_id, map_tag, servers.size());
	SOpenBanquetID id;
	id.gs_id = gs_id;
	id.map_tag = map_tag;

	ServerMap::iterator it = servers.find(id);
	if (it == servers.end())
	{
		return ERR_BATTLE_MAP_NOTEXIST;
	}
	if (!(it->second.status & OBS_FIGHTING))
	{
		Log::log(LOG_ERR, "OnBattleEnter status %x never start", it->second.status);
		return ERR_OPB_NOT_OPEN;
	}

	if(!iscross)// 跨服-跨服
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL == pinfo )
		{
			Log::log(LOG_ERR,"OnBattleEnter roleid=%d, iscross=%d is not online", roleid, iscross);
			return ERR_OPB_APPOINT_TARGETNOTFOUND;
		}

		if(!it->second.GetEnterFaction(roleid, pinfo->chgs_territoryrole))
		{
			Log::log(LOG_ERR,"OnBattleEnter roleid=%d, iscross=%d GetEnterFaction fail", roleid, iscross);
			return ERR_OPB_ROLE_NOT_FIND_WAIT;
		}

		LOG_TRACE( "OnBattleEnter TryChangeGS_Re roleid=%d,gs_id=%d,map_tag=%d,pinfo->chgs_territoryrole=%d",roleid, it->second.id.gs_id, it->second.id.map_tag,pinfo->chgs_territoryrole);
		int reason = ENTER_OPENBANQUT_REASON;
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, it->second.id.gs_id, it->second.id.map_tag, reason));
	}
	else// 原服-跨服
	{
		/*
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL == pinfo )
		{
			Log::log(LOG_ERR,"OnBattleEnter roleid=%d, iscross=%d is not online", roleid, iscross);
			return ERR_OPB_APPOINT_TARGETNOTFOUND;
		}
		*/
		char faction = 0;
		if(!it->second.GetEnterFaction(roleid, faction))
		{
			Log::log(LOG_ERR,"OnBattleEnter roleid=%d, iscross=%d GetEnterFaction fail", roleid, iscross);
			return ERR_OPB_ROLE_NOT_FIND_WAIT;
		}
	}

	it->second.SetRoleStatus(roleid, EOBRS_ACCEPT);
	return ERR_SUCCESS;
}

bool OpenBanquetManager::ReEnter(int roleid, int& gs_id, int& map_tag, char& battle_type)
{
	// 只有一个流水席战场
	ServerMap::iterator it = servers.begin();
	for(; it != servers.end(); ++it)
	{
		OpenBanquetInfo& pInfo = it->second;
	
		TRolesMap::iterator attack_it = pInfo.attack_map.find(roleid);
		if(attack_it != pInfo.attack_map.end())
		{
			gs_id = pInfo.id.gs_id;
			map_tag = pInfo.id.map_tag;
			battle_type = EOBF_ATTACKER;
			
			pInfo.RemoveTimeOut(roleid);
			LOG_TRACE( "OnBattleEnter::ReEnter roleid=%d,gs_id=%d,map_tag=%d,battle_type=%d",roleid, gs_id, map_tag, battle_type);
			return true;
		}

		TRolesMap::iterator defend_it = pInfo.defend_map.find(roleid);
		if(defend_it != pInfo.defend_map.end())
		{
			gs_id = pInfo.id.gs_id;
			map_tag = pInfo.id.map_tag;
			battle_type = EOBF_DEFENDER;
			
			pInfo.RemoveTimeOut(roleid);
			LOG_TRACE( "OnBattleEnter::ReEnter roleid=%d,gs_id=%d,map_tag=%d,battle_type=%d",roleid, gs_id, map_tag, battle_type);
			return true;
		}
	}

	return false;
}

int OpenBanquetManager::OnCancel(int roleid, int gs_id, int map_tag, char isqueue)
{
	OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(gs_id, map_tag);
	if(!pOpenBanquetInfo)
	{
		LOG_TRACE("OpenBanquetManager::OnCancel but pOpenBanquetTag is NULL! gs_id=%d, map_tag=%d", gs_id, map_tag);
		return ERR_OPB_MAP_NOTEXIST;
	}

	pOpenBanquetInfo->QueueCancel(roleid, isqueue);
	return ERR_SUCCESS;
}

int OpenBanquetManager::TryAbnormalOffline(int roleid, int gs_id, int map_tag)
{
	OpenBanquetInfo* pOpenBanquetInfo = FindMapInfo(gs_id, map_tag);
	if(!pOpenBanquetInfo)
	{
		//LOG_TRACE("OpenBanquetManager::TryAbnormalOffline but pOpenBanquetTag is NULL! gs_id=%d, map_tag=%d", gs_id, map_tag);
		return ERR_OPB_MAP_NOTEXIST;
	}

	char faction;
	if(!pOpenBanquetInfo->FindRole(roleid, faction))
	{
		return ERR_OPB_ROLE_NOT_IN_BATTLE; 
		//如果玩家正常退出，将在此返回，因为已经从管理器中删除了
	}

	time_t now = GetTime();
	pOpenBanquetInfo->SetTimeOut(roleid, faction, now);	
	return ERR_SUCCESS;
}

void OpenBanquetManager::SetMaxHalfRoleNum(int roleid, unsigned int num)
{
	LOG_TRACE("SetMaxHalfRoleNum roleid=%d, num=%d", roleid, num);
	ServerMap::iterator it = servers.begin();
	for(; it != servers.end(); ++it)
	{
		LOG_TRACE("real SetMaxHalfRoleNum roleid=%d, num=%d", roleid, num);
		OpenBanquetInfo& pInfo = it->second;
		pInfo.EOBI_MAX_HALF_ROLES_NUM = num;
	}
}

void OpenBanquetManager::SetHalfRoleNum(int roleid)
{
	LOG_TRACE("SetHalfRoleNum roleid=%d", roleid);
	ServerMap::iterator it = servers.begin();
	for(; it != servers.end(); ++it)
	{
		LOG_TRACE("real SetHalfRoleNum roleid=%d", roleid);
		OpenBanquetInfo& pInfo = it->second;
		pInfo.attack_map.insert(28845992);
	}

}



};

