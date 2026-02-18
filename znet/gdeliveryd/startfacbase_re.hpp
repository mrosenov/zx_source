
#ifndef __GNET_STARTFACBASE_RE_HPP
#define __GNET_STARTFACBASE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "facbasemanager.h"

namespace GNET
{

class StartFacBase_Re : public GNET::Protocol
{
	#include "startfacbase_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("startfacbase_re ret %d fid %d gsid %d index %d mall_indexes size %d", retcode, fid, gsid, index, mall_indexes.size());
		FacBaseManager::GetInstance()->OnBaseStart(retcode, fid, gsid, index, mall_indexes);
	}
};

};

#endif
