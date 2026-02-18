
#ifndef __GNET_CONSIGNLISTLARGECATEGORY_HPP
#define __GNET_CONSIGNLISTLARGECATEGORY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ConsignListLargeCategory : public GNET::Protocol
{
	#include "consignlistlargecategory"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
