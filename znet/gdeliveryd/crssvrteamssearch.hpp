
#ifndef __GNET_CRSSVRTEAMSSEARCH_HPP
#define __GNET_CRSSVRTEAMSSEARCH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "dbcrosscrssvrteamssearch.hrp"
#include "centraldeliveryserver.hpp"

namespace GNET
{

class CrssvrTeamsSearch : public GNET::Protocol
{
	#include "crssvrteamssearch"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("CrssvrTeamsSearch roleid=%d, name.size=%d", roleid, name.size());
	
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();
		bool iscentral = dsm->IsCentralDS();
		if(!iscentral) // 在原服，需要将消息转发给跨服服务器
		{
			int zoneid = GDeliveryServer::GetInstance()->zoneid;
			(*this).zoneid = zoneid;		
	                CentralDeliveryClient::GetInstance()->SendProtocol(this);
			return;
		}
		else // 在跨服
		{

			LOG_TRACE("CrssvrTeamsSearch Recv On Central roleid=%d, name.size=%d, zoneid=%d", roleid, name.size(), zoneid);

			DBCrossCrssvrTeamsSearchArg arg;
			arg.roleid = roleid;
			arg.zoneid = zoneid;
			arg.name = name;
			DBCrossCrssvrTeamsSearch* rpc = (DBCrossCrssvrTeamsSearch*) Rpc::Call( RPC_DBCROSSCRSSVRTEAMSSEARCH,arg);
			GameDBClient::GetInstance()->SendProtocol(rpc);
			return;
		}
	}
};

};

#endif
