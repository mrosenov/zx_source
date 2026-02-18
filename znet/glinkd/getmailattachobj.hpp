
#ifndef __GNET_GETMAILATTACHOBJ_HPP
#define __GNET_GETMAILATTACHOBJ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gmailsyncdata"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class GetMailAttachObj : public GNET::Protocol
{
	#include "getmailattachobj"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( !GLinkServer::ValidRole(sid,roleid) ) return;
		localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
