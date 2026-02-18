
#ifndef __GNET_FACTIONINFOUPDATE_HPP
#define __GNET_FACTIONINFOUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformclient.hpp"

#include "factionidbean"
#include "factioninfobean"

namespace GNET
{

class FactionInfoUpdate : public GNET::Protocol
{
	#include "factioninfoupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionInfoUpdate fid=%lld updateflag=%d",factionid.factionid,updateflag);
		GTPlatformClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
