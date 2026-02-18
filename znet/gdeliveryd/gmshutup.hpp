
#ifndef __GNET_GMSHUTUP_HPP
#define __GNET_GMSHUTUP_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gauthclient.hpp"
#include "gdeliveryserver.hpp"
#include "maplinkserver.h"
#include "centraldeliveryserver.hpp"

namespace GNET
{

class GMShutup : public GNET::Protocol
{
	#include "gmshutup"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv GMShutUp gmroleid %d dstroleid %d", gmroleid, dstuserid);
		int uid = UserContainer::Roleid2Userid(dstuserid);
		if(uid) 
		{
			int tmp_dstuserid = dstuserid;
			dstuserid = uid;
			LinkServer::GetInstance().BroadcastProtocol( this );
			GRoleForbid forbid(Forbid::FBD_FORBID_TALK, forbid_time, 0, reason);
			ForbidUserTalk::GetInstance().SetForbidUserTalk(dstuserid, forbid);
			bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
			if (!is_central)
				GAuthClient::GetInstance()->SendProtocol(this);
			else
			{
				UserInfo * pinfo = UserContainer::GetInstance().FindUser(dstuserid);
				if (pinfo && pinfo->src_zoneid!=0)
				{
					this->dstuserid = tmp_dstuserid;
					if (CentralDeliveryServer::GetInstance()->DispatchProtocol(pinfo->src_zoneid, this))
						LOG_TRACE("GMShutUp dispatch to src_zoneid %d success, dstroleid %d", pinfo->src_zoneid, dstuserid);
					else
						Log::log(LOG_ERR, "GMShutUp gmroleid %d dstuserid %d dispatch to src_zoneid %d error", gmroleid, dstuserid, pinfo->src_zoneid);
				}
				else
					Log::log(LOG_ERR, "GMShutUp gmroleid %d dstuserid %d src_zoneid %d invalid", gmroleid, dstuserid, pinfo==NULL?-1:0);
			}
		}
	}
};

};

#endif
