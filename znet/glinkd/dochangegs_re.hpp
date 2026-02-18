
#ifndef __GNET_DOCHANGEGS_RE_HPP
#define __GNET_DOCHANGEGS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class DoChangeGS_Re : public GNET::Protocol
{
	#include "dochangegs_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("recv dochangegs_re ret %d roleid %d dst_lineid %d mapid %d x %f y %f z %f", retcode, roleid, dst_lineid, mapid, scalex, scaley, scalez);
		GLinkServer * lsm = GLinkServer::GetInstance();
		if (!lsm->ValidRole(localsid, roleid))
			return;
		if (retcode == ERR_SUCCESS)
		{
			SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
			if (sinfo)
				sinfo->gsid = dst_lineid;
			RoleData * uinfo = lsm->GetRoleInfo(roleid);
			if (uinfo) 
				uinfo->gs_id = dst_lineid;
			lsm->ChangeState(localsid,&state_GReadyGame);
			lsm->Send(localsid, this);
		}
	}
};

};

#endif
