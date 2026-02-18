
#ifndef __GNET_GMSHUTDOWNLINE_RE_HPP
#define __GNET_GMSHUTDOWNLINE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GMShutdownLine_Re : public GNET::Protocol
{
	#include "gmshutdownline_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
