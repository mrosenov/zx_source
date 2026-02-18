
#include <conf.h>
#include <string>
#include "actobject.h"
#include "playertemplate.h"
#include "template/itemdataman.h"
#include "template/npcgendata.h"
#include "item_manager.h"
#include "faction.h"
#include "reborn_skill.h"
#include "cooldowncfg.h"
#include "player_imp.h"

#define SECONDS_OF_ALL_DAY  (24*3600)
extern abase::timer g_timer;

int64_t __allow_login_class_mask = 0xFFFFFFFFFFFFFFFFLL;
int64_t __allow_login_class_mask1 = 0xFFFFFFFFFFFFFFFFLL;
player_template player_template::_instance;
player_template::player_template()
{
	memset(_class_list,0,sizeof(_class_list));
	memset(_level_adjust_table,0,sizeof(_level_adjust_table));
	memset(_team_adjust,0,sizeof(_team_adjust));
	memset(_team_cls_adjust,0,sizeof(_team_cls_adjust));
	memset(_produce_levelup_exp,0,sizeof(_produce_levelup_exp));
	memset(_produce_exp,0,sizeof(_produce_exp));
	memset(_sns_config,0, sizeof(_sns_config));
	memset(&_rune_data, 0, sizeof(_rune_data));
	for(int i = 0; i < GetMaxLevelLimit() + 1; i ++)
	{
		//放置基本的组队经验值
		//后面根据模板里的数据进行调整
		_exp_list[0].push_back(i*i*500);
		_exp_list[1].push_back(i*i*500);
		_exp_list[2].push_back(i*i*500);
		_exp_list[3].push_back(i*i*500);

		_pet_exp_list.push_back(i*i*500);
		_talent_point_list[0].push_back(0);
		_talent_point_list[1].push_back(0);
		_talent_point_list[2].push_back(0);
		_talent_point_list[3].push_back(0);

		_skill_point_list[0].push_back(0);
		_skill_point_list[1].push_back(0);
		_skill_point_list[2].push_back(0);
		_skill_point_list[3].push_back(0);
	}

	for(int i =0;i < MAX_TALISMAN_LEVEL;i ++)
	{
		_talisman_exp_list.push_back(0);
	}

	_debug_mode = false;

	_exp_bonus = 0.f;
	_money_bonus = 0.f;

	memset(&_pk_bet_data, 0, sizeof(_pk_bet_data));
	memset(&_kingdom_data, 0, sizeof(_kingdom_data));
	memset(&_bath_award, 0, sizeof(_bath_award));
	memset(&_fuwen_exp_config, 0, sizeof(_fuwen_exp_config));
}

bool
player_template::__Load(const char * filename,itemdataman * pDataMan)
{
	ONET::Conf conf(filename);
	try { 
		
		_debug_mode = atoi(conf.find("GENERAL","debug_command_mode").c_str())?true:false;
		_max_player_level = atoi(conf.find("GENERAL","logic_level_limit").c_str());
		_max_player_reborn_level = atoi(conf.find("GENERAL","logic_reborn_level_limit").c_str());
		__allow_login_class_mask  = atoll(conf.find("GENERAL","allow_login_class_mask").c_str());
		__allow_login_class_mask1  = atoll(conf.find("GENERAL","allow_login_class_mask1").c_str());
		if(_max_player_level <= 0 || _max_player_level > MAX_PLAYER_LEVEL) _max_player_level = MAX_PLAYER_LEVEL;
		if(_max_player_reborn_level <= 0 || _max_player_reborn_level > MAX_PLAYER_LEVEL) _max_player_reborn_level = MAX_PLAYER_LEVEL;

		_exp_bonus = atof(conf.find("GENERAL","exp_bonus").c_str());
		_money_bonus = atof(conf.find("GENERAL","money_bonus").c_str());
		int p = atoi(conf.find("GENERAL","no_output").c_str());
		__SETPRTFLAG(!p);
	}               
	catch(const std::string & str)
	{
		printf("%s\n",str.c_str());
		return false;
	}
	return __LoadDataFromDataMan(*pDataMan); 
}

