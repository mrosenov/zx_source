
#ifndef __GNET_TOPFLOWERREQ_HPP
#define __GNET_TOPFLOWERREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "topflowermanager.h"

namespace GNET
{

class TopFlowerReq : public GNET::Protocol
{
	#include "topflowerreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TopFlowerReq, roleid:%d, localsid:%d,page:%d, isRecv:%d",send_roleid, localsid, page, isRecv);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			LOG_TRACE("send_roleid(%d) Try to TopFlowerReq in centralDS, Refuse him!",
					send_roleid);
			return;
		}
		
		if(TopFlowerManager::Instance()->GetStatus() != TOPFLOWER_READY)
		{
			DEBUG_PRINT("TopFlowerReq GetStatus  TOPFLOWER_NOT_READY");
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_TOP_NOTREADY, localsid));
			return;
		}

		if(!TopFlowerManager::Instance()->CheckGetTop())
		{
			DEBUG_PRINT("TopFlowerReq  CheckGetTop TOPFLOWER_NOT_READY");
			manager->Send(sid, TopFlower_Err(S2C_TOPFLOWER_TOP_NOTREADY, localsid));
			return;
		}

		if(page < 0)
		{
			LOG_TRACE("roleid:%d, page < 0", send_roleid);
			return;
		}

		TopFlower_Re re;
		re.localsid = localsid;
		re.isRecv = isRecv;
		re.page = page;
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		{
			Thread::RWLock::RDScoped l(TopFlowerManager::Instance()->GetLocker());	
			TopFlowerManager::Instance()->GetTop(isRecv, page, re.data, re.all_count);
			TopFlowerManager::Instance()->GetRolePos(isRecv, send_roleid, re.my_pos, re.my_count);

		}

		LOG_TRACE("recv TopFlowerReq, roleid:%d, re.size:%d, my_pos:%d, my_count:%d", send_roleid, re.data.size(), re.my_pos, re.my_count);
		dsm->Send(sid, re);
	}		
};

};

#endif
