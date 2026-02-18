
#ifndef __GNET_SENDREFCASHUSED_HPP
#define __GNET_SENDREFCASHUSED_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "referencemanager.h"


namespace GNET
{

class SendRefCashUsed : public GNET::Protocol
{
	#include "sendrefcashused"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ReferenceManager::GetInstance()->OnReferralUseCash(roleid, bonus_cont);
	}
};

};

#endif
