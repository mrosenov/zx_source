
#ifndef __GNET_STOPFACBASE_RE_HPP
#define __GNET_STOPFACBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class StopFacBase_Re : public GNET::Protocol
{
	#include "stopfacbase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("stopfacbase_re ret %d fid %d gsid %d index %d", retcode, fid, gameid, base_tag);
		FacBaseManager::GetInstance()->OnBaseStop(retcode, fid, gameid, base_tag);
	}
};

};

#endif
