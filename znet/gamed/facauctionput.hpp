
#ifndef __GNET_FACAUCTIONPUT_HPP
#define __GNET_FACAUCTIONPUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void facbase_task_put_auction(int base_tag, int roleid, char * rolename, int name_len, int itemid);

namespace GNET
{

class FacAuctionPut : public GNET::Protocol
{
	#include "facauctionput"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		facbase_task_put_auction(base_tag, roleid, (char*)rolename.begin(), rolename.size(), itemid);
	}
};

};

#endif
