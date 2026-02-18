
#ifndef __GNET_FACTIONPKRAIDINVITE_RE_HPP
#define __GNET_FACTIONPKRAIDINVITE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "raidmanager.h"

namespace GNET
{

class FactionPkRaidInvite_Re : public GNET::Protocol
{
	#include "factionpkraidinvite_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                LOG_TRACE("FactionPkRaidInvite_Re roleid=%d result=%d",roleid,result);
		RaidManager::GetInstance()->OnFactionPkInviteRe(roleid, inviter_factionid, dst_factionid, map_id, result);
	}
};

};

#endif
