
#ifndef __GNET_FACAUCTIONPUT_HPP
#define __GNET_FACAUCTIONPUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacAuctionPut : public GNET::Protocol
{
	#include "facauctionput"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facauctionput roleid %d fid %d name.size %d itemid %d", roleid, fid, rolename.size(), itemid);
		int base_gs = 0;
		int ret = FacBaseManager::GetInstance()->GetBaseIndex(fid, base_gs, this->base_tag);
		if (ret != ERR_SUCCESS)
		{
			Log::log(LOG_ERR, "facauctionput roleid %d fid %d itemid %d, err %d", roleid, fid, itemid, ret);
			return;
		}
		GProviderServer::GetInstance()->DispatchProtocol(base_gs, this);
	}
};

};

#endif
