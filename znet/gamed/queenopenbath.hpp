
#ifndef __GNET_QUEENOPENBATH_HPP
#define __GNET_QUEENOPENBATH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void queen_open_bath();

namespace GNET
{

class QueenOpenBath : public GNET::Protocol
{
	#include "queenopenbath"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		queen_open_bath();
	}
};

};

#endif
