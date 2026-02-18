
#ifndef __GNET_GRAIDMEMBERCHANGE_HPP
#define __GNET_GRAIDMEMBERCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GRaidMemberChange : public GNET::Protocol
{
	#include "graidmemberchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
