
#ifndef __GNET_FACBASEDATADELIVER_HPP
#define __GNET_FACBASEDATADELIVER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void handle_facbase_deliver_cmd(int roleid, int base_tag, const void * buf, size_t size);

namespace GNET
{

class FacBaseDataDeliver : public GNET::Protocol
{
	#include "facbasedatadeliver"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		handle_facbase_deliver_cmd(roleid, base_tag, data.begin(), data.size());
	}
};

};

#endif
