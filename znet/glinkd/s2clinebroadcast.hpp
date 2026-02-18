
#ifndef __GNET_S2CLINEBROADCAST_HPP
#define __GNET_S2CLINEBROADCAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class S2CLineBroadcast : public GNET::Protocol
{
	#include "s2clinebroadcast"

	class DispatchData
	{
		const GamedataSend * pro;
		const int lineid;
	public:
		DispatchData(const GamedataSend * _pro, int  _line) : pro(_pro), lineid(_line) {}
		~DispatchData() {}
		void operator() (std::pair<const int, GNET::RoleData> pair)
		{
			if (pair.second.status == _STATUS_ONGAME && pair.second.gs_id == lineid)
				GLinkServer::GetInstance()->Send(pair.second.sid, pro);
		}	
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GamedataSend gds(data);
		GLinkServer* lsm = GLinkServer::GetInstance();
		if (lsm->roleinfomap.size())
			std::for_each(lsm->roleinfomap.begin(), lsm->roleinfomap.end(), 
					DispatchData(&gds, lineid));
	}
};

};

#endif
