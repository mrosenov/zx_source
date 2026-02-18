
#ifndef __GNET_COMMONDATASYNC_HPP
#define __GNET_COMMONDATASYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "commondata.h"

namespace GNET
{

class CommonDataSync : public GNET::Protocol
{
	#include "commondatasync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		CommonDataMan::Instance()->DispatchData(session_key, data_type, data);
	}
};

};

#endif
