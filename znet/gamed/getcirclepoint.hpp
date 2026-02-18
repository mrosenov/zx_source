
#ifndef __GNET_GETCIRCLEPOINT_HPP
#define __GNET_GETCIRCLEPOINT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCirclePoint : public GNET::Protocol
{
	#include "getcirclepoint"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
