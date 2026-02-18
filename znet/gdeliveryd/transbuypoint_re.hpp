
#ifndef __GNET_TRANSBUYPOINT_RE_HPP
#define __GNET_TRANSBUYPOINT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "transid"
#include "sellid"

namespace GNET
{

class TransBuyPoint_Re : public GNET::Protocol
{
	#include "transbuypoint_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
