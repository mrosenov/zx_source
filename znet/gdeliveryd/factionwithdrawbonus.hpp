
#ifndef __GNET_FACTIONWITHDRAWBONUS_HPP
#define __GNET_FACTIONWITHDRAWBONUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionmanager.h"
#include "factionwithdrawbonus_re.hpp"

namespace GNET
{

class FactionWithDrawBonus : public GNET::Protocol
{
	#include "factionwithdrawbonus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(!pinfo)
			return;
		int ret=FactionManager::Instance()->WithdrawFactionMoney(pinfo->factionid,roleid);
		if(ret)
		{
			FactionWithDrawBonus_Re re(ret,pinfo->localsid);
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, re);
		}
	}
};

};

#endif
