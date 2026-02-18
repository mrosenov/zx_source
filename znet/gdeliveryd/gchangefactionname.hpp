
#ifndef __GNET_GCHANGEFACTIONNAME_HPP
#define __GNET_GCHANGEFACTIONNAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"
#include "prechangefactionname.hrp"

namespace GNET
{

class GChangeFactionName : public GNET::Protocol
{
	#include "gchangefactionname"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gchangefactionname roleid %d fid %d newname.size %d scale %d itemid %d item_pos %d", roleid, fid, newname.size(), scale, itemid, item_pos);
		GMailEndSync sync(0, ERR_FC_CHECKCONDITION, roleid);
		GDeliveryServer * dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to change factionname on Central Delivery Server, refuse him!", roleid);
			manager->Send(sid, sync);
			return;
		}
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo )
			return;
		/*
		if (!NameManager::GetInstance()->IsInit())
		{
			Log::log(LOG_ERR, "role %d try to change rolename, but NameManager is not init", roleid);
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeRolename_Re(ERR_SP_NOT_INIT, roleid, newname, pinfo->localsid));
			return;
		}
		*/
		if(!UniqueNameClient::GetInstance()->IsConnect() || !GameDBClient::GetInstance()->IsConnect())
		{
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeFactionName_Re(ERR_COMMUNICATION, roleid, fid, newname, scale, pinfo->localsid));
			return;
		}
		/*
		int tmpid = 0;
		if (UserContainer::GetInstance().FindRoleId(newname, tmpid))
		{
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeRolename_Re(ERR_DUPLICATRECORD, roleid, newname, pinfo->localsid));
			return;
		}
		*/
		//if (!CreateRole::ValidRolename(newname))
		int ret = -1;
		if (scale == 0)
			ret = FactionManager::Instance()->CanChangeName(fid, roleid, newname);
		else if (scale == 1)
			ret = FamilyManager::Instance()->CanChangeName(fid, roleid, newname);
		if (ret != ERR_SUCCESS)
		{
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeFactionName_Re(ret, roleid, fid, newname, scale, pinfo->localsid));
			return;
		}
		PreChangeFactionname * rpc=(PreChangeFactionname *)Rpc::Call( RPC_PRECHANGEFACTIONNAME, PreChangeFactionnameArg(dsm->zoneid, fid, newname, scale));
		rpc->itemid = itemid;
		rpc->item_pos = item_pos;
		rpc->roleid = roleid;
		Utility::SwapSyncData(rpc->syncdata, syncdata);
		UniqueNameClient::GetInstance()->SendProtocol(rpc);
		LOG_TRACE("gchangefactionname roleid %d send to uniquenamed", roleid);
	}
};

};

#endif
