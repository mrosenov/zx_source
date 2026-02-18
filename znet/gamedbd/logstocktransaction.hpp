
#ifndef __GNET_LOGSTOCKTRANSACTION_HPP
#define __GNET_LOGSTOCKTRANSACTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class LogStockTransaction : public GNET::Protocol
{
	#include "logstocktransaction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
