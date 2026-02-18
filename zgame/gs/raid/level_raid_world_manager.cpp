#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conf.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <glog.h>
#include <algorithm>
#include <sstream>

#include "level_raid_world_manager.h"
#include "../faction.h"


level_raid_world_manager::level_raid_world_manager()
{
	_level_config_tid = 0;
	_final_ctrl = 0;
	memset(_final_conds, 0, sizeof(_conds));	
	_final_result = false;
	_final_win_ctrl_id = -1;
	_curr_level_ctrl = 0;
	_curr_level_index = -1;
	memset(_conds, 0, sizeof(_conds));
	memset(_level_result, 0, sizeof(_level_result));
	_curr_level_win_ctrl_id = -1;
}

void level_raid_world_manager::Reset()
{
	raid_world_manager::Reset();

	_level_config_tid = 0;
	_final_ctrl = 0;
	_final_result = false;
	_final_win_ctrl_id = -1;
	_curr_level_ctrl = 0;
	_curr_level_index = -1;
	memset(_level_result, 0, sizeof(_level_result));
	_curr_level_win_ctrl_id = -1;
	ClearFinalConditions();
	ClearLevelConditions();
	_start_hidden_level = 0;  // Youshuang add
}

int level_raid_world_manager::CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	_level_config_tid = rwt.level_config_tid;
	_start_hidden_level = 0;  // Youshuang add
	return raid_world_manager::CreateRaid(raidroom_id, rwt, tag, index, roleid_list, buf, size, difficulty);
}

void level_raid_world_manager::OnActiveSpawn(int id,bool active)
{
	raid_world_manager::OnActiveSpawn(id, active);
	if(active)
	{
		__PRINTF("Active level raid spawn: id=%d\n", id);
		if(_final_ctrl > 0 && _final_conds[LEVEL_CONDITION_TYPE_CTRLACTIVE])
		{
			_final_conds[LEVEL_CONDITION_TYPE_CTRLACTIVE]->SetValue(id);
		}
		if(_curr_level_ctrl > 0 && _conds[LEVEL_CONDITION_TYPE_CTRLACTIVE])
		{
			_conds[LEVEL_CONDITION_TYPE_CTRLACTIVE]->SetValue(id);
		}
		AddLevelConditions(id);

		if(id == 82706)
		{
			GLog::log( GLOG_INFO, "关卡副本关卡82706开启，raid_id=%d, 玩家[%s]\n", _raid_id, GetAllPlayerIdsStr().c_str());
			_start_hidden_level = 1;
			NotifyBossBeenKilled(); // Youshuang add
		}
	}
	else
	{
		__PRINTF("Deactive level raid spawn: id=%d\n", id);
		if(id == _final_ctrl)
		{
			bool meet = true;
			for(int i = 0; i < LEVEL_CONDITION_NUM; i ++)
			{
				if(_final_conds[i] && !_final_conds[i]->Meet())
				{
					meet = false;
					break;
				}
			}
			if(meet)
			{
				if(_final_win_ctrl_id > 0) 
				{
					ActiveSpawn( this, _final_win_ctrl_id, true );
				}
				GLog::log( GLOG_INFO, "关卡副本总挑战达成，raid_id=%d, 玩家[%s], 控制器ID=%d, 打开胜利控制器Id=%d\n", _raid_id, GetAllPlayerIdsStr().c_str(), id, _final_win_ctrl_id);
				__PRINTF("Final level meet condition, open win_ctrl=%d\n", _final_win_ctrl_id);
			}
			else
			{
				GLog::log( GLOG_INFO, "关卡副本总挑战失败，raid_id=%d, 玩家[%s], 控制器ID=%d\n", _raid_id, GetAllPlayerIdsStr().c_str(), id);
			}
			SendRaidLevelStatusChange(-1, false, meet);
			_final_result = meet;
			_final_ctrl = 0;
			ClearFinalConditions();
		}
		else if(id == _curr_level_ctrl)
		{
			bool meet = true;
			for(int i = 0; i < LEVEL_CONDITION_NUM; i ++)
			{
				if(_conds[i] && !_conds[i]->Meet())
				{
					meet = false;
					break;
				}
			}
			if(meet)
			{
				if(_curr_level_win_ctrl_id > 0) 
				{
					ActiveSpawn( this, _curr_level_win_ctrl_id, true );
				}
				GLog::log( GLOG_INFO, "关卡副本关卡%d挑战达成，raid_id=%d, 玩家[%s], 控制器ID=%d, 打开胜利控制器ID=%d\n", _curr_level_ctrl, _raid_id, GetAllPlayerIdsStr().c_str(), id, _curr_level_win_ctrl_id);
				__PRINTF("Level %d meet condition, open win_ctrl=%d\n", _curr_level_index, _curr_level_win_ctrl_id);
			}
			else
			{
				GLog::log( GLOG_INFO, "关卡副本关卡%d挑战失败，raid_id=%d, 玩家[%s], 控制器ID=%d\n", _curr_level_ctrl, _raid_id, GetAllPlayerIdsStr().c_str(), id);
			}
			SendRaidLevelStatusChange(_curr_level_index, false, meet);
			_level_result[_curr_level_index] = meet;
			_curr_level_ctrl = 0;
			_curr_level_index = -1;
			ClearLevelConditions();
		}
		NotifyBossBeenKilled(); // Youshuang add
	}
}

