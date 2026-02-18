
#ifndef __GNET_CHATBROADCAST_HPP
#define __GNET_CHATBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "worldchat.hpp"
#include "forwardchat.hpp"
#include "conv_charset.h"
#include "gdeliveryserver.hpp"

namespace GNET
{

class ChatBroadCast : public GNET::Protocol
{
	#include "chatbroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("ChatBroadCast: challel=%d roleid=%d", channel, srcroleid);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		WorldChat chat;
		chat.channel = channel;
		chat.roleid  = srcroleid;
		if(chat.roleid>32){	
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(srcroleid);
			if(NULL == pinfo || NULL == pinfo->user)
				return;
			chat.name = pinfo->name;
			chat.sender_zoneid = pinfo->user->src_zoneid;
			if(dsm->iweb_sid != _SID_INVALID)
			{
				ForwardChat data(dsm->zoneid,pinfo->gameid,pinfo->userid,srcroleid,pinfo->name,msg);
				dsm->Send(dsm->iweb_sid, data);
			}
		}
		chat.emotion = emotion;
		chat.msg = msg;
		chat.data = data;
		if (channel == GP_CHAT_RUMOR)
		{
			chat.roleid = 0;
			chat.name.clear();
			chat.sender_zoneid = 0;
		}
		LinkServer::GetInstance().BroadcastProtocol(&chat);
	}
};

};

#endif
