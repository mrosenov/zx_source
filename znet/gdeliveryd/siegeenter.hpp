
#ifndef __GNET_SIEGEENTER_HPP
#define __GNET_SIEGEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "siegeenter_re.hpp"
#include "glog.h"


namespace GNET
{

class SiegeEnter : public GNET::Protocol
{
	#include "siegeenter"

	int save_sid;
	void EnterSiegeServer(int gs_id, int dst_gs_id, int tag)
	{
		GProviderServer::GetInstance()->DispatchProtocol(gs_id, PlayerChangeGS(roleid, localsid, dst_gs_id, tag, 0, 0, 0, 0));

	}
	void SendError(int retcode)
	{
		LOG_TRACE("SiegeEnter_Re retcode=%d, roleid=%d,battle_id=%d", retcode, roleid, battle_id);
		SiegeEnter_Re re(retcode, roleid, battle_id);
		re.localsid = localsid;
		GDeliveryServer::GetInstance()->Send(save_sid, re);
	}
	bool IsNewComer(int faction_join_time, int familyid)
	{
		int now = Timer::GetTime();
		if (now < faction_join_time + 7 * 86400)
			return true;

		FamilyInfo * family = FamilyManager::Instance()->Find(familyid);
		if (now < family->info.jointime + 7 * 86400)
			return true;
		return false;
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SiegeEnter roleid=%d,battle_id=%d", roleid, battle_id);
		save_sid = sid;
		int gs_id;
		bool b_gm = false;
		int jointime, familyid, factionid;
		int userid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo)
			       return;	
			if (pinfo->IsGM())
			{
				b_gm = true;
			}
			else
			{
				if (pinfo->factionid == 0) 
				{ 
					SendError(ERR_SIEGE_NOFACTION);
					return;
				}
				jointime = pinfo->jointime;
				familyid = pinfo->familyid;
				factionid = pinfo->factionid;
			}
			userid= pinfo->userid;
			gs_id = pinfo->gameid;
		}
		CityDetail * city = SiegeManager::GetInstance()->Find(battle_id);
		if (!city)
		{
			SendError(ERR_SIEGE_NOTFOUND);
			return;
		}
		if (city->challengers.size() > 0)
		{
			GChallenger & challenger = city->challengers.front();
			int now = Timer::GetTime();
			if (challenger.begin_time <= now && challenger.begin_time > now - 450)
			{
				LOG_TRACE("SiegeEnter: clearing normal field, can't enter, roleid=%d", roleid);
				return;
			}
		}
		if (city->status == CityDetail::STATUS_WAITING)
		{
			if (b_gm)
			{
				GLog::action("gowar,cid=%d:uid=%d:rid=%d:fid=%d",battle_id,userid,roleid,factionid);
				EnterSiegeServer(gs_id, city->tag_common.gs_id, city->tag_common.tag);
			}
			else
			{

				if (factionid != city->GetOwner())
				{
					SendError(ERR_SIEGE_CANTENTERNORMAL);
					return;
				}
				if (IsNewComer(jointime, familyid))
				{
					SendError(ERR_BS_NEWBIE_BANNED);
					return;
				}
				GLog::action("gowar,cid=%d:uid=%d:rid=%d:fid=%d",battle_id,userid,roleid,factionid);
				EnterSiegeServer(gs_id, city->tag_common.gs_id, city->tag_common.tag);
			}
		}
		else 
		{
			if (b_gm)
			{
				GLog::action("gowar,cid=%d:uid=%d:rid=%d:fid=%d",battle_id,userid,roleid,factionid);
				EnterSiegeServer(gs_id, city->tag_battle.gs_id, city->tag_battle.tag);
			}
			else
			{
				int owner = city->GetOwner();
				int challenger = city->GetAttacker();
				int assistant = city->GetAssistant();
				if (factionid != owner && factionid != challenger && factionid != assistant)
				{
					SendError(ERR_SIEGE_CANTENTERBATTLE);
					return;
				}
				if (IsNewComer(jointime, familyid))
				{
					SendError(ERR_BS_NEWBIE_BANNED);
					return;
				}
				GLog::action("gowar,cid=%d:uid=%d:rid=%d:fid=%d",battle_id,userid,roleid,factionid);
				
				EnterSiegeServer(gs_id, city->tag_battle.gs_id, city->tag_battle.tag);
			}
		}

		SiegeEnter_Re re(ERR_SUCCESS, roleid, battle_id);
		re.localsid = localsid;
		city->GetFactionInfo(re);
		LOG_TRACE("SiegeEnter_Re retcode=%d, roleid=%d,battle_id=%d", re.retcode, roleid, battle_id);
		GDeliveryServer::GetInstance()->Send(save_sid, re);

	}
};

};

#endif
