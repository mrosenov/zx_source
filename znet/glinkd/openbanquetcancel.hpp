
#ifndef __GNET_OPENBANQUETCANCEL_HPP
#define __GNET_OPENBANQUETCANCEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetCancel : public GNET::Protocol
{
	#include "openbanquetcancel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv OpenBanquetCancel gs_id=%d, map_id=%d, roleid=%d", gs_id, map_id, roleid);

		//if (!GLinkServer::ValidRole(sid,send_roleid))
		//	return;
		
		localsid = sid;
		LOG_TRACE("send OpenBanquetCancel to gdeliveryd localsid:%d", localsid);

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
