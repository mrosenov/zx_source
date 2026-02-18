
#ifndef __GNET_STOCKBOARD_RE_HPP
#define __GNET_STOCKBOARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class StockBoard_Re : public GNET::Protocol
{
	#include "stockboard_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
