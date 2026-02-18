
#ifndef __GNET_FACDYNBUYAUCTION_HPP
#define __GNET_FACDYNBUYAUCTION_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynBuyAuction : public GNET::Protocol
{
	#include "facdynbuyauction"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
