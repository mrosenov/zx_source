
#ifndef __GNET_STOCKTRANSITION_HPP
#define __GNET_STOCKTRANSITION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class StockTransition : public GNET::Protocol
{
	#include "stocktransition"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
