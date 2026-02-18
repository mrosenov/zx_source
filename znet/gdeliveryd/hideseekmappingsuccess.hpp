
#ifndef __GNET_HIDESEEKMAPPINGSUCCESS_HPP
#define __GNET_HIDESEEKMAPPINGSUCCESS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HideSeekMappingSuccess : public GNET::Protocol
{
	#include "hideseekmappingsuccess"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
