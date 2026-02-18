
#ifndef __GNET_GKINGDOMBATTLEENTER_HPP
#define __GNET_GKINGDOMBATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleEnter : public GNET::Protocol
{
	#include "gkingdombattleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if ( NULL==pinfo )
			return;
		int retcode = KingdomManager::GetInstance()->TryEnterBattle(fieldtype, pinfo);
		if (retcode != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, KingdomBattleEnter_Re(retcode, pinfo->localsid));
		LOG_TRACE("gkingdombattleenter roleid %d fieldtype %d ret %d", roleid, fieldtype, retcode);
	}
};

};

#endif
