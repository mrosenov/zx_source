
#ifndef __GNET_FACTIONLIST_HPP
#define __GNET_FACTIONLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionmanager.h"

namespace GNET
{

class FactionList : public GNET::Protocol
{
	#include "factionlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		FactionList_Re list;
		list.localsid = localsid;
		list.istree = istree;
		list.page = page;
		list.familyid = familyid;
		list.base_status = 0;
		list.activity = 0;
		if(familyid)
			FamilyManager::Instance()->GetMember(familyid, list, istree);
		else
		{
			if (istree == 0)
				FactionManager::Instance()->GetMemberFlat(factionid, page, list);
			else
				FactionManager::Instance()->GetMemberTree(factionid, list);
		}
		manager->Send(sid, list);
		LOG_TRACE("factionlist page %d roleid %d factionid %d familyid %d istree %d, base_status %d act %d",	page, roleid, factionid, familyid, istree, list.base_status, list.activity);
	}
};

};

#endif
