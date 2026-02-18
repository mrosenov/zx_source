
#ifndef __GNET_TERRITORYSCOREUPDATE_HPP
#define __GNET_TERRITORYSCOREUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TerritoryScoreUpdate : public GNET::Protocol
{
	#include "territoryscoreupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
