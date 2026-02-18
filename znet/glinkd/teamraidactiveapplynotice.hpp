
#ifndef __GNET_TEAMRAIDACTIVEAPPLYNOTICE_HPP
#define __GNET_TEAMRAIDACTIVEAPPLYNOTICE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TeamRaidActiveApplyNotice : public GNET::Protocol
{
	#include "teamraidactiveapplynotice"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
