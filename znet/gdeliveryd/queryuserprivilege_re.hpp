
#ifndef __GNET_QUERYUSERPRIVILEGE_RE_HPP
#define __GNET_QUERYUSERPRIVILEGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "announcegm.hpp"
#include "gproviderserver.hpp"
#include "gdeliveryserver.hpp"

#include "privilege.hxx"
#include "maplinkserver.h"
#include "mapuser.h"
namespace GNET
{

class QueryUserPrivilege_Re : public GNET::Protocol
{
	#include "queryuserprivilege_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int roleid;
		int status;
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
			if (NULL==pinfo) return;
			roleid=pinfo->roleid;
			status=pinfo->status;
			pinfo->privileges = auth;
		}
		//send user's privilege to all linkserver
		LinkServer::GetInstance().BroadcastProtocol( this );
	}
};

};

#endif
