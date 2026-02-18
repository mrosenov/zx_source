
#ifndef __GNET_CLAIMCIRCLEGRADBONUS_HPP
#define __GNET_CLAIMCIRCLEGRADBONUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ClaimCircleGradBonus : public GNET::Protocol
{
	#include "claimcirclegradbonus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	        if(CircleManager::Instance()->IsCircleOpen()==false)
			return;
	//	LOG_TRACE("ClaimCircleGradBonus roleid=%d",roleid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(pinfo)
		{
			BonusExp re(roleid, BONUS_CIRCLE);
			int64_t bonus_exp;
			unsigned int limit = 2000000000l;
			if(CircleManager::Instance()->GetGradBonus(circleid,roleid,bonus_exp,true))
			{
				re.bonus_exp = limit;
				while(bonus_exp> limit)	
				{
					bonus_exp -= limit;
					GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, re);
				}
				re.bonus_exp = bonus_exp;
	//			LOG_TRACE("ClaimCircleGradBonus bonus=%d",bonus_exp);
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, re);
			}
	/*		else
			{
				re.bonus_exp = 100000;
				LOG_TRACE("fake ClaimCircleGradBonus bonus=%d",re.bonus_exp);
				GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, re);
			}
	*/	}
	}
};

};

#endif
