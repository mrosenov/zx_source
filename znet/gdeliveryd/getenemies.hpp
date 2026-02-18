
#ifndef __GNET_GETENEMIES_HPP
#define __GNET_GETENEMIES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "getenemies_re.hpp"

namespace GNET
{

class GetEnemies : public GNET::Protocol
{
	#include "getenemies"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GetEnemies_Re re;
		LOG_TRACE("Getenemy: roleid=%d, fulllist=%d", roleid, fulllist);
		time_t now = Timer::GetTime();
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());	
			PlayerInfo* pinfo = UserContainer::GetInstance().FindRole(roleid);
			if( NULL == pinfo )
				return;
			if(pinfo->friend_ver<0)
				return;
			if(now-pinfo->access_time<30)
				return;

			for(GEnemyInfoVector::iterator i=pinfo->enemies.begin(),ie=pinfo->enemies.end();i!=ie;++i)
			{
				if(fulllist==ENEMY_IDLIST)
					re.online.push_back(RoleLinePair(i->rid,-1));
				else
				{
					PlayerInfo * user = UserContainer::GetInstance().FindRoleOnline(i->rid);
					if(NULL!=user)
					{
						i->lineid = user->gameid;
						if(fulllist==ENEMY_ONLINE)
							re.online.push_back(RoleLinePair(i->rid,user->gameid));
					}
					else
						i->lineid = -1;
				}
			}
			if(fulllist==ENEMY_FULLLIST)
				re.enemies = pinfo->enemies;
		}
		re.fulllist = fulllist;
		re.localsid = localsid;
		manager->Send(sid, re);
	}
};

};

#endif
