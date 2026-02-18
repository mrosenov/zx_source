
#ifndef __GNET_COMPLAIN2GM_RE_HPP
#define __GNET_COMPLAIN2GM_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "statusannounce.hpp"
namespace GNET
{

class Complain2GM_Re : public GNET::Protocol
{
	#include "complain2gm_re"

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
