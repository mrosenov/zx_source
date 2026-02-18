
#ifndef __GNET_SENDTERRITORYSTART_RE_HPP
#define __GNET_SENDTERRITORYSTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SendTerritoryStart_Re : public GNET::Protocol
{
	#include "sendterritorystart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "SendTerritoryStart_Re: retcode=%d tid=%d\n", retcode, territoryid);
		TerritoryManager::GetInstance()->OnTerritoryStart(territoryid, retcode);
	}
};

};

#endif
