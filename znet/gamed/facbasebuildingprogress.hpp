
#ifndef __GNET_FACBASEBUILDINGPROGRESS_HPP
#define __GNET_FACBASEBUILDINGPROGRESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void facbase_building_progress(int base_tag, int fid, int task_id, int value);

namespace GNET
{

class FacBaseBuildingProgress : public GNET::Protocol
{
	#include "facbasebuildingprogress"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		facbase_building_progress(base_tag, fid, task_id, value);
	}
};

};

#endif
