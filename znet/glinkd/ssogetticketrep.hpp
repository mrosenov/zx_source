
#ifndef __GNET_SSOGETTICKETREP_HPP
#define __GNET_SSOGETTICKETREP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userssoinfo"

namespace GNET
{

class SSOGetTicketRep : public GNET::Protocol
{
	#include "ssogetticketrep"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int localsid = 0;
		Marshal::OctetsStream os(reserved);
		try
		{
			os >> localsid;
		}
		catch ( Marshal::Exception & )
		{
			Log::log(LOG_ERR, "ssogetticketrep, reserved unmarshal error, userid=%lld",
					user.userid);
			return;
		}
		if (!GLinkServer::GetInstance()->ValidUser(localsid , static_cast<int>(user.userid))) 
			return;
		GLinkServer::GetInstance()->Send(localsid, this);
	}
};

};

#endif
