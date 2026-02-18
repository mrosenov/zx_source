
#ifndef __GNET_SYNCPLAYERFACTION_HPP
#define __GNET_SYNCPLAYERFACTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{
void ReceivePlayerFactionInfo(int roleid,unsigned int faction_id,unsigned int family_id , char faction_title, void * faction_name,size_t name_len);

class SyncPlayerFaction : public GNET::Protocol
{
	#include "syncplayerfaction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ReceivePlayerFactionInfo(rid,factionid, familyid,title,factionname.begin(),factionname.size());
	}
};

};

#endif
