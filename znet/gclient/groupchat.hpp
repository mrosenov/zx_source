
#ifndef __GNET_GROUPCHAT_HPP
#define __GNET_GROUPCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class GroupChat : public GNET::Protocol
{
	#include "groupchat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
