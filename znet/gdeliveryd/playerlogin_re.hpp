
#ifndef __GNET_PLAYERLOGIN_RE_HPP
#define __GNET_PLAYERLOGIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gauthclient.hpp"
#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "playeroffline.hpp"

#include "gauthclient.hpp"
#include "getrolebase.hrp"
#include "friendstatus.hpp"

#include "gamedbclient.hpp"
#include "conv_charset.h"
#include "announcegm.hpp"
#include "dochangegs_re.hpp"
#include "queryrewardtype_re.hpp"
#include "postoffice.h"
#include "chatroom.h"
#include "mapforbid.h"
#include "mapuser.h"
#include "announceserverattribute.hpp"
#include "maplinkserver.h"
#include "sectmanager.h"
#include "gamemaster.h"
#include "factionmanager.h"
#include "familymanager.h"
#include "siegemanager.h"
#include "disconnectplayer.hpp"
#include "territorymanager.h"
#include "kingdommanager.h"
#include "circlemanager.h"
#include "remoteloginquery.hpp"
#include "centraldeliveryserver.hpp"
#include "roleinfo"
#include "raidmanager.h"
#include "mappasswd.h"
#include "openbanquetmanager.h"
#include "crssvrteamsmanager.h"
#include "crosscrssvrteamsmanager.h"
#include "facbasemanager.h"


namespace GNET
{
class PlayerLogin_Re : public GNET::Protocol
{
	#include "playerlogin_re"
	static void RealLogin (int roleid, UserInfo * pinfo, const PlayerLogin_Re & pro)
	{
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, QueryRewardType_Re(roleid,pinfo->rewardtype,pinfo->rewarddata,
					(Passwd::GetInstance().GetAlgo(pinfo->userid) & ALGORITHM_FILL_INFO_MASK)));
		PlayerInfo* role = UserContainer::GetInstance().FindRole(roleid);
		if(!role)
			return;
		role->factionid = pro.factionid;
		role->factiontitle = pro.factiontitle;
		role->familyid = pro.familyid;
		role->level = pro.level;
		role->occupation = pro.occupation;
		role->jointime = pro.jointime;
		role->battlescore = pro.battlescore;
		role->world_tag = pro.worldtag;
		//getrolebase info
		if(pinfo->gmstatus & GMSTATE_ACTIVE)
		{
			MasterContainer::Instance().Insert(pinfo->userid, roleid, pinfo->linksid, pinfo->localsid, pinfo->privileges);
			AnnounceGM agm;
			agm.roleid = roleid;
			agm.auth = pinfo->privileges;
			GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, agm );
		}
		GRoleInfo* info = RoleInfoCache::Instance().GetOnlogin(roleid);
		if(info)
		{
			info->lastlogin_time = Timer::GetTime(); // 防止360账户登录小退后DBClearConsumable::Check360User再次被触发
			role->name = info->name;
			role->sectid = info->sectid;
			if(SectManager::Instance()->CheckSect(info->sectid, roleid))
				SectManager::Instance()->OnLogin(role->sectid, roleid, pinfo->gameid);
			UserContainer::GetInstance().InsertName( info->name, roleid );
			if(info->spouse)
			{
				PlayerInfo * su = UserContainer::GetInstance().FindRoleOnline(info->spouse);
				if(su )
					GDeliveryServer::GetInstance()->Send(su->linksid,FriendStatus(roleid,pinfo->gameid,su->localsid));
			}
		}
		//update player's mailbox
		PostOffice::GetInstance().OnRoleOnline( roleid,pinfo->linksid,pinfo->localsid );
		FactionManager::Instance()->OnLogin(roleid,pro.factionid,pro.factiontitle,pro.familyid,
				pro.contribution, pro.devotion, pinfo->gameid, pinfo->linksid, pinfo->localsid);
		SiegeManager::GetInstance()->OnLogin(roleid, pro.factionid, pinfo->linksid, pinfo->localsid);
		TerritoryManager::GetInstance()->OnLoginWorld(roleid);
		CircleManager::Instance()->OnLogin(roleid,pro.circleid, pinfo->gameid, pinfo->linksid, pinfo->localsid);
		
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		if (!dsm->IsCentralDS())
			CrssvrTeamsManager::Instance()->OnLogin(roleid);
		else
			CrossCrssvrTeamsManager::Instance()->OnLogin(roleid, pinfo->src_zoneid, pinfo->linksid, pinfo->localsid);

	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		int userid = UserContainer::Roleid2Userid(roleid);
		if(!userid)
			return;
		LOG_TRACE("playerlogin_re:result=%d,roleid=%d,userid=%d,fid=%d,title=%d,level=%d,occupation=%d,battlescore=%d,flag=%d,client_tag=%d", 
				result, roleid, userid, factionid, factiontitle, level, occupation,battlescore,flag,client_tag);

