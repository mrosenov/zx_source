
#ifndef __GNET_KEYSREQ_HPP
#define __GNET_KEYSREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KeysReq : public GNET::Protocol
{
	#include "keysreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
