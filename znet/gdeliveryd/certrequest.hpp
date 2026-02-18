
#ifndef __GNET_CERTREQUEST_HPP
#define __GNET_CERTREQUEST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CertRequest : public GNET::Protocol
{
	#include "certrequest"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
