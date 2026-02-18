
#ifndef __GNET_REPORT2GM_RE_HPP
#define __GNET_REPORT2GM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"

#include "statusannounce.hpp"
namespace GNET
{

class Report2GM_Re : public GNET::Protocol
{
	#include "report2gm_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if ( !lsm->ValidLocalsid(localsid,roleid) )
			return;
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
