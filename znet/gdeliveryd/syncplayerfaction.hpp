
#ifndef __GNET_SYNCPLAYERFACTION_HPP
#define __GNET_SYNCPLAYERFACTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionmanager.h"
#include "circlemanager.h"
#include "snsplayerinfomanager.h"
#include "crssvrteamsmanager.h"

namespace GNET
{

class SyncPlayerFaction : public GNET::Protocol
{
	#include "syncplayerfaction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("SyncPlayerFaction, roleid=%d,factionid=%d,familyid=%d,contribution=%d,"
			"devotion=%d,level=%d,battlescore=%d", 
			rid, factionid,familyid,contribution,devotion,level,battlescore);

		FactionManager::Instance()->Sync2Game(rid,factionid,familyid,contribution,devotion);
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(rid);
			if (pinfo) 
			{
				STAT_MIN5("Level", 1);
				pinfo->level = level;
				if (battlescore) pinfo->battlescore = battlescore;
			}

			GRoleInfo *roleinfo = RoleInfoCache::Instance().Get(rid);
			if (roleinfo != NULL)
			{	
				if(roleinfo->level < level || roleinfo->reborn_cnt < reborn_cnt)
				{
					roleinfo->level = level;
					roleinfo->reborn_cnt = reborn_cnt;
					CircleManager::Instance()->OnLevelUp(rid,circleid,level,reborn_cnt);
					//NameManager::GetInstance()->OnLevelUp(rid, roleinfo->name, level);
				}
				else
				{
					roleinfo->level = level;
					roleinfo->reborn_cnt = reborn_cnt;
				}
			}
			
			CrssvrTeamsManager::Instance()->UpdateRoleLvl(rid, level);
			SNSPlayerInfoManager::GetInstance()->UpdateSNSRoleLevel(rid, level);
		}
	}
};

};

#endif
