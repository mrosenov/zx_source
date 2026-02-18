
#ifndef __GNET_DELROLEANNOUNCE_HPP
#define __GNET_DELROLEANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "accessdb.h"
#include "waitdelkey"
namespace GNET
{

class DelRoleAnnounce : public GNET::Protocol
{
	#include "delroleannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( this->waitdelist.size() )
			DEBUG_PRINT("delroleannounce received. size=%d\n", waitdelist.size());
		GetTimeoutRole(this->waitdelist);
		manager->Send(sid,this);
	}
};

};

#endif
