
#ifndef __GNET_FACTIONAPPOINT_HPP
#define __GNET_FACTIONAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionappoint_re.hpp"

namespace GNET
{

class FactionAppoint : public GNET::Protocol
{
	#include "factionappoint"

	void DoFactionAppoint(Manager *manager, Manager::Session::ID sid)
	{
		DBFactionAppointArg arg;
		char mytitle,oldtitle;
		int familyid,localsid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid==0 || (title!=TITLE_MASTER && pinfo->factiontitle>=title)) 
				return;
			arg.fid = pinfo->factionid;
			mytitle = pinfo->factiontitle;
			localsid = pinfo->localsid;

		}

		if(mytitle==TITLE_MASTER && !FactionManager::Instance()->IsMaster(arg.fid, roleid))
			return;
		if(!FactionManager::Instance()->GetMemberinfo(arg.fid, candidate, familyid, oldtitle))
			return;
		if(oldtitle==TITLE_MEMBER || oldtitle==title )
			return;
		if(title==TITLE_MASTER && (oldtitle!=TITLE_VICEMASTER || mytitle!=TITLE_MASTER))
			return;
		if(title!=TITLE_MASTER && title!=TITLE_VICEMASTER && title!=TITLE_CAPTAIN && title!=TITLE_HEADER)
			return;
		if(title==TITLE_MASTER && KingdomManager::GetInstance()->GetKingFaction()==(int)arg.fid)
		{
			manager->Send(sid, FactionAppoint_Re(ERR_FC_CHANGE_KINGDOM, roleid, candidate, title, localsid, scale));
			return;
		}
		Log::formatlog("factionappoint","factionid=%d:roleid=%d:mytitle=%d:candidate=%d,title=%d",
				arg.fid,roleid,mytitle,candidate,title);

		arg.title = title;
		arg.roleid = candidate;
		int ret = FactionManager::Instance()->FillAppointArg(arg, mytitle);
		if(ret==1)
		{
			manager->Send(sid, FactionAppoint_Re(ERR_FC_FULL, roleid, candidate, title, localsid, scale));
			return;
		}
		else if(ret)
			return;

		DBFactionAppoint* rpc = (DBFactionAppoint*) Rpc::Call( RPC_DBFACTIONAPPOINT,arg);
		rpc->roleid = roleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	void DoFamilyAppoint(Manager *manager, Manager::Session::ID sid)
	{
		if (title!=TITLE_HEADER)
			return;

		DBFamilyAppointArg arg;
		int localsid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);

			if (NULL==pinfo || pinfo->familyid==0 || pinfo->factiontitle != TITLE_HEADER)
				return;
			DEBUG_PRINT("FactionAppoint, roleid=%d, family=%d, title=%d", roleid, pinfo->familyid, pinfo->factiontitle);

			arg.familyid = pinfo->familyid;
			localsid = pinfo->localsid;
		}
		if(!FamilyManager::Instance()->IsMember(arg.familyid, candidate))
			return;

		Log::formatlog("familyappoint","familyid=%d:roleid=%d:candidate=%d,title=%d", arg.familyid,roleid,candidate,title);
		arg.title = title;
		arg.roleid = candidate;
		DBFamilyAppoint* rpc = (DBFamilyAppoint*) Rpc::Call( RPC_DBFAMILYAPPOINT,arg);
		rpc->roleid = roleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);

	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionAppoint, roleid=%d, candidate=%d, title=%d, scale=%d", roleid, candidate, title, scale);
		switch(scale)
		{
		case 0: // faction
			DoFactionAppoint(manager, sid);
			break;
		case 1: // family
			DoFamilyAppoint(manager, sid);
			break;
		}
		
	}
};

};

#endif
