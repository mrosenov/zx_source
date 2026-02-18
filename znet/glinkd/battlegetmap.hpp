
#ifndef __GNET_BATTLEGETMAP_HPP
#define __GNET_BATTLEGETMAP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gfieldinfo"
#include "gterritory"
#include "battlegetmap_re.hpp"

namespace GNET
{

class BattleGetMap : public GNET::Protocol
{
	#include "battlegetmap"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		BattleGetMap_Re re(0, 0);
		GLinkServer::GetInstance()->Send(sid, re);
	}
};

};

#endif
