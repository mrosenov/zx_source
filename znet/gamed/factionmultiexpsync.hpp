
#ifndef __GNET_FACTIONMULTIEXPSYNC_HPP
#define __GNET_FACTIONMULTIEXPSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace  GMSV
{
void OnSyncFacMultiExp(int roleid, int multi, int end_time);
}

namespace GNET
{

class FactionMultiExpSync : public GNET::Protocol
{
	#include "factionmultiexpsync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GMSV::OnSyncFacMultiExp(roleid, multi, end_time);
	}
};

};

#endif
