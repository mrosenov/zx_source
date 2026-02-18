
#ifndef __GNET_AU2GAME_HPP
#define __GNET_AU2GAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "flowertoprecord"
#include "topflowermsg_re.hpp"

namespace GNET
{
	/*
				struct TopKey
				{
					int number;
					int64_t timestamp;
					TopKey(int n, int t) : number(n), timestamp(t) {}
					bool operator < ( const TopKey & rhs ) const
					{
						return number > rhs.number || number == rhs.number && timestamp < rhs.timestamp;
					} 
				};
				struct TopValue
				{
					int userid;
					Octets rolename;
					TopValue( int u, const Octets & r) : userid(u), rolename(r) {}
				};
	*/

class AU2Game : public GNET::Protocol
{
	#include "au2game"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("au2game userid %d qtype %d info.size %d ret %d",
			userid, qtype, info.size(), retcode);
		//通知客户端之前要验证玩家是否在游戏中
		switch(qtype)
		{
		case AU_REQ_FASTPAY:
			{
				UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
				if (userinfo == NULL ||	userinfo->status != _STATUS_ONGAME) 
					return;
				FastPay_Re re(retcode, userid);
				Marshal::OctetsStream os(info);
				try
				{
					os >> re.cash;
					os >> re.cash_after_discount;
					os >> re.merchant_id;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, userid %d qtype %d ret %d",
							info.size(), userid, qtype, retcode);
					return;
				}
				re.localsid = userinfo->localsid;
				GDeliveryServer::GetInstance()->Send(userinfo->linksid, re); 
			}
			break;
		case AU_REQ_FASTPAY_BINDINFO:
			{
				UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
				if (NULL == userinfo) 
					return;
				FastPayBindInfo notice;
				Marshal::OctetsStream os(info);
				try
				{
					os >> notice.merchantlist;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, userid %d qtype %d ret %d",
							info.size(), userid, qtype, retcode);
					return;
				}
				userinfo->merchantlist = notice.merchantlist;
				if (userinfo->status == _STATUS_ONGAME)
				{
					notice.userid = userid;
					notice.localsid = userinfo->localsid;
					GDeliveryServer::GetInstance()->Send(userinfo->linksid, notice); 
				}
			}
			break;
		case AU_REQ_FLOWER_TOP:
			{
				int activitytype = 0; //0 送花榜 1 收花榜
				int reserved1 = 0;
				Octets reserved2;
				Marshal::OctetsStream os(info);
				try
				{
					os >> activitytype >> reserved1 >> reserved2;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, qtype %d ",
							info.size(), qtype);
					return;
				}
				if (activitytype == WEB_ACT_GET_SEND_TOP ||
						activitytype == WEB_ACT_GET_RECV_TOP)
				{
					Game2AU re(userid, qtype, Octets(), 0);
					std::vector<FlowerTopRecord> toplist;
					TopFlowerManager::Instance()->GetTopForWeb(activitytype, toplist);
					OctetsStream re_os, top_os;
					top_os << toplist;
					re_os << activitytype << top_os;
					re.info = re_os;
					GAuthClient::GetInstance()->SendProtocol(re);
				}
				else if (activitytype == WEB_ACT_GET_CASH_AVAIL)
				{
					int cash_userid = reserved1;
					int cash_roleid = 0;
					bool ongame = false;
					UserInfo * userinfo = UserContainer::GetInstance().FindUser(cash_userid);
					if (userinfo)
					{
						ongame = (userinfo->gameid > 0);
						cash_roleid = userinfo->roleid;
					}
					GetCashAvail * rpc = (GetCashAvail *)Rpc::Call(RPC_GETCASHAVAIL, GetCashAvailArg(cash_roleid, cash_userid, 0));
					if (ongame)
						GProviderServer::GetInstance()->DispatchProtocol(userinfo->gameid, rpc);
					else
						GameDBClient::GetInstance()->SendProtocol(rpc);
					LOG_TRACE("au2game get cash_available, userid %d ongame %d roleid %d", cash_userid, ongame, cash_roleid);
				}
				else
				{
					Log::log(LOG_ERR, "au2game qtype %d invalid activitytype %d", qtype, activitytype);
					return;
				}
			}
			break;
		case AU_REQ_BROADCAST:
			{
				int zoneid;
				int district_id;
				int recv_roleid;
				Octets recv_rolename;
				Octets send_rolename;
				Octets lover_msg; // 爱情宣言
				int add_count;

				Marshal::OctetsStream os(info);
				try
				{
					os >> recv_rolename >> send_rolename >> lover_msg >> add_count >> recv_roleid >> district_id >> zoneid;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, qtype %d ",
							info.size(), qtype);
					return;
				}


				// 通知或广播
				TopFlowerMsg_Re _msg_re;
				_msg_re.district_id = district_id;
				_msg_re.zoneid = zoneid;
				_msg_re.recv_rolename = recv_rolename;
				_msg_re.send_rolename = send_rolename;
				_msg_re.msg = lover_msg;
				_msg_re.count = add_count;

				LOG_TRACE("send flower top broadcast, district %d, zoneid %d", _msg_re.district_id, _msg_re.zoneid);

				// 本服广播
				_msg_re.myself_roleid = recv_roleid;
				LinkServer::GetInstance().BroadcastProtocol(_msg_re);
			}
			break;
		case AU_REQ_TOUCH_POINT:
			{
				int64_t income = 0;
				int64_t remain = 0;
				Marshal::OctetsStream os(info);
				try
				{
					os >> income >> remain;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, qtype %d ",
							info.size(), qtype);
					return;
				}
				LOG_TRACE("user %d get touch point income %lld  remain %lld ret %d", userid, income, remain, retcode);
				UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
				if (userinfo == NULL ||	userinfo->status != _STATUS_ONGAME) 
					return;
				if (retcode == 0)
					userinfo->touchpoint = remain;
				GDeliveryServer::GetInstance()->Send(userinfo->linksid,
						GetTouchPoint_Re(retcode==0?ERR_TOUCH_SUCCESS:ERR_TOUCH_COMMUNICATION, 0, userinfo->localsid, remain));
			}
			break;
		case AU_REQ_PASSPORT_CASHADD:
			{
				int64_t addupmoney = 0;
				Marshal::OctetsStream os(info);
				try
				{
					os >> addupmoney;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, qtype %d ",
							info.size(), qtype);
					return;
				}
				LOG_TRACE("user %d getpassportcashadd %lld ret %d", userid, addupmoney, retcode);
				UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
				if (userinfo == NULL ||	userinfo->status != _STATUS_ONGAME) 
					return;
				GDeliveryServer::GetInstance()->Send(userinfo->linksid,
						GetPassportCashAdd_Re(retcode==0?ERR_TOUCH_SUCCESS:ERR_TOUCH_COMMUNICATION, 0, userinfo->localsid, addupmoney));
			}
			break;
		case AU_REQ_TOUCH_POINT_EXCHG:
			{
				int64_t orderid, income, remain;
				orderid = income = remain = 0;
				int number, reserved;
				number = reserved = 0;
				Octets context;
				Marshal::OctetsStream os(info);
				try
				{
					os >> orderid >> number >> context >> income >> remain >>reserved;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal error, qtype %d ",
							info.size(), qtype);
					return;
				}
				int roleid, flag, count;
				roleid = flag = count = 0;
				try
				{
					Marshal::OctetsStream(context) >> roleid >> flag >> count;
				}
				catch ( Marshal::Exception & )
				{
					Log::log(LOG_ERR, "au2game info.size %d unmarshal context error, qtype %d ",
							context.size(), qtype);
					return;
				}
				Log::formatlog("touchpointexchange", "recv from au, roleid %d userid %d order %lld flag %d count %d points_need %d new_points %lld ret %d", roleid, userid, orderid, flag, count, number, remain, retcode);
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
				if( NULL == pinfo )
					return;
				retcode = (retcode==0?ERR_TOUCH_SUCCESS:ERR_TOUCH_EXCHG_FAIL);
				TouchPointExchange_Re client_re(retcode, roleid, flag, count, remain, pinfo->localsid);
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, client_re);
				GTouchPointExchange_Re gs_re(retcode, roleid, flag, count);
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, gs_re);
			}
			break;
		default:
			break;
		}
	}
}; };

#endif
