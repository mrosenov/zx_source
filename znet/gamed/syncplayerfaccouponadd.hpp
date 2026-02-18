
#ifndef __GNET_SYNCPLAYERFACCOUPONADD_HPP
#define __GNET_SYNCPLAYERFACCOUPONADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SyncPlayerFacCouponAdd : public GNET::Protocol
{
	#include "syncplayerfaccouponadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
