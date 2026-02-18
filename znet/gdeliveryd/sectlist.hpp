
#ifndef __GNET_SECTLIST_HPP
#define __GNET_SECTLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "sectmanager.h"
namespace GNET
{

class SectList : public GNET::Protocol
{
	#include "sectlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("SectList, master=%d,roleid=%d", master,roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if(NULL==pinfo || pinfo->sectid!=master)
			return;
		SectList_Re re;
		re.localsid = localsid;
		if(SectManager::Instance()->GetSect(master, re,roleid))
			manager->Send(sid, re);
	}
};

};

#endif
