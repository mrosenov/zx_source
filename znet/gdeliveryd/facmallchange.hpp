
#ifndef __GNET_FACMALLCHANGE_HPP
#define __GNET_FACMALLCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacMallChange : public GNET::Protocol
{
	#include "facmallchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("facmallchange fid %d size %d", fid, mall_indexes.size());
		FacBaseManager::GetInstance()->OnMallChange(fid, mall_indexes);
	}
};

};

#endif
