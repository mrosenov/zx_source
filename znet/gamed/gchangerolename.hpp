
#ifndef __GNET_GCHANGEROLENAME_HPP
#define __GNET_GCHANGEROLENAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GChangeRolename : public GNET::Protocol
{
	#include "gchangerolename"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
