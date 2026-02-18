
#ifndef __GNET_SIEGEINFOGET_HPP
#define __GNET_SIEGEINFOGET_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "siegemanager.h"
#include "siegeinfoget_re.hpp"


namespace GNET
{

class SiegeInfoGet : public GNET::Protocol
{
	#include "siegeinfoget"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SiegeInfoGet_Re re;
		re.localsid = localsid;
		re.retcode = SiegeManager::GetInstance()->GetInfo(battle_id, re.city, re.challengers);
		LOG_TRACE("SiegeInfoGet roleid=%d,battle_id=%d,challengers.size=%d", roleid, battle_id,re.challengers.size());
		/*
		for (std::vector<GChallenger>::iterator it = re.challengers.begin(); it != re.challengers.end(); ++it)
		{
			LOG_TRACE("SiegeChallenger fid=%d, master=%d, begin_time=%d", it->challenger.fid, it->challenger.master, it->begin_time);
		}
		*/
		manager->Send(sid, re);
	}
};

};

#endif
