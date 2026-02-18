
#ifndef __GNET_FACBASESTOPNOTICE_HPP
#define __GNET_FACBASESTOPNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacBaseStopNotice : public GNET::Protocol
{
	#include "facbasestopnotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid, this);	
	}
};

};

#endif
