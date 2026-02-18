
#ifndef __GNET_GCRSSVRTEAMSPOSTSCORE_HPP
#define __GNET_GCRSSVRTEAMSPOSTSCORE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


void get_cs6v6_info(int roleid, int role_score, int team_score, int team_rank, int award_timestamp, int award_type, int award_level);

namespace GNET
{

class GCrssvrTeamsPostScore : public GNET::Protocol
{
	#include "gcrssvrteamspostscore"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		get_cs6v6_info(roleid, rolescore, teamscore, toppos, weektoptime, award_type, award_level);
	}
};

};

#endif
