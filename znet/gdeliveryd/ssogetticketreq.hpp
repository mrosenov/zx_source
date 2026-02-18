
#ifndef __GNET_SSOGETTICKETREQ_HPP
#define __GNET_SSOGETTICKETREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userssoinfo"
#include "ssogetticketrep.hpp"
#include "getcouponsreq.hpp"
#include "gproviderserver.hpp"
#include "mapuser.h"

namespace GNET
{

class SSOGetTicketReq : public GNET::Protocol
{
	#include "ssogetticketreq"
	bool IsCouponType(const Octets &context)
	{
		bool ret = false;
		if(context.size() == 0)
		      return ret;

		string type((char *)context.begin(), context.size());
		//type.insert(type.end(), (char *)context.begin(), (char *)context.end());
		if(type == "webhot")
		{
			ret = true;
		}
		return ret;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("ssogetticketreq userid %lld toaid %d tozoneid %d info.size %d context.size %d reserved.size %d",
					user.userid, toaid, tozoneid, info.size(), local_context.size(), reserved.size());
		int userid = static_cast<int>(user.userid);
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
		if (NULL == userinfo) 
			return;
		// 在活动期间，需要检查角色拥有的兑换券的数量
		// 并且由于跨服没有连AU，所以只有原服才能检查兑换券
		Coupon *coupon = Coupon::GetInstance();
		if(IsCouponType(local_context) && coupon->NeedCheckCoupon() && !GDeliveryServer::GetInstance()->IsCentralDS())
		{
			coupon->InsertLocalContext(userid, local_context);
			GetCouponsReq req;
			req.userid = userid;
			req.toaid = toaid;
			req.tozoneid = tozoneid;
			req.roleid = userinfo->roleid;
			coupon->GetCoupons(req.itemids);
			GProviderServer::GetInstance()->DispatchProtocol(userinfo->gameid, req);
			return;
		}
		this->user = userinfo->ssoinfo;
		this->loginip = userinfo->ip;
		if (!GAuthClient::GetInstance()->SendProtocol(this))
		{
			OctetsStream os;
			os << userinfo->localsid;
			manager->Send(sid, SSOGetTicketRep(5/*网络通信错误*/, userinfo->ssoinfo, Octets(), local_context, os));
		}
	}
};

};

#endif