void level_raid_world_manager::OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much)
{
	raid_world_manager::OnPlayerDeath(pPlayer, killer, deadman, deadman_battle_faction, kill_much);
	spin_autolock keeper(_raid_lock);
	if(_final_ctrl && _final_conds[LEVEL_CONDITION_TYPE_DEATH])
	{
		_final_conds[LEVEL_CONDITION_TYPE_DEATH]->IncValue(1);
	}
	if(_curr_level_ctrl && _conds[LEVEL_CONDITION_TYPE_DEATH])
	{
		_conds[LEVEL_CONDITION_TYPE_DEATH]->IncValue(1);
	}
}

void level_raid_world_manager::OnKillMonster(gplayer* pPlayer, int monster_tid, int faction)
{
	raid_world_manager::OnKillMonster(pPlayer, monster_tid, faction);
	spin_autolock keeper(_raid_lock);
	if(_final_ctrl && _final_conds[LEVEL_CONDITION_TYPE_NPCKILL])
	{
		_final_conds[LEVEL_CONDITION_TYPE_NPCKILL]->IncValue(monster_tid, 1);
	}
	if(_curr_level_ctrl && _conds[LEVEL_CONDITION_TYPE_NPCKILL])
	{
		_conds[LEVEL_CONDITION_TYPE_NPCKILL]->IncValue(monster_tid, 1);
	}
}

void level_raid_world_manager::OnUsePotion(gplayer* pPlayer, char type, int value)
{
	raid_world_manager::OnUsePotion(pPlayer, type, value);
	spin_autolock keeper(_raid_lock);
	if(_final_ctrl && _final_conds[type == 0 ? LEVEL_CONDITION_TYPE_USEHPPOTION : LEVEL_CONDITION_TYPE_USEMPPOTION])
	{
		_final_conds[type == 0 ? LEVEL_CONDITION_TYPE_USEHPPOTION : LEVEL_CONDITION_TYPE_USEMPPOTION]->IncValue(value);
	}
	if(_curr_level_ctrl && _conds[type == 0 ? LEVEL_CONDITION_TYPE_USEHPPOTION : LEVEL_CONDITION_TYPE_USEMPPOTION])
	{
		_conds[type == 0 ? LEVEL_CONDITION_TYPE_USEHPPOTION : LEVEL_CONDITION_TYPE_USEMPPOTION]->IncValue(value);
	}
}

void level_raid_world_manager::OnGatherMatter(gplayer* pPlayer, int matter_tid, int cnt)
{
	raid_world_manager::OnGatherMatter(pPlayer, matter_tid, cnt);
	spin_autolock keeper(_raid_lock);
	if(_final_ctrl && _final_conds[LEVEL_CONDITION_TYPE_GATHER])
	{
		_final_conds[LEVEL_CONDITION_TYPE_GATHER]->IncValue(matter_tid, cnt);
		SendRaidLevelInfo(true);
	}
	if(_curr_level_ctrl && _conds[LEVEL_CONDITION_TYPE_GATHER])
	{
		_conds[LEVEL_CONDITION_TYPE_GATHER]->IncValue(matter_tid, cnt);
		SendRaidLevelInfo(false);
	}
}

void level_raid_world_manager::GetRaidLevelResult(bool& final, bool level_result[10])
{
	final = _final_result;
	memcpy(level_result, _level_result, sizeof(_level_result));
}

int  level_raid_world_manager::GetRaidLevelConfigTid()
{
	return _level_config_tid;
}

