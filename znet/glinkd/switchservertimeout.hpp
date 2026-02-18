
#ifndef __GNET_SWITCHSERVERTIMEOUT_HPP
#define __GNET_SWITCHSERVERTIMEOUT_HPP

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

class SwitchServerTimeout : public GNET::Protocol
{
	#include "switchservertimeout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
