
#ifndef __GNET_GETKDCTOKENTOGATE_HPP
#define __GNET_GETKDCTOKENTOGATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetKDCTokenToGate : public GNET::Protocol
{
	#include "getkdctokentogate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
