
#ifndef __GNET_GTTEAMDISMISS_HPP
#define __GNET_GTTEAMDISMISS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTTeamDismiss : public GNET::Protocol
{
	#include "gtteamdismiss"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
