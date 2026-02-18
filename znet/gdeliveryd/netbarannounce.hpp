
#ifndef __GNET_NETBARANNOUNCE_HPP
#define __GNET_NETBARANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class NetBarAnnounce : public GNET::Protocol
{
	#include "netbarannounce"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("NetBarAnnounce: user %d title %d netbartitle.size %d", userid, title, netbartitle.size());
		UserInfo *puser = UserContainer::GetInstance().FindUser(userid);
		if (puser == NULL)
			return;
		puser->netbar_master = title;
		if (netbartitle.size() <= 16)
			puser->netbar_title = netbartitle;
		try {
			Marshal::OctetsStream os(awardtype);
			os >> puser->netbar_level >> puser->award_type;
		} catch (...) {
			Log::log(LOG_ERR, "NetBarAnnounce: Invalid parameter!");
		}

		LOG_TRACE("NetBarAnnounce: user %d netbar_master = %d, netbar_level = %d, award_type = %d", userid, puser->netbar_master, puser->netbar_level, puser->award_type);
		Conf *conf = Conf::GetInstance();
		std::string netmarble = conf->find(GDeliveryServer::GetInstance()->Identification(), "korea_netmarble");
		if (puser->netbar_level != -1 && puser->award_type != -1 && puser->gameid != 0 && puser->gameid != _GAMESERVER_ID_INVALID && puser->roleid !=0 )
		{
			if (netmarble == "true" && puser->netbar_level >=1 && puser->netbar_level <= 3 && puser->award_type == 9)
			{
				for (int i = 1; i <= 4; i++)
				{
					GProviderServer::GetInstance()->DispatchProtocol(puser->gameid, NetBarReward(puser->roleid, puser->netbar_level, i));
					LOG_TRACE("Korea NetBarAnnounce roleid %d level %d type %d send gs %d", puser->roleid, puser->netbar_level, i, puser->gameid);
				}
			}
			else
				GProviderServer::GetInstance()->DispatchProtocol(puser->gameid, NetBarReward(puser->roleid, puser->netbar_level, puser->award_type));
		}
	}
};

};

#endif
