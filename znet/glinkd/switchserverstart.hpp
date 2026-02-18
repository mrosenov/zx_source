
#ifndef __GNET_SWITCHSERVERSTART_HPP
#define __GNET_SWITCHSERVERSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gproviderserver.hpp"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "log.h"

#include "switchservercancel.hpp"
#include "playerstatussync.hpp"
namespace GNET
{
class SwitchServerStart : public GNET::Protocol
{
	#include "switchserverstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
