
#ifndef __GNET_INSTANCINGAVAILABLELIST_HPP
#define __GNET_INSTANCINGAVAILABLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingAvailableList : public GNET::Protocol
{
	#include "instancingavailablelist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("InstancingAvailablelist: roleid=%d", roleid);
		InstancingAvailableList_Re re(ERR_SUCCESS, roleid);
		re.localsid = localsid;

		std::set<int> fields; 
		InstancingManager::GetInstance()->GetBattleTidList(fields);

		re.fields.assign(fields.begin(), fields.end());
		for (std::vector<int>::iterator it = re.fields.begin(), ie = re.fields.end(); it != ie; ++it)
			LOG_TRACE("BattleTid:%d", *it);
		manager->Send(sid, re);
	}
};

};

#endif
