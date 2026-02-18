
#ifndef __GNET_SENDREFLEVELUP_HPP
#define __GNET_SENDREFLEVELUP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "referencemanager.h"

namespace GNET
{

class SendRefLevelUp : public GNET::Protocol
{
	#include "sendreflevelup"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("SendRefLevelUp roleid %d exp %lld level %d reborn_cnt %d", roleid, exp_cont, level, reborn_cnt);
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(roleid);
			if (roleinfo && roleinfo->src_zoneid != 0)
			{
				if (CentralDeliveryServer::GetInstance()->DispatchProtocol(roleinfo->src_zoneid, this))
					LOG_TRACE("SendRefLevelUp roleid %d dispatch to src_zoneid %d", roleid, roleinfo->src_zoneid);
				else
					Log::log(LOG_ERR, "SendRefLevelUp roleid %d exp %lld level %d reborn_cnt %d dispatch to src_zoneid %d error", roleid, exp_cont, level, reborn_cnt, roleinfo->src_zoneid);
			}
			else
				Log::log(LOG_ERR, "SendRefLevelUp roleid %d src_zoneid %d invalid", roleid, roleinfo==NULL?-1:0);
		}
		else
			ReferenceManager::GetInstance()->OnReferralLevelUp(roleid, exp_cont, level, reborn_cnt);
	}
};

};

#endif
