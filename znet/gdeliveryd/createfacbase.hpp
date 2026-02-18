
#ifndef __GNET_CREATEFACBASE_HPP
#define __GNET_CREATEFACBASE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class CreateFacBase : public GNET::Protocol
{
	#include "createfacbase"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("createfacbase roleid %d", roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo || pinfo->factionid == 0)
			return;
		if (GDeliveryServer::GetInstance()->IsCentralDS())
			return;
		int ret = FactionManager::Instance()->CreateBase(pinfo->factionid, roleid);
		if (ret != ERR_SUCCESS)
			manager->Send(sid, CreateFacBase_Re(ret, roleid, localsid, 0));
	}
};

};

#endif
