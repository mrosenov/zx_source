
#ifndef __GNET_FACTIONMULTIEXPSYNC_HPP
#define __GNET_FACTIONMULTIEXPSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionMultiExpSync : public GNET::Protocol
{
	#include "factionmultiexpsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
