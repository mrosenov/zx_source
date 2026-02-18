
#ifndef __GNET_SYNCGSROLEINFO2PLATFORM_HPP
#define __GNET_SYNCGSROLEINFO2PLATFORM_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gconsigngsroleinfo"

namespace GNET
{

class SyncGsRoleInfo2Platform : public GNET::Protocol
{
	#include "syncgsroleinfo2platform"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("syncgsroleinfo2platform roleid %d", roleid);
		DBSNSGetRole * rpc = (DBSNSGetRole *)Rpc::Call(RPC_DBSNSGETROLE, DBSNSGetRoleArg(roleid));
		rpc->gsroleinfo = this->gsroleinfo;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
