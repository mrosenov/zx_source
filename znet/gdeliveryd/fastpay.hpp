
#ifndef __GNET_FASTPAY_HPP
#define __GNET_FASTPAY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "fastpay_re.hpp"

namespace GNET
{

class FastPay : public GNET::Protocol
{
	#include "fastpay"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("fastpay userid %d cash %d cash_after_discount %d merchant_id %d",
			userid, cash, cash_after_discount, merchant_id);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			Log::log(LOG_ERR, "User(%d) Try to fastpay in centralDS, Refuse him!",
					userid);
			return;
		}
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
		if (NULL == userinfo) 
			return;
		Game2AU req(userid, AU_REQ_FASTPAY, Octets(), 0);
		OctetsStream os;
		os << cash;
		os << cash_after_discount;
		os << merchant_id;
		req.info = os;
		if (!GAuthClient::GetInstance()->SendProtocol(req)) 
			manager->Send(sid, FastPay_Re(4/*ÍøÂçÍ¨ÐÅ´íÎó*/, userid, cash, cash_after_discount, merchant_id, userinfo->localsid));
	}
};

};

#endif
