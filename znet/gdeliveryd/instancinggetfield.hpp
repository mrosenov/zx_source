
#ifndef __GNET_INSTANCINGGETFIELD_HPP
#define __GNET_INSTANCINGGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class InstancingGetField : public GNET::Protocol
{
	#include "instancinggetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();

		InstancingGetField_Re re;
		re.retcode = ERR_BATTLE_MAP_NOTEXIST;
		re.localsid = localsid;
		InstancingManager::GetInstance()->GetFieldInfo(roleid, gs_id, map_id, re);
		dsm->Send(sid, re);
	}
};

};

#endif
