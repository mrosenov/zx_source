
#ifndef __GNET_FACBASEPROPCHANGE_HPP
#define __GNET_FACBASEPROPCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void facbase_prop_change(int noti_roleid, int base_tag, int fid, int prop_type, int delta);

namespace GNET
{

class FacBasePropChange : public GNET::Protocol
{
	#include "facbasepropchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		facbase_prop_change(roleid, base_tag, fid, prop_type, delta);
	}
};

};

#endif
