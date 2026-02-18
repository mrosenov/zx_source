
#ifndef __GNET_GCRSSVRTEAMSPOSTSCORE_HPP
#define __GNET_GCRSSVRTEAMSPOSTSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GCrssvrTeamsPostScore : public GNET::Protocol
{
	#include "gcrssvrteamspostscore"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
