
#ifndef __GNET_GMKICKOUTUSER_HPP
#define __GNET_GMKICKOUTUSER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "gmkickoutuser_re.hpp"
#include "kickoutuser.hpp"
#include "gauthclient.hpp"
#include "forbid.hxx"
#include "mapforbid.h"
#include "dbforbiduser.hrp"
namespace GNET
{
class GMKickoutUser : public GNET::Protocol
{
	#include "gmkickoutuser"
	void Kickout(Manager *manager, Manager::Session::ID sid, int userid, int src_zoneid)
	{
		LOG_TRACE("GMKickoutUser do Kickout! userid %d src_zoneid %d", userid, src_zoneid);
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		bool kick_remote = false;
		int masterid = 0;
		if (!is_central && manager==CentralDeliveryClient::GetInstance())
		{
			kick_remote = true; //封禁信息是跨服转发过来的
			masterid = 888;
		}
		else
		{
			GameMaster* master = MasterContainer::Instance().Find(gmroleid);
			if(!master)
				return;
			masterid = master->userid;
		}
		Log::formatlog("gamemaster","GMKickoutUser:GM=%d:userid=%d:forbid_time=%d:reason_size=%d", masterid, userid, forbid_time, reason.size());
		//add user to forbidlogin map
		if ( forbid_time>=1 )
		{
			UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
			if (NULL!=pinfo)
			{
				GDeliveryServer::GetInstance()->Send(pinfo->linksid,KickoutUser(userid, pinfo->localsid, ERR_KICKOUT));
				UserContainer::GetInstance().UserLogout(pinfo,KICKOUT_LOCAL,true);
			}
		}
		if ( forbid_time > 0 )
		{
			if (!is_central)
			{
				DBForbidUser *rpc = (DBForbidUser *)Rpc::Call(RPC_DBFORBIDUSER, ForbidUserArg(1, masterid, 2, userid, forbid_time, reason));
				if (GameDBClient::GetInstance()->SendProtocol(rpc))
				{
					LOG_TRACE("GMKickoutUser tell DB success");
					if (!kick_remote)
						manager->Send(sid,GMKickoutUser_Re(ERR_SUCCESS,gmroleid,localsid,userid));
				}
				else
				{
					manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION, gmroleid,localsid,userid));
					Log::log(LOG_ERR, "DBForbidUser gmuserid %d kickuserid %d send to DB error", masterid, userid);
				}
			}
			else
			{
				if (src_zoneid != 0)
				{
/*
					char src_type = 0;
					if (manager == GDeliveryServer::GetInstance())
						src_type = 1;
					else if (manager == GAntiCheatClient::GetInstance())
						src_type = 2;
					if (CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, KickoutSendBack(gm_roleid, masterid, src_type, localsid, kickuserid, forbid_time, reason)))
*/
					if (CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, this))
					{
						LOG_TRACE("GMKickoutUser:GM=%d:userid=%d:Dispatch to src_zoneid %d", masterid, userid, src_zoneid);
						manager->Send(sid,GMKickoutUser_Re(ERR_SUCCESS,gmroleid,localsid,userid));
					}
					else
						manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION, gmroleid,localsid,userid));
				}
				else
				{
					manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION, gmroleid,localsid,userid));
					Log::log(LOG_ERR, "GMKickoutUser on central DS, user %d src_zoneid is 0", userid);
				}
			}
		}
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv GMKickoutUser");
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(kickuserid);
		int userid = UserContainer::Roleid2Userid(kickuserid);
		if(!userid || (is_central && (!roleinfo || roleinfo->src_zoneid == 0)))
		{
			UserContainer::LegacyFetchUserid(manager, sid, PROTOCOL_GMKICKOUTUSER, this->Clone(), kickuserid);
			LOG_TRACE("GMKickoutUser LegacyFetch userid, roleid=%d", kickuserid);
			return;
		}
		Kickout(manager, sid, userid, roleinfo==NULL?0:roleinfo->src_zoneid);
	}
};

};

#endif
