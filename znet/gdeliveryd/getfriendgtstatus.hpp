
#ifndef __GNET_GETFRIENDGTSTATUS_HPP
#define __GNET_GETFRIENDGTSTATUS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gtplatformagent.h"

namespace GNET
{

class GetFriendGTStatus : public GNET::Protocol
{
	#include "getfriendgtstatus"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
/*		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole((roleid));
		if( NULL == pinfo || pinfo->friend_ver<0)//no friendlist
			return;
		GFriendInfoVector * plist = &(pinfo->friends);
		GTPlatformAgent::GetFriendStatus(roleid,*plist);*/
		GTPlatformAgent::GetFriendStatus(roleid,friendid);
	}
};

};

#endif
