
#ifndef __GNET_OPENBANQUETENTER_RE_HPP
#define __GNET_OPENBANQUETENTER_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class OpenBanquetEnter_Re : public GNET::Protocol
{
	#include "openbanquetenter_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("OpenBanquetEnter_Re::retcode=%d,roleid=%d,endtime=%d", retcode, roleid, endtime);
		RoleData *data = GLinkServer::GetInstance()->GetRoleInfo(roleid);
		if(data != NULL)
		{
			GLinkServer::GetInstance()->Send(data->sid, this);
		}
	}
};

};

#endif
