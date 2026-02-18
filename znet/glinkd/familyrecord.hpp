
#ifndef __GNET_FAMILYRECORD_HPP
#define __GNET_FAMILYRECORD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include <algorithm>
namespace GNET
{

class FamilyRecord : public GNET::Protocol
{
	#include "familyrecord"

	class DispatchData
	{
		const FamilyRecord* data;
	public:
		DispatchData(const FamilyRecord* d):data(d) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,data);
		}	
	};
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm = GLinkServer::GetInstance();
		if (manager == lsm) //protocol from client
		{
			localsid = sid;
			GDeliveryClient::GetInstance()->SendProtocol( this );
			return;
		}

		if(localsid)
			lsm->Send(localsid,this);
		else
		{
			Thread::RWLock::RDScoped l(lsm->locker_map);
			if ( lsm->roleinfomap.size() )
				std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this));
		}
	}
};

};

#endif
