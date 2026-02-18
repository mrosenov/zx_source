
#ifndef __GNET_GFENGSHENRAIDAPPLY_HPP
#define __GNET_GFENGSHENRAIDAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "teamraidapplyingrole"

namespace GNET
{

class GFengShenRaidApply : public GNET::Protocol
{
	#include "gfengshenraidapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int roleid=0;
		if(roles.size() != 1)
		{
			return;
		}
		else
		{
			roleid = roles[0].roleid;
		}

		LOG_TRACE("GFengShenRaidApply mapid=%d leader=%d issrc=%d, role.size=%d",
				map_id, roleid, issrc, roles.size());
	
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		if(iscentral) // ÔÚ¿ç·þ
		{
			RaidManager::GetInstance()->OnHideSeekRaidApply(map_id,roles,HST_FENGSHEN, true, sid, 0, 0);
			return;
		}

	}
};

};

#endif
