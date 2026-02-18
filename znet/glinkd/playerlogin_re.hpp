
#ifndef __GNET_PLAYERLOGIN_RE_HPP
#define __GNET_PLAYERLOGIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "selectrole_re.hpp"
#include "statusannounce.hpp"
#include "glinkserver.hpp"
namespace GNET
{

class PlayerLogin_Re : public GNET::Protocol
{
	#include "playerlogin_re"
	void ConvertErrCode()
	{
		if (result==-1) result=ERR_GAMEDB_FAIL;
		else if (result==1) result=ERR_ENTERWORLD_FAIL;
		else if (result==2) result=ERR_EXCEED_MAXNUM;
		else if (result==3 || result==4) result=ERR_IN_WORLD;
		else if (result==5) result=ERR_INSTANCE_OVERFLOW;
		else if (result==ERR_COMMUNICATION || result== ERR_SERVEROVERLOAD || result==ERR_EXCEED_LINE_MAXNUM || result==ERR_INVALID_LINEID || result==ERR_CDS_COMMUNICATION || result==ERR_FORBIDROLE_GLOBAL)
			;
		else result=ERR_ENTERWORLD_FAIL;
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		SelectRole_Re cmd(result);
		LOG_TRACE("Recv PlayerLogin_Re roleid %d ret %d flag %d, worldtag=%d", roleid, result, flag, worldtag);
		if (result!=ERR_SUCCESS)
		{
			if (result == ERR_ROLEFORBID)
			{
				if (flag == 0 || flag == DIRECT_TO_RAID)
					lsm->ChangeState(localsid,&state_GSelectRoleServer);
				else
				{
					Log::log(LOG_DEBUG,"glinkd::cross server(flag=%d)login failed,role forbid,roleid=%d,gameid=%d,localsid=%d",
							flag,roleid,src_provider_id,localsid);
					lsm->ChangeState(localsid, &state_Null);
				}
			}
			else
			{
				Log::log(LOG_DEBUG,"glinkd::playerlogin_re login failed,result=%d,roleid=%d,gameid=%d,localsid=%d",
					result,roleid,src_provider_id,localsid);
				ConvertErrCode();
				cmd.result = result;
				lsm->ChangeState(localsid,&state_Null);
				GLinkServer::GetInstance()->SendErrorInfo(localsid, cmd.result, "Server Network Error.");
			}
		}
		else
		{
			if(flag == DIRECT_TO_RAID || flag == DIRECT_TO_CNETRALMAP)
				cmd.direct_enter_raid = 1;

			cmd.select_roleinfo.worldtag = this->client_tag;
			cmd.select_roleinfo.posx = this->x;
			cmd.select_roleinfo.posy = this->y;
			cmd.select_roleinfo.posz = this->z;

			if (flag != 0)
			{
				try
				{
					Marshal::OctetsStream(this->roleinfo_pack) >> cmd.select_roleinfo;
				}
				catch(...)
				{
					Log::log(LOG_ERR, "PlayerLogin_Re %d roleinfo unpack error", roleid);
					lsm->Close(localsid);
				}
			}
			lsm->RoleLogin(localsid, roleid, src_provider_id, cmd.auth, cmd.select_roleinfo.src_zoneid);
			cmd.lastlogin_ip = this->lastlogin_ip; 
			cmd.checksum = this->checksum;
			DEBUG_PRINT("selectrole_Re:result=%d, auth=%d, lastlogin_ip=%d direct_enter_raid=%d,x=%f,y=%f,z=%f,worldtag=%d", cmd.result, cmd.auth.size(), cmd.lastlogin_ip,cmd.direct_enter_raid,x,y,z,cmd.select_roleinfo.worldtag);
			GLinkServer::GetInstance()->Send(localsid,cmd);
		}
	}
};

};

#endif
