
#ifndef __GNET_SENDHOMETOWNEXCHGMONEY_HPP
#define __GNET_SENDHOMETOWNEXCHGMONEY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"
#include "hometownmanager.h"

namespace GNET
{

class SendHometownExchgMoney : public GNET::Protocol
{
	#include "sendhometownexchgmoney"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		HometownManager::GetInstance()->ExchgMoney(roleid, sid, amount, syncdata);
	}
};

};

#endif
