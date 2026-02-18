
#ifndef __GNET_GTTEAMMEMBERUPDATE_HPP
#define __GNET_GTTEAMMEMBERUPDATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GTTeamMemberUpdate : public GNET::Protocol
{
	#include "gtteammemberupdate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gtteammemberupdate teamid %lld member size %d oper=%d, sid=%d", teamid, members.size(), operation, sid);
		GTPlatformClient::GetInstance()->SendProtocol(this);
		//暂时不需要处理
	}
};

};

#endif
