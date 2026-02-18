
#ifndef __GNET_ACFORBIDUSER_HPP
#define __GNET_ACFORBIDUSER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACForbidUser : public GNET::Protocol
{
	#include "acforbiduser"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
