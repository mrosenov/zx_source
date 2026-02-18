
#ifndef __GNET_GRAIDSERVERREGISTER_HPP
#define __GNET_GRAIDSERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidfieldinfo"
#include "raidmanager.h"
#include "graidserverregister.hpp"
namespace GNET
{

class GRaidServerRegister : public GNET::Protocol
{
	#include "graidserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		RaidManager::GetInstance()->RegisterServer(sid,gs_id,fields);
	}
};

};

#endif
