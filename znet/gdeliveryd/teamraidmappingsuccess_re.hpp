
#ifndef __GNET_TEAMRAIDMAPPINGSUCCESS_RE_HPP
#define __GNET_TEAMRAIDMAPPINGSUCCESS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"
#include "crossbattlemanager.hpp"

namespace GNET
{

class TeamRaidMappingSuccess_Re : public GNET::Protocol
{
	#include "teamraidmappingsuccess_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("TeamRaidMappingSuccess_Re roleid=%d, raidroom_id=%d, agree=%d, iscross=%d", roleid, raidroom_id, agree, iscross);

		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		if(!iscentral) // 在原服，需要将消息转发给跨服服务器
		{
			//LOG_TRACE("TeamRaidMappingSuccess_Re send to Central roleid=%d", roleid);
	                CentralDeliveryClient::GetInstance()->SendProtocol(*this);
			return;
		}

		if(iscentral) // 在跨服
		{
			//LOG_TRACE("TeamRaidMappingSuccess_Re Recv On Central roleid=%d", roleid);
			RaidManager::GetInstance()->OnTeamRaidMappingSuccess_Re(roleid,raidroom_id,agree);
		}
	}
};

};

#endif
