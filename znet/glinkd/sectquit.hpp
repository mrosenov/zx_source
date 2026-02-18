
#ifndef __GNET_SECTQUIT_HPP
#define __GNET_SECTQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SectQuit : public GNET::Protocol
{
	#include "sectquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
