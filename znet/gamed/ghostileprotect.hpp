
#ifndef __GNET_GHOSTILEPROTECT_HPP
#define __GNET_GHOSTILEPROTECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GHostileProtect : public GNET::Protocol
{
	#include "ghostileprotect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
