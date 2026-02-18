
#ifndef __GNET_GETCOUPONSREQ_HPP
#define __GNET_GETCOUPONSREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCouponsReq : public GNET::Protocol
{
	#include "getcouponsreq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
