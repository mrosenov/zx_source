
#ifndef __GNET_OPEN_CONTROLER_HPP
#define __GNET_OPEN_CONTROLER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

bool OnFlowerActiveCtrl(int ctrl_id);

namespace GNET
{

class Open_Controler : public GNET::Protocol
{
	#include "open_controler"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
			OnFlowerActiveCtrl(id);
	}
};

};

#endif
