
#ifndef __GNET_PKMESSAGE_HPP
#define __GNET_PKMESSAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PKMessage : public GNET::Protocol
{
	#include "pkmessage"
	class DispatchData
	{
		const PKMessage* pkmsg;
	public:
		DispatchData(const PKMessage* msg):pkmsg(msg) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,pkmsg);
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
