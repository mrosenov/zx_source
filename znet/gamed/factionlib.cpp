#include "types.h"
#include "obj_interface.h"

#include "libcommon.h"
#include "gproviderclient.hpp"
#include "playerfactioninfo.hpp"
#include "battleserverregister.hpp"
#include "gbattleend.hpp"
//#include "battleenter.hpp"
#include "battlestart_re.hpp"
#include "factioncreate.hpp"
#include "factiondismiss.hpp"
#include "factionupgrade.hpp"
#include "ghostileprotect.hpp"
#include "sendfactioncreate.hpp"
#include "hostileprotect.hpp"
#include "gsiegeserverregister.hpp"
#include "sendsiegestart_re.hpp"
#include "gsiegeend.hpp"
#include "siegechallenge.hpp"
#include "sendsiegechallenge.hpp"
#include "siegesetassistant.hpp"
#include "siegebroadcat.hpp"
#include "gbattleleave.hpp"
#include "ginstancingleave.hpp"
#include "ginstancingstatus.hpp"
#include "instancingregister.hpp"
#include "sendinstancingjoin.hpp"
#include "sendinstancingstart_re.hpp"
#include "sendhometownexchgmoney.hpp"
#include "territoryserverregister.hpp"
#include "sendterritorystart_re.hpp"
#include "gterritoryend.hpp"
#include "gterritoryenter.hpp"
#include "gterritoryleave.hpp"
#include "gterritorychallenge.hpp"
#include "gterritoryitemget.hrp"
#include "gbattlejoin.hpp"
#include "guniquebid.hrp"
#include "guniquegetitem.hrp"
#include "graidserverregister.hpp"
#include "graidopen.hpp"
#include "graidjoin.hpp"
#include "sendraidstart_re.hpp"
#include "sendraidenter_re.hpp"
#include "graidleave.hpp"
#include "graidend.hpp"
#include "graidentersuccess.hpp"
#include "registerglobalcounter.hpp"
#include "globaldropbroadcast.hpp"
#include "gchangerolename.hpp"
#include "gchangefactionname.hpp"
#include "gfactionpkraidfightnotify.hpp"
#include "gfactionpkraiddeposit.hpp"
#include "gfactionpkraidkill.hpp"
#include "gkingdombattleregister.hpp"
#include "kingdombattlestart_re.hpp"
#include "gbattleleave.hpp"
#include "gkingdombattleend.hpp"
#include "gkingdombattlehalf.hpp"
#include "gkingdombattleenter.hpp"
#include "gkingdombattleleave.hpp"
#include "gkingdomattackerfail.hpp"
#include "gtouchpointexchange.hpp"
#include "gopenbanquetleave.hpp"
#include "gkingissuetask.hpp"
#include "gopenbanquetjoin.hpp"
#include "sendcrssvrteamscreate.hpp" 
#include "gkinggetreward.hpp"
#include "gfacbaseserverregister.hpp"
#include "startfacbase_re.hpp"
#include "stopfacbase_re.hpp"
#include "gfacbaseenter.hpp"
#include "gfacbaseleave.hpp"

#include "../gdbclient/db_if.h"
#include "factionlib.h"

#define GDELIVERY_SERVER_ID  0

#define CASE_PROTO_HANDLE(_proto_name_)\
	case _proto_name_::PROTOCOL_TYPE:\
	{\
		_proto_name_ proto;\
		proto.unmarshal( os );\
		if ( proto.GetType()!=_proto_name_::PROTOCOL_TYPE || !proto.SizePolicy(os.size()) )\
			return false; \
		return Handle_##_proto_name_( proto,obj_if );\
	}

#define CASE_FACTION_PROTO_HANDLE(_proto_name_)\
	case _proto_name_::PROTOCOL_TYPE:\
	{\
		_proto_name_ proto;\
		proto.unmarshal( os );\
		if ( proto.GetType()!=_proto_name_::PROTOCOL_TYPE || !proto.SizePolicy(os.size()) )\
			return false; \
		return Handle_##_proto_name_( proto,obj_if );\
	}


