
#ifndef __GNET_FACBASEBUILDINGPROGRESS_HPP
#define __GNET_FACBASEBUILDINGPROGRESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseBuildingProgress : public GNET::Protocol
{
	#include "facbasebuildingprogress"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facbasebuildingprogress roleid %d fid %d taskid %d value %d", roleid, fid, task_id, value);
		int base_gs = 0;
		int ret = FacBaseManager::GetInstance()->GetBaseIndex(fid, base_gs, this->base_tag);
		if (ret != ERR_SUCCESS)
		{
			Log::log(LOG_ERR, "facbasebuildingprogress roleid %d fid %d taskid %d value %d, err %d", roleid, fid, task_id, value, ret);
			return;
		}
		GProviderServer::GetInstance()->DispatchProtocol(base_gs, this);
	}
};

};

#endif
