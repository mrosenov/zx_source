
#ifndef __GNET_OPENBANQUETQUEUECOUNT_HPP
#define __GNET_OPENBANQUETQUEUECOUNT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetQueueCount : public GNET::Protocol
{
	#include "openbanquetqueuecount"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv OpenBanquetqueuecount roleid=%d", roleid);

		//if (!GLinkServer::ValidRole(sid,send_roleid))
		//	return;
		
		localsid = sid;
		LOG_TRACE("send OpenBanquetqueuecount to gdeliveryd localsid:%d", localsid);

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
