
#ifndef __GNET_CONTESTRESULT_HPP
#define __GNET_CONTESTRESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_contest_result(int role_id,int score,int place,int right_amount);

namespace GNET
{

class ContestResult : public GNET::Protocol
{
	#include "contestresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		player_contest_result(roleid,score,place,right_amount);
	}
};

};

#endif
