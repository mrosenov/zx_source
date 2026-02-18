
#ifndef __GNET_GTERRITORYEND_HPP
#define __GNET_GTERRITORYEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTerritoryEnd : public GNET::Protocol
{
	#include "gterritoryend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE( "GTerritoryEnd: tid=%d result=%d\n", territoryid, result);
		TerritoryManager::GetInstance()->OnTerritoryEnd(territoryid, result);
	}
};

};

#endif
