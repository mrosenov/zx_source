
#ifndef __GNET_FACBASEPROPCHANGE_HPP
#define __GNET_FACBASEPROPCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBasePropChange : public GNET::Protocol
{
	#include "facbasepropchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facbasepropchange roleid %d fid %d type %d delta %d", roleid, fid, prop_type, delta);
		int base_gs = 0;
		int ret = FacBaseManager::GetInstance()->GetBaseIndex(fid, base_gs, this->base_tag);
		if (ret != ERR_SUCCESS)
		{
			Log::log(LOG_ERR, "facbasepropchange roleid %d fid %d type %d delta %d, err %d", roleid, fid, prop_type, delta, ret);
			return;
		}
		GProviderServer::GetInstance()->DispatchProtocol(base_gs, this);
	}
};

};

#endif
