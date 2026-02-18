
#ifndef __GNET_DISCOUNTANNOUNCE_HPP
#define __GNET_DISCOUNTANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "merchantdiscount"

namespace GNET
{

class DiscountAnnounce : public GNET::Protocol
{
	#include "discountannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("discountannounce discount.size %d", discount.size());
		this->reserved1 = 0; //0 表示需要广播
		GDeliveryServer::GetInstance()->fastpay_discounts = discount;
		LinkServer::GetInstance().BroadcastProtocol(this);
	}
};

};

#endif
