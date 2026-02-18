
#ifndef __GNET_QUERYBATTLEFLAGBUFF_HPP
#define __GNET_QUERYBATTLEFLAGBUFF_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueryBattleFlagBuff : public GNET::Protocol
{
	#include "querybattleflagbuff"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("QueryBattleFlagBuff::roleid=%d,gsid=%d,worldtag=%d,fid=%d", roleid,gsid,worldtag,factionid.factionid);
		BattleFlagKey bfkey;
		BattleFlagManager::MakeFactionID(bfkey, roleid, factionid.factionid, gsid, worldtag);
		BattleFlagManager::GetInstance()->QueryBattleFlagBuff(bfkey);
	}
};

};

#endif
