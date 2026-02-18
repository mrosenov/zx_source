
#ifndef __GNET_GOPENBANQUETCLOSE_HPP
#define __GNET_GOPENBANQUETCLOSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GOpenBanquetClose : public GNET::Protocol
{
	#include "gopenbanquetclose"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
