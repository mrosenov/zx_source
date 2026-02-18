
#ifndef __GNET_REFWITHDRAWEXP_HPP
#define __GNET_REFWITHDRAWEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "referencemanager.h"


namespace GNET
{

class RefWithdrawExp : public GNET::Protocol
{
	#include "refwithdrawexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		RefWithdrawExp_Re re;
		re.roleid = roleid;
		re.localsid = localsid;
		re.retcode = ReferenceManager::GetInstance()->WithdrawExp(roleid);
		manager->Send(sid, re);
	}
};

};

#endif
