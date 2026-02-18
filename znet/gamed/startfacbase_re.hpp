
#ifndef __GNET_STARTFACBASE_RE_HPP
#define __GNET_STARTFACBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StartFacBase_Re : public GNET::Protocol
{
	#include "startfacbase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
