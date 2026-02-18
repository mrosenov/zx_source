
#ifndef __GNET_UPDATEROLECIRCLE_HPP
#define __GNET_UPDATEROLECIRCLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


void update_circle_info(int roleid, unsigned int circle_id, unsigned char circle_mask);

namespace GNET
{

class UpdateRoleCircle : public GNET::Protocol
{
	#include "updaterolecircle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		update_circle_info(roleid, circleid, titlemask);
	}
};

};

#endif
