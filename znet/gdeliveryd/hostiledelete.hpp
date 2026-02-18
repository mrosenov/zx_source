
#ifndef __GNET_HOSTILEDELETE_HPP
#define __GNET_HOSTILEDELETE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbhostiledelete.hrp"

namespace GNET
{

class HostileDelete : public GNET::Protocol
{
	#include "hostiledelete"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		return;
		/*
		LOG_TRACE("GAddHostile, roleid=%d, factionid=%d, hostile=%d", roleid, factionid, hostile);
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			UserInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid!=(unsigned int)factionid || pinfo->factiontitle>TITLE_VICEMASTER)
				return;
		}	
		int retcode =  FactionManager::Instance()->CanDeleteHostile(factionid, hostile);
		if (retcode < 0)
			return;
		if (ERR_SUCCESS == retcode)
		{
			DBHostileDeleteArg arg;
			arg.factionid = factionid;
			arg.hostile.push_back(hostile);
			DBHostileDelete* rpc = (DBHostileDelete*) Rpc::Call( RPC_DBHOSTILEDELETE, arg);
			rpc->sendclient = true;
			rpc->localsid = localsid;
			rpc->linksid = sid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return;
		}
		manager->Send(sid, HostileDelete_Re(retcode, factionid, hostile, localsid));
		*/

	}
};

};

#endif
