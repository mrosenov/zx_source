
#ifndef __GNET_BATTLESERVERREGISTER_HPP
#define __GNET_BATTLESERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "openbanquetmanager.h"
#include "gdeliveryserver.hpp"
#include "battlemanager.h"

namespace GNET
{

class BattleServerRegister : public GNET::Protocol
{
	#include "battleserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("battleregister", "gs_id=%d:size=%d", gs_id, fields.size());
		std::vector<BattleFieldInfo> openbanquet_fields;
		std::vector<BattleFieldInfo> normal_fields;

		std::vector<BattleFieldInfo>::iterator it_field = fields.begin();
		for(; it_field != fields.end(); ++it_field)
		for(int i=0; i<(int)fields.size(); ++i)
		{
			if((*it_field).battle_type == BATTLEFIELD_TYPE_FLOW_CRSSVR)
			{
				openbanquet_fields.push_back(*it_field);
			}
			else
			{
				normal_fields.push_back(*it_field);
			}
		}
		
		LOG_TRACE("BattleServerRegister gs_id=%d:openbanquet.size=%d, normal_fields.size=%d", gs_id, openbanquet_fields.size(), normal_fields.size());
		if(GDeliveryServer::GetInstance()->IsCentralDS() && openbanquet_fields.size() > 0)
		{
			OpenBanquetManager::GetInstance()->RegisterServer(sid, gs_id, openbanquet_fields);
		}
		else
		{
			BattleManager::GetInstance()->RegisterServer(sid, gs_id, normal_fields);
		}
	}
};

};

#endif
