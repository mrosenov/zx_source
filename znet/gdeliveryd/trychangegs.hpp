
#ifndef __GNET_TRYCHANGEGS_HPP
#define __GNET_TRYCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "blockedrole.h"
#include "trychangegs_re.hpp"

namespace GNET
{

class TryChangeGS : public GNET::Protocol
{
	#include "trychangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//如果通过PlayerChangeGS_Re返回错误码 需要填充gslineid
		LOG_TRACE("trychangegs:roleid=%d, dst_lineid=%d",roleid, dst_lineid);
		if (!GDeliveryServer::GetInstance()->IsCentralDS())
			return;
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL || pinfo->localsid != localsid || pinfo->gameid == dst_lineid || BlockedRole::GetInstance()->IsRoleBlocked(roleid))
			return;

		// 当前线如果不是正常线路，不让换线 
		if(!GProviderServer::GetInstance()->IsNormalLine(pinfo->gameid))
		{
			GDeliveryServer::GetInstance()->Send( pinfo->linksid, TryChangeGS_Re(ERR_NO_LINE_CHGS, roleid, dst_lineid, localsid, 0));
			Log::log(LOG_ERR, "trychangegs pinfo->gameid:%d is not normaline", pinfo->gameid);
			return;
		}

		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, localsid, dst_lineid, -1));
	}
};

};

#endif
