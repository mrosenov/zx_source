#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threadpool.h>
#include <conf.h>
#include <io/pollio.h>
#include <io/passiveio.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <glog.h>
#include <algorithm>

#include "../template/itemdataman.h"
#include "../template/npcgendata.h"
#include "raid_player.h"
#include "raid_world_manager.h"
#include "raid_world_message_handler.h"
#include <factionlib.h>
#include "../general_indexer_cfg.h"
#include "../player_imp.h"

raid_world_manager::raid_world_manager()
{
	_raid_id = 0;
	_raid_lock = 0;
	_raid_result = 0;
	_status = RS_IDLE;

	_max_player_count = 0;
	_need_level_min = 0;
	_need_level_max = 0;

	_raid_time = 1800;
	_invincible_time = 0;
	_close_raid_time = 0;
	_player_leave_time = 0;
	_win_ctrl_id = 0;
	_win_task_id = 0;
	memset(_forbidden_items_id, 0, sizeof(_forbidden_items_id));
	memset(_forbidden_skill_id, 0, sizeof(_forbidden_skill_id));

	_heartbeat_counter = 0;
	_seek_heartbeat_counter = 0;
	_raid_info_seq = 0;
	_update_info_seq = 0;	
	_apply_count = 0;

	_start_timestamp = -1;
	_raid_winner_id = 0;
	_max_killer_id = 0;

	_attacker_score = 0;
	_defender_score = 0;

	_attacker_data.Clear();
	_defender_data.Clear();
	_all_monster_killed = false;
	_difficulty = 0;
	_difficulty_ctrl_id = 0;
}

raid_world_manager::~raid_world_manager()
{
	Release();	
}
int raid_world_manager::TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	if(_status != RS_IDLE)
	{
		return -1;	
	}
	return 0;
}

