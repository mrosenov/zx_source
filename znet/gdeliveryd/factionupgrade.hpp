
#ifndef __GNET_FACTIONUPGRADE_HPP
#define __GNET_FACTIONUPGRADE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbfactionupdate.hrp"

namespace GNET
{

class FactionUpgrade : public GNET::Protocol
{
	#include "factionupgrade"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("FactionUpgrade", "fid=%d,roleid=%d,scale=%d", factionid,roleid,scale);
		DBFactionUpdateArg arg;
		arg.reason = 1;
		if(!FactionManager::Instance()->FillUpdateArg(factionid,roleid,arg))
			return;
		DBFactionUpdate* rpc = (DBFactionUpdate*) Rpc::Call( RPC_DBFACTIONUPDATE,arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
