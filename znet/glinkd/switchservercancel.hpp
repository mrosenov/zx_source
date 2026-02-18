
#ifndef __GNET_SWITCHSERVERCANCEL_HPP
#define __GNET_SWITCHSERVERCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "log.h"
#include "glinkserver.hpp"
#include "gproviderserver.hpp"
#include "playerstatussync.hpp"
#include "gproviderserver.hpp"
namespace GNET
{

class SwitchServerCancel : public GNET::Protocol
{
	#include "switchservercancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
