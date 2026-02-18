
#ifndef __GNET_GKINGTRYCALLGUARDS_HPP
#define __GNET_GKINGTRYCALLGUARDS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingTryCallGuards : public GNET::Protocol
{
	#include "gkingtrycallguards"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo)
			return;
		int remain_times = 0;
		int retcode = KingdomManager::GetInstance()->TryCallGuards(roleid, pinfo->gameid, mapid, posx, posy, posz, remain_times);
		GDeliveryServer::GetInstance()->Send(pinfo->linksid, KingCallGuards_Re(retcode, pinfo->localsid, remain_times));
		LOG_TRACE("gkingtrycallguards roleid %d lineid %d mapid %d x %f y %f z %f, ret %d remain_times %d", roleid, pinfo->gameid, mapid, posx, posy, posz, retcode, remain_times);
	}
};

};

#endif
