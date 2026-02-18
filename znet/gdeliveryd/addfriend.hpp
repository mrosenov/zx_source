
#ifndef __GNET_ADDFRIEND_HPP
#define __GNET_ADDFRIEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"
namespace GNET
{

class AddFriend : public GNET::Protocol
{
	#include "addfriend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		unsigned int dstlsid = 0, dstsid = 0;
		AddFriendRqstArg arg;
		int ret = 0;

		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		{
			if(dstroleid<=0)
				UserContainer::GetInstance().FindRoleId( dstname, dstroleid );
			if(dstroleid==srcroleid)
				return;

			Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
			PlayerInfo * pinfosrc = UserContainer::GetInstance().FindRoleOnline( srcroleid );
			if( NULL == pinfosrc )
				return;
			GRoleInfo *prole = RoleInfoCache::Instance().Get(srcroleid);
			if (NULL == prole)
				return;
			UserInfo * puser = UserContainer::GetInstance().FindUser(pinfosrc->userid);
			if(NULL == puser)
				return;

			if (puser->rolelist.GetRoleCount() == 0)
			        arg.referrer = puser->real_referrer>0?puser->real_referrer:puser->au_suggest_referrer;
		        else
		                arg.referrer = puser->real_referrer;

			arg.srcname = pinfosrc->name;
			if (prole->reborn_cnt == 0 && !(pinfosrc->IsGM()))
				arg.srcrolelevel = pinfosrc->level;
			arg.srcfactionid = pinfosrc->factionid;
			arg.srcsectid = pinfosrc->sectid;
			GFriendInfoVector& list = pinfosrc->friends;

			if(pinfosrc->friend_ver<0)
				ret = ERR_FS_NOTINITIALIZED;
			else if(list.size()>=100)
				ret = ERR_FS_NOSPACE;
			else if(dstroleid>0)
			{
				PlayerInfo * pinfodst = UserContainer::GetInstance().FindRoleOnline( dstroleid );
				if (NULL != pinfodst )
				{
					dstlsid = pinfodst->localsid;
					dstsid = pinfodst->linksid;
					dstname = pinfodst->name;
					for(GFriendInfoVector::iterator i=list.begin(), ie=list.end(); i!=ie; ++i)
					{
						if(i->rid==dstroleid && dstname==i->name)
						{
							ret = ERR_FS_DUPLICATE;
							break;
						}
					}
				}
				else
					ret = ERR_FS_OFFLINE;
			}
			else
				ret = ERR_FS_OFFLINE;
		}

		if(ret!=0)
		{
			AddFriend_Re re;
			re.retcode = ret;
			re.info.rid = dstroleid;
			re.info.name.swap(dstname);
			re.srclsid = srclsid;
			dsm->Send(sid,re);
			return;
		}
		arg.srcroleid = srcroleid;
		arg.dstlsid = dstlsid;
		AddFriendRqst* rqst = (AddFriendRqst*)Rpc::Call(RPC_ADDFRIENDRQST, &arg);
		rqst->srclsid = srclsid;
		rqst->srcsid = sid;
		rqst->dstlsid = dstlsid;
		rqst->dstsid = dstsid;
		rqst->dstname = dstname;
		rqst->dstroleid = dstroleid;
		dsm->Send(dstsid, rqst);
	}
};

};

#endif
