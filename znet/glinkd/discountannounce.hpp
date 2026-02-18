
#ifndef __GNET_DISCOUNTANNOUNCE_HPP
#define __GNET_DISCOUNTANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "merchantdiscount"

namespace GNET
{

class DiscountAnnounce : public GNET::Protocol
{
	#include "discountannounce"

	class DispatchData
	{
		const DiscountAnnounce * announce;
	public:
		DispatchData(const DiscountAnnounce * anno) : announce(anno) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid, announce);
		}
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (reserved1 != 0)
		{
			lsm->Send(reserved1, this);
			return;
		}
		Log::trace("broadcast fastpay discount");
		Thread::RWLock::RDScoped l(lsm->locker_map);
		if ( lsm->roleinfomap.size() )
			std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this));
	}
};

};

#endif
