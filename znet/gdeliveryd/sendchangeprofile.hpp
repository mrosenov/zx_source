
#ifndef __GNET_SENDCHANGEPROFILE_HPP
#define __GNET_SENDCHANGEPROFILE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gamedbclient.hpp"
#include "dbchangeprofile.hrp"


namespace GNET
{

class SendChangeProfile : public GNET::Protocol
{
	#include "sendchangeprofile"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DBChangeProfile *rpc = (DBChangeProfile *)Rpc::Call(RPC_DBCHANGEPROFILE, DBChangeProfileArg(roleid, faceid, hairid, earid, tailid, fashionid));
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
};

};

#endif
