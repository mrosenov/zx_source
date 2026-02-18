
#ifndef __GNET_QUERYROLENAMEHISTORY_HPP
#define __GNET_QUERYROLENAMEHISTORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryRolenameHistory : public GNET::Protocol
{
	#include "queryrolenamehistory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		std::vector<NameHistory> list;
		if (NameManager::GetInstance()->GetNameHistory(name, list))
		{
			manager->Send(sid, QueryRolenameHistory_Re(ERR_SUCCESS, name, localsid, list));
			return;
		}
		DBQueryRoleName * rpc = (DBQueryRoleName *) Rpc::Call(RPC_DBQUERYROLENAME, DBQueryRoleNameArg(name));
		rpc->linksid = sid;
		rpc->localsid = localsid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