void level_raid_world_manager::SendRaidLevelInfo(int playerId, bool final)
{
	msg_level_raid_info info;
	memset(&info, 0, sizeof(info));
	if(final)
	{
		info.level = -1;
		if(_final_conds[LEVEL_CONDITION_TYPE_GATHER])
		{
			info.matter_cnt = _final_conds[LEVEL_CONDITION_TYPE_GATHER]->GetValue();
		}
		if(_final_conds[LEVEL_CONDITION_TYPE_TIME])
		{
			info.start_time = _final_conds[LEVEL_CONDITION_TYPE_TIME]->GetValue();
		}
	}
	else if(_curr_level_index >= 0)
	{
		info.level = _curr_level_index;
		if(_conds[LEVEL_CONDITION_TYPE_GATHER])
		{
			info.matter_cnt = _final_conds[LEVEL_CONDITION_TYPE_GATHER]->GetValue();
		}
		if(_conds[LEVEL_CONDITION_TYPE_TIME])
		{
			info.start_time = _final_conds[LEVEL_CONDITION_TYPE_TIME]->GetValue();
		}
	}
	else
	{
		return;
	}

	MSG msg;
	BuildMessage(msg, GM_MSG_LEVEL_RAID_INFO_CHANGE, XID(GM_TYPE_PLAYER, playerId), XID(-1, -1), A3DVECTOR(0,0,0), 0, &info, sizeof(info));
	gmatrix::SendMessage(msg);
}

void level_raid_world_manager::SendRaidLevelInfo(bool final)
{
	msg_level_raid_info info;
	memset(&info, 0, sizeof(info));
	if(final)
	{
		info.level = -1;
		if(_final_conds[LEVEL_CONDITION_TYPE_GATHER])
		{
			info.matter_cnt = _final_conds[LEVEL_CONDITION_TYPE_GATHER]->GetValue();
		}
		if(_final_conds[LEVEL_CONDITION_TYPE_TIME])
		{
			info.start_time = _final_conds[LEVEL_CONDITION_TYPE_TIME]->GetValue();
		}
	}
	else if(_curr_level_index > 0)
	{
		info.level = _curr_level_index;
		if(_conds[LEVEL_CONDITION_TYPE_GATHER])
		{
			info.matter_cnt = _final_conds[LEVEL_CONDITION_TYPE_GATHER]->GetValue();
		}
		if(_conds[LEVEL_CONDITION_TYPE_TIME])
		{
			info.start_time = _final_conds[LEVEL_CONDITION_TYPE_TIME]->GetValue();
		}
	}
	else
	{
		return;
	}
	PlayerInfoMap::iterator it = _attacker_data.player_info_map.begin();
	for(; it != _attacker_data.player_info_map.end(); ++ it)
	{
		MSG msg;
		BuildMessage(msg, GM_MSG_LEVEL_RAID_INFO_CHANGE, XID(GM_TYPE_PLAYER, it->first), XID(-1, -1), A3DVECTOR(0,0,0), 0, &info, sizeof(info));
		gmatrix::SendMessage(msg);
	}
	it = _defender_data.player_info_map.begin();
	for(; it != _defender_data.player_info_map.end(); ++ it)
	{
		MSG msg;
		BuildMessage(msg, GM_MSG_LEVEL_RAID_INFO_CHANGE, XID(GM_TYPE_PLAYER, it->first), XID(-1, -1), A3DVECTOR(0,0,0), 0, &info, sizeof(info));
		gmatrix::SendMessage(msg);
	}
}

void level_raid_world_manager::ClearFinalConditions()
{
	for(int i = 0; i < LEVEL_CONDITION_NUM; i ++)
	{
		if(_final_conds[i])
		{
			delete _final_conds[i];
			_final_conds[i] = 0;
		}
	}
}

void level_raid_world_manager::ClearLevelConditions()
{
	for(int i = 0; i < LEVEL_CONDITION_NUM; i ++)
	{
		if(_conds[i])
		{
			delete _conds[i];
			_conds[i] = 0;
		}
	}
}

