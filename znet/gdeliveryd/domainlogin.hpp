
#ifndef __GNET_DOMAINLOGIN_HPP
#define __GNET_DOMAINLOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "domaindaemon.h"
namespace GNET
{

class DomainLogin : public GNET::Protocol
{
	#include "domainlogin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DomainDaemon::Instance()->OnLogin(nonce, sid, localsid);
	}
};

};

#endif
