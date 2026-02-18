
#ifndef __GNET_DBCONSIGNSKIPSN_HPP
#define __GNET_DBCONSIGNSKIPSN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class DBConsignSkipSN : public GNET::Protocol
{
	#include "dbconsignskipsn"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
