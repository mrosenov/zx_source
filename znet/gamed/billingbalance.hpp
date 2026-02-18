
#ifndef __GNET_BILLINGBALANCE_HPP
#define __GNET_BILLINGBALANCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_billing_cash_notify(int role_id, int total_cash);
namespace GNET
{

class BillingBalance : public GNET::Protocol
{
	#include "billingbalance"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!result)
			player_billing_cash_notify(userid, balance);  // Delivery send roleid 
	}
};

};

#endif
