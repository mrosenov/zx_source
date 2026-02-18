
#ifndef __GNET_KINGDOMTITLECHANGE_HPP
#define __GNET_KINGDOMTITLECHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomTitleChange : public GNET::Protocol
{
	#include "kingdomtitlechange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid ,this);	
	}
};

};

#endif
