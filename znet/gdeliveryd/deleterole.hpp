
#ifndef __GNET_DELETEROLE_HPP
#define __GNET_DELETEROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "delrole.hrp"
#include "deleterole_re.hpp"
#include "dbdeleterole.hrp"
#include "mapuser.h"
namespace GNET
{

class DeleteRole : public GNET::Protocol
{
	#include "deleterole"
	/* this will not delete the role in DB, only mark the role should be delete*/
	void Mark_Delete(GDeliveryServer* dsm, Manager::Session::ID sid)  
	{
		int userid = UserContainer::Roleid2Userid(roleid);
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		UserInfo* user = UserContainer::GetInstance().FindUser(userid);
		if (!user || user->linksid!=sid || user->localsid!=localsid)
			return;
		GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(roleid);
		if (roleinfo == NULL || roleinfo->delete_flag != _ROLE_STATUS_NORMAL)
		{
			dsm->Send(sid,DeleteRole_Re(ERR_PERMISSION_DENIED, roleid, localsid));
			return;
		}
		DelRole* rpc=(DelRole*) Rpc::Call(RPC_DELROLE,RoleId(roleid));
		rpc->save_link_sid=sid;
		rpc->save_localsid=localsid;
		if (!GameDBClient::GetInstance()->SendProtocol(rpc))
		{
			dsm->Send(sid,DeleteRole_Re(ERR_DELETEROLE,roleid,localsid));
		}
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
		{
			Log::log(LOG_ERR, "roleid %d try to delete role on Central Delivery Server, refuse him!", roleid);
			return;
		}
		Mark_Delete(dsm,sid);
	}
};

};

#endif
