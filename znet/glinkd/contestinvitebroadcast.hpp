
#ifndef __GNET_CONTESTINVITEBROADCAST_HPP
#define __GNET_CONTESTINVITEBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "contestinvite.hpp"


namespace GNET
{

class ContestInviteBroadcast : public GNET::Protocol
{
	#include "contestinvitebroadcast"
	class DispatchData
	{
		const ContestInvite * invite;
	public:
		DispatchData(const ContestInvite * inv):invite(inv) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,invite);
		}	
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		ContestInvite invite(0, remain_time, 0);
		GLinkServer* lsm=GLinkServer::GetInstance();
		Thread::RWLock::RDScoped l(lsm->locker_map);
		if ( lsm->roleinfomap.size() )
			std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(&invite));
	
	}
};

};

#endif
