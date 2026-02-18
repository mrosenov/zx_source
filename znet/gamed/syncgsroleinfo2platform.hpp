
#ifndef __GNET_SYNCGSROLEINFO2PLATFORM_HPP
#define __GNET_SYNCGSROLEINFO2PLATFORM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gsnsgsroleinfo"

namespace GNET
{

class SyncGsRoleInfo2Platform : public GNET::Protocol
{
	#include "syncgsroleinfo2platform"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
