
#ifndef __GNET_REGISTERGLOBALCOUNTER_HPP
#define __GNET_REGISTERGLOBALCOUNTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "globaldropmanager.h"
#include "globalcounterinfo"

namespace GNET
{

class RegisterGlobalCounter : public GNET::Protocol
{
	#include "registerglobalcounter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("RegisterGlobalCounter size=%d",fields.size());
		GlobalDropManager::GetInstance()->RegisterNewCounter(fields);
	}
};

};

#endif
