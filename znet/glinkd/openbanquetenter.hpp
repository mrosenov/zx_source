
#ifndef __GNET_OPENBANQUETENTER_HPP
#define __GNET_OPENBANQUETENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetEnter : public GNET::Protocol
{
	#include "openbanquetenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetEnter roleid=%d, gs_id=%d, map_id=%d, iscross=%d", roleid, gs_id, map_id, iscross);

		if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
