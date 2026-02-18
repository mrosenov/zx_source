
#ifndef __GNET_GFACBASEENTER_HPP
#define __GNET_GFACBASEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "facbasemanager.h"

namespace GNET
{

class GFacBaseEnter : public GNET::Protocol
{
	#include "gfacbaseenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo || pinfo->factionid == 0)
			return;
		int dst_gs = 0, map_tag = 0;
		int ret = FacBaseManager::GetInstance()->GetBaseIndex(pinfo->factionid, dst_gs, map_tag);
		if (ret != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, FacBaseEnter_Re(ret, roleid, pinfo->localsid));
		else
			manager->Send(sid, PlayerChangeGS(roleid, pinfo->localsid, dst_gs, map_tag, CHG_GS_REASON_ENTER_FBASE));
		LOG_TRACE("gfacbaseenter roleid %d ret %d", roleid, ret);
	}
};

};

#endif
