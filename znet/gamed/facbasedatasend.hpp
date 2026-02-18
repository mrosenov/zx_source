
#ifndef __GNET_FACBASEDATASEND_HPP
#define __GNET_FACBASEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void handle_facbase_cmd(int linkid, int localsid,int roleid, int base_tag, const void * buf, size_t size);

namespace GNET
{

class FacBaseDataSend : public GNET::Protocol
{
	#include "facbasedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		handle_facbase_cmd(linkid,localsid,roleid,base_tag,data.begin(),data.size());
	}
};

};

#endif
