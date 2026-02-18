
#ifndef __GNET_KINGDOMAPPOINT_HPP
#define __GNET_KINGDOMAPPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "kingdomappoint_re.hpp"

namespace GNET
{

class KingdomAppoint : public GNET::Protocol
{
	#include "kingdomappoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int ret = KingdomManager::GetInstance()->Appoint(roleid, candidate_name, title);
		if (ret != ERR_SUCCESS)
			manager->Send(sid, KingdomAppoint_Re(ret, roleid, 0, title, localsid, Octets()));
		LOG_TRACE("kingdomappoint ret %d roleid %d", ret, roleid);
	}
};

};

#endif
