
#ifndef __GNET_SYNCBATHTIMES_HPP
#define __GNET_SYNCBATHTIMES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void sync_bath_count(int roleid, int times);

namespace GNET
{

class SyncBathTimes : public GNET::Protocol
{
	#include "syncbathtimes"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		sync_bath_count(roleid, times);
	}
};

};

#endif
