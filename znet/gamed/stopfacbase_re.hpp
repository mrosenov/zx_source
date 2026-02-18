
#ifndef __GNET_STOPFACBASE_RE_HPP
#define __GNET_STOPFACBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StopFacBase_Re : public GNET::Protocol
{
	#include "stopfacbase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
