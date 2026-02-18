
#ifndef __GNET_ACKICKOUTUSER_HPP
#define __GNET_ACKICKOUTUSER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gmkickoutuser.hpp"
#include "gdeliveryserver.hpp"
#include "gauthclient.hpp"
#include "dbforbiduser.hrp"

namespace GNET
{

class ACKickoutUser : public GNET::Protocol
{
	#include "ackickoutuser"
	void Kickout(Manager *manager, Manager::Session::ID sid, int real_userid, int src_zoneid)
	{
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		bool kick_remote = false;
		if (!is_central && manager==CentralDeliveryClient::GetInstance())
			kick_remote = true;
		if (!kick_remote)
		{
			STAT_MIN5("ACKickoutUser", 1);
			LOG_TRACE("ACKickoutUser do Kickout! userid %d src_zoneid %d", real_userid, src_zoneid);
		}
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(real_userid);
		//如果 idtype == 0 即通过userid封禁跨服上的玩家 只有玩家在线才能被正确地redirect 到原服
		if (src_zoneid == 0 && pinfo != NULL)
			src_zoneid = pinfo->src_zoneid;
		if (NULL!=pinfo && forbid_time!=1)
		{
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,KickoutUser(real_userid, pinfo->localsid, (forbid_time != -1)?ERR_ACKICKOUT:0));
			UserContainer::GetInstance().UserLogout(pinfo,KICKOUT_LOCAL,true);
		}
		if (!is_central)
		{ 
			if (idtype == 0)
			{
				//add user to forbidlogin map
				if ( forbid_time>1 )
				{
					GRoleForbid forbid(Forbid::FBD_FORBID_LOGIN,forbid_time,time(NULL),reason); 
					ForbidLogin::GetInstance().SetForbidLogin( real_userid, forbid );
				}
				// send to Auth,let AU send kickoutuser command
				if ( forbid_time >= 0 )
				{ 
					GMKickoutUser gmkou;
					gmkou.gmroleid = gmuserid;
					gmkou.kickuserid = real_userid;
					gmkou.forbid_time = forbid_time;
					gmkou.reason.swap(reason);
					if ( GAuthClient::GetInstance()->SendProtocol(gmkou) )
					{
						LOG_TRACE("ACKickoutUser tell AU success");
						if (!kick_remote)
							manager->Send(sid,GMKickoutUser_Re(ERR_SUCCESS,gmuserid,0,real_userid));
					}
					else		
					{
						Log::log(LOG_ERR, "ACKickoutUser kickuserid %d send to AU error, kick_remote=%d", real_userid, kick_remote==true?1:0);
						if (!kick_remote)
							manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION,gmuserid,0,real_userid));
					}
				}
			}
			else if(forbid_time!=-1)
			{
				char oper = forbid_time>1 ? 1 : 2; // 1 封禁  2 解封
				DBForbidUser *rpc = (DBForbidUser *)Rpc::Call(RPC_DBFORBIDUSER, ForbidUserArg(oper, gmuserid, 1, real_userid, forbid_time, reason));
				if( GameDBClient::GetInstance()->SendProtocol( rpc ) )
				{
					LOG_TRACE("ACKickoutUser tell DB success");
					if (!kick_remote)
						manager->Send(sid,GMKickoutUser_Re(ERR_SUCCESS,gmuserid,0,real_userid));
				}
				else
				{
					Log::log(LOG_ERR, "ACKickoutUser kickuserid %d send to AU error, kick_remote=%d", real_userid, kick_remote==true?1:0);
					if (!kick_remote)
						manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION,gmuserid,0,real_userid));
				}
			}
		}
		else
		{
			if (src_zoneid != 0)
			{
				if (CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, this))
				{
					LOG_TRACE("ACKickoutUser:Dispatch to src_zoneid %d", src_zoneid);
					manager->Send(sid, GMKickoutUser_Re(ERR_SUCCESS, gmuserid, 0, real_userid));
				}
				else
					manager->Send(sid, GMKickoutUser_Re(ERR_COMMUNICATION, gmuserid, 0, real_userid));
			}
			else
			{
				manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION, gmuserid,0,real_userid));
				Log::log(LOG_ERR, "ACKickoutUser on central DS, user %d src_zoneid is 0", real_userid);
			}
		}
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
	//	STAT_MIN5("ACKickoutUser", 1);
		LOG_TRACE("ACKickoutUser: GM=%d,idtype=%d,userid=%d,forbid_time=%d,reason_size=%d",gmuserid,idtype,userid,forbid_time,reason.size());

		int src_zoneid = 0;
		int real_userid = userid;
		if( idtype != 0 ) // byroleid
		{
			real_userid = UserContainer::Roleid2Userid(userid) ;
			bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
			GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(userid);
			if(!real_userid || (is_central && (!roleinfo||roleinfo->src_zoneid==0)))
			{
				LOG_TRACE("ACKickoutUser LegacyFetch userid, roleid=%d", userid);
				UserContainer::LegacyFetchUserid(manager, sid, PROTOCOL_ACKICKOUTUSER, this->Clone(), userid);
				return;
			}
			src_zoneid = (roleinfo==NULL?0:roleinfo->src_zoneid);
		}
		Kickout(manager, sid, real_userid, src_zoneid);
	}
};

};

#endif
