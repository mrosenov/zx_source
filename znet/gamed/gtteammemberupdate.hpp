
#ifndef __GNET_GTTEAMMEMBERUPDATE_HPP
#define __GNET_GTTEAMMEMBERUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTTeamMemberUpdate : public GNET::Protocol
{
	#include "gtteammemberupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
