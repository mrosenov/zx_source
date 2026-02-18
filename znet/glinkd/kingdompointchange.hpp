
#ifndef __GNET_KINGDOMPOINTCHANGE_HPP
#define __GNET_KINGDOMPOINTCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class KingdomPointChange : public GNET::Protocol
{
	#include "kingdompointchange"

	class DispatchData
	{
		const KingdomPointChange * notice;
	public:
		DispatchData(const KingdomPointChange * _notice) : notice(_notice) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME)
				GLinkServer::GetInstance()->Send(pair.second.sid, notice);
		}
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (localsid != 0)
		{
			lsm->Send(localsid ,this);	
			return;
		}
		if ( lsm->roleinfomap.size() )
			std::for_each(lsm->roleinfomap.begin(),lsm->roleinfomap.end(),DispatchData(this));
	}
};

};

#endif
