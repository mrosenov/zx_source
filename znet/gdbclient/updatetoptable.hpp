
#ifndef __GNET_UPDATETOPTABLE_HPP
#define __GNET_UPDATETOPTABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class UpdateTopTable : public GNET::Protocol
{
	#include "updatetoptable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
