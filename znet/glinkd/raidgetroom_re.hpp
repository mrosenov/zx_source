
#ifndef __GNET_RAIDGETROOM_RE_HPP
#define __GNET_RAIDGETROOM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gbattlerole"

namespace GNET
{

class RaidGetRoom_Re : public GNET::Protocol
{
	#include "raidgetroom_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
