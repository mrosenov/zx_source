
#ifndef __GNET_GATEOFFLINECHAT_HPP
#define __GNET_GATEOFFLINECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "ggtofflinemsg"

namespace GNET
{

class GateOfflineChat : public GNET::Protocol
{
	#include "gateofflinechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(dstroleid);
		if(pinfo)
		{
			localsid = pinfo->localsid;
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,this);
		}
	}
};

};

#endif
