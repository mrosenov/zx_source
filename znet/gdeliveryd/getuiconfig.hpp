
#ifndef __GNET_GETUICONFIG_HPP
#define __GNET_GETUICONFIG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryserver.hpp"
#include "getuiconfig_re.hpp"

#include "getrolebase.hrp"
#include "gamedbclient.hpp"
#include "gtplatformagent.h"
#include "crossbattlemanager.hpp"
#include "mapuser.h"
#include "raidmanager.h"
namespace GNET
{

class GetUIConfig : public GNET::Protocol
{
	#include "getuiconfig"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
		if(info)
		{
			LOG_TRACE( "GetUIConfig: roleid=%d,size=%d",roleid,info->config_data.size());
			GetUIConfig_Re	re(ERR_SUCCESS,roleid,localsid,Octets());
			//大于4000说明client存的数据太大，此时数据已错，发回空值，client重新设置这个值			
			if(info->config_data.size()<=4000)
				re.ui_config = info->config_data;
			manager->Send(sid,re);
			GTPlatformAgent::OnPlayerLogin(roleid,*info);
	                PlayerInfo * player = UserContainer::GetInstance().FindRole(roleid);
        	        if(player && player->factionid)
				if(!RaidManager::GetInstance()->IsRaidLine(player->gameid))		
					RaidManager::GetInstance()->NotifyRolePkFaction(player->factionid,roleid);
		
			// 收到这条GetUIConfig表明客户端已经初始化好了
			// 如果加入跨服战场，根据战场是否开启回复不同的消息，以便客户端显示战场查询按钮
			
			CrossBattleManager *crossbattle = CrossBattleManager::GetInstance();
			// 流水席战场，要根据玩家状态，显示报名界面
			const CrossBattleInfo* pcbif = crossbattle->GetCrossBattleInfo(roleid, OPENBANQUET_BATTLE);
			if(pcbif != NULL && GDeliveryServer::GetInstance()->IsCentralDS())
			{
				OpenBanquetManager::GetInstance()->OnQueue(roleid, pcbif->_gsid, pcbif->_mapid);
			}

			const FightingTag *tag = crossbattle->GetFightingTag(roleid);
			if(tag != NULL && GDeliveryServer::GetInstance()->IsCentralDS())
			{
				if(tag->_isactive == true)
				{
					Log::log(LOG_DEBUG, "roleid=%d enter crossbattle", roleid);
					manager->Send(sid, BattleEnter_Re(ERR_SUCCESS, roleid, tag->_endtime, tag->_subbattle_type, 0, false));
				}

				OpenBanquetManager::GetInstance()->OnRealEnter(roleid, tag->_gsid, tag->_tag);
				// 收到上面的消息后，玩家已经在战场中，因此需要删除其可以进入的战场信息
				crossbattle->EraseFightingTag(roleid);
			}
			else
			{
				LOG_TRACE( "GetUIConfig: PlayerLogin roleid=%d",roleid);
				crossbattle->PlayerLogin(roleid);
			}

			
		}
		else
			Log::log(LOG_ERR, "GetUIConfig, role cache miss, roleid=%d", roleid);
	}
};

};

#endif
