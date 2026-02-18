
#ifndef __GNET_C2SHOMETOWNDATASEND_HPP
#define __GNET_C2SHOMETOWNDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "hometownmanager.h"
#include "mapuser.h"
#include "s2chometowndatasend.hpp"
#include "gdeliveryserver.hpp"
#include "hometownpacket.h"


namespace GNET
{

class C2SHometowndataSend : public GNET::Protocol
{
	#include "c2shometowndatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		HOMETOWN::WRAPPER response;
		HometownManager *htm = HometownManager::GetInstance();
		htm->HandleCmd(roleid, (char*)data.begin(), data.size(), response);
		GDeliveryServer::GetInstance()->Send(sid, S2CHometowndataSend(roleid, localsid, response.get_buf()));
	}
};

};

#endif
