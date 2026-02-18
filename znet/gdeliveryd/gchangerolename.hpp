
#ifndef __GNET_GCHANGEROLENAME_HPP
#define __GNET_GCHANGEROLENAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmailsyncdata"

namespace GNET
{

class GChangeRolename : public GNET::Protocol
{
	#include "gchangerolename"
	/*
	void SendErr(Manager * manager, Manager::Session::ID sid, const PlayerInfo * pinfo, int ret, const GMailEndSync & sync, const Octets & newname)
	{
		manager->Send(sid, sync);
		GDeliveryServer::GetInstance()->Send(pinfo->linksid, ChangeRolename_Re(ERR_DUPLICATRECORD, pinfo->roleid, newname, pinfo->localsid));
		*/
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("gchangerolename roleid %d newname.size %d itemid %d item_pos %d", roleid, newname.size(), itemid, item_pos);
		GMailEndSync sync(0, ERR_FC_CHECKCONDITION, roleid);
		GDeliveryServer * dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to change rolename on Central Delivery Server, refuse him!", roleid);
			manager->Send(sid, sync);
			return;
		}
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (NULL == pinfo )
			return;
		if (!NameManager::GetInstance()->IsInit())
		{
			Log::log(LOG_ERR, "role %d try to change rolename, but NameManager is not init", roleid);
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeRolename_Re(ERR_SP_NOT_INIT, roleid, newname, pinfo->localsid));
			return;
		}
		if(!UniqueNameClient::GetInstance()->IsConnect() || !GameDBClient::GetInstance()->IsConnect())
		{
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeRolename_Re(ERR_COMMUNICATION, roleid, newname, pinfo->localsid));
			return;
		}
		int tmpid = 0;
		if (UserContainer::GetInstance().FindRoleId(newname, tmpid))
		{
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeRolename_Re(ERR_DUPLICATRECORD, roleid, newname, pinfo->localsid));
			return;
		}
		if (!CreateRole::ValidRolename(newname))
		{
			manager->Send(sid, sync);
			dsm->Send(pinfo->linksid, ChangeRolename_Re(ERR_INVALIDCHAR, roleid, newname, pinfo->localsid));
			return;
		}
		PreChangeRolename * rpc=(PreChangeRolename *)Rpc::Call( RPC_PRECHANGEROLENAME, PreChangeRolenameArg(dsm->zoneid, pinfo->userid, roleid, newname));
		rpc->itemid = itemid;
		rpc->item_pos = item_pos;
		Utility::SwapSyncData(rpc->syncdata, syncdata);
		UniqueNameClient::GetInstance()->SendProtocol(rpc);
		LOG_TRACE("gchangerolename roleid %d send to uniquenamed", roleid);
	}
};

};

#endif
