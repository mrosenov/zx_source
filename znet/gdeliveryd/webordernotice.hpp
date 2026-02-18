
#ifndef __GNET_WEBORDERNOTICE_HPP
#define __GNET_WEBORDERNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "weborder"

namespace GNET
{

class WebOrderNotice : public GNET::Protocol
{
	#include "webordernotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
