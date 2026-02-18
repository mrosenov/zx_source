
#ifndef __GNET_SYNCFAMILYDATA_HPP
#define __GNET_SYNCFAMILYDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class SyncFamilyData : public GNET::Protocol
{
	#include "syncfamilydata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
