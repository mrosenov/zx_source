
#ifndef __GNET_SENDCHANGEPROFILE_HPP
#define __GNET_SENDCHANGEPROFILE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendChangeProfile : public GNET::Protocol
{
	#include "sendchangeprofile"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
