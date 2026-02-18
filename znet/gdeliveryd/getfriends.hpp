
#ifndef __GNET_GETFRIENDS_HPP
#define __GNET_GETFRIENDS_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "getfriends_re.hpp"
#include "mapuser.h"
#include "gtplatformagent.h"
namespace GNET
{

class GetFriends : public GNET::Protocol
{
	#include "getfriends"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GetFriends_Re re;
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		{
			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());	
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRole((roleid));
			if( NULL == pinfo )
				return;
			if(pinfo->friend_ver<0)
			{
				GetFriendList* rpc = (GetFriendList*) Rpc::Call(RPC_GETFRIENDLIST,RoleId(roleid));
				rpc->roleid = roleid;
				rpc->bUpdateFriend = true;
				GameDBClient::GetInstance()->SendProtocol(rpc);
				return;
			}
			else
			{
				re.groups = pinfo->groups;
				GFriendInfoVector * plist = &(pinfo->friends);
				GTPlatformAgent::GetFriendStatus(roleid,*plist);
				FriendStatus stat(roleid, pinfo->gameid, 0);
				GClientFriendInfo friendinfo;	
				for (GFriendInfoVector::iterator i = plist->begin(); i!=plist->end(); ++i)
				{
					PlayerInfo * ruser = UserContainer::GetInstance().FindRoleOnline(i->rid);
					if (NULL != ruser)
					{
						for (GFriendInfoVector::iterator k=ruser->friends.begin(),ke=ruser->friends.end();k!=ke;++k)
						{
							if (k->rid==roleid)
							{
								//LOG_TRACE("GetFriends send FriendStatus %d", ruser->gameid);
								stat.localsid = ruser->localsid;
								i->lineid = ruser->gameid;
								dsm->Send(ruser->linksid, stat);
								friendinfo.lineid = ruser->gameid;
								break;
							}
						}
					}
					else
					{
						i->lineid = -1;
						friendinfo.lineid = -1;
					}
					friendinfo.rid = i->rid;
					friendinfo.gid = i->gid;
					friendinfo.name = i->name;
					re.friends.push_back(friendinfo);
				}
		//		re.friends = pinfo->friends;
			}
		}
		re.roleid = roleid;
		re.reserved = 0;
		re.localsid = localsid;
		dsm->Send(sid, re);
	}
};

};

#endif
