
#ifndef __GNET_SENDSIEGESTART_RE_HPP
#define __GNET_SENDSIEGESTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendSiegeStart_Re : public GNET::Protocol
{
	#include "sendsiegestart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SendSiegeStart_Re: retcode=%d,battle_id=%d,world_tag=%d,sid=%d", 
			retcode, battle_id, world_tag, sid);
		SiegeManager::GetInstance()->OnStart(retcode, battle_id, world_tag);
	}
};

};

#endif
