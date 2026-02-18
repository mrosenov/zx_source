
#ifndef __GNET_SENDREFADDEXP_HPP
#define __GNET_SENDREFADDEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void ref_add_exp(int roleid, int64_t inc);

namespace GNET
{

class SendRefAddExp : public GNET::Protocol
{
	#include "sendrefaddexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ref_add_exp(roleid, exp);
	}
};

};

#endif
