
#ifndef __GNET_FRIENDCALLBACKINFO_RE_HPP
#define __GNET_FRIENDCALLBACKINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcallbackinfo"

namespace GNET
{

class FriendCallbackInfo_Re : public GNET::Protocol
{
	#include "friendcallbackinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(localsid, roleid))
			return;
		DEBUG_PRINT("GLink::FriendCallbackInfo_Re, retcode=%d, roleid=%d, mailnum=%d, timestamp=%d, is_subscribe=%d, cooldown_list.size()=%d\n", 
		retcode, roleid, callback_limit.mailnum, callback_limit.timestamp, callback_limit.is_subscribe, callback_limit.cooldown_list.size());

		unsigned int tmp = localsid;
		localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
