
#ifndef __GNET_ACHIEVEMENTMESSAGE_HPP
#define __GNET_ACHIEVEMENTMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"

namespace GNET
{

class AchievementMessage : public GNET::Protocol
{
	#include "achievementmessage"
	class DispatchData
	{
		const AchievementMessage* achmsg;
	public:
		DispatchData(const AchievementMessage* msg):achmsg(msg) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,achmsg);
		}	
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		Thread::RWLock::RDScoped l(lsm->locker_map);
		if ( lsm->roleinfomap.size() )
			std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this));
	}
};

};

#endif
