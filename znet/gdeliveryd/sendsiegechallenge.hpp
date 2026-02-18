
#ifndef __GNET_SENDSIEGECHALLENGE_HPP
#define __GNET_SENDSIEGECHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"
#include "siegechallenge_re.hpp"
#include "dbsiegechallenge.hrp"
#include "factionmanager.h"

namespace GNET
{

class SendSiegeChallenge : public GNET::Protocol
{
	#include "sendsiegechallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SendSiegeChallenge roleid=%d,battle_id=%d,factionid=%d,day=%d", roleid,battle_id,factionid,day);
		int linksid, localsid, master;

		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid!=(unsigned int)factionid || pinfo->factiontitle>TITLE_VICEMASTER)
			{
				manager->Send(sid, GMailEndSync(0, ERR_SIEGE_FACTIONLIMIT, roleid));
				return;
			}
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
		}
		if (SiegeManager::GetInstance()->IsInCooldown(factionid))
		{
			manager->Send(sid, GMailEndSync(0, ERR_SIEGE_COOLING, roleid));
			GDeliveryServer::GetInstance()->Send(linksid, SiegeChallenge_Re(ERR_SIEGE_COOLING,battle_id, 0, localsid));
			return;
		}
		SiegeManager::GetInstance()->SetCooldown(factionid);

		FactionDetailInfo * faction = FactionManager::Instance()->Find(factionid);
		if (!faction)
		{
			manager->Send(sid, GMailEndSync(0, ERR_SIEGE_FACTIONLIMIT, roleid));
			return;
		}
		master = faction->info.master;
		//帮派等级为4级才能报名城战
		if (faction->info.level < 3 ||  Timer::GetTime() - faction->info.createtime <  3600*24*7 )
		{
			GDeliveryServer::GetInstance()->Send(linksid,
					SiegeChallenge_Re(ERR_SIEGE_FACTIONLIMIT,battle_id,0,localsid));
			manager->Send(sid, GMailEndSync(0, ERR_SIEGE_FACTIONLIMIT, roleid));
			return;
		}
		int begin_time;
		int rslt = SiegeManager::GetInstance()->CanChallenge(battle_id, factionid, begin_time);
		if (rslt == ERR_SUCCESS)
		{
			DBSiegeChallengeArg arg(battle_id, roleid, factionid, master, day, begin_time, attach_obj_id, syncdata);
			DBSiegeChallenge *rpc = (DBSiegeChallenge *) Rpc::Call(RPC_DBSIEGECHALLENGE, arg);
			rpc->localsid = localsid;
			rpc->link_sid = linksid;
			rpc->roleid = roleid;
			rpc->gamesid = sid;
			GameDBClient::GetInstance()->SendProtocol(rpc);

		}
		else
		{
			GDeliveryServer::GetInstance()->Send(linksid, SiegeChallenge_Re(rslt,battle_id, 0, localsid));
			manager->Send(sid, GMailEndSync(0, rslt,roleid));
		}
	}
};

};

#endif
