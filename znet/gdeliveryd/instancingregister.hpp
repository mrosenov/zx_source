
#ifndef __GNET_INSTANCING_REGISTER_HPP
#define __GNET_INSTANCING_REGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "instancingfieldinfo"

namespace GNET
{

class InstancingRegister : public GNET::Protocol
{
	#include "instancingregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("instancingregister", "gs_id=%d:size=%d", gs_id, fields.size());
		InstancingManager::GetInstance()->RegisterServer(sid, gs_id, fields);
	}
};

};

#endif
