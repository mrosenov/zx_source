
#ifndef __GNET_GETFACACTTOPTABLE_HPP
#define __GNET_GETFACACTTOPTABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFacActTopTable : public GNET::Protocol
{
	#include "getfacacttoptable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL)
			return;
		GetFacActTopTable_Re re;
		re.pageid = pageid;
		re.maxsize = 0;
		re.selfrank = 0;
		re.localsid = localsid;
		TopManager::Instance()->ClientGetWeeklyTable(TOP_FACTION_ACTIVITY, pageid, re.maxsize, re.items);
		if (pinfo->factionid)
			re.can_create_base = (FactionManager::Instance()->CanCreateBase(pinfo->factionid, roleid) == ERR_SUCCESS);
		manager->Send(sid, re);
		LOG_TRACE( "getfacacttoptable roleid=%d, pageid=%d maxsize=%d can_create_base %d", roleid, pageid, re.maxsize, re.can_create_base);
	}
};

};

#endif
