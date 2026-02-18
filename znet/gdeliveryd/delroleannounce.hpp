
#ifndef __GNET_DELROLEANNOUNCE_HPP
#define __GNET_DELROLEANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "accountdelrole.hrp"
#include "groledbclient.hpp"
#include "gdeliveryserver.hpp"
#include "uniquenameclient.hpp"
#include "waitdelkey"
namespace GNET
{

class DelRoleAnnounce : public GNET::Protocol
{
	#include "delroleannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( !UniqueNameClient::GetInstance()->IsConnect() )
			return;
		DeleteHelper::Instance()->AddDeleteList(waitdelist);
	}
};

};

#endif
