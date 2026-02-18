
#ifndef __GNET_TRYRECONNECT_RE_HPP
#define __GNET_TRYRECONNECT_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryReconnect_Re : public GNET::Protocol
{
	#include "tryreconnect_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::log(LOG_DEBUG, "[reconnect]: receive reconnect response from delivery userid=%d, localsid=%d, sid=%d\n", userid, localsid, sid); 

		GLinkServer* lsm=GLinkServer::GetInstance();

		SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
		if (!sinfo)
			return;

		sinfo->userid = userid;
		sinfo->identity = account;
		sinfo->login_time = time(NULL);
		lsm->SetISecurity(localsid, ARCFOURSECURITY, iseckey);
		lsm->SetOSecurity(localsid,COMPRESSARCFOURSECURITY, oseckey);


		lsm->ChangeState(localsid, &state_GKeyReestablished);
		lsm->Send(localsid, this); 

		iseckey.clear();
		oseckey.clear();

		Log::log(LOG_DEBUG, "[reconnect]: send reconnect response to client userid=%d, localsid=%d, sid=%d\n", userid, localsid, sid); 

		if(result == -1) {
			lsm->Close(localsid);
		}
	}
};

};

#endif
