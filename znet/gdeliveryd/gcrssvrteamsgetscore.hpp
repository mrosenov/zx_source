
#ifndef __GNET_GCRSSVRTEAMSGETSCORE_HPP
#define __GNET_GCRSSVRTEAMSGETSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"

namespace GNET
{

class GCrssvrTeamsGetScore : public GNET::Protocol
{
	#include "gcrssvrteamsgetscore"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		
		GCrssvrTeamsPostScore re(roleid);
		re.award_type = award_type;
		re.award_level = award_level;

		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();

		LOG_TRACE( "recv GCrssvrTeamsGetScore, roleid=%d,award_type=%d on iscentral=%d", roleid, award_type, iscentral);

		PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(!pinfo)
		{
			LOG_TRACE( "GCrssvrTeamsGetScore, roleid=%d, pinfo == NULL, iscentral=%d", roleid, iscentral);
			return;
		}

		if(!iscentral) // 在原服
		{
			re.retcode = CrssvrTeamsManager::Instance()->GetGsPostScore(roleid, award_type, re);
		}

		if(iscentral) // 在跨服
		{
			Log::log(LOG_ERR, "roleid %d try to GCrssvrTeamsGetScore on Central Delivery Server, refuse him!", roleid);
			return;
			//re.retcode = CrossCrssvrTeamsManager::Instance()->GetGsPostScore(roleid, award_type, re);
		}

		//int gs_id = GProviderServer::GetInstance()->GetGSID(sid); 
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, re); 	
		LOG_TRACE("GCrssvrTeamsGetScore roleid=%d, retcode=%d, gameid=%d, rolescore=%d, teamscore=%d, toppos=%d, weektoptime=%d award_type=%d, award_level=%d", 
				roleid, re.retcode, pinfo->gameid, re.rolescore, re.teamscore, re.toppos, re.weektoptime, re.award_type, re.award_level);

	}
};

};

#endif
