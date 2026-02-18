
#ifndef __GNET_UPDATEFAMILYSKILL_HPP
#define __GNET_UPDATEFAMILYSKILL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class UpdateFamilySkill : public GNET::Protocol
{
	#include "updatefamilyskill"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