bool 
player_template::__LoadDataFromDataMan(itemdataman & dataman)
{
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_CONFIG,dt);
	int64_t character_flag  = 0;
	int64_t character_flag1 = 0;
	char exp_flag = 0;
	char adj_flag = 0;
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_CONFIG,dt))
	{

		if(dt == DT_CHARACTER_CLASS_CONFIG)
		{
			const CHARACTER_CLASS_CONFIG &config = *(const CHARACTER_CLASS_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_CHARACTER_CLASS_CONFIG && &config);
			int cls = config.character_class_id;
			if(cls < 0 || cls >=QUSER_CLASS_COUNT)
			{
				ASSERT(false);
				return false;
			}

			if((cls <= 0x3F && (character_flag & ((int64_t)1 << cls))) || (cls > 0x3F && (character_flag1 & ((int64_t)1 << (cls - 0x40)))))
			{
				printf("发现了重复的角色数据 为%d号职业\n",cls);
				return false;
			}

			_class_list[cls].base.hp          = config.hp          ;
			_class_list[cls].base.mp          = config.mp          ;
			_class_list[cls].base.dmg         = config.dmg         ;
			_class_list[cls].base.defense     = config.defense     ;
			_class_list[cls].base.attack      = config.attack      ;
			_class_list[cls].base.armor       = config.armor       ;
			_class_list[cls].base.crit_rate   = config.crit_rate*10.f;
			_class_list[cls].base.crit_damage = config.crit_damage*0.01f;
			_class_list[cls].base.anti_stunt  = config.anti_stunt  ;
			_class_list[cls].base.anti_weak   = config.anti_weak   ;
			_class_list[cls].base.anti_slow   = config.anti_slow   ;
			_class_list[cls].base.anti_silence= config.anti_silence;
			_class_list[cls].base.anti_sleep  = config.anti_sleep  ;
			_class_list[cls].base.anti_twist  = config.anti_twist  ;
			
			_class_list[cls].lvlup.hp          = config.lvlup_hp          ;
			_class_list[cls].lvlup.mp          = config.lvlup_mp          ;
			_class_list[cls].lvlup.dmg         = config.lvlup_dmg         ;
			_class_list[cls].lvlup.defense     = config.lvlup_defense     ;
			_class_list[cls].lvlup.attack      = config.lvlup_attack      ;
			_class_list[cls].lvlup.armor       = config.lvlup_armor       ;
			_class_list[cls].lvlup.crit_rate   = config.lvlup_crit_rate*10.f;
			_class_list[cls].lvlup.crit_damage = config.lvlup_crit_damage*0.01f;
			_class_list[cls].lvlup.anti_stunt  = config.lvlup_anti_stunt  ;
			_class_list[cls].lvlup.anti_weak   = config.lvlup_anti_weak   ;
			_class_list[cls].lvlup.anti_slow   = config.lvlup_anti_slow   ;
			_class_list[cls].lvlup.anti_silence= config.lvlup_anti_silence;
			_class_list[cls].lvlup.anti_sleep  = config.lvlup_anti_sleep  ;
			_class_list[cls].lvlup.anti_twist  = config.lvlup_anti_twist  ;


			_class_list[cls].class_id 	= config.character_class_id;
			_class_list[cls].base_level 	= config.level_required;
			_class_list[cls].faction 	= config.faction;
			_class_list[cls].enemy_faction 	= config.enemy_faction;
			_class_list[cls].walk_speed 	= config.walk_speed;
			_class_list[cls].run_speed 	= config.run_speed;
			_class_list[cls].hp_gen[0]	= config.hp_gen1;
			_class_list[cls].hp_gen[1]	= config.hp_gen2;
			_class_list[cls].hp_gen[2]	= config.hp_gen3;
			_class_list[cls].hp_gen[3]	= config.hp_gen4;
			_class_list[cls].mp_gen[0]	= config.mp_gen1;
			_class_list[cls].mp_gen[1]	= config.mp_gen2;
			_class_list[cls].mp_gen[2]	= config.mp_gen3;
			_class_list[cls].mp_gen[3]	= config.mp_gen4;

			_class_list[cls].character_combo_id = config.character_combo_id;
			_class_list[cls].character_combo_id2 = config.character_combo_id2;
			_class_list[cls].xp_skill = config.xp_skill;
			   
			if(cls <= 0x3F)
			{
				character_flag |= (int64_t)1 << cls;
			}
			else
			{
				character_flag1 |= (int64_t)1 << (cls - 0x40);
			}
		}
		else if(dt == DT_DEITY_CHARACTER_CONFIG)
		{
			const DEITY_CHARACTER_CONFIG &config = *(const DEITY_CHARACTER_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_DEITY_CHARACTER_CONFIG && &config);
			memcpy(_deity_prop_data, config.deity_config, sizeof(config.deity_config));
		}	
		else if(dt == DT_PARAM_ADJUST_CONFIG)
		{
			const PARAM_ADJUST_CONFIG &adjust = *(const PARAM_ADJUST_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_PARAM_ADJUST_CONFIG && &adjust);
			adj_flag = 1;
			//进行级别差修正
			int level = 100;
			int j = MAX_LEVEL_DIFF;
			level_adjust lad = {0.f,0.f,0.f,0.f};
		
			int k = adjust.adjust_config_id;
			if (k > 1 || k < 0) continue;

			for(size_t i =0; i < 16; i++)
			{
				if(adjust.level_diff_adjust[i].level_diff == 0) break;
				if(adjust.level_diff_adjust[i].level_diff >= level ) 
				{
					printf("在级别经验物品修正表内发现错误\n");
					return false;
				}
				level = adjust.level_diff_adjust[i].level_diff;
				lad.exp_adjust		= adjust.level_diff_adjust[i].adjust_exp;
				lad.money_adjust	= adjust.level_diff_adjust[i].adjust_money;	
				lad.item_adjust		= adjust.level_diff_adjust[i].adjust_matter;
				lad.attack_adjust 	= adjust.level_diff_adjust[i].adjust_attack;
				if(lad.attack_adjust < 0.f) lad.attack_adjust = 0.f;
				if(lad.attack_adjust > 1.f) lad.attack_adjust = 0.f;
				for(;j >= level-BASE_LEVEL_DIFF && j >= 0;j-- ){_level_adjust_table[k][j]=lad;}
			}
			for(;j >= 0 ;j-- ) {_level_adjust_table[k][j]=lad;}

			//进行队伍修正
			for(size_t i = 0; i < TEAM_MEMBER_CAPACITY+1 ; i ++)
			{	
				_team_adjust[i].exp_adjust	= adjust.team_adjust[i].adjust_exp;
			}

			for(size_t i =0; i < TEAM_MAX_RACE_COUNT+1; i ++)
			{
				_team_cls_adjust[i].exp_adjust	= adjust.team_profession_adjust[i].adjust_exp/20.0f;
				//_team_cls_adjust[i].exp_adjust	= adjust.team_profession_adjust[i].adjust_exp;
			}

		}
		else if(dt == DT_PLAYER_LEVELEXP_CONFIG)
		{
			const PLAYER_LEVELEXP_CONFIG  &level= *(const PLAYER_LEVELEXP_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_PLAYER_LEVELEXP_CONFIG && &level);

			exp_flag = 1;
			//玩家省级曲线表
			for(size_t i = 0; i < (size_t)GetMaxLevelLimit(); i ++)
			{
				_exp_list[0][i] = (int64_t)level.exp[i];
				if(_exp_list[0][i]  <= 0)
				{
					printf("级别%d升级所需经验值为不大于0(%lld)\n",i, _exp_list[0][i]);

				}
			}

			for(size_t i = 0; i < (size_t)GetMaxLevelLimit(); i ++)
			{
				_exp_list[1][i] = (int64_t)level.exp_lev_1[i];
				if(_exp_list[1][i]  <= 0)
				{
					printf("级别%d升级所需经验值为不大于0(%lld)\n",i, _exp_list[1][i]);

				}
			}
			for(size_t i = 0; i < (size_t)GetMaxLevelLimit(); i ++)
			{
				_exp_list[2][i] = (int64_t)level.exp_lev_2[i];
				if(_exp_list[2][i]  <= 0)
				{
					printf("级别%d升级所需经验值为不大于0(%lld)\n",i, _exp_list[2][i]);

				}
			}
			for(size_t i = 0; i < (size_t)GetMaxLevelLimit(); i ++)
			{
				_exp_list[3][i] = (int64_t)level.exp_lev_3[i];
				if(_exp_list[3][i]  <= 0)
				{
					printf("级别%d升级所需经验值为不大于0(%lld)\n",i, _exp_list[3][i]);

				}
			}

			//设置生产升级曲线表
			for(size_t i = 0; i < (size_t)GetMaxProduceLevel(); i ++)
			{
				_produce_levelup_exp[i] = level.prod_exp_need[i];
				for(size_t j = 0;j < (size_t)GetMaxProduceLevel(); j ++)
				{
					_produce_exp[i][j] = level.prod_exp_gained[i][j];
				}
			}

			//设置法宝升级曲线标
			for(size_t i =0; i < _talisman_exp_list.size(); i ++)
			{
				_talisman_exp_list[i] = level.talisman_exp[i];
			}

			//设置宠物升级曲线表
			for(size_t i = 0;i < _pet_exp_list.size();++i)
			{
				_pet_exp_list[i] = (int)level.pet_exp[i];
			}

			for(size_t i = 0; i < 100; ++i)
			{
				if(level.deity[i].deity_exp < 0 || level.deity[i].failure_exp < 0) 
				{
					printf("封神升级修为或者升级失败惩罚修为小于0\n");
					return false;
				}
				if(level.deity[i].levelup_prob < 0 || level.deity[i].levelup_prob > 1)
				{
					printf("封神升级成功的概率错误\n");
					return false;

				}
				if(level.deity[i].deity_exp < level.deity[i].failure_exp)
				{
					printf("封神升级成功需要的经验小于封神失败惩罚经验\n");
					return false;
				}

				_deity_exp_list[i].deity_exp = (int64_t)level.deity[i].deity_exp;
				_deity_exp_list[i].failure_exp = (int64_t)level.deity[i].failure_exp;
				_deity_exp_list[i].levelup_prob = level.deity[i].levelup_prob;
			}

		}
		//分版本导致有两个特殊id配置表
		else if(dt == DT_SPECIAL_ID_CONFIG && id == SPECIAL_ID_CONFIG_TEMPLID)
		{
			const SPECIAL_ID_CONFIG  &cfg= *(const SPECIAL_ID_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_SPECIAL_ID_CONFIG && &cfg);
				
			printf("获取特殊物品ID:(%f,%d,%d,%d)\n",cfg.monster_drop_prob, cfg.id_money_matter, cfg.id_speaker, cfg.id_destroying_matter);
			g_config.money_drop_rate 	= cfg.monster_drop_prob;
			g_config.item_money_id		= cfg.id_money_matter;
			g_config.item_speaker_id	= cfg.id_speaker;
			g_config.item_speaker_id2	= cfg.id_speaker2;
			g_config.item_destroy_id	= cfg.id_destroying_matter;
			g_config.battle_attacker_drop 	= cfg.id_attacker_droptable;
			g_config.battle_defender_drop 	= cfg.id_defender_droptable;
			g_config.talisman_reset_item	= cfg.id_talisman_reset_matter;
			g_config.talisman_combine_fee	= cfg.fee_talisman_merge;
			g_config.talisman_enchant_fee	= cfg.fee_talisman_enchant;
			g_config.id_war_material1	= cfg.id_war_material1;
			g_config.id_war_material2	= cfg.id_war_material2;
			//宠物服务相关费用
			g_config.fee_adopt_pet		= cfg.fee_pet_gained;
			g_config.fee_free_pet		= cfg.fee_pet_free;
			g_config.fee_refine_pet		= cfg.fee_pet_refine;
			g_config.fee_rename_pet		= cfg.fee_pet_rename;
			g_config.renew_mount_material	= cfg.id_vehicle_upgrade_item;
			g_config.element_data_version	= cfg.version;

			
			g_config.lock_item_fee 		= cfg.fee_lock;
			g_config.unlock_item_fee	= cfg.fee_unlock;
			g_config.unlock_item_id 	= cfg.unlock_item_id;
			g_config.item_unlocking_id 	= cfg.unlocking_item_id;
			g_config.item_broken_id 	= cfg.id_damaged_item;
			g_config.item_restore_broken_id	= cfg.id_repair_damaged_item;
			g_config.spirit_charge_fee	= cfg.fee_restore_soul;
			g_config.item_blood_enchant 	= cfg.id_bleed_identity_host_item;
			g_config.base_spirit_stone_id	= cfg.id_lowgrade_soul_stone;
			g_config.adv_spirit_stone_id	= cfg.id_highgrade_soul_stone;
			g_config.item_change_style	= cfg.id_change_face_item;
			g_config.item_arena		= cfg.id_enter_arena_item;
			g_config.item_reborn_arena	= cfg.id_enter_arena_reborn_item;
			g_config.item_bid_request	= cfg.id_unique_bid_item;

			g_config.fee_gem_refine = cfg.fee_gem_refine;
			g_config.fee_gem_extract = cfg.fee_gem_extract;
			g_config.fee_gem_tessellation = cfg.fee_gem_tessellation;		
			g_config.fee_gem_dismantle = cfg.fee_gem_single_dismantle;	
			g_config.fee_gem_smelt = cfg.fee_gem_smelt;	
			g_config.fee_gem_slot_identify = cfg.fee_gem_slot_identify;	
			g_config.fee_gem_slot_customize = cfg.fee_gem_slot_customize;
			g_config.fee_gem_slot_rebuild = cfg.fee_gem_slot_rebuild;
			g_config.gem_upgrade_upper_limit = cfg.gem_upgrade_upper_limit;	

			g_config.id_gem_smelt_article[0] = cfg.id_gem_smelt_article;		
			g_config.id_gem_smelt_article[1] = cfg.id_gem_smelt_article_1;		
			g_config.id_gem_smelt_article[2] = cfg.id_gem_smelt_article_2;		
			g_config.id_gem_refine_article[0] = cfg.id_gem_refine_article;
			g_config.id_gem_refine_article[1] = cfg.id_gem_refine_article_1;
			g_config.id_gem_refine_article[2] = cfg.id_gem_refine_article_2;
			g_config.id_gem_slot_lock_article[0] = cfg.id_gem_slot_lock_article;
			g_config.id_gem_slot_lock_article[1] = cfg.id_gem_slot_lock_article_1;
			g_config.id_gem_slot_lock_article[2] = cfg.id_gem_slot_lock_article_2;

			for(int i = 0; i < 3; i ++)
			{
				g_config.id_gem_slot_rebuild_article[i] = cfg.id_gem_slot_rebuild_article[i];
			}

			if(g_config.id_gem_smelt_article[0] < 1 && g_config.id_gem_smelt_article[1] < 1 && g_config.id_gem_smelt_article[2] < 1)
			{
				printf("宝石熔炼消耗物品ID错误\n");
				return false;
			}
			if(g_config.id_gem_refine_article[0] < 1 && g_config.id_gem_refine_article[1] < 1 && g_config.id_gem_refine_article[2] < 1)
			{
				printf("宝石精练消耗物品ID错误\n");
				return false;
			}
			if(g_config.id_gem_slot_lock_article[0] < 1 && g_config.id_gem_slot_lock_article[1] < 1 && g_config.id_gem_slot_lock_article[2] < 1)
			{
				printf("宝石插槽重铸锁定物品ID错误\n");
				return false;
			}
			if(g_config.id_gem_slot_rebuild_article[0] < 1 && g_config.id_gem_slot_rebuild_article[1] < 1 && g_config.id_gem_slot_rebuild_article[2] < 1)
			{
				printf("宝石插槽重铸消耗物品ID错误\n");
				return false;
			}

			g_config.consign_role_type					= cfg.consign_role_type;
			g_config.consign_role_item_id					= cfg.id_consign_role_item;
			g_config.item_treasure_region_upgrade_id	= cfg.id_treasure_region_upgrade_item;
			memcpy(g_config.item_speaker2_id, cfg.id_speaker_special, sizeof(g_config.item_speaker2_id));
			memcpy(g_config.item_speaker3_id, cfg.id_speaker_special_anony, sizeof(g_config.item_speaker3_id));
			memcpy(g_config.item_change_name_id, cfg.id_change_name, sizeof(g_config.item_change_name_id));
			memcpy(g_config.item_change_faction_name_id, cfg.id_change_name_guild, sizeof(g_config.item_change_faction_name_id));
			memcpy(g_config.item_change_family_name_id, cfg.id_change_name_family, sizeof(g_config.item_change_family_name_id));

			memcpy(g_config.item_equip_hole_id, cfg.id_equip_hole, sizeof(g_config.item_equip_hole_id));
			memcpy(g_config.item_astrology_upgrade_id, cfg.id_xingzuo_levelup, sizeof(g_config.item_astrology_upgrade_id));
			g_config.fee_astrology_add = cfg.fee_xingzuo_add;
			g_config.fee_astrology_remove = cfg.fee_xingzuo_remove;
			g_config.fee_astrology_identify = cfg.fee_xingzuo_identify;
			g_config.item_fix_prop_lose = cfg.id_fix_prop_lose;
			g_config.id_produce_jinfashen = cfg.id_tmpl_jinfashen_config;

			memcpy(g_config.fuwen_fragment_id, cfg.id_rune2013_fragment, sizeof(cfg.id_rune2013_fragment));
			memcpy(g_config.fuwen_compose_extra_id, cfg.id_rune2013_merge_extra_num, sizeof(cfg.id_rune2013_merge_extra_num));
			memcpy(g_config.fuwen_erase_id, cfg.id_rune2013_erase, sizeof(cfg.id_rune2013_erase));

		}
		else if(dt == DT_PLAYER_TALENT_CONFIG)
		{
			const PLAYER_TALENT_CONFIG  &cfg= *(const PLAYER_TALENT_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_PLAYER_TALENT_CONFIG && &cfg);
			printf("读取天赋配置表\n");
			for(size_t i = 0; i < 200; i ++)
			{
				_talent_point_list[0][i + 1] = cfg.talent_point[i];
			}

			for(size_t i = 0; i < 200; i ++)
			{
				_talent_point_list[1][i + 1] = cfg.talent_point_1[i];
			}
			for(size_t i = 0; i < 200; i ++)
			{
				_talent_point_list[2][i + 1] = cfg.talent_point_2[i];
			}
			for(size_t i = 0; i < 200; i ++)
			{
				_talent_point_list[3][i + 1] = cfg.talent_point_3[i];
			}
		}
		else if(dt == DT_PLAYER_SKILL_POINT_CONFIG)
		{
			const PLAYER_SKILL_POINT_CONFIG  &cfg= *(const PLAYER_SKILL_POINT_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_PLAYER_SKILL_POINT_CONFIG && &cfg);
			printf("读取技能点配置表\n");
			for(size_t i = 0; i < 200; i ++)
			{
				_skill_point_list[0][i + 1] = cfg.skill_point[i];
			}

			for(size_t i = 0; i < 200; i ++)
			{
				_skill_point_list[1][i + 1] = cfg.skill_point_1[i];
			}
			for(size_t i = 0; i < 200; i ++)
			{
				_skill_point_list[2][i + 1] = cfg.skill_point_2[i];
			}
			for(size_t i = 0; i < 200; i ++)
			{
				_skill_point_list[3][i + 1] = cfg.skill_point_3[i];
			}
		}
		else if(dt == DT_PIE_LOVE_CONFIG)
		{
			const PIE_LOVE_CONFIG &cfg = *(const PIE_LOVE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_PIE_LOVE_CONFIG && &cfg); 
			printf("读取征友平台配置表\n");
			for(size_t i = 0; i < 5; i++)
			{
				_sns_config[i].fee_press_msg = cfg.sns_config[i].fee_press_msg;
				_sns_config[i].id_press_msg_item = cfg.sns_config[i].id_press_msg_item;
				_sns_config[i].id2_press_msg_item = cfg.sns_config[i].id2_press_msg_item;
				_sns_config[i].id_press_msg_task= cfg.sns_config[i].id_press_msg_task;
				_sns_config[i].id_support_msg_item = cfg.sns_config[i].id_support_msg_item;
				_sns_config[i].id_support_msg_task = cfg.sns_config[i].id_support_msg_task;
				_sns_config[i].id_against_msg_item = cfg.sns_config[i].id_against_msg_item;
				_sns_config[i].id_against_msg_task = cfg.sns_config[i].id_against_msg_task;
				_sns_config[i].fee_apply_msg= cfg.sns_config[i].fee_apply_msg;
				_sns_config[i].id_apply_msg_item = cfg.sns_config[i].id_apply_msg_item;
				_sns_config[i].id_apply_msg_task = cfg.sns_config[i].id_apply_msg_task;
				_sns_config[i].fee_response_msg = cfg.sns_config[i].fee_response_msg;
				_sns_config[i].id_response_msg_item = cfg.sns_config[i].id_response_msg_item;
				_sns_config[i].id_accept_apply_task1= cfg.sns_config[i].id_accept_apply_task1;
				_sns_config[i].id_accept_apply_task2= cfg.sns_config[i].id_accept_apply_task2;
				_sns_config[i].id_accept_apply_task3= cfg.sns_config[i].id_accept_apply_task3;
			}
		}
		else if(dt == DT_SCORE_TO_RANK_CONFIG)
		{
			const SCORE_TO_RANK_CONFIG &cfg = *(const SCORE_TO_RANK_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_SCORE_TO_RANK_CONFIG&& &cfg);
		        //目前仅仅战场允许配置
			ASSERT(cfg.map_id >= 100 && cfg.map_id <= 200);
			printf("读取战场等级和积分的配置表\n");	
			std::vector<battle_rank> rank_score;
			for(int i = 0; i < 10; i++)
			{
				battle_rank tmp;
				tmp.score = cfg.score2rank[i].score;
				tmp.task_id = cfg.score2rank[i].task_id; 
				rank_score.push_back(tmp);
			}
			_rank_score[cfg.map_id] = rank_score;
			_map_controller[cfg.map_id] = cfg.controller;
		}
		else if(dt == DT_BATTLE_DROP_CONFIG)
		{
			const BATTLE_DROP_CONFIG &cfg = *(const BATTLE_DROP_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_BATTLE_DROP_CONFIG&& &cfg);
		        //目前仅仅战场允许配置
			ASSERT(cfg.map_id >= 100 && cfg.map_id <= 200);
			death_drop_item _item;
			RANK_DROP_ITEM _rank_drop;

			for(int i = 0; i < 10; i++)
			{
				std::vector<death_drop_item> death_drop;
				float prop = 0.0f;
				for(int j = 0; j < 10; j++)
				{
					_item.id = cfg.drops[i][j].id_obj;
					_item.probability = cfg.drops[i][j].probability;
					death_drop.push_back(_item);
					prop += cfg.drops[i][j].probability;	
				}
				ASSERT(fabs(1.0f - prop) < 1e-5); 
				_rank_drop[i] = death_drop;
			}
			_death_drop_item[cfg.map_id] = _rank_drop;
		}
		
		else if(dt == DT_BATTLE_DEPRIVE_CONFIG)
		{
			const BATTLE_DEPRIVE_CONFIG &cfg = *(const BATTLE_DEPRIVE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_BATTLE_DEPRIVE_CONFIG&& &cfg);
		        //目前仅仅战场允许配置
			ASSERT(cfg.map_id >= 100 && cfg.map_id <= 200);
			deprive_item _item;
			std::vector<deprive_item> _deprive;
			for(size_t i = 0; i < 10; i++)
			{
				_item.id= cfg.deprive[i].id_obj;
				_item.max_num = cfg.deprive[i].max_num;
				if(_item.id != 0)
				{
					_deprive.push_back(_item);
				}	
			}
			_deprive_item[cfg.map_id] = _deprive;	
		}	
		else if(dt == DT_BATTLE_SCORE_CONFIG)
		{
			const BATTLE_SCORE_CONFIG &cfg = *(const BATTLE_SCORE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_BATTLE_SCORE_CONFIG&& &cfg);
		        //目前仅仅战场允许配置
			ASSERT(cfg.map_id >= 100 && cfg.map_id <= 200);

			std::vector<unsigned int> score;	
			for(size_t i = 0; i < 10; ++i)
			{
				score.push_back(cfg.scores[i].score);
			}
			_kill_score[cfg.map_id] = score;
			
			score.clear();
			for(size_t j = 0; j < 8; ++j)
			{
				score.push_back(cfg.ext_score[j].ext_score);
			}
			_conKill_score[cfg.map_id] = score;
			
			std::vector<monster_score> m_score;
			monster_score _score;
			for(size_t k = 0; k < 50; ++k)
			{
				_score.monster_id = cfg.monster_scores[k].monster_id;
				_score.monster_score = cfg.monster_scores[k].monster_score;
				m_score.push_back(_score);
			}
			_monster_score[cfg.map_id] = m_score;

			_battle_win_score[cfg.map_id] = cfg.victory_score;
		}
		else if(dt == DT_INSTANCING_BATTLE_CONFIG )
		{
			const INSTANCING_BATTLE_CONFIG &cfg = *(const INSTANCING_BATTLE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_INSTANCING_BATTLE_CONFIG&& &cfg);
		        //目前仅仅战场允许配置
			ASSERT(cfg.map_id >= 100 && cfg.map_id <= 200);
			ASSERT(cfg.valid_tollgate_num > 0 && cfg.valid_tollgate_num <= 20);

			//战场信息模板
			instance_field_info _battle_info;
			_battle_info.tid = cfg.id;
			_battle_info.max_player_limit = cfg.max_player;
			_battle_info.manual_start_player_num = cfg.open_player_num;
			_battle_info.auto_start_player_num = cfg.open_player_num2;
			_battle_info.reborn_limit = cfg.renascence_count;
			_battle_info.min_level_limit = cfg.level_lower_limit;
			_battle_info.max_level_limit = cfg.level_upper_limit;
			_battle_info.max_prof_count_limit = 0;
			_battle_info.faction_limit = 0; 
			_battle_info.required_item = cfg.active_item;
			_battle_info.required_money = cfg.active_fee;
			_battle_info.queuing_time = cfg.apply_time;
			_battle_info.fighting_time = cfg.run_time;
			_battle_info.preparing_time = cfg.reopen_time;
			_instance_field_info[cfg.map_id] = _battle_info;
				
			//关卡模板
			std::vector<instance_level_info> _level;
			instance_level_info _level_info;
			for(size_t i = 0; i < (size_t)cfg.valid_tollgate_num; ++i)
			{
				_level_info.score = cfg.tollgate[i].score;
				_level_info.time_limit = cfg.tollgate[i].time_limit;
				_level_info.score_per_sec = cfg.tollgate[i].score_per_sec;
				_level_info.open_ctrl_id = cfg.tollgate[i].open_controller;
				_level_info.finish_ctrl_id = cfg.tollgate[i].finish_controller;
				_level_info.death_penlty_score = cfg.tollgate[i].death_penlty;
				_level_info.status = 0;
				_level_info.start_timestamp = 0;
				_level_info.close_timestamp = 0;
				_level.push_back(_level_info);
			}
			_instance_level_info[cfg.map_id] = _level;
				
			//怪物积分模板	
			std::vector<instance_monster_score> _score;
			instance_monster_score _score_info;
			for(size_t j = 0; j < 20; ++j)
			{
				_score_info.monster_id = cfg.monster[j].monster_id;
				_score_info.score = cfg.monster[j].score;
				_score.push_back(_score_info);
			}
			_instance_monster_score[cfg.map_id] = _score;

		
			//奖励模板	
			std::vector<instance_award_info> _award;
			instance_award_info _award_info;
			for(size_t k = 0; k < 5; ++k)
			{
				_award_info.victory_score = cfg.award_level[k].victory_score_level;
				_award_info.victory_task_id = cfg.award_level[k].victory_task_id;
				_award_info.lost_score = cfg.award_level[k].failure_score_level;
				_award_info.lost_task_id= cfg.award_level[k].failure_task_id;
				_award.push_back(_award_info);
			}
			_instance_award_info[cfg.map_id] = _award;

			_instance_win_ctrlID[cfg.map_id] = cfg.victory_controller;
		}
		else if(dt == DT_ORIGINAL_SHAPE_CONFIG)
		{
			const ORIGINAL_SHAPE_CONFIG &cfg = *(const ORIGINAL_SHAPE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_ORIGINAL_SHAPE_CONFIG && &cfg);
			std::vector<original_shape> _original_shape;
			original_shape _shape; 
			float prop;
			for(size_t i = 0; i < 20; ++i)
			{
				if(cfg.shape_map[i].monster_id <= 0) continue;
				_shape.monster_id = cfg.shape_map[i].monster_id;
				memcpy(&_shape, &cfg.shape_map[i], sizeof(original_shape));
				prop =  _shape.original_info[0].probability + _shape.original_info[1].probability 
					+ _shape.original_info[2].probability;
				ASSERT(fabs(1-prop) < 1e-5);
				_original_shape.push_back(_shape);
			}
			if(!_original_shape.empty())
			{
				_original_shape_info[cfg.table_id] = _original_shape;
			}
		}

		else if(dt == DT_ARENA_DROPTABLE_CONFIG)
		{
			const ARENA_DROPTABLE_CONFIG &cfg = *(const ARENA_DROPTABLE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_ARENA_DROPTABLE_CONFIG&& &cfg);
			std::set<int> drop_item;
			for(size_t i = 0; i < 10; ++i)
			{
				drop_item.insert(cfg.droptable[i].itemid);
			}
			_arena_drop_item[cfg.mapid] = drop_item;

		}

		else if(dt == DT_COMBINE_SKILL_EDIT_CONFIG)
		{
			const COMBINE_SKILL_EDIT_CONFIG &cfg = *(const COMBINE_SKILL_EDIT_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_COMBINE_SKILL_EDIT_CONFIG&& &cfg);

			for(size_t i = 0; i < 30; ++i)
			{
				ASSERT(cfg.edit_config[i].cool_index == 0 || 
				      (cfg.edit_config[i].cool_index >= COOLDOWN_INDEX_COMBINE_BEGIN && cfg.edit_config[i].cool_index <= COOLDOWN_INDEX_COMBINE_END));
			}
			memcpy(_combine_edit_config, cfg.edit_config, sizeof(cfg.edit_config));
		}
		else if(dt == DT_UPGRADE_EQUIP_CONFIG)
		{
			const UPGRADE_EQUIP_CONFIG &cfg = *(const UPGRADE_EQUIP_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_UPGRADE_EQUIP_CONFIG && &cfg);

			_equipment_upgrade_data.reserve(400);

			for(size_t i = 0; i < 200; ++i)
			{
				if(cfg.upgrade_config[i].equip_id > 0 && cfg.upgrade_config[i].stone_id > 0 && cfg.upgrade_config[i].out_equip_id > 0)
				{
					for(size_t j = 0; j < _equipment_upgrade_data.size(); ++j)
					{
						if(_equipment_upgrade_data[j].equip_id == cfg.upgrade_config[i].equip_id ||
								_equipment_upgrade_data[j].output_equip_id == cfg.upgrade_config[i].out_equip_id)
						{
							ASSERT(false); 
						}
					}
					equipment_upgrade_data data;
					data.equip_id = cfg.upgrade_config[i].equip_id;
					data.stone_id = cfg.upgrade_config[i].stone_id;
					data.stone_num = cfg.upgrade_config[i].num;
					data.output_equip_id = cfg.upgrade_config[i].out_equip_id;
					
					//检查升级前后的装备类型，位置，强化石等是否一致
					DATA_TYPE dt1;
					const EQUIPMENT_ESSENCE &ess1 = *(const EQUIPMENT_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(data.equip_id, ID_SPACE_ESSENCE,dt1); 
					if(dt1 != DT_EQUIPMENT_ESSENCE || &ess1 == NULL)
					{
						ASSERT(false);
					}
					
					DATA_TYPE dt2;
					const EQUIPMENT_ESSENCE &ess2 = *(const EQUIPMENT_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(data.output_equip_id, ID_SPACE_ESSENCE,dt2); 
					if(dt2 != DT_EQUIPMENT_ESSENCE || &ess2 == NULL)
					{
						ASSERT(false);
					}
					ASSERT(ess1.equip_type == ess2.equip_type);
					ASSERT(ess1.equip_mask == ess2.equip_mask);
			//		ASSERT(ess1.id_estone == ess2.id_estone);

					_equipment_upgrade_data.push_back(data);
				}
			}
		}
		else if(DT_ONLINE_GIFT_BAG_CONFIG == dt)
		{
			const ONLINE_GIFT_BAG_CONFIG &cfg = *(const ONLINE_GIFT_BAG_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
	        ASSERT(dt == DT_ONLINE_GIFT_BAG_CONFIG && &cfg);
			
			online_giftbag_config data;
			data.id							= cfg.id; 
			data.award_type					= cfg.award_type;
			data.start_time_per_day			= cfg.start_time_per_day;
			data.end_time_per_day			= cfg.end_time_per_day;
		
			//时区调整放在TrySetOnlineGiftBagInfo()函数里
			data.start_time_per_interval	= cfg.start_time_per_interval;
			data.end_time_per_interval		= cfg.end_time_per_interval;
			data.overdue_time				= cfg.expire_time;

			data.renascence_count			= cfg.renascence_count;
			data.require_min_level			= cfg.require_min_level;
			data.require_max_level			= cfg.require_max_level;
			data.require_gender				= cfg.require_gender;
			data.character_combo_id			= cfg.character_combo_id;
			data.character_combo_id2		= cfg.character_combo_id2;
			data.require_race				= cfg.require_race;

			//数据检查
			ASSERT( TrySetOnlineGiftBagInfo(data) );
			//已经过了领奖最后期限的模板不读入内存，必须在TrySetOnlineGiftBagInfo()调用完毕后检查
			if(g_timer.get_systime() >= data.overdue_time)
			{
				continue;
			}

			//最多包含16个小礼包
			for(int i = 0; i < MAX_SMALL_ONLINE_GB_COUNT; i++)
			{
				online_giftbag_config::small_gift_bag_info tmpinfo;
				tmpinfo.gift_bag_id			= cfg.small_gift_bags[i].gift_bag_id;
				tmpinfo.deliver_interval	= cfg.small_gift_bags[i].deliver_interval;

				data.small_gift_bags.push_back(tmpinfo);
			}

			_online_giftbag_config[data.id] = data;
		}
		else if(dt == DT_SCROLL_REGION_CONFIG)
		{
			const SCROLL_REGION_CONFIG &cfg = *(const SCROLL_REGION_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_SCROLL_REGION_CONFIG && &cfg);

			treasure_region region;
			memset(&region, 0, sizeof(region));

			region.region_id = cfg.id;
			region.region_index = cfg.region_pos;

			float level_prob = 0.0f;
			for(size_t i = 0; i < 6; ++i)
			{
				level_prob += cfg.region_level_prob[i];
			}

			if(fabs(1.0f - level_prob) > 1e-5) 
			{
				printf("挖宝区域初始等级对应的概率和不为1: id=%d, prob=%f\n", cfg.id, level_prob);
				return false;
			}

			memcpy(region.init_level_prob, cfg.region_level_prob, sizeof(region.init_level_prob));
			memcpy(region.levelup_prob, cfg.region_level_up_prob, sizeof(region.levelup_prob));

			region.unlock_region_item = cfg.hide_region_unlock_item_id;

			mempcpy(region.treasure_award, cfg.region_award_list, sizeof(region.treasure_award));

			_treasure_region.push_back(region);

		}
		else if(dt == DT_RUNE_DATA_CONFIG)
		{	
			const RUNE_DATA_CONFIG &cfg = *(const RUNE_DATA_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_RUNE_DATA_CONFIG && &cfg);

			memcpy(_rune_data.id_rune_identify, cfg.id_rune_identify, sizeof(cfg.id_rune_identify));
			memcpy(_rune_data.id_rune_refine, cfg.id_rune_refine, sizeof(cfg.id_rune_refine));
			memcpy(_rune_data.id_rune_reset, cfg.id_rune_reset, sizeof(cfg.id_rune_reset));
			memcpy(_rune_data.id_rune_change_slot, cfg.id_rune_change_slot, sizeof(cfg.id_rune_change_slot));

			_rune_data.exp_identify = cfg.exp_identify;
			_rune_data.exp_return = cfg.exp_return;
			_rune_data.exp_refresh = cfg.exp_refresh;

			memcpy(_rune_data.exp_decompose, cfg.decomposeo_exp, sizeof(cfg.decomposeo_exp));
			memcpy(_rune_data.lvlup_exp, cfg.exp, sizeof(cfg.exp));
			memcpy(_rune_data.equip_req, cfg.equip_req, sizeof(cfg.equip_req));
			memcpy(_rune_data.fixed_prop, cfg.fixed_property, sizeof(cfg.fixed_property));
		}
		else if(dt == DT_LITTLE_PET_UPGRADE_CONFIG)
		{
			const LITTLE_PET_UPGRADE_CONFIG & cfg = *(const LITTLE_PET_UPGRADE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_LITTLE_PET_UPGRADE_CONFIG && &cfg);

			memcpy(_little_pet.feed_info, cfg.feed_pet, sizeof(cfg.feed_pet));

			for(size_t i = 0; i < 5; ++i)
			{
				_little_pet.level_info[i].lvlup_exp = cfg.pet_upgrade_info_list[i].required_exp;
				_little_pet.level_info[i].award_item = cfg.pet_upgrade_info_list[i].award_item;
			}
		}
		else if(dt == DT_RUNE_COMB_PROPERTY)
		{
			const RUNE_COMB_PROPERTY & cfg = *(const RUNE_COMB_PROPERTY*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_RUNE_COMB_PROPERTY && &cfg);

			if(cfg.max_ids <= 0) continue;
			if(cfg.id <= 0) continue;

			for(size_t i = 0; i < _rune_combo_data.size(); ++i)
			{
				if(_rune_combo_data[i].id == cfg.id)
				{
					printf("发现相同id的符文组合, id=%d\n", cfg.id);
				       	continue;
				}
			}
			
			rune_combo_data combo_data;
			combo_data.id = cfg.id;

			for(size_t i = 0; i < (size_t)cfg.max_ids; ++i)
			{
				if(cfg.ids[i] > 0) combo_data.stone_id.push_back(cfg.ids[i]);
			}
			ASSERT((size_t)cfg.max_ids == combo_data.stone_id.size());

			for(size_t i =0; i < 9; ++i)
			{
				if(cfg.addons[i] > 0)
				{
					combo_data.addon_id.push_back(cfg.addons[i]);
				}
			}	

			_rune_combo_data.push_back(combo_data);
		}
		else if(dt == DT_DROP_INTERNAL_CONFIG)
		{
			const DROP_INTERNAL_CONFIG & cfg = *(DROP_INTERNAL_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_DROP_INTERNAL_CONFIG && &cfg);	
			if(cfg.start_time + cfg.time_of_duration * 3600 > g_timer.get_systime())
			{
				count_drop_item item;
				item.template_id = id;
				item.item_num = cfg.item_num;
				item.start_time = cfg.start_time - gmatrix::Instance()->GetServerGMTOff();
				item.time_of_duration = cfg.time_of_duration;
				item.item_id = cfg.item_id;
				item.speak_interval = cfg.remain_speak_interval;

				COUNT_DROP_MAP::iterator iter = _count_drops.find(cfg.item_id);
				if(iter != _count_drops.end())
				{
					iter->second.push_back(item);
				}
				else
				{
					_count_drops[cfg.item_id].push_back(item);
				}
				printf("Global drop count item template_id=%d, id=%d, total_cnt=%d, start_time=%d, time_of_duration=%d\n", id, cfg.item_id, cfg.item_num, cfg.start_time, cfg.time_of_duration);
			}
		}
		else if(dt == DT_PK2012_GUESS_CONFIG)
		{
			const PK2012_GUESS_CONFIG& cfg = *(PK2012_GUESS_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_PK2012_GUESS_CONFIG && &cfg);
			ASSERT(cfg.guess_start_time < cfg.guess_end_time && cfg.accept_award_start_time < cfg.accept_award_end_time && cfg.guess_end_time < cfg.accept_award_start_time);
			ASSERT(cfg.first_id == -1 || (cfg.first_id >= 0 && cfg.first_id < 8));
			ASSERT(cfg.second_id == -1 || (cfg.second_id >= 0 && cfg.second_id < 8));
			ASSERT(cfg.third_id == -1 || (cfg.third_id >= 0 && cfg.third_id < 8));
			ASSERT((cfg.first_id == -1 && cfg.second_id == -1 && cfg.third_id == -1) || (cfg.first_id != cfg.second_id && cfg.first_id != cfg.third_id && cfg.second_id != cfg.third_id));
			_pk_bet_data.first_id = cfg.first_id;
			_pk_bet_data.second_id = cfg.second_id;
			_pk_bet_data.third_id = cfg.third_id;
			_pk_bet_data.guess_start_time = cfg.guess_start_time;
			_pk_bet_data.guess_end_time = cfg.guess_end_time;
			_pk_bet_data.accept_award_start_time = cfg.accept_award_start_time;
			_pk_bet_data.accept_award_end_time = cfg.accept_award_end_time;
			_pk_bet_data.champion_guess_item = cfg.champion_guess_item;
			_pk_bet_data.champion_guess_itemNum = cfg.champion_guess_itemNum;
			_pk_bet_data.champion_guess_award = cfg.champion_guess_award;
			_pk_bet_data.award_back_ratio = cfg.award_back_ratio;
			_pk_bet_data.guess_item = cfg.guess_item;
			_pk_bet_data.guess_item_num = cfg.guess_item_num;
			_pk_bet_data.guess_award_item = cfg.guess_award_item;
			_pk_bet_data.guess_award_item3 = cfg.guess_award_item3;
			_pk_bet_data.guess_award_item2 = cfg.guess_award_item2;
			_pk_bet_data.guess_award_item1 = cfg.guess_award_item1;
		}
		else if(dt == DT_UI_TRANSFER_CONFIG)
		{	
			const UI_TRANSFER_CONFIG& cfg = *(UI_TRANSFER_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_UI_TRANSFER_CONFIG && &cfg);

			for (size_t i = 0; i < sizeof(cfg.trans_point_list)/sizeof(UI_TRANSFER_CONFIG::TRANS_POINT); i++) {
				const UI_TRANSFER_CONFIG::TRANS_POINT& cur = cfg.trans_point_list[i];
				if (cur.map_id != 0) // 策划配置了地图
			   	{
					if (0 == cur.fee) {
						printf("上古世界传送，配置了地图，但没有配置fee");
						return false;
					}
					if (.0 == cur.pos[0] || .0 == cur.pos[1] || .0 == cur.pos[2]) {
						printf("上古世界传送，配置了地图，但pos(x,y,z)全部为零");
						return false;
					}			
				}				

				// 不依赖策划顺序配置
				ui_transfer_config_entry entry;
				entry.map_id = cur.map_id;
				entry.fee = (unsigned int)cur.fee;
				memcpy(entry.pos, cur.pos, sizeof(cur.pos));
			
				_ui_transfer_config.push_back(entry);
			}
		}
		else if(dt == DT_COLLISION_RAID_AWARD_CONFIG)
		{
			const COLLISION_RAID_AWARD_CONFIG & cfg = *(COLLISION_RAID_AWARD_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_COLLISION_RAID_AWARD_CONFIG && &cfg);

			_collision_raid_award.daily_award_item = cfg.daily_award_item;
			memcpy(_collision_raid_award.award_list, cfg.award_list, sizeof(cfg.award_list));

		}
		else if(dt == DT_LIVENESS_CONFIG)
		{
			const LIVENESS_CONFIG & cfg = *(LIVENESS_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_LIVENESS_CONFIG && &cfg);
			
			memcpy(_liveness_cfg.gifts, cfg.gifts, sizeof(cfg.gifts));
			for (int i = 0; i < 15; ++i) {
				_liveness_cfg.type_infos[i].score = cfg.scores[i];
			}	
			for (int i = 0; i < 120; ++i) {
				const LIVENESS_CONFIG::ELEMENT& element = cfg.element_list[i];
				if (element.name[0] == 0) continue; // 无效的

				_liveness_cfg.index2typeid.insert(std::make_pair(i, element.type_activity));
				if (element.type_judge == 0) { // 任务
					_liveness_cfg.taskindexs.insert(std::make_pair(element.require_id, i));	
				} else if (element.type_judge == 1) { // 进入地图
					_liveness_cfg.mapindexs.insert(std::make_pair(element.require_id, i));
				} else if (element.type_judge == 2) { // 特殊类型
					_liveness_cfg.specialindexs.insert(std::make_pair(element.require_id, i));
				}
				_liveness_cfg.type_infos[element.type_activity].indexes.insert(i);
			}
		}
		else if(dt == DT_PROP_ADD_CONFIG)
		{
			const PROP_ADD_CONFIG & cfg = *(PROP_ADD_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_PROP_ADD_CONFIG && &cfg);
			memcpy(prop_add_limit, cfg.prop_add_limit, sizeof(prop_add_limit));	
		}
		else if(dt == DT_KING_WAR_CONFIG)
		{
			const KING_WAR_CONFIG & cfg = *(KING_WAR_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_KING_WAR_CONFIG && &cfg);

			_kingdom_data.kingdom_continue_item = cfg.king_continue_item;
			_kingdom_data.kingdom_continue_item_num = cfg.king_continue_item_num;
			memcpy(&_kingdom_data.monster, cfg.monster, sizeof(cfg.monster));
			memcpy(&_kingdom_data.kingdom_shop, cfg.king_shop, sizeof(cfg.king_shop));
			memcpy(&_kingdom_data.normal_shop, cfg.normal_shop, sizeof(cfg.normal_shop));
		}
		// Youshuang add
		else if(dt == DT_COLORANT_CONFIG)
		{
			const COLORANT_CONFIG& cfg = *(COLORANT_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT( dt == DT_COLORANT_CONFIG && &cfg );
			AddColorProbabilityConfig( cfg );
		}
		else if(dt == DT_FACTION_SHOP_CONFIG)
		{
			const FACTION_SHOP_CONFIG& cfg = *(FACTION_SHOP_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT( dt == DT_FACTION_SHOP_CONFIG && &cfg );
			InsertMallItems( cfg );
			
		}
		else if(dt == DT_FACTION_AUCTION_CONFIG)
		{
			const FACTION_AUCTION_CONFIG& cfg = *(FACTION_AUCTION_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT( dt == DT_FACTION_AUCTION_CONFIG && &cfg );
			InsertAuctionItems( cfg );
		}
		else if(dt == DT_FACTION_MONEY_SHOP_CONFIG)
		{
			const FACTION_MONEY_SHOP_CONFIG& cfg = *(FACTION_MONEY_SHOP_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT( dt == DT_FACTION_MONEY_SHOP_CONFIG && &cfg );
			InsertCashItems( cfg );
		}
		else if(dt == DT_COLORANT_DEFAULT_CONFIG)
		{
			const COLORANT_DEFAULT_CONFIG& cfg = *(COLORANT_DEFAULT_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT( dt == DT_COLORANT_DEFAULT_CONFIG && &cfg );
			SetInitColor( cfg );
		}
		// else if(dt == DT_SIGN_IN_CONFIG)
		// {
			// const SIGN_IN_CONFIG& cfg = *(SIGN_IN_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			// ASSERT( dt == DT_SIGN_IN_CONFIG && &cfg );
			// SetNewYearAward( cfg );
		// }
		// end
		else if(dt == DT_BASHE_AWARD_CONFIG)
		{
			const BASHE_AWARD_CONFIG & cfg = *(BASHE_AWARD_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_BASHE_AWARD_CONFIG && &cfg);

			_bath_award.period = cfg.period;
			float prob = 0.0f;
			for(size_t i = 0; i < 10; ++i)
			{
				prob += cfg.award_info[i].prob;
			}

			if(fabs(1.0f - prob) > 1e-6)
			{
				printf("澡堂泡澡奖励未归一化: id=%d\n", id);
				return false;
			}

			memcpy(&_bath_award.bath_award_info[0], &cfg.award_info[0], sizeof(cfg.award_info));
		}
		else if(dt == DT_RUNE2013_CONFIG)
		{
			const RUNE2013_CONFIG & cfg = *(RUNE2013_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_RUNE2013_CONFIG && &cfg);

			_fuwen_exp_config.max_level = cfg.lv_limit;
			memcpy(&_fuwen_exp_config.lvlup_exp, cfg.lvup_exp, sizeof(cfg.lvup_exp)); 
		}
		else if (dt == DT_BUILDING_REGION_CONFIG)
		{
			const BUILDING_REGION_CONFIG & cfg = *(BUILDING_REGION_CONFIG *)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_BUILDING_REGION_CONFIG && &cfg);
			gmatrix::GetFacBaseInfo().region_tid = id;
		}
		else if (dt == DT_FACTION_TRANSFER_CONFIG)
		{
			const FACTION_TRANSFER_CONFIG & cfg = *(FACTION_TRANSFER_CONFIG *)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_FACTION_TRANSFER_CONFIG && &cfg);
			gmatrix::GetFacBaseInfo().region_transfer_tid = id;
		}
		else if(dt == DT_PERSONAL_LEVEL_AWARD_CONFIG)
		{
			const PERSONAL_LEVEL_AWARD_CONFIG & cfg = *(PERSONAL_LEVEL_AWARD_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_PERSONAL_LEVEL_AWARD_CONFIG && &cfg);

			memcpy(_cs_6v6_personal_level_award_list, cfg.personal_level_award_info, sizeof(cfg.personal_level_award_info));
		}
		else if(dt == DT_TEAM_SEASON_AWARD_CONFIG)
		{
			const TEAM_SEASON_AWARD_CONFIG & cfg = *(TEAM_SEASON_AWARD_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_TEAM_SEASON_AWARD_CONFIG && &cfg);

			memcpy(_cs_6v6_team_award_list, cfg.team_season_awrd_info, sizeof(cfg.team_season_awrd_info));
		}
		else if(dt == DT_WEEK_CURRNCY_AWARD_CONFIG)
		{
			const WEEK_CURRNCY_AWARD_CONFIG & cfg= *(WEEK_CURRNCY_AWARD_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_WEEK_CURRNCY_AWARD_CONFIG && &cfg);

			memcpy(_cs_6v6_exchange_award_list, cfg.week_currncy_awrd_info, sizeof(cfg.week_currncy_awrd_info));
		}
		else if(dt == DT_CROSS6V6_ITEM_EXCHANGE_CONFIG)
		{
			const CROSS6V6_ITEM_EXCHANGE_CONFIG & cfg= *(CROSS6V6_ITEM_EXCHANGE_CONFIG*)dataman.get_data_ptr(id, ID_SPACE_CONFIG, dt);
			ASSERT(dt == DT_CROSS6V6_ITEM_EXCHANGE_CONFIG && &cfg);

			memcpy(_cs_6v6_item_exchange_list, cfg.cross6v6_item_exchange_info, sizeof(cfg.cross6v6_item_exchange_info));
		}
	}
		
	if(!exp_flag)
	{
		printf("模板里缺少升级曲线表\n");
		//return false;
	}

	if(!adj_flag)
	{
		printf("模板里缺少数据修正表\n");
		//return false;
	}
	return true;
}

// Youshuang add
int player_template::GetID( int color, int quality ) const 
{
	QUALITY_MAP::const_iterator quality_iter = _quality_table.find( quality );
	if( quality_iter == _quality_table.end() ){ return -1; }
	COLOR_MAP color_table = quality_iter->second;
	COLOR_MAP::const_iterator color_iter = color_table.find( color );
	if( color_iter != color_table.end() ){ return color_iter->second.id; }
	return -1;
}
	
int player_template::GetProbability( int color, int quality ) const
{
	QUALITY_MAP::const_iterator quality_iter = _quality_table.find( quality );
	if( quality_iter == _quality_table.end() ){ return -1; }
	COLOR_MAP color_table = quality_iter->second;
	COLOR_MAP::const_iterator color_iter = color_table.find( color );
	if( color_iter != color_table.end() ){ return color_iter->second.probability; }
	return -1;
}

int player_template::GetBestQuality() const
{
	int best = -1;
	for( QUALITY_MAP::const_iterator iter = _quality_table.begin(); iter != _quality_table.end(); iter++ )
	{
		int quality = iter->first;
		if( best == -1 )
		{
			best = quality;
		}
		else if( best > quality )
		{ 
			best = quality; 
		}
	}
	return best;
}

int player_template::HasQuality( int quality ) const
{
	if( quality == 5 ){ return QUALITY_SPECIAL; }
	QUALITY_MAP::const_iterator quality_iter = _quality_table.find( quality );
	return ( quality_iter != _quality_table.end() ) ? QUALITY_OK : QUALITY_WRONG;
}

bool player_template::HasColor( int color ) const
{
	QUALITY_MAP::const_iterator quality_iter = _quality_table.begin();
	if( quality_iter == _quality_table.end() )
	{
		return false;
	}
	COLOR_MAP color_table = quality_iter->second;
	COLOR_MAP::const_iterator color_iter = color_table.find( color );
	return ( color_iter != color_table.end() );
}

int player_template::GetBetterQuality( int quality ) const
{
	int res = GetBestQuality();
	for( QUALITY_MAP::const_iterator iter = _quality_table.begin(); iter != _quality_table.end(); iter++ )
	{
		if( ( iter->first < quality ) && ( res < iter->first ) )
		{
			res = iter->first;
		}
	}
	return res;
}

void player_template::AddColorProbabilityConfig( const COLORANT_CONFIG& cfg )
{
	DATA_TYPE dt1;
	int colorant_cnt = sizeof( cfg.colorant_info ) / sizeof( COLORANT_CONFIG::colorant_prop );
	for( int i = 0; i < colorant_cnt; ++i )
	{
		if( cfg.colorant_info[i].colorant_item_id <= 0 )
		{
			continue;
		}
		const COLORANT_ITEM_ESSENCE &ess1 = *(const COLORANT_ITEM_ESSENCE*)gmatrix::GetDataMan().get_data_ptr( cfg.colorant_info[i].colorant_item_id, ID_SPACE_ESSENCE, dt1 );
		if( dt1 != DT_COLORANT_ITEM_ESSENCE || &ess1 == NULL)
		{
			ASSERT(false);
		}
		int color = ess1.color;
		int quality = ess1.quality;
		fashion_colorant_probability tmp( cfg.colorant_info[i].colorant_item_id, ( int )( cfg.colorant_info[i].probability * 10000 ) );
		_quality_table[quality][color] = tmp;
	}

}

int player_template::GetTotalProbability() const
{
	int total = 0;
	for( QUALITY_MAP::const_iterator iter = _quality_table.begin(); iter != _quality_table.end(); iter++ )
	{
		total += GetTotalProbabilityByQuality( iter->second );
	}
	return total;
}
	
int player_template::GetTotalProbabilityByQuality( const COLOR_MAP& colors ) const
{
	int ret = 0;
	for( COLOR_MAP::const_iterator iter = colors.begin(); iter != colors.end(); iter++ )
	{
		ret += iter->second.probability;
	}
	return ret;
}

int player_template::GetResultColor( int quality, int orignial_quality, const std::set<int>& colors, int colorant_cnt ) const
{
	QUALITY_MAP::const_iterator quality_iter = _quality_table.find( quality );
	if( quality_iter == _quality_table.end() ){ return -1; }
	
	COLOR_MAP color_table = quality_iter->second;
	int total = GetTotalProbabilityByQuality( color_table );
	if( quality == GetBestQuality() && orignial_quality == quality && colorant_cnt > 2 )
	{
		for( std::set<int>::iterator iter = colors.begin(); iter != colors.end(); iter++ )
		{
			total -= GetProbability( *iter, quality );
		}
	}
	if( total < 0 )
	{
		return -1;
	}
	int res = abase::Rand( 0, total );
	for( COLOR_MAP::const_iterator color_iter = color_table.begin(); color_iter != color_table.end(); color_iter++ )
	{
		if( quality == GetBestQuality() && orignial_quality == quality && colorant_cnt > 2 && colors.find( color_iter->first ) != colors.end() )
		{
			continue;
		}
		else
		{
			res -= GetProbability( color_iter->first, quality );
		}
		if( res < 0 )
		{
			return color_iter->first;
		}
	}
	return -1;
}

int player_template::GetRandomGoodsID() const
{
	int total = GetTotalProbability();
	if( total < 1 ){ return -1; }
	int res = abase::Rand( 0, total - 1 );
	for( QUALITY_MAP::const_iterator quality_iter = _quality_table.begin(); quality_iter != _quality_table.end(); quality_iter++ )
	{
		COLOR_MAP color_table = quality_iter->second;
		for( COLOR_MAP::const_iterator color_iter = color_table.begin(); color_iter != color_table.end(); color_iter++ )
		{
			res -= color_iter->second.probability;
			if( res < 0 )
			{
				return color_iter->second.id;
			}
		}
	}
	return -1;
}

void player_template::InsertMallItems( const FACTION_SHOP_CONFIG& cfg )
{
	int item_cnt = sizeof( cfg.faction_item_list ) / sizeof( cfg.faction_item_list[0] );
	for( int i = 0; i < item_cnt; ++i )
	{
		if( cfg.faction_item_list[i].item_id > 0 )
		{
			facbase_mall_item_info mall_item;
			mall_item.idx = cfg.faction_item_list[i].item_index;
			mall_item.tid = cfg.faction_item_list[i].item_id;
			mall_item.build_id = cfg.faction_item_list[i].build_id;
			mall_item.build_level = cfg.faction_item_list[i].build_level;
			mall_item.needed_faction_coupon = cfg.faction_item_list[i].contribution;
			mall_items[mall_item.idx] = mall_item;
		}
	}
}

void player_template::InsertAuctionItems( const FACTION_AUCTION_CONFIG& cfg)
{

	int item_cnt = sizeof( cfg.faction_auction_list ) / sizeof( cfg.faction_auction_list[0] );
	for( int i = 0; i < item_cnt; ++i )
	{
		if( cfg.faction_auction_list[i].item_id > 0 )
		{
			facbase_auction_item_info auction_item;
			auction_item.tid = cfg.faction_auction_list[i].item_id;
			auction_item.needed_faction_coupon = cfg.faction_auction_list[i].baseprice;
			auction_item.aucitontime = cfg.faction_auction_list[i].aucitontime;
			auction_items[auction_item.tid] = auction_item;
		}
	}
}

void player_template::InsertCashItems( const FACTION_MONEY_SHOP_CONFIG& cfg)
{
	int item_cnt = sizeof( cfg.faction_money_item_list ) / sizeof( cfg.faction_money_item_list[0] );
	for( int i = 0; i < item_cnt; ++i )
	{
		if( cfg.faction_money_item_list[i].item_index > 0 )
		{
			facbase_cash_item_info cash_item;
			cash_item.idx = cfg.faction_money_item_list[i].item_index;
			cash_item.build_id = cfg.faction_money_item_list[i].build_id;
			cash_item.build_level = cfg.faction_money_item_list[i].build_leve;
			cash_item.needed_faction_cash = cfg.faction_money_item_list[i].price;
			cash_item.cooldown_time = cfg.faction_money_item_list[i].cooltime;
			cash_item.award_type = cfg.faction_money_item_list[i].type;
			cash_item.auition_item_id = cfg.faction_money_item_list[i].auition_item_id;
			cash_item.controller_id = cfg.faction_money_item_list[i].pen_controller_id;
			cash_item.exp_multi = cfg.faction_money_item_list[i].exp_multi;
			cash_item.multi_exp_time = cfg.faction_money_item_list[i].multi_exp_time;
			cash_items[cash_item.idx] = cash_item;
		}
	}
}

const facbase_mall_item_info* player_template::GetItemInfoByIndex( int idx ) const
{
	MALL_ITEM_INFO::const_iterator iter = mall_items.find( idx );
	if( iter == mall_items.end() )
	{
		return NULL;
	}
	return &(iter->second);
}

const facbase_auction_item_info* player_template::GetAuctionItemInfoByID( int item_id ) const
{
	AUCTION_ITEM_INFO::const_iterator iter = auction_items.find( item_id );
	if( iter == auction_items.end() )
	{
		return NULL;
	}
	return &(iter->second);
}

const facbase_cash_item_info* player_template::GetCashItemInfoByIndex( int idx ) const
{
	CASH_ITEM_INFO::const_iterator iter = cash_items.find( idx );
	if( iter == cash_items.end() )
	{
		return NULL;
	}
	return &(iter->second);
}

void player_template::SetInitColor( const COLORANT_DEFAULT_CONFIG& cfg )
{
	int item_cnt = sizeof( cfg.colorant ) / sizeof( cfg.colorant[0] );
	for( int i = 0; i < item_cnt; ++i )
	{
		if( cfg.colorant[i] > 0 )
		{
			init_colorant_items.push_back( cfg.colorant[i] );
		}
	}
}

int player_template::GetInitColor() const
{
	int color_cnt = init_colorant_items.size();
	if( color_cnt < 1 ){ return -1; }
	int res_color = abase::Rand( 0, color_cnt - 1 );
	return init_colorant_items[res_color];
}

// void player_template::SetNewYearAward( const SIGN_IN_CONFIG& cfg )
// {
	// time_t t1 = time(NULL);
	// struct tm tm1;
	// localtime_r( &t1, &tm1 );
	// int tz_adjust = -tm1.tm_gmtoff;    //编辑器保存的时间是UTC,倒计时时间段根据gs运行的时区进行调整
	
	// _newyear_award_template.start = cfg.sign_start_time + tz_adjust;
	// _newyear_award_template.end_sign = cfg.sign_end_time + tz_adjust;
	// _newyear_award_template.end_fetch_award = cfg.exchange_end_time + tz_adjust;

	// _newyear_award_template.sign_score = cfg.sign_score;
	// _newyear_award_template.sign_award_item_id = cfg.sign_package;
	// int item_cnt = sizeof( cfg.package_list ) / sizeof( cfg.package_list[0] );
	// for( int i = 0; i < item_cnt; ++i )
	// {
		// if( cfg.package_list[i].package_id > 0 )
		// {
			// newyear_award_item tmp;
			// tmp.score_needed = cfg.package_list[i].score_consume;
			// tmp.fetch_count = cfg.package_list[i].exchange_count;
			// tmp.award_item_id = cfg.package_list[i].package_id;
			// _newyear_award_template.items.push_back( tmp );
		// }
	// }
// }
// end

bool 
player_template::TrySetOnlineGiftBagInfo(online_giftbag_config & data)
{
	//检查参数逻辑
	if(data.start_time_per_interval <= 0 || data.end_time_per_interval < data.start_time_per_interval) 
	{
		return false;
	}
	if(data.overdue_time <= 0 || data.overdue_time < data.end_time_per_interval)
	{
		return false;
	}

	//获取时区信息
	time_t t1 = time(NULL);
	struct tm tm1;
	localtime_r(&t1, &tm1);
	int tz_adjust = -tm1.tm_gmtoff;    //编辑器保存的时间是UTC,倒计时时间段根据gs运行的时区进行调整
	//调整时区
	data.start_time_per_interval	+= tz_adjust;
	data.end_time_per_interval		+= tz_adjust;
	data.overdue_time				+= tz_adjust;

	//时间段时间和每日的时间不能共存
	switch(data.award_type)
	{
		//时间段
		case 0:
		{
			//时区调整已经在前面处理
		}
		break;

		//每日
		case 1:
		{
			if(data.start_time_per_interval < 0 || data.end_time_per_interval <= data.start_time_per_interval) 
			{
				return false;
			}
			if( data.start_time_per_day < 0 || data.end_time_per_day > SECONDS_OF_ALL_DAY
			 || data.end_time_per_day <= data.start_time_per_day ) 
			{
				return false;
			}
		}
		break;

		default:
			return false;
	}

	//等级上下限
	if(data.require_min_level < 0) 
	{
		return false;
	}

	if(data.require_max_level > 0 && data.require_max_level < data.require_min_level)
	{
		return false;
	}

	//性别限制，0-男，1-女，2-男女都可
	switch(data.require_gender)
	{
		case 0:
		case 1:
		case 2:
			break;

		default:
			return false;
	}

	//种族限制，0-人族，1-兽族， 2-都可以
	switch(data.require_race)
	{
		case 0:
		case 1:
		case 2:
			break;

		default:
			return false;
	}

	return true;
}

int player_template::__GetPropAddLimit(int idx)
{
	if(idx < 0 || idx >= PROP_ADD_NUM) return -1;
	return prop_add_limit[idx];
}

bool 
player_template::__LevelUp(int cls, int oldlvl,player_extend_prop & data, gactive_imp * pImp) const
{
	if(((size_t)cls) >= QUSER_CLASS_COUNT) return false; 
	if(oldlvl <=0 || oldlvl > 10000) return false;

	pImp->_faction &= FACTION_DYNAMIC_MASK;
	pImp->_faction |= _class_list[cls].faction;	//对于NPC会不会有问题??
	pImp->_enemy_faction = _class_list[cls].enemy_faction;	//这里直接赋值应该可以

	return __BuildPlayerData(cls, oldlvl + 1, pImp->GetDeityLevel(), data, pImp);
}

bool
player_template::__DeityLevelUp(int cls,int dt_level, gactive_imp * pImp) const
{
	if(dt_level < 0 || dt_level > MAX_DEITY_LEVEL) return false; 

	return __BuildPlayerData(cls, pImp->GetObjectLevel(), dt_level +1, pImp->_base_prop,pImp);
}	

bool player_template::__IncPropAdd(int cls, gactive_imp * pImp) const
{
	return __BuildPlayerData(cls, pImp->GetObjectLevel(), pImp->GetDeityLevel(), pImp->_base_prop, pImp);
}

int  
player_template::__Rollback(int cls, player_extend_prop & data) const
{
	return 0;
}

int  
player_template::__Rollback(int cls, player_extend_prop & data,int str, int agi ,int vit, int eng) const
{
	return 0;
}

int64_t
player_template::__GetLvlupExp(size_t reborn_count, int cur_lvl) const
{
	if(reborn_count >= MAX_REBORN_COUNT + 1)
	{
		ASSERT(false);
		return cur_lvl * cur_lvl * 500;
	}
	
	if(((size_t)cur_lvl) < (size_t)GetMaxLevelLimit())
	{
		return _exp_list[reborn_count][cur_lvl];
	}
	return cur_lvl * cur_lvl * 500;
}

int64_t
player_template::__GetDeityLvlupExp(int cur_lvl) const
{
	ASSERT(cur_lvl >= 0 && cur_lvl <= MAX_DEITY_LEVEL && cur_lvl < 100); 
	return _deity_exp_list[cur_lvl].deity_exp; 
}

int64_t
player_template::__GetDeityLvlupPunishExp(int cur_lvl) const
{
	ASSERT(cur_lvl >= 0 && cur_lvl <= MAX_DEITY_LEVEL && cur_lvl < 100); 
	return _deity_exp_list[cur_lvl].failure_exp; 
}

float
player_template::__GetDeityLvlupProb(int cur_lvl) const
{
	ASSERT(cur_lvl >= 0 && cur_lvl <= MAX_DEITY_LEVEL && cur_lvl < 100); 
	return _deity_exp_list[cur_lvl].levelup_prob; 
}


int  
player_template::__GetPetLvlupExp(int cur_lvl) const
{
	if(((size_t)cur_lvl) <= (size_t)GetMaxLevelLimit())
	{
		return _pet_exp_list[cur_lvl];
	}
	return cur_lvl * cur_lvl * 500;
}

void 
player_template::BuildExProp(const prop_data &base, const prop_data &lvlup, int cur_lvl,int base_level, player_extend_prop &prop)
{
#define CALC_EX_PROP(base, lvlup ,name) (base.name + lvlup.name * (cur_lvl - base_level))
	memset(&prop,0,sizeof(prop));
	prop.max_hp 		= (int)(CALC_EX_PROP(base,lvlup,hp));
	prop.max_mp 		= (int)(CALC_EX_PROP(base,lvlup,mp)); 
	prop.crit_rate		= (int)(CALC_EX_PROP(base,lvlup,crit_rate)*10);
	prop.crit_damage	= CALC_EX_PROP(base,lvlup,crit_damage);

	int damage = (int)(CALC_EX_PROP(base,lvlup,dmg));
	prop.damage_low 	= damage;
	prop.damage_high	= damage;
	prop.defense 		= (int)(CALC_EX_PROP(base,lvlup,defense));
	prop.attack 		= (int)(CALC_EX_PROP(base,lvlup,attack));
	prop.armor 		= (int)(CALC_EX_PROP(base,lvlup,armor));
	prop.resistance[0]	= (int)(CALC_EX_PROP(base,lvlup,anti_stunt));
	prop.resistance[1]	= (int)(CALC_EX_PROP(base,lvlup,anti_weak ));
	prop.resistance[2]	= (int)(CALC_EX_PROP(base,lvlup,anti_slow ));
	prop.resistance[3]	= (int)(CALC_EX_PROP(base,lvlup,anti_silence));
	prop.resistance[4]	= (int)(CALC_EX_PROP(base,lvlup,anti_sleep));
	prop.resistance[5]	= (int)(CALC_EX_PROP(base,lvlup,anti_twist));

}

bool 
player_template::__BuildPlayerData(int cls, int cur_lvl, int cur_dt_lvl, player_extend_prop & prop, gactive_imp * pImp) const
{
	if(((size_t)cls) >= QUSER_CLASS_COUNT) 
	{
		ASSERT(false);
		return false; 
	}
	const q_class_data & cls_data = _class_list[cls];
#define CALC_PLAYER_PROP(data,name) (data.base.name + data.lvlup.name * (cur_lvl - data.base_level))
	memset(&prop,0,sizeof(prop));
	prop.max_hp 		= (int)(CALC_PLAYER_PROP(cls_data,hp));
	prop.max_mp 		= (int)(CALC_PLAYER_PROP(cls_data,mp)); 
	prop.hp_gen[0]  	= cls_data.hp_gen[0];
	prop.hp_gen[1]  	= cls_data.hp_gen[1];
	prop.hp_gen[2]  	= cls_data.hp_gen[2];
	prop.hp_gen[3]  	= cls_data.hp_gen[3];
	prop.mp_gen[0]  	= cls_data.mp_gen[0];
	prop.mp_gen[1]  	= cls_data.mp_gen[1];
	prop.mp_gen[2]  	= cls_data.mp_gen[2];
	prop.mp_gen[3]  	= cls_data.mp_gen[3];
	prop.walk_speed		= cls_data.walk_speed;
	prop.run_speed		= cls_data.run_speed;
	prop.crit_rate		= (int)(CALC_PLAYER_PROP(cls_data,crit_rate));
	prop.crit_damage	= CALC_PLAYER_PROP(cls_data,crit_damage);

	int damage = (int)(CALC_PLAYER_PROP(cls_data,dmg));
	prop.damage_low 	= damage;
	prop.damage_high	= damage;
	prop.defense 		= (int)(CALC_PLAYER_PROP(cls_data,defense));
	prop.attack 		= (int)(CALC_PLAYER_PROP(cls_data,attack));
	prop.armor 		= (int)(CALC_PLAYER_PROP(cls_data,armor));
	prop.resistance[0]	= (int)(CALC_PLAYER_PROP(cls_data,anti_stunt));
	prop.resistance[1]	= (int)(CALC_PLAYER_PROP(cls_data,anti_weak ));
	prop.resistance[2]	= (int)(CALC_PLAYER_PROP(cls_data,anti_slow ));
	prop.resistance[3]	= (int)(CALC_PLAYER_PROP(cls_data,anti_silence));
	prop.resistance[4]	= (int)(CALC_PLAYER_PROP(cls_data,anti_sleep));
	prop.resistance[5]	= (int)(CALC_PLAYER_PROP(cls_data,anti_twist));
	prop.invisible_rate	= cur_lvl;
	prop.anti_invisible_rate= cur_lvl;
	
	//加入转生属性修正
	int r_prof[4];
	int r_level[4];
	int reborn_count = pImp->QueryRebornInfo(r_level + 1,r_prof);
	r_level[0] = cur_lvl;
	gmatrix::GetRebornBonusMan().ActivatePropBonus(reborn_count, r_level, r_prof, prop);

	//加入封神属性纠正
	if(cur_dt_lvl > 0) __ActivateDeityProp(cur_dt_lvl-1, prop);

	//加入丹药增强属性修正
	__ActivatePropAdd(prop, (gplayer_imp*)pImp);

	property_policy::UpdateInvisible(pImp);
	
	return true;
}

void
player_template::__ActivateDeityProp(int dt_level, player_extend_prop & prop) const
{
	if(dt_level < 0 || dt_level >= MAX_DEITY_LEVEL) return;

	prop.max_dp += _deity_prop_data[dt_level].dp;
	prop.max_hp += _deity_prop_data[dt_level].hp;
	prop.max_mp += _deity_prop_data[dt_level].mp;
	prop.damage_low += _deity_prop_data[dt_level].dmg;
	prop.damage_high += _deity_prop_data[dt_level].dmg;
	prop.defense += _deity_prop_data[dt_level].defense;
	prop.deity_power += _deity_prop_data[dt_level].deity_power;
	prop.resistance[0] += _deity_prop_data[dt_level].anti_stunt;
	prop.resistance[1] += _deity_prop_data[dt_level].anti_weak;
	prop.resistance[2] += _deity_prop_data[dt_level].anti_slow;
	prop.resistance[3] += _deity_prop_data[dt_level].anti_silence;
	prop.resistance[4] += _deity_prop_data[dt_level].anti_sleep;
	prop.resistance[5] += _deity_prop_data[dt_level].anti_twist;
	prop.dp_gen[0] = _deity_prop_data[dt_level].dpgen[0];
	prop.dp_gen[1] = _deity_prop_data[dt_level].dpgen[1];
	prop.dp_gen[2] = _deity_prop_data[dt_level].dpgen[2];
	prop.dp_gen[3] = _deity_prop_data[dt_level].dpgen[3];
}

void player_template::__ActivatePropAdd(player_extend_prop & prop, gplayer_imp* pImp) const
{
	prop.damage_low += pImp->GetPropAdd(PROP_ADD_DAMAGE);
	prop.damage_high += pImp->GetPropAdd(PROP_ADD_DAMAGE);
	prop.defense += pImp->GetPropAdd(PROP_ADD_DEF);
	prop.max_hp += pImp->GetPropAdd(PROP_ADD_HP);
	prop.max_mp += pImp->GetPropAdd(PROP_ADD_MP);
	prop.attack += pImp->GetPropAdd(PROP_ADD_ATTACK);
	prop.armor += pImp->GetPropAdd(PROP_ADD_ARMOR);
	prop.crit_rate += pImp->GetPropAdd(PROP_ADD_CRIT_RATE);
	prop.anti_crit += pImp->GetPropAdd(PROP_ADD_CRIT_ANTI);
	prop.crit_damage += pImp->GetPropAdd(PROP_ADD_CRIT_DAMAGE) * 0.001f;
	prop.anti_crit_damage += pImp->GetPropAdd(PROP_ADD_CRIT_DAMAGE_ANTI) * 0.001f;
	prop.skill_attack_rate += pImp->GetPropAdd(PROP_ADD_SKILL_ATTACK_RATE);
	prop.skill_armor_rate += pImp->GetPropAdd(PROP_ADD_SKILL_ARMOR_RATE);
	prop.resistance[0] += pImp->GetPropAdd(PROP_ADD_RESISTANCE_1);
	prop.resistance[1] += pImp->GetPropAdd(PROP_ADD_RESISTANCE_2);
	prop.resistance[2] += pImp->GetPropAdd(PROP_ADD_RESISTANCE_3);
	prop.resistance[3] += pImp->GetPropAdd(PROP_ADD_RESISTANCE_4);
	prop.resistance[4] += pImp->GetPropAdd(PROP_ADD_RESISTANCE_5);
	prop.cult_defense[0] += pImp->GetPropAdd(PROP_ADD_CULT_DEFANCE_1);
	prop.cult_defense[1] += pImp->GetPropAdd(PROP_ADD_CULT_DEFANCE_2);
	prop.cult_defense[2] += pImp->GetPropAdd(PROP_ADD_CULT_DEFANCE_3);
	prop.cult_attack[0] += pImp->GetPropAdd(PROP_ADD_CULT_ATTACK_1);
	prop.cult_attack[1] += pImp->GetPropAdd(PROP_ADD_CULT_ATTACK_2);
	prop.cult_attack[2] += pImp->GetPropAdd(PROP_ADD_CULT_ATTACK_3);
}

void 
player_template::__InitPlayerData(int cls,gactive_imp * pImp)
{
	if(((size_t)cls) >= QUSER_CLASS_COUNT) 
	{
		ASSERT(false);
		return; 
	}
	
	pImp->_faction = _class_list[cls].faction;
	pImp->_enemy_faction = _class_list[cls].enemy_faction;
	
	__BuildPlayerData(cls, pImp->GetObjectLevel(), pImp->GetDeityLevel(), pImp->_base_prop,pImp);
}

bool
player_template::__GetTownPosition(const A3DVECTOR & source, A3DVECTOR & target)
{
	for(size_t i = 0; i < _region_list.size(); i ++)
	{
		if(_region_list[i].second.IsIn(source.x,source.z))
		{
			target = _region_list[i].first;
			return true;
		}
	}
	return false;
}

bool 
player_template::__IncProduceExp(int & produce_level, int & produce_exp, int exp )
{	
	if((size_t)(produce_level-1) >= (size_t)GetMaxProduceLevel()) return false;
	if(produce_level == GetMaxProduceLevel()) return false; 	//已经到达满级， 不再获得经验
	if(exp <=0) return false;
	produce_exp += exp;
	do 
	{
		int lexp = _produce_levelup_exp[produce_level];
		if(lexp > produce_exp) break; 
		produce_exp -= lexp;
		produce_level ++;
		if((size_t)produce_level >= (size_t)GetMaxProduceLevel()) 
		{
			//满级
			produce_exp = 0;
			break;
		}
	}while(1);
	return true;
}


bool 
player_template::__ProduceItem(int & produce_level, int & produce_exp, size_t recipe_level)
{	
	if((size_t)(produce_level-1) >= (size_t)GetMaxProduceLevel()) return false;
	if(recipe_level - 1 >= (size_t)GetMaxProduceLevel()) return false;

	if(produce_level == GetMaxProduceLevel()) return false; 	//已经到达满级， 不再获得经验
	int exp = _produce_exp[produce_level-1][recipe_level-1];
	if(exp <=0) return false;
	return __IncProduceExp(produce_level, produce_exp, exp);
}

int 
player_template::CalcPKReputation(int repa, int repd)
{
	if(repa <= 0) repa = 1;
	if(repd <= 0) repd = 1;
	float loga = log(repa*0.01f)*0.43429448190325182765112891891661;
	float logd = log(repd*0.01f)*0.43429448190325182765112891891661;
	if(loga <1) loga = 1;
	if(loga >5) loga = 5;
	if(logd <1) logd = 1;
	if(logd >5) logd = 5;
	loga *= loga;
	logd *= logd;
	int max = (int)(600 * loga);
	int rep = (int)(100 + 100 * ((float)repd/(float)repa));
	if(rep > max) rep = max;
	rep = (int)(rep * ((logd - loga)*0.1 + 2.5));
	if(rep < 0) rep = 1;
	if(rep > repd) rep = repd;
	return rep;
}


#define	MAX_BASE_HP  	400
#define	MAX_BASE_MP 	400
#define MAX_BASE_DAMAGE	50
#define MAX_BASE_DEFENSE 155
#define MAX_BASE_ARMOR 	50

bool 
player_template::__CheckData(int cls, int level, const player_extend_prop &data)
{
	return true;
}

int 
player_template::__GetSNSRequiredMoney(char m_type, char o_type)
{
	int message_type = (int)m_type;
	int op_type = (int)o_type;
	if(message_type > 3 || message_type < 0){
		return 0;
	}
	switch(op_type)
	{
		case SNS_OP_PRESSMESSAGE:
			return _sns_config[message_type].fee_press_msg;
		case SNS_OP_APPLY:
			return _sns_config[message_type].fee_apply_msg;
		case SNS_OP_APPLYMSG:
			return _sns_config[message_type].fee_response_msg;
		default:
			return 0;
	}	
}

int
player_template::__GetSNSRequiredItemID(char m_type, char o_type)
{
	int message_type = (int)m_type;
	int op_type = (int)o_type;
	if(message_type > 3 || message_type < 0){
		return -1;
	}
	switch(op_type)
	{
		case SNS_OP_PRESSMESSAGE:
			return _sns_config[message_type].id_press_msg_item;
		case SNS_OP_APPLY:
			return _sns_config[message_type].id_apply_msg_item;
		case SNS_OP_APPLYMSG:
			return _sns_config[message_type].id_response_msg_item;
		case SNS_OP_AGREE:
			return _sns_config[message_type].id_support_msg_item;
		case SNS_OP_DENY:
			return _sns_config[message_type].id_against_msg_item;
		default:
			return -1;
	}	
}

int
player_template::__GetSNSRequiredItemID2(char m_type, char o_type)
{
	int message_type = (int)m_type;
	int op_type = (int)o_type;
	if(message_type > 3 || message_type < 0){
		return -1;
	}
	switch(op_type)
	{
		case SNS_OP_PRESSMESSAGE:
			return _sns_config[message_type].id2_press_msg_item;
		default:
			return -1;
	}	
}

int
player_template::__GetSNSRequiredTaskID(char m_type, char o_type, int charm)
{
	int message_type = (int)m_type;
	int op_type = (int)o_type;
	if(message_type > 3 || message_type < 0){
		return -1;
	}
	switch(op_type)
	{
		case SNS_OP_PRESSMESSAGE:
			return _sns_config[message_type].id_press_msg_task;
		case SNS_OP_APPLY:
			return _sns_config[message_type].id_apply_msg_task;
		case SNS_OP_AGREE:
			return _sns_config[message_type].id_support_msg_task;
		case SNS_OP_DENY:
			return _sns_config[message_type].id_against_msg_task;
		case SNS_OP_ACCEPTAPPLY:
			if(charm > 0 && charm <=500){
				return _sns_config[message_type].id_accept_apply_task1; 
			}
			else if(charm >500 && charm <=5000){
				return _sns_config[message_type].id_accept_apply_task2;
			}
			else if(charm >5000 && charm <10000){
				return _sns_config[message_type].id_accept_apply_task3;
			}
			return -1;
		default:
			return -1;
	}
}

int 
player_template::__GetRankByScore(int map_id, int score)
{
	if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
	if(score < 0) return -1;

	std::map<int, std::vector<battle_rank> >::iterator iter = _rank_score.find(map_id);
	if(iter == _rank_score.end()) return -1;
	
	for(size_t i = (iter->second).size(); i > 0; )
	{
		--i;
		if(score >= (iter->second)[i].score)
		{
			return i+1;	
		}	
	}
	return -1;
}

int
player_template::__GetTaskIDByRank(int map_id, int rank)
{
	if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
	if(rank < 0 || rank > 10) return -1;
	
	std::map<int, std::vector<battle_rank> >::iterator iter = _rank_score.find(map_id);
	if(iter == _rank_score.end()) return -1;

	return (iter->second)[rank-1].task_id;
}

int
player_template::__GetDeathDropItemID(int map_id, int rank)
{
	if(map_id < MIN_BATTLE_MAPID || map_id > MAX_BATTLE_MAPID) return -1;
	if(rank < 0 || rank > 10) return -1;

	std::map<int, RANK_DROP_ITEM>::iterator iter = _death_drop_item.find(map_id);
	if(iter == _death_drop_item.end()) return -1;

	RANK_DROP_ITEM::iterator iter2 = (iter->second).find(rank-1);
        if(iter2 == (iter->second).end()) return -1;

       	float *prob = &(iter2->second.begin()->probability);	
	int index = abase::RandSelect(prob, sizeof(death_drop_item), (iter2->second).size());	

	if(index < 0 || index >= 10) return -1;
	return (iter2->second)[index].id;	
}

bool
player_template::__CheckOriginalInfo(int table_id, int monster_id)
{
	std::map<int, std::vector<original_shape> >::iterator iter = _original_shape_info.find(table_id);
	if(iter == _original_shape_info.end() ) return false;
	for(size_t i = 0; i < (iter->second).size(); ++i)
	{
		if((iter->second)[i].monster_id == monster_id) return true;
	}
	return false;
}
	
int
player_template::__GetOriginalID(int table_id, int monster_id, int & target_type)
{
	std::map<int, std::vector<original_shape> >::iterator iter = _original_shape_info.find(table_id);
	if(iter == _original_shape_info.end() ) return -1;
	for(size_t i = 0; i < (iter->second).size(); ++i)
	{
		if((iter->second)[i].monster_id == monster_id)
		{
			int index = abase::RandSelect(&(iter->second)[i].original_info[0].probability, 
					sizeof((iter->second)[i].original_info[0]), 3);
			target_type = (iter->second)[i].original_info[index].original_type;
			return (iter->second)[i].original_info[index].original_id;
		}
	}
	return -1;
}

bool player_template::__GetCountDrop(int itemId, count_drop_item& item)
{
	COUNT_DROP_MAP::iterator it = _count_drops.find(itemId);
	if(it == _count_drops.end())
	{
		return false;
	}
	std::vector<count_drop_item>& items = it->second;
	std::vector<count_drop_item>::iterator it2 = items.begin();
	int curr_time = g_timer.get_systime();
	for(; it2 != items.end(); ++ it2)
	{
		count_drop_item& cdi = *it2;
		if(curr_time >= cdi.start_time  && curr_time < cdi.start_time + cdi.time_of_duration * 3600) 
		{
			memcpy(&item, &cdi, sizeof(count_drop_item));
			return true;
		}
	}
	return false;
}

void player_template::__GetAllCountDrops(std::vector<count_drop_item>& retitems)
{
	COUNT_DROP_MAP::iterator it = _count_drops.begin();
	for(; it != _count_drops.end(); ++ it)
	{
		std::vector<count_drop_item>& items = it->second;
		std::vector<count_drop_item>::iterator it2 = items.begin();
		for(size_t i = 0; i < items.size(); i ++)
		{
			retitems.push_back(items[i]);
		}
	}
}

void 
property_policy::UpdatePlayer(int cls,gactive_imp * pImp)
{
	UpdatePlayerBaseProp(cls,pImp);
	UpdateSpeed(pImp);
	UpdateAttack(cls,pImp);
	UpdateMagic(pImp);
	UpdateDefense(cls,pImp);
}

void 
property_policy::UpdateNPC(gactive_imp *pImp)
{
	UpdateNPCBaseProp(pImp);
	UpdateSpeed(pImp);
	UpdateAttack(0,pImp);
	UpdateMagic(pImp);
	UpdateDefense(0,pImp);
}

void property_policy::UpdatePet(gactive_imp* pImp)
{
	UpdateNPCBaseProp(pImp);
	UpdateSpeed(pImp);
	UpdatePetAttack(0,pImp);
	UpdateMagic(pImp);
	UpdateDefense(0,pImp);
}

#define PP_COMBINE_PLAYER_PROP(pImp,name) \
	Result2(pImp->_base_prop.name + pImp->_cur_item.name, pImp->_en_percent.name, pImp->_en_point.name)

#define PP_COMBINE_PLAYER_PROP_2(pImp,name,scale_name) \
	Result2(pImp->_base_prop.name + pImp->_cur_item.name, pImp->_en_percent.scale_name, pImp->_en_point.name)

#define PP_COMBINE_PLAYER_PROP_3(pImp,name,scale_name,point_name) \
	Result2(pImp->_base_prop.name + pImp->_cur_item.name, pImp->_en_percent.scale_name, pImp->_en_point.point_name)
void 
property_policy::UpdatePlayerBaseProp(int cls,gactive_imp * pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;
	dest = pImp->_base_prop;
	UpdateMPHPGen(pImp);
	UpdateDPGen(pImp);
	dest.max_hp = PP_COMBINE_PLAYER_PROP(pImp, max_hp);
	dest.max_mp = PP_COMBINE_PLAYER_PROP(pImp, max_mp);
	dest.max_dp = PP_COMBINE_PLAYER_PROP(pImp, max_dp);

	if(pImp->IsRenMa())
	{
		dest.max_hp += (int)(dest.max_mp * 0.6); 
	}

	if(dest.max_hp <= 0) dest.max_hp = 1;
	pImp->SetRefreshState();
}

void 
property_policy::UpdateMPHPGen(gactive_imp * pImp)
{
	player_extend_prop & base = pImp->_base_prop;
	player_extend_prop & dest = pImp->_cur_prop;

	int enh = pImp->_en_percent.hp_gen;
	dest.hp_gen[0] = (int)(base.hp_gen[0] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.hp_gen[1] = (int)(base.hp_gen[1] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.hp_gen[2] = (int)(base.hp_gen[2] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.hp_gen[3] = (int)(base.hp_gen[3] * ((float)( 1000 + enh))*0.001f + 0.5f);

	enh = pImp->_en_percent.mp_gen;
	dest.mp_gen[0] = (int)(base.mp_gen[0] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.mp_gen[1] = (int)(base.mp_gen[1] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.mp_gen[2] = (int)(base.mp_gen[2] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.mp_gen[3] = (int)(base.mp_gen[3] * ((float)( 1000 + enh))*0.001f + 0.5f);
}

void 
property_policy::UpdateDPGen(gactive_imp * pImp)
{
	player_extend_prop & base = pImp->_base_prop;
	player_extend_prop & dest = pImp->_cur_prop;

	int enh = pImp->_en_percent.dp_gen;
	dest.dp_gen[0] = (int)(base.dp_gen[0] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.dp_gen[1] = (int)(base.dp_gen[1] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.dp_gen[2] = (int)(base.dp_gen[2] * ((float)( 1000 + enh))*0.001f + 0.5f);
	dest.dp_gen[3] = (int)(base.dp_gen[3] * ((float)( 1000 + enh))*0.001f + 0.5f);
}

void 
property_policy::UpdateNPCBaseProp(gactive_imp * pImp)
{
	player_extend_prop & base = pImp->_base_prop;
	player_extend_prop & dest = pImp->_cur_prop;
	/* 当前数据首先是从基础属性中来 */
	dest = base;
	UpdateMPHPGen(pImp);
	pImp->SetRefreshState();
}

void 
property_policy::UpdateAttack(int cls,gactive_imp * pImp)
{
	player_item_prop &cur_item  = pImp->_cur_item;
	player_extend_prop & dest = pImp->_cur_prop;
	player_enhanced_param & en_point = pImp->_en_point;

	dest.damage_low  = PP_COMBINE_PLAYER_PROP_3(pImp,damage_low ,damage,damage);
	dest.damage_high = PP_COMBINE_PLAYER_PROP_3(pImp,damage_high,damage,damage);
	dest.deity_power = PP_COMBINE_PLAYER_PROP(pImp,deity_power);
	dest.attack = PP_COMBINE_PLAYER_PROP(pImp,attack);

	if(cur_item.weapon_class <= 0)
	{
		dest.attack_range = 2.0f; 
		cur_item.attack_cycle = 20;
		cur_item.attack_point = 10;
	}
	else
	{
		dest.attack_range = cur_item.attack_range;
	}

	dest.attack_range += pImp->_parent->body_size;	//在自身的攻击范围内加入自身的大小
	dest.attack_range += en_point.attack_range;

	dest.skill_attack_rate	= pImp->_base_prop.skill_attack_rate + pImp->_en_point.skill_attack_rate;
	dest.skill_armor_rate	= pImp->_base_prop.skill_armor_rate + pImp->_en_point.skill_armor_rate;

	//计算攻击暴击率
	UpdateCrit(pImp);
	
	for(size_t i =0; i < CULT_DEF_NUMBER; i++)
		dest.cult_attack[i] = pImp->_en_point.cult_attack[i];
}

void property_policy::UpdatePetAttack(int cls,gactive_imp * pImp)
{
//	player_item_prop &cur_item  = pImp->_cur_item;
	player_extend_prop & dest = pImp->_cur_prop;
	dest.damage_low  = PP_COMBINE_PLAYER_PROP_3(pImp,damage_low ,damage,damage);
	dest.damage_high = PP_COMBINE_PLAYER_PROP_3(pImp,damage_high,damage,damage);
	dest.attack = PP_COMBINE_PLAYER_PROP(pImp,attack);

	dest.skill_attack_rate	= pImp->_base_prop.skill_attack_rate + pImp->_en_point.skill_attack_rate;
	dest.skill_armor_rate	= pImp->_base_prop.skill_armor_rate + pImp->_en_point.skill_armor_rate;
	//计算攻击暴击率
	UpdateCrit(pImp);
}

void 
property_policy::UpdateCrit(gactive_imp * pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;
	dest.crit_damage = pImp->_base_prop.crit_damage + pImp->_en_point.crit_damage;
	int crit_rate = pImp->_base_prop.crit_rate + pImp->_en_point.crit_rate + pImp->_crit_rate;
//	if(crit_rate > 950) crit_rate = 950;  取消限制，由于游戏里面存在反暴
	dest.crit_rate = crit_rate;

	dest.anti_crit = pImp->_base_prop.anti_crit + pImp->_en_point.anti_crit;
	dest.anti_crit_damage = pImp->_base_prop.anti_crit_damage + pImp->_en_point.anti_crit_damage;
}


void
property_policy::UpdateTransform(gactive_imp *pImp)
{
	player_extend_prop & base = pImp->_base_prop;
	player_extend_prop & dest = pImp->_cur_prop;
	player_enhanced_param & en_point = pImp->_en_point;
	q_scale_enhanced_param & en_percent = pImp->_en_percent;

	dest.anti_transform = Result(base.anti_transform , en_point.anti_transform, en_percent.anti_transform);
}

void
property_policy::UpdateInvisible(gactive_imp *pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;

	dest.invisible_rate = pImp->_base_prop.invisible_rate + pImp->_en_point.invisible_rate; 
	dest.anti_invisible_rate = pImp->_base_prop.anti_invisible_rate + pImp->_en_point.anti_invisible_rate; 

	((gactive_object*)pImp->_parent)->anti_invisible_rate = dest.anti_invisible_rate; 
}


void 
property_policy::UpdateMagic(gactive_imp * pImp)
{
	player_extend_prop & base = pImp->_base_prop;
	player_extend_prop & dest = pImp->_cur_prop;
	player_enhanced_param & en_point = pImp->_en_point;
	q_scale_enhanced_param & en_percent = pImp->_en_percent;


	dest.resistance[0] = Result(base.resistance[0] , en_point.resistance[0], en_percent.resistance[0]);
	dest.resistance[1] = Result(base.resistance[1] , en_point.resistance[1], en_percent.resistance[1]);
	dest.resistance[2] = Result(base.resistance[2] , en_point.resistance[2], en_percent.resistance[2]);
	dest.resistance[3] = Result(base.resistance[3] , en_point.resistance[3], en_percent.resistance[3]);
	dest.resistance[4] = Result(base.resistance[4] , en_point.resistance[4], en_percent.resistance[4]);
	dest.resistance[5] = Result(base.resistance[5] , en_point.resistance[5], en_percent.resistance[5]);

	dest.resistance_tenaciy[0] = Result(base.resistance_tenaciy[0] , en_point.resistance_tenaciy[0], en_percent.resistance_tenaciy[0]);
	dest.resistance_tenaciy[1] = Result(base.resistance_tenaciy[1] , en_point.resistance_tenaciy[1], en_percent.resistance_tenaciy[1]);
	dest.resistance_tenaciy[2] = Result(base.resistance_tenaciy[2] , en_point.resistance_tenaciy[2], en_percent.resistance_tenaciy[2]);
	dest.resistance_tenaciy[3] = Result(base.resistance_tenaciy[3] , en_point.resistance_tenaciy[3], en_percent.resistance_tenaciy[3]);
	dest.resistance_tenaciy[4] = Result(base.resistance_tenaciy[4] , en_point.resistance_tenaciy[4], en_percent.resistance_tenaciy[4]);
	dest.resistance_tenaciy[5] = Result(base.resistance_tenaciy[5] , en_point.resistance_tenaciy[5], en_percent.resistance_tenaciy[5]);
	
	dest.resistance_proficiency[0] = Result(base.resistance_proficiency[0] , en_point.resistance_proficiency[0], en_percent.resistance_proficiency[0]);
	dest.resistance_proficiency[1] = Result(base.resistance_proficiency[1] , en_point.resistance_proficiency[1], en_percent.resistance_proficiency[1]);
	dest.resistance_proficiency[2] = Result(base.resistance_proficiency[2] , en_point.resistance_proficiency[2], en_percent.resistance_proficiency[2]);
	dest.resistance_proficiency[3] = Result(base.resistance_proficiency[3] , en_point.resistance_proficiency[3], en_percent.resistance_proficiency[3]);
	dest.resistance_proficiency[4] = Result(base.resistance_proficiency[4] , en_point.resistance_proficiency[4], en_percent.resistance_proficiency[4]);
	dest.resistance_proficiency[5] = Result(base.resistance_proficiency[5] , en_point.resistance_proficiency[5], en_percent.resistance_proficiency[5]);
	dest.anti_diet = base.anti_diet + en_point.anti_diet; 
}

void 
property_policy::UpdateSpeed(gactive_imp *pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;
	player_extend_prop & src = pImp->_base_prop;
	float run_speed  = src.run_speed;
	float walk_speed = src.walk_speed;
	if(pImp->_en_point.override_speed > 1e-3)
	{
		//覆盖选项存在，使用之
		run_speed = pImp->_en_point.override_speed;

		float enh_speed = pImp->_en_percent.speed*0.001f;
		if(enh_speed<0)
		{
			run_speed *= enh_speed+1.0;
		}
		if(pImp->_en_point.run_speed < 0)
		{
			run_speed += pImp->_en_point.run_speed;
		}
		run_speed += pImp->_en_point.mount_speed;
		walk_speed = run_speed * 0.3f;
	}
	else
	{
		float enh_speed = pImp->_en_percent.speed*0.001f+1.0;
		float inc_speed  = pImp->_en_point.run_speed;
		//这里限制一下最大速度固定增益
		if(inc_speed > 6.0f) 
		{
			inc_speed = 6.0f; 
			__PRINTF("发生固定速度越界情况");
		}
		walk_speed = walk_speed*enh_speed + inc_speed;
		run_speed = run_speed*enh_speed + inc_speed;
	}

	if(run_speed > MAX_RUN_SPEED) run_speed = MAX_RUN_SPEED;
	if(run_speed <= 1e-3 ) run_speed = MIN_RUN_SPEED;
	if(walk_speed > MAX_RUN_SPEED) walk_speed = MAX_RUN_SPEED;
	if(walk_speed <= 1e-3 ) walk_speed = MIN_RUN_SPEED;
	dest.run_speed = run_speed;
	dest.walk_speed = walk_speed;
}

void 
property_policy::UpdateDefense(int cls,gactive_imp *pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;
	int defense = PP_COMBINE_PLAYER_PROP(pImp,defense);
	int armor = PP_COMBINE_PLAYER_PROP(pImp,armor);
	dest.defense = defense;
	dest.armor = armor;

	for(size_t i =0; i < CULT_DEF_NUMBER; i++)
		dest.cult_defense[i] = pImp->_en_point.cult_defense[i];
}

/*
 *	部分update
 */
void 
property_policy::UpdateLife(gactive_imp * pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;

	dest.max_hp = PP_COMBINE_PLAYER_PROP(pImp,max_hp);
	if(dest.max_hp <= 0) dest.max_hp = 1;

	if(pImp->IsRenMa())
	{
		dest.max_hp += (int)(dest.max_mp * 0.6); 
	}

	if(pImp->_basic.rage > dest.max_hp)
	{
		pImp->_basic.rage = dest.max_hp;
	}

	if(pImp->_basic.hp > dest.max_hp)
	{
		pImp->_basic.hp = dest.max_hp;
	}
}

void 
property_policy::UpdateMana(gactive_imp * pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;

	dest.max_mp = PP_COMBINE_PLAYER_PROP(pImp,max_mp);
	if(pImp->_basic.mp > dest.max_mp)
	{
		pImp->_basic.mp = dest.max_mp;
	}

	if(pImp->IsRenMa()) UpdateLife(pImp);
}

void 
property_policy::UpdateDeity(gactive_imp * pImp)
{
	player_extend_prop & dest = pImp->_cur_prop;

	dest.max_dp = PP_COMBINE_PLAYER_PROP(pImp,max_dp);
	if(pImp->_basic.dp > dest.max_dp)
	{
		pImp->_basic.dp = dest.max_dp;
	}
}
