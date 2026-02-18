#ifndef __LOGOUTROLE_TASK_H
#define __LOGOUTROLE_TASK_H
#include "thread.h"
#include "ggroupinfo"
#include "gfriendinfo"
#include "friendstatus.hpp"
#include "friendlistpair"
#include "putfriendlist.hrp"
#include "chatroom.h"
#include "mapuser.h"
#include "factionmanager.h"
#include "stockexchange.h"
#include "battlemanager.h"
namespace GNET
{
	class LogoutRoleTask : public Thread::Runnable
	{
		int roleid;
		int friend_ver;
		GGroupInfoVector  groups;
		GFriendInfoVector friends; 
		GEnemyInfoVector  enemies; 
	public:
		LogoutRoleTask(PlayerInfo& user)
		{
			roleid = user.roleid;
			friend_ver = user.friend_ver;
			if(user.friend_ver>=0)
			{
				user.friend_ver = -1; //data cleared, so set friend_ver to uninitialized
				groups.swap(user.groups);
				friends.swap(user.friends);
				enemies.swap(user.enemies);
			}
		}
		~LogoutRoleTask(){}

		void Run()
		{
			GDeliveryServer* dsm = GDeliveryServer::GetInstance();
			RoomManager::GetInstance()->ResetRole(roleid);

			FriendStatus stat(roleid, -1, 0);
			GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
			if(info&&info->spouse)
			{
				PlayerInfo * su = UserContainer::GetInstance().FindRoleOnline(info->spouse);
				if(su)
				{
					stat.localsid = su->localsid;
					dsm->Send(su->linksid,stat);
				}
			}
			
			{
				Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
				for(GFriendInfoVector::iterator it = friends.begin();it!=friends.end();it++)
				{
					PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(it->rid);
					if (NULL != pinfo )
					{
						stat.localsid = pinfo->localsid;
						dsm->Send(pinfo->linksid,stat);
					}
				}
			}
			time_t now = Timer::GetTime();
			for(GEnemyInfoVector::iterator it=enemies.begin();it!=enemies.end();)
			{
				if(it->status==0 && now-it->pktime>864000)
				{
					friend_ver = 1;
					it = enemies.erase(it);
				}
				else
					++it;
			}
			if(friend_ver>0)
			{
				FriendListPair pair;
				pair.key = roleid;
				pair.value.groups.swap(groups); 
				pair.value.friends.swap(friends); 
				pair.value.enemies.swap(enemies); 
				PutFriendList* rpc = (PutFriendList*) Rpc::Call(RPC_PUTFRIENDLIST,pair);
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
			delete this;
		}

		static void Add(PlayerInfo& u)
		{
			if(u.roleid != 0)
			{
				Thread::Pool::AddTask(new LogoutRoleTask(u));
			}
		}
	};
}
#endif
