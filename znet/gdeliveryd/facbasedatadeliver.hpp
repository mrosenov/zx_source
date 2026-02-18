
#ifndef __GNET_FACBASEDATADELIVER_HPP
#define __GNET_FACBASEDATADELIVER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseDataDeliver : public GNET::Protocol
{
	#include "facbasedatadeliver"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//协议流程 client -> 普通gs -> gdeliveryd -> base_gs ->...
		LOG_TRACE("facbasedatadeliver roleid %d fid %d data.size %d", roleid, fid, data.size());
		int gs = 0;
		int ret = FacBaseManager::GetInstance()->GetBaseIndex(fid, gs, this->base_tag);
		if (ret != ERR_SUCCESS)
		{
			Log::log(LOG_ERR, "facbasedatadeliver roleid %d data.size %d, err %d", roleid, data.size(), ret);
			return;
		}
		GProviderServer::GetInstance()->DispatchProtocol(gs, this);	
	}
};

};

#endif
