
#ifndef __GNET_GETFRIENDGTSTATUS_HPP
#define __GNET_GETFRIENDGTSTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetFriendGTStatus : public GNET::Protocol
{
	#include "getfriendgtstatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	//	DEBUG_PRINT("GetFriendGTStatus roleid=%d");
	       	if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
