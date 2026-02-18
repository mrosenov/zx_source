
#ifndef __GNET_RAIDBEGINVOTES_HPP
#define __GNET_RAIDBEGINVOTES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RaidBeginVotes : public GNET::Protocol
{
	#include "raidbeginvotes"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
