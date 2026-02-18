
#ifndef __GNET_SENDDATAANDIDENTITY_RE_HPP
#define __GNET_SENDDATAANDIDENTITY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "playerchangeds_re.hpp"
#include "activateplayerdata.hrp"
#include "delplayerdata.hrp"

namespace GNET
{

class SendDataAndIdentity_Re : public GNET::Protocol
{
	#include "senddataandidentity_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv SendDataAndIdentity_Re retcode %d roleid %d userid %d flag %d dst_zoneid %d", retcode, roleid, userid, flag, dst_zoneid);
		if (IsTransToCentralDS(flag) || flag == DIRECT_TO_CENTRALDS)
		{
			if (GDeliveryServer::GetInstance()->IsCentralDS())
				return;
		}
		else if (IsTransToNormalDS(flag))
		{
			if (!GDeliveryServer::GetInstance()->IsCentralDS())
				return;
		}
		else
			return;
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if (pinfo == NULL || (pinfo->status != _STATUS_REMOTE_HALFLOGIN && pinfo->status != _STATUS_REMOTE_CACHERANDOM))
		{
			Log::log (LOG_ERR, "SendDataAndIdentity_Re userid %d roleid %d ret %d, user status %d invalid",
					userid, roleid, retcode, pinfo==NULL?0:pinfo->status);
			return;
		}
		if (retcode != ERR_SUCCESS)
		{
			Log::log(LOG_ERR, "SendDataAndIdentity_Re roleid %d userid %d, errno %d", roleid, userid, retcode);
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, KickoutUser(userid, pinfo->localsid, retcode));
			UserContainer::GetInstance().UserLogout(pinfo);
			if (flag == DIRECT_TO_CENTRALDS && retcode == ERR_REMOTE_VERIFYFAILED)
			{
				LOG_TRACE("SendDataAndIdentity_Re Activate roleid %d data", roleid);
				GameDBClient::GetInstance()->SendProtocol((ActivatePlayerData *)Rpc::Call(RPC_ACTIVATEPLAYERDATA, ActivatePlayerDataArg(roleid)));
			}
			else
				RemoteLoggingUsers::GetInstance().Pop(userid);
			return;
		}
		PlayerChangeDS_Re re(ERR_SUCCESS, roleid, userid, pinfo->chgds_lineid, flag, pinfo->rand_key, dst_zoneid, pinfo->localsid);
		GDeliveryServer::GetInstance()->Send(pinfo->linksid, re);
		LOG_TRACE("Send PlayerChangeDS_Re to link, roleid %d userid %d gsid %d flag %d rand_key.size %d dst_zoneid %d localsid %d",
							re.roleid, re.userid, re.gsid, re.flag, re.random.size(), re.dst_zoneid, pinfo->localsid);
		STAT_MIN5("LogoutNormal", 1);	
		if (flag == CENTRALDS_TO_DS)
		{
			pinfo->src_zoneid = 0; //此处用了一个小捷径 目的是 UserLogout 时不向原服发 RemoteLogout. 因为此时原服 User 已经清除 (SendDataAndIdentity)
			UserContainer::GetInstance().UserLogout(pinfo); //pinfo 析构
			RemoteLoggingUsers::GetInstance().Pop(userid);
			DelPlayerData * rpc = (DelPlayerData *)Rpc::Call(RPC_DELPLAYERDATA, DelPlayerDataArg(roleid, userid));
			if (!GameDBClient::GetInstance()->SendProtocol(rpc))
				Log::log(LOG_ERR, "DelPlayerData send to DB error, roleid=%d", roleid, userid);
			else
				LOG_TRACE("Tell DB to delete player data roleid=%d", roleid);
		}
	}
};

};

#endif
