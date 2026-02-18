
#ifndef __GNET_FACDYNBUILDINGCOMPLETE_HPP
#define __GNET_FACDYNBUILDINGCOMPLETE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FacDynBuildingComplete : public GNET::Protocol
{
	#include "facdynbuildingcomplete"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
