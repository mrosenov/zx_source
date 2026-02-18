
#ifndef __GNET_INSTANCINGENTERFAIL_HPP
#define __GNET_INSTANCINGENTERFAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingEnterFail : public GNET::Protocol
{
	#include "instancingenterfail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int gs_id = GProviderServer::GetInstance()->GetGSID(sid);
		LOG_TRACE("instancingenterfail, roleid=%d gsid=%d mapid=%d", roleid, gs_id, map_id);
		InstancingManager::GetInstance()->OnBattleEnterFail(roleid, gs_id, map_id);
	}
};

};

#endif
