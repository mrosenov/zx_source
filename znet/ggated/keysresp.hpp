
#ifndef __GNET_KEYSRESP_HPP
#define __GNET_KEYSRESP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "tokengenerator.h"
#include "customkey"
namespace GNET
{

class KeysResp : public GNET::Protocol
{
	#include "keysresp"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(TokenGenerator::GetInstance().OnGetNewKeys(keylist,timestamp))
			DEBUG_PRINT("KeysResp OnGetNewKeys error");
	}
};

};

#endif
