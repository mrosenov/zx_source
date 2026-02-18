
#ifndef __GNET_REMOTELOGINQUERY_HPP
#define __GNET_REMOTELOGINQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
//#include "remoteloginquery_re.hpp"


namespace GNET
{

class RemoteLoginQuery : public GNET::Protocol
{
	#include "remoteloginquery"
	void Process(Manager *manager, Manager::Session::ID sid);
	//map.cpp ∂®“Â
};

};

#endif
