
#ifndef __GNET_STARTFACTIONMULTIEXP_HPP
#define __GNET_STARTFACTIONMULTIEXP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StartFactionMultiExp : public GNET::Protocol
{
	#include "startfactionmultiexp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//默认帮派一定加载 且此处一定可以成功
		Log::formatlog("startfactionmultiexp", "fid %d multi %d end_time %d", fid, multi, end_time);
		FactionManager::Instance()->OnStartMultiExp(fid, multi, end_time);
	}
};

};

#endif
