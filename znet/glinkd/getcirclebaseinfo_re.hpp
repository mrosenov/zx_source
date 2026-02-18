
#ifndef __GNET_GETCIRCLEBASEINFO_RE_HPP
#define __GNET_GETCIRCLEBASEINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcirclebaseinfo"

namespace GNET
{

class GetCircleBaseInfo_Re : public GNET::Protocol
{
	#include "getcirclebaseinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
