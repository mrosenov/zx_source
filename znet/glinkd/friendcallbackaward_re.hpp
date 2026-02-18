
#ifndef __GNET_FRIENDCALLBACKAWARD_RE_HPP
#define __GNET_FRIENDCALLBACKAWARD_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackAward_Re : public GNET::Protocol
{
	#include "friendcallbackaward_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(localsid, roleid))
			return;
		DEBUG_PRINT("GLink::FriendCallbackAward_Re, roleid=%d, localsid=%d, award_type=%d\n", roleid, localsid, award_type);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
