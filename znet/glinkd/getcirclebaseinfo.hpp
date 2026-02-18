
#ifndef __GNET_GETCIRCLEBASEINFO_HPP
#define __GNET_GETCIRCLEBASEINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetCircleBaseInfo : public GNET::Protocol
{
	#include "getcirclebaseinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid,roleid))
			return;
		if ( circleidlist.size() > 32 || circleidlist.size()==0 ) return;
		this->localsid=sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
