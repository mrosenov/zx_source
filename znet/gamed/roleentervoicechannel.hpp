
#ifndef __GNET_ROLEENTERVOICECHANNEL_HPP
#define __GNET_ROLEENTERVOICECHANNEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void OnRoleEnterVoiceChannel(long roleid);

namespace GNET
{

class RoleEnterVoiceChannel : public GNET::Protocol
{
	#include "roleentervoicechannel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		OnRoleEnterVoiceChannel(roleid);
	}
};

};

#endif
