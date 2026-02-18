
#ifndef __GNET_GHIDESEEKRAIDAPPLY_HPP
#define __GNET_GHIDESEEKRAIDAPPLY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GHideSeekRaidApply : public GNET::Protocol
{
	#include "ghideseekraidapply"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int roleid=0;
		if(roles.size()>0)
		{
			roleid = roles[0].roleid;
		}
		LOG_TRACE("GHideSeekApply mapid=%d leader=%d issrc=%d, role.size=%d",
				map_id, roleid, issrc, roles.size());
	
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		if(!iscentral) // ÔÚÔ­·þ
		{
			RaidManager::GetInstance()->OnHideSeekRaidApply(map_id,roles,HST_HIDESEEK, false, sid, 0, 0);
			return;
		}
	
	}
};

};

#endif
