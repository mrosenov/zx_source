
#ifndef __GNET_ROLELEAVEVOICECHANNEL_HPP
#define __GNET_ROLELEAVEVOICECHANNEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void OnRoleLeaveVoiceChannel(long roleid);

namespace GNET
{

class RoleLeaveVoiceChannel : public GNET::Protocol
{
	#include "roleleavevoicechannel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		OnRoleLeaveVoiceChannel(roleid);
	}
};

};

#endif
