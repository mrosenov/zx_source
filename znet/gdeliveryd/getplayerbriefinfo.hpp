
#ifndef __GNET_GETPLAYERBRIEFINFO_HPP
#define __GNET_GETPLAYERBRIEFINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "getplayerbriefinfo_re.hpp"

#include "getrolebase.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
namespace GNET
{

class GetPlayerBriefInfo : public GNET::Protocol
{
	#include "getplayerbriefinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GetPlayerBriefInfo_Re	getpbi_re(ERR_SUCCESS,roleid,localsid,PlayerBriefInfoVector(),reason);
		for (size_t i=0;i<playerlist.size();i++)
		{	
			int rid = playerlist[i];
			//LOG_TRACE( "******GetPlayerBriefInfo: asker=%d, roleid=%d",roleid,rid);
			GRoleInfo* info = RoleInfoCache::Instance().Get(rid);
			if (info) 
				getpbi_re.playerlist.add(PlayerBriefInfo(info->id,0,info->name,info->gender));
			else if(RoleInfoCache::Instance().IsInexistent(rid))
			{
				if (!GDeliveryServer::GetInstance()->IsCentralDS())
					break;
				else
				{
					char * str = "xx";
					Octets name_u2, username = Octets(str, strlen(str));
					CharsetConverter::conv_charset_t2u(username, name_u2);
					getpbi_re.playerlist.add(PlayerBriefInfo(rid, 0, name_u2, 0));
				}
			}
			else
			{
				GetRoleBase* rpc = (GetRoleBase*) Rpc::Call(RPC_GETROLEBASE,RoleId(rid));
				rpc->response_type = _RESPONSE_GET_PBI;
				rpc->roleid = roleid;
				rpc->link_sid = sid;
				rpc->localsid = localsid;
				rpc->reason = reason;
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
		}
		if (getpbi_re.playerlist.size())
			manager->Send(sid,getpbi_re);
	}
};

};

#endif
