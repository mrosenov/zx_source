
#ifndef __GNET_GETTOPTABLE_HPP
#define __GNET_GETTOPTABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gettoptable_re.hpp"
#include "topmanager.h"
namespace GNET
{

class GetTopTable : public GNET::Protocol
{
	#include "gettoptable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "GetTopTable: roleid=%d, tableid=%d, pageid=%d.\n", roleid, tableid, pageid);
		int selfid = 0;
		if(!pageid)
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if(!pinfo) 
				return;
			if(tableid<TOP_FACTION_LEVEL || 
				(tableid>=TOP_PERSONAL_DEITY_LEVEL_START && tableid<TOP_PERSONAL_DEITY_LEVEL_END)\
				|| tableid == TOP_TOWERRAID_LEVEL || (tableid >= TOP_PERSONAL_CREDIT_START2&&tableid < TOP_FACTION_CREDIT_START2))
				selfid = roleid;
			else if(tableid == TOP_CRSSVRTEAMS_SCORE)
				selfid = tableid;
			else if(tableid<TOP_FAMILY_TASK_START || (tableid>=TOP_FACTION_CREDIT_START2&&tableid<TOP_TABLEID_MAX))
				selfid = pinfo->factionid;
			else
				selfid = pinfo->familyid;
		}
		GetTopTable_Re re;
		re.tableid = tableid;
		re.pageid = pageid;
		re.selfrank = 0;
		re.maxsize = 0;
		TopManager::Instance()->GetDailyTable(selfid, re);
		re.localsid = localsid;

		if(re.tableid == TOP_CRSSVRTEAMS_SCORE) // 请求战队时，selfrank做当前战队的排行信息
		{
			GDeliveryServer *dsm = GDeliveryServer::GetInstance();
			bool iscentral = dsm->IsCentralDS();
			if(!iscentral)
			{
				re.selfrank = CrssvrTeamsManager::Instance()->GetDailyTopPosByRoleId(roleid);
			}
			else
			{
				re.selfrank = CrossCrssvrTeamsManager::Instance()->GetDailyTopPosByRoleId(roleid);
			}
			LOG_TRACE("GetTopTable: roleid=%d, selfrank=%d", roleid, re.selfrank);
		}


		manager->Send(sid, re);
	}
};

};

#endif
