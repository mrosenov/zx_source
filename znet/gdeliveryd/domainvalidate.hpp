
#ifndef __GNET_DOMAINVALIDATE_HPP
#define __GNET_DOMAINVALIDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class DomainValidate : public GNET::Protocol
{
	#include "domainvalidate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DomainDaemon::Instance()->OnValidate(nonce, sid, localsid);
	}
};

};

#endif
