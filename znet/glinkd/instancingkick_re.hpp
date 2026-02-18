
#ifndef __GNET_INSTANCINGKICK_RE_HPP
#define __GNET_INSTANCINGKICK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingKick_Re : public GNET::Protocol
{
	#include "instancingkick_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
