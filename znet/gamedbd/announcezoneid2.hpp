
#ifndef __GNET_ANNOUNCEZONEID2_HPP
#define __GNET_ANNOUNCEZONEID2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gamedbserver.hpp"
namespace GNET
{

class AnnounceZoneid2 : public GNET::Protocol
{
	#include "announcezoneid2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//only delivery send this protocol at start-up
		if (blreset != (char)GameDBManager::GetInstance()->IsCentralDB())
		{
			Log::log(LOG_ERR, "AnnounceZoneid, DS type %d does not match DB type, disconnect", blreset);
			manager->Close(sid);
			return;
		}

		GameDBServer::GetInstance()->delivery_sid=sid;
		GameDBServer::GetInstance()->delivery_zoneid=zoneid;
		if(zoneid!=GameDBServer::GetInstance()->zoneid)
			Log::log( LOG_ERR, "announcezoneid2,zoneid=%d delivery_zoneid=%d unequal\n",GameDBServer::GetInstance()->zoneid,zoneid);
		DEBUG_PRINT("announcezoneid2,zoneid=%d delivery_zoneid=%d\n",GameDBServer::GetInstance()->zoneid,zoneid);

	}
};

};

#endif
