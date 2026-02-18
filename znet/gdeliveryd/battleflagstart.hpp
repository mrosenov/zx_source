
#ifndef __GNET_BATTLEFLAGSTART_HPP
#define __GNET_BATTLEFLAGSTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "factionidbean"
#include "battleflagmanager.hpp"

namespace GNET
{

class BattleFlagStart : public GNET::Protocol
{
	#include "battleflagstart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("BattleFlagStart::roleid=%d,flagid=%d,duration=%d,gsid=%d,worldtag=%d,fid=%lld", roleid, flagid, duration, gsid, worldtag, factionid.factionid);
		BattleFlagKey bfkey;
		BattleFlagManager::MakeFactionID(bfkey, roleid, factionid.factionid, gsid, worldtag);
		BattleFlagManager::GetInstance()->BattleFlagStart(flagid, duration, bfkey);
	}
};

};

#endif
