
#ifndef __GNET_GOPENBANQUETLEAVE_HPP
#define __GNET_GOPENBANQUETLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GOpenBanquetLeave : public GNET::Protocol
{
	#include "gopenbanquetleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
