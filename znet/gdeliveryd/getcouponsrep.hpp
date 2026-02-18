
#ifndef __GNET_GETCOUPONSREP_HPP
#define __GNET_GETCOUPONSREP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCouponsRep : public GNET::Protocol
{
	#include "getcouponsrep"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("GetCouponsRep userid=%d toaid=%d tozoneid=%d coupons.size=%d", userid, toaid, tozoneid, coupons.size());
		UserInfo *userinfo = UserContainer::GetInstance().FindUser(userid);
		if(userinfo == NULL)
		      return;

		SSOGetTicketReq req;
		req.user = userinfo->ssoinfo;
		req.loginip = userinfo->ip;
		req.toaid = toaid;
		req.tozoneid = tozoneid;
		GRoleInfo *info = RoleInfoCache::Instance().GetOnlist(roleid);
		if(info == NULL)
		      return;
		OctetsStream os;
		int zoneid = GDeliveryServer::GetInstance()->zoneid;
		os << roleid;
		os << info->name;
		os << info->level;
		os << info->create_time;
		os << coupons;
		os << zoneid;
		req.info = os;
		Coupon *coupon = Coupon::GetInstance();
		coupon->GetLocalContext(userid, req.local_context);
		coupon->EraseLocalContext(userid);

		if (!GAuthClient::GetInstance()->SendProtocol(req))
		{
			OctetsStream os;
			os << userinfo->localsid;
			GDeliveryServer::GetInstance()->Send(userinfo->linksid, SSOGetTicketRep(5/*ÍøÂçÍ¨ÐÅ´íÎó*/, userinfo->ssoinfo, Octets(), Octets(), os));
		}
		LOG_TRACE("Send SSOGetTicketReq to Au roleid=%d, coupons.size=%d, zoneid=%d, aid=%d", roleid, coupons.size(), zoneid, toaid);
	}
};

};

#endif
