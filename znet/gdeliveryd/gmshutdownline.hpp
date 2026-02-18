
#ifndef __GNET_GMSHUTDOWNLINE_HPP
#define __GNET_GMSHUTDOWNLINE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gmshutdownline_re.hpp"


namespace GNET
{

class GMShutdownLine : public GNET::Protocol
{
	#include "gmshutdownline"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("gmshutdownline", "gmroleid=%d, gs_id=%d", gmroleid, gs_id);
		if ( GProviderServer::GetInstance()->DispatchProtocol(gs_id, *this))
		{
			manager->Send(sid, GMShutdownLine_Re(ERR_SUCCESS, gmroleid, localsid, gs_id));
			LOG_TRACE("gmshutdownline, gmroleid=%d, gs_id=%d, send success", gmroleid, gs_id);
		}
		else
		{
			manager->Send(sid, GMShutdownLine_Re(ERR_COMMUNICATION, gmroleid, localsid, gs_id));
			LOG_TRACE("gmshutdownline, gmroleid=%d, gs_id=%d, send failed", gmroleid, gs_id);
		}
	}
};

};
#endif
