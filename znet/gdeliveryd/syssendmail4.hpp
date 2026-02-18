
#ifndef __GNET_SYSSENDMAIL4_HPP
#define __GNET_SYSSENDMAIL4_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "webmallfunction"

namespace GNET
{

class SysSendMail4 : public GNET::Protocol
{
	#include "syssendmail4"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("syssendmail4", "paytype %d orderid %lld userid %d roleid %lld rolename.size %d func.size %d", paytype, orderid, userid, roleid, rolename.size(), functions.size());
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			Log::log(LOG_ERR, "receive syssendmail4 in cross server, refuse it ! orderid = %lld", orderid);
			return;
		}
		int roleid32 = (int)roleid;
		SysSendMail4_Re re(paytype, orderid, ERR_SUCCESS, userid, roleid);
//		if (paytype != WEB_ORDER_PAY_CASH || orderid <= 0 || userid <= 0 || (roleid32<=0 && rolename.size()==0) || functions.size() != 1) 目前不支持通过 rolename 下单
		if (paytype != WEB_ORDER_PAY_CASH || orderid <= 0 || userid <= 0 || roleid32<=0 || functions.size() != 1)
		{
			re.retcode = ERR_WEBORDER_ARG_INVALID;
			GAuthClient::GetInstance()->SendProtocol(re);
			return;
		}
		if (!GameDBClient::GetInstance()->IsConnect())
		{
			re.retcode = ERR_WEBORDER_COMMUNICATION;
			GAuthClient::GetInstance()->SendProtocol(re);
			return;
		}
		int money_need = 0;
		WebMallFunctionVector::const_iterator it, ite = functions.end();
		for (it = functions.begin(); it != ite; ++it)
		{
			if (it->count <= 0 || it->price <= 0 || it->name.size()==0)
			{
				Log::log(LOG_ERR, "syssendmail4, orderid %lld userid %d func.count %d func.price %d name.size %d", orderid, userid, it->count, it->price, it->name.size());
				re.retcode = ERR_WEBORDER_ARG_INVALID;
				GAuthClient::GetInstance()->SendProtocol(re);
				return;
			}
			money_need += (it->count * it->price);
		}
		bool ongame = false;
		int ongame_roleid = 0;
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
		if (userinfo)
		{
			ongame = (userinfo->gameid > 0);
			ongame_roleid = userinfo->roleid;
		}
		DBSaveWebOrderArg order(paytype, orderid, userid, roleid32, rolename, functions, money_need, ongame);
		if (paytype == WEB_ORDER_PAY_CASH && ongame) //有角色在线 先询问 gs 元宝是否充足
		{
			LOG_TRACE("syssendmail orderid %lld userid %d send cash query to gs %d",
					orderid, userid, userinfo->gameid);
			GetCashAvail * rpc = (GetCashAvail *)Rpc::Call(RPC_GETCASHAVAIL, GetCashAvailArg(ongame_roleid, userid, 1));
			rpc->order = order;
			GProviderServer::GetInstance()->DispatchProtocol(userinfo->gameid, rpc);
			return;
		}
		LOG_TRACE("syssendmail orderid %lld userid %d send to db", orderid, userid);
		DBSaveWebOrder * rpc = (DBSaveWebOrder *)Rpc::Call(RPC_DBSAVEWEBORDER, order);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
