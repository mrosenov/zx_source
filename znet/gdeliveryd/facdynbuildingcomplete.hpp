
#ifndef __GNET_FACDYNBUILDINGCOMPLETE_HPP
#define __GNET_FACDYNBUILDINGCOMPLETE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynBuildingComplete : public GNET::Protocol
{
	#include "facdynbuildingcomplete"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facdynbuildingcomplete fid %d tid %d level %d", fid, tid, level);

		FAC_DYNAMIC::building_complete log = {tid, level};
		FactionManager::Instance()->RecordDynamic(fid, FAC_DYNAMIC::BUILDING_COMPLETE, log);
	}
};

};

#endif
