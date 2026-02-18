#ifndef __GNET_FACTIONRESIGN_HPP
#define __GNET_FACTIONRESIGN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FactionResign : public GNET::Protocol
{
	#include "factionresign"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DBFactionAppointArg arg;
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if(!pinfo || !pinfo->factionid || pinfo->factiontitle>=TITLE_HEADER|| pinfo->factiontitle==TITLE_MASTER) 
				return;
			arg.fid = pinfo->factionid;
		}
		arg.roleid = roleid;
		arg.title = TITLE_HEADER;

		Log::formatlog("FactionResign", "title=%d fid=%d roleid=%d", arg.title, arg.fid, arg.roleid);
		DBFactionAppoint* rpc = (DBFactionAppoint*) Rpc::Call( RPC_DBFACTIONAPPOINT,arg);
		rpc->roleid = roleid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
