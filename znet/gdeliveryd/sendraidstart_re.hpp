
#ifndef __GNET_SENDRAIDSTART_RE_HPP
#define __GNET_SENDRAIDSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"
namespace GNET
{

class SendRaidStart_Re : public GNET::Protocol
{
	#include "sendraidstart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SendRaidStart_Re gs_id=%d, raidroom_id=%d, index=%d,retcode=%d",gs_id, raidroom_id, index,retcode);	
		RaidManager::GetInstance()->OnGSRaidStart(gs_id, raidroom_id, index,retcode);
	}
};

};

#endif
