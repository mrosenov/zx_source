
#ifndef __GNET_ROLELIST_HPP
#define __GNET_ROLELIST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "getuser.hrp"
#include "getroleinfo.hrp"
#include "rolelist_re.hpp"
#include "gdeliveryserver.hpp"

#include "gauthclient.hpp"
#include "mapuser.h"
namespace GNET
{

class RoleList : public GNET::Protocol
{
	#include "rolelist"
	void GetNextRole(UserInfo* userinfo,Manager::Session::ID sid)
	{
		int referrer = 0;
		if (userinfo->rolelist.GetRoleCount() == 0)
			referrer = userinfo->real_referrer>0?userinfo->real_referrer:userinfo->au_suggest_referrer;
		else
			referrer = userinfo->real_referrer;

		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		int next_role=userinfo->rolelist.GetNextRole();
		if (next_role==_HANDLE_END)
		{
			dsm->Send(sid,RoleList_Re(ERR_SUCCESS,_HANDLE_END,userid,localsid,RoleInfoVector(), referrer));
			return;
		}
		int roleid = userinfo->logicuid+next_role;
		GRoleInfo* info = RoleInfoCache::Instance().GetOnlist(roleid);
		if(info && (info->status&CACHESTATUS_COMPLETE)!=0)
		{
			//LOG_TRACE("******role cache hit roleid=%d", roleid);
			GRoleInfo roleinfo_chop = RoleInfoCache::ChopRoleInfo(*info);
			RoleList_Re re;
			re.result = ERR_SUCCESS;
			re.handle = (roleid%MAX_ROLE_COUNT);
			re.userid = userid;
			re.localsid = localsid;
			RoleInfo roleinfo;
			re.rolelist.add(RoleInfoCache::ConvertRoleInfo(roleinfo_chop, roleinfo));
			re.referrer = referrer;
			GDeliveryServer::GetInstance()->Send(sid,re);
			return;
		}
		GetRoleInfo* rpc = (GetRoleInfo*) Rpc::Call(RPC_GETROLEINFO,RoleId(roleid));
		rpc->userid = userid;
		rpc->source = GetRoleInfo::SOURCE_LOCAL;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		UserInfo* userinfo=UserContainer::GetInstance().FindUser(userid);
		if (NULL==userinfo) 
			return; 
		if (GDeliveryServer::GetInstance()->IsCentralDS())
		{
			Log::log(LOG_ERR, "User %d try to Get RoleList from CentralDS, refuse him!", userid);
			return;
		}

		/* send request to game DB */
		if (!userinfo->rolelist.IsRoleListInitialed()) 
		{
			GetUser* rpc=(GetUser*) Rpc::Call(RPC_GETUSER,UserArg(userid, userinfo->ip));
			rpc->save_link_sid=sid;
			rpc->save_localsid=localsid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
		{
			GetNextRole(userinfo,sid);
		}
		return;
	}
};

};

#endif
