
#ifndef __GNET_GKINGTRYCALLGUARDS_HPP
#define __GNET_GKINGTRYCALLGUARDS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingTryCallGuards : public GNET::Protocol
{
	#include "gkingtrycallguards"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
