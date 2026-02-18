
#ifndef __GNET_GAMEDATAREQ_HPP
#define __GNET_GAMEDATAREQ_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GameDataReq : public GNET::Protocol
{
	#include "gamedatareq"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
