
#ifndef __GNET_FACTIONPKRAIDCHALLENGECANCEL_RE_HPP
#define __GNET_FACTIONPKRAIDCHALLENGECANCEL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionPkRaidChallengeCancel_Re : public GNET::Protocol
{
	#include "factionpkraidchallengecancel_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
