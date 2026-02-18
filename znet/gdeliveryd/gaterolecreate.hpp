
#ifndef __GNET_GATEROLECREATE_HPP
#define __GNET_GATEROLECREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ggateroleinfo"

namespace GNET
{

class GateRoleCreate : public GNET::Protocol
{
	#include "gaterolecreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
