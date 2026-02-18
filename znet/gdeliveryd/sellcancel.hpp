
#ifndef __GNET_SELLCANCEL_HPP
#define __GNET_SELLCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "cardmarket.h"
#include "dbsellcancel.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
#include "gdeliveryserver.hpp"
namespace GNET
{

class SellCancel : public GNET::Protocol
{
	#include "sellcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
