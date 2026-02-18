
#ifndef __GNET_PLAYERLOGIN_HPP
#define __GNET_PLAYERLOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "blockedrole.h"
#include "mapforbid.h"

namespace GNET
{

class PlayerLogin : public GNET::Protocol
{
	#include "playerlogin"
	void SendFailResult(GDeliveryServer* dsm,Manager::Session::ID sid,int retcode);
	void SendForbidInfo(GDeliveryServer* dsm,Manager::Session::ID sid,const GRoleForbid& forbid);
	bool PermitLogin(GDeliveryServer* dsm,Manager::Session::ID sid);
	void DoLogin(Manager::Session::ID sid, int flag_);
	int TryRemoteLogin();

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("gdelivery::receive playerlogin from link,roleid=%d,lineid=%d,linkid=%d,localsid=%d,flag=%d\n",
			roleid,lineid,provider_link_id,localsid,flag);
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		//是否是预注册阶段 禁止登陆角色
		if (!ForbidLogin::GetInstance().IsRoleLoginAllowedGlobal())
		{
			SendFailResult(dsm, sid, ERR_FORBIDROLE_GLOBAL);
			return;
		}

		if (RoleInfoCache::Instance().IsRoleDeleted(roleid) || BlockedRole::GetInstance()->IsRoleBlocked(roleid) || RoleInfoCache::Instance().IsRoleSelling(roleid))
			return;

		bool is_central = dsm->IsCentralDS();
		bool is_frozen = RoleInfoCache::Instance().IsRoleFrozen(roleid);
		if (is_central)
		{
			if (!IsTransToCentralDS(flag) && flag != DIRECT_TO_CENTRALDS)
				return;
		}
		else
		{
			if (flag != CENTRALDS_TO_DS && flag != 0)
				return;
			if (flag == 0 && is_frozen)
				flag = DIRECT_TO_CENTRALDS;
/*
			if (flag == CENTRALDS_TO_DS)
				flag = 0;//跨服返回到原服登录逻辑与普通登录到原服逻辑一致
*/
		}

		if (flag == DIRECT_TO_CENTRALDS && !is_central)
		{
			int ret = TryRemoteLogin();
			if (ret != ERR_SUCCESS)
				SendFailResult(dsm, sid, ret);
		}
		else
		{
			if (is_frozen)
				return;
			if ( !PermitLogin(dsm,sid) )
				return;
			//old version
			DoLogin(sid, flag);
		}
	}
};

};

#endif
