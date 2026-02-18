
#ifndef __GNET_SENDINSTANCINGSTART_RE_HPP
#define __GNET_SENDINSTANCINGSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendInstancingStart_Re : public GNET::Protocol
{
	#include "sendinstancingstart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		InstancingManager::GetInstance()->OnBattleStart(gs_id, map_id, retcode);
	}
};

};

#endif
