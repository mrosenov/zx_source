
#ifndef __GNET_REMOTELOGINQUERY_RE_HPP
#define __GNET_REMOTELOGINQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "touchplayerdata.hrp"
#include "playerlogin_re.hpp"

namespace GNET
{

class RemoteLoginQuery_Re : public GNET::Protocol
{
	#include "remoteloginquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("Recv RemoteLoginQuery_Re retcode %d roleid %d userid %d flag %d",
					retcode, roleid, userid, flag);
		GDeliveryServer * dsm = GDeliveryServer::GetInstance();
		if (!dsm->IsCentralDS())
			return;
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if (pinfo == NULL || pinfo->status != _STATUS_REMOTE_LOGINQUERY)
		{
			Log::log(LOG_ERR, "RemoteLoginQuery_Re userid %d status %d invalid",
					userid, pinfo==NULL?0:pinfo->status);
			return;
		}
		//PlayerIdentityMatch Ê± Push µÄ
		RemoteLoggingUsers::GetInstance().Pop(userid);
		if (retcode == ERR_SUCCESS && flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLEMAP /*|| flag == DS_TO_OPENBANQUET*/)
		{
			TouchPlayerData * rpc = (TouchPlayerData *)Rpc::Call(RPC_TOUCHPLAYERDATA, TouchPlayerDataArg(roleid, userid));
			if (!GameDBClient::GetInstance()->SendProtocol(rpc))
			{
				Log::log(LOG_ERR, "RemoteLoginQuery_Re TouchPlayerData Failed, roleid %d, userid %d", roleid, userid);
				retcode = 105;
			}
			else
				LOG_TRACE("Send to TouchPlayerData userid %d roleid %d", userid, roleid);
		}
		if (retcode != ERR_SUCCESS || flag == DIRECT_TO_CENTRALDS || flag == /*DIRECT_TO_OPENBANQUET*/DIRECT_TO_CNETRALMAP)
		{
			PlayerLogin_Re re;
			try
			{
				Marshal::OctetsStream(pinfo->playerlogin_re_pack) >> re;
				pinfo->playerlogin_re_pack.clear();
			}
			catch(...)
			{
				Log::log(LOG_ERR, "RemoteLoginQuery_Re userid %d roleid %d unpack error", userid, roleid);
				return;
			}
			re.result = retcode;
			if (retcode == ERR_SUCCESS)
			{
				int lastlogin_time = 0;
				GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
				if(info)
				{
					lastlogin_time = info->lastlogin_time;
				}
				PlayerLogin_Re::RealLogin(roleid, pinfo, re);
				DEBUG_PRINT( "RemoteLoginQuery_Re worldtag=%d, x=%f, y=%f, z=%f\n", re.worldtag, re.x, re.y, re.z);

				pinfo->status = _STATUS_READYGAME;
				info = RoleInfoCache::Instance().Get(roleid);
				if (info)
				{
					GRoleInfo roleinfo_chop = RoleInfoCache::ChopRoleInfo(*info);
					RoleInfo roleinfo;
					RoleInfoCache::ConvertRoleInfo(roleinfo_chop, roleinfo);
					roleinfo.lastlogin_time = lastlogin_time;
					DEBUG_PRINT( "RemoteLoginQuery_Re flag=%d, roleinfo.worldtag=%d, re.worldtag=%d", flag, roleinfo.worldtag, re.worldtag);
					//if(flag == /*DIRECT_TO_OPENBANQUET*/DIRECT_TO_CNETRALMAP)
					//{
					//	roleinfo.worldtag = re.worldtag;
					//}
					re.roleinfo_pack.swap(Marshal::OctetsStream()<<roleinfo);
				}
				else
					Log::log(LOG_ERR, "RemoteLoginQuery_Re, %d roleinfo not in cache", roleid);
			}
			else
			{
				UserContainer::GetInstance().UserLogout(pinfo);
			}

			re.src_provider_id = pinfo->gameid;
			re.lastlogin_ip = pinfo->lastlogin_ip;
			GDeliveryServer* dsm=GDeliveryServer::GetInstance();
			dsm->Send(pinfo->linksid, re);
			dsm->BroadcastStatus();
			LOG_TRACE("Send PlayerLogin_Re to glink, userid %d roleid %d ret %d, x=%f,y=%f,z=%f",
					userid, roleid, re.result, re.x, re.y, re.z);
		}
/*
   PlayerLogin_Re::RealLogin(roleid, pinfo, re);
   pinfo->status = _STATUS_READYGAME;
*/
	}
};

};

#endif
