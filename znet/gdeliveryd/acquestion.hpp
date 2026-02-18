
#ifndef __GNET_ACQUESTION_HPP
#define __GNET_ACQUESTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"

namespace GNET
{

class ACQuestion : public GNET::Protocol
{
	#include "acquestion"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL != pinfo )
			dsm->Send( pinfo->linksid, this );
	}
};

};

#endif
