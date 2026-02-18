
#ifndef __GNET_TRYRECONNECT_HPP
#define __GNET_TRYRECONNECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryReconnect : public GNET::Protocol
{
	#include "tryreconnect"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		localsid = sid;
		GLinkServer *lsm = (GLinkServer *)manager;
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (!sinfo)
			return;

		int client_ip = ((struct sockaddr_in*)(sinfo->GetPeer()))->sin_addr.s_addr;
		this->loginip = client_ip;
		if (GDeliveryClient::GetInstance()->SendProtocol(this))
		{
			Log::log(LOG_DEBUG, "[reconnect]: send reconnect to deliveryd, roleid=%d, client_ip=%d, sid=%d\n", roleid, client_ip, sid); 
		}
		else
		{
			Log::log(LOG_DEBUG, "[reconnect]: failed to send reconnect to deliveryd, roleid=%d, client_ip=%d, sid=%d\n", roleid, client_ip, sid); 
		}
	}
};

};

#endif
