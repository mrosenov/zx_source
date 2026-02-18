
#ifndef __GNET_GETTOPTABLE_HPP
#define __GNET_GETTOPTABLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class GetTopTable : public GNET::Protocol
{
	#include "gettoptable"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT ("GetTopTable:  tableid=%d, pageid=%d.",tableid, pageid);
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
