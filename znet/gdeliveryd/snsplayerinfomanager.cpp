#include "snsplayerinfomanager.h"
#include "snsmessagemanager.h"
#include "dbsnsgetplayerinfocache.hrp"
#include "dbsnsgetplayerinfo.hrp"
#include "dbsnsgetroleinfo.hrp"
#include "dbsnsupdateplayerinfo.hrp"
#include "dbsnsupdateroleinfo.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
#include "sectmanager.h"
#include "familymanager.h"

namespace GNET
{

#define STAR_TO_CHARM(STAR)	(20000*((STAR)*(STAR)*(STAR)+4*(STAR)))

int SNSPlayerInfoManager::STAR_CHARM[SNS_LIMIT_CHARMSTAR+1];

bool SNSPlayerInfoManager::Initialize()
{
	for (int i = 0;  i <= SNS_LIMIT_CHARMSTAR; i++)
		STAR_CHARM[i] = STAR_TO_CHARM(i);
	open = true;
	return true;
}

void SNSPlayerInfoManager::OnLoad(const GSNSPlayerInfo &info)
{
	SNSPlayerInfo &snsinfo = m_cache[info.roleid].playerinfo;
	GSNSToSNSPlayerInfo(info, snsinfo);

	//如果是帮主从帮派中获取帮派魅力值信息
	PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(info.roleid);
	if (pinfo!=NULL)
	{
		FactionDetailInfo *pfaction = FactionManager::Instance()->Find(pinfo->factionid);
		if (pfaction!=NULL)
		{
			int faction_charm = pfaction->GetCharm();

			if (pinfo->factiontitle == TITLE_MASTER)
			{
				if (faction_charm != snsinfo.charm_faction)
				{
					snsinfo.charm_faction = faction_charm;

					if (SNSMessageManager::GetInstance()->HavePlayerPressedMessage(info.roleid))
						SyncDB(snsinfo);
				}
			}
			else
			{
				if (snsinfo.charm_faction != 0)
				{
					snsinfo.charm_faction = 0;

					if (SNSMessageManager::GetInstance()->HavePlayerPressedMessage(info.roleid))
						SyncDB(snsinfo);
				}
			}
		}
	}
}

void SNSPlayerInfoManager::LoadSNSPlayerList(vector<int> &roleid_list)
{
	DBSNSGetPlayerInfoCache::GetNextPlayer(roleid_list);
}

void SNSPlayerInfoManager::OnPlayerLogin(int roleid)
{
	if (!open)
		return;
	UpdateSNSRoleInfo(roleid);

	DBSNSGetPlayerInfo *rpc = (DBSNSGetPlayerInfo *)Rpc::Call(RPC_DBSNSGETPLAYERINFO, DBSNSGetPlayerInfoArg(roleid));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void SNSPlayerInfoManager::OnPlayerLogout(int roleid)
{
	if (!SNSMessageManager::GetInstance()->HavePlayerPressedMessage(roleid))
		m_cache.erase(roleid);
}

void SNSPlayerInfoManager::UpdateSNSPlayerInfo(const SNSPlayerInfo &info)
{
	SNSPlayerInfo *localinfo = &m_cache[info.roleid].playerinfo;

	localinfo->roleid = info.roleid;
	localinfo->is_private = info.is_private;
	localinfo->rolename = info.rolename;
	localinfo->nickname = info.nickname;
	localinfo->age = info.age;
	localinfo->birthday = info.birthday;
	localinfo->province = info.province;
	localinfo->city = info.city;
	localinfo->career = info.career;
	localinfo->address = info.address;
	localinfo->introduce = info.introduce;

	SyncDB(*localinfo);
}

//尝试从RoleInfoCache中获取最新的数据，并更新到数据库
void SNSPlayerInfoManager::UpdateSNSRoleInfo(int roleid)
{
	GRoleInfo *pinfo = RoleInfoCache::Instance().Get(roleid);

	if (pinfo != NULL)
	{
		GSectInfo *sectinfo = NULL;
		if (roleid != pinfo->sectid)			//自己不是师傅
			sectinfo = SectManager::Instance()->FindSect(pinfo->sectid);
		FamilyInfo *familyinfo = FamilyManager::Instance()->Find(pinfo->familyid);
		FactionDetailInfo *factioninfo = familyinfo==NULL ? NULL : familyinfo->pfaction;	

		GSNSRoleInfo info(pinfo->level, pinfo->reborndata.size()/sizeof(int), pinfo->gender, pinfo->occupation, pinfo->spouse, pinfo->name,
				sectinfo==NULL ? Octets() : sectinfo->name,
				familyinfo==NULL ? Octets() : familyinfo->info.name,
				factioninfo==NULL ? Octets() : factioninfo->info.name,
				pinfo->title,
				factioninfo==NULL ? 0 : factioninfo->info.level);
		OnLoad(roleid, info);

		if (SNSMessageManager::GetInstance()->HavePlayerPressedMessage(roleid))
			SyncDB(roleid, info);
	}
}

void SNSPlayerInfoManager::UpdateSNSRoleLevel(int roleid, unsigned char level)
{
	if (IsPlayerExist(roleid))
	{
		SNSRoleInfo *localinfo = &m_cache[roleid].roleinfo;
		localinfo->level = level;

		if (SNSMessageManager::GetInstance()->HavePlayerPressedMessage(roleid))
			SyncDB(roleid, *localinfo);
	}
}

void SNSPlayerInfoManager::UpdateSNSPlayerCharm(SNS_CHARM_TYPE type, int roleid, int charm_diff)
{
	if (IsPlayerExist(roleid))
	{
		SNSPlayerInfo *localinfo = &m_cache[roleid].playerinfo;
		int *charm = NULL;
		switch (type)
		{
			case SNS_CHARM_TYPE_PERSONAL:
				charm = &localinfo->charm_personal; 
				break;
			case SNS_CHARM_TYPE_FRIEND:
				charm = &localinfo->charm_friend; 
				break;
			case SNS_CHARM_TYPE_SECT:
				charm = &localinfo->charm_sect; 
				break;
			case SNS_CHARM_TYPE_FACTION:
				charm = &localinfo->charm_faction; 
				break;
			default:
				break;
		}

		if (charm != NULL)
		{
			*charm += charm_diff;
			if (*charm < 0)
				*charm = 0;

			SyncDB(*localinfo);
		}
	}
}

void SNSPlayerInfoManager::SetSNSPlayerCharm(SNS_CHARM_TYPE type, int roleid, int newcharm)
{
	if (IsPlayerExist(roleid))
	{
		SNSPlayerInfo *localinfo = &m_cache[roleid].playerinfo;
		int *charm = NULL;
		switch (type)
		{
			case SNS_CHARM_TYPE_PERSONAL:
				charm = &localinfo->charm_personal; 
				break;
			case SNS_CHARM_TYPE_FRIEND:
				charm = &localinfo->charm_friend; 
				break;
			case SNS_CHARM_TYPE_SECT:
				charm = &localinfo->charm_sect; 
				break;
			case SNS_CHARM_TYPE_FACTION:
				charm = &localinfo->charm_faction; 
				break;
			default:
				break;
		}

		if (charm != NULL)
		{
			*charm = newcharm;
			if (*charm < 0)
				*charm = 0;

			SyncDB(*localinfo);
		}
	}
}

void SNSPlayerInfoManager::UpdateSNSPlayerPressTime(int roleid, int time)
{
	if (IsPlayerExist(roleid))
	{
		SNSPlayerInfo *localinfo = &m_cache[roleid].playerinfo;
		localinfo->press_time = time;

		SyncDB(*localinfo);
	}
}

void SNSPlayerInfoManager::UpdateSNSPlayerApplyTime(int roleid, int time)
{
	if (IsPlayerExist(roleid))
	{
		SNSPlayerInfo *localinfo = &m_cache[roleid].playerinfo;
		localinfo->apply_time = time;
	}
}

void SNSPlayerInfoManager::UpdateSNSPlayerLeaveMsgTime(int roleid, int time)
{
	if (IsPlayerExist(roleid))
	{
		SNSPlayerInfo *localinfo = &m_cache[roleid].playerinfo;
		localinfo->leavemsg_time = time;
	}
}

void SNSPlayerInfoManager::UpdateSNSPlayerVoteTime(int roleid, int time)
{
	if (IsPlayerExist(roleid))
	{
		SNSPlayerInfo *localinfo = &m_cache[roleid].playerinfo;
		localinfo->vote_time = time;
	}
}

void SNSPlayerInfoManager::OnFactionAppointMaster(const FactionDetailInfo *pfaction, int master, int candidate)
{
	LOG_TRACE("SNSPlayerInfoManager::OnFactionAppointMaster, factionid=%d, faction_charm=%d, master=%d, candidate=%d\n", pfaction->info.fid, pfaction->info.charm, master, candidate);
	if (IsPlayerExist(master))
	{
		SNSPlayerInfo *localinfo = &m_cache[master].playerinfo;
		if (localinfo->charm_faction != 0)
		{
			localinfo->charm_faction = 0;
			SyncDB(*localinfo);
		}
	}

	if (IsPlayerExist(candidate))
	{
		SNSPlayerInfo *localinfo = &m_cache[candidate].playerinfo;

		if (localinfo->charm_faction != pfaction->info.charm)
		{
			localinfo->charm_faction = pfaction->info.charm;
			SyncDB(*localinfo);
		}
	}
}

void SNSPlayerInfoManager::OnFactionDelete(const FactionDetailInfo *pfaction)
{
	LOG_TRACE("SNSPlayerInfoManager::OnFactionDelete, factionid=%d, faction_charm=%d, master=%d\n", pfaction->info.fid, pfaction->info.charm, pfaction->info.master);

	if (IsPlayerExist(pfaction->info.master))
	{
		SNSPlayerInfo *localinfo = &m_cache[pfaction->info.master].playerinfo;

		if (localinfo->charm_faction != 0)
		{
			localinfo->charm_faction = 0;
			SyncDB(*localinfo);
		}
	}
}

void SNSPlayerInfoManager::SyncDB(const SNSPlayerInfo &info)
{
	GSNSPlayerInfo ginfo;
	GSNSToSNSPlayerInfo(info, ginfo);
	DBSNSUpdatePlayerInfo *rpc = (DBSNSUpdatePlayerInfo *)Rpc::Call(RPC_DBSNSUPDATEPLAYERINFO, DBSNSUpdatePlayerInfoArg(ginfo));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void SNSPlayerInfoManager::SyncDB(int roleid, const SNSRoleInfo &info)
{
	GSNSRoleInfo ginfo;
	GSNSToSNSRoleInfo(info, ginfo);
	SyncDB(roleid, ginfo);
}

void SNSPlayerInfoManager::SyncDB(int roleid, const GSNSRoleInfo &ginfo)
{
	DBSNSUpdateRoleInfo *rpc = (DBSNSUpdateRoleInfo *)Rpc::Call(RPC_DBSNSUPDATEROLEINFO, DBSNSUpdateRoleInfoArg(roleid, ginfo));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void SNSPlayerInfoManager::OnRolenameChange(int roleid, const Octets & newname)
{
	PLAYERMAP::iterator it = m_cache.find(roleid);
	if (it != m_cache.end())
	{
		if (it->second.roleinfo.rolename == it->second.roleinfo.sectname)
			it->second.roleinfo.sectname = newname;
		it->second.roleinfo.rolename = newname;
		it->second.playerinfo.rolename = newname;
	}
}

};
