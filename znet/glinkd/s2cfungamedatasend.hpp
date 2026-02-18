
#ifndef __GNET_S2CFUNGAMEDATASEND_HPP
#define __GNET_S2CFUNGAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


#include "glinkserver.hpp"
#include "fungamedatasend.hpp"
#include "gdeliveryclient.hpp"

namespace GNET
{

class S2CFunGamedataSend : public GNET::Protocol
{
	#include "s2cfungamedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();

		if (!lsm->ValidRole(localsid,roleid))
		{
			return;
		}
		lsm->Send(localsid,FunGamedataSend(data));
	}
};

};

#endif
