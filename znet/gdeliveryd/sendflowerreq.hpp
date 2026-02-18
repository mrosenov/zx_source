
#ifndef __GNET_SENDFLOWERREQ_HPP
#define __GNET_SENDFLOWERREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "dbflowergetroleid.hrp"
#include "dbflowergetroleexist.hrp"
#include "flowertakeoff.hrp"
#include "topflower_err.hpp"

namespace GNET
{

class SendFlowerReq : public GNET::Protocol
{
	#include "sendflowerreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("sendflowerreq recv send_roleid:%d, dest_roleid:%d", send_roleid, dest_roleid);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			LOG_TRACE("send_roleid(%d) Try to SendFlowerReq in centralDS, Refuse him!",
					send_roleid);
			return;
		}
		
		if(count <= 0)
		{
			LOG_TRACE("roleid:%d, count <= 0", send_roleid);
			return;
		}

		if(TopFlowerManager::Instance()->GetStatus() != TOPFLOWER_READY)
		{
			DEBUG_PRINT("SendFlowerReq  TOPFLOWER_NOT_READY");
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_TOP_NOTREADY,localsid));
			return;
		}
		
		if(!TopFlowerManager::Instance()->CheckOpenTop())
		{
			DEBUG_PRINT("SendFlowerReq  TOPFLOWER_NOT_READY");
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_TOP_NOTREADY, localsid));
			return;
		}

		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pSendInfo = UserContainer::GetInstance().FindRoleOnline((send_roleid));
		if( NULL == pSendInfo )
		{
			DEBUG_PRINT("SendFlowerReq pSendInfo is NULL, send_roleid:%d\n", send_roleid );
			return;
		}
		GRoleInfo* pSendRoleInfo = RoleInfoCache::Instance().Get(send_roleid);
		if(NULL == pSendRoleInfo)
		{
			LOG_TRACE("SendFlowerReq, pSendRoleInfo is NULL, send_roleid:%d\n", send_roleid);
			return;
		}

		if(dest_roleid == 0)// 客户端没有得到目标的roleid
		{
			LOG_TRACE("SendFlowerReq rolename:%s, size=%d.", rolename.begin() ,rolename.size());
			if(rolename.size() <= 0)
			{
				manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_NAME_ERR, localsid));
				return;
			}

			DBFlowerGetRoleId* rpc = (DBFlowerGetRoleId*) Rpc::Call(RPC_DBFLOWERGETROLEID,DBFlowerGetRoleIdArg(rolename));
			rpc->linksid = sid;
			rpc->recv_rolename = rolename;
			rpc->send_rolename = pSendInfo->name;
			rpc->lover_msg = msg;
			rpc->gameid = pSendInfo->gameid;
			rpc->localsid = localsid;
			rpc->add_count = count;
			rpc->send_roleid = send_roleid;
			rpc->send_userid = pSendInfo->userid;
			rpc->send_gender = pSendRoleInfo->gender;
			GameDBClient::GetInstance()->SendProtocol(rpc);

		}
		else
		{
			// 先判断roleid的合法性
			PlayerInfo* pRecvInfo = UserContainer::GetInstance().FindRole((dest_roleid));
			if( NULL == pRecvInfo )
			{
				DEBUG_PRINT("SendFlowerReq pRecvInfo is NULL, dest_roleid:%d\n", dest_roleid );
				// 需要进一步去数据库验证
				DBFlowerGetRoleExist* rpc = (DBFlowerGetRoleExist*) Rpc::Call(RPC_DBFLOWERGETROLEEXIST,DBFlowerGetRoleExistArg(dest_roleid));
				rpc->linksid = sid;
				rpc->localsid = localsid;
				rpc->gameid = pSendInfo->gameid;
				rpc->lover_msg = msg;
				rpc->send_rolename = pSendInfo->name;
				rpc->add_count = count;
				rpc->send_roleid = send_roleid;
				rpc->recv_roleid = dest_roleid;
				rpc->send_userid = pSendInfo->userid;
				rpc->send_gender = pSendRoleInfo->gender;
				GameDBClient::GetInstance()->SendProtocol(rpc);

			}
			else
			{		
				DEBUG_PRINT("SendFlowerReq Send FlowerTakeOff rpc, dest_roleid:%d\n", dest_roleid );

				GRoleInfo* pRoleInfo = RoleInfoCache::Instance().Get(dest_roleid);
				if(NULL == pRoleInfo)
				{
					// 跨服则进入
					LOG_TRACE("sendflowerreq, NULL == pRoleInfo");
					
					// 需要进一步去数据库验证
					DBFlowerGetRoleExist* rpc = (DBFlowerGetRoleExist*) Rpc::Call(RPC_DBFLOWERGETROLEEXIST,DBFlowerGetRoleExistArg(dest_roleid));
					rpc->linksid = sid;
					rpc->localsid = localsid;
					rpc->gameid = pSendInfo->gameid;
					rpc->lover_msg = msg;
					rpc->send_rolename = pSendInfo->name;
					rpc->add_count = count;
					rpc->send_roleid = send_roleid;
					rpc->recv_roleid = dest_roleid;
					rpc->send_userid = pSendInfo->userid;
					rpc->send_gender = pSendRoleInfo->gender;
					GameDBClient::GetInstance()->SendProtocol(rpc);

					return;
				}

				if(pRoleInfo->gender != 1)
				{
					// 性别不符
					LOG_TRACE("sendflowerreq, gender is err");
					GDeliveryServer::GetInstance()->Send(sid,TopFlower_Err(S2C_TOPFLOWER_GENDER_ERR, localsid));	

					return;
				}

				// 发送gs扣除请求
				FlowerTakeOff* rpc = (FlowerTakeOff*) Rpc::Call(RPC_FLOWERTAKEOFF, FlowerTakeOffArg(send_roleid, count));
				rpc->linksid = sid;
				rpc->localsid = localsid;
				rpc->recv_rolename = pRoleInfo->name;
				rpc->send_rolename = pSendInfo->name;
				rpc->lover_msg = msg;
				rpc->add_count = count;
				rpc->send_roleid = send_roleid;
				rpc->recv_roleid = dest_roleid;
				rpc->send_userid = pSendInfo->userid;
				rpc->recv_userid = pRoleInfo->userid;
				rpc->recv_gender = pRoleInfo->gender;
				rpc->send_gender = pSendRoleInfo->gender;
				GProviderServer::GetInstance()->DispatchProtocol( pSendInfo->gameid, rpc );

				//if(pRecvInfo->gender != 1)
				//{
				//	// 性别不符
				//	LOG_TRACE("sendflowerreq, gender is err");
				//	return;
				//}

				/*FlowerTakeOff* rpc = (FlowerTakeOff*) Rpc::Call(RPC_FLOWERTAKEOFF, FlowerTakeOffArg(send_roleid, count));
				rpc->add_count = count;
				rpc->send_roleid = send_roleid;
				rpc->recv_roleid = dest_roleid;
				GProviderServer::GetInstance()->DispatchProtocol( pSendInfo->gameid, rpc );*/

			}
		}
	}
};

};

#endif
