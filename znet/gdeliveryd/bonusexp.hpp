
#ifndef __GNET_BONUSEXP_HPP
#define __GNET_BONUSEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BonusExp : public GNET::Protocol
{
	#include "bonusexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
