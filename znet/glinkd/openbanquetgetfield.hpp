
#ifndef __GNET_OPENBANQUETGETFIELD_HPP
#define __GNET_OPENBANQUETGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetGetField : public GNET::Protocol
{
	#include "openbanquetgetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv OpenBanquetGetField gs_id=%d, roleid=%d", gs_id, roleid);

		//if (!GLinkServer::ValidRole(sid,send_roleid))
		//	return;
		
		localsid = sid;
		LOG_TRACE("send OpenBanquetGetField to gdeliveryd localsid:%d", localsid);

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
