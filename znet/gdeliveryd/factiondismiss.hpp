
#ifndef __GNET_FACTIONDISMISS_HPP
#define __GNET_FACTIONDISMISS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbfactiondel.hrp"
#include "dbundeletefaction.hrp"
#include "dbundeletefamily.hrp"
#include "factiondismiss_re.hpp"

namespace GNET
{

class FactionDismiss : public GNET::Protocol
{
#include "factiondismiss"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo;
		int localsid;
		int linksid;
		Log::formatlog("FactionDismiss", "fid=%d,roleid=%d,scale=%d,oper=%d",factionid,roleid,scale,oper);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (NULL==pinfo)
				return;
			localsid = pinfo->localsid;
			linksid  = pinfo->linksid;
		}
		switch(scale)
		{
		case 0: // faction
			if (pinfo->factionid != factionid || pinfo->factiontitle != TITLE_MASTER)
				return;
			if (oper == 1) // dismiss
			{
				int ret = FactionManager::Instance()->CanDismiss(factionid,roleid);
				if(!ret)
				{
					DBFactionDel* rpc = (DBFactionDel*) Rpc::Call( RPC_DBFACTIONDEL,FactionId(factionid));
					rpc->localsid = localsid;
					rpc->linksid = linksid;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
				else
					dsm->Send(linksid, FactionDismiss_Re(ret, factionid, localsid,scale,oper));
			}
			else // undismiss
			{
				DBUndeleteFaction * rpc = (DBUndeleteFaction*) 
					Rpc::Call( RPC_DBUNDELETEFACTION, FactionId(factionid));
				rpc->localsid = localsid;
				rpc->linksid = linksid;
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
			break;
		case 1: // family
			if (pinfo->familyid != factionid || pinfo->factiontitle != TITLE_HEADER || pinfo->factionid)
			{
				dsm->Send(linksid, FactionDismiss_Re(ERR_PERMISSION_DENIED, factionid, localsid,scale,oper));
				return;
			}
			if (oper == 1) // dismiss
			{
				int ret = FamilyManager::Instance()->CanDismiss(factionid);
				if(!ret)
				{
					DBFamilyDel* rpc = (DBFamilyDel*) Rpc::Call( RPC_DBFAMILYDEL, FamilyId(factionid));
					rpc->master = pinfo->name;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
				else
					dsm->Send(linksid, FactionDismiss_Re(ret, factionid, localsid,scale,oper));
			}
			else // undismiss
			{
				DBUndeleteFamily* rpc = (DBUndeleteFamily*) Rpc::Call( RPC_DBUNDELETEFAMILY, FamilyId(factionid));
				rpc->localsid = localsid;
				rpc->linksid = linksid;
				rpc->isclient = true;
				GameDBClient::GetInstance()->SendProtocol(rpc);

			}
			break;
		}
	}
};
};


#endif
