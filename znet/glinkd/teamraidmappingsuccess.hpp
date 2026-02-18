
#ifndef __GNET_TEAMRAIDMAPPINGSUCCESS_HPP
#define __GNET_TEAMRAIDMAPPINGSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gcrssvrteamsdata"
#include "teamraidrolebrief"

namespace GNET
{

class TeamRaidMappingSuccess : public GNET::Protocol
{
	#include "teamraidmappingsuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TeamRaidMappingSuccess roleid=%d, localsid=%d", roleid, localsid);

		//只让玩家获得自己阵营的玩家信息，避免玩家作弊
		bool team_a = false;
		bool team_b = false;
		for(size_t i = 0; i < teamdata_a_detail.size(); ++i)
		{
			if(teamdata_a_detail[i].roleid == roleid)
			{
				team_a = true;
				break;
			}
		}

		for(size_t i = 0; i < teamdata_b_detail.size(); ++i)
		{
			if(teamdata_b_detail[i].roleid == roleid)
			{
				team_b = true;
				break;
			}
		}

		if(team_a)
		{
			teamdata_b = GCrssvrTeamsData();
			teamdata_b_detail.clear();
			b_ranking = 0;
		}
		else if(team_b)
		{
			teamdata_a = GCrssvrTeamsData();
			teamdata_a_detail.clear();
			a_ranking = 0;
		}
		
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
