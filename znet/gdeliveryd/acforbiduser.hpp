
#ifndef __GNET_ACFORBIDUSER_HPP
#define __GNET_ACFORBIDUSER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "base64.h"

namespace GNET
{

class ACForbidUser : public GNET::Protocol
{
	#include "acforbiduser"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
	//	STAT_MIN5("ACKickoutUser", 1);
		LOG_TRACE("ACForbidUser: userid=%lld, roleid=%lld, forbid_time=%d, score=%d", accountid, roleid, forbid_time, score);
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		int real_userid = accountid;
		int src_zoneid = 0;
		if (!is_central)
		{
			if (real_userid <= 0)
			{
				int uid = UserContainer::Roleid2Userid(roleid);
				if (uid == 0)
				{
					LOG_TRACE("ACForbidUser LegacyFetch userid, roleid=%lld", roleid);
					UserContainer::LegacyFetchUserid(manager, sid, PROTOCOL_ACFORBIDUSER, this->Clone(), roleid);
					return;
				}
				real_userid = uid;
			}
		}
		else
		{
			if (roleid > 0)		
			{
				if (real_userid <= 0)
					real_userid = UserContainer::Roleid2Userid(roleid);
				GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(roleid);
				if (!real_userid || (!roleinfo||roleinfo->src_zoneid==0))
				{
					LOG_TRACE("ACForbidUser LegacyFetch userid, roleid=%lld", roleid);
					UserContainer::LegacyFetchUserid(manager, sid, PROTOCOL_ACFORBIDUSER, this->Clone(), roleid);
					return;
				}
				src_zoneid = (roleinfo==NULL?0:roleinfo->src_zoneid);
			}
		}
		Kickout(manager, sid, real_userid, src_zoneid);
	}

	void Kickout(Manager *manager, Manager::Session::ID sid, int real_userid, int src_zoneid)
	{
		if (real_userid <= 0) return;
		bool is_central = GDeliveryServer::GetInstance()->IsCentralDS();
		bool kick_remote = false;
		if (!is_central && manager==CentralDeliveryClient::GetInstance())
			kick_remote = true;
		if (!kick_remote)
		{
			STAT_MIN5("ACKickoutUser", 1);
			LOG_TRACE("ACForbidUser do Kickout! userid %d src_zoneid %d", real_userid, src_zoneid);
			Octets reason_encrypt;
			Base64Encoder::Convert(reason_encrypt, logreason);
			Log::formatlog("acforbiduser", "gmuser=1984:userid=%d:roleid=%lld:forbid_time=%d:reason=%.*s",
					real_userid, roleid, forbid_time, reason_encrypt.size(), (char*)reason_encrypt.begin());
		}
		UserInfo *pinfo = UserContainer::GetInstance().FindUser(real_userid);
		if (src_zoneid <= 0 && pinfo != NULL)
			src_zoneid = pinfo->src_zoneid;
		if (pinfo && forbid_time != 1) {
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, KickoutUser(real_userid, pinfo->localsid, (forbid_time != -1) ? ERR_ACKICKOUT : 0));
			UserContainer::GetInstance().UserLogout(pinfo, KICKOUT_LOCAL, true);
		}
		if (!is_central)
		{
			if (forbid_time > 1) {
				GRoleForbid forbid(Forbid::FBD_FORBID_LOGIN, forbid_time, time(NULL), reason);
				ForbidLogin::GetInstance().SetForbidLogin(real_userid, forbid);
			}
			if (forbid_time >= 0) {
				GMKickoutUser gmkou;
				gmkou.gmroleid = 1984;
				gmkou.kickuserid = real_userid;
				gmkou.forbid_time = forbid_time;
				gmkou.reason.swap(reason);
				if (GAuthClient::GetInstance()->SendProtocol(gmkou))
				{
					LOG_TRACE("ACForbidUser tell AU success");
					if (!kick_remote)
						manager->Send(sid, GMKickoutUser_Re(ERR_SUCCESS, 1984, 0, real_userid));
				}
				else
				{
					Log::log(LOG_ERR, "ACForbidUser kickuserid %d send to AU error, kick_remote=%d", real_userid, kick_remote==true?1:0);
					if (!kick_remote)
						manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION,1984,0,real_userid));
				}
			}
			else if (forbid_time == -2)
			{
				DBForbidUser * rpc = (DBForbidUser *)Rpc::Call(RPC_DBFORBIDUSER, ForbidUserArg(1, 1984, 3, real_userid, forbid_time, reason, score));
				bool success = GameDBClient::GetInstance()->SendProtocol(rpc);
				if (!kick_remote)
					manager->Send(sid, GMKickoutUser_Re(success?ERR_SUCCESS:ERR_COMMUNICATION, 1984,0,real_userid));
			}
		}
		else
		{
			if (src_zoneid != 0)
			{
				this->accountid = real_userid;
				if (CentralDeliveryServer::GetInstance()->DispatchProtocol(src_zoneid, this))
				{
					LOG_TRACE("ACForbidUser:Dispatch to src_zoneid %d", src_zoneid);
					manager->Send(sid, GMKickoutUser_Re(ERR_SUCCESS, 1984, 0, real_userid));
				}
				else
					manager->Send(sid, GMKickoutUser_Re(ERR_COMMUNICATION, 1984, 0, real_userid));
			}
			else
			{
				manager->Send(sid,GMKickoutUser_Re(ERR_COMMUNICATION, 1984,0,real_userid));
				Log::log(LOG_ERR, "ACForbidUser on central DS, user %d src_zoneid is 0", real_userid);
			}
		}
	}
};

};

#endif
