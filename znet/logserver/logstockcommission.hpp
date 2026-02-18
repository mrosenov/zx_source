
#ifndef __GNET_LOGSTOCKCOMMISSION_HPP
#define __GNET_LOGSTOCKCOMMISSION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "stockorder"

namespace GNET
{

class LogStockCommission : public GNET::Protocol
{
	#include "logstockcommission"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
