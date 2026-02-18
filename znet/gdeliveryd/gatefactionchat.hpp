
#ifndef __GNET_GATEFACTIONCHAT_HPP
#define __GNET_GATEFACTIONCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformagent.h"

namespace GNET
{

class GateFactionChat : public GNET::Protocol
{
	#include "gatefactionchat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateFactionChat: fid=%d src=%d channel=%d",fid,src,channel);
		GTPlatformAgent::OnGateFactionChat(*this);
	}
};

};

#endif
