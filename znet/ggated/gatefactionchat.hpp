
#ifndef __GNET_GATEFACTIONCHAT_HPP
#define __GNET_GATEFACTIONCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gtmanager.h"
namespace GNET
{

class GateFactionChat : public GNET::Protocol
{
	#include "gatefactionchat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateFactionChat: channel=%d sender=%d fid=%d", channel,src,fid);
		if(GTManager::Instance()->OnGameFactionChat(*this))
                         DEBUG_PRINT("GateFactionChat:OnGameFactionChat err");
	}
};

};

#endif
