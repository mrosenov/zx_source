
#ifndef __GNET_FASTPAY_HPP
#define __GNET_FASTPAY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FastPay : public GNET::Protocol
{
	#include "fastpay"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo || !sinfo->policy.Update(FAST_PAY))
			return;
		if (!GLinkServer::GetInstance()->ValidUser(sid, userid)) 
		{
			Log::log(LOG_ERR, "fastpay userid %d invalid, sid %d", userid, sid);
			return;
		}
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
