
#ifndef __GNET_PLAYERBASEINFO_HPP
#define __GNET_PLAYERBASEINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gdeliveryserver.hpp"
#include "playerbaseinfo_re.hpp"

#include "getrolebase.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
namespace GNET
{

class PlayerBaseInfo : public GNET::Protocol
{
	#include "playerbaseinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerBaseInfo_Re re(ERR_SUCCESS,roleid,localsid,GRoleBase());

		for (size_t i=0;i<playerlist.size();i++)
		{
			int rid = playerlist[i];
			//LOG_TRACE( "******PlayerBaseInfo: asker=%d, roleid=%d",roleid,rid);
			GRoleInfo* info = RoleInfoCache::Instance().Get(rid);
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
			else if(RoleInfoCache::Instance().IsInexistent(rid))
				break;
			else
			{
				GetRoleBase* rpc = (GetRoleBase*) Rpc::Call(RPC_GETROLEBASE,RoleId(rid));
				rpc->response_type = _RESPONSE_BASEINFO;
				rpc->roleid = roleid;
				rpc->link_sid = sid;
				rpc->localsid = localsid;
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
		}
	}
};

};

#endif
