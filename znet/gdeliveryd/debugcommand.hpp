
#ifndef __GNET_DEBUGCOMMAND_HPP
#define __GNET_DEBUGCOMMAND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "factionmanager.h"
#include "battlemanager.h"
#include "precreatefamily.hrp"
#include "clearstorehousepasswd.hrp"
#include "hostilefaction"
#include "hostileinfo"
#include "dbhostileadd.hrp"
#include "dbhostiledelete.hrp"
#include "dbhostileupdate.hrp"
#include "siegemanager.h"
#include "circlemanager.h"
#include "contestmanager.h"
#include "snssetplayerinfo.hpp"
#include "sendsnspressmessage.hpp"
#include "snsplayerinfomanager.h"
#include "localmacro.h"
#include "hometownmanager.h"
#include "farmcompomanager.h"
#include "hometownitembody.h"
#include "gconsignstart.hpp"
#include "consignmanager.h"
#include "timertask.h"
#include "mapgameattr.h"
#include "querygameserverattr_re.hpp"
#include "protocolexecutor.h"
#include "vipinfocache.hpp"
#include "raidmanager.h"
#include "testcasegen.h"
#include "openbanquetmanager.h"
#include "gcrssvrteamsgetscore.hpp"
#include "ghideseekraidapply.hpp"
#include "crssvrteamsrename.hpp"
#include "gfengshenraidapply.hpp"
//#include "fakeaddcash.hpp"

namespace GNET
{

class DebugCommand : public GNET::Protocol
{
	#include "debugcommand"

	void AnnounceLink()
	{
		LinkServer::GetInstance().BroadcastProtocol(
				AnnounceServerAttribute( GDeliveryServer::GetInstance()->serverAttr.GetAttr() )
			);
	}

	void Display2Client(std::string str, PlayerInfo * player)
	{
		ChatMultiCast csc;
		Octets os_gbk(str.c_str(), str.size());
		CharsetConverter::conv_charset_t2u(os_gbk, csc.msg);
		csc.channel = GP_CHAT_LOCAL;
		csc.srcroleid = player->roleid;
		csc.playerlist.push_back(Player(player->roleid, player->localsid));
		GDeliveryServer::GetInstance()->Send(player->linksid, csc);
	}
	void UpdateServerAttr( unsigned char attribute,const Octets& value)
	{
		try
		{
			GDeliveryServer* dsm=GDeliveryServer::GetInstance();
			Marshal::OctetsStream os(value);
			unsigned char attr = 0;
			os >> attr;
			switch ( attribute )
			{
				case Privilege::PRV_MULTIPLE_EXP:
				{
					dsm->serverAttr.SetMultipleExp(attr);
					break;
				}
				case Privilege::PRV_DOUBLEMONEY:
				{
					dsm->serverAttr.SetDoubleMoney(attr);
					break;
				}
				case Privilege::PRV_DOUBLEOBJECT:
				{
					dsm->serverAttr.SetDoubleObject(attr);
					break;
				}
				case Privilege::PRV_DOUBLESP:
				{
					dsm->serverAttr.SetDoubleSP(attr);
					break;
				}
				case Privilege::PRV_LAMBDA:
				{
					dsm->serverAttr.SetLambda(attr);
					break;
				}
			}//end of switch	
			AnnounceLink();
		}
		catch ( Marshal::Exception e )
		{ }
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LOG_TRACE("DebugCommand: roleid=%d tag=%d size=%d", roleid, tag, data.size());
		Marshal::OctetsStream os;

		char def_buf[256];
		memset(def_buf, 0, sizeof(def_buf));
		strncpy(def_buf, (char*)data.begin(), std::min(sizeof(def_buf)-1, data.size()));

		switch(tag)
		{
			case 1:
			{
				char buf[256];
				int fid, rid, con, devotion;
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				sscanf(buf, "%d-%d-%d-%d", &fid, &rid, &con, &devotion);
				printf( "%d-%d-%d-%d\n", fid, rid, con, devotion );
				FamilyManager::Instance()->Sync2Game(fid, rid, con, devotion);
			}
			break;
			case 2:
			{
				// 模拟跳线debug命令 真正实现功能时加协议
				// 命令：	d_delcmd 2 线号-地图号-x坐标-z坐标
				//		d_delcmd 2 101-21-100-100
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				int lineid, world_tag, x, z;
				sscanf(buf, "%d-%d-%d-%d", &lineid, &world_tag, &x, &z );
				DEBUG_PRINT( "lineid=%d,world_tag=%d,x=%d,z=%d\n", lineid, world_tag, x, z ); 
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if( NULL == pinfo )
				{
					DEBUG_PRINT( "pinfo is NULL\n" );
					return;
				}
				DEBUG_PRINT( "send gmsv playerchangegs\n" );
				PlayerChangeGS chgs;
				chgs.roleid = roleid;
				chgs.localsid = pinfo->localsid; 
				chgs.gslineid = lineid;
				chgs.mapid = world_tag;
				chgs.reason = 0;
				chgs.scalex = x;
				chgs.scaley = 0;
				chgs.scalez = z;
				GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, chgs );
			}
			break;
			/*
			case 4:
			{
				// 开启城战debug命令
				// 命令：	d_delcmd 3 线号-地图号-攻击帮-防守帮-时间（秒）
				//		d_delcmd 3 101-21-46-47-600
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				int lineid, world_tag, attacker, defender, lasttime;
				sscanf(buf, "%d-%d-%d-%d-%d", &lineid, &world_tag, &attacker, &defender, &lasttime );
				DEBUG_PRINT( "lineid=%d,world_tag=%d,attacker=%d,defender=%d,lasttime=%d\n", 
						lineid, world_tag, attacker, defender, lasttime );
				int k = time( NULL );
				BattleStart a( world_tag, 0, attacker, defender, k + lasttime );
				GProviderServer::GetInstance()->DispatchProtocol( lineid, a );
				
			}
			break;
			*/
			case 3:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());	
				buf[data.size()] = 0;
				int cmdid;
				int gs_id=0;
				int map_id=0;
				int team=0;
				sscanf(buf, "%d-%d-%d-%d", &cmdid, &gs_id, &map_id, &team);
				printf("cmdid=%d gs_id=%d, map_id=%d, team=%d", cmdid, gs_id, map_id, (unsigned char)team);	
				
