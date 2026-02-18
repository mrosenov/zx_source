
#ifndef __GNET_FACBASEDATASEND_HPP
#define __GNET_FACBASEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseDataSend : public GNET::Protocol
{
	#include "facbasedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (manager == GLinkServer::GetInstance())
		{
			//客户端发来的消息
			SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
			if (!sinfo || !sinfo->policy.Update(FAC_BASE_DATA_SEND) || sinfo->roleid != roleid/*等同于 ValidRole 验证*/)
				return;
			this->linkid = GProviderServer::GetProviderServerID();
			this->localsid = sid;
			GDeliveryClient::GetInstance()->SendProtocol(this);
		}
		else
		{ 	//基地gs直接返回给客户端的消息
			//或者是 deliveryd 广播的基地消息
			//或者是 deliveryd 转发而来的基地成员的消息
			//GLinkServer::GetInstance()->Send(localsid, this);	
			GLinkServer* lsm=GLinkServer::GetInstance();
			//if (lsm->ValidRole(localsid, roleid))
			//facbasebroadcast 转发过来的数据 roleid 都是 0
			lsm->AccumulateSend(localsid, GamedataSend(data));
		}
	}
};

};

#endif
