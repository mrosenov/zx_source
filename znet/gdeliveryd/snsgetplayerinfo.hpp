
#ifndef __GNET_SNSGETPLAYERINFO_HPP
#define __GNET_SNSGETPLAYERINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "localmacro.h"
#include "snsplayerinfomanager.h"
#include "mapuser.h"

namespace GNET
{

class SNSGetPlayerInfo : public GNET::Protocol
{
	#include "snsgetplayerinfo"

	//check whether rid2 is a friend of rid1
	int CheckFriend(int rid1, int rid2)
	{
		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(rid1);
		if (pinfo == NULL)
			return SNS_ERR_ROLEOFFLINE;

		GFriendInfoVector::iterator it, ie = pinfo->friends.end();
		for (it = pinfo->friends.begin(); it != ie; ++it)
		{
			if (it->rid == rid2)
				return SNS_ERR_SUCCESS;
		}

		return SNS_ERR_PRIVILEGE;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		SNSGetPlayerInfo_Re resp;
		resp.roleid = roleid;
		resp.localsid = localsid;

		SNSPlayerInfoCache info;
		if (SNSPlayerInfoManager::GetInstance()->GetSNSPlayerInfo(asker, info))
		{
			if (check_private && info.playerinfo.is_private)
				resp.retcode = CheckFriend(asker, roleid);
			else
				resp.retcode = SNS_ERR_SUCCESS;

			if (resp.retcode == SNS_ERR_SUCCESS)
			{
				resp.info = info;
				resp.info.playerinfo.press_time = 0;
				resp.info.playerinfo.apply_time = 0;
				resp.info.playerinfo.leavemsg_time = 0;
				resp.info.playerinfo.vote_time = 0;
			}
		}
		else
			resp.retcode = SNS_ERR_ROLEOFFLINE;

		manager->Send(sid, resp);
	}
};

};

#endif
