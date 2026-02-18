
#ifndef __GNET_ACREMOTECODE_HPP
#define __GNET_ACREMOTECODE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
namespace GNET
{

class ACRemoteCode : public GNET::Protocol
{
	#include "acremotecode"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( dstroleid );
		if ( NULL != pinfo )
			dsm->Send( pinfo->linksid, this );
	}
};

};

#endif
