
#ifndef __GNET_HIDESEEKRAIDQUIT_HPP
#define __GNET_HIDESEEKRAIDQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HideSeekRaidQuit : public GNET::Protocol
{
	#include "hideseekraidquit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
                if ( !GLinkServer::ValidRole( sid,roleid ) )
			return;
		localsid = sid;
		GDeliveryClient::GetInstance()->SendProtocol( this );
	}
};

};

#endif
