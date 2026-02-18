
#ifndef __GNET_SENDDATAANDIDENTITY_HPP
#define __GNET_SENDDATAANDIDENTITY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "userplayerdata"

namespace GNET
{
class SendDataAndIdentity : public GNET::Protocol
{
	#include "senddataandidentity"

	void Process(Manager *manager, Manager::Session::ID sid);
	//map.cpp÷–∂®“Â
};

};

#endif
