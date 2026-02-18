
#ifndef __GNET_GLOBALDROPBROADCAST_HPP
#define __GNET_GLOBALDROPBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"

namespace GNET
{

class GlobalDropBroadCast : public GNET::Protocol
{
	#include "globaldropbroadcast"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm = GLinkServer::GetInstance();
		GLinkServer::RoleInfoMap::iterator it,ite=lsm->roleinfomap.end();
		for(it = lsm->roleinfomap.begin();it!=ite;++it)
			 GLinkServer::GetInstance()->Send(it->second.sid,this);
	}
};

};

#endif
