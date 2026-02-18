
#ifndef __GNET_LOGSTOCKBALANCE_HPP
#define __GNET_LOGSTOCKBALANCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LogStockBalance : public GNET::Protocol
{
	#include "logstockbalance"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
