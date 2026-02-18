#include "playerlogin.hpp"

#include "gdeliveryserver.hpp"
#include "gproviderserver.hpp"
#include "playerlogin_re.hpp"
#include "announceforbidinfo.hpp"
#include "mapforbid.h"
#include "mapuser.h"
#include "gamedbclient.hpp"
#include "dbclearconsumable.hrp"
#include "centraldeliveryclient.hpp"
#include "referencemanager.h"
#include "raidmanager.h"
#include "crossbattlemanager.hpp"
#include "openbanquetmanager.h"

namespace GNET
{
	void PlayerLogin::SendFailResult(GDeliveryServer* dsm,Manager::Session::ID sid,int retcode) {
		PlayerLogin_Re re;
		re.result = retcode;
		re.roleid = roleid;
		re.localsid = localsid;
		re.flag = flag;
		dsm->Send(sid,re);
	}
	void PlayerLogin::SendForbidInfo(GDeliveryServer* dsm,Manager::Session::ID sid,const GRoleForbid& forbid) {
		dsm->Send(sid,AnnounceForbidInfo(roleid,localsid,forbid));
	}
	bool PlayerLogin::PermitLogin(GDeliveryServer* dsm,Manager::Session::ID sid)
	{
		GRoleForbid	forbid;
		if( ForbidRoleLogin::GetInstance().GetForbidRoleLogin( roleid, forbid ) )
		{
			SendForbidInfo(dsm,sid,forbid);
			SendFailResult(dsm,sid,ERR_ROLEFORBID);
			return false;
		}
		return true;
	}
	void PlayerLogin::DoLogin(Manager::Session::ID sid,  int flag_)
	{
		bool blSuccess=false;
		Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
		int userid = UserContainer::Roleid2Userid(roleid);
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if (NULL != pinfo && pinfo->localsid==localsid && pinfo->status==_STATUS_ONLINE)
		{ 	
			GDeliveryServer* dsm=GDeliveryServer::GetInstance();
			bool is_central = dsm->IsCentralDS();
			int gs_id = lineid;
			if (pinfo->rolelist.IsRoleExist(roleid) && gs_id!=_GAMESERVER_ID_INVALID)
			{
				auth = pinfo->privileges;
				loginip = pinfo->ip;
				int old_gs = gs_id;
				bool raid_reenter=RaidManager::GetInstance()->TryReenterRaid(roleid,gs_id,worldtag);
				if(raid_reenter && (flag_ == 0 || flag_ == DIRECT_TO_CENTRALDS))// 0表示直接登录原服
				{
					LOG_TRACE("RaidReenter roleid=%d, gs_id=%d index=%d",roleid, gs_id, worldtag);
					lineid = gs_id;
					usepos = 1;
					x=0;
					y=0;
					z=0;
					if(is_central)
						flag = DIRECT_TO_CNETRALMAP;
					else
						flag = DIRECT_TO_RAID;
				}
				// 检查是否有开启的跨服战场，如果有强制进入指定的战场地图
				const FightingTag *tag = CrossBattleManager::GetInstance()->GetFightingTag(roleid);
				if((flag == DS_TO_CENTRALDS || flag == DS_TO_BATTLEMAP) && tag != NULL && tag->_isactive == true)
				{
					usepos = 1;
					worldtag = tag->_tag;
					territory_role = tag->_team;
				}

				if(flag == DS_TO_BATTLEMAP)
				{
					usepos = 1;
					flag = DS_TO_CENTRALDS;
				}

				int op_gs;
				int op_map;
				char op_group;
				bool openbanquet_reenter = OpenBanquetManager::GetInstance()->ReEnter(roleid, op_gs, op_map, op_group);
				if(openbanquet_reenter && flag_ == DIRECT_TO_CENTRALDS)
				{
					LOG_TRACE("openbanquet Reenter roleid=%d, gs_id=%d map_id=%d, group=%d",roleid, op_gs, op_map, op_group);
					lineid = op_gs;
					gs_id = lineid;
					worldtag = op_map;
					usepos = 1;
					x=0;
					y=0;
					z=0;
					flag = DIRECT_TO_CNETRALMAP;
					//flag = DIRECT_TO_OPENBANQUET;
					territory_role = op_group;

				}
				LOG_TRACE("PlayerLogin::DoLogin roleid=%d raid_reenter=%d, gs_id=%d, worldtag=%d group=%d,usepos=%d", roleid, raid_reenter, lineid, worldtag, territory_role,usepos);
				if (GProviderServer::GetInstance()->DispatchProtocol(gs_id,this))
				{
					DBClearConsumable::Check360User(pinfo);
					pinfo->status=_STATUS_SELECTROLE;
					pinfo->linkid=provider_link_id;
					pinfo->gameid = gs_id;
					ForbiddenUsers::GetInstance().Push(userid,roleid,pinfo->status);
					UserContainer::GetInstance().RoleLogin(pinfo, roleid);
					blSuccess=true;
					if(raid_reenter && (flag_ == 0 || flag_ == DIRECT_TO_CENTRALDS))
					{
						PlayerInfo* role = UserContainer::GetInstance().FindRole(roleid);
						if(role)
						{
							role->old_gs = old_gs;//set old_gs to player selected gs
							RaidManager::GetInstance()->SetReenterRole(roleid,*role);
						}
					}
				}
			}
		}
		if (!blSuccess)
			SendFailResult( GDeliveryServer::GetInstance(),sid,ERR_LOGINFAIL);
	}
	int PlayerLogin::TryRemoteLogin()
	{
		//与PlayerChangeDS_Re保持一致
		GRoleInfo * roleinfo = RoleInfoCache::Instance().Get(roleid);	
		if (roleinfo == NULL)
			return -1;
		int userid = UserContainer::Roleid2Userid(roleid);
		UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
		if (pinfo==NULL || pinfo->localsid!=localsid || pinfo->status!=_STATUS_ONLINE)
			return -2;
		CentralDeliveryClient * cdc = CentralDeliveryClient::GetInstance();
		if (!cdc->IsConnect())
			return ERR_CDS_COMMUNICATION;
		int ret = cdc->SelectLine(lineid);
		if (ret != ERR_SUCCESS)
			return ret;
		pinfo->chgds_lineid = lineid;
		int version = roleinfo->data_timestamp;
		RoleInfoCache::Instance().Remove(roleid);
		RemoteLoggingUsers::GetInstance().Push(userid, roleid, pinfo->status);
		UserContainer::GetInstance().RoleLogin(pinfo, roleid);//会将pinfo置为_STATUS_READYGAME状态
		pinfo->status = _STATUS_REMOTE_HALFLOGIN;//放在RoleLogin之后
		{
			ReferenceManager::GetInstance()->OnLogin(roleid);
		}
		Octets random_key;
		Security *rand = Security::Create(RANDOM);
		rand->Update(random_key.resize(32));
		rand->Destroy();
		pinfo->rand_key = random_key;
		int src_zoneid = GDeliveryServer::GetInstance()->zoneid;
		UserPlayerData data;
		data.base.create_time = roleinfo->create_time;
		SendDataAndIdentity pro(roleid, userid, src_zoneid, pinfo->ip, pinfo->iseckey, pinfo->oseckey, pinfo->account, pinfo->rand_key, DIRECT_TO_CENTRALDS, data, version, pinfo->logintime, (char)(pinfo->gmstatus&GMSTATE_ACTIVE!=0), pinfo->rewardtype, pinfo->rewarddata, pinfo->privileges);
		ForbidUserTalk::GetInstance().GetForbidUserTalk(userid, pro.forbid_talk);
		LOG_TRACE("SendDataAndIdentity(no data) to Central DS, roleid %d userid %d ip %d isec.size %d osec.size %d account.size %d random_key.size %d version %d logintime %d forbid_time %d create_time %d",
				roleid, userid, pinfo->ip, pinfo->iseckey.size(), pinfo->oseckey.size(), pinfo->account.size(), pinfo->rand_key.size(), version, pinfo->logintime, pro.forbid_talk.time, data.base.create_time);
		cdc->SendProtocol(pro);
		return ERR_SUCCESS;
	}
}
