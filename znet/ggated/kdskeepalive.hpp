
#ifndef __GNET_KDSKEEPALIVE_HPP
#define __GNET_KDSKEEPALIVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "tokengenerator.h"

namespace GNET
{

class KDSKeepAlive : public GNET::Protocol
{
	#include "kdskeepalive"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("KDSKeepAlive recv code=%d",code);
		TokenGenerator::GetInstance().OnKDSKeepAlive();
	}
};

};

#endif
