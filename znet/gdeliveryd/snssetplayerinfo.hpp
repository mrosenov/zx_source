
#ifndef __GNET_SNSSETPLAYERINFO_HPP
#define __GNET_SNSSETPLAYERINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "snssetplayerinfo_re.hpp"

#include "snsplayerinfo"
#include "snsplayerinfomanager.h"
#include "mapuser.h"

namespace GNET
{

class SNSSetPlayerInfo : public GNET::Protocol
{
	#include "snssetplayerinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo!=NULL && pinfo->roleid==roleid && pinfo->linksid==sid)
		{
			SNSPlayerInfo info(roleid, is_private, pinfo->name, nickname, age, birthday, province, city, career, address, introduce);
			SNSPlayerInfoManager::GetInstance()->UpdateSNSPlayerInfo(info);

			manager->Send(sid, SNSSetPlayerInfo_Re(ERR_SUCCESS, roleid, localsid));
		}
	}
};

};

#endif
