
#ifndef __GNET_GSHOPSETDISCOUNTSCHEME_HPP
#define __GNET_GSHOPSETDISCOUNTSCHEME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void change_shop_discount(unsigned int new_discount);

namespace GNET
{

class GShopSetDiscountScheme : public GNET::Protocol
{
	#include "gshopsetdiscountscheme"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		change_shop_discount(scheme);
	}
};

};

#endif
