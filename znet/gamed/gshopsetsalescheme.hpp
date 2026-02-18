
#ifndef __GNET_GSHOPSETSALESCHEME_HPP
#define __GNET_GSHOPSETSALESCHEME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void change_shop_sale(unsigned int new_scheme);

namespace GNET
{

class GShopSetSaleScheme : public GNET::Protocol
{
	#include "gshopsetsalescheme"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		change_shop_sale(scheme);

	}
};

};

#endif
