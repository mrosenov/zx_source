#ifndef _GNET_DISABLEAUTOLOCK_HPP
#define _GNET_DISABLEAUTOLOCK_HPP

#include "callid.hxx"

namespace GNET
{
class DisableAutolock : public GNET::Protocol
{
	#include "disableautolock"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		UserInfo *userinfo = UserContainer::GetInstance().FindUser(userid);
		if(!userinfo)
		  return;
		Log::formatlog("gamemaster", "disableautolock:userid=%d:sid=%d", userid, sid);
		userinfo->autolock = PairSet();
		userinfo->autolock.SetValue(LOCKTIME_NOW, 1);
	}
};

};
#endif
