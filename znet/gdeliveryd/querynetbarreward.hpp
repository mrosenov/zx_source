
#ifndef __GNET_QUERYNETBARREWARD_HPP
#define __GNET_QUERYNETBARREWARD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "netbarreward.hpp"
#include "gproviderserver.hpp"
#include "mapuser.h"


namespace GNET
{

class QueryNetBarReward : public GNET::Protocol
{
	#include "querynetbarreward"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (NULL==pinfo) 
			return;
		UserInfo* user = pinfo->user;
		if (user==NULL || user->status!=_STATUS_ONGAME)
			return;
		//韩国网吧奖励的award_type只能是9
		Conf *conf = Conf::GetInstance();
		std::string netmarble = conf->find(GDeliveryServer::GetInstance()->Identification(), "korea_netmarble");
		if (user->netbar_level != -1 && user->award_type != -1)
		{
			if (netmarble == "true" && user->award_type == 9 && user->netbar_level >= 1 && user->netbar_level <= 3)
			{
				for (int i = 1; i <= 4; i++)
				{
					GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, NetBarReward(roleid, user->netbar_level, i));
					LOG_TRACE("Korea GS %d Query NetBarReward roleid %d level %d type %d", pinfo->gameid, roleid, user->netbar_level, i);
				}
			}
			else
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, NetBarReward(roleid, user->netbar_level, user->award_type));
		}
	}
};

};

#endif
