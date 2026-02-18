
#ifndef __GNET_CIRCLENOTIFYLINK_HPP
#define __GNET_CIRCLENOTIFYLINK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "circlenotify.hpp"

namespace GNET
{

class CircleNotifyLink : public GNET::Protocol
{
	#include "circlenotifylink"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,CircleNotify(msgtype,roleid,circleid,name,grade));
	}
};

};

#endif
