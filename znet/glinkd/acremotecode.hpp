
#ifndef __GNET_ACREMOTECODE_HPP
#define __GNET_ACREMOTECODE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
namespace GNET
{

class ACRemoteCode : public GNET::Protocol
{
	#include "acremotecode"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		bool ret=true;
		unsigned int localsid=0;
		{
			Thread::RWLock::RDScoped l(lsm->locker_map);
			RoleData * uinfo = lsm->GetRoleInfo(dstroleid);
			if(uinfo && uinfo->status  == _STATUS_ONGAME)
			{
				if ( !lsm->Send(uinfo->sid, this, true) )
				{
					ret = false;
					localsid = uinfo->sid;
				}
			}
		}
		if ( !ret )
		{
			size_t osbytes = lsm->GetOsbytes(localsid);
			lsm->Close( localsid );
			Log::log(LOG_DEBUG,"glinkd::acremotecode :: send to roleid=%d failed, sid=%d, osbytes=%d", dstroleid, localsid, osbytes);
		}
	}
};

};

#endif
