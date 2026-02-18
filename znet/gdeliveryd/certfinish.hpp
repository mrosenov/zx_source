
#ifndef __GNET_CERTFINISH_HPP
#define __GNET_CERTFINISH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class CertFinish : public GNET::Protocol
{
	#include "certfinish"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GAuthClient *au_client = GAuthClient::GetInstance();
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();
		au_client->SetOSecurity(sid,ARCFOURSECURITY, au_client->osec_key);
		au_client->SetState(&(GAuthClient::_TState_Available));
		// 设置version为1，以期望AU返回其版本号 
		if (au_client->SendProtocol(AnnounceZoneid3(dsm->zoneid,dsm->aid, au_client->blreset, au_client->local_ip, 0, 0, 1, 0)))
		{
			LOG_TRACE("send AnnounceZoneid to au ok,aid=%d,zoneid=%d", dsm->aid, dsm->zoneid);
			if (au_client->blreset)
				au_client->blreset = false;
		}
		else
			Log::log(LOG_WARNING,"send AnnounceZoneid to au failed,aid=%d,zoneid=%d", dsm->aid, dsm->zoneid);
	}
};

};

#endif