		// 将GRoleInfo里的位置缓存成gs发来的最新的位置和地图信息
		GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
		if(info)
		{
			info->worldtag = client_tag;
			info->posx = x;
			info->posy = y;
			info->posz = z;

			DEBUG_PRINT( "playerlogin_re:client_tag=%d, x=%f, y=%f, z=%f,info.worldtag=%d,info.x=%f,info.y=%f,info.z=%f\n", client_tag, x, y, z,info->worldtag,info->posx,info->posy,info->posz );
		}

		GDeliveryServer* dsm=GDeliveryServer::GetInstance();
		//remove user from forbidden login set
		ForbiddenUsers::GetInstance().Pop(userid);

		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if (NULL == pinfo)
		{
			/* send playeroffline to game server, src_provider_id is linkserver ID */
			if (manager->Send(sid,PlayerOffline(roleid,src_provider_id,localsid))) {
				ForbiddenUsers::GetInstance().Push(userid,roleid,_STATUS_ONLINE);
			}
			return;
		}
		/* if login success, set information in UserInfo struct*/
/*
		if (result==ERR_SUCCESS && pinfo->status!=_STATUS_SWITCH)
		{
			manager->Send(sid, QueryRewardType_Re(roleid,pinfo->rewardtype,pinfo->rewarddata algo));
			PlayerInfo* role = UserContainer::GetInstance().FindRole(roleid);
			if(!role)
				return;
			role->factionid = factionid;
			role->factiontitle = factiontitle;
			role->familyid = familyid;
			role->level = level;
			role->occupation = occupation;
			role->jointime = jointime;
			role->battlescore = battlescore;
			role->world_tag = worldtag;
			//getrolebase info
			if(pinfo->gmstatus & GMSTATE_ACTIVE)
			{
				MasterContainer::Instance().Insert(userid, roleid, pinfo->linksid, pinfo->localsid, pinfo->privileges);
				AnnounceGM agm;
				agm.roleid = roleid;
				agm.auth = pinfo->privileges;
				GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, agm );
			}
			GRoleInfo* info = RoleInfoCache::Instance().GetOnlogin(roleid);
			if(info)
			{
				info->lastlogin_time = Timer::GetTime(); // 防止360账户登录小退后DBClearConsumable::Check360User再次被触发
				role->name = info->name;
				role->sectid = info->sectid;
				if(SectManager::Instance()->CheckSect(info->sectid, roleid))
					SectManager::Instance()->OnLogin(role->sectid, roleid, pinfo->gameid);
				UserContainer::GetInstance().InsertName( info->name, roleid );
				if(info->spouse)
				{
					PlayerInfo * su = UserContainer::GetInstance().FindRoleOnline(info->spouse);
					if(su )
						dsm->Send(su->linksid,FriendStatus(roleid,pinfo->gameid,su->localsid));
				}
			}
			//update player's mailbox
			PostOffice::GetInstance().OnRoleOnline( roleid,pinfo->linksid,pinfo->localsid );
			FactionManager::Instance()->OnLogin(roleid,factionid,factiontitle,familyid,
					contribution, devotion, pinfo->gameid, pinfo->linksid, pinfo->localsid);
			SiegeManager::GetInstance()->OnLogin(roleid, factionid, pinfo->linksid, pinfo->localsid);
			TerritoryManager::GetInstance()->OnLoginWorld(roleid);
			CircleManager::Instance()->OnLogin(roleid,circleid, pinfo->gameid, pinfo->linksid, pinfo->localsid);
		}
*/
		if( _STATUS_SWITCH == pinfo->status )
		{
			PlayerInfo * player = UserContainer::GetInstance().FindRole(roleid);
			if(!player)
				return;
			LOG_TRACE("playerlogin_re switch line roleid=%d, from line=%d to line=%d client_tag %d",roleid, pinfo->gameid, player->chgs_dstlineid, client_tag);
			int raid_mapid = 0;
			if(RaidManager::GetInstance()->IsRaidLine(pinfo->gameid))
				RaidManager::GetInstance()->GSRoleLeave(pinfo->gameid,roleid);
			bool raid_enter=RaidManager::GetInstance()->GetMapid(roleid,raid_mapid);
			if(raid_enter && RaidManager::GetInstance()->IsRaidLine(player->chgs_dstlineid))
			{
				LOG_TRACE("Raidenter roleid=%d, worldtag=%d mapid=%d",roleid, worldtag, raid_mapid);
				worldtag = raid_mapid;
			}	
			if (client_tag == 0)
				client_tag = worldtag;
			DoChangeGS_Re chg_re( result, roleid, player->chgs_dstlineid, player->localsid, client_tag, 0, x, y, z);
			TerritoryManager::GetInstance()->OnLoginTerritory(roleid, result, player->gameid, player->world_tag, player->chgs_dstlineid, player->chgs_mapid);
			KingdomManager::GetInstance()->OnLoginBattle(roleid, result, player->gameid, player->world_tag, player->chgs_dstlineid, player->chgs_mapid);

			if( ERR_SUCCESS == result )
			{
				pinfo->status = _STATUS_READYGAME;
				// 换线成功更改gsid
				pinfo->gameid = player->chgs_dstlineid;
				if(GProviderServer::GetInstance()->IsNormalLine(player->gameid))
				{
					player->old_gs = player->gameid;
				}
				player->gameid = pinfo->gameid;
				player->world_tag = worldtag;
				FactionManager::Instance()->OnLogin(roleid,factionid,factiontitle,familyid,contribution,
						devotion, pinfo->gameid, pinfo->linksid, pinfo->localsid);
				SiegeManager::GetInstance()->OnLogin(roleid, factionid, pinfo->gameid, pinfo->localsid);
				CircleManager::Instance()->OnLogin(roleid,circleid, pinfo->gameid, pinfo->linksid, pinfo->localsid);
			
				//GDeliveryServer* dsm = GDeliveryServer::GetInstance();
				if (dsm->IsCentralDS())
				{
					OpenBanquetManager::GetInstance()->OnRealEnter(roleid, player->chgs_dstlineid, player->chgs_mapid);
				}
				//CrssvrTeamsManager::Instance()->OnLogin(roleid, pinfo->linksid, pinfo->localsid);
				if(SectManager::Instance()->CheckSect(player->sectid, roleid))
				{
						SectManager::Instance()->OnLogin(player->sectid, roleid, pinfo->gameid);
				}
			}
			else
			{
				DEBUG_PRINT( "换线登录gs失败由于是退出成功后登录失败,玩家已经不在以前的gs中了,需要断掉, 执行完全清理过程,sid=%d\n", roleid );
				dsm->Send(pinfo->linksid, DisconnectPlayer( roleid, -1, pinfo->localsid, -1 ));
				UserContainer::GetInstance().UserLogout(pinfo);
				return;
			}
			DEBUG_PRINT( "worldtag=%d, x=%f, y=%f, z=%f, reason=%d\n", worldtag, x, y, z, chg_re.reason );
			dsm->Send( pinfo->linksid, chg_re );
		}
		else
		{
			if (result == ERR_SUCCESS && (flag == 0 || flag == CENTRALDS_TO_DS || flag== DIRECT_TO_RAID))
				RealLogin(roleid, pinfo, *this);
			if (flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLEMAP || flag == DIRECT_TO_CENTRALDS || /*flag==DS_TO_OPENBANQUET ||*/ flag==/*DIRECT_TO_OPENBANQUET*/DIRECT_TO_CNETRALMAP)
			{
				//remoteloginquery
				GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
				if (dsm->IsCentralDS() && info)
				{
					RemoteLoginQuery query(result, roleid, userid, flag);
					if (CentralDeliveryServer::GetInstance()->DispatchProtocol(info->src_zoneid, query))
					{
						LOG_TRACE("RemoteLoginQuery flag(%d) src_zoneid %d roleid %d userid %d change user->status from %d to _STATUS_REMOTELOGIN_QUERY",
								flag, info->src_zoneid, roleid, userid, pinfo->status);
						pinfo->status = _STATUS_REMOTE_LOGINQUERY;
						//LOG_TRACE("RemoteLoginQuery playerlogin_re_pack worldtag %d", worldtag);
						pinfo->playerlogin_re_pack = Marshal::OctetsStream()<<(*this);
					}
					else
					{
						if (result == ERR_SUCCESS)
							result = -100;
						Log::log(LOG_ERR, "RemoteLoginQuery flag(%d) send to src_zoneid %d roleid %d userid %d failed",
								flag, info->src_zoneid, roleid, userid);
					}
				}
				else
					result = -101;

				if (result != ERR_SUCCESS)
				{
					UserContainer::GetInstance().UserLogout(pinfo);
					return;
				}
			}
			if (result!=ERR_SUCCESS || flag==0 || flag==CENTRALDS_TO_DS || flag==DIRECT_TO_RAID)
			{ 
				/* when send to link server, set src_provider_id to proper game_id */
				this->src_provider_id=pinfo->gameid;

				if (flag != 0 && result == ERR_SUCCESS && flag!= DIRECT_TO_RAID /*&& flag!=DS_TO_OPENBANQUET*/)
				{
					GRoleInfo* info = RoleInfoCache::Instance().Get(roleid);
					if (info)
					{
						GRoleInfo roleinfo_chop = RoleInfoCache::ChopRoleInfo(*info);
						RoleInfo roleinfo;
						RoleInfoCache::ConvertRoleInfo(roleinfo_chop, roleinfo);
						this->roleinfo_pack.swap(Marshal::OctetsStream()<<roleinfo);
					}
					else
						Log::log(LOG_ERR, "PlayerLogin_Re, %d roleinfo not in cache", roleid);
				}
				else if(flag==DIRECT_TO_RAID && result == ERR_SUCCESS)
				{
					int raid_mapid;
					bool raid_enter=RaidManager::GetInstance()->GetMapid(roleid,raid_mapid);
					if(raid_enter && RaidManager::GetInstance()->IsRaidLine(pinfo->gameid))
					{
						RoleInfo roleinfo;
						roleinfo.worldtag = raid_mapid;
						roleinfo.posx = x;
						roleinfo.posy = y;
						roleinfo.posz = z;

						this->roleinfo_pack.swap(Marshal::OctetsStream()<<roleinfo);
						LOG_TRACE("Raid offline reenter roleid=%d, worldtag=%d change to  mapid=%d origin x=%f y=%f z=%f",
								roleid, worldtag, raid_mapid,x,y,z);
					}
				}

				this->lastlogin_ip = pinfo->lastlogin_ip;

				Octets checksum;
				Security *rand = Security::Create(RANDOM);
				rand->Update(checksum.resize(32));
				rand->Destroy();
				pinfo->checksum = checksum;

				this->checksum = pinfo->checksum;

				dsm->Send(pinfo->linksid,this);
				dsm->BroadcastStatus();
			}
		}
	}
};

};

#endif
