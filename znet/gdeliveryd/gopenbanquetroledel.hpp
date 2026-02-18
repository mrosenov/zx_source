
#ifndef __GNET_GOPENBANQUETROLEDEL_HPP
#define __GNET_GOPENBANQUETROLEDEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GOpenBanquetRoleDel : public GNET::Protocol
{
	#include "gopenbanquetroledel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
