
#ifndef __GNET_UNDODELETEROLE_HPP
#define __GNET_UNDODELETEROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "undodeleterole_re.hpp"
#include "dbundodeleterole.hrp"
#include "mapuser.h"
namespace GNET
{

class UndoDeleteRole : public GNET::Protocol
{
	#include "undodeleterole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		int userid = UserContainer::Roleid2Userid(roleid);
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		UserInfo* user = UserContainer::GetInstance().FindUser(userid);
		if (!user || user->linksid!=sid || user->localsid!=localsid)
			return;
		if ( user->rolelist.IsRoleExist(roleid) && RoleInfoCache::Instance().IsRoleDeleted(roleid))
		{
			DBUndoDeleteRole* rpc=(DBUndoDeleteRole*) Rpc::Call(RPC_DBUNDODELETEROLE,DBUndoDeleteRoleArg(roleid));
			rpc->userid = user->userid;
			if (!GameDBClient::GetInstance()->SendProtocol(rpc))
			{
				dsm->Send(sid,UndoDeleteRole_Re(ERR_UNDODELROLE,roleid,localsid));
			}
		}
		else
		{
			dsm->Send(sid,UndoDeleteRole_Re(ERR_UNDODELROLE,roleid,localsid));
		}
	}
};

};

#endif
