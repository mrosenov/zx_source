
#ifndef __GNET_GETKDCTOKEN_HPP
#define __GNET_GETKDCTOKEN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gamegateserver.hpp"
#include "mapuser.h"
#include "getkdctokentogate.hpp"

namespace GNET
{

class GetKDCToken : public GNET::Protocol
{
	#include "getkdctoken"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if(!pinfo)
			return;
		userid = pinfo->userid;
		Octets passport = pinfo->user->account;
		linksid = sid;
		GetKDCTokenToGate msg(roleid,pinfo->name,localsid,linksid,userid,passport);
		GameGateServer::GetInstance()->GateSend(&msg);
	}
};

};

#endif
