
#ifndef __GNET_FACDYNWINAUCTION_HPP
#define __GNET_FACDYNWINAUCTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynWinAuction : public GNET::Protocol
{
	#include "facdynwinauction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
