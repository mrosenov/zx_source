
#ifndef __GNET_OPENBANQUETJOIN_HPP
#define __GNET_OPENBANQUETJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetJoin : public GNET::Protocol
{
	#include "openbanquetjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv OpenBanquetJoin gs_id=%d, map_id=%d, is_team=%d, role.size=%d", gs_id, map_id, is_team, roles.size());

		//if (!GLinkServer::ValidRole(sid,send_roleid))
		//	return;
		
		localsid = sid;
		LOG_TRACE("send OpenBanquetJoin to gdeliveryd localsid:%d", localsid);

		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
