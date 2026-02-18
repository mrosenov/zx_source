
#ifndef __GNET_FACTIONEXPEL_HPP
#define __GNET_FACTIONEXPEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbfactionleave.hrp"

namespace GNET
{

class FactionExpel : public GNET::Protocol
{
	#include "factionexpel"

	void ExpelFaction(Manager *manager, Manager::Session::ID sid)
	{
		int contribution;
		if(FactionManager::Instance()->CanExpel(roleid, factionid, expellee, contribution))
		{
			DBFactionLeaveArg arg(factionid, expellee);
			arg.contribution = contribution;
			DBFactionLeave* rpc = (DBFactionLeave*) Rpc::Call( RPC_DBFACTIONLEAVE,arg);
			rpc->master = roleid;
			rpc->sendclient = true;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * p = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (p)
				manager->Send(sid,FactionExpel_Re(ERR_PERMISSION_DENIED,roleid,0,expellee,
						Octets(),scale,p->localsid));
		}
	}

	void ExpelFamily(Manager *manager, Manager::Session::ID sid)
	{
		if(FamilyManager::Instance()->CanExpel(roleid, factionid, expellee))
		{
			DBFamilyLeaveArg arg(factionid, expellee);
			DBFamilyLeave* rpc = (DBFamilyLeave*) Rpc::Call( RPC_DBFAMILYLEAVE,arg);
			rpc->master = roleid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * p = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (p)
				manager->Send(sid,FactionExpel_Re(ERR_PERMISSION_DENIED,roleid,0,expellee,
						Octets(),scale,p->localsid));
		}
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("FactionExpel", "roleid=%d,factionid=%d,expellee=%d,scale=%d",roleid,factionid,expellee,scale);
		switch(scale)
		{
		case 0:
			ExpelFaction(manager, sid);
			break;
		case 1:
			ExpelFamily(manager, sid);
			break;
		}
	}
};

};

#endif
