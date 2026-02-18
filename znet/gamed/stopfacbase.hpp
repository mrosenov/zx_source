
#ifndef __GNET_STOPFACBASE_HPP
#define __GNET_STOPFACBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

extern void facbase_stop(int fid, int worldtag);

namespace GNET
{

class StopFacBase : public GNET::Protocol
{
	#include "stopfacbase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		facbase_stop(fid, base_tag);
	}
};

};

#endif
