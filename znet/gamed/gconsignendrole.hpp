
#ifndef __GNET_GCONSIGNENDROLE_HPP
#define __GNET_GCONSIGNENDROLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void consign_end_role(int roleid, int retcode);

namespace GNET
{

class GConsignEndRole : public GNET::Protocol
{
	#include "gconsignendrole"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		consign_end_role(roleid, retcode);
	}
};

};

#endif
