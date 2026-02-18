
#ifndef __GNET_QUEENOPENBATH_HPP
#define __GNET_QUEENOPENBATH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueenOpenBath : public GNET::Protocol
{
	#include "queenopenbath"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
