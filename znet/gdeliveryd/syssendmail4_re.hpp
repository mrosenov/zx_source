
#ifndef __GNET_SYSSENDMAIL4_RE_HPP
#define __GNET_SYSSENDMAIL4_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SysSendMail4_Re : public GNET::Protocol
{
	#include "syssendmail4_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
