
#ifndef __GNET_DOMAINCMD_HPP
#define __GNET_DOMAINCMD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "domaindaemon.h"

namespace GNET
{

class DomainCmd : public GNET::Protocol
{
	#include "domaincmd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DomainDaemon::Instance()->Process(sid, *this);
	}
};

};

#endif
