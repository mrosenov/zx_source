
#ifndef __GNET_REMOVEROLE_HPP
#define __GNET_REMOVEROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtplatformclient.hpp"
namespace GNET
{

class RemoveRole : public GNET::Protocol
{
	#include "removerole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("RemoveRole roleid=%lld",roleid);
		GTPlatformClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
