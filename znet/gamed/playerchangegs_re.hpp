
#ifndef __GNET_PLAYERCHANGEGS_RE_HPP
#define __GNET_PLAYERCHANGEGS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class PlayerChangeGS_Re : public GNET::Protocol
{
	#include "playerchangegs_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
