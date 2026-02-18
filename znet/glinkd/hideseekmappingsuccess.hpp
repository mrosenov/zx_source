
#ifndef __GNET_HIDESEEKMAPPINGSUCCESS_HPP
#define __GNET_HIDESEEKMAPPINGSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HideSeekMappingSuccess : public GNET::Protocol
{
	#include "hideseekmappingsuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv HideSeekMappingSuccess roleid=%d, localsid=%d", roleid, localsid);
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
