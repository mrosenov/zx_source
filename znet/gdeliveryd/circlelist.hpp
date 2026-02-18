
#ifndef __GNET_CIRCLELIST_HPP
#define __GNET_CIRCLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CircleList : public GNET::Protocol
{
	#include "circlelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	        if(CircleManager::Instance()->IsCircleOpen()==false)
			return;
		CircleList_Re list;
		list.localsid = localsid;
		CircleManager::Instance()->GetFullInfo(circleid, roleid, list);
		manager->Send(sid, list);
	}
};

};

#endif
