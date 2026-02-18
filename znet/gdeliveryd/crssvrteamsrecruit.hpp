
#ifndef __GNET_CRSSVRTEAMSRECRUIT_HPP
#define __GNET_CRSSVRTEAMSRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "crssvrteamsmanager.h"

namespace GNET
{

class CrssvrTeamsRecruit : public GNET::Protocol
{
	#include "crssvrteamsrecruit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("CrssvrTeamsRecruit,inviter=%d,invitee=%d,teamid=%d",roleid,invitee,teamid);
		
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to CrssvrTeamsRecruit on Central Delivery Server, refuse him!", roleid);
			return;
		}

		int inviter_localsid, inviter_linksid;

		// 邀请人是否在线
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pinviterinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL==pinviterinfo)
		{
			LOG_TRACE("CrssvrTeamsRecruit, roleid=%d, teamid=%d is not online", roleid, teamid);
			return;
		}
		inviter_linksid = pinviterinfo->linksid;
		inviter_localsid = pinviterinfo->localsid;

		// 邀请人的teamid验证
		int inviter_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
		if(inviter_teamid <= 0 || inviter_teamid != teamid)
		{
			LOG_TRACE("CrssvrTeamsRecruit, roleid=%d, inviter_teamid=%d, teamid=%d not equal", roleid, inviter_teamid, teamid);
			dsm->Send(inviter_linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_TEAMID_INVAILD, roleid, teamid, inviter_localsid));
			return;
		}

		if(invitee == 0)// 客户端没有得到目标的roleid
		{
			if(name.size() <= 0)
			{
				dsm->Send(inviter_linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_INVALIDNAME, roleid, teamid, inviter_localsid));
				return;
			}

			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			bool isfind = UserContainer::GetInstance().FindRoleId(name, invitee);

			LOG_TRACE("CrssvrTeamsRecruit rolename:%s, size=%d.invitee=%d", name.begin() ,name.size(), invitee);
			if(!isfind)
			{
				dsm->Send(inviter_linksid, CrssvrTeamsNotify_Re(ERR_CRSSVRTEAMS_NAME_NOT_FIND, roleid, teamid, inviter_localsid));
				return;
			}
		}

		CrssvrTeamsInviteArg arg;
		int invitee_linksid, invitee_localsid;
		int ret = CrssvrTeamsManager::Instance()->CanRecruit(teamid, roleid, invitee, invitee_linksid, invitee_localsid);
		if(ret != ERR_SUCCESS)
		{
			dsm->Send(inviter_linksid, CrssvrTeamsNotify_Re(ret, roleid, teamid, inviter_localsid));
			return;
		}

		LOG_TRACE("CrssvrTeamsRecruit,inviter=%d,invitee=%d,teamid=%d",roleid,invitee,teamid);

		arg.roleid = roleid;
		arg.teamid = teamid;
		arg.invitee = invitee;
		CrssvrTeamsInvite* rpc = (CrssvrTeamsInvite*) Rpc::Call( RPC_CRSSVRTEAMSINVITE,arg);
		rpc->roleid = invitee;
		rpc->linksid = inviter_linksid;
		rpc->localsid = inviter_localsid;
		rpc->teamid = teamid;
		dsm->Send(invitee_linksid, rpc);
	}
};

};

#endif
















