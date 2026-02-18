
#ifndef __GNET_SECTRECRUIT_HPP
#define __GNET_SECTRECRUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "sectmanager.h"
#include "sectrecruit_re.hpp"


namespace GNET
{

class SectRecruit : public GNET::Protocol
{
	#include "sectrecruit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pown = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(!pown || pown->level<105)
			return;

		SectInviteArg arg;
		SectRecruit_Re re;

		arg.roleid = roleid;
		arg.role_name = pown->name;
		arg.invitee = invitee;

		LOG_TRACE("SectRecruit roleid=%d, invitee=%d", roleid, invitee);
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(invitee);
		re.sectid = roleid;
		re.disciple = invitee;
		re.localsid = pown->localsid;
		if(!pinfo) 
		{
			re.retcode =  ERR_SECT_OFFLINE;
			manager->Send(sid, re);
			return;
		}
		if(pinfo->level>75)
		{
			re.retcode =  ERR_SECT_INVALIDLEVEL;
			manager->Send(sid, re);
			return;
		}
		if(pinfo->sectid)
		{
			re.retcode =  ERR_SECT_UNAVAILABLE;
			manager->Send(sid, re);
			return;
		}

		if(pown->sectid)
		{
			int ret = SectManager::Instance()->PreRecruit(roleid, invitee);
			if(ret)
			{
				re.retcode =  ret;
				manager->Send(sid, re);
				return;
			}
		}
		SectInvite* rpc = (SectInvite*) Rpc::Call( RPC_SECTINVITE,arg);
		rpc->localsid = pown->localsid;
		rpc->linksid = pown->linksid;
		manager->Send(pinfo->linksid, rpc);
	}
};

};

#endif
