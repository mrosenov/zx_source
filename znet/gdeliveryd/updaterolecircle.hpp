
#ifndef __GNET_UPDATEROLECIRCLE_HPP
#define __GNET_UPDATEROLECIRCLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class UpdateRoleCircle : public GNET::Protocol
{
	#include "updaterolecircle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
