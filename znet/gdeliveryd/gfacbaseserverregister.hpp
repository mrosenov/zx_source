
#ifndef __GNET_GFACBASESERVERREGISTER_HPP
#define __GNET_GFACBASESERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFacBaseServerRegister : public GNET::Protocol
{
	#include "gfacbaseserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gfacbaseserverregister gs %d count %d", gs_id, base_count);
		FacBaseManager::GetInstance()->RegisterServer(gs_id, base_count);
	}
};

};

#endif
