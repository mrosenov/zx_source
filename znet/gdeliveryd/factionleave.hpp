
#ifndef __GNET_FACTIONLEAVE_HPP
#define __GNET_FACTIONLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbfamilyleave.hrp"
#include "dbfactionleave.hrp"

namespace GNET
{

class FactionLeave : public GNET::Protocol
{
	#include "factionleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("FactionLeave", "fid=%d,roleid=%d,scale=%d", factionid, roleid, scale);
		switch (scale)
		{
		case 0: // faction
			{
				unsigned int familyid;
				{
					Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
					PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
					if (NULL==pinfo || pinfo->factionid !=factionid || pinfo->factiontitle!=TITLE_HEADER) 
						return;
					familyid = pinfo->familyid;
				}
				int contribution;
				if(FactionManager::Instance()->CanLeave(familyid, factionid, contribution))
				{
					DBFactionLeaveArg arg(factionid, familyid, contribution);
					DBFactionLeave* rpc = (DBFactionLeave*) Rpc::Call( RPC_DBFACTIONLEAVE,arg);
					rpc->sendclient = true;
					rpc->master = 0;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
			break;
		case 1: //family
			{
				if(FamilyManager::Instance()->CanLeave(roleid, factionid))
				{
					DBFamilyLeaveArg arg(factionid, roleid); 
					DBFamilyLeave* rpc = (DBFamilyLeave*) Rpc::Call( RPC_DBFAMILYLEAVE, arg);
					rpc->master = 0;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
			break;
		}
	}
};

};

#endif
