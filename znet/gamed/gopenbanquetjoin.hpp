
#ifndef __GNET_GOPENBANQUETJOIN_HPP
#define __GNET_GOPENBANQUETJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GOpenBanquetJoin : public GNET::Protocol
{
	#include "gopenbanquetjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
