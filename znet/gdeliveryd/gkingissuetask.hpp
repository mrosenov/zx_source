
#ifndef __GNET_GKINGISSUETASK_HPP
#define __GNET_GKINGISSUETASK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GKingIssueTask : public GNET::Protocol
{
	#include "gkingissuetask"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo)
			return;
		int ret = KingdomManager::GetInstance()->IssueTask(roleid, task_type);
		if (ret != ERR_SUCCESS)
		{
			manager->Send(sid, GKingIssueTask_Re(ret, roleid, task_type));
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, KingIssueTask_Re(ret, roleid, task_type, pinfo->localsid));
		}
		LOG_TRACE("gkingissuetask roleid %d type %d, ret %d", roleid, task_type, ret);
	}
};

};

#endif