int raid_world_manager::CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	_status = RS_READY;
	__PRINTF( "副本初始化%s 初始化\n", rwt.servername.c_str() );
	_raid_id = rwt.rwinfo.raid_id;
	int ret = global_world_manager::Init(0, rwt.servername.c_str(), tag, index);
	if( 0 != ret )
	{
		return ret;
	}
	delete _message_handler;
	_message_handler = new raid_world_message_handler( this, &_plane );

	if(!OnInit()) 
	{
		return -10010;
	}
	__PRINTF( "副本创建请求, raid_template_id=%d, raid_id=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);

	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt.rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		__PRINTF("无效的副本模板: %d\n", rwt.rwinfo.raid_template_id);
		GLog::log( GLOG_INFO, "副本开启失败,原因：副本模板不存在 raid_template_id=%d, raid_id=%d, _status=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status );
		return -10011;
	}	
	
	_max_player_count = raid_config.max_player_num;
	_need_level_min = raid_config.player_min_level;
	_need_level_max = raid_config.player_max_level;

	_raid_time = raid_config.total_exist_time;
	_invincible_time = raid_config.invincible_time;
	_close_raid_time = 300;
	_player_leave_time = 180;
	_raidroom_id = raidroom_id;

	_win_ctrl_id = raid_config.controller_id;
	_win_task_id = raid_config.award_task_id;

	memcpy(_forbidden_items_id, raid_config.forbiddon_items_id, sizeof(_forbidden_items_id));
	memcpy(_forbidden_skill_id, raid_config.forbiddon_skill_id, sizeof(_forbidden_skill_id));

	for(size_t i = 0; i < sizeof(raid_config.map_variable_id) / sizeof(raid_config.map_variable_id[0]); i ++)
	{
		if(raid_config.map_variable_id[i] >= GENERAL_INDEX_MAP_DESIGNER_BEGIN && raid_config.map_variable_id[i] <= GENERAL_INDEX_MAP_DESIGNER_END)
		{
			_global_var_list.push_back(raid_config.map_variable_id[i]);
		}
	}
	
	// 判断时间戳
	if( _raid_time < 0 )
	{
		__PRINTF( "raid_time参数有误，持续时间为负 raid_time=%d\n", _raid_time);
		GLog::log( GLOG_INFO, "副本开启失败,原因：持续时间为负数 raid_template_id=%d, raid_id=%d, time=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _raid_time);
		return -10012;
	}
	_start_timestamp = g_timer.get_systime();
	_end_timestamp = _start_timestamp + _raid_time;
	_cur_timestamp = _end_timestamp;
	_raid_id = rwt.rwinfo.raid_id;
	_apply_count = roleid_list.size();

	for(size_t i = 0; i < roleid_list.size(); ++ i)
	{
		_raid_member_list.push_back(roleid_list[i]);
	}

	_difficulty = difficulty;
	if(difficulty > 0 && difficulty <= 10)
	{
		_difficulty_ctrl_id = raid_config.level_controller_id[difficulty-1];
	}

	// Youshuang add
	_level_info.clear();
	_tid_2_bossinfo.clear();
	int raid_level_info_id = raid_config.strategy[0];
	if( difficulty > 0 && difficulty <= 10 )
	{
		raid_level_info_id = raid_config.strategy[ difficulty-1 ];
	}
	if( raid_level_info_id > 0 )
	{
		DATA_TYPE dt1;
		const TRANSCRIPT_STRATEGY_CONFIG& level_info_config = *(const TRANSCRIPT_STRATEGY_CONFIG*) gmatrix::GetDataMan().get_data_ptr( raid_level_info_id, ID_SPACE_CONFIG, dt1 );
		if(dt1 != DT_TRANSCRIPT_STRATEGY_CONFIG || &level_info_config == NULL)
		{
			__PRINTF("无效的副本级别模板: %d\n", raid_level_info_id );
			GLog::log( GLOG_INFO, "副本开启失败, raid_level_info_id=%d, raid_id=%d, _status=%d\n", raid_level_info_id, rwt.rwinfo.raid_id, _status );
			return -10011;
		}
		for( int i = 0; i < (int)(sizeof( level_info_config.level_list ) / sizeof( level_info_config.level_list[0] )); ++i )
		{
			if( level_info_config.level_list[i].level_type <= LEVEL_NONE )
			{
				break;
			}
			raid_level_info tmp;
			tmp.howto = level_info_config.level_list[i].level_type;
			int j = 0;
			for( ; j < (int)(sizeof( level_info_config.level_list[i].monster ) / sizeof( level_info_config.level_list[i].monster[0] )); ++j )
			{
				int tid = level_info_config.level_list[i].monster[j];
				if( tid <= 0 )
				{
					break;
				}
				raid_boss_info* tmp_boss = new raid_boss_info( tid, 0, 0 );
				if( !tmp_boss )
				{
					return -10011;
				}
				tmp.boss.push_back( tmp_boss );
				_tid_2_bossinfo.insert( std::pair<int, raid_boss_info*>( tid, tmp_boss ) );
			}
			if( j == 0 )
			{
				break;
			}
			_level_info.push_back( tmp );
		}
	}
	// end
	
	if(_difficulty_ctrl_id > 0) ActiveSpawn( this, _difficulty_ctrl_id, true );

	OnCreate();
	_status = RS_RUNNING;

	__PRINTF( "副本:%d:%d开启,持续时间:%d\n", _raid_id, GetWorldTag(), _raid_time); 
	GLog::log( GLOG_INFO, "副本开启成功 raid_template_id=%d, raid_id=%d, end_time=%d, tag=%d\n", rwt.rwinfo.raid_template_id,_raid_id, _end_timestamp, tag);
	return 0;
}

void raid_world_manager::ForceCloseRaid()
{
	spin_autolock keeper( _raid_lock );
	
	if (_status != RS_RUNNING) return; //防止副本被重复Release()

	_status = RS_CLOSING;

	int exception_raid_close_time = 180;	//关闭时间延长至180s, 目前delivery可能潜在可能在gs没有发送sendraidend的时候会发送这个，导致玩家还没有退出的时候世界已经关闭  
	int exception_player_leave_time = 10;
	
	_cur_timestamp = g_timer.get_systime() + exception_raid_close_time;
	_kill_monster_timestamp = _cur_timestamp - 15;
	_player_leave_time = exception_player_leave_time;
	
	//KillAllMonsters();

	// 战斗结束时更新一次信息
	SendRaidInfo();

	// 计算战斗结果
	__PRINTF( "副本:%d 结束发布结果开始准备清理result=%d, close=%d\n", GetWorldTag(), _raid_result, exception_raid_close_time );
	GLog::log( GLOG_INFO, "副本强制结束,gs_id=%d, raid_world_tag=%d, raid_id=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _raid_id, _end_timestamp );
}

void raid_world_manager::Release()
{
	// 不需要残留怪物检测，策划保证所有怪物在45后消失
	GLog::log( GLOG_INFO, "副本结束清理结束,gs_id=%d, raid_world_tag=%d, raid_id=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _raid_id, _end_timestamp );
	Reset();
	global_world_manager::Release();
	gmatrix::DecRaidWorldInstance(_raid_id);
	gmatrix::CleanWorldActiveCtrlList(GetWorldTag());
	_status = RS_IDLE;
	__PRINTF("副本清理完毕，进入空闲状态RS_IDLE\n");
}

void raid_world_manager::Reset()
{
	_raid_result = 0;

	_max_player_count = 0;
	_need_level_min = 0;
	_need_level_max = 0;

	_raid_time = 1800;
	_invincible_time = 0;
	_close_raid_time = 0;
	_player_leave_time = 0;
	_win_ctrl_id = 0;
	_win_task_id = 0;
	memset(_forbidden_items_id, 0, sizeof(_forbidden_items_id));
	memset(_forbidden_skill_id, 0, sizeof(_forbidden_skill_id));

	_heartbeat_counter = 0;
	_seek_heartbeat_counter = 0;
	_raid_info_seq = 0;
	_update_info_seq = 0;	
	_apply_count = 0;

	_start_timestamp = -1;
	_raid_winner_id = 0;
	_max_killer_id = 0;

	_attacker_score = 0;
	_defender_score = 0;

	_attacker_data.Clear();
	_defender_data.Clear();

	_defender_list.clear();
	_attacker_list.clear();
	_all_list.clear();

	_raid_member_list.clear();
	_all_monster_killed = false;
	_global_var_list.clear();
	// Youshuang add
	_level_info.clear();
	for( std::multimap<int, raid_boss_info*>::iterator iter = _tid_2_bossinfo.begin(); iter != _tid_2_bossinfo.end(); ++iter )
	{
		if( !iter->second )
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
	_tid_2_bossinfo.clear();
	// end
}

void raid_world_manager::KillAllMonsters()
{	
	//需要残留怪物,物品等检测 全杀,策划保证战斗停止后所有控制器不会再触发	
	__PRINTF("副本(gs_id: %d,raid_id: %d,world_tag: %d) 杀死所有npc\n",		gmatrix::GetServerIndex(),_raid_id,GetWorldTag());	
	MSG msg;	
	BuildMessage(msg,GM_MSG_BATTLE_NPC_DISAPPEAR,XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
	std::vector<exclude_target> empty;
	_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF,empty);
}

void raid_world_manager::Heartbeat()
{
	if(_status == RS_READY || _status == RS_IDLE) return;

	global_world_manager::Heartbeat();
	++_heartbeat_counter; 
	++_seek_heartbeat_counter;
	if( (_heartbeat_counter) % 20 == 0 )
	{
		spin_autolock keeper( _raid_lock );
		switch ( _status )
		{	
			case RS_RUNNING:
			{
				OnRunning();
			}
			break;

			case RS_CLOSING:
			{
				// 这个期间踢出玩家
				// 在玩家heartbeat逻辑里完成规定时间内的踢人
				if( _kill_monster_timestamp < g_timer.get_systime() && !_all_monster_killed)
				{
					KillAllMonsters();
					_all_monster_killed = true;
				}
				if( _cur_timestamp < g_timer.get_systime() )
				{
					OnClosing();
					// 在这之前需要确保非gm玩家已经被踢出
					__PRINTF( "副本:%d，关闭，恢复到初始状态\n", GetWorldTag());
					Release();
				}
			}
			break;

			default:
			{
				__PRINTF( "ERROR! _status=%d\n", _status );
				ASSERT( false );
			}
		};
	}
	// 每30秒更新一次信息
	if( (_heartbeat_counter) % ( 30 * 20 ) == 0 )
	{
		SendRaidInfo();
		_heartbeat_counter = 0;
	}
}

void raid_world_manager::OnCreate()
{
}

void raid_world_manager::OnReady()
{
}

void raid_world_manager::OnRunning()
{
	// 战斗逻辑,如果时间到或者产生结果了
	if( _cur_timestamp < g_timer.get_systime() || _raid_result > 0)
	{
		OnClose();
		_status = RS_CLOSING;
		_cur_timestamp = g_timer.get_systime() + _close_raid_time;
		_kill_monster_timestamp = _cur_timestamp - 90;

		//KillAllMonsters();

		// 战斗结束时更新一次信息
		SendRaidInfo();

		// 计算战斗结果
		__PRINTF( "副本:%d 结束发布结果开始准备清理result=%d, close=%d\n", GetWorldTag(), _raid_result, _close_raid_time );
		GLog::log( GLOG_INFO, "副本结束,gs_id=%d, raid_world_tag=%d, raid_id=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _raid_id, _end_timestamp );
	}
}

void raid_world_manager::OnClose()
{
	if(_difficulty_ctrl_id > 0) ActiveSpawn( this, _difficulty_ctrl_id, false);
}

void raid_world_manager::SetRaidResult(int result)
{
	spin_autolock keeper( _raid_lock );
	_raid_result = result;
}

void raid_world_manager::OnClosing()
{
	GNET::SendRaidEnd( gmatrix::Instance()->GetServerIndex(), _raid_id, GetWorldTag(), 0); 
}

int raid_world_manager::OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm )
{
	if(_status != RS_RUNNING)
	{
		GLog::log(GLOG_INFO, "副本还没有开启, 玩家不能进入: roleid=%d, line_id=%d, raid_type=%d, raid_id=%d, raid_world_tag=%d, raid_status=%d",
				pInfo->id, gmatrix::Instance()->GetServerIndex(),GetRaidType(),_raid_id,GetWorldTag(),_status);
		return -1000;
	}
	return 0;
}

void raid_world_manager::PlayerEnter( gplayer* pPlayer, int faction, bool reenter)
{
	__PRINTF( "int raid_world_manager::PlayerEnter, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _raid_lock );
	AddMapNode( _all_list, pPlayer );
	gplayer_imp* _imp = (gplayer_imp*)pPlayer->imp;
	player_raid_info tempinfo;
	memset( &tempinfo, 0, sizeof(player_raid_info) );
	tempinfo.cls = pPlayer->GetClass();
	tempinfo.level = _imp->_basic.level;
	tempinfo.reborn_cnt = _imp->GetRebornCount();
	if( faction == RF_ATTACKER )
	{
		_attacker_data.player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode( _attacker_list, pPlayer );
	}
	else if( faction == RF_DEFENDER )
	{
		_defender_data.player_info_map[pPlayer->ID.id] = tempinfo;
		AddMapNode( _defender_list, pPlayer );
	}
	OnPlayerEnter(pPlayer, reenter, faction);
	SendRaidInfo();
}

void raid_world_manager::PlayerEnterWorld(gplayer* pPlayer, int faction, bool reenter)
{
	OnPlayerEnterWorld(pPlayer,reenter,faction);
}

void raid_world_manager::PlayerLeave( gplayer* pPlayer, int faction, bool cond_kick)
{
	__PRINTF( "int raid_world_manager::PlayerLeave, pPlayer=%p, faction=%d\n", pPlayer, faction);
	spin_autolock keeper( _raid_lock );
	DelMapNode( _all_list, pPlayer );
	PlayerInfoIt it;
	if( faction == RF_ATTACKER )
	{
		DelMapNode( _attacker_list, pPlayer );
		it = _attacker_data.player_info_map.find( pPlayer->ID.id );
		if( it != _attacker_data.player_info_map.end() )
		{
			_attacker_data.player_info_map.erase( it );
		}
	}
	else if( faction == RF_DEFENDER )
	{
		DelMapNode( _defender_list, pPlayer );
		it = _defender_data.player_info_map.find( pPlayer->ID.id );
		if( it != _defender_data.player_info_map.end() )
		{
			_defender_data.player_info_map.erase( it );
		}
	}
	OnPlayerLeave(pPlayer, cond_kick, faction);
	SendRaidInfo();
}

void raid_world_manager::DumpWorldMsg()
{
	__PRINTF( "-------------------- world message dump --------------------\n" );
	__PRINTF( "raid_id=%d, tag=%d, status=%d, result=%d\n", _raid_id, GetWorldTag(), _status, _raid_result );
	__PRINTF( "总人数:%d, 攻击方人数:%d, 防守方人数:%d\n",
		_attacker_data.player_info_map.size() + _defender_data.player_info_map.size(),
		_attacker_data.player_info_map.size(), _defender_data.player_info_map.size() ); 

	__PRINTF( "玩家信息:\n" );
	PlayerInfoIt it;
	for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
	{
		__PRINTF( "\tid=%d,cls=%d,level=%d,reborn_cnt=%d\n",
			it->first, it->second.cls, it->second.level, it->second.reborn_cnt);
	}
	for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
	{
		__PRINTF( "\tid=%d,cls=%d,level=%d,reborn_cnt=%d\n",
			it->first, it->second.cls, it->second.level, it->second.reborn_cnt);
	}
	__PRINTF( "\n" );
}

void raid_world_manager::RecordTownPos(const A3DVECTOR& pos,int faction)
{
	__PRINTF("注册回城点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//现在还无法区分攻方和攻方辅助方
		_attacker_data.town_list.push_back(pos);
	}       
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.town_list.push_back(pos);
	}       
	else
	{
		ASSERT(false && "注册回城点 faction错误 ");
	}
}

void raid_world_manager::RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id)
{
	__PRINTF("注册复活点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	competitor_data::revive_pos_t temp;
	temp.pos = pos;
	temp.active = 0;
	if(faction & FACTION_OFFENSE_FRIEND)
	{
		//允许同一控制器控制多个
		_attacker_data.revive_map[cond_id].push_back(temp);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.revive_map[cond_id].push_back(temp);
	}
	else
	{
		ASSERT(false && "注册复活点 faction错误 ");
	}
}

void raid_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{       
	__PRINTF("注册进入点: pos(%f,%f,%f),faction= %d\n",pos.x,pos.y,pos.z,faction);
	ASSERT(faction & (FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND));
	if(faction & FACTION_OFFENSE_FRIEND)
	{       
		//现在还无法区分攻方和攻方辅助方
		_attacker_data.entry_list.push_back(pos);
	}       
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_data.entry_list.push_back(pos);
	}
	else
	{
		ASSERT(false && "注册进入点 no faction");
	}       
}      

void raid_world_manager::RecordRaidBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building )
{
	__PRINTF( "RecordRaidBuilding pos(%f,%f,%f), faction=%d, tid=%d, key=%d\n", pos.x, pos.y, pos.z, faction, tid, is_key_building );
	ASSERT( faction & ( FACTION_OFFENSE_FRIEND | FACTION_DEFENCE_FRIEND ) );
	if( faction & FACTION_OFFENSE_FRIEND )
	{
		if( is_key_building )
		{
			++_attacker_data.buildings.key_building_init;
		}
		else
		{
			++_attacker_data.buildings.normal_building_init;
		}
	}
	else
	{
		if( is_key_building )
		{
			++_defender_data.buildings.key_building_init;
		}
		else
		{
			++_defender_data.buildings.normal_building_init;
		}
	}
}

bool raid_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	//获得随机复活点
	int raid_faction = pImp->GetRaidFaction();
	spin_autolock keeper(_raid_lock);
	std::vector<A3DVECTOR> revive_pos_list;
	competitor_data::CtrlReviveMapIt it;
	if(RF_ATTACKER == raid_faction)
	{
		for(it = _attacker_data.revive_map.begin();it != _attacker_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{
				//诛仙的战场里复活点控制器不用了,真晕-_-b
				revive_pos_list.push_back(it->second[i].pos);
			}
		}
	}
	else if(RF_DEFENDER == raid_faction)
	{
		for(it = _defender_data.revive_map.begin();it != _defender_data.revive_map.end(); ++it)
		{
			size_t size = it->second.size();
			for(size_t i = 0;i < size;++i)
			{       
				revive_pos_list.push_back(it->second[i].pos);
			}               
		}
	}
	else
	{
		return false;
	}
	size_t size = revive_pos_list.size();
	if(0 == size) return false;
	pos = revive_pos_list[abase::Rand(0,revive_pos_list.size() - 1)];
	world_tag = GetWorldTag();
	return true;
}

void raid_world_manager::GetRandomEntryPos(A3DVECTOR& pos,int raid_faction)
{
	pos = A3DVECTOR(0,0,0);
	spin_autolock keeper(_raid_lock);
	if(RF_ATTACKER == raid_faction)
	{       
		if(0 == _attacker_data.entry_list.size()) return;
		pos = _attacker_data.entry_list[abase::Rand(0,_attacker_data.entry_list.size() - 1)];
	}
	else if(RF_DEFENDER == raid_faction)
	{       
		if(0 == _defender_data.entry_list.size()) return;
		pos = _defender_data.entry_list[abase::Rand(0,_defender_data.entry_list.size() - 1)];
	}
	//gm和旁观者用这个
	else
	{
		if(0 == _defender_data.entry_list.size()) return;
		pos = _defender_data.entry_list[abase::Rand(0,_defender_data.entry_list.size() - 1)];
	}
} 
	
int raid_world_manager::TranslateCtrlID( int which )
{
	/*if( which >= CTRL_CONDISION_ID_RAID_MIN && which <= CTRL_CONDISION_ID_RAID_MAX )
	{
		return witch;
	}*/
	return which;
}

void raid_world_manager::SendRaidInfo()
{
	_raid_info_seq ++;
}

void raid_world_manager::HandleModifyRaidDeath( int player_id, int death_count, size_t content_length, const void *content)
{
	spin_autolock keeper( _raid_lock );
	if(RS_RUNNING != _status) return;
	/*PlayerInfoIt it = _attacker_data.player_info_map.find( player_id );
	if( it != _attacker_data.player_info_map.end() )
	{       
		++_defender_data.kill_count;
		it->second.death = death_count;
		UpdatePlayerInfo(it, content_length, content);
	}  
	else
	{
		it = _defender_data.player_info_map.find( player_id );
		if( it != _defender_data.player_info_map.end() )
		{       
			++_attacker_data.kill_count;
			it->second.death = death_count;
			UpdatePlayerInfo(it, content_length, content);
		}
	}
	SendRaidInfo();*/
}


void raid_world_manager::HandleModifyRaidKill( int player_id, int kill_count, size_t content_length, const void *content)
{
	spin_autolock keeper( _raid_lock );
	if(RS_RUNNING != _status) return;
	/*PlayerInfoIt it = _attacker_data.player_info_map.find( player_id );
	if( it != _attacker_data.player_info_map.end() )
	{       
		it->second.kill = kill_count;
		UpdatePlayerInfo(it, content_length, content);
	}  
	else
	{
		it = _defender_data.player_info_map.find( player_id );
		if( it != _defender_data.player_info_map.end() )
		{       
			it->second.kill = kill_count;
			UpdatePlayerInfo(it, content_length, content);
		}
	}
	SendRaidInfo();*/
}

void raid_world_manager::HandleSyncRaidInfo(int player_id, int reserver,  size_t content_length, const void *content)
{
	spin_autolock keeper( _raid_lock );
	if(RS_RUNNING != _status && RS_CLOSING != _status) return;
	
	PlayerInfoIt it = _attacker_data.player_info_map.find( player_id );
	if( it != _attacker_data.player_info_map.end() )
	{       
		UpdatePlayerInfo(it, content_length, content);
	}  
	else
	{
		it = _defender_data.player_info_map.find( player_id );
		if( it != _defender_data.player_info_map.end() )
		{       
			UpdatePlayerInfo(it, content_length, content);
		}
	}
	SendRaidInfo();
}

void raid_world_manager::RaidFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int emote_id, int self_id)
{
	if( faction & (FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND) )
	{
		multi_send_chat_msg( _attacker_list, buf, size, channel, emote_id, aux_data, dsize, self_id);
	}       
	else if( faction & (FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND) )
	{
		multi_send_chat_msg( _defender_list, buf, size, channel, emote_id, aux_data, dsize, self_id);
	}
}


