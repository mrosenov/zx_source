
#ifndef __GNET_FACBASEDATABROADCAST_HPP
#define __GNET_FACBASEDATABROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseDataBroadcast : public GNET::Protocol
{
	#include "facbasedatabroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facbasedatabroadcast fid %d data.size %d", fid, data.size());
		FacBaseDataSend pro(0, 0, 0, data, 0);
		FactionManager::Instance()->Broadcast(fid, pro, pro.localsid);
	}
};

};

#endif
