
#ifndef __GNET_INSTANCINGGETLIST_HPP
#define __GNET_INSTANCINGGETLIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingGetList : public GNET::Protocol
{
	#include "instancinggetlist"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("InstancingGetList: roleid=%d, battle_tid=%d", roleid, battle_tid);
		InstancingGetList_Re re(ERR_SUCCESS, battle_tid);
		re.localsid = localsid;
		InstancingManager::GetInstance()->GetMap(roleid, battle_tid, re.fields, re.enter_count);
		LOG_TRACE("InstancingGetList_Re: retcode=%d, battle_tid=%d, fields_size=%d, localsid=%d", re.retcode, re.battle_tid, re.fields.size(), re.localsid);
		for (std::vector<GInstancingFieldInfo>::iterator it = re.fields.begin(), ie = re.fields.end(); it != ie; ++it)
			LOG_TRACE("Field:gs_id=%d, map_id=%d", it->gs_id, it->map_id);
		manager->Send(sid, re);
	}
};

};

#endif
