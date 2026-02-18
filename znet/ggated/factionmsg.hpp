
#ifndef __GNET_FACTIONMSG_HPP
#define __GNET_FACTIONMSG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionidbean"
#include "rolemsgbean"
#include "gtmanager.h"
namespace GNET
{

class FactionMsg : public GNET::Protocol
{
	#include "factionmsg"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionMsg: factionid=%lld,type=%d,sender=%lld",factionid.factionid,factionid.ftype,message.sender);
		if(GTManager::Instance()->OnGTFactionMsg(factionid.factionid,factionid.ftype,message))
		{
			DEBUG_PRINT("FactionMsg:OnGTFactionMsg error");
		}
	
	}
};

};

#endif
