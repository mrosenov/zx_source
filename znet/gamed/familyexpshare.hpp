
#ifndef __GNET_FAMILYEXPSHARE_HPP
#define __GNET_FAMILYEXPSHARE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void family_exp_delivery(int roleid, int family_id, int64_t exp);

namespace GNET
{

class FamilyExpShare : public GNET::Protocol
{
	#include "familyexpshare"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		family_exp_delivery(roleid, familyid, exp);
	}
};

};

#endif
