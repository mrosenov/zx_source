
#ifndef __GNET_CROSSBATTLEJOINTEAM_HPP
#define __GNET_CROSSBATTLEJOINTEAM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrossBattleJoinTeam : public GNET::Protocol
{
	#include "crossbattlejointeam"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("CrossBattleJoinTeam::roleid=%d,battle_type=%d,subbattle_type=%d,gsid=%d,mapid=%d, starttime=%d", roleid, battle_type, subbattle_type, gsid, mapid, starttime);
		CrossBattleManager *crossbattle = CrossBattleManager::GetInstance();
		crossbattle->InsertCrossPlayer(battle_type, subbattle_type, gsid, mapid, roleid, starttime, team);
	}
};

};

#endif
