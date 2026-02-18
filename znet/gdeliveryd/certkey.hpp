
#ifndef __GNET_CERTKEY_HPP
#define __GNET_CERTKEY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CertKey : public GNET::Protocol
{
	#include "certkey"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