void level_raid_world_manager::AddLevelConditions(int ctrl_id)
{
	if(_final_ctrl > 0 && _curr_level_ctrl > 0)
	{
		return;
	}
	DATA_TYPE dt;
	const CHALLENGE_2012_CONFIG& config = *(const CHALLENGE_2012_CONFIG*) gmatrix::GetDataMan().get_data_ptr(_level_config_tid, ID_SPACE_CONFIG, dt);
	if(dt != DT_CHALLENGE_2012_CONFIG || &config == NULL)
	{
		__PRINTF("无效的副本条件检测模板: %d\n", _level_config_tid);
		GLog::log( GLOG_INFO, "原因：副本条件检测模板不存在raid_id=%d, _status=%d, level_config_tid=%d\n", _raid_id, _status, _level_config_tid);
		return;
	}	
	if(ctrl_id == config.main_config.controler_id && _final_ctrl == 0)
	{
		_final_ctrl = ctrl_id;
		_final_win_ctrl_id = config.main_config.win_control_id;
		AddLevelConditions(config.main_config, true);
		SendRaidLevelStatusChange(-1, true, false);
		return;
	}

	if(_curr_level_ctrl == 0)
	{
		for(size_t i = 0; i < sizeof(config.lv_config) / sizeof(CHALLENGE_2012_CONFIG::CONFIG_TMPL); i ++)
		{
			if(ctrl_id == config.lv_config[i].controler_id)
			{
				_curr_level_ctrl = ctrl_id;
				_curr_level_win_ctrl_id = config.lv_config[i].win_control_id;
				_curr_level_index = i;
				AddLevelConditions(config.lv_config[i], false);
				SendRaidLevelStatusChange(i, true, false);
				return;
			}
		}
	}
}

void level_raid_world_manager::AddLevelConditions(const CHALLENGE_2012_CONFIG::CONFIG_TMPL& config, bool final)
{
	level_condition** cond = final ? _final_conds : _conds;
	if(config.monster_id > 0 && config.monster_count_min >= 0 && config.monster_count_max >= 0 && config.monster_count_max >= config.monster_count_min)
	{
		cond[LEVEL_CONDITION_TYPE_NPCKILL] = new level_condition_npc_kill(config.controler_id, config.monster_id, config.monster_count_max, config.monster_count_min);
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_NPCKILL\n");
	}
	if(config.mine_id > 0 && config.mine_count_min >= 0 && config.mine_count_max >= 0 && config.mine_count_max >= config.mine_count_min)
	{
		cond[LEVEL_CONDITION_TYPE_GATHER] = new level_condition_gather(config.controler_id, config.mine_id, config.mine_count_max, config.mine_count_min);
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_GATHER\n");
	}
	if(config.dead_count > 0)
	{
		cond[LEVEL_CONDITION_TYPE_DEATH] = new level_condition_death(config.controler_id, config.dead_count);
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_DEATH\n");
	}
	if(config.time_limit > 0)
	{
		cond[LEVEL_CONDITION_TYPE_TIME] = new level_condition_time(config.controler_id, config.time_limit);
		cond[LEVEL_CONDITION_TYPE_TIME]->Start();
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_TIME\n");
	}
	if(config.medic_limit_hp > 0)
	{
		cond[LEVEL_CONDITION_TYPE_USEHPPOTION] = new level_condition_use_hp_potion(config.controler_id, config.medic_limit_hp);
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_NPCKILL\n");
	}
	if(config.medic_limit_mp > 0)
	{
		cond[LEVEL_CONDITION_TYPE_USEMPPOTION] = new level_condition_use_mp_potion(config.controler_id, config.medic_limit_mp);
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_USEMPPOTION\n");
	}
	if(config.controller_id_challenge > 0 && config.controller_id_min >= 0 && config.controller_id_max >= 0 && config.controller_id_max >= config.controller_id_min)
	{
		cond[LEVEL_CONDITION_TYPE_GLOBALVALUE] = new level_condition_global_value(config.controler_id, config.controller_id_challenge, config.controller_id_max, config.controller_id_min, GetWorldTag(), GetWorldIndex());
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_GLOBALVALUE\n");
	}
	if(config.controller_id_win)
	{
		cond[LEVEL_CONDITION_TYPE_CTRLACTIVE] = new level_condition_ctrl_active(config.controler_id, config.controller_id_win);
		__PRINTF("Add Level Condition, LEVEL_CONDITION_TYPE_CTRLACTIVE\n");
	}
}

void level_raid_world_manager::SendRaidLevelStatusChange(int playerId, int level, bool start, bool result)
{
	if(start)
	{
		MSG msg;
		BuildMessage(msg, GM_MSG_LEVEL_RAID_START, XID(GM_TYPE_PLAYER, playerId), XID(-1, -1), A3DVECTOR(0,0,0), level);
		gmatrix::SendMessage(msg);
	}
	else
	{

		MSG msg;
		BuildMessage2(msg, GM_MSG_LEVEL_RAID_END, XID(GM_TYPE_PLAYER, playerId), XID(-1, -1), A3DVECTOR(0,0,0), level, result);
		gmatrix::SendMessage(msg);
	}
}

