
#ifndef __GNET_GETREMOTEROLEINFO_RE_HPP
#define __GNET_GETREMOTEROLEINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "groleinfo"

namespace GNET
{

class GetRemoteRoleInfo_Re : public GNET::Protocol
{
	#include "getremoteroleinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv GetRemoteRoleInfo_Re retcode %d userid %d roleid %d create_time %d version %d", retcode, userid, roleid, value.create_time, value.data_timestamp);
		if (retcode != ERR_SUCCESS)
			Log::log(LOG_ERR, "GetRemoteRoleInfo_Re errno %d userid %d roleid %d", retcode, userid, roleid);
		else
		{
			GRoleInfo * role_old = RoleInfoCache::Instance().Get(roleid);
			if (role_old && (role_old->status&CACHESTATUS_COMPLETE)!=0 && value.data_timestamp > role_old->data_timestamp)
			{
				value.delete_flag = _ROLE_STATUS_FROZEN;
				value.data_timestamp = role_old->data_timestamp;
				value.src_zoneid = role_old->src_zoneid;
				RoleInfoCache::Instance().Insert(roleid, value);
				LOG_TRACE("UpdateRemoteRoleInfo roleid %d old_version %d", roleid, role_old->data_timestamp);
			}
		}
		DelayRolelistTask::OnRecvInfo(userid, roleid);
	}
};

};

#endif
