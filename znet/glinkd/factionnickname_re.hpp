
#ifndef __GNET_FACTIONNICKNAME_RE_HPP
#define __GNET_FACTIONNICKNAME_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FactionNickname_Re : public GNET::Protocol
{
	#include "factionnickname_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
