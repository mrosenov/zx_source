
#ifndef __GNET_BATTLEGETFIELD_HPP
#define __GNET_BATTLEGETFIELD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "battlemanager.h"
#include "battlegetfield_re.hpp"
#include "centraldeliveryserver.hpp"
#include "crossbattlemanager.hpp"

namespace GNET
{

class BattleGetField : public GNET::Protocol
{
	#include "battlegetfield"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();

		BattleGetField_Re re;
		re.retcode = ERR_BATTLE_MAP_NOTEXIST;
		re.roleid = roleid;
		re.localsid = localsid;
		re.iscross = iscross;
		BattleManager::GetInstance()->GetFieldInfo(gs_id, map_id, roleid, re);
		LOG_TRACE("BattleGetField: roleid=%d, gs_id=%d, map_id=%d, retcode=%d, iscross=%d", roleid, gs_id, map_id, re.retcode, iscross);
		bool iscentralds = dsm->IsCentralDS();
		if(iscross && !iscentralds)
		{
			CentralDeliveryClient::GetInstance()->SendProtocol(this);
		}
		else
		{
			manager->Send(sid, re);
		}
	}
};

};

#endif
