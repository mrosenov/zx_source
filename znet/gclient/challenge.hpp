
#ifndef __GNET_CHALLENGE_HPP
#define __GNET_CHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "response.hpp"
 
#include "glinkclient.h"

namespace GNET
{

class Challenge : public GNET::Protocol
{
	#include "challenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkClient * cm=(GLinkClient*) manager;
                Response response;
		response.use_token=0;
                response.Setup(cm->identity,cm->password,nonce);
                cm->SendProtocol(response);
                cm->password.swap(response.response);
                DEBUG_PRINT("client:Process challenge...,version is %d\n",version);
		cm->SetClientState(ECS_CONNECTED);
                //printf("client:Process challenge...,version is %d\n",version);
                return;// TODO
	}
};

};

#endif
