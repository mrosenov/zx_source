
#ifndef __GNET_PUBLICCHAT_HPP
#define __GNET_PUBLICCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gproviderserver.hpp"
#include "gdeliveryclient.hpp"
#include "../common/conv_charset.h"
#include "privatechat.hpp"
#include "privilege.hxx"
#include "base64.h"
#include "glog.h"
namespace GNET
{

class PublicChat : public GNET::Protocol
{
	#include "publicchat"
	void SendForbidInfo(GLinkServer* lsm,Manager::Session::ID sid,const GRoleForbid& forbid)
	{
		lsm->Send(sid,AnnounceForbidInfo(roleid,_SID_INVALID,forbid));
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(msg.size()>256 || data.size()>100)
			return;
		GLinkServer* lsm=GLinkServer::GetInstance();
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (!sinfo || sinfo->roleid != roleid || roleid<=0 || !sinfo->policy.Update(CHAT_POLICY))
			return; 
		GRoleForbid forbid;
		if (lsm->IsForbidChat(sinfo->userid, roleid, forbid))
		{
			SendForbidInfo(lsm, sid, forbid);
			return;
		}
		{
			Octets out;
			Base64Encoder::Convert(out, msg);
			Log::log(LOG_CHAT, "Chat: src=%d chl=%d msg=%.*s", roleid, channel, out.size(), (char*)out.begin()); 
		}
		localsid = sid;
		switch (channel)
		{
			case GP_CHAT_LOCAL:
			case GP_CHAT_TRADE:
				sinfo->protostat.publicchat++;
			case GP_CHAT_TEAM:
			case GP_CHAT_WORLD:
			case GP_CHAT_ZONE:
			case GP_CHAT_SPEAKER:
			case GP_CHAT_RUMOR:
				if (channel == GP_CHAT_ZONE)
				{
					RoleData * role = lsm->GetRoleInfo(roleid);
					if (role == NULL || role->src_zoneid == 0)
						return;
				}
				GProviderServer::GetInstance()->DispatchProtocol(sinfo->gsid,this);
				break;
			case GP_CHAT_BROADCAST:	
				//privilege check
				if (! lsm->PrivilegeCheck(sid, roleid,Privilege::PRV_BROADCAST) )
				{
					Log::log(LOG_ERR,"WARNING: user %d GM_OP_BROADCAST permission denied.\n", roleid);
					return;
				}
				else
				{
					Log::gmoperate(roleid,Privilege::PRV_BROADCAST,"Broadcast");
				}
				GLog::action("GM, gmid=%d:cmd=broadcast",roleid);
				GDeliveryClient::GetInstance()->SendProtocol(this);
				break;	
		}
	}
};

};

#endif
