
#ifndef __GNET_FAMILYUSESKILL_HPP
#define __GNET_FAMILYUSESKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FamilyUseSkill : public GNET::Protocol
{
	#include "familyuseskill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
