
#ifndef __GNET_ACCLIENTTIMEOUT_HPP
#define __GNET_ACCLIENTTIMEOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ACClientTimeout : public GNET::Protocol
{
	#include "acclienttimeout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(sid);
		if (!sinfo)
			return;
		Log::formatlog("acclienttimeout", "roleid=%d,userid=%d,logontime=%d", sinfo->roleid, sinfo->userid, time(NULL)-sinfo->login_time);
	}
};

};

#endif
