
#ifndef __GNET_REFWITHDRAWEXP_HPP
#define __GNET_REFWITHDRAWEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RefWithdrawExp : public GNET::Protocol
{
	#include "refwithdrawexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid, roleid))
			return; 

		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
