
#ifndef __GNET_FACTIONPKRAIDBEGIN_HPP
#define __GNET_FACTIONPKRAIDBEGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidBegin : public GNET::Protocol
{
	#include "factionpkraidbegin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
