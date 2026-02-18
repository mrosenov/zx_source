
#ifndef __GNET_BONUSEXP_HPP
#define __GNET_BONUSEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


void recv_deliveryd_exp(int roleid, int64_t bonus_exp, unsigned int award_type);

namespace GNET
{

class BonusExp : public GNET::Protocol
{
	#include "bonusexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		recv_deliveryd_exp(roleid, bonus_exp, award_type);
	}
};

};

#endif
