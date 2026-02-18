
#ifndef __GNET_FORBIDSERVICE_HPP
#define __GNET_FORBIDSERVICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "serviceforbidcmd"

void forbid_sync(int type, std::vector<int> & ids);

namespace GNET
{

class ForbidService : public GNET::Protocol
{
	#include "forbidservice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		for(size_t i = 0; i < cmdlist.size(); ++i)
		{
			forbid_sync(cmdlist[i].cmdtype, cmdlist[i].arglist);
		}
	}
};

};

#endif
