
#ifndef __GNET_GKINGGETREWARD_RE_HPP
#define __GNET_GKINGGETREWARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void king_get_reward_response(int retcode, int roleid);

namespace GNET
{

class GKingGetReward_Re : public GNET::Protocol
{
	#include "gkinggetreward_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		king_get_reward_response(retcode, roleid);
	}
};

};

#endif
