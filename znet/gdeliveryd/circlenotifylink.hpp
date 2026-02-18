
#ifndef __GNET_CIRCLENOTIFYLINK_HPP
#define __GNET_CIRCLENOTIFYLINK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CircleNotifyLink : public GNET::Protocol
{
	#include "circlenotifylink"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
