
#ifndef __GNET_CRSSVRTEAMSSEARCH_HPP
#define __GNET_CRSSVRTEAMSSEARCH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CrssvrTeamsSearch : public GNET::Protocol
{
	#include "crssvrteamssearch"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv CrssvrTeamsSearch roleid=%d, name.size=%d", roleid, name.size());
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
