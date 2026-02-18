
#ifndef __GNET_BONUSITEM_HPP
#define __GNET_BONUSITEM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class BonusItem : public GNET::Protocol
{
	#include "bonusitem"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
