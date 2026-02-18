
#ifndef __GNET_GHOSTILEPROTECT_HPP
#define __GNET_GHOSTILEPROTECT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "dbhostileprotect.hrp"
#include "hostileprotect_re.hpp"
#include "gmailendsync.hpp"

namespace GNET
{

class GHostileProtect : public GNET::Protocol
{
	#include "ghostileprotect"
	bool CheckItem()
	{
		GRoleInventoryVector::iterator it;
		for (it = syncdata.inventory.items.begin(); it != syncdata.inventory.items.end(); ++ it)
		{
			if (it->pos == item_pos)
			{
				if (it->id == (unsigned int)item_id && it->count >= item_num)
					return true;
				break;
			}
		}
		return false;
	}

	void SendErr( int errcode, UserInfo& ui)
	{
		GDeliveryServer::GetInstance()->Send(ui.linksid, HostileProtect_Re(errcode, factionid, ui.localsid));
		syncdata.inventory.items.clear();
		GProviderServer::GetInstance()->DispatchProtocol(ui.gameid, GMailEndSync(0,errcode, roleid, syncdata));
	}


	void Process(Manager *manager, Manager::Session::ID sid)
	{
		return;
		/*
		LOG_TRACE("GHostileProtect, roleid=%d, factionid=%d", roleid, factionid);
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		int localsid;
		int linksid;
		{
			UserInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo) 
				return;
			if (pinfo->factionid!=(unsigned int)factionid || pinfo->factiontitle>TITLE_VICEMASTER)
			{
				SendErr(ERR_HOSTILE_PRIVILEGE, *pinfo);
				return;
			}
			if (!CheckItem())
			{
				SendErr(ERR_HOSTILE_PROTECTITEM, *pinfo);
				return;
			}
			linksid = pinfo->sid;
			localsid = pinfo->localsid;
		}
		DBHostileProtectArg arg;
		arg.roleid = roleid;
		arg.factionid = factionid;
		arg.syncdata = syncdata;
		arg.item_pos = item_pos;
		arg.item_id = item_id;
		arg.item_num = item_num;
		DBHostileProtect* rpc = (DBHostileProtect*) Rpc::Call( RPC_DBHOSTILEPROTECT, arg);
		rpc->linksid = linksid;
		rpc->localsid = localsid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		*/
	}
};

};

#endif
