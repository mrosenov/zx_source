
#ifndef __GNET_INSTANCINGGETFIELD_RE_HPP
#define __GNET_INSTANCINGGETFIELD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gbattlerole"

namespace GNET
{

class InstancingGetField_Re : public GNET::Protocol
{
	#include "instancinggetfield_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
