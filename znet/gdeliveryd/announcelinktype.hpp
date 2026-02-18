
#ifndef __GNET_ANNOUNCELINKTYPE_HPP
#define __GNET_ANNOUNCELINKTYPE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
namespace GNET
{

class AnnounceLinkType : public GNET::Protocol
{
	#include "announcelinktype"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("iweb", "register sid=%d:type=%d", sid, link_type);
		GDeliveryServer::GetInstance()->iweb_sid = sid;
	}
};

};

#endif
