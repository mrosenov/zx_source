
#ifndef __GNET_GETKDCTOKEN_RE_HPP
#define __GNET_GETKDCTOKEN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"

namespace GNET
{

class GetKDCToken_Re : public GNET::Protocol
{
	#include "getkdctoken_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		dsm->Send(linksid,this);
	}
};

};

#endif
