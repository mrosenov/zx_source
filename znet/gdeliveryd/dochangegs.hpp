
#ifndef __GNET_DOCHANGEGS_HPP
#define __GNET_DOCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "raidmanager.h"

namespace GNET
{

class DoChangeGS : public GNET::Protocol
{
	#include "dochangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("dochangegs roleid %d dst_lineid %d localsid %d", roleid, dst_lineid, localsid);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo == NULL || pinfo->localsid != localsid || pinfo->chgs_dstlineid != dst_lineid || pinfo->user == NULL || pinfo->user->status != _STATUS_SWITCH)
			return;
		if(GProviderServer::GetInstance()->IsNormalLine(dst_lineid))//回原线时检查人数限制，进特殊线目前不查
		{
			int ret = GProviderServer::GetInstance()->SelectLine(dst_lineid);
			if(pinfo->chgs_dstlineid != dst_lineid)
				pinfo->chgs_dstlineid = dst_lineid;
    	        	if(ret != ERR_SUCCESS)
			{
//				GDeliveryServer::GetInstance()->Send(pinfo->linksid, DisconnectPlayer( roleid, -1, pinfo->localsid, -1 ));
	                        UserContainer::GetInstance().UserLogout(pinfo->user);
        	                return;
                	}
		}	
		PlayerLogin pl( roleid, dst_lineid, pinfo->user->linkid, localsid );
		pl.auth = pinfo->user->privileges;
		pl.usepos = 1; //只要是 changegs 逻辑就用
		pl.x = pinfo->chgs_pos.x;
		pl.y = pinfo->chgs_pos.y;
		pl.z = pinfo->chgs_pos.z;
		pl.worldtag = pinfo->chgs_mapid;
		pl.loginip = pinfo->user->ip;
		pl.territory_role = pinfo->chgs_territoryrole;
                if(RaidManager::GetInstance()->IsRaidLine(dst_lineid))
		{
			RaidManager::GetInstance()->SetRoleGroup(roleid,pl.territory_role);
		}	
		LOG_TRACE("dochangegs roleid %d dst_lineid %d localsid %d worldtag=%d x=%f y=%f, z=%f group=%d", roleid, dst_lineid, localsid,pl.worldtag ,pl.x,pl.y,pl.z,pl.territory_role);
		if (GProviderServer::GetInstance()->DispatchProtocol(dst_lineid, pl))
		{
			ForbiddenUsers::GetInstance().Push(pinfo->userid,roleid,pinfo->user->status);
			/*
			pinfo->old_gs = pinfo->gameid;
			pinfo->gameid = dst_lineid;
			pinfo->user->gameid = dst_lineid;
			*/
		}
	}
};

};

#endif
