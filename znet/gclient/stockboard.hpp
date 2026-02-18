
#ifndef __GNET_STOCKBOARD_HPP
#define __GNET_STOCKBOARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class StockBoard : public GNET::Protocol
{
	#include "stockboard"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
