
#ifndef __GNET_QUERYREWARDTYPE_HPP
#define __GNET_QUERYREWARDTYPE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "maprewardtype.h"
#include "queryrewardtype_re.hpp"
namespace GNET
{

class QueryRewardType : public GNET::Protocol
{
	#include "queryrewardtype"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline( roleid );
		if(NULL == pinfo)
			return;
		UserInfo* user = pinfo->user;
		manager->Send( sid, QueryRewardType_Re( roleid, user->rewardtype, user->rewarddata,
					(Passwd::GetInstance().GetAlgo(user->userid) & ALGORITHM_FILL_INFO_MASK)));
	}
};

};

#endif
