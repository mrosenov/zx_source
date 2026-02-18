
#ifndef __GNET_FACDYNBUILDINGUPGRADE_HPP
#define __GNET_FACDYNBUILDINGUPGRADE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynBuildingUpgrade : public GNET::Protocol
{
	#include "facdynbuildingupgrade"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facdynbuildingupgrade fid %d tid %d level %d", fid, tid, level);

		FAC_DYNAMIC::building_upgrade log = {tid, level, cost_grass, cost_mine, cost_monster_food, cost_monster_core, 20, {}};
		FactionDynamic::GetName(rolename, log.rolename, log.rolenamesize);
		FactionManager::Instance()->RecordDynamic(fid, FAC_DYNAMIC::BUILDING_UPGRADE, log);
	}
};

};

#endif
