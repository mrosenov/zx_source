
#ifndef __GNET_TOPFLOWERGETGIFT_HPP
#define __GNET_TOPFLOWERGETGIFT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "topflowermanager.h"

namespace GNET
{

class TopFlowerGetGift : public GNET::Protocol
{
	#include "topflowergetgift"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TopFlowerGetReq, isRecv:%d, roleid:%d, localsid:%d", isRecv, roleid, localsid);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			LOG_TRACE("roleid(%d) Try to TopFlowerGetGift in centralDS, Refuse him!",
					roleid);
			return;
		}

		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pSendInfo = UserContainer::GetInstance().FindRoleOnline((roleid));
		if( NULL == pSendInfo )
		{
			DEBUG_PRINT("TopFlowerGetReq pSendInfo is NULL, roleid:%d\n", roleid );
			return;
		}

		if(!TopFlowerManager::Instance()->CheckOpenGift())
		{
			DEBUG_PRINT("TopFlowerGetReq CheckOpenGift is fail, roleid:%d\n", roleid );
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_GIFT_TIME_NOT, localsid));
			return;
		}

		if( !TopFlowerManager::Instance()->IsGiftTake(isRecv, roleid) )
		{
			DEBUG_PRINT("TopFlowerGetReq IsGiftTake is fail, roleid:%d\n", roleid );
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_GIFT_HAS_TAKE, localsid));
			return;
		}

		int _pos = -1;
		int _count = 0;
		FlowerGiftTakeArg _arg;
		_arg.id = -1;
		_arg.roleid = roleid;
		_arg.type = 0; // 0 是礼包
		bool _in_top = TopFlowerManager::Instance()->GetRolePos(isRecv, roleid, _pos, _count);
		if(!_in_top)
		{
			// 没在榜里
			if(isRecv)
			{
				if(_count >= 9)
					_arg.id = 55690;
			}
			else
			{
				if(_count >=9)
					_arg.id = 55691;
			}
		}
		else
		{
			// 打榜 了
			if(_pos == 0)
			{
				if(isRecv)
					_arg.id = 55678;
				else
					_arg.id = 55684;

			}
			else if(_pos == 1)
			{
				if(isRecv)
					_arg.id = 55679;
				else
					_arg.id = 55685;
			}
			else if(_pos == 2)
			{
				if(isRecv)
					_arg.id = 55680;
				else
					_arg.id = 55686;
			}
			else if(_pos >=3 && _pos<10)
			{
				if(isRecv)
					_arg.id = 55681;
				else
					_arg.id = 55687;
			}
			else if(_pos>=10 && _pos<100)
			{
				if(isRecv)
					_arg.id = 55682;
				else
					_arg.id = 55688;
			}
			else if(_pos>=100 && _pos<500)
			{
				if(isRecv)
					_arg.id = 55683;
				else
					_arg.id = 55689;
			}
			else
			{}
		}

		
		Log::formatlog("topflower","prepare to take gift.roleid=%d, itemid:%d, isRecv:%d,pos:%d",roleid,_arg.id,isRecv,_pos);
		
		if(_arg.id == -1)
		{
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_GIFT_NOT_IN_TOP, localsid));
		}
		else
		{
			FlowerGiftTake* rpc = (FlowerGiftTake*) Rpc::Call(RPC_FLOWERGIFTTAKE, _arg);
			rpc->linksid = sid;
			rpc->localsid = localsid;
			rpc->roleid = roleid;
			rpc->itemid = _arg.id;
			rpc->isRecv = isRecv;
			GProviderServer::GetInstance()->DispatchProtocol( pSendInfo->gameid, rpc );
		}
	}
};

};

#endif
