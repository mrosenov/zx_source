
#ifndef __GNET_GKINGDOMPOINTCHANGE_HPP
#define __GNET_GKINGDOMPOINTCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomPointChange : public GNET::Protocol
{
	#include "gkingdompointchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
