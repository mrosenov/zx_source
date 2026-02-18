
#ifndef __GNET_SETCUSTOMDATA_RE_HPP
#define __GNET_SETCUSTOMDATA_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class SetCustomData_Re : public GNET::Protocol
{
	#include "setcustomdata_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("glinkd::receive setcustomdata_re from gdelivery.roleid=%d,retcode=%d\n",roleid,result);
		GLinkServer::GetInstance()->Send(localsid,this);

	}
};

};

#endif