namespace GNET
{
	bool Handle_FactionCreate( FactionCreate& proto,object_interface& obj_if )
	{
		if(proto.roleid!=obj_if.GetSelfID().id ) 
			return false;
		if(proto.scale)
		{
			if(obj_if.GetBasicProp().level<25 || obj_if.GetMoney()<50000)
				return false;
		}
		else if(obj_if.GetBasicProp().level<35 || obj_if.GetMoney()<300000)
			return false;

		SendFactionCreate request(proto.roleid, proto.factionname);
		request.scale = proto.scale;

		if (!GetSyncData(request.syncdata,obj_if)) 
			return false;
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,request ) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}
	bool Handle_FactionDismiss( FactionDismiss& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return false;
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}
	bool Handle_FactionUpgrade( FactionUpgrade& proto,object_interface& obj_if )
	{
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return false;
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}
	bool Handle_HostileProtect( HostileProtect& proto,object_interface& obj_if )
	{
		// 检测id
		if ( proto.roleid!=obj_if.GetSelfID().id ) 
			return false;
		// 检测帮派
		if( proto.factionid != obj_if.GetMafiaID() )
			return false;
		// 没有帮派
		if( obj_if.GetMafiaID() == 0 )
			return false;
		// 没有物品
		if( !obj_if.CheckItem( proto.item_pos, proto.item_id, 1 ) )
		{
			return false;
		}
		// 发送同步数据,物品数据给delivery
		GHostileProtect send_proto;
		if( !GetSyncData( send_proto.syncdata, obj_if ) )
			return false;
		send_proto.roleid = proto.roleid;
		send_proto.factionid = proto.factionid;
		send_proto.item_pos = proto.item_pos;
		send_proto.item_id = proto.item_id;
		send_proto.item_num = 1;
		if( obj_if.TradeLockPlayer( 0, DBMASK_PUT_SYNC_TIMEOUT ) == 0 )
		{
			if( GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, send_proto ) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	bool ForwardFactionOP( int type,int roleid,const void* pParams,size_t param_len,object_interface obj_if )	
	{
		try {
			Marshal::OctetsStream os( Octets(pParams,param_len) );
			switch (type)
			{
				CASE_FACTION_PROTO_HANDLE(FactionCreate)
				CASE_FACTION_PROTO_HANDLE(FactionDismiss)
				CASE_FACTION_PROTO_HANDLE(FactionUpgrade)
				CASE_FACTION_PROTO_HANDLE(HostileProtect)
				default:
					return false;	
			}
		}
		catch ( Marshal::Exception )
		{
			return false;
		}
		return 0;
	}

	bool QueryPlayerFactionInfo( int roleid )
	{
		PlayerFactionInfo pfi;
		pfi.rolelist.add(roleid);
		//return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,pfi);
		return 1;
	}

	bool QueryPlayerFactionInfo( const int* list, int list_len )
	{
		PlayerFactionInfo pfi;
		if (list==NULL || list_len<=0) return false;
		for( ; list_len-- ; pfi.rolelist.add(*list++) );
		//return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,pfi);
		return 1;
	}

	bool SendBattleEnd( int gs_id, int battle_world_tag, int battle_id, char battle_type )
	{
		GBattleEnd proto( gs_id, battle_world_tag, battle_id, battle_type);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}

	void SendBattleStatus(int gs_id ,int battle_id, int tag ,int status)
	{
		GInstancingStatus proto(gs_id, battle_id, tag, status);
		GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool ResponseBattleStart( int retcode, int gs_id, int battle_world_tag, int battle_id, char battle_type)
	{
		BattleStart_Re proto( retcode, gs_id, battle_world_tag, battle_id, battle_type);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}

	bool SendPlayerLeave(int gs_id,int battle_id,int world_tag,int player_id,int cond_kick)
	{
		GBattleLeave proto(gs_id,battle_id,world_tag,player_id,cond_kick);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendPlayerLeaveInstence(int gs_id,int battle_id,int player_id,int cond_kick)
	{
		GInstancingLeave proto(gs_id,battle_id,player_id,cond_kick);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendServerRegister( int gs_id, const void* param, size_t len )
	{
		struct battle_field_info
		{
			int battle_id;
			int battle_world_tag;
			int level_min;
			int level_max;
			int battle_type;
			int reborn_count_need;     //进入战场条件,0代表普通战场，1代表飞升战场
		    int fighting_time;         //战场战斗时间，单位为秒
		    int max_enter_perday;      //每个玩家每天能参加的次数
		    int cooldown_time;         //连续报名的间隔时间,冷却时间,单位为秒
		    int max_player_count;      //战场最大人数
		};
		std::vector<BattleFieldInfo> field;
		if( len % sizeof(battle_field_info) != 0 ) return false;
		if( len / sizeof(battle_field_info)  <= 0 ) return false;
		size_t i = 0;
		const battle_field_info* pTemp = (const battle_field_info*)param;
		for( i = 0; i < len / sizeof(battle_field_info); ++i )
		{
			BattleFieldInfo info;
			info.map_id             = pTemp->battle_id;
			info.tag                = pTemp->battle_world_tag;
			info.level_min          = pTemp->level_min;
			info.level_max          = pTemp->level_max;
			info.battle_type        = pTemp->battle_type;
			info.reborn_count_need  = pTemp->reborn_count_need;
			info.fighting_time      = pTemp->fighting_time;
			info.entermax_perday    = pTemp->max_enter_perday;
			info.cooldown_time      = pTemp->cooldown_time;
			info.maxplayer_perround = pTemp->max_player_count;
			field.push_back( info );
			++pTemp;
		}
		BattleServerRegister proto( gs_id, field );
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}

	bool SendInstanceRegister(int gs_id,const void* param,size_t len)
	{
		struct battle_field_info
		{
			int battle_id;
			int world_tag;
			int tid;
			int queuing_time;
			int fighting_time;
			int preparing_time;
			int max_player_limit;
			int manual_start_player_num;
			int auto_start_player_num;
			int reborn_limit;
			int min_level_limit;
			int max_level_limit;
			int max_prof_count_limit;
			int faction_limit;
			int required_item;
			int required_money;
		};
		std::vector<InstancingFieldInfo> fields;
		if(len % sizeof(battle_field_info) != 0) return false;
		if(len / sizeof(battle_field_info) <= 0) return false;
		const battle_field_info* pTemp = (const battle_field_info*)param;
		for(size_t i = 0;i < len / sizeof(battle_field_info);++i)
		{
			InstancingFieldInfo info;
			info.battle_id = pTemp->battle_id;
			info.world_tag= pTemp->world_tag;
			info.tid= pTemp->tid;
			info.queuing_time = pTemp->queuing_time;
			info.fighting_time = pTemp->fighting_time;
			info.preparing_time = pTemp->preparing_time;

			info.attacker.max_player_limit = pTemp->max_player_limit;
			info.attacker.manual_start_player_num = pTemp->manual_start_player_num;
			info.attacker.auto_start_player_num = pTemp->auto_start_player_num;
			info.attacker.reborn_limit = pTemp->reborn_limit;
			info.attacker.min_level_limit = pTemp->min_level_limit;
			info.attacker.max_level_limit = pTemp->max_level_limit;
			info.attacker.max_prof_count_limit = pTemp->max_prof_count_limit;
			info.attacker.faction_limit = pTemp->faction_limit;
			info.attacker.required_item = pTemp->required_item;
			info.attacker.required_money = pTemp->required_money;

			fields.push_back( info );
			++pTemp;
		}
		InstancingRegister proto(gs_id,fields);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendBattleFieldRegister(int gs_id,int battle_id,int world_tag,unsigned char use_for_battle)
	{
		GSiegeServerRegister proto(battle_id,world_tag,use_for_battle);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendBattleFieldStartRe(int gs_id,int battle_id,int world_tag,int retcode)
	{
		SendSiegeStart_Re proto(retcode,battle_id,world_tag);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendBattleFieldEnd(int gs_id,int battle_id,int world_tag,int result)
	{
		GSiegeEnd proto(battle_id,world_tag,result);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendBattleFieldBroadcast(int mafia_id,int msg_type,const void* buf,size_t size)
	{
		Marshal::OctetsStream os(Octets(buf,size));
		SiegeBroadcat proto;
		proto.fid = mafia_id;
		proto.msg_type = msg_type;
		proto.msg = os;
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool Handle_SiegeChallenge( SiegeChallenge& proto,object_interface& obj_if )
	{       
		if(proto.roleid!=obj_if.GetSelfID().id )
		{
			printf("roleid invalid\n");
			return false;
		}
		if( proto.factionid == 0 || proto.factionid != obj_if.GetMafiaID() )
		{
			printf("faction id invalid\n");
			return false;
		}
		if(proto.day)
		{
			if (obj_if.GetMoney() < (unsigned int)1000000*proto.day)
			{
				printf("money not enough\n");
				return false;
			}
		}
		SendSiegeChallenge request(proto.roleid, proto.battle_id, proto.factionid, proto.day,16381); 
		if(!GetSyncData(request.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			DEBUG_PRINT("Send SendSiegeChallenge to delivery (%d,%d,%d,%d)\n",
				proto.roleid,proto.battle_id,proto.factionid,proto.day);
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,request ) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}   
	bool Handle_SiegeSetAssistant( SiegeSetAssistant & proto,object_interface& obj_if )
	{       
		if ( proto.roleid!=obj_if.GetSelfID().id )
			return false;
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto);
	}

	bool SendInstancingJoinToDelivery(int role_id,int gs_id,int battle_id,object_interface obj_if)
	{       
		if(role_id!=obj_if.GetSelfID().id )
		{
			printf("roleid invalid\n");
			return false;
		}
		SendInstancingJoin proto(role_id,gs_id,battle_id,1);
		if(!GetSyncData(proto.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			DEBUG_PRINT("Send SendInstancingJoin to delivery (%d,%d,%d,%d)\n",
				proto.roleid,proto.gs_id,proto.map_id,proto.team);
			if (GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto))
				return true;
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}

	bool ForwardBattleOP(int type,int roleid,const void* pParams,size_t param_len,object_interface obj_if)
	{
		try {
			Marshal::OctetsStream os( Octets(pParams,param_len) );
			switch (type)
			{
				CASE_PROTO_HANDLE(SiegeChallenge)
				CASE_PROTO_HANDLE(SiegeSetAssistant)
				default:
					return false;	
			}
		}
		catch ( Marshal::Exception )
		{
			return false;
		}
		return true;
	}
	
	bool ResponseInstanceStart( int retcode, int gs_id, int battle_id)
	{
		SendInstancingStart_Re proto( retcode, gs_id, battle_id);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto);
	}

	bool SendExchangeHometownMoney(int roleid, int amount, object_interface obj_if)
	{
		if(roleid!=obj_if.GetSelfID().id )
		{
			return false;
		}
		SendHometownExchgMoney proto(roleid, amount);
		if(!GetSyncData(proto.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto))
				return true;
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}
	
	bool SendTerritoryRegister( int gs_id, const void* param, size_t len )
	{
		struct territory_field_info
		{
			int battle_type;
			int battle_world_tag;
			int player_limit;
		};
		TerritoryServerVector field;
		assert(len % sizeof(territory_field_info) == 0);
		assert(len / sizeof(territory_field_info)  > 0);

		size_t i = 0;
		const territory_field_info* pTemp = (const territory_field_info*)param;
		for( i = 0; i < len / sizeof(territory_field_info); ++i )
		{
			TerritoryServer info;
			info.map_type = pTemp->battle_type;
			info.map_tag = pTemp->battle_world_tag;
			info.player_limit = pTemp->player_limit;
			field.push_back( info );
			++pTemp;
		}
		TerritoryServerRegister proto( gs_id, field );
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}
	
	bool ResponseTerritoryStart( int retcode, int territory_id)
	{
		SendTerritoryStart_Re proto( retcode, territory_id);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto);
	}
	
	bool SendTerritoryEnd(int territory_id, int result)
	{
		GTerritoryEnd proto( territory_id, result);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}
	
	bool SendTerritoryEnter(int roleid, unsigned int mafia_id, int territory_id)
	{
		GTerritoryEnter proto(roleid, mafia_id, territory_id);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}
	
	bool SendTerritoryLeave(int roleid, int territory_id, int map_id, float x, float y, float z)
	{
		GTerritoryLeave proto(roleid, territory_id, map_id, x, y, z);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}
	
	bool SendTerritoryChallenge(int roleid, int territory_id, unsigned int faction_id, int itemid,  int item_count, object_interface obj_if)
	{
		GTerritoryChallenge proto(roleid, territory_id, faction_id, itemid, item_count); 
		if(!GetSyncData(proto.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,proto) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}


	bool SendTerritoryGetAward(int roleid, unsigned int faction_id, int territory_id, char reward_type,  int itemid, int itemcount, int money, object_interface obj_if)
	{
		GTerritoryItemGetArg arg;
		arg.roleid = roleid;
		arg.factionid = faction_id;
		arg.territoryid = territory_id;
		arg.itemtype = reward_type;
		arg.itemid = itemid;
		arg.item_count = itemcount;
		arg.money = money;
		
		GTerritoryItemGet *rpc = (GTerritoryItemGet*)Rpc::Call(RPC_GTERRITORYITEMGET, arg);
		
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( 0,rpc) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}


	bool SendArenaChallenge(int roleid, int gs_id, int map_id, int item_id, int item_index, object_interface obj_if)
	{
		GBattleJoin proto(roleid, gs_id, map_id, item_id, item_index);
		proto.localsid = obj_if.GetLinkSID(); 
		if(!GetSyncData(proto.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,proto))
				return true;
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}

	bool SendUniqueBidRequest(int roleid, int money_upperbound, int money_lowerbound, object_interface obj_if)
	{
		GUniqueBidArg arg;
		arg.roleid = roleid;
		arg.lower_bound_money = money_lowerbound;
		arg.upper_bound_money = money_upperbound;
		
		GUniqueBid *rpc = (GUniqueBid*)Rpc::Call(RPC_GUNIQUEBID, arg);
		
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( 0,rpc) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	bool SendUniqueBidGetItem(int roleid, object_interface obj_if)
	{
		GUniqueGetItemArg arg;
		arg.roleid = roleid;
		
		GUniqueGetItem *rpc = (GUniqueGetItem*)Rpc::Call(RPC_GUNIQUEGETITEM, arg);
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( 0,rpc) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	bool SendRaidServerRegister(int gs_id, const void* param, size_t len)
	{
		struct raid_world_info
		{
			int raid_id;                    
			int raid_template_id;
			int raid_type;                  
			int queuing_time;         
			int raid_max_instance;
			struct
			{
				int max_player_limit;
				int min_start_player_num;
			} group_limit;
		};

		if(len % sizeof(raid_world_info) != 0) return false;
		if(len / sizeof(raid_world_info) <= 0) return false;

		std::vector<RaidFieldInfo> infos;
		const raid_world_info* ri = (const raid_world_info*)param;
		for(size_t i = 0; i < len / sizeof(raid_world_info); ++ i)
		{
			RaidFieldInfo info;
			info.map_id = ri->raid_id;
			info.raid_template_id = ri->raid_template_id;
			info.raid_type = ri->raid_type;
			info.queuing_time = ri->queuing_time;
			info.raid_max_instance = ri->raid_max_instance;
			info.group1_limit.max_player_limit = ri->group_limit.max_player_limit;
			info.group1_limit.min_start_player_num = ri->group_limit.min_start_player_num;
			infos.push_back(info);
			++ ri;
		}
		GRaidServerRegister msg(gs_id, infos);
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendRaidOpen(int roleid, int map_id, int itemid, int item_count, char can_vote, char difficulty, const char* roomname, size_t roomname_len, object_interface obj_if)
	{
		GRaidOpen msg;
		msg.roleid = roleid;
		msg.map_id = map_id;
		msg.itemid = itemid;
		msg.item_count = item_count;
		msg.roomname.replace(roomname, roomname_len);	
		msg.can_vote = can_vote;
		msg.difficulty = difficulty;
		if(!GetSyncData(msg.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,msg) )
			{
				return true;
			}
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}

	bool SendRaidJoin(int roleid, int map_id, int raidroom_id, int localsid, int raid_faction)
	{
		GRaidJoin msg;
		msg.roleid = roleid;
		msg.map_id = map_id;
		msg.raidroom_id = raidroom_id;
		msg.groupid = raid_faction;
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendRaidStartRe(int retcode, int gs_id, int map_id, int raidroom_id, int index)
	{
		SendRaidStart_Re msg;
		msg.retcode = retcode;
		msg.gs_id = gs_id;
		msg.map_id = map_id;
		msg.raidroom_id = raidroom_id;
		msg.index = index;
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendRaidEnterRe(int roleid, int gs_id, int retcode, int map_id, int index, int raid_type)
	{
		SendRaidEnter_Re msg;
		msg.roleid = roleid;
		msg.gs_id = gs_id;
		msg.retcode = retcode;
		msg.map_id = map_id;
		msg.index = index;
		msg.raid_type = raid_type;
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendRaidLeave(int gs_id, int roleid, int map_id, int index, float scalex, float scaley, float scalez, int reason)
	{
		GRaidLeave msg;
		msg.gs_id = gs_id;
		msg.roleid = roleid;
		msg.map_id = map_id;
		msg.index = index;
		msg.scalex = scalex;
		msg.scaley = scaley;
		msg.scalez = scalez;
		msg.reason = reason;
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendRaidEnd(int gs_id, int map_id, int index, int result)
	{
		GRaidEnd msg;
		msg.gs_id = gs_id;
		msg.map_id = map_id;
		msg.index = index;
		msg.result = result;
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendRaidEnterSuccess(int gs_id, int role_id, int map_id, int index)
	{
		GRaidEnterSuccess msg;
		msg.gs_id = gs_id;
		msg.roleid = role_id;
		msg.map_id = map_id;
		msg.index = index;
		return GProviderClient::DispatchProtocol(0, msg);	
	}

	bool SendGFactionPkRaidFightNotify(int map_id, int raidroom_id, int end_time, char is_start)
	{
		GFactionPkRaidFightNotify msg;
		msg.mapid		= map_id;
		msg.raidroom_id	= raidroom_id;
		msg.end_time	= end_time;
		msg.is_start	= is_start;
		return GProviderClient::DispatchProtocol(0, msg);
	}

	bool SendGFactionPkRaidDeposit(int map_id, int index, unsigned int money)
	{
		GFactionPkRaidDeposit msg;
		msg.mapid = map_id;
		msg.index = index;
		msg.money = money;
		return GProviderClient::DispatchProtocol(0, msg);
	}

	bool SendGFactionPkRaidKill(int map_id, int index, int killer, int victim, int victim_faction)
	{
		GFactionPkRaidKill msg;
		msg.map_id = map_id;
		msg.index = index;
		msg.killer = killer;
		msg.victim = victim;
		msg.victim_faction = victim_faction;
		return GProviderClient::DispatchProtocol(0, msg);
	}

	bool SendGlobalCountRegister(const void* param, size_t len)
	{
		struct count_drop_item
		{
			int template_id;
			int item_id;
			int item_num;
			int start_time;
			int time_of_duration;
			int speak_interval;
		}; 

		std::vector<GlobalCounterInfo> items;

		if( len % sizeof(count_drop_item) != 0 ) return false;
		if( len / sizeof(count_drop_item)  <= 0 ) return false;
		size_t i = 0;
		const count_drop_item* pTemp = (const count_drop_item*)param;
		for( i = 0; i < len / sizeof(count_drop_item); ++i )
		{
			GlobalCounterInfo info;
			info.template_id = pTemp->template_id;
			info.itemid = pTemp->item_id;
			info.max_count = pTemp->item_num;
			info.start_time = pTemp->start_time;
			info.duration = pTemp->time_of_duration;
			info.speak_interval = pTemp->speak_interval;
			++pTemp;
			items.push_back(info);
		}
		
		RegisterGlobalCounter proto(items);
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}

	bool SendGlobalDropBoardcast(int role_id, int item_id, int left_cnt)
	{
		GlobalDropBroadCast proto(role_id, item_id, left_cnt);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID, proto);
	}

	bool SendChangeRoleName(int role_id, size_t name_len, char * name, int item_id, int item_pos, object_interface obj_if)
	{
		GChangeRolename msg;
		msg.roleid = role_id;
		msg.newname.replace(name, name_len);
		msg.itemid = item_id;
		msg.item_pos = item_pos;

		if(!GetSyncData(msg.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,msg) )
			{
				return true;
			}
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}
	
	bool SendChangeFactionName(int role_id, int fid, size_t name_len, char * name, char type, int item_id, int item_pos, object_interface obj_if)
	{
		GChangeFactionName msg;
		msg.roleid = role_id;
		msg.fid = fid;
		msg.newname.replace(name, name_len);
		msg.scale = type;
		msg.itemid = item_id;
		msg.item_pos = item_pos;

		if(!GetSyncData(msg.syncdata,obj_if)) 
		{
			printf("GetSyncData failed\n");
			return false;
		}
		
		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{
			if (GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID,msg) )
			{
				return true;
			}
			obj_if.TradeUnLockPlayer();
		}
		printf("TradeLockPlayer failed\n");
		return false;
	}

	bool SendKingdomRegister( int gs_id, const void* param, size_t len )
	{
		struct kingdom_field_info
		{
			int type;
			int world_tag;
		};
		KingdomBattleInfoVector field;
		assert(len % sizeof(kingdom_field_info) == 0);
		assert(len / sizeof(kingdom_field_info)  > 0);

		size_t i = 0;
		const kingdom_field_info* pTemp = (const kingdom_field_info*)param;
		for( i = 0; i < len / sizeof(kingdom_field_info); ++i )
		{
			KingdomBattleInfo info;
			info.fieldtype = pTemp->type;
			info.tagid = pTemp->world_tag;
			field.push_back( info );
			++pTemp;
		}
		GKingdomBattleRegister proto( gs_id, field );
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, proto );
	}

	bool SendFacBaseRegister(int gs_id, int max_base)
	{
		GFacBaseServerRegister pro(gs_id, max_base);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID, pro);
	}

	bool ResponseKingdomStart(int retcode, char fieldtype, int tag_id, int defender, std::vector<int> & attacker_list, void * defender_name, size_t name_len)
	{
		KingdomBattleStart_Re msg;
		msg.retcode = retcode;
		msg.fieldtype = fieldtype;
		msg.tagid = tag_id;
		msg.defender = defender;
		msg.attackers = attacker_list;
		msg.def_name = Octets(defender_name, name_len); 
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, msg);
	}

	bool SendKingdomBattleHalf(char fieldtype, int tag_id, char result, std::vector<int> & failattackers)
	{
		GKingdomBattleHalf msg;
		msg.fieldtype = fieldtype;
		msg.tagid = tag_id;
		msg.result = result;
		msg.failattackers = failattackers;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, msg);
	}

	bool SendKingdomEnd(char fieldtype, int tag_id, int winner_mafia_id)
	{
		GKingdomBattleEnd msg;
		msg.fieldtype = fieldtype;
		msg.tagid = tag_id;
		msg.winnerid = winner_mafia_id;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, msg);
	}

	bool SendKingdomEnter(int roleid, char fieldtype)
	{
		GKingdomBattleEnter msg;
		msg.roleid = roleid;
		msg.fieldtype = fieldtype;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, msg);
	}

	bool SendKingdomLeave(int roleid, char fieldtype, int tagid, int map_id, float x, float y, float z)
	{
		GKingdomBattleLeave msg;
		msg.roleid = roleid;
		msg.fieldtype = fieldtype;
		msg.tagid = tagid;
		msg.map_tag = map_id;
	        msg.scalex = x;
		msg.scaley = y;
		msg.scalez = z;	
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, msg);
	}

	bool SendKingdomAttackerFail(char fieldtype, int tagid, int failer)
	{
		GKingdomAttackerFail msg;
		msg.fieldtype = fieldtype;
		msg.tagid = tagid;
		msg.failer = failer;
		return GProviderClient::DispatchProtocol( GDELIVERY_SERVER_ID, msg);

	}

	
	bool SendTouchPointExchange(int roleid, int flag, int count, int points_need, object_interface obj_if)
	{
		GTouchPointExchange msg;
		msg.roleid = roleid;
		msg.flag = flag;
		msg.count = count;
		msg.points_need = points_need;

		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( 0,msg) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	bool SendOpenBanquetLeave(int roleid, int gs_id, int map_id)
	{
		GOpenBanquetLeave msg(roleid);
		msg.map_id = map_id;
		msg.gs_id = gs_id;
		printf("msg.gs_id=%d\n", msg.gs_id);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID, msg);
	}

	bool SendOpenBanquetJoin(int gs_id, bool is_team, std::vector<int>& roles)
	{
		GOpenBanquetJoin msg;
		msg.gs_id = gs_id;
		msg.is_team = is_team ? 1 : 0;
		for(std::vector<int>::iterator it = roles.begin(); it != roles.end(); ++ it)
		{
			msg.roles.push_back(*it);
		}
		return GProviderClient::DispatchProtocol(0, msg);
	}


	bool SendKingDeliverTask(int roleid, int task_type, object_interface obj_if)
	{
		GKingIssueTask msg;
		msg.roleid = roleid;
		msg.task_type = task_type;

		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( 0,msg) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	bool SendCreateCrossVr(int roleid, int name_len, char team_name[], object_interface obj_if)
	{
		SendCrssvrTeamsCreate msg;
		msg.roleid = roleid;
		msg.teamname = Octets(team_name, name_len);
		if (!GetSyncData(msg.syncdata,obj_if)) 
			return false;

		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID,msg))
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	

	bool SendKingGetReward(int roleid, object_interface obj_if)
	{
		GKingGetReward msg;
		msg.roleid = roleid;

		if(obj_if.TradeLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT)==0)
		{                       
			if (GProviderClient::DispatchProtocol( 0,msg) )
				return true;
			obj_if.TradeUnLockPlayer();
		}
		return false;
	}

	bool SendFacBaseStartRe(int retcode, int fid, int gs_id, int index, const std::set<int> & mall_indexes)
	{
		return GProviderClient::DispatchProtocol(0, StartFacBase_Re(retcode, fid, gs_id, index, mall_indexes));	
	}
	bool SendFacBaseEnter(int roleid)
	{
		return GProviderClient::DispatchProtocol(0, GFacBaseEnter(roleid));
	}
	bool SendFacBaseLeave(int roleid, int map_id, float x, float y, float z)
	{
		GFacBaseLeave proto(roleid, map_id, x, y, z);
		return GProviderClient::DispatchProtocol(GDELIVERY_SERVER_ID, proto);
	}
	bool SendFacBaseStopRe(int retcode, int fid, int gs_id, int index)
	{
		return GProviderClient::DispatchProtocol(0, StopFacBase_Re(retcode, fid, gs_id, index));	
	}
};

