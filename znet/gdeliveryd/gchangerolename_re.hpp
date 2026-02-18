
#ifndef __GNET_GCHANGEROLENAME_RE_HPP
#define __GNET_GCHANGEROLENAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GChangeRolename_Re : public GNET::Protocol
{
	#include "gchangerolename_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
