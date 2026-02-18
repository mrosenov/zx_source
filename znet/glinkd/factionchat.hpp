
#ifndef __GNET_FACTIONCHAT_HPP
#define __GNET_FACTIONCHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "chatmessage.hpp"
namespace GNET
{

class FactionChat : public GNET::Protocol
{
	#include "factionchat"
	void SendForbidInfo(GLinkServer* lsm,Manager::Session::ID sid,const GRoleForbid& forbid)
	{
		lsm->Send(sid,AnnounceForbidInfo(src,_SID_INVALID,forbid));
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (manager==GDeliveryClient::GetInstance())
		{
			if ( GLinkServer::IsRoleOnGame( localsid ) )
			{
				if(channel!=GP_CHAT_SYSTEM)
					GLinkServer::GetInstance()->Send(localsid,this);
				else
				{
					ChatMessage chatmsg(GP_CHAT_SYSTEM,0,src,msg, data);
					GLinkServer::GetInstance()->Send(localsid,chatmsg);
				}
			}
		}
		else if(manager==GLinkServer::GetInstance())
		{
			if(msg.size()>256 || data.size()>100)
				return;
			GLinkServer* lsm=GLinkServer::GetInstance();
			SessionInfo * sinfo = lsm->GetSessionInfo(sid);
			if (!sinfo || sinfo->roleid!=src || src<=0 || !sinfo->policy.Update(CHAT_POLICY))
				return;
			GRoleForbid forbid;
			if (lsm->IsForbidChat(sinfo->userid, src, forbid))
			{
				SendForbidInfo(lsm, sid, forbid);
				return;
			}
			GProviderServer::GetInstance()->DispatchProtocol(sinfo->gsid,this);
		}
	}
};

};

#endif
