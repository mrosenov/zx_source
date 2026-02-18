
#ifndef __GNET_TOPFLOWERMSG_RE_HPP
#define __GNET_TOPFLOWERMSG_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TopFlowerMsg_Re : public GNET::Protocol
{
	#include "topflowermsg_re"
	class DispatchData
	{
		const TopFlowerMsg_Re* flower_msg;
	public:
		DispatchData(const TopFlowerMsg_Re* msg):flower_msg(msg) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid,flower_msg);
		}	
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv TopFlowerMsg_Re, localsid:%d, myself_roleid:%d", localsid, myself_roleid);
		{
			GLinkServer* lsm=GLinkServer::GetInstance();
			Thread::RWLock::RDScoped l(lsm->locker_map);
			if ( lsm->roleinfomap.size() )
				std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this));
		}
	}
};

};

#endif
