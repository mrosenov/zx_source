
#ifndef __GNET_GUPDATEFACACT_HPP
#define __GNET_GUPDATEFACACT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GUpdateFacAct : public GNET::Protocol
{
	#include "gupdatefacact"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gupdatefacact fid %d delta %d", fid, delta);
		FactionManager::Instance()->UpdateActivity(fid, delta);
	}
};

};

#endif
