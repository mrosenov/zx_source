
#ifndef __GNET_GETENEMIES_RE_HPP
#define __GNET_GETENEMIES_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "genemyinfo"
#include "rolelinepair"

namespace GNET
{

class GetEnemies_Re : public GNET::Protocol
{
	#include "getenemies_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);	
	}
};

};

#endif