void level_raid_world_manager::SendRaidLevelStatusChange(int level, bool start, bool result)
{
	if(start)
	{
		PlayerInfoMap::iterator it = _attacker_data.player_info_map.begin();
		for(; it != _attacker_data.player_info_map.end(); ++ it)
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_LEVEL_RAID_START, XID(GM_TYPE_PLAYER, it->first), XID(-1, -1), A3DVECTOR(0,0,0), level);
			gmatrix::SendMessage(msg);
		}
		it = _defender_data.player_info_map.begin();
		for(; it != _defender_data.player_info_map.end(); ++ it)
		{
			MSG msg;
			BuildMessage(msg, GM_MSG_LEVEL_RAID_START, XID(GM_TYPE_PLAYER, it->first), XID(-1, -1), A3DVECTOR(0,0,0), level);
			gmatrix::SendMessage(msg);
		}
	}
	else
	{
		PlayerInfoMap::iterator it = _attacker_data.player_info_map.begin();
		for(; it != _attacker_data.player_info_map.end(); ++ it)
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_LEVEL_RAID_END, XID(GM_TYPE_PLAYER, it->first), XID(-1, -1), A3DVECTOR(0,0,0), level, result);
			gmatrix::SendMessage(msg);
		}
		it = _defender_data.player_info_map.begin();
		for(; it != _defender_data.player_info_map.end(); ++ it)
		{
			MSG msg;
			BuildMessage2(msg, GM_MSG_LEVEL_RAID_END, XID(GM_TYPE_PLAYER, it->first), XID(-1, -1), A3DVECTOR(0,0,0), level, result);
			gmatrix::SendMessage(msg);
		}
	}
}

bool level_raid_world_manager::OnInit()
{
	return (0 == _attacker_data.buildings.normal_building_init || 0 == _attacker_data.buildings.key_building_init ||
		0 == _defender_data.buildings.normal_building_init || 0 == _defender_data.buildings.key_building_init ||
		_attacker_data.buildings.normal_building_init != _defender_data.buildings.normal_building_init ||
		_attacker_data.buildings.key_building_init != _defender_data.buildings.key_building_init );
}

int level_raid_world_manager::OnDestroyNormalBuilding( int faction )
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

void level_raid_world_manager::OnCreate()
{
	raid_world_manager::OnCreate();
	ActiveSpawn( this, CTRL_CONDISION_ID_RAID_BEGIN, true ); 
}

void level_raid_world_manager::OnClosing()
{
	raid_world_manager::OnClosing();
	//ActiveSpawn( this, CTRL_CONDISION_ID_RAID_BEGIN, false );
	//ActiveSpawn( this, CTRL_CONDISION_ID_RAID_ATTACKER_KEY, false );
	//ActiveSpawn( this, CTRL_CONDISION_ID_RAID_DEFENDER_KEY, false );
	if(0 == _raid_result) _raid_result = RAID_DRAW;
}

void level_raid_world_manager::OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction)
{
	if(_final_ctrl > 0)
	{
		SendRaidLevelStatusChange(pPlayer->ID.id, -1, true, false);
		SendRaidLevelInfo(pPlayer->ID.id, true);
	}
	if(_curr_level_ctrl > 0)
	{
		SendRaidLevelStatusChange(pPlayer->ID.id, _curr_level_index, true, false);
		SendRaidLevelInfo(pPlayer->ID.id, false);
	}
}

std::string level_raid_world_manager::GetAllPlayerIdsStr()
{
	std::stringstream ss;
	PlayerInfoMap::iterator it = _attacker_data.player_info_map.begin();
	for(; it != _attacker_data.player_info_map.end(); ++ it)
	{
		ss << it->first << ",";
	}
	it = _defender_data.player_info_map.begin();
	for(; it != _defender_data.player_info_map.end(); ++ it)
	{
		ss << it->first << ",";
	}
	return ss.str();
}

// Youshuang add
int level_raid_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	raid_world_manager::OnMobDeath( pPlane, faction, tid, pos, attacker_id );
        raid_boss_info* boss = GetBossByID( tid, 1 );
	if( !boss ) 
        {       
               return -1;
        }       
        boss->is_killed = 1;
	return NotifyBossBeenKilled();
}

std::vector<raid_boss_info> level_raid_world_manager::GetKilledBossList() const
{
	std::vector<raid_boss_info> res = raid_world_manager::GetKilledBossList();
	int cnt = res.size();
	cnt = ( cnt < 10 ) ? cnt : 10;
	for( int i = 0; i < cnt; ++i )
	{
		res[i].achievement = ( _level_result[i] ) ? 2 : 1;
	}
	if( _raid_id == 538 )
	{
		raid_boss_info aboss;
		aboss.achievement = 0;
		aboss.is_killed = _start_hidden_level;
		res.push_back( aboss );
	}
	return res;
}
// end
