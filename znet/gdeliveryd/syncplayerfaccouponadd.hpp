
#ifndef __GNET_SYNCPLAYERFACCOUPONADD_HPP
#define __GNET_SYNCPLAYERFACCOUPONADD_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class SyncPlayerFacCouponAdd : public GNET::Protocol
{
	#include "syncplayerfaccouponadd"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("syncplayerfaccouponadd roleid %d value %lld", roleid, fac_coupon_add);
		GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(roleid);
		if (roleinfo != NULL)
		{
			roleinfo->fac_coupon_add = fac_coupon_add;
		}
	}
};

};

#endif
