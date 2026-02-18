
#ifndef __GNET_UPDATEQUEEN_HPP
#define __GNET_UPDATEQUEEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gkingdomfunctionary"

namespace GNET
{

class UpdateQueen : public GNET::Protocol
{
	#include "updatequeen"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
