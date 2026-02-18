
#ifndef __GNET_GCONSIGNEND_HPP
#define __GNET_GCONSIGNEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void consign_end(int roleid, char need_read);

namespace GNET
{

class GConsignEnd : public GNET::Protocol
{
	#include "gconsignend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		consign_end(roleid, need_readDB);
	}
};

};

#endif
