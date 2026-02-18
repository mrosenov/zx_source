
#ifndef __GNET_SYNCROLEDATA_HPP
#define __GNET_SYNCROLEDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SyncRoleData : public GNET::Protocol
{
	#include "syncroledata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
