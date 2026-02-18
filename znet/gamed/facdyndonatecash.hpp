
#ifndef __GNET_FACDYNDONATECASH_HPP
#define __GNET_FACDYNDONATECASH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynDonateCash : public GNET::Protocol
{
	#include "facdyndonatecash"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
