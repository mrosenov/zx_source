
#ifndef __GNET_FACTIONANNOUNCE_HPP
#define __GNET_FACTIONANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionannounce_re.hpp"

namespace GNET
{

class FactionAnnounce : public GNET::Protocol
{
	#include "factionannounce"
	void FactionUpdate(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionAnnounce, roleid=%d, factionid=%d, size=%d", roleid, factionid, announce.size());
		DBFactionUpdateArg arg;
		int localsid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid !=factionid || pinfo->factiontitle>TITLE_VICEMASTER) 
				return;
			localsid = pinfo->localsid;
		}
		if(announce.size()>100)
		{
			manager->Send(sid, FactionAnnounce_Re(ERR_FC_INVALIDNAME,roleid, Octets(),localsid, scale));
			return;
		}
		
		arg.reason = 0;
		if(!FactionManager::Instance()->FillUpdateArg(factionid,roleid,arg))
			return;
		arg.announce.swap(announce);
		DBFactionUpdate* rpc = (DBFactionUpdate*) Rpc::Call( RPC_DBFACTIONUPDATE,arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	void FamilyUpdate(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionAnnounce, roleid=%d, familyid=%d, size=%d", roleid, factionid, announce.size());
		DBFamilyUpdateArg arg;
		int localsid;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->familyid !=factionid || pinfo->factiontitle == TITLE_MEMBER) 
				return;
			localsid = pinfo->localsid;
		}
		if(announce.size()>100)
		{
			manager->Send(sid, FactionAnnounce_Re(ERR_FC_INVALIDNAME,roleid, Octets(),localsid,scale));
			return;
		}
		
		arg.announce.swap(announce);
		arg.familyid = factionid;
		arg.rid = roleid;
		DBFamilyUpdate* rpc = (DBFamilyUpdate*) Rpc::Call( RPC_DBFAMILYUPDATE,arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionAnnounce, roleid=%d, fid=%d, scale=%d, size=%d", roleid, factionid, scale, announce.size());
		switch(scale)
		{
		case 0 : // faction
			FactionUpdate(manager, sid);
			break;
		case 1 : // family
			FamilyUpdate(manager, sid);
			break;
		}
	}
};

};

#endif
