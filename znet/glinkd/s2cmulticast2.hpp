
#ifndef __GNET_S2CMULTICAST2_HPP
#define __GNET_S2CMULTICAST2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "player"
#include "gamedatasend.hpp"
#include "glinkserver.hpp"

namespace GNET
{

class S2CMulticast2 : public GNET::Protocol
{
	#include "s2cmulticast2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GamedataSend gds(data);
		for(size_t i=0;i < playerlist.GetVector().size(); ++i)
		{
			GLinkServer::GetInstance()->Send( playerlist[i].localsid, gds);
		}
	}
};

};

#endif
