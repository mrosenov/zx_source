
#ifndef __GNET_GRAIDJOIN_HPP
#define __GNET_GRAIDJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
#include "raidjoin_re.hpp"
namespace GNET
{

class GRaidJoin : public GNET::Protocol
{
	#include "graidjoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int ret=RaidManager::GetInstance()->OnGRaidJoin(roleid,map_id,raidroom_id,groupid);
		LOG_TRACE("GRaidJoin: roleid=%d, map_id=%d, raidroom_id=%d groupid=%d ret=%d", roleid, map_id, raidroom_id, groupid,ret);
		if(ret==0)
			return;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(pinfo)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, RaidJoin_Re(ret,raidroom_id,map_id,pinfo->localsid));
	}
};

};

#endif
