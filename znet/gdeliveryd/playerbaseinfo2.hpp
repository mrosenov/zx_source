
#ifndef __GNET_PLAYERBASEINFO2_HPP
#define __GNET_PLAYERBASEINFO2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerBaseInfo2 : public GNET::Protocol
{
	#include "playerbaseinfo2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerBaseInfo2_Re re(ERR_SUCCESS,roleid,localsid,cloneid,GRoleBase());
		GRoleInfo* info = RoleInfoCache::Instance().Get(playerid);
		if(info)
		{
			re.player.id = info->id;
			re.player.name = info->name;
			re.player.faceid = info->faceid;
			re.player.hairid = info->hairid;
			re.player.gender = info->gender;
			re.player.earid = info->earid;
			re.player.tailid = info->tailid;
			re.player.fashionid = info->fashionid;
			manager->Send(sid,re);
		}
		else if(!RoleInfoCache::Instance().IsInexistent(playerid))
		{
			GetRoleBase* rpc = (GetRoleBase*) Rpc::Call(RPC_GETROLEBASE,RoleId(playerid));
			rpc->response_type = _RESPONSE_BASEINFO2;
			rpc->roleid = roleid;
			rpc->cloneid = cloneid;
			rpc->link_sid = sid;
			rpc->localsid = localsid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
	}
};

};

#endif
