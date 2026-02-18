
#ifndef __GNET_REMOTELOGOUT_HPP
#define __GNET_REMOTELOGOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "map.h"

namespace GNET
{

class RemoteLogout : public GNET::Protocol
{
	#include "remotelogout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		if (dsm->IsCentralDS())
			return;
		UserInfo * userinfo = UserContainer::GetInstance().FindUser(userid);
		if (userinfo == NULL || (userinfo->status != _STATUS_REMOTE_HALFLOGIN && userinfo->status != _STATUS_REMOTE_LOGIN))
		{
			Log::log(LOG_ERR, "Recv RemoteLogout, but user %d status %d invalid", userid, userinfo==NULL?0:userinfo->status);
			return;
		}
		else
		{
			LOG_TRACE("Recv RemoteLogout userid %d status %d", userid, userinfo->status);
			STAT_MIN5("LogoutRemote", 1);
		
			PlayerInfo* _playerInfo = userinfo->role;
			if(!_playerInfo)
			{
				Log::log(LOG_ERR, "Recv RemoteLogout, but playerinfo is null, userid:%d", userid);
				return;
			}

			int roleid = _playerInfo->roleid;	
			FriendStatus stat(roleid, -2, 0);
			PlayerInfo * playerinfo = UserContainer::GetInstance().FindRole(roleid);
			if(!playerinfo)
			{
				LOG_TRACE("RemoteLogout playerinfo is NULL");
			}
			if(playerinfo)
			{
				LOG_TRACE("RemoteLogout roleid:%d friend_ver:%d, size=:%d", roleid, playerinfo->friend_ver, playerinfo->friends.size());
			}
			if(playerinfo && playerinfo->friend_ver>=0)
			{
				Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
				for(GFriendInfoVector::iterator it = playerinfo->friends.begin();it!=playerinfo->friends.end();it++)
				{
					PlayerInfo * _pinfo = UserContainer::GetInstance().FindRole(it->rid);
					LOG_TRACE("RemoteLogout findrole id:%d", it->rid);
					if (NULL != _pinfo )
					{
						LOG_TRACE("RemoteLogout send FriendStatus userid %d status %d", userid, userinfo->status);
						stat.localsid = _pinfo->localsid;
						GDeliveryServer::GetInstance()->Send(_pinfo->linksid,stat);
					}
				}			
			}
			UserContainer::GetInstance().UserLogout(userinfo, 0, true);
		}
	}
};

};

#endif
