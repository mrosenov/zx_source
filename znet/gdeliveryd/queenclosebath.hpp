
#ifndef __GNET_QUEENCLOSEBATH_HPP
#define __GNET_QUEENCLOSEBATH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class QueenCloseBath : public GNET::Protocol
{
	#include "queenclosebath"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
