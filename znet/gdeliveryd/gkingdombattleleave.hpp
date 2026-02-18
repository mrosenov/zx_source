
#ifndef __GNET_GKINGDOMBATTLELEAVE_HPP
#define __GNET_GKINGDOMBATTLELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingdomBattleLeave : public GNET::Protocol
{
	#include "gkingdombattleleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL)
			return;
		int retcode = ERR_SUCCESS;
		//retcode = KingdomManager::GetInstance()->TryLeaveBattle(fieldtype, tagid, roleid); //战场结束之后 才会传出玩家
		if (retcode != ERR_SUCCESS)
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, KingdomBattleLeave_Re(retcode, pinfo->localsid));
		if (retcode == ERR_SUCCESS && pinfo->old_gs != _GAMESERVER_ID_INVALID)
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, pinfo->old_gs, map_tag, 0, scalex, scaley, scalez));
		LOG_TRACE("gkingdombattleleave:roleid=%d,fieldtype=%d,map=%d,x=%f,y=%f,z=%f,retcode=%d",roleid, fieldtype, map_tag, scalex, scaley, scalez, retcode);
	}
};

};

#endif
