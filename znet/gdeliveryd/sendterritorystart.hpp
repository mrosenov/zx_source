
#ifndef __GNET_SENDTERRITORYSTART_HPP
#define __GNET_SENDTERRITORYSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendTerritoryStart : public GNET::Protocol
{
	#include "sendterritorystart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
