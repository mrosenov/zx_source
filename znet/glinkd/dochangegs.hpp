
#ifndef __GNET_DOCHANGEGS_HPP
#define __GNET_DOCHANGEGS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class DoChangeGS : public GNET::Protocol
{
	#include "dochangegs"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("dochangegs roleid %d dst_lineid %d localsid %d", roleid, dst_lineid, localsid);
		GLinkServer * lsm = GLinkServer::GetInstance();
		if (!lsm->ValidRole(sid, roleid))
			return;
		RoleData * uinfo = lsm->GetRoleInfo(roleid);
		if (uinfo == NULL || uinfo->status != _STATUS_SWITCH)
			return;
		uinfo->status = _STATUS_ONLINE;
		this->localsid = sid;
		if (GDeliveryClient::GetInstance()->SendProtocol(this))
			lsm->ChangeState(sid, &state_GSelectRoleReceive);
		else
			lsm->SessionError(sid, ERR_COMMUNICATION, "Server Network Error.");
	}
};

};

#endif