void raid_world_manager::UpdatePlayerInfo(PlayerInfoIt &it, size_t content_length, const void * content)
{
	if(content_length == sizeof(player_raid_info))
	{
		it->second = *(player_raid_info*)content;
	}
}

void raid_world_manager::OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much )
{
	spin_autolock keeper( _raid_lock );
	// 检查第一次击杀情况,现在改成双方只有一个第一滴血了。。。。。
	if( deadman_battle_faction == RF_DEFENDER)
	{
		if( _attacker_data.first_kill )
		{
			_attacker_data.first_kill = false;
			_defender_data.first_kill = false;
			// 发送信息
			BroadcastRaidMsg( 0, 1, _raid_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
			__PRINTF( "在 %d 战场(%d线), %d 杀死了 %d, 获得第一滴血\n",_raid_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
		}
	}
	else if( deadman_battle_faction == RF_ATTACKER)
	{
		if( _defender_data.first_kill )
		{
			_defender_data.first_kill = false;
			_attacker_data.first_kill = false;
			// 发送信息
			BroadcastRaidMsg( 0, 2, _raid_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
			__PRINTF( "在 %d 战场(%d线), %d 杀死了 %d, 获得第一滴血\n",_raid_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
		}
	}
	// 检查阻止杀戮
	if( kill_much )
	{
		kill_much = false;
		// 发送信息
		BroadcastRaidMsg( 2, deadman_battle_faction == RF_DEFENDER?1:2,_raid_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
		__PRINTF( "在 %d 战场(%d线), %d 成功阻止了 %d 的杀戮\n",_raid_id, gmatrix::Instance()->GetServerIndex(), killer, deadman );
	}
}

int raid_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	npc_template * pTemplate = npc_stubs_manager::Get( tid );
	if( !pTemplate ) return 0;
	if( ROLE_IN_WAR_BATTLE_KEY_BUILDING == pTemplate->role_in_war )
	{
		OnDestroyKeyBuilding( faction );
	}
	else if( ROLE_IN_WAR_BATTLE_NORMAL_BUILDING == pTemplate->role_in_war) 
	{
		OnDestroyNormalBuilding( faction );
	}
	return 0; 
}

int raid_world_manager::OnDestroyKeyBuilding( int faction )
{
	spin_autolock keeper( _raid_lock );
	if( _raid_result > 0 || RS_RUNNING != _status ) return -1;
	if( faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
	{
		_raid_result = RAID_DEFENDER_WIN;
	}
	else
	{
		_raid_result = RAID_ATTACKER_WIN;
	}
	SendRaidInfo();
	return 0;
}

void raid_world_manager::BroadcastRaidMsg( unsigned char type, unsigned char raid_faction, int raid_id, int gs_id, int killer, int deadman )
{
#pragma pack(1)
	struct
	{
		unsigned char type;		// 类型 0 第一次击杀 1 杀神 2 阻止杀戮
		unsigned char raid_faction;	// killer的阵营 1 攻方 2 守方
		int raid_id;			// 战场id
		int gs_id;			// 线号
		int killer;			// 杀人玩家id
		int deadman;			// 被杀玩家id
	} data;
#pragma pack()
	data.type = type;
	data.raid_faction = raid_faction;
	data.raid_id = raid_id;
	data.gs_id = gs_id;
	data.killer = killer;
	data.deadman = deadman;
	multi_send_chat_msg( _all_list, &data,sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0,0, CHAT_RAID);
}

bool raid_world_manager::GetRaidInfo( int& info_seq, std::vector<player_info_raid>& info, bool bForce)
{
	spin_autolock keeper( _raid_lock );
	if( _raid_info_seq != info_seq || bForce )
	{
		info.clear();
		info_seq = _raid_info_seq;
	
		S2C::CMD::raid_info::player_info_in_raid temp;
		PlayerInfoIt it;
		for( it = _attacker_data.player_info_map.begin(); it != _attacker_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.cls = it->second.cls;
			temp.level = it->second.level;
			temp.reborn_cnt = it->second.reborn_cnt;
			info.push_back( temp );
		}
		for( it = _defender_data.player_info_map.begin(); it != _defender_data.player_info_map.end(); ++it )
		{
			temp.id = it->first;
			temp.cls = it->second.cls;
			temp.level = it->second.level;
			temp.reborn_cnt = it->second.reborn_cnt;
			info.push_back( temp );
		}
		return true;
	}
	return false;
}

void raid_world_manager::GetRaidUseGlobalVars(std::vector<int>& var_list)
{
	spin_autolock keeper( _raid_lock );
	for(size_t i= 0; i < GetRaidUseGlobalVarCnt(); i ++)
	{
		int key = GetRaidUseGlobalVar(i);	
		if(key >= GENERAL_INDEX_MAP_BEGIN && key <= GENERAL_INDEX_MAP_END)
		{
			var_list.push_back(key);
		}
	}
}

size_t raid_world_manager::GetRaidUseGlobalVarCnt()
{
	return _global_var_list.size();
}

int raid_world_manager::GetRaidUseGlobalVar(size_t index)
{
	if(index < 0 || index >= _global_var_list.size())
	{
		return -1;
	}
	return _global_var_list[index];
}

bool raid_world_manager::IsLimitSkillInWorld(int skill_id)
{
	for(int i = 0; i < 10; i ++)
	{
		if(_forbidden_skill_id[i] > 0 && _forbidden_items_id[i] == skill_id)
		{
			return true;
		}
	}
	return false;
}

bool raid_world_manager::IsForbiddenItem(int item_type)
{
	for(int i = 0; i < 10; i ++)
	{
		if(_forbidden_items_id[i] > 0 && _forbidden_items_id[i] == item_type)
		{
			return true;
		}
	}
	return false;
}

void raid_world_manager::OnActiveSpawn(int id,bool active)
{
	if(active)
	{
		if(id == _win_ctrl_id)
		{
			_raid_result = RAID_ATTACKER_WIN;
			GLog::log( GLOG_INFO, "副本激活胜利控制器ctrl_id=%d", id); 
		}
	}
	global_world_manager::OnActiveSpawn(id, active, true, GetWorldTag());
}

world_manager::INIT_RES raid_world_manager::OnInitMoveMap()
{
	raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
	if(!rwt)
	{
		__PRINTF("初始化副本NPC通路图失败，找不到副本模板%d\n", GetRaidID());
		return world_manager::INIT_FAIL;
	}
	__PRINTF("副本自己初始化 MoveMap\n");
	_movemap = rwt->GetMoveMap();
	return world_manager::INIT_SUCCESS;
}
world_manager::INIT_RES raid_world_manager::OnInitCityRegion()
{
	raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
	if(!rwt)
	{
		__PRINTF("初始化城市区域失败，找不到副本模板%d\n", GetRaidID());
		return world_manager::INIT_FAIL;
	}
	__PRINTF("副本自己初始化 CityRegion\n");
	_region = rwt->GetCityRegion();
	return world_manager::INIT_SUCCESS;
}
world_manager::INIT_RES raid_world_manager::OnInitBuffArea()
{
	raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
	if(!rwt)
	{
		__PRINTF("初始化BUFF区域失败，找不到副本模板%d\n", GetRaidID());
		return world_manager::INIT_FAIL;
	}
	__PRINTF("副本自己初始化 BuffArea\n");
	_buffarea = rwt->GetBuffArea();
	return world_manager::INIT_SUCCESS;
}
world_manager::INIT_RES raid_world_manager::OnInitPathMan()
{
	raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
	if(!rwt)
	{
		__PRINTF("初始化NPC线路失败，找不到副本模板%d\n", GetRaidID());
		return world_manager::INIT_FAIL;
	}
	__PRINTF("副本自己初始化 PathMan\n");
	_pathman = rwt->GetPathMan();
	return world_manager::INIT_SUCCESS;
}

bool raid_world_manager::OnTraceManAttach()
{
	raid_world_template* rwt = gmatrix::GetRaidWorldTemplate(GetRaidID());
	if(!rwt)
	{
		__PRINTF("副本加载共享凸包数据失败 %d\n", GetRaidID());
		return false;
	}
	__PRINTF("副本加载共享凸包数据\n");
	_trace_man.Attach(&rwt->GetTraceMan());
	return true;
}

void raid_world_manager::OnDisconnect()
{
	if (IsIdle())
		return;
	printf("##############找到副本%d,卸载实例###############\n", GetWorldTag());
	ForceCloseRaid();
}
// Youshuang add
raid_boss_info* raid_world_manager::GetBossByID( int tid, unsigned char killed ) const
{
	raid_boss_info* res = NULL;
	typedef std::multimap< int, raid_boss_info* >::const_iterator cit;
	typedef std::pair< cit, cit > Range;
	Range range = _tid_2_bossinfo.equal_range( tid );
	for( cit iter = range.first; iter != range.second; ++iter )
	{
		raid_boss_info* tmp = iter->second;
		if( !tmp || ( tmp->is_killed == killed ) )
		{
			continue;
		}
		if( !res )
		{
			res = tmp;
			continue;
		}
		int better = killed ? ( res->level - tmp->level ) : ( tmp->level - res->level );
		if( better > 0 )
		{
			res = tmp;
		}
	}
	return res;
}

std::vector<raid_boss_info> raid_world_manager::GetKilledBossList() const
{
	std::vector<raid_boss_info> res;
	int level_cnt = _level_info.size();
	for( int i = 0; i < level_cnt; ++i )
	{
		int boss_cnt_in_level = _level_info[i].boss.size();
		switch( _level_info[i].howto )
		{
			case LEVEL_OR:
			{
				raid_boss_info aboss;
				for( int j = 0; j < boss_cnt_in_level; ++j )
				{
					raid_boss_info* tmp = _level_info[i].boss[j];
					if( !tmp ){ continue; }
					if( tmp->is_killed == 1 )
					{
						aboss.is_killed = 1;
					}
					aboss.achievement = tmp->achievement;
				}
				res.push_back( aboss );
				break;
			}
			case LEVEL_AND:
			{
				raid_boss_info aboss;
				aboss.is_killed = 1;
				for( int j = 0; j < boss_cnt_in_level; ++j )
				{
					raid_boss_info* tmp = _level_info[i].boss[j];
					if( !tmp ){ continue; }
					if( tmp->is_killed != 1 )
					{
						aboss.is_killed = 0;
					}
					aboss.achievement = tmp->achievement;
				}
				res.push_back( aboss );
				break;
			}
			case LEVEL_ALL:
			{
				for( int j = 0; j < boss_cnt_in_level; ++j )
				{
					raid_boss_info* tmp = _level_info[i].boss[j];
					res.push_back( *tmp );
				}
				break;
			}
			default:
				break;
		}
	}
	return res;
}

void raid_world_manager::OnMobReborn( int tid )
{
	raid_boss_info* boss = GetBossByID( tid, 0 );
	if( boss )
	{
		boss->is_killed = 0;
	}
}

int raid_world_manager::NotifyBossBeenKilled()
{
	packet_wrapper tmp;
	using namespace S2C;
	CMD::Make<CMD::raid_boss_been_killed>::From( tmp, GetKilledBossList() );
	multi_send_ls_msg( _all_list, tmp );
	return 0;
}
// end
////////////////////////////////////////////////////////////////////////////////////////////////////
//普通副本逻辑
////////////////////////////////////////////////////////////////////////////////////////////////////
bool cr_world_manager::OnInit()
{
	return (0 == _attacker_data.buildings.normal_building_init || 0 == _attacker_data.buildings.key_building_init ||
		0 == _defender_data.buildings.normal_building_init || 0 == _defender_data.buildings.key_building_init ||
		_attacker_data.buildings.normal_building_init != _defender_data.buildings.normal_building_init ||
		_attacker_data.buildings.key_building_init != _defender_data.buildings.key_building_init );
}

int cr_world_manager::OnDestroyNormalBuilding( int faction )
{
	spin_autolock keeper( _raid_lock );
	if( _raid_result || RS_RUNNING != _status ) return -1;
	if( faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
	{
		--_attacker_data.buildings.normal_building_left;
		if( _attacker_data.buildings.normal_building_left <= 0 )
		{
			ActiveSpawn( this, CTRL_CONDISION_ID_RAID_ATTACKER_KEY, true );
		}
	}
	else
	{
		--_defender_data.buildings.normal_building_left;
		if( _defender_data.buildings.normal_building_left <= 0 )
		{
			ActiveSpawn( this, CTRL_CONDISION_ID_RAID_DEFENDER_KEY, true );
		}
	}
	SendRaidInfo();
	return 0;
}

void cr_world_manager::OnCreate()
{
	raid_world_manager::OnCreate();
	ActiveSpawn( this, CTRL_CONDISION_ID_RAID_BEGIN, true ); 
}

void cr_world_manager::OnClosing()
{
	raid_world_manager::OnClosing();
	//ActiveSpawn( this, CTRL_CONDISION_ID_RAID_BEGIN, false );
	//ActiveSpawn( this, CTRL_CONDISION_ID_RAID_ATTACKER_KEY, false );
	//ActiveSpawn( this, CTRL_CONDISION_ID_RAID_DEFENDER_KEY, false );
	if(0 == _raid_result) _raid_result = RAID_DRAW;
}

// Youshuang add
int cr_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	raid_world_manager::OnMobDeath( pPlane, faction, tid, pos, attacker_id );
	raid_boss_info* boss = GetBossByID( tid, 1 );
	if( !boss )
	{
		return -1;
	}
	boss->is_killed = 1;
	return NotifyBossBeenKilled();
};
// end
