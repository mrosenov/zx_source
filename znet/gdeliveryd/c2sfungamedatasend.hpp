
#ifndef __GNET_C2SFUNGAMEDATASEND_HPP
#define __GNET_C2SFUNGAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "fungamemanager.h"


namespace GNET
{

class C2SFunGamedataSend : public GNET::Protocol
{
	#include "c2sfungamedatasend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if ( NULL==pinfo )
			return; 
		GRoleInfo *role = RoleInfoCache::Instance().Get(roleid);
		if (NULL == role)
			return;

		FunGameManager &fgm = FunGameManager::GetInstance();
		if(!fgm.IsActive()) 
			return;
		
		fgm.HandleClientCmd(roleid, (char*)data.begin(), data.size(), pinfo, role);
	}
};

};

#endif
