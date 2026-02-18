
#ifndef __GNET_AUTHDVERSION_HPP
#define __GNET_AUTHDVERSION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gauthclient.hpp"


namespace GNET
{

class AuthdVersion : public GNET::Protocol
{
	#include "authdversion"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv AuthdVersion retcode %d version %d", retcode, version);
		GAuthClient *auClient = GAuthClient::GetInstance();
		if(ERR_SUCCESS != retcode)
		{
			auClient->Need2Reconnect(false);
			auClient->Close(sid);
		}
		else
		{
//			auClient->SetState(&GAuthClient::_TState_Available);
			GDeliveryServer *dServer = GDeliveryServer::GetInstance();
			// 临时关闭二代ukey逻辑
			//version = 0;
			auClient->SetVersion(version);
			dServer->SetAuthVersion(version);
			LinkServer::GetInstance().BroadcastProtocol(AnnounceChallengeAlgo(dServer->ChallengeAlgo(), version));
		}
	}
};

};

#endif
