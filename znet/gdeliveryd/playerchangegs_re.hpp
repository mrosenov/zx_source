
#ifndef __GNET_PLAYERCHANGEGS_RE_HPP
#define __GNET_PLAYERCHANGEGS_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "localmacro.h"
#include "disconnectplayer.hpp" 
#include "tradediscard.hpp"
#include "userlogout.hrp"
#include "logoutroletask.h"
#include "mapforbid.h"
#include "trade.h"
#include "territorymanager.h"
#include "trychangegs_re.hpp"
#include "blockedrole.h"
#include "playerlogin.hpp"
namespace GNET
{

class PlayerChangeGS_Re : public GNET::Protocol
{
	#include "playerchangegs_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT( "recv playerchangegs_re,retcode=%d,roleid=%d,localsid=%d,dstgslineid=%d,mapid=%d,"
				"reason=%d, x=%f, y=%f, z=%f\n",
				retcode, roleid, localsid, gslineid, mapid, reason, scalex, scaley, scalez );
		GDeliveryServer* lsm = GDeliveryServer::GetInstance();
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if( NULL == pinfo || pinfo->localsid!=localsid || BlockedRole::GetInstance()->IsRoleBlocked(roleid))
			return;
		//检查发消息的gs是当前gs
		int sourcegs = GProviderServer::GetInstance()->GetGSID(sid);
		if(pinfo->gameid != sourcegs)
		{
			DEBUG_PRINT("playerchangegs_re sourcegs invalid, retcode=%d,roleid=%d,sourcegs=%d,currentgs=%d",
				retcode, roleid, sourcegs, pinfo->gameid);
			return;
		}
		//检查是否已开始换线
		if(pinfo->ingame == false)
		{
			DEBUG_PRINT("playerchangegs_re already in switch line");
			return;
		}
		// 收到gs发来的换线结果，先把数据保存起来
		pinfo->chgs_dstlineid = gslineid;
		pinfo->chgs_mapid = mapid;
		pinfo->chgs_pos.x = scalex;
		pinfo->chgs_pos.y = scaley;
		pinfo->chgs_pos.z = scalez;
		TerritoryManager::GetInstance()->OnChangeGS(roleid, retcode, pinfo->gameid, pinfo->world_tag, gslineid, mapid, pinfo->chgs_territoryrole);
		KingdomManager::GetInstance()->OnChangeGS(roleid, retcode, pinfo->gameid, pinfo->world_tag, gslineid, mapid);
		OpenBanquetManager::GetInstance()->OnChangeGS(roleid, retcode, pinfo->gameid, pinfo->world_tag, gslineid, mapid);
/*
		if( ERR_CHGS_SUCCESS == retcode )
		{
			// 这里执行登录逻辑
			if( _GAMESERVER_ID_INVALID != gslineid )
			{
				PlayerLogin pl( roleid, gslineid, pinfo->user->linkid, pinfo->localsid );
				pl.auth = pinfo->user->privileges;
				pl.usepos = 1;// 只要是changegs 逻辑就用
				pl.x = scalex;
				pl.y = scaley;
				pl.z = scalez;
				pl.worldtag = mapid;
				pl.loginip = pinfo->user->ip;
				pl.territory_role = territoryrole;
				
				if( GProviderServer::GetInstance()->DispatchProtocol( gslineid, pl ) )
				{
					ForbiddenUsers::GetInstance().Push(pinfo->userid,roleid,pinfo->user->status);
					pinfo->user->status = _STATUS_SWITCH;
					retcode = ERR_CHGS_INVALIDGS;
					gslineid = pinfo->gameid;
				}
			}
			return;
		}
*/
		if( ERR_CHGS_DBERROR == retcode )
		{
			lsm->Send(pinfo->linksid, DisconnectPlayer( roleid, -1, pinfo->localsid, -1 ));
			UserContainer::GetInstance().UserLogout(pinfo->user);
			return;
		}
		//ERR_SUCCESS 或者其他错误
		if (retcode != ERR_SUCCESS)
		{//玩家继续在原 gs 游戏
			retcode = ERR_CHG_GS_STATUS;
//			pinfo->user->status = _STATUS_ONGAME;
                        if(RaidManager::GetInstance()->IsRaidLine(gslineid))
	                        RaidManager::GetInstance()->GSRoleLeave(gslineid,roleid);
		}
		else
		{
			//can not enter battlefield in special gs,so clear
			ContestManager::GetInstance().OnPlayerLogout(roleid);
			if(!GDeliveryServer::GetInstance()->IsCentralDS())
			{
				BattleManager::GetInstance()->OnLogout(roleid, pinfo->gameid);
			}
			InstancingManager::GetInstance()->OnLogout(roleid, pinfo->gameid);
			SNSPlayerInfoManager::GetInstance()->OnPlayerLogout(roleid);
			FunGameManager::GetInstance().OnPlayerLogout(roleid);
			HometownManager::GetInstance()->OnPlayerLogout(roleid);
		       	if(pinfo->sectid)
	                       SectManager::Instance()->OnLogout(pinfo->sectid, roleid, pinfo->level, pinfo->occupation);		      
			LogoutRoleTask::Add(*pinfo);
			pinfo->ingame = false;
			pinfo->user->status = _STATUS_SWITCH;

			ACStatusAnnounce2 acsa;
			acsa.status = _STATUS_OFFLINE;
			acsa.info_list.push_back( ACOnlineStatus2(roleid,0,0) );
			GAntiCheatClient::GetInstance()->SendProtocol(acsa);
		}
		// 如果换线不成功填充原gsid
		//gslineid = pinfo->gameid;
		DEBUG_PRINT( "send link playerchangegs_re\n" );
		lsm->Send( pinfo->linksid, TryChangeGS_Re(retcode, roleid, gslineid, localsid, reason));
	}
};

};

#endif
