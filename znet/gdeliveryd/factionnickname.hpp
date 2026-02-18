
#ifndef __GNET_FACTIONNICKNAME_HPP
#define __GNET_FACTIONNICKNAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FactionNickname : public GNET::Protocol
{
	#include "factionnickname"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionNickname, roleid=%d, factionid=%d, receiver=%d", roleid, factionid, receiver);
		PlayerInfo * pinfo = NULL;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid!=factionid || pinfo->factiontitle>TITLE_VICEMASTER || !pinfo->familyid) 
				return;
		}
		GFolk info;
		int familyid;
		char title;
		if(!FactionManager::Instance()->GetMemberinfo(pinfo->factionid, receiver, familyid, title))
				return;
		if(familyid>0 && !FamilyManager::Instance()->SetNickname(familyid, roleid, receiver, nickname, info))
                        return;
	}
};

};

#endif
