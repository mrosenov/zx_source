
#ifndef __GNET_GETCOUPONSREP_HPP
#define __GNET_GETCOUPONSREP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCouponsRep : public GNET::Protocol
{
	#include "getcouponsrep"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
