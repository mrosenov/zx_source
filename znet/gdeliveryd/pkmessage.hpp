
#ifndef __GNET_PKMESSAGE_HPP
#define __GNET_PKMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PKMessage : public GNET::Protocol
{
	#include "pkmessage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GRoleInfo* pinfo1 = RoleInfoCache::Instance().Get(killer_id);
		GRoleInfo* pinfo2 = RoleInfoCache::Instance().Get(deader_id);

		if (pinfo1 == NULL || pinfo2 == NULL)
			return;

		killer_name = pinfo1->name;
		deader_name = pinfo2->name;
		LinkServer::GetInstance().BroadcastProtocol(*this);
	}
};

};

#endif
