
#ifndef __GNET_VIPINFONOTIFY_HPP
#define __GNET_VIPINFONOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class VIPInfoNotify : public GNET::Protocol
{
	#include "vipinfonotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
