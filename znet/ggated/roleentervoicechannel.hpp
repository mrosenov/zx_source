
#ifndef __GNET_ROLEENTERVOICECHANNEL_HPP
#define __GNET_ROLEENTERVOICECHANNEL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class RoleEnterVoiceChannel : public GNET::Protocol
{
	#include "roleentervoicechannel"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("roleentervoicechannel userid %lld roleid %lld zoneid %d seq %lld timestamp %lld",
				userid, roleid, zoneid, seq, timestamp);
		manager->Send(sid, RoleEnterVoiceChannelAck(userid, roleid, zoneid, seq, timestamp));
		int roleid32 = 0;
		if(!GTManager::Instance()->Check_64to32_Cast(roleid, roleid32))
			return;
		RoleInfo * role = RoleContainer::GetInstance().FindRole(roleid32);
		int gameid = -1;
		if (role && role->GameOnline() && (gameid=role->GetGameId())!=-1)
		{
			LOG_TRACE("roleentervoicechannel roleid %lld send to gs %d", roleid, gameid);
			GProviderServer::GetInstance()->DispatchProtocol(gameid, this);
		}
		else
			LOG_TRACE("roleentervoicechannel roleid %lld roleonline %d gameid %d",
					roleid, role==NULL?-1:role->GameOnline(), gameid);
	}
};

};

#endif
