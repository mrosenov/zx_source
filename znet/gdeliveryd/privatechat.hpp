
#ifndef __GNET_PRIVATECHAT_HPP
#define __GNET_PRIVATECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "rolestatusannounce.hpp"
#include "message"
#include "putmessage.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"
#include "gamegateserver.hpp"

namespace GNET
{

static char GM_Name[] = { 'G', 0, 'M', 0};

class PrivateChat : public GNET::Protocol
{
	#include "privatechat"
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		unsigned int srclsid=0,srcsid=0;

		Octets gmname(GM_Name, 4);
		srcrolelevel = 0;
		srcfactionid = 0;
		srcsectid = 0;
		//LOG_TRACE( "******PrivateChat: size=%d, sid=%d srcrole=%d,route=%d",data.size(),sid,srcroleid,route);
		if (channel < CHANNEL_GMREPLY)
		{
			if(route!= FROM_GT)
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(srcroleid);
				if(NULL == pinfo)
					return;
				GRoleInfo *prole = RoleInfoCache::Instance().Get(srcroleid);
				if (prole == NULL)
					return;
				UserInfo * puser = UserContainer::GetInstance().FindUser(pinfo->userid);
				if(NULL == puser)
					return;

				if (puser->rolelist.GetRoleCount() == 0)
					referrer = puser->real_referrer>0?puser->real_referrer:puser->au_suggest_referrer;
				else
					referrer = puser->real_referrer;

				srclsid = pinfo->localsid;
				srcsid  = pinfo->linksid;
				src_name = pinfo->name;
				emotion = pinfo->emotion;
				if (prole->reborn_cnt == 0 && !(pinfo->IsGM()))
					srcrolelevel = pinfo->level;//为CHANNEL_NORMAL频道填充发送者等级
				srcfactionid = pinfo->factionid;
				srcsectid = pinfo->sectid;
	
				if(pinfo->IsGM() && (channel==CHANNEL_NORMAL || channel==CHANNEL_NORMALRE))
				{
					src_name.replace(GM_Name, 4);
					srcroleid = -1;
				}
			}
			else
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(srcroleid);
				if(pinfo && pinfo->IsGM() && channel==CHANNEL_NORMAL )
				{
					src_name.replace(GM_Name, 4);
					srcroleid = -1;
				}
			}
		}
		else
		{
			if(channel!=CHANNEL_GMREPLY)
				return;
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(srcroleid);
			if((NULL == pinfo || !pinfo->IsGM()) && sid!=dsm->iweb_sid)
				return;
			src_name.replace(GM_Name, 4);
			srcroleid = -1;
		}


		if(dst_name==gmname)
		{
			MasterContainer::Instance().Broadcast(*this, this->dstroleid);
			return;
		}

		if(dstroleid<=0)
		{
			if(dst_name.size()<MAX_NAME_SIZE)
				UserContainer::GetInstance().FindRoleId( dst_name, dstroleid );
			else 
				return;
		}

		if(dstroleid>0)
		{
		/*	if(channel==CHANNEL_NORMAL || channel==CHANNEL_FRIEND)
			{*/
				unsigned char src_route = route;
				if(src_route != FROM_GT)
				{
					route = FROM_GAME;
				}
	
//				if(src_route == TO_GT_AND_GAME || src_route== TO_GT)
				if (src_route != FROM_GT && src_route != TO_GAME && channel != CHANNEL_NORMALRE)
				{
					GameGateServer::GetInstance()->GTSend(this);
//					if(src_route== TO_GT)
//						return;
				}
	
//				if(src_route == FROM_GT || src_route == TO_GT_AND_GAME || src_route== TO_GAME)
				{
					PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(dstroleid);
					if (pinfo)
					{
						dsm->Send(pinfo->linksid,this);
						return;
					}
				}
		/*	}
			else
			{
				route = FROM_GAME;
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(dstroleid);
				if (pinfo)
				{
					dsm->Send(pinfo->linksid,this);
					return;
				}

			}*/
		}
		if(channel==CHANNEL_FRIEND)
		{
			Message arg;
			arg.channel = channel;
			arg.srcroleid = srcroleid;
			arg.dstroleid = dstroleid;
			arg.src_name.swap(src_name);
			arg.dst_name.swap(dst_name);
			arg.msg.swap(msg);
			PutMessage* rpc = (PutMessage*) Rpc::Call(RPC_PUTMESSAGE, arg);
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else if (channel!=CHANNEL_GMREPLY)
			dsm->Send(srcsid,RoleStatusAnnounce(_ZONE_INVALID,_ROLE_INVALID,srclsid,_STATUS_OFFLINE,dst_name));
	}
};

};

#endif
