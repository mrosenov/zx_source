
#ifndef __GNET_SYNCBATHTIMES_HPP
#define __GNET_SYNCBATHTIMES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SyncBathTimes : public GNET::Protocol
{
	#include "syncbathtimes"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("syncbathtimes gssid %d roleid %d times %d", sid, roleid, times);
		GProviderServer::GetInstance()->BroadcastProtocol(this);
	}
};

};

#endif
