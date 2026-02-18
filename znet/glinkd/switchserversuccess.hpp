
#ifndef __GNET_SWITCHSERVERSUCCESS_HPP
#define __GNET_SWITCHSERVERSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "log.h"
#include "glinkserver.hpp"
#include "playerstatussync.hpp"
#include "gproviderserver.hpp"
namespace GNET
{

class SwitchServerSuccess : public GNET::Protocol
{
	#include "switchserversuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
