
#ifndef __GNET_FACDYNBUILDINGUPGRADE_HPP
#define __GNET_FACDYNBUILDINGUPGRADE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynBuildingUpgrade : public GNET::Protocol
{
	#include "facdynbuildingupgrade"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
