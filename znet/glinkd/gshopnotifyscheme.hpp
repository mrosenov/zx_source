
#ifndef __GNET_GSHOPNOTIFYSCHEME_HPP
#define __GNET_GSHOPNOTIFYSCHEME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GShopNotifyScheme : public GNET::Protocol
{
	#include "gshopnotifyscheme"

	class DispatchData
	{
		const GShopNotifyScheme* notify;
	public:
		DispatchData(const GShopNotifyScheme* msg):notify(msg) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,notify);
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
