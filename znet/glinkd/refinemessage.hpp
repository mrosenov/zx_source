
#ifndef __GNET_REFINEMESSAGE_HPP
#define __GNET_REFINEMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RefineMessage : public GNET::Protocol
{
	#include "refinemessage"
	
	class DispatchData
	{
		const RefineMessage* rmsg;
	public:
		DispatchData(const RefineMessage* msg):rmsg(msg) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,rmsg);
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
