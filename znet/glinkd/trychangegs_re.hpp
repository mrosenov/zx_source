
#ifndef __GNET_TRYCHANGEGS_RE_HPP
#define __GNET_TRYCHANGEGS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class TryChangeGS_Re : public GNET::Protocol
{
	#include "trychangegs_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv trychangegs_re ret %d roleid %d dst_lineid %d localsid %d", retcode, roleid, dst_lineid, localsid);
		GLinkServer* lsm = GLinkServer::GetInstance();
		if (!lsm->ValidRole(localsid, roleid))
			return;
		if (retcode == ERR_SUCCESS)
		{
			SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
			if (sinfo)
				sinfo->ingame = false;
			RoleData * uinfo = lsm->GetRoleInfo(roleid);
			if (uinfo)
				uinfo->status = _STATUS_SWITCH;
		}
		lsm->Send(localsid, this);
	}
};

};

#endif
