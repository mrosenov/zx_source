
#ifndef __GNET_S2CHOMETOWNDATASEND_HPP
#define __GNET_S2CHOMETOWNDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "hometowndatasend.hpp"


namespace GNET
{

class S2CHometowndataSend : public GNET::Protocol
{
	#include "s2chometowndatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (!lsm->ValidRole(localsid,roleid)) return;
		lsm->Send(localsid,HometowndataSend(data));
	}
};

};

#endif
