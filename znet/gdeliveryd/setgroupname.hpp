
#ifndef __GNET_SETGROUPNAME_HPP
#define __GNET_SETGROUPNAME_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "setgroupname_re.hpp"
#include "mapuser.h"
#include "gtplatformagent.h"
namespace GNET
{

class SetGroupName : public GNET::Protocol
{
	#include "setgroupname"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		char ret = ERR_FS_NOFOUND;
		if(name.size()>20||gid==0)
			ret = ERR_FS_ERRPARAM;
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		if(ret==ERR_FS_NOFOUND)
		{
			Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRole((roleid));
			if( NULL==pinfo || pinfo->friend_ver<0)
				return;
			GGroupInfoVector* plist = &(pinfo->groups);
			for(GGroupInfoVector::iterator itg = plist->begin(), itge=plist->end();itg!=itge;++itg)
			{
				if(itg->gid!=gid)
					continue;
				ret=0;
				if(name.size()==0)
				{
					plist->erase(itg);
					GFriendInfoVector* flist = &(pinfo->friends);
					for(GFriendInfoVector::iterator itf=flist->begin(),itfe=flist->end();itf!=itfe;++itf)
					{
						if(itf->gid==gid)
						{
							itf->gid = 0;
						//	GTPlatformAgent::ModifyFriend(FRIEND_MOVE,roleid,itf->rid,0);
						//	handled by gt platform
						}
					}
					GTPlatformAgent::ModifyFriendGroup(OP_DEL,roleid,gid,name);
				}
				else
				{
					itg->name = name;
					GTPlatformAgent::ModifyFriendGroup(OP_MOD,roleid,gid,name);
				}
				break;
			}
			if(ret==ERR_FS_NOFOUND)
			{
				if(plist->size()>=10)
					ret = ERR_FS_NOSPACE;
				else
				{
					plist->push_back(GGroupInfo(gid,name));
					ret = 0;
					GTPlatformAgent::ModifyFriendGroup(OP_ADD,roleid,gid,name);
				}
			}
			if(ret==0)
				pinfo->friend_ver++;
		}
		dsm->Send(sid, SetGroupName_Re(ret, roleid, gid, name, localsid));
	}
};

};

#endif
