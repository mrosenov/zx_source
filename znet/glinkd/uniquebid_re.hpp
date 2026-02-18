
#ifndef __GNET_UNIQUEBID_RE_HPP
#define __GNET_UNIQUEBID_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class UniqueBid_Re : public GNET::Protocol
{
	#include "uniquebid_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
