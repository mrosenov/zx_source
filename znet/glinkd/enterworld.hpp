
#ifndef __GNET_ENTERWORLD_HPP
#define __GNET_ENTERWORLD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "gproviderserver.hpp"
namespace GNET
{

class EnterWorld : public GNET::Protocol
{
	#include "enterworld"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::log(LOG_DEBUG, "glinkd:: enterworld, roleid=%d,localsid=%d",roleid,sid);
		DEBUG_PRINT("glinkd:: enterworld, roleid=%d,localsid=%d\n",roleid,sid);
		GLinkServer* lsm=GLinkServer::GetInstance();
		if(!lsm->SetOnline(roleid, sid))
			return;
		this->localsid=sid;
		this->provider_link_id=GProviderServer::GetProviderServerID();
		if ( GDeliveryClient::GetInstance()->SendProtocol(this) )
		{
			lsm->ChangeState(sid,&state_GDataExchgServer);
			lsm->SetAliveTime(sid, _CLIENT_TTL);
		}
		else
			GLinkServer::GetInstance()->SessionError(sid,ERR_COMMUNICATION,"Server Network Error.");
	}
};

};

#endif
