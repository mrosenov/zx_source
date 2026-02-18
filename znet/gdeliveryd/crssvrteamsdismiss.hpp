
#ifndef __GNET_CRSSVRTEAMSDISMISS_HPP
#define __GNET_CRSSVRTEAMSDISMISS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "crssvrteamsmanager.h"

namespace GNET
{

class CrssvrTeamsDismiss : public GNET::Protocol
{
	#include "crssvrteamsdismiss"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo;
		int linksid;
		Log::formatlog("CrssvrTeamsDismiss", "teamid=%d:roleid=%d",teamid,roleid);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsDismiss on Central Delivery Server, refuse him!", roleid);
			return;
		}

		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinfo)
		{
			LOG_TRACE("CrssvrTeamsDismiss, roleid=%d is not online", roleid);
			return;
		}
		localsid = pinfo->localsid;
		linksid  = pinfo->linksid;

		// teamid验证
		int role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
		if(role_teamid <= 0 || role_teamid != teamid)
		{
			LOG_TRACE("CrssvrTeamsDismiss, roleid=%d, role_teamid=%d, teamid=%d not equal \n", roleid, role_teamid, teamid);
			dsm->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_TEAMID_INVAILD, roleid, teamid, localsid));
			return;
		}

		// 看跨服服务器是否连接, 解散队伍需要先判断
		bool central_conn = CentralDeliveryClient::GetInstance()->IsConnect();
		if(!central_conn)
		{
			LOG_TRACE("CrssvrTeamsDismiss, roleid=%d, role_teamid=%d, teamid=%d central_conn is false", roleid, role_teamid, teamid);
			dsm->Send(linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_CROSS_DISCONNECT, roleid, teamid, localsid));
			return;
		}

		// 是否可以解散
		int ret = CrssvrTeamsManager::Instance()->CanDismiss(teamid,roleid);
		if(!ret)
		{
			DBCrssvrTeamsDel* rpc = (DBCrssvrTeamsDel*) Rpc::Call( RPC_DBCRSSVRTEAMSDEL,TeamId(teamid));
			rpc->localsid = localsid;
			rpc->linksid = linksid;
			rpc->roleid = roleid;
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
			dsm->Send(linksid, CrssvrTeamsNotify_Re(ret, roleid, teamid, localsid));
	}
};

};

#endif