				switch(cmdid)
				{
				case 0:
					BattleManager::GetInstance()->DebugList(gs_id);
					break;
				case 1:
					BattleManager::GetInstance()->DebugStartBattle(gs_id, map_id);
					break;
				case 2:
					BattleManager::GetInstance()->DebugBattleEnter(gs_id, map_id, roleid);
					break;
				case 3:
					BattleManager::GetInstance()->DebugBattleJoin(gs_id, map_id, roleid, team);
					break;
				case 4:
					BattleManager::GetInstance()->DebugBattleLeave(gs_id, roleid);
					break;
				case 5:
				break;
				}	
			}
			break;
			case 6:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());	
				buf[data.size()] = 0;
				int cmdid;
				int factionid=0;
				int hostileid=0;
				sscanf(buf, "%d-%d-%d", &cmdid, &factionid, &hostileid);
				printf("cmdid=%d factionid=%d, hostileid=%d\n", cmdid, factionid, hostileid);	
	
				switch(cmdid)
				{
				case 0:
					{
					HostileInfo info;
					bool ret = FactionManager::Instance()->GetHostileInfo(factionid, info);

					printf("ret=%d,actionpoint=%d,protecttime=%s,status=%d,updatetime=%s",ret, info.actionpoint
						,ctime((time_t*)&info.protecttime), info.status, ctime((time_t*)&info.updatetime));
					for (std::vector<HostileFaction>::iterator it = info.hostiles.begin()
							; it != info.hostiles.end(); ++it)
					{
						printf("\thostilefactionid=%d,addtime=%s",it->fid,ctime((time_t*)&it->addtime));	
					}
					}
					break;
				case 1:
					{
						DBHostileAddArg arg(factionid, hostileid);

						DBHostileAdd* rpc = (DBHostileAdd*) Rpc::Call( RPC_DBHOSTILEADD, arg);
						rpc->linksid = 0;
						rpc->localsid = 0;
						GameDBClient::GetInstance()->SendProtocol(rpc);
					}
					break;
				case 2:
					{
						DBHostileDeleteArg arg(factionid);
						arg.hostile.push_back(hostileid);
						DBHostileDelete* rpc = (DBHostileDelete*) Rpc::Call( RPC_DBHOSTILEDELETE, arg);
						rpc->sendclient = false;
						GameDBClient::GetInstance()->SendProtocol(rpc);
					}
					break;
				}
			}
			break;
			case 5:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				int fid;
				int contribution;
				sscanf(buf, "%d-%d", &fid, &contribution);
				printf("fid=%d, contribution=%d\n", fid, contribution);
				FactionDetailInfo * faction = FactionManager::Instance()->Find(fid);
				if (!faction)	
				{
					printf("faction(%d) not found\n", fid);
					return;
				}
				faction->info.contribution += contribution;
			}
			break;
			case 7:
			{
				ClearStorehousePasswdArg arg;
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;

				arg.roleid = atoi(buf);
				ClearStorehousePasswd* rpc=(ClearStorehousePasswd*) Rpc::Call( RPC_CLEARSTOREHOUSEPASSWD,arg);
				GameDBClient::GetInstance()->SendProtocol( rpc );
			}
			break;

			//答题调试命令 
			case 11: //Contest start 
			{
				char buf[256];
				int questionindex;
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				sscanf(buf, "%d", &questionindex);
				
				printf( "ContestDebugCommand: contest start roleid is %d, index is %d\n", roleid, questionindex);
				ContestManager::GetInstance().DebugBegin(roleid, questionindex);
			}
			break;
			case 12: //Contest end 
			{
				printf( "ContestDebugCommand: contest end roleid is %d\n", roleid);
				ContestManager::GetInstance().DebugEnd(roleid);
			}
			break;

			// 战场使用调试命令
			case 101:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());	
				buf[data.size()] = 0;
				int gs_id;
				int battle_world_tag;
				int battle_id;
				int last_time;
				sscanf(buf, "%d-%d-%d-%d", &gs_id, &battle_world_tag, &battle_id, &last_time );
				printf( "gs_id=%d, battle_world_tag=%d, battle_id=%d, last_time=%d\n",
					gs_id, battle_world_tag, battle_id, last_time );
				BattleStart proto( gs_id, battle_world_tag, battle_id, last_time + time(NULL) );
				GProviderServer::GetInstance()->DispatchProtocol( gs_id, proto );
			}
			break;

			case 102:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());	
				buf[data.size()] = 0;
				int gs_id;
				int battle_world_tag;
				int battle_id;
				int battle_faction;
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if( NULL == pinfo )
				{
					DEBUG_PRINT( "pinfo is NULL\n" );
					return;
				}
				sscanf( buf, "%d-%d-%d-%d", &gs_id, &battle_world_tag, &battle_id, &battle_faction );
				printf( "gs_id=%d, battle_world_tag=%d, battle_id=%d, battle_faction=%d\n",
					gs_id, battle_world_tag, battle_id, battle_faction );
				SendBattleEnter proto( roleid, battle_world_tag, battle_id, battle_faction );
				GProviderServer::GetInstance()->DispatchProtocol( gs_id, proto );
			}
			break;

			// 敌对帮派调试命令
			case 103:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());	
				buf[data.size()] = 0;
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if( NULL == pinfo )
				{
					DEBUG_PRINT( "pinfo is NULL\n" );
					return;
				}
				HostileAdd_Re proto;
				int gs_id;
				sscanf( buf, "%d-%d-%d", &gs_id, &proto.factionid, &proto.hostile );
				printf( "gs_id=%d, factionid=%d, hostile=%d\n", gs_id, proto.factionid, proto.hostile );
				proto.retcode = 0;
				proto.localsid = 0;
				GProviderServer::GetInstance()->DispatchProtocol( gs_id, proto );
				unsigned temp = proto.factionid;
				proto.factionid = proto.hostile;
				proto.hostile = temp;
				GProviderServer::GetInstance()->DispatchProtocol( gs_id, proto );
			}
			break;

			case 104:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());	
				buf[data.size()] = 0;
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if( NULL == pinfo )
				{
					DEBUG_PRINT( "pinfo is NULL\n" );
					return;
				}
				HostileDelete_Re proto;
				int gs_id;
				sscanf( buf, "%d-%d-%d", &gs_id, &proto.factionid, &proto.hostile );
				printf( "gs_id=%d, factionid=%d, hostile=%d\n", gs_id, proto.factionid, proto.hostile );
				proto.retcode = 0;
				proto.localsid = 0;
				GProviderServer::GetInstance()->DispatchProtocol( gs_id, proto );
				unsigned temp = proto.factionid;
				proto.factionid = proto.hostile;
				proto.hostile = temp;
				GProviderServer::GetInstance()->DispatchProtocol( gs_id, proto );
			}
			break;

			case 105:
			{
				SiegeManager::GetInstance()->DebugStart();
			}
			break;
			case 110:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				TerritoryManager::GetInstance()->SetForgedTime(atoi(buf));
				LOG_TRACE("DebugCommand: roleid=%d territorymanager setforgedtime=%d", roleid, atoi(buf));
			}
			break;
			case 111:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				int territoryid;
				sscanf(buf, "%d", &territoryid);
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if(NULL == pinfo)
					return;
				TerritoryManager::GetInstance()->DebugSetOwner(territoryid, pinfo->factionid);
				LOG_TRACE("DebugCommand: territorymanager set territory %d owner %d", territoryid, pinfo->factionid);
			}
			break;
			case 112:
			{
				char buf[256];
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				int territoryid;
				sscanf(buf, "%d", &territoryid);
				TerritoryManager::GetInstance()->DebugSetOwner(territoryid, 0);
				LOG_TRACE("DebugCommand: territorymanager set territory %d owner 0");
			}
			break;
			case 201:
			{
				//战场开启命令
				//命令：	d_delcmd 201 线号-城战id-地图号-攻-守-攻击辅助方,持续时间
				//		d_delcmd 201 432-1-201-111-222-333-3600
				char buf[256];
				strncpy(buf,(char*)data.begin(),data.size());
				buf[data.size()] = 0;
				int gs_id,battle_id,world_tag,attacker,defender,attacker_assistant,last_time;
				sscanf(buf,"%d-%d-%d-%d-%d-%d-%d",&gs_id,&battle_id,&world_tag,&attacker,&defender,
					&attacker_assistant,&last_time);
				Thread::RWLock::WRScoped l(UserContainer::GetInstance().GetLocker());
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if(NULL == pinfo)
				{
					return;
				}
				int k = time(NULL);
				SendSiegeStart proto(battle_id,world_tag,defender,attacker,attacker_assistant,k + last_time);
				GProviderServer::GetInstance()->DispatchProtocol(gs_id,proto);
			}
			break;
			case 202:
			{
				char buf[256];
				strncpy(buf,(char*)data.begin(),data.size());
				buf[data.size()] = 0;
				BattleManager::GetInstance()->SetForgedTime(atoi(buf));
			}
			break;
			case 203:
			{
				BattleManager::GetInstance()->SetDebugMode();
			}
			break;

			//SNS调试相关
			case 300:
			{
				//命令：	d_delcmd 300
				SNSSetPlayerInfo pro;
				pro.roleid = roleid;
				pro.is_private = true;
				pro.nickname = Octets("abc", 3);
				pro.birthday = 10;
				pro.province = 1;
				pro.city = 2;
				pro.career = Octets("student", 7);
				pro.address = Octets("beijing", 7);
				pro.introduce = Octets("goodboy", 8);
				pro.Process(manager, sid);
			}
			break;
			case 301:
			{
				SNSCancelMessage pro;
				pro.roleid = roleid;
				pro.message_id = 0;
				pro.message_type = 0;
				pro.Process(manager, sid);
			}
			break;
			case 302:
			{
				SNSGetMessage pro;
				pro.roleid = roleid;
				pro.message_id = 0;
				pro.message_type = 0;
				pro.Process(manager, sid);
			}
			break;
			case 303:
			{
				SNSAcceptApply pro;
				pro.roleid = roleid;
				pro.apply_roleid = 0;
				pro.message_type = 0;
				pro.message_id = 0;
				pro.oper = 0;
				pro.Process(manager, sid);
			}
			break;
			case 304:
			{
				SNSGetPlayerInfo pro;
				pro.roleid = roleid;
				pro.asker = 0;
				pro.Process(manager, sid);
			}
			break;
			case 305:
			{
				SendSNSPressMessage msg;
				msg.roleid = roleid;
				msg.message = Octets("111222", 6);
				msg.condition = GApplyCondition((unsigned char)-1, -1, 0, 0);
				for (int i = 0; i < 80; i++)
				{
					msg.message_type = i%SNS_MESSAGE_TYPE_MAX;
					msg.Process(manager, sid);
				}
			}
			case 306:
			{
				int charm_type, charm;
				char buf[256] = {0};
				strncpy(buf, (char*)data.begin(), data.size());
				sscanf(buf, "%d-%d", &charm_type, &charm);
				buf[data.size()] = 0;
				SNSPlayerInfoManager::GetInstance()->UpdateSNSPlayerCharm((SNS_CHARM_TYPE)charm_type, roleid, charm);
			}
			break;
			case 307:
			{
				int add_type, add_amount;
				sscanf((char *)data.begin(), "%d-%d", &add_type, &add_amount);
				if (add_type>=0 && add_type<=3)
				{
					FarmManager *fm = FarmCompoManager::GetInstance()->GetFarm(roleid);
					if (fm != NULL)
					{
						if (add_type == 0)			//加农场组件币
							fm->_owner._money += add_amount;
						else if (add_type == 1)		//加农场组件魅力
							fm->_owner._charm += add_amount;
						else if (add_type == 2)		//加道具
						{
							HometownItemData idata;
							const HometownItemBody *body = HometownItemBodyMan::GetInstance()->GetBody(add_amount);
							if (body != NULL)
							{
								body->FillItemData(idata);
								idata._count = 1;
								fm->_owner._pocket.AddItem(idata);
							}
						}
						else if (add_type == 3)		//加果实
						{
							HometownItemData idata;
							const HometownItemBody *body = HometownItemBodyMan::GetInstance()->GetBody(add_amount);
							if (body != NULL)
							{
								body->FillItemData(idata);
								idata._count = 1;
								fm->_owner._pocket.AddItem(idata);
							}
						}
						fm->SetDirty(true);
					}
				}
				else
				{
					HometownPlayerRef player;
					if (HometownManager::GetInstance()->FindPlayerOnline(roleid, player))
					{
						player->_money += add_amount;
						player->SetDirty(true);
					}
				}
			}
			break;
			case 400:
			{
				int flag;
				int forge_time = 0;
				char buf[256] = {0};
				strncpy(buf, (char*)data.begin(), data.size());
				buf[data.size()] = 0;
				sscanf(buf, "%d", &flag);
				if (flag == 1)
					forge_time = UniqueAuctionManager::BEGIN_BID_TIME+UniqueAuctionManager::BEGIN_LAG_TIME-30;
				else if (flag == 2)
					forge_time = UniqueAuctionManager::END_BID_TIME+30;
				else
					forge_time = flag;
				UniqueAuctionManager::GetInstance()->SetForgedTime(forge_time);
				LOG_TRACE("DebugCommand: roleid=%d UniqueAuctionManager setforgedtime=%d", roleid, forge_time);
			}
			break;
			case 410:
			{
				int mon=0, day=0;
				char buf[256] = {0};
				strncpy(buf, (char*)data.begin(), data.size());
				sscanf(buf, "%d-%d", &mon, &day);
				UniqueAuctionManager::GetInstance()->DebugGenerateItem(mon-1, day);
				LOG_TRACE("DebugCommand: roleid=%d UniqueAuctionManager Generate WinItem (%d, %d)", roleid, mon, day);
			}
			break;
			case 401:
			{
				unsigned int add_amount;
				sscanf((char *)data.begin(), "%d", &add_amount);
				if (add_amount > 500)
					add_amount = 500;
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if(pinfo)
				{
					int circleid = pinfo->GetCircleID();
					if(circleid!=0)
						CircleManager::Instance()->AddPoint(circleid, add_amount);
				}
				else
					LOG_TRACE("DebugCommand: roleid=%d Role not in cache ", roleid);
			}
			break;
			case 402:
			{
				unsigned int money_amount;
				sscanf((char *)data.begin(), "%d", &money_amount);
				GConsignStart start(
						roleid,
						10,
						0,
						0,
						0,
						money_amount,
						10,
						1,
						Octets(),
						0
						);

				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if(pinfo)
				{
					UserInfo * userinfo = UserContainer::GetInstance().FindUser(pinfo->userid);
					if(userinfo)
						ConsignManager::GetInstance()->TryPrePost(start,userinfo->ip,*pinfo);
				}
				else
					LOG_TRACE("DebugCommand: roleid=%d Role not in cache ", roleid);
			}
			break;
			case 420:
			{
				unsigned int disconnect_au;
				sscanf((char *)data.begin(), "%d", &disconnect_au);
				LOG_TRACE("DebugCommand: roleid=%d Change Conn_state of AU %d", roleid, disconnect_au);
				GAuthClient *au_client = GAuthClient::GetInstance();
				if(disconnect_au)
				{
			//		au_client->Need2Reconnect(false);
					au_client->Close(au_client->GetActiveSid());
					Conf::GetInstance()->put(au_client->Identification(), "port", "29201");
				}
				else
				{
					//au_client->Need2Reconnect(true);
					//Thread::HouseKeeper::AddTimerTask(new ReconnectTask(au_client, 1), 1);
					Conf::GetInstance()->put(au_client->Identification(), "port", "29200");
				}
			}
			break;
			case 421:
			{
				unsigned int attr = 0, value = 0;
				sscanf((char *)data.begin(), "%d-%d", &attr, &value);
				unsigned char cvalue = value;
				LOG_TRACE("DebugCommand: roleid=%d SetGameAttr %d-%d", roleid, attr, value);
				Octets oct;
				Marshal::OctetsStream stream;
				stream << cvalue;
				oct = stream;
				if(!GameAttrMap::IsValid(attr, oct))
				{
					LOG_TRACE("DebugCommand:roleid=%d SetGameAttr is invalid", roleid);
				}
				else
				{
					if(GameAttrMap::Put(attr, oct))
					{
						QueryGameServerAttr_Re qgsa_re;
						qgsa_re.attr.push_back(GameAttr(attr, oct));
						GProviderServer::GetInstance()->BroadcastProtocol(qgsa_re);
						UpdateServerAttr(attr, oct);
					}
				}
			}
			break;
			case 422:
			{
				unsigned int mail_num = 0;
				sscanf((char *)data.begin(), "%d", &mail_num);
				LOG_TRACE("DebugCommand: roleid=%d Send %d SysMails", roleid, mail_num);
				std::string test;
				test = "test";
				DBSendMailArg arg;
				arg.mail.header.id = 0;
				arg.mail.header.sender = 0;
				arg.mail.header.sndr_type = _MST_WEB;
				arg.mail.header.receiver = roleid;
				arg.mail.header.send_time = Timer::GetTime();
				arg.mail.header.attribute = (1 << _MA_UNREAD);

				Octets gbk_test(test.c_str(), test.length() + 1);
				CharsetConverter::conv_charset_g2u(gbk_test, arg.mail.header.sender_name);
				CharsetConverter::conv_charset_g2u(gbk_test, arg.mail.header.title);
				CharsetConverter::conv_charset_g2u(gbk_test, arg.mail.context);

				for(unsigned int i = 0; i < mail_num; i++)
				{
					DBSendMail *rpc = (DBSendMail *)Rpc::Call(RPC_DBSENDMAIL, arg);
					rpc->save_linksid = 0;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
			break;
		/*	case 423:
			{
				unsigned int add_amount=0,userid=0;
				sscanf((char *)data.begin(), "%d-%d", &userid,&add_amount);
				if (add_amount < 0)
					add_amount = 0;
				LOG_TRACE("DebugCommand FakeAddCash: Userid=%d add_amount=%d", userid,add_amount);
				FakeAddCash msg(userid,0,0,add_amount);
				GameDBClient::GetInstance()->SendProtocol(msg);
			}
			break;*/
			case 424:
			{
				unsigned int line_num=0;
				sscanf((char *)data.begin(), "%d", &line_num);
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if( NULL == pinfo )
				{
					DEBUG_PRINT( "pinfo is NULL\n" );
					return;
				}
				LOG_TRACE("DebugCommand Set Line Player Limit: rolegsid=%d playerlimit=%d",pinfo->gameid,line_num);
				GProviderServer::GetInstance()->SetLineLimit(pinfo->gameid,line_num);
			}
			break;
			case 425:
			{
				unsigned int mapid,gsid,roomid;
				int count;
				sscanf((char *)data.begin(), "%d-%d-%d-%d",&gsid,&mapid,&roomid,&count);
				LOG_TRACE("DebugCommand Raid debug start gsid=%d mapid=%d",gsid,mapid);
				if(count<1||count>200)
					count=10;
				SendRaidStart st(mapid,roomid);
				for(int i=0;i<count;i++)
					GProviderServer::GetInstance()->DispatchProtocol(gsid,st);
			}
			break;
			case 426:
			{
				unsigned int gsid,index;
				sscanf((char *)data.begin(), "%d-%d",&gsid,&index);
				LOG_TRACE("DebugCommand Raid debug end gsid=%d index=%d",gsid,index);
				GProviderServer::GetInstance()->DispatchProtocol(gsid,SendRaidClose(1,0,index,0));	
			}
			break;
			case 427:
			{
				//sscanf((char *)data.begin(), "%d-%d",&gsid,&index);
				LOG_TRACE("DebugCommand Raid debug openroom roleid=%d",roleid);
		//		GProviderServer::GetInstance()->DispatchProtocol(gsid,PlayerChangeGS(roleid,it->localsid,gs_id,index,0,0,0,0));
				PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if( NULL == pinfo )
				{
					DEBUG_PRINT( "pinfo is NULL\n" );
					return;
				}
				GMailSyncData md;
				OctetsStream ot;
		/*		short n1 = 0x7200;
				short n2 = 0x6f00;
				short n3 = 0x6200;
				short n4 = 0x6f00;
				short n5 = 0x7400;
				ot<<n1<<n2<<n3<<n4<<n5;
				short n1 = 0x6100;
				ot<<n1;*/
				static int times=0;
				times++;
//	                        Octets src_name_ucs2;
			        char username[11]="raid";
        			int n = strlen(username);
 			       	snprintf(username+n,11-n,"%d",times);
				for(unsigned int i=0;i<strlen(username);i++)
				{
					unsigned short sh = (username[i]<<8);
					ot<<sh;
				}
//				CharsetConverter::conv_charset_g2u(Octets(username,strlen(username)),src_name_ucs2);		
				RaidManager::GetInstance()->OpenRaid(static_cast<int>(roleid), pinfo->gameid,530,0,0,md,ot,1);    
			}
			break;
			case 428:
			{
				int raidroom_id;
				sscanf((char *)data.begin(), "%d",&raidroom_id);
				LOG_TRACE("DebugCommand Raid debug joinroom roleid=%d roomid=%d",roleid,raidroom_id);
				RaidManager::GetInstance()->OnGRaidJoin(roleid,530,raidroom_id);    
			}
			break;
			case 429:
			{
				int userid = 0, currenttime = 0, starttime = 0, endtime = 0, viplevel = 0;
				sscanf((char *)data.begin(), "%d-%d-%d-%d-%d",&userid, &currenttime, &starttime, &endtime, &viplevel);
				ProtocolExecutor *task = new ProtocolExecutor(manager, sid, new VIPAnnounce(userid, 0, 0, currenttime, starttime, endtime, 0, viplevel));
				Thread::Pool::AddTask(task);
			}
			break;
			case 431:
			{
				int isopen = 0;
				sscanf((char*)data.begin(), "%d", &isopen);
				VIPInfoCache::GetInstance()->SetIsOpen(isopen);
			}
			break;
			case 432:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if(pinfo)
				{
					int currenttime = Timer::GetTime(), viplevel = 0, viptime = 1800;
					sscanf((char *)data.begin(), "%d-%d", &viplevel, &viptime);
					ProtocolExecutor *task = new ProtocolExecutor(manager, sid, new VIPAnnounce(pinfo->userid, 0, 0, currenttime, currenttime, currenttime+viptime, 0, viplevel));
					Thread::Pool::AddTask(task);
				}
			}
			break;

			case 433:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo && pinfo->user)
					GAuthClient::GetInstance()->SendProtocol(SSOGetTicketReq(pinfo->user->ssoinfo, pinfo->user->ip, 101, -1)); 

			}
			break;

			case 434:
			{
				int year_offset;
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if(pinfo)
				{
					sscanf((char *)data.begin(), "%d",&year_offset);
					LOG_TRACE("DebugCommand modify role create time, roleid=%d dec year=%d",roleid,year_offset);
					if(year_offset>5 || year_offset<-5)
						return;
					ModifyRoleCreateTime* rpc = (ModifyRoleCreateTime*) Rpc::Call( RPC_MODIFYROLECREATETIME, ModifyRoleCreateTimeArg(roleid,year_offset));
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}	
			}
			break;

			case 435:
			{
				int algo = 0;
				sscanf((char *)data.begin(), "%d", &algo);
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo && pinfo->user)
					Passwd::GetInstance().CacheAlgo(pinfo->userid, algo);
			}
			break;

			case 436:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo)
				{
					DBQueryRoleName * rpc = (DBQueryRoleName *) Rpc::Call(RPC_DBQUERYROLENAME, DBQueryRoleNameArg(pinfo->name));
					rpc->linksid = 0;
					rpc->localsid = 0;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
			break;
			case 437:
			{
				unsigned int disconnect_db;
				sscanf((char *)data.begin(), "%d", &disconnect_db);
				LOG_TRACE("DebugCommand: roleid=%d Change Conn_state of DB %d", roleid, disconnect_db);
				GameDBClient *db_client = GameDBClient::GetInstance();
				if(disconnect_db)
				{
					db_client->Need2Reconnect(false);
					db_client->Close(db_client->GetActiveSid());
				}
				else if (!db_client->IsConnect())
				{
					db_client->Need2Reconnect(true);
					Thread::HouseKeeper::AddTimerTask(new ReconnectTask(db_client, 1), 1);
				}
			}
			break;
			case 438:
			{
				int number = 0;
				sscanf((char *)data.begin(), "%d", &number);
				RaidManager::GetInstance()->SetTeamRaidLimit(number);
			}
			break;
			case 439:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				GDeliveryServer *server = GDeliveryServer::GetInstance();
				if(pinfo)
				{
					SSOGetTicketReq *req = new SSOGetTicketReq;
					req->user.userid = pinfo->userid;
					req->toaid = server->aid;
					req->tozoneid = server->zoneid;
					ProtocolExecutor *task = new ProtocolExecutor(manager, sid, req);
					Thread::Pool::AddTask(task);
				}
			}
			break;
			case 440:
			{
				int scale = 0;
				sscanf((char *)data.begin(), "%d", &scale);
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo == NULL)
					return;
				if (scale == 0)
					FactionManager::Instance()->DebugClearCD(pinfo->factionid);
				else
					FamilyManager::Instance()->DebugClearCD(pinfo->familyid);
			}
			break;
			case 441:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo == NULL)
					return;
				KingdomManager::GetInstance()->DebugClearAttacker();
			}
			break;
			case 442:
			{
				int fid = 0;
				sscanf(def_buf, "%d", &fid);
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo == NULL)
					return;
				if (fid == 0)
					fid = pinfo->factionid;
				if (fid)
					KingdomManager::GetInstance()->DebugAddAttacker(fid);
			}
			break;
			case 443:
			{
				KingdomManager::GetInstance()->DebugStartBattle();
			}
			break;
			case 451:
			{
				int attacker_count = 0;
				sscanf(def_buf, "%d", &attacker_count);
				KingdomManager::GetInstance()->DebugClearAttacker();
				if (attacker_count > 0)
					KingdomBattle::ATTACKER_COUNT = attacker_count;
				LOG_TRACE("debugset attackersize %d", attacker_count);
			}
			break;
			case 452:
			{
				int prepare_time = 0;
				sscanf(def_buf, "%d", &prepare_time);
				if (prepare_time > 0)
					KingdomBattle::PREPARE_TIME = prepare_time;
				LOG_TRACE("debugset preparetime %d", prepare_time);
			}
			break;
			case 453:
			{
				int result = 0;
				int winner = 0;
				sscanf(def_buf, "%d-%d", &result, &winner);
				if (winner != 0)
				{
					PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
					winner = pinfo->factionid;
				}	
				KingdomManager::GetInstance()->DebugSetWinner(result, winner);
			}
			break;
			case 454:
			{
				int points_delta = 0;
				sscanf(def_buf, "%d", &points_delta);
				KingdomManager::GetInstance()->OnPointChange(points_delta);
			}
			break;
			case 455:
			{
				KingdomManager::GetInstance()->DebugClearLimits();
			}
			case 456:
			{
				KingdomManager::GetInstance()->DebugCloseTask();
			}
			break;
			case 457:
			{
				KingdomManager::GetInstance()->DebugEndTask();
			}
			break;
			case 458:
			{
				int openorclose = 0;
				sscanf(def_buf, "%d", &openorclose);
				if (openorclose)
					GProviderServer::GetInstance()->BroadcastProtocol(QueenOpenBath(0));
				else
					GProviderServer::GetInstance()->BroadcastProtocol(QueenCloseBath(0));
			}
			break;
			case 444:
			{
				int flag = 0;
				sscanf((char *)data.begin(), "%d", &flag);
				RaidManager::GetInstance()->SetDebugMode(!(bool)flag);
			}
			break;
			case 445:
			{
				RaidManager::GetInstance()->ClearTeamApplyCD(roleid);
			}
			break;
			case 446:
			{
				//人为制造非法协议发 给 gs
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (pinfo == NULL)
					return;
				GProviderServer::GetInstance()->DispatchProtocol( pinfo->gameid, CertAnswer());
			}
			break;
			case 447:
			{
				if (GDeliveryServer::GetInstance()->IsCentralDS())
					CentralDeliveryServer::GetInstance()->CloseAll();
				else
					CentralDeliveryClient::GetInstance()->CloseAll();
			}
			break;
			case 448:
			{
				int forbid_role, forbid_time, ac_score;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d-%d", &forbid_role, &forbid_time, &ac_score);
				ProtocolExecutor *task = new ProtocolExecutor(GAntiCheatClient::GetInstance(), 0, new ACForbidUser(0, forbid_role, 0, forbid_time, ac_score, Octets(), Octets()));
				Thread::Pool::AddTask(task);
			}
			break;
			case 449:
			{
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(sizeof(buf)-1, data.size()));
				Marshal::OctetsStream info;
				info << (int)atoi(buf) << (int)0 << Octets();
				ProtocolExecutor *task = new ProtocolExecutor(GAuthClient::GetInstance(), 0, new AU2Game(0, AU_REQ_FLOWER_TOP, info, 0, 0));
				Thread::Pool::AddTask(task);
			}
			break;
			case 460:
			{
				int flevel = 0;
				int fid = 0;
				sscanf(def_buf, "%d", &flevel);
				if (flevel > 0)
				{
					PlayerInfo* pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
					if (pinfo)
						fid = pinfo->factionid;
				}
				if (fid)
				{
					DBFactionUpdateArg arg;
					arg.fid = fid;
					arg.level = flevel-1;
					arg.prosperity = 43927000;
					arg.reason = 1;
					DBFactionUpdate* rpc = (DBFactionUpdate*) Rpc::Call( RPC_DBFACTIONUPDATE,arg);
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
			break;
			case 461:
			{
				int cmdtype = 0;
				char arg_str[256];
				memset(arg_str, 0, sizeof(arg_str));
				std::vector<int> cmdandargs;
				sscanf(def_buf, "%d-%s", &cmdtype, arg_str);
				cmdandargs.push_back(cmdtype);
				char * delim = "-";
				char * token = strtok(arg_str, delim);
				while(NULL != token)
				{
					cmdandargs.push_back(atoi(token));
					token = strtok(NULL, delim);
				}
				ServiceForbidCmd cmd;
				if (IwebCmd::MakeCmd(cmdandargs, cmd))
				{
					std::vector<ServiceForbidCmd> cmdlist;
					cmdlist.push_back(cmd);
					DebugOperationCmd * rpc = (DebugOperationCmd *) Rpc::Call(RPC_DEBUGOPERATIONCMD, DebugOperationCmdArg(0, cmdlist));
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
			}
			break;
			case 462:
			{
				DebugOperationCmd * rpc = (DebugOperationCmd *) Rpc::Call(RPC_DEBUGOPERATIONCMD, DebugOperationCmdArg(1));
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
			break;
			case 463:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo)
					Display2Client(IwebCmd::GetInstance()->GetCmdStr(), pinfo);
			}
			break;
			case 464:
			{
				int cash_userid = 0;
				sscanf(def_buf, "%d", &cash_userid);
				int cash_roleid = 0;
				if (cash_userid == 0)
				{
					PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
					if (pinfo == NULL)
						return;
					cash_userid = pinfo->userid;
				}
				bool ongame = false;
				UserInfo * userinfo = UserContainer::GetInstance().FindUser(cash_userid);
				if (userinfo)
				{
					ongame = (userinfo->gameid > 0);
					cash_roleid = userinfo->roleid;
				}
				GetCashAvail * rpc = (GetCashAvail *)Rpc::Call(RPC_GETCASHAVAIL, GetCashAvailArg(cash_roleid, cash_userid, 0));
				if (ongame)
					GProviderServer::GetInstance()->DispatchProtocol(userinfo->gameid, rpc);
				else
					GameDBClient::GetInstance()->SendProtocol(rpc);
				LOG_TRACE("au2game get cash_available, userid %d ongame %d roleid %d", cash_userid, ongame, cash_roleid);
			}
			break;
			case 465:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo == NULL)
					return;
				int64_t orderid = 0;
				sscanf((char *)data.begin(), "%lld", &orderid);
				WebMallFunction func(538, Octets(), 2, 899, 900);
				WebMallGoods goods(16552, 1, 0x01, 0);
				func.goods.push_back(goods);
				WebMallFunctionVector funcs;
				funcs.push_back(func);
				ProtocolExecutor *task = new ProtocolExecutor(NULL, 0, new SysSendMail4(2, orderid+10000, pinfo->userid, roleid, Octets(), Octets(), Octets(), funcs));
				Thread::Pool::AddTask(task);
			}
			break;
			case 466:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo == NULL)
					return;
				int dynamic_type = 0;
				sscanf((char *)data.begin(), "%d", &dynamic_type);

				switch(dynamic_type)
				{
					case FAC_DYNAMIC::MEMBER_CHANGE:
					{
						FAC_DYNAMIC::member_change log = {FAC_DYNAMIC::member_change::MEMBER_JOIN, 20, {}, 20, {}};
						FactionDynamic::GetName(pinfo->name, log.rolename, log.rolenamesize);
						FactionDynamic::GetName(pinfo->name, log.familyname, log.familynamesize);
						FactionManager::Instance()->RecordDynamic(pinfo->factionid, dynamic_type, log);
					}
					break;
				}
			}
			break;
			case 467:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
				if (NULL == pinfo || pinfo->factionid == 0)
					return;
				int ret = FactionManager::Instance()->CreateBase(pinfo->factionid, roleid);
				LOG_TRACE("faction %d create base, ret %d", pinfo->factionid, ret);
			}
			break;
			case 468:
			{
				int total = 0;
				sscanf(def_buf, "%d", &total);
				FacBaseManager::GetInstance()->DebugSetLimit(total);
			}
			break;
			case 469:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo)
				{
					char buf[128];
					memset(buf, 0, sizeof(buf));
					snprintf(buf, sizeof(buf)-1, "faction base number %d", FacBaseManager::GetInstance()->GetCurNum());
					Display2Client(std::string(buf), pinfo);
				}
			}
			break;
			case 470:
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo && pinfo->factionid)
					FactionManager::Instance()->DebugClearDynamic(pinfo->factionid);
			}
			break;

			case 999://执行测试用例
			{
				int testcaseid = 0;
				int param1 = 0;
				int param2 = 0;
				int param3 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d-%d-%d", &testcaseid, &param1, &param2, &param3); 
				TestCaseBase::RunTest(testcaseid, roleid, param1, param2, param3);
			}
			break;
			
			case 1000://命令开启流水席战场
			{
				//命令：        d_delcmd 1000 1-xxx //设置时间
				//命令：        d_delcmd 1000 2 // 还原时间
				//命令：        d_delcmd 1000 3 // 清除当天完成限制
				int param1 = 0;
				int param2 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 

				time_t now = Timer::GetTime();
				//time_t begin = OpenBanquetManager::GetInstance()->BattleBeginTime();
				LOG_TRACE("DebugCommand: roleid=%d, param1=%d, param2=%d now=%d", roleid, param1, param2, now);
				if(param1 == 1)// 设置新时间
					OpenBanquetManager::GetInstance()->SetForgedTime(param2);	
				else if(param1 == 2)// 重置现在时间
				{
					OpenBanquetManager::GetInstance()->SetForgedTime(0);
				}
				else if(param1 == 3) // 清除当天完成限制
				{
					OpenBanquetManager::GetInstance()->ClearPerdayLimit(roleid);
				}
				else if(param1 == 4) // 设置战场开始时间前1分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 59;
					dt.tm_hour = 18;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-now);
				}
				else if(param1 == 5) // 设置结束前11分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 49;
					dt.tm_hour = 20;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-now);
				}
				else if(param1 == 6) // 设置结束前1分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 59;
					dt.tm_hour = 20;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-now);
				}
				else if(param1 == 7) // 去掉时间限制
				{
					OpenBanquetManager::GetInstance()->SetStartTime(0);
				}
				else if(param1 == 8) // 设置延长结束检查前1分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 19;
					dt.tm_hour = 21;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-now);
				}
				else if(param1 == 9) // 设置结束前1分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 29;
					dt.tm_hour = 21;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-now);
				}
				else if(param1 == 10) // 设置凌晨前1分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 59;
					dt.tm_hour = 23;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-now);
				}
				else if(param1 == 11) // 设置第二天战场开启1分钟
				{
					struct tm dt;
					time_t now = Timer::GetTime() + 86400;
					localtime_r(&now, &dt);
					dt.tm_sec = 30;
					dt.tm_min = 59;
					dt.tm_hour = 18;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-Timer::GetTime());

					OpenBanquetManager::GetInstance()->SetForgedTime(t_base-Timer::GetTime());
				}
				else if(param1 == 12) // 设置一方人数
				{
					OpenBanquetManager::GetInstance()->SetMaxHalfRoleNum(roleid, param2);
				}
				else if(param1 == 13) // 攻击方加个人
				{
					OpenBanquetManager::GetInstance()->SetHalfRoleNum(roleid);

				}
			}
			break;
			case 1001:// 6v6战队报名
			{
				int param1 = 0; // map_id
				int param2 = 0; // iscross
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo == NULL)
				{
					LOG_TRACE("DebugCommand pinfo==NULL");
					return;
				}
				int zoneid = pinfo->user->src_zoneid;
				std::vector<TeamRaidApplyingRole> roles;
				TeamRaidApplyingRole role;
				role.roleid = roleid;
				role.score = 1000;
				roles.push_back(role);
				int teamid = CrossCrssvrTeamsManager::Instance()->GetRoleTeamId(roleid);
				LOG_TRACE("DebugCommand Recv On Central map_id=%d, zoneid=%d, roleid=%d, teamid=%d, iscross=%d", param1, zoneid, roleid, teamid, param2);
	                        {
                 	               RaidManager::GetInstance()->OnRaidTeamApply(param1,roles,1, param2, sid, zoneid, teamid, 0, 0);
				}
			}
			break;
			case 1002:// 6v6战队各个模块模拟测试
			{
				int param1 = 0; // 
				int param2 = 0; // 
				int param3 = 0; // 
				int param4 = 0; // 

				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d-%d-%d", &param1, &param2, &param3, &param4); 
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline((roleid));
				if (pinfo == NULL)
				{
					LOG_TRACE("DebugCommand pinfo==NULL");
					return;
				}
				if(param1 == 1)// 模拟战斗结束结算消息
				{
					//int gs_id = 200;//param2;
					//int index = 5101;//param3;
					//int map_id = 543;
					int result = param2;
					LOG_TRACE("DebugCommand DebugRaidEnd roleid=%d, result=%d",roleid, result);
					RaidManager::GetInstance()->DebugRaidEnd(roleid, result);
					//RaidManager::GetInstance()->OnRaidEnd(gs_id,index,map_id,result);
				}
				else if(param1 == 2) // 模拟领取奖励消息
				{
					int award_type = param2;
					ProtocolExecutor *task = new ProtocolExecutor(manager, sid, new GCrssvrTeamsGetScore(roleid, award_type, 0));
					Thread::Pool::AddTask(task);
				}
				else if(param1 == 3)// 清除改名cd和离开战队cd
				{
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->DebugClearCD(roleid);
					else
						CrssvrTeamsManager::Instance()->DebugClearCD(roleid);
					LOG_TRACE("DebugCommand:  DebugClearCD, roleid=%d", roleid);
				}
				else if(param1 == 4)// 清除内存中的战队信息
				{
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->ClearTeamByRoleId(roleid);
					else
						CrssvrTeamsManager::Instance()->ClearTeamByRoleId(roleid);
					LOG_TRACE("DebugCommand:  ClearTeamByRoleId, roleid=%d", roleid);
				}
				else if(param1 == 5)//清除赛季排行榜
				{
					GTopTable top_null;
					std::map<int, int> pos_null;
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->SendSeasonTopSave(0, top_null, pos_null);
					else
						CrssvrTeamsManager::Instance()->SendSeasonTopSave(0, top_null, pos_null);
					LOG_TRACE("DebugCommand: SendSeasonTopSave, roleid=%d", roleid);
				}



				//ProtocolExecutor *task = new ProtocolExecutor(manager, sid, new CrssvrTeamsSearch(roleid, Octets(param2, strlen(param2))));
				//Thread::Pool::AddTask(task);
			}
			break;
			case 1003:// 6v6战队赛季时间相关
			{
				//命令：        d_delcmd 1003 1 // 设置逻辑时间为赛季开始前一分钟
				//命令：        d_delcmd 1003 2 // 设置逻辑时间为赛季结束前一分钟
				//命令：        d_delcmd 1003 3 // 设置逻辑时间为下一个赛季开始前1分钟
				int param1 = 0;
				int param2 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 

				time_t now = Timer::GetTime();
				LOG_TRACE("DebugCommand: roleid=%d, param1=%d, param2=%d now=%d", roleid, param1, param2, now);
				if(param1 == 1)// 设置新时间为10月1日前前一分钟,战场准备开启
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_mon = 8;
					dt.tm_mday = 30;
					dt.tm_sec = 40;
					dt.tm_min = 59; 
					dt.tm_hour = 23;
					time_t t_base = mktime(&dt);
					
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);
					else
						CrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);
				}
				else if(param1 == 2) // 设置战场时间为12月25日前一分钟，战场准备关闭
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_mon = 11;
					dt.tm_mday = 25;
					dt.tm_sec = 40;
					dt.tm_min = 59; 
					dt.tm_hour = 23;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);
				
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);
					else
						CrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);

				}
				else if(param1 == 3) // 设置战场时间为10月1日过一星期
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_mon = 9;
					dt.tm_mday = 6;
					dt.tm_sec = 40;
					dt.tm_min = 59; 
					dt.tm_hour = 23;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);
				
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);
					else
						CrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);

				}
				else if(param1 == 4) // 设置战场时间为10月1日过2星期
				{
					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_mon = 9;
					dt.tm_mday = 13;
					dt.tm_sec = 40;
					dt.tm_min = 59; 
					dt.tm_hour = 23;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);
				
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);
					else
						CrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);

				}
				else if(param1 == 5) // 设置战场时间为任意时间
				{
					int param1 = 0;
					int param2 = 0;
					int param3 = 0;
					char buf[256];
					memset(buf, 0, sizeof(buf));
					strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
					sscanf(buf, "%d-%d-%d", &param1, &param2, &param3); 


					struct tm dt;
					time_t now = Timer::GetTime();
					localtime_r(&now, &dt);
					dt.tm_mon = param2;
					dt.tm_mday = param3;
					dt.tm_sec = 40;
					dt.tm_min = 59; 
					dt.tm_hour = 23;
					time_t t_base = mktime(&dt);
					LOG_TRACE("DebugCommand:  t_base %s, tbase %d, tbase-now=%d", ctime(&t_base), t_base, t_base-now);
				
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						CrossCrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);
					else
						CrssvrTeamsManager::Instance()->SetForgedTime(t_base-now);

				}

			}
			break;
			case 1004:
			{
				int param1 = 0;
				int param2 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 

				time_t now = Timer::GetTime();
				LOG_TRACE("DebugCommand: roleid=%d, param1=%d, param2=%d now=%d", roleid, param1, param2, now);

				if(param1 == 1)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRoleScore(roleid, param2);
				}
				else if(param1 == 2)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRoleTeamScore(roleid, param2);
				}
				else if(param1 == 3)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRolesScore(roleid, param2);
				}
				else if(param1 == 4)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugTeamWin(roleid, param2);
				}
				else if(param1 == 5)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugTeamPunish(roleid, param2);
				}
				else if(param1 == 6)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRolePunish(roleid, param2);
				}
				else if(param1 == 7)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRoleLastMaxScore(roleid, param2);
				}
				else if(param1 == 8)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRoleSeasonPos(roleid, param2);
				}
				else if(param1 == 9)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->DebugRoleWeekPos(roleid, param2);
				}
				else if(param1 == 10)
				{
					if (!GDeliveryServer::GetInstance()->IsCentralDS())
						CrssvrTeamsManager::Instance()->SendTeamGetReq(roleid, param2, false);
				}

			}
			break;
			case 1005:
			{
				int param1 = 0;
				int param2 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 

				time_t now = Timer::GetTime();
				LOG_TRACE("DebugCommand: roleid=%d, param1=%d, param2=%d now=%d", roleid, param1, param2, now);

				if(param1 == 1)
				{
					//if (!GDeliveryServer::GetInstance()->IsCentralDS())
						RaidManager::GetInstance()->SetHideSeekRaidLimit(TEAM_RAID_NORMAL,param2);
				}
				else if(param1 == 2)// 模拟报名
				{
					//if (!GDeliveryServer::GetInstance()->IsCentralDS())
					{
						GHideSeekRaidApply* req = new GHideSeekRaidApply;
						req->map_id = 549;
						TeamRaidApplyingRole role;
						role.roleid = param2;
						std::vector<TeamRaidApplyingRole> roles;
						req->roles.push_back(role);
						ProtocolExecutor *task = new ProtocolExecutor(manager, sid, req);
						Thread::Pool::AddTask(task);

					}
				}
				else if(param1 == 3) // 模拟组队报名
				{
					int param1 = 0;
					int param2 = 0;
					int param3 = 0;
					char buf[256];
					memset(buf, 0, sizeof(buf));
					strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
					sscanf(buf, "%d-%d-%d", &param1, &param2, &param3); 


					//if (!GDeliveryServer::GetInstance()->IsCentralDS())
					{
						GHideSeekRaidApply* req = new GHideSeekRaidApply;
						req->map_id = 549;
						TeamRaidApplyingRole role;
						role.roleid = param2;
						std::vector<TeamRaidApplyingRole> roles;
						req->roles.push_back(role);

						role.roleid = param3;
						req->roles.push_back(role);
						ProtocolExecutor *task = new ProtocolExecutor(manager, sid, req);
						Thread::Pool::AddTask(task);
					}
				}
				else if(param1 == 4) // 退出报名
				{
					//if (!GDeliveryServer::GetInstance()->IsCentralDS())
					{
						HideSeekRaidQuit* req = new HideSeekRaidQuit;
						req->roleid = param2;
						req->map_id = 549;
						ProtocolExecutor *task = new ProtocolExecutor(manager, sid, req);
						Thread::Pool::AddTask(task);
					}
				}

			}
			break;
			case 1006:
			{
				int param1 = 0;
				int param2 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 

				time_t now = Timer::GetTime();
				LOG_TRACE("DebugCommand: roleid=%d, param1=%d, param2=%d now=%d", roleid, param1, param2, now);

				CrssvrTeamsRename* ct_rename = new CrssvrTeamsRename;
				ct_rename->roleid = roleid;
				ct_rename->teamid = param1;
				if(param2 != 0)
					ct_rename->new_teamsname = Octets("student", 7); 
				ProtocolExecutor *task = new ProtocolExecutor(manager, sid, ct_rename);
				Thread::Pool::AddTask(task);

			}
			break;
			case 1007:
			{
				int param1 = 0;
				int param2 = 0;
				char buf[256];
				memset(buf, 0, sizeof(buf));
				strncpy(buf, (char*)data.begin(), std::min(data.size(), sizeof(buf)-1));
				sscanf(buf, "%d-%d", &param1, &param2); 

				time_t now = Timer::GetTime();
				LOG_TRACE("DebugCommand: roleid=%d, param1=%d, param2=%d now=%d", roleid, param1, param2, now);

				if(param1 == 1)
				{
					if (GDeliveryServer::GetInstance()->IsCentralDS())
						RaidManager::GetInstance()->SetHideSeekRaidLimit(TEAM_RAID_REBORN, param2);
				}
				else if(param1 == 2)// 模拟报名
				{
					if (GDeliveryServer::GetInstance()->IsCentralDS())
					{
						GFengShenRaidApply* req = new GFengShenRaidApply;
						req->map_id = 547;
						TeamRaidApplyingRole role;
						role.roleid = param2;
						std::vector<TeamRaidApplyingRole> roles;
						req->roles.push_back(role);
						ProtocolExecutor *task = new ProtocolExecutor(manager, sid, req);
						Thread::Pool::AddTask(task);

					}
				}
				else if(param1 == 4) // 退出报名
				{
					if (GDeliveryServer::GetInstance()->IsCentralDS())
					{
						HideSeekRaidQuit* req = new HideSeekRaidQuit;
						req->roleid = param2;
						req->map_id = 549;
						ProtocolExecutor *task = new ProtocolExecutor(manager, sid, req);
						Thread::Pool::AddTask(task);
					}
				}

			}
			break;


			default:
			break;
		}
	}
};
};

#endif
