
#ifndef __GNET_INSTANCINGENTER_RE_HPP
#define __GNET_INSTANCINGENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingEnter_Re : public GNET::Protocol
{
	#include "instancingenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
