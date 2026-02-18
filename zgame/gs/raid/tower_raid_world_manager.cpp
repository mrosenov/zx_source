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

#include "tower_raid_world_manager.h"
#include "../template/itemdataman.h"
#include "../template/npcgendata.h"
#include "raid_player.h"
#include "raid_world_manager.h"
#include "raid_world_message_handler.h"
#include <factionlib.h>
#include "../clstab.h"
#include "../npc.h"


tower_raid_world_manager::tower_raid_world_manager()
{
	_tower_opened_level = 0;
	_init_tower_level = 0;
	_curr_tower_level = 0;
	memset(_level_tids, 0, sizeof(_level_tids));

	_monster_result_send_cnt = 0;
	_monster_cnt = 0;
	memset(_monsters, 0, sizeof(_monsters));
	memset(_rand_send_client, 0, sizeof(_rand_send_client));
	_level_victory_controller = 0;
	_actived_controller = 0;
	_reset_monster_wait_time = -1;
}

tower_raid_world_manager::~tower_raid_world_manager()
{
}

int tower_raid_world_manager::TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	if(_status != RS_IDLE)
	{
		return -1;
	}
	if(size != sizeof(tower_raid_data))
	{
		return -2;
	}

	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_CONFIG& raid_config = *(const TOWER_TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt.rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		__PRINTF("无效的副本模板: %d\n", rwt.rwinfo.raid_template_id);
		GLog::log( GLOG_INFO, "副本开启失败,原因：副本模板不存在 raid_template_id=%d, raid_id=%d, _status=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status );
		return -2;
	}	

	if(raid_config.tower_layer_count > MAX_TOWER_RAID_LEVEL)
	{
		__PRINTF("模板爬塔副本层数%d超过服务器设定:%d\n", raid_config.tower_layer_count, MAX_TOWER_RAID_LEVEL);
		GLog::log( GLOG_INFO, "模板爬塔副本层数%d超过服务器设定:%d, raid_template_id=%d, raid_id=%d, _status=%d\n", raid_config.tower_layer_count, MAX_TOWER_RAID_LEVEL, rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status);
		return -2;
	}

	const tower_raid_data* trd = (const tower_raid_data*)buf;
	_init_tower_level = trd->init_tower_level;
	if(_init_tower_level < 0 || _init_tower_level >= raid_config.tower_layer_count || _init_tower_level % raid_config.tower_pass_count != 0 || _init_tower_level > 60)
	{
		__PRINTF("玩家选择初始化层数错误 _init_tower_level=%d, raid_template_id=%d, raid_id=%d\n", _init_tower_level, rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
		GLog::log( GLOG_INFO, "玩家选择初始化层数错误 _init_tower_level=%d, raid_template_id=%d, raid_id=%d\n", _init_tower_level, rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
		return -2;
	}
	return 0;
}

int tower_raid_world_manager::CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
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
		return -10015;
	}
	__PRINTF( "副本创建请求, raid_template_id=%d, raid_id=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
	
	if(size != sizeof(tower_raid_data))
	{
		return -10010;
	}

	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_CONFIG& raid_config = *(const TOWER_TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt.rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		__PRINTF("无效的副本模板: %d\n", rwt.rwinfo.raid_template_id);
		GLog::log( GLOG_INFO, "副本开启失败,原因：副本模板不存在 raid_template_id=%d, raid_id=%d, _status=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status );
		return -10012;
	}	

	if(raid_config.tower_layer_count > MAX_TOWER_RAID_LEVEL)
	{
		__PRINTF("模板爬塔副本层数%d超过服务器设定:%d\n", raid_config.tower_layer_count, MAX_TOWER_RAID_LEVEL);
		GLog::log( GLOG_INFO, "模板爬塔副本层数%d超过服务器设定:%d, raid_template_id=%d, raid_id=%d, _status=%d\n", raid_config.tower_layer_count, MAX_TOWER_RAID_LEVEL, rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status);
		return -10013;
	}

	const tower_raid_data* trd = (const tower_raid_data*)buf;
	_init_tower_level = trd->init_tower_level;
	if(_init_tower_level < 0 || _init_tower_level >= raid_config.tower_layer_count || _init_tower_level % raid_config.tower_pass_count != 0)
	{
		__PRINTF("玩家选择初始化层数错误 _init_tower_level=%d, raid_template_id=%d, raid_id=%d\n", _init_tower_level, rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
		GLog::log( GLOG_INFO, "玩家选择初始化层数错误 _init_tower_level=%d, raid_template_id=%d, raid_id=%d\n", _init_tower_level, rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
		return -10014;
	}
	_max_player_count = raid_config.max_player_num;
	_need_level_min = raid_config.player_min_level;
	_need_level_max = raid_config.player_max_level;

	_raid_time = raid_config.total_exist_time;
	_invincible_time = raid_config.invincible_time;
	_close_raid_time = 300;
	_player_leave_time = 180;

	_win_ctrl_id = raid_config.controller_id;
	_win_task_id = raid_config.award_task_id;

	memcpy(_forbidden_items_id, raid_config.forbiddon_items_id, sizeof(_forbidden_items_id));
	memcpy(_forbidden_skill_id, raid_config.forbiddon_skill_id, sizeof(_forbidden_skill_id));

	//初始化塔相关的数据
	_tower_opened_level = raid_config.opened_tower_layer_num;
	for(size_t i = 0; i < sizeof(raid_config.enable_skill_id) / sizeof(int); i++)
	{
		if(raid_config.enable_skill_id[i] > 0)
		{
			_addition_skill_list.push_back(raid_config.enable_skill_id[i]);
		}
	}
	memcpy(_level_tids, raid_config.property_config_id, sizeof(_level_tids));
	
	// 判断时间戳
	if( _raid_time < 0 )
	{
		__PRINTF( "raid_time参数有误，持续时间为负 raid_time=%d\n", _raid_time);
		GLog::log( GLOG_INFO, "副本开启失败,原因：持续时间为负数 raid_template_id=%d, raid_id=%d, time=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _raid_time);
		return -10016;
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

	OnCreate();
	_status = RS_RUNNING;

	__PRINTF( "副本:%d:%d开启,持续时间:%d\n", _raid_id, GetWorldTag(), _raid_time); 
	GLog::log( GLOG_INFO, "副本开启成功 raid_template_id=%d, raid_id=%d, end_time=%d\n", rwt.rwinfo.raid_template_id,_raid_id, _end_timestamp);
	return 0;
}

void tower_raid_world_manager::Reset()
{
	raid_world_manager::Reset();
	_tower_opened_level = 0;
	_init_tower_level = 0;
	_curr_tower_level = 0;
	memset(_level_tids, 0, sizeof(_level_tids));

	_monster_result_send_cnt = 0;
	_monster_cnt = 0;
	memset(_monsters, 0, sizeof(_monsters));
	memset(_rand_send_client, 0, sizeof(_rand_send_client));
	_level_victory_controller = 0;
	_actived_controller = 0;
	_addition_skill_list.clear();
	_tower_player_info_map.clear();
	ActiveSpawn( this, _actived_controller, false);
	_reset_monster_wait_time = -1;
}

void tower_raid_world_manager::OnKillMonster(gplayer* pPlayer, int monster_tid, int faction)
{
	spin_autolock keeper(_raid_lock);
	if(monster_tid <= 0)
	{
		return;
	}
	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& config  = *(const TOWER_TRANSCRIPTION_PROPERTY_CONFIG*) gmatrix::GetDataMan().get_data_ptr(_level_tids[_curr_tower_level], ID_SPACE_CONFIG, dt);
	bool effect_level = false;
	if (&config)
	{
		effect_level = false;
		for(size_t i = 0; i < sizeof(config.monster_id_list) / sizeof(int); i ++)
		{
			if (monster_tid == config.monster_id_list[i])
			{
				effect_level = true;
				break;
			}
		}
	}
	if (!effect_level) return;
	for(int i = 0; i < _monster_cnt; i ++)
	{
		if(_monsters[i].monster_tid == monster_tid && _monsters[i].monster_state != MONSTER_STATE_KILLED)
		{
			_monsters[i].monster_state = MONSTER_STATE_KILLED;
			__PRINTF("怪物%d都被玩家杀死，_monster_cnt:%d\n", monster_tid, _monster_cnt);
			break;
		}
	}


	bool allKilled = true;
	for(int i = 0; i < _monster_cnt; i ++)
	{
		if(_monsters[i].monster_state != MONSTER_STATE_KILLED)
		{
			allKilled = false;
			break;
		}
	}

	if(allKilled)
	{
		__PRINTF("所有怪物都被玩家杀死，准备下一层, curr_tower_level:%d\n", _curr_tower_level);
		_monster_result_send_cnt = 0;
		_monster_cnt = 0;
		memset(_monsters, 0, sizeof(_monsters));
		memset(_rand_send_client, 0, sizeof(_rand_send_client));

		gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
		pImp->TowerLevelPass(_curr_tower_level, g_timer.get_systime() - _level_start_time);
		pImp->SetEndLevelToday(_curr_tower_level);
		if(_curr_tower_level == _tower_opened_level - 1)
		{
			if( _raid_result > 0 || RS_RUNNING != _status ) return;
			if( faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
			{
				_raid_result = RAID_DEFENDER_WIN;
			}
			else
			{
				_raid_result = RAID_ATTACKER_WIN;
			}
			SendRaidInfo();
		}
		else
		{
			_curr_tower_level ++;	
			pImp->_runner->random_tower_monster(_curr_tower_level, 0);
		}
	}
}

void tower_raid_world_manager::GenLevelMonsters(gplayer* gplayer, int level, char client_idx)
{
	spin_autolock keeper(_raid_lock);
	if(level < 0 || level >= _tower_opened_level)
	{
		return;
	}

	DATA_TYPE dt;
	const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& config  = *(const TOWER_TRANSCRIPTION_PROPERTY_CONFIG*) gmatrix::GetDataMan().get_data_ptr(_level_tids[level], ID_SPACE_CONFIG, dt);
	if(dt != DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG || &config == NULL)
	{
		__PRINTF("无效的爬塔副本层属性模板: %d\n", _level_tids[level]);
		GLog::log( GLOG_INFO, "生成爬塔副本怪物失败,原因：层属性模板不存在 level=%d, level_tid=%d\n", level, _level_tids[level]);
	}

	if(_monster_cnt == 0)
	{
		_monster_cnt = abase::RandSelect(config.monster_gen_prob, 4) + 1;
		std::vector<int> avaiableIds;
		for(size_t i = 0; i < sizeof(config.monster_id_list) / sizeof(int); i ++)
		{
			if(config.monster_id_list[i] > 0)
			{
				avaiableIds.push_back(config.monster_id_list[i]);
			}
		}
		size_t cnt = avaiableIds.size();
		float* prob = new float[cnt];
		for(size_t i = 0; i < cnt; i ++)
		{
			prob[i] = 1.0f / cnt;
		}
		for(int i = 0; i < _monster_cnt; i ++)
		{
			_monsters[i].monster_tid = avaiableIds[RandSelect(prob, (int)cnt)];
			_monsters[i].monster_state = MONSTER_STATE_RAND;
		}
		__PRINTF("Tower_Level=%d, Random Monster: cnt=%d, monster[0]=%d, monster[1]=%d, monster[2]=%d, monster[3]=%d\n", level, _monster_cnt, _monsters[0].monster_tid, _monsters[1].monster_tid, _monsters[2].monster_tid, _monsters[3].monster_tid);

		for(int i = 0; i < 4; i ++)
		{
			_rand_send_client[i] = i;
		}
		for(int i = 0; i < 4; i ++)
		{
			int idx = abase::Rand(0, 3 - i);
			char temp = _rand_send_client[3-i];
			_rand_send_client[3 - i] = _rand_send_client[idx];
			_rand_send_client[idx] = temp;
		}
		printf("_rand_send_client, %d,%d,%d,%d\n", _rand_send_client[0], _rand_send_client[1], _rand_send_client[2], _rand_send_client[3]);
		_curr_reborn_pos.x = config.renascence_pos[0];
		_curr_reborn_pos.y = config.renascence_pos[1];
		_curr_reborn_pos.z = config.renascence_pos[2];
	}
	
	gplayer_imp* pImp = (gplayer_imp*)gplayer->imp;
	if(_monster_result_send_cnt >= 0 && _monster_result_send_cnt < 4)
	{
		int idx = _rand_send_client[_monster_result_send_cnt];
		_monsters[idx].client_idx = client_idx;
		pImp->_runner->random_tower_monster_result(_monsters[idx].monster_tid, _monsters[idx].client_idx);
		printf("random_tower_monster_result: tid=%d, client_idx=%d\n", _monsters[idx].monster_tid, _monsters[idx].client_idx);
		_monster_result_send_cnt ++;
		if(_monster_result_send_cnt == 4)
		{
			for(int i = 0; i < _monster_cnt; i ++)
			{
				if(GenMonster(_monster_cnt, _monsters[i].monster_tid, config))
				{
					_monsters[i].monster_state = MONSTER_STATE_GENED;
					pImp->AddMeetTowerMonster(_monsters[i].monster_tid);
				}
				else
				{
					__PRINTF("生成怪物失败tid=%d, tower_level=%d\n", _monsters[i].monster_tid, level);
				}
			}
			__PRINTF("打开胜利控制器, _curr_tower_level=%d, control=%d\n", _curr_tower_level - 1, _level_victory_controller);
			ActiveSpawn( this, _actived_controller, false);
			ActiveSpawn( this, _level_victory_controller, true );
			_actived_controller = _level_victory_controller;
			_level_victory_controller = config.success_controller_id;
			_level_start_time = g_timer.get_systime();
			pImp->FullHPAndMP();
			pImp->ResetTowerPlayerPet();
		}
	}
}

void tower_raid_world_manager::ResetLevelMonsters(gplayer* pPlayer, int level)
{
	size_t cnt = sizeof(_monsters) / sizeof(level_monster);
	for(size_t i = 0; i < cnt; i ++)
	{
		_monsters[i].monster_state = 0;
	}
	KillAllMonsters();
	_reset_monster_wait_time = 4;
	if (_monster_result_send_cnt < 4) _reset_monster_wait_time = 0;
}

bool tower_raid_world_manager::GenMonster(int monster_cnt, int id, const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& config)
{ 
	npc_template * pTemplate = npc_stubs_manager::Get(id);
	if(!pTemplate) 
	{
		return false;
	}

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;

	world *plane = GetWorldByIndex(0);
	if(!plane)
	{
		return false;
	}

	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	A3DVECTOR pos;
	pos.x = config.monster_range_center[0];
	pos.y = config.monster_range_center[1];
	pos.z = config.monster_range_center[2];
	pos.x += abase::Rand(-config.monster_range_radius,config.monster_range_radius);
	pos.z += abase::Rand(-config.monster_range_radius,config.monster_range_radius);
	float height = GetTerrain().GetHeightAt(pos.x,pos.z);
	if(pos.y < height) pos.y = height;

	gnpc * pNPC = npc_spawner::CreateMobBase(NULL,plane,ent,-1,0,pos,cid,abase::Rand(0,255),CLS_NPC_AI_POLICY_BASE,0,NULL,-1, 0, 0);
	if(!pNPC)
	{
		return false;
	}
	//pNPC->SetOwnerType(gowner::OWNER_SELF);
	//pNPC->SetOwnerID(gplayer->ID.id, 0);
	//pNPC->SetObjectState(gactive_object::STATE_NPC_OWNER);
	gnpc_imp * pImp = (gnpc_imp*)pNPC->imp;
	//pImp->SetDisappearLifeExpire(true);

	if(monster_cnt < 4)
	{
		int i = monster_cnt - 1;
		basic_prop& baseProp = pImp->_basic;
		baseProp.hp += config.monster_addon_property[i].hp;
		baseProp.mp += config.monster_addon_property[i].mp;

		q_extend_prop& extProp = pImp->_base_prop;
		extProp.max_hp += config.monster_addon_property[i].hp;
		extProp.max_mp += config.monster_addon_property[i].mp;
		extProp.damage_low +=config.monster_addon_property[i].attack;
		extProp.damage_high +=config.monster_addon_property[i].attack;
		extProp.defense +=config.monster_addon_property[i].defence;
		extProp.attack += config.monster_addon_property[i].hit;
		extProp.armor += config.monster_addon_property[i].evade;
		extProp.crit_rate += config.monster_addon_property[i].critical_rate;
		extProp.crit_damage += config.monster_addon_property[i].critical_damage;
		for(int i = 0; i < 6; i ++)
		{
			extProp.resistance[i] += config.monster_addon_property[i].resist[i];
		}
		extProp.anti_crit += config.monster_addon_property[i].anti_critical_rate;
		extProp.anti_crit_damage += config.monster_addon_property[i].anti_critical_damage;		

		q_enhanced_param& enProp = pImp->_en_point;
		enProp.spec_damage += config.monster_addon_property[i].addon_damage;
		enProp.dmg_reduce += config.monster_addon_property[i].damage_resistance;
		enProp.skill_attack_rate += config.monster_addon_property[i].skill_attack_rate;
		enProp.skill_armor_rate += config.monster_addon_property[i].skill_armor_rate;	
	}

	plane->InsertNPC(pNPC);
	pImp->_runner->enter_world();
	pImp->OnCreate();
	pNPC->Unlock();
	return true;
}

void tower_raid_world_manager::OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction) 
{
	gplayer_imp* _imp = (gplayer_imp*)pPlayer->imp;
	
	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(pPlayer->ID.id);
	if(it != _tower_player_info_map.end())
	{	
		tower_player_info& tpInfo = it->second;
		PlayerSkillMap::iterator it2 = tpInfo.skills.begin();
		for(;it2 != tpInfo.skills.end(); ++ it2)
		{
			player_sys_skill& s = it2->second;
			_imp->InsertSysSkill(s.skill_id, s.skill_level);
		}
		tpInfo.reenter = reenter;
	}
	else
	{	
		tower_player_info tpInfo;
		tpInfo.used_skill_cnt = 0;
		for(size_t i = 0; i < _addition_skill_list.size(); i ++)
		{
			player_sys_skill s;
			s.skill_id = _addition_skill_list[i];
			s.skill_level = 1;
			s.used = false;
			tpInfo.skills[s.skill_id] = s;
			_imp->InsertSysSkill(s.skill_id, s.skill_level);
		}
		tpInfo.deathCnt = MAX_PLAYER_DEATH_CNT;
		tpInfo.reenter = reenter;
		_tower_player_info_map[pPlayer->ID.id] = tpInfo;
	}

	
	if(reenter)
	{
		it = _tower_player_info_map.find(pPlayer->ID.id);
		if(it != _tower_player_info_map.end())
		{
			tower_player_info& tpInfo = it->second;
			pPlayer->pos = tpInfo.leavePos;
			__PRINTF("玩家爬塔副本断线重入位置: x=%3f, y=%3f, z=%3f\n", pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);
		}
	}
	else
	{
		//生成玩家所在级别关口的怪物
		_curr_tower_level = _init_tower_level;	
		_imp->SetStartLevelToday(_init_tower_level);
	}
}

void tower_raid_world_manager::GetAllDataWorldManager(gplayer_imp * pImp)
{
	spin_autolock keeper(_raid_lock);
	raw_wrapper raw(256);
	SaveTowerGiveSkillClient(pImp->GetParent(), raw);
	pImp->_runner->player_tower_give_skills(raw.data(), raw.size());


	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(pImp->_parent->ID.id);
	if(it == _tower_player_info_map.end())
	{	
		return;
	}
	tower_player_info& tpInfo = it->second;
	if(tpInfo.reenter)
	{
		if(_monster_cnt > 0)
		{
			if(_monster_result_send_cnt > 0 && _monster_result_send_cnt < 4)
			{
				for(int i = 0; i < _monster_result_send_cnt; i ++)
				{
					pImp->_runner->random_tower_monster_result(_monsters[i].monster_tid, _monsters[i].client_idx);
				}
				pImp->_runner->random_tower_monster(_curr_tower_level, 0);
			}
			else if(_monster_result_send_cnt == 4)
			{
				pImp->_runner->random_tower_monster(_curr_tower_level, 1);
			}
		}
		else
		{
			pImp->_runner->random_tower_monster(_curr_tower_level, 0);
		}
		pImp->ResetTowerPlayerPet(false);
	}
	else
	{
		pImp->FullHPAndMP();
		pImp->ResetTowerPlayerPet(true);
		__PRINTF("Start random tower_monster\n");
		pImp->_runner->random_tower_monster(_curr_tower_level, 0);
	}
}

void tower_raid_world_manager::OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction) 
{
	gplayer_imp* _imp = (gplayer_imp*)pPlayer->imp;
	for(size_t i = 0; i < _addition_skill_list.size(); i ++)
	{
		_imp->ClearSysSkill(_addition_skill_list[i]);
	}
	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(pPlayer->ID.id);
	if(it != _tower_player_info_map.end())
	{
		tower_player_info& tpInfo = it->second;
		tpInfo.leavePos = pPlayer->pos;
		__PRINTF("玩家%d下线，退出位置(x=%f,y=%f,z=%f)\n", pPlayer->ID.id, tpInfo.leavePos.x, tpInfo.leavePos.y, tpInfo.leavePos.z);
	}
}

void tower_raid_world_manager::OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much )
{
	spin_autolock keeper(_raid_lock);
	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(pPlayer->ID.id);
	if(it != _tower_player_info_map.end())
	{
		tower_player_info& tpInfo = it->second;
		tpInfo.deathCnt --;
		if(tpInfo.deathCnt <= 0)
		{
			if( _raid_result > 0 || RS_RUNNING != _status ) return;
			if( deadman_battle_faction & ( FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND ) )
			{
				_raid_result = RAID_ATTACKER_WIN;
			}
			else
			{
				_raid_result = RAID_DEFENDER_WIN;
			}
			SendRaidInfo();
			KillAllMonsters();
		}
		else
		{
			ResetLevelMonsters(pPlayer, _curr_tower_level);
		}
	}	
}

bool tower_raid_world_manager::CanCastSkillInWorld(int skill_id, int who)
{
	spin_autolock keeper(_raid_lock);
	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(who);
	if(it == _tower_player_info_map.end())
	{
		return false;
	}
	tower_player_info& tpInfo = it->second;
	PlayerSkillMap::iterator it2 = tpInfo.skills.find(skill_id);
	if(it2 == tpInfo.skills.end())
	{
		return true;
	}
	player_sys_skill& s = it2->second;
	if(s.skill_id == skill_id && s.used)
	{
		return false;
	}
	if(tpInfo.used_skill_cnt >= 3)
	{
		return false;
	}
	return true;
}

void tower_raid_world_manager::OnCastSkill(gactive_imp* pImp, int skill_id)
{
	spin_autolock keeper(_raid_lock);
	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(pImp->_parent->ID.id);
	if(it != _tower_player_info_map.end())
	{
		tower_player_info& tpInfo = it->second;
		PlayerSkillMap::iterator it2 = tpInfo.skills.find(skill_id);
		if(it2 != tpInfo.skills.end())
		{
			player_sys_skill& s = it2->second;
			s.used = true;
			tpInfo.used_skill_cnt ++;
			pImp->_runner->set_tower_give_skill(s.skill_id, s.skill_level, s.used);	
		}
	}
}

void tower_raid_world_manager::SaveTowerGiveSkillClient(gplayer* pPlayer, archive& ar)
{
	TowerPlayerInfoMap::iterator it = _tower_player_info_map.find(pPlayer->ID.id);
	if(it != _tower_player_info_map.end())
	{
		tower_player_info& tpInfo = it->second;
		ar << tpInfo.skills.size();
		PlayerSkillMap::iterator it2 = tpInfo.skills.begin();
		for(;it2 != tpInfo.skills.end(); ++ it2)
		{
			player_sys_skill& s = it2->second;
			ar << s.skill_id << s.skill_level << s.used;
		}
	}
}

bool tower_raid_world_manager::GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag)
{
	pos = _curr_reborn_pos;
	tag = _world_tag;
	return true;
}

bool tower_raid_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	pos = _curr_reborn_pos;
	world_tag = _world_tag;
	return true;
}

void tower_raid_world_manager::OnRunning()
{
	raid_world_manager::OnRunning();
	if(_reset_monster_wait_time > 0)
	{
		_reset_monster_wait_time --;
		if(_reset_monster_wait_time <= 0)
		{
			printf("Reset tower monster, level=%d\n", _curr_tower_level);
			DATA_TYPE dt;
			const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& config  = *(const TOWER_TRANSCRIPTION_PROPERTY_CONFIG*) gmatrix::GetDataMan().get_data_ptr(_level_tids[_curr_tower_level], ID_SPACE_CONFIG, dt);
			if(dt != DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG || &config == NULL)
			{
				__PRINTF("无效的爬塔副本层属性模板: %d\n", _level_tids[_curr_tower_level]);
				GLog::log( GLOG_INFO, "生成爬塔副本怪物失败,原因：层属性模板不存在 level=%d, level_tid=%d\n", _curr_tower_level, _level_tids[_curr_tower_level]);
				_reset_monster_wait_time = -1;
				return;
			}		
			for(int i = 0; i < _monster_cnt; i ++)
			{
				if(GenMonster(_monster_cnt, _monsters[i].monster_tid, config))
				{
					_monsters[i].monster_state = MONSTER_STATE_GENED;
				}
			}
			_reset_monster_wait_time = -1;
		}
	}
}
