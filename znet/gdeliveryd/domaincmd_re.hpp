
#ifndef __GNET_DOMAINCMD_RE_HPP
#define __GNET_DOMAINCMD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "domaindaemon.h"

namespace GNET
{

class DomainCmd_Re : public GNET::Protocol
{
	#include "domaincmd_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DomainDaemon::Instance()->Deliver(*this);
	}
};

};

#endif
