
#ifndef __GNET_GSHOPSETDISCOUNTSCHEME_HPP
#define __GNET_GSHOPSETDISCOUNTSCHEME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gshopnotifyscheme.hpp"
#include "maplinkserver.h"

namespace GNET
{

class GShopSetDiscountScheme : public GNET::Protocol
{
	#include "gshopsetdiscountscheme"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		GProviderServer::GetInstance()->BroadcastProtocol(this);
		dsm->gshop_scheme.discount = scheme;
		LinkServer::GetInstance().BroadcastProtocol(GShopNotifyScheme(scheme, dsm->gshop_scheme.sale));
		LOG_TRACE("GShopSetDiscountScheme: discount=0x%x\n", scheme);
	}
};

};

#endif
