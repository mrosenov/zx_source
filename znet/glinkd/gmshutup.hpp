
#ifndef __GNET_GMSHUTUP_HPP
#define __GNET_GMSHUTUP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryclient.hpp"
#include "glinkserver.hpp"
#include "gmshutup_re.hpp"
#include "privilege.hxx"
namespace GNET
{

class GMShutup : public GNET::Protocol
{
	#include "gmshutup"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (manager==lsm)
		{
			//只有协议来自客户端才做下面的操作，来自delivery不做此操作
			if (! lsm->PrivilegeCheck(sid, gmroleid,Privilege::PRV_FORBID_TALK) )
			{
				Log::log(LOG_ERR,"WARNING: user %d try to use PRV_FORBID_TALK privilege that he doesn't have.\n",gmroleid);
				return;
			}
			char content[256];
			sprintf(content,"ShutupUser: shutupUserid=%d,forbidtime=%d",dstuserid,forbid_time);
			Log::gmoperate(gmroleid,Privilege::PRV_FORBID_TALK,content);	
			
			GDeliveryClient::GetInstance()->SendProtocol(this);
			lsm->Send(sid,GMShutup_Re(ERR_SUCCESS,dstuserid,forbid_time));
		}
		lsm->ForbidUserChat(dstuserid, forbid_time, reason);
	}
};

};

#endif
