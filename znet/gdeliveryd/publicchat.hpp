
#ifndef __GNET_PUBLICCHAT_HPP
#define __GNET_PUBLICCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "chatsinglecast.hpp"
#include "chatbroadcast.hpp"
#include "trade.h"
#include "maplinkserver.h"
#include "mapuser.h"
namespace GNET
{

class PublicChat : public GNET::Protocol
{
	#include "publicchat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		switch (channel)
		{
			case GP_CHAT_BROADCAST:	
				{
				ChatBroadCast cbc(channel,0,roleid,msg,data);
				LinkServer::GetInstance().BroadcastProtocol( cbc );
				}
				break;
		}

	}
};

};

#endif
