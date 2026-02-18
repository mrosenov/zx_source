
#ifndef __GNET_HOSTILEADD_HPP
#define __GNET_HOSTILEADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbhostileadd.hrp"
#include "hostileadd_re.hpp"

namespace GNET
{

class HostileAdd : public GNET::Protocol
{
	#include "hostileadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		return;
		/*
		LOG_TRACE("HostileAdd, roleid=%d, factionid=%d, hostile=%d", roleid, factionid, hostile);
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		int localsid;
		int linksid;
		{
			UserInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid!=(unsigned int)factionid || pinfo->factiontitle>TITLE_VICEMASTER)
				return;
			linksid = pinfo->sid;
			localsid = pinfo->localsid;
		}

		int retcode = FactionManager::Instance()->CanAddHostile(factionid, hostile);
		if (retcode < 0)
			return;
		if (ERR_SUCCESS == retcode)
		{
			DBHostileAddArg arg;
			arg.roleid = roleid;
			arg.factionid = factionid;
			arg.hostile = hostile;

			DBHostileAdd* rpc = (DBHostileAdd*) Rpc::Call( RPC_DBHOSTILEADD, arg);
			rpc->linksid = linksid;
			rpc->localsid = localsid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return;
		}

		GDeliveryServer::GetInstance()->Send(linksid, HostileAdd_Re(retcode, factionid, hostile, localsid));
		*/
	}
};

};

#endif
