
#ifndef __GNET_FACTIONRECRUIT_HPP
#define __GNET_FACTIONRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FactionRecruit : public GNET::Protocol
{
#include "factionrecruit"
	void InviteFaction(Manager *manager, Manager::Session::ID sid)
	{
		int localsid, linksid;

		FactionInviteArg arg;
		FactionRecruit_Re re;

		arg.roleid = roleid;
		arg.factionid = factionid;
		arg.scale = scale;

		re.factionid = factionid;
		re.inviter = roleid;
		re.invitee = invitee;
		re.scale = scale;	
		int ret = FactionManager::Instance()->OnRecruit(factionid, arg.faction_name);
		if(ret==2)
			return;
		else
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->factionid!=factionid || pinfo->factiontitle>TITLE_CAPTAIN) 
				return;

			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
			arg.role_name = pinfo->name;
			re.localsid = localsid;
			if(ret==1)
			{
				re.retcode = ERR_FC_FULL;
				manager->Send(sid, re);
				return;
			}

			pinfo = UserContainer::GetInstance().FindRoleOnline(invitee);
			if (NULL==pinfo )
			{
				re.retcode = ERR_FC_OFFLINE;
				manager->Send(sid, re);
				return;
			}

			invitee = pinfo->familyid;
			FamilyInfo * finfo = FamilyManager::Instance()->Find(invitee);
			if (!finfo)
			{
				re.retcode = ERR_FC_INVITEENOFAMILY;
				manager->Send(sid, re);
				return;
			}
			if (finfo->info.factionid)
			{
				re.retcode = ERR_FC_ACCEPT_REACCEPT;
				manager->Send(sid, re);
				return;
			}
			if (Timer::GetTime() - finfo->info.jointime <= 24 * 3600) // 1 day
			{
				re.retcode = ERR_FC_RECRUIT_FAMILY_CD; 
				manager->Send(sid, re);
				return;	
			}
			pinfo = UserContainer::GetInstance().FindRoleOnline(finfo->info.master);
			if (NULL == pinfo )
			{
				re.retcode = ERR_FC_OFFLINE;
				re.invitee = finfo->info.master;
				manager->Send(sid, re);
				return;
			}

			arg.invitee = finfo->info.master;
			FactionInvite* rpc = (FactionInvite*) Rpc::Call( RPC_FACTIONINVITE,arg);
			rpc->linksid = linksid;
			rpc->familyid = pinfo->familyid;
			rpc->localsid = localsid;
			manager->Send(pinfo->linksid, rpc);
		}
	}
	void InviteFamily(Manager *manager, Manager::Session::ID sid)
	{
		int localsid, linksid;

		FactionInviteArg arg;
		FactionRecruit_Re re;

		arg.roleid = roleid;
		arg.invitee = invitee;
		arg.factionid = factionid;
		arg.scale = scale;

		re.factionid = factionid;
		re.inviter = roleid;
		re.invitee = invitee;
		re.scale = scale;	
		int ret = FamilyManager::Instance()->OnRecruit(factionid, arg.faction_name);
		
		if(ret==2)
			return;
		else
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo || pinfo->familyid!=factionid || pinfo->factiontitle > TITLE_HEADER) 
				return;
			 
			linksid = pinfo->linksid;
			localsid = pinfo->localsid;
			arg.role_name = pinfo->name;
			re.localsid = localsid;

			if(ret==1)
			{
				re.retcode = ERR_FC_FULL;
				manager->Send(sid, re);
				return;
			}
			pinfo = UserContainer::GetInstance().FindRoleOnline(invitee);
			if (NULL==pinfo ) 
			{
				re.retcode = ERR_FC_OFFLINE;
				manager->Send(sid, re);
				return;
			}
			if (pinfo->level < 20)
			{
				re.retcode = ERR_FC_INVITEELEVEL;
				manager->Send(sid, re);
				return;
			}
			if (pinfo->familyid) 
			{
				re.retcode = ERR_FC_ACCEPT_REACCEPT;
				manager->Send(sid, re);
				return;
			}
			else if (Timer::GetTime() - pinfo->jointime <= 24 * 3600) // 1 day
			{
				re.retcode = ERR_FC_LEAVINGFAMILY;
				manager->Send(sid, re);
				return;	
			}

			FactionInvite* rpc = (FactionInvite*) Rpc::Call( RPC_FACTIONINVITE,arg);
			rpc->linksid = linksid;
			rpc->localsid = localsid;
			manager->Send(pinfo->linksid, rpc);
		}
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("FactionRecruit,inviter=%d,invitee=%d,factionid=%d,scale=%d",roleid,invitee,factionid,scale);

		switch (scale)	
		{
			case 0: //faction
				InviteFaction(manager, sid);
				break;
			case 1: // family
				InviteFamily(manager, sid);
				break;
		}
	}

};

};
#endif
