
#ifndef __GNET_GSHOPSETSALESCHEME_HPP
#define __GNET_GSHOPSETSALESCHEME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gproviderserver.hpp"
#include "gdeliveryserver.hpp"
#include "gshopnotifyscheme.hpp"
#include "maplinkserver.h"

namespace GNET
{

class GShopSetSaleScheme : public GNET::Protocol
{
	#include "gshopsetsalescheme"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		GProviderServer::GetInstance()->BroadcastProtocol(this);
		dsm->gshop_scheme.sale = scheme;
		LinkServer::GetInstance().BroadcastProtocol(GShopNotifyScheme(dsm->gshop_scheme.discount, scheme));
		LOG_TRACE("GShopSetSaleScheme: sale=0x%x\n", scheme);
	}
};

};

#endif
