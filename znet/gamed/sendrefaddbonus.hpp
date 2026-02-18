
#ifndef __GNET_SENDREFADDBONUS_HPP
#define __GNET_SENDREFADDBONUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void ref_add_bonus(int roleid, int inc);

namespace GNET
{

class SendRefAddBonus : public GNET::Protocol
{
	#include "sendrefaddbonus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ref_add_bonus(roleid, bonus);
	}
};

};

#endif
