
#ifndef __GNET_GFACBASELEAVE_HPP
#define __GNET_GFACBASELEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GFacBaseLeave : public GNET::Protocol
{
	#include "gfacbaseleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo == NULL)
			return;
		if (pinfo->old_gs != _GAMESERVER_ID_INVALID)
			GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, PlayerChangeGS(roleid, pinfo->localsid, pinfo->old_gs, map_tag, 0, scalex, scaley, scalez));
		LOG_TRACE("gfacbaseleave:roleid=%d,map=%d,x=%f,y=%f,z=%f", roleid, map_tag, scalex, scaley, scalez);
	}
};

};

#endif
