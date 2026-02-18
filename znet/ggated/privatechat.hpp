
#ifndef __GNET_PRIVATECHAT_HPP
#define __GNET_PRIVATECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtmanager.h"
namespace GNET
{

class PrivateChat : public GNET::Protocol
{
	#include "privatechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("PrivateChat: receiver=%d sender=%d", dstroleid,srcroleid);
		if(GTManager::Instance()->OnGamePrivateChat(*this))
			DEBUG_PRINT("PrivateChat:OnGamePrivateChat err");
	}
};

};

#endif
