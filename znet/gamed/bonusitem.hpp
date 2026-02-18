
#ifndef __GNET_BONUSITEM_HPP
#define __GNET_BONUSITEM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void recv_deliveryd_item(int roleid, unsigned int award_type, unsigned int item_id);

namespace GNET
{

class BonusItem : public GNET::Protocol
{
	#include "bonusitem"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		recv_deliveryd_item(roleid, award_type, itemid);
	}
};

};

#endif
