
#ifndef __GNET_PLAYERFACTIONINFO_RE_HPP
#define __GNET_PLAYERFACTIONINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
namespace GNET
{

class PlayerFactionInfo_Re : public GNET::Protocol
{
	#include "playerfactioninfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	}
};

};

#endif
