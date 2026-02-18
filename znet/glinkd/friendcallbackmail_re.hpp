
#ifndef __GNET_FRIENDCALLBACKMAIL_RE_HPP
#define __GNET_FRIENDCALLBACKMAIL_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FriendCallbackMail_Re : public GNET::Protocol
{
	#include "friendcallbackmail_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(!GLinkServer::ValidRole(localsid, roleid))
			return;
		DEBUG_PRINT("GLink::FriendCallbackMail_Re, retcode=%d, sender=%d, receiver=%d\n", retcode, roleid, receiver_roleid);
		unsigned int tmp = localsid;
		this->localsid = 0;
		GLinkServer::GetInstance()->Send(tmp, this);
	}
};

};

#endif
