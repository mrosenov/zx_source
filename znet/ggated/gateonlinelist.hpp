
#ifndef __GNET_GATEONLINELIST_HPP
#define __GNET_GATEONLINELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gaterolecache.h"

#include "ggateonlineinfo"
namespace GNET
{

class GateOnlineList : public GNET::Protocol
{
	#include "gateonlinelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("GateOnlineList start=%d,finish=%d size=%d",start,finish,onlinelist.size());
		RoleContainer::GetInstance().GameOnlineSync(start,finish,onlinelist);
	}
};

};

#endif
