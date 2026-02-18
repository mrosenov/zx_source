#include <arandomgen.h>
#include <glog.h>
#include "template/itemdataman.h"
#include "template/npcgendata.h"
#include "template/globaldataman.h"
#include "template_loader.h"
#include "faction.h"
#include "gmatrix.h"
#include "mobactivedataman.h"

float __sctab[256][2];
static int InitSinCosTable()
{
	for(size_t i = 0; i <256; i++)
	{
		double ang = 3.1415926535*2/256.0 *i;
		__sctab[i][0] = sin(ang);
		__sctab[i][1] = cos(ang);
	}
	return 0;
}

bool 
npc_stubs_manager::LoadTemplate(itemdataman & dataman)
{
//初始化一下sin cos 表
	InitSinCosTable();

	DATA_TYPE dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_MONSTER_ESSENCE)
		{	
			const MONSTER_ESSENCE &mob = *(const MONSTER_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&mob && dt == DT_MONSTER_ESSENCE);

			npc_template mt;
			memset(&mt,0,sizeof(mt));
			mt.tid = mob.id;

			mt.body_size = mob.size;
			mt.faction = mob.faction;
			mt.isfly = mob.air_prop;  // Youshuang add
			mt.show_damage = mob.show_damage;  // Youshuang add

			mt.enemy_faction = mob.enemy_faction;

			mt.monster_faction = mob.monster_faction;
			mt.role_in_war =  mob.role_in_war;

			if(mob.role_in_war)
			{
				// 如果是城战双方 ，则加入攻方友军和守方友军的faction
				if(mt.faction & FACTION_BATTLEOFFENSE)
				{
					//攻方
					mt.faction |= FACTION_OFFENSE_FRIEND;
				}
				else if(mt.faction & FACTION_BATTLEDEFENCE)
				{
					//守方
					mt.faction |= FACTION_DEFENCE_FRIEND;
				}
			}

			//根据城战内容进行修正
			switch (mob.role_in_war)
			{
				case 1:	//标志性建筑
					//这里不作任何修正
					break;
					
				case 2:	//炮塔 （复活点保护塔）
					//这里将自身的faction清除，以完成不受任何攻击能力
					mt.faction &= FACTION_DEFENCE_FRIEND|FACTION_OFFENSE_FRIEND;
					break;
					
				case 3: //箭塔
					//箭塔拥有特殊的变身逻辑
					//这里填写的是箭塔或者箭塔建造中所使用的
					//箭塔基座NPC也使用此种
					break;

				case 4: //攻城车
					mt.faction &= ~(FACTION_BATTLEDEFENCE|FACTION_BATTLEOFFENSE);
					break;

				case 6: //复活点标记NPC 此种NPC不会被真正产生，只是一个DUMMY
					break;
				
				case 7: //服务npc
					mt.faction &= FACTION_DEFENCE_FRIEND|FACTION_OFFENSE_FRIEND;
					break;
			}

			mt.id_strategy = mob.id_strategy;
			mt.sight_range = mob.sight_range;
			mt.trigger_policy = mob.common_strategy; //怪物策略ID
			
			if(mt.trigger_policy)
			{
				if(gmatrix::GetTriggerMan().GetPolicy(mt.trigger_policy) == NULL)
				{
					//找不到合适的策略
					printf("怪物%d的策略%d无法在策略文件中找到\n",mt.tid,mt.trigger_policy);
					mt.trigger_policy = 0;
				}
			}
//			mt.id_skill = mob.id_skill;
//			mt.id_skill_level = mob.skill_level;

			mt.immune_type = mob.immune_type; 
			mt.aggressive_mode = mob.aggressive_mode;
			mt.monster_faction_ask_help = mob.monster_faction_ask_help;
			mt.monster_faction_can_help = mob.monster_faction_can_help;
			mt.aggro_range = mob.aggro_range;
			mt.aggro_time = (int)mob.aggro_time;
			if(mt.aggro_time <= 0) mt.aggro_time = 1;
			mt.patrol_mode = mob.patroll_mode?1:0;
			mt.after_death = mob.after_death;
			mt.is_boss = mob.is_boss?1:0;
			mt.kill_exp = mob.killed_exp;
			mt.kill_drop = mob.killed_drop;
			mt.drop_no_protected = 0;
			mt.no_exp_punish = mob.adjust_exp;
			mt.can_catch = mob.can_catch;
			mt.player_cannot_attack = mob.player_cannot_attack;
			mt.no_idle = mob.tick_stratege;
			mt.task_share = mob.task_share;
			mt.item_drop_share = mob.item_drop_share;
			mt.buff_area_id = mob.buff_area_id;
			for(size_t j = 0; j < 32; j ++)
			{
				if(mob.skills[j].id_skill <= 0) continue;
				int id_skill = mob.skills[j].id_skill;
				int lvl_skill = mob.skills[j].level;
//				int type = GNET::SkillWrapper::GetType(id_skill); 现在不再取得技能类型了，一律认为是攻击
				int type = 1;
				if(type < 0) 
				{
					printf("怪物%d的使用技能%d不存在\n",mt.tid,id_skill);
					continue;
				}

				switch(type)
				{
					case 1: //攻击
						npc_template::copy_skill(mt.skills.as_count,mt.skills.attack_skills,
								id_skill,lvl_skill);
						break;

					case 2: //祝福
						npc_template::copy_skill(mt.skills.bs_count,mt.skills.bless_skills,
								id_skill,lvl_skill);
						break;

					case 3: //诅咒
						npc_template::copy_skill(mt.skills.cs_count,mt.skills.curse_skills,
								id_skill,lvl_skill);
						break;

					default:
					//其他
					printf("怪物%d的技能%d类型未知%d\n",mt.tid,id_skill,type);
				}
				
			}

			if((mt.id_strategy == 2 || mt.id_strategy == 3) && (mt.skills.as_count==0 && mt.skills.cs_count==0))
			{
				printf("技能怪物%d没有设置技能号 %d\n",mt.tid,mt.skills.cs_count);
			}

			//设置模板基本属性
			mt.base_lvl_data.hp		= mob.hp          ;
			mt.base_lvl_data.mp		= mob.mp          ;
			mt.base_lvl_data.dmg		= mob.dmg         ;
			mt.base_lvl_data.defense	= mob.defense     ;
			mt.base_lvl_data.attack		= mob.attack      ;
			mt.base_lvl_data.armor		= mob.armor       ;
			mt.base_lvl_data.crit_rate	= mob.crit_rate   ;
			mt.base_lvl_data.crit_damage	= mob.crit_damage*0.01f;
			mt.base_lvl_data.anti_stunt	= mob.anti_stunt  ;
			mt.base_lvl_data.anti_weak	= mob.anti_weak   ;
			mt.base_lvl_data.anti_slow	= mob.anti_slow   ;
			mt.base_lvl_data.anti_silence	= mob.anti_silence;
			mt.base_lvl_data.anti_sleep	= mob.anti_sleep  ;
			mt.base_lvl_data.anti_twist	= mob.anti_twist  ;

			mt.lvl_up_data.hp		= mob.lvlup_hp          ;
			mt.lvl_up_data.mp		= mob.lvlup_mp          ;
			mt.lvl_up_data.dmg		= mob.lvlup_dmg         ;
			mt.lvl_up_data.defense		= mob.lvlup_defense     ;
			mt.lvl_up_data.attack		= mob.lvlup_attack      ;
			mt.lvl_up_data.armor		= mob.lvlup_armor       ;
			mt.lvl_up_data.crit_rate	= mob.lvlup_crit_rate   ;
			mt.lvl_up_data.crit_damage	= mob.lvlup_crit_damage*0.01f;
			mt.lvl_up_data.anti_stunt	= mob.lvlup_anti_stunt  ;
			mt.lvl_up_data.anti_weak	= mob.lvlup_anti_weak   ;
			mt.lvl_up_data.anti_slow	= mob.lvlup_anti_slow   ;
			mt.lvl_up_data.anti_silence	= mob.lvlup_anti_silence;
			mt.lvl_up_data.anti_sleep	= mob.lvlup_anti_sleep  ;
			mt.lvl_up_data.anti_twist	= mob.lvlup_anti_twist  ;

			mt.base_exp 			= mob.exp;
			mt.lvl_up_exp			 = mob.lvlup_exp;
			
			mt.bp.level 		= mob.level;
			mt.bp.exp 		= mob.exp;
			mt.bp.hp 		= mob.hp;
			mt.bp.mp 		= mob.mp;
			mt.bp.status_point 	= 1;

			player_template::BuildExProp(mt.base_lvl_data, mt.lvl_up_data,mob.level,mob.level,mt.ep);
			mt.ep.hp_gen[0] = mob.hp_gen2;
			mt.ep.hp_gen[1] = mob.hp_gen1;
			mt.ep.hp_gen[2] = 0;
			mt.ep.hp_gen[3] = 0;
			mt.ep.mp_gen[0] = mob.mp_gen2;
			mt.ep.mp_gen[1] = mob.mp_gen1;
			mt.ep.mp_gen[2] = 0;
			mt.ep.mp_gen[3] = 0;
			mt.ep.walk_speed = mob.walk_speed;
			mt.ep.run_speed = mob.run_speed;

			mt.ip.attack_cycle = SECOND_TO_TICK(2.0f);
			mt.ip.attack_point = SECOND_TO_TICK(0.4f);
			mt.ip.weapon_class = 1;
			mt.ip.attack_range = mob.attack_range;

			mt.ep.attack_range = mt.ip.attack_range;
			mt.ep.anti_crit = (int)(mob.dec_crit_rate * 1000);
			mt.ep.anti_crit_damage = mob.dec_crit_dmg;
			mt.ep.skill_attack_rate = (int)(mob.attack_prop * 1000);
			mt.ep.skill_armor_rate = (int)(mob.armor_prop * 1000);

			mt.dmg_reduce = mob.extra_defence;
			mt.spec_damage = mob.extra_damage;
			mt.is_skill_drop_adjust = mob.is_drop_adjustby_skill;
			
			Insert(mt);

			// Youshuang mod
			int drop_table_sz = sizeof( mob.drop_table ) / sizeof( mob.drop_table[0] );
			int drop_table_ids = 0;
			float drop_table_props = 0.0;
			for( int i = 0; i < drop_table_sz; ++i )
			{
				drop_table_ids += mob.drop_table[i].id_drop_table;
				drop_table_props += mob.drop_table[i].prop_drop_table;
			}
			if( (drop_table_ids != 0) && fabs( 1.0f - drop_table_props ) > 1e-6 )
			{
				printf("怪物%d的掉落表概率不对\n", mob.id);
				return false;
			}
			// end
			/*
			if(mob.drop_table[0].id_drop_table + mob.drop_table[1].id_drop_table != 0)
			{
				if(fabs(1.0f - mob.drop_table[0].prop_drop_table - mob.drop_table[1].prop_drop_table) > 1e-6)
				{
					printf("怪物%d的两个掉落表概率不对\n", mob.id);
					return false;
				}
			}
			*/
		}
		else if(dt == DT_DROPTABLE_ESSENCE)
		{
			const DROPTABLE_ESSENCE &drop = *(const DROPTABLE_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&drop && dt == DT_DROPTABLE_ESSENCE);
			size_t count = sizeof(drop.drops) / sizeof(drop.drops[0]);
			float p = 0.f;
			for(size_t i = 0; i < count; i ++)
			{
				if(drop.drops[i].probability < 0) 
				{
					printf("掉落表%d发现负数概率\n",id);
					return false;
				}
				if(drop.drops[i].probability > 0 && drop.drops[i].id_obj <= 0)
				{
					printf("掉落表%d发现无效物品\n",id);
					//return false;
				}
				p += drop.drops[i].probability;
			}
			if(fabs(1.0f - p) > 1e-6)
			{
				printf("掉落表%d未归一化\n", id);
				return false;
			}
		}
	}

	//由于NPC内部的怪物ID，所以先读入全部的怪物数据才能进行NPC数据的读取
	abase::vector<TRANS_TARGET_SERV> & waypoint_list = globaldata_gettranstargetsserver();
	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_NPC_ESSENCE)
		{	
			//枚举NPC的数据
			const NPC_ESSENCE &npc = *(const NPC_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&npc && dt == DT_NPC_ESSENCE);
			//使用特殊的模板
			int mobs_id = npc.id_src_monster;
			npc_template * mobs = NULL;
			if(mobs_id == 0 || (mobs = Get(mobs_id)) == NULL)
			{
				printf("错误的功能NPC模板%d，没有对应的怪物id %d\n",npc.id,mobs_id);
				continue;
			}
			npc_template nt;
			nt = *mobs;
			nt.tid = npc.id;

			nt.npc_data =  (npc_template::npc_statement*)malloc(sizeof(npc_template::npc_statement));
			memset(nt.npc_data, 0, sizeof(npc_template::npc_statement));

			nt.npc_data->refresh_time = SECOND_TO_TICK(npc.refresh_time);
			nt.npc_data->attack_rule = npc.attack_rule;
			nt.npc_data->tax_rate = 0;
			nt.npc_data->id_territory = npc.id_territory;

			nt.npc_data->carrier_mins[0] = npc.vehicle_min_length; 
			nt.npc_data->carrier_mins[1] = npc.vehicle_min_height; 
			nt.npc_data->carrier_mins[2] = npc.vehicle_min_width; 
			nt.npc_data->carrier_maxs[0] = npc.vehicle_max_length; 
			nt.npc_data->carrier_maxs[1] = npc.vehicle_max_height; 
			nt.npc_data->carrier_maxs[2] = npc.vehicle_max_width; 


			//处理各种服务
			if(npc.id_sell_service)
			{
				//卖出服务
				DATA_TYPE dt2;
				const NPC_SELL_SERVICE &service = *(const NPC_SELL_SERVICE*)dataman.get_data_ptr(npc.id_sell_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_SELL_SERVICE))
				{
					printf("发现了错误的transmit service %d 在NPC %d\n",npc.id_sell_service,nt.tid);
					continue;
				}
				//卖出服务
				int number = 0;
				for(int i = 0; i < 8 ; ++i)
				{
					for(int j = 0; j < 48; ++j)
					{
						nt.npc_data->service_sell_goods[number].goods = service.pages[i].id_goods[j];
						float discount = service.pages[i].discount;
						if(discount > 0.0f || discount < -1.0f)
						{
							__PRINTF("折扣率有问题,item(%d,%f)\n",service.pages[i].id_goods[j],discount);
						}
						if(discount > 0.0f)discount = 0.0f;
						if(discount < -1.0f)discount = -1.0f;
						nt.npc_data->service_sell_goods[number].discount = discount;
						number ++;
					}
				}
				nt.npc_data->service_sell_num = number;

				//只要有卖出服务就有买入服务
				nt.npc_data->service_purchase = true;
			}

			if(npc.id_reputation_shop_service)
			{
				nt.npc_data->reputation_shop_id = npc.id_reputation_shop_service;
			}
			
			if(npc.id_heal_service)
			{
				nt.npc_data->service_heal= true;
			}

			if(npc.id_transmit_service)
			{
				DATA_TYPE dt2;
				const NPC_TRANSMIT_SERVICE &service = *(const NPC_TRANSMIT_SERVICE*)dataman.get_data_ptr(npc.id_transmit_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TRANSMIT_SERVICE))
				{
					printf("发现了错误的transmit service %d 在NPC %d\n",npc.id_transmit_service,nt.tid);
					continue;
				}
				
				int num = 0;
				for(int i = 0; i < 16 ; ++i)
				{
					if(service.targets[i].id_world <= 0) continue;
					nt.npc_data->transmit_entry[num].fee = service.targets[i].fee;
					nt.npc_data->transmit_entry[num].require_level = service.targets[i].required_level;
					float x,y,z;
					x = service.targets[i].x;
					y = service.targets[i].y;
					z = service.targets[i].z;
					nt.npc_data->transmit_entry[num].target_pos = A3DVECTOR(x,y,z);
					nt.npc_data->transmit_entry[num].target_tag = service.targets[i].id_world;
					num ++;
				}
				nt.npc_data->service_transmit_target_num = num;
			}

			if(npc.id_task_in_service)
			{	
				DATA_TYPE dt2;
				const NPC_TASK_IN_SERVICE &service = *(const NPC_TASK_IN_SERVICE*)dataman.get_data_ptr(npc.id_task_in_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TASK_IN_SERVICE))
				{
					printf("发现了错误的task in service %d 在NPC %d\n",npc.id_task_in_service,nt.tid);
					continue;
				}
				
				int num = 0;
				for(int i = 0; i < 256; ++i)
				{
					if(!service.id_tasks[i]) continue;
					nt.npc_data->service_task_in_list[num] = service.id_tasks[i];
					num ++;
				}
				nt.npc_data->service_task_in_num = num;
			}

			if(npc.id_task_out_service)
			{	
				DATA_TYPE dt2;
				const NPC_TASK_OUT_SERVICE &service = *(const NPC_TASK_OUT_SERVICE*)dataman.get_data_ptr(npc.id_task_out_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TASK_OUT_SERVICE))
				{
					printf("发现了错误的task out service %d 在NPC %d\n",npc.id_task_out_service,nt.tid);
					continue;
				}

				int num = 1;
				nt.npc_data->service_task_out_list[0] = service.id_task_set;
				for(int i = 0; i < 256; ++i)
				{
					if(!service.id_tasks[i]) continue;
					nt.npc_data->service_task_out_list[num] = service.id_tasks[i];
					num ++;
				}
				nt.npc_data->service_task_out_num = num;
				if(num == 1 && service.id_task_set == 0)
				{	
					//空任务则忽略
					nt.npc_data->service_task_out_num = 0;
				}
			}

			if(npc.id_task_matter_service)
			{	
				DATA_TYPE dt2;
				const NPC_TASK_MATTER_SERVICE &service = *(const NPC_TASK_MATTER_SERVICE*)dataman.get_data_ptr(npc.id_task_matter_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TASK_MATTER_SERVICE))
				{
					printf("发现了错误的task matter service %d 在NPC %d\n",npc.id_task_matter_service,nt.tid);
					continue;
				}

				int num = 0;
				for(int i = 0; i < 32; ++i)
				{
					if(!service.tasks[i].id_task) continue;
					nt.npc_data->service_task_matter_list[num] = service.tasks[i].id_task;
					num ++;
				}
				nt.npc_data->service_task_matter_num = num;
			}

			if(npc.id_equip_soul_service)
			{
				nt.npc_data->service_npc_produce_id = npc.id_equip_soul_service;
			}

			// ---------  上古传送添加 by sunjunbo 2012-08-20  -----------
			// 1. 激活传送点服务
			nt.npc_data->id_open_ui_trans = npc.id_open_trans;
			// 2. 使用传送点服务
			if(npc.id_trans_dest_service)
			{
				DATA_TYPE dt2;
				const NPC_UI_TRANSFER_SERVICE &service = *(const NPC_UI_TRANSFER_SERVICE*)dataman.get_data_ptr(npc.id_trans_dest_service, ID_SPACE_ESSENCE, dt2);
				if(!(&service && dt2 == DT_NPC_UI_TRANSFER_SERVICE))
				{
					printf("发现了错误的使用上古传送服务 %d 在NPC %d\n",npc.id_trans_dest_service,nt.tid);
					continue;
				}

				ASSERT(sizeof(service.trans_id) == sizeof(nt.npc_data->use_ui_trans_id));

				memcpy(nt.npc_data->use_ui_trans_id, service.trans_id, sizeof(service.trans_id));
				nt.npc_data->id_use_ui_trans = npc.id_trans_dest_service;
			}
			// --------------------------------------

			if(npc.service_install)
			{
				nt.npc_data->service_install = true;
			}

			if(npc.service_uninstall)
			{
				nt.npc_data->service_uninstall = true;
			}

			if(npc.id_storage_service)
			{
				nt.npc_data->service_storage = true;
			}

			if(npc.combined_services & 0x0001)
			{
				nt.npc_data->renew_mount_service = 1;
			}

			if(npc.combined_services & 0x0004)
			{
				nt.npc_data->lock_item_service = 1;
			}

			//设置本身的路点信息
			if(npc.combined_services & 0x0008)
			{
				if(npc.id_to_discover)
				{
					nt.npc_data->service_waypoint_id = npc.id_to_discover;
					size_t j = 0;
					for(; j < waypoint_list.size(); j ++)
					{
						if((int)waypoint_list[j].id == (int)npc.id_to_discover)
						{
							
							break;
						}
					}
					if(j == waypoint_list.size()) 
					{
						printf("npc%d发现服务的id不正确 %d\n", npc.id,npc.id_to_discover);
					}
				}
				else
				{
					printf("NPC%d发现服务ID为0\n",npc.id);
				}
			}

			if(npc.combined_services & 0x10)
			{
				//有帮派服务 
				nt.npc_data->service_faction = 1;
			}

			if(npc.combined_services & 0x20)
			{
				//有恢复破损物品的接口
				nt.npc_data->service_restore_broken = 1;
			}

			if(npc.combined_services & 0x40)
			{
				//有邮件服务
				nt.npc_data->service_mail = 1;
			}
			
			if(npc.combined_services & 0x80)
			{
				//有拍卖服务
				nt.npc_data->service_auction = 1;
			}

			if(npc.combined_services & 0x100)
			{
				//有双倍经验服务
				nt.npc_data->service_double_exp = 1;
			}

			if(npc.combined_services & 0x200)
			{
				//孵化宠物蛋服务
				nt.npc_data->service_hatch_pet = 1;
			}
			
			if(npc.combined_services & 0x400)
			{
				//还原宠物蛋服务
				nt.npc_data->service_recover_pet = 1;
			}

			if(npc.combined_services & 0x2000)
			{
				//点卡交易
				nt.npc_data->service_cash_trade = 1;
			}

			if(npc.combined_services & 0x4000) 
			{
				//帮派仓库
				printf("发现了帮派仓库服务\n");
				nt.npc_data->service_storage_mafia = 1;
			}

			if(npc.combined_services & 0x8000) 
			{
				//法宝服务
				nt.npc_data->service_talisman = 1;
			}

			if(npc.combined_services & 0x10000)
			{
				//城战报名服务
				nt.npc_data->battle_field_challenge_service = 1;
			}

			if(npc.combined_services & 0x20000)
			{
				//战场入场服务
				//nt.npc_data->battle_field_enter = 1;
			}

			if(npc.combined_services & 0x40000)
			{
				//城战日常建设服务
				nt.npc_data->battle_field_construct_service = 1;
			}
			if(npc.combined_services & 0x80000)
			{
				nt.npc_data->pet_service_adopt = 1;
			}
			if(npc.combined_services & 0x100000)
			{
				nt.npc_data->pet_service_free = 1;
			}
			if(npc.combined_services & 0x200000)
			{
				nt.npc_data->pet_service_combine = 1;
			}
			if(npc.combined_services & 0x400000)
			{
				nt.npc_data->pet_service_rename = 1;
			}

			if(npc.combined_services & 0x800000)
			{
				nt.npc_data->service_blood_enchant = 1;
			}

			if(npc.combined_services & 0x1000000)
			{
				nt.npc_data->service_spirit_addon = 1;
			}

			if(npc.combined_services & 0x2000000)
			{
				nt.npc_data->service_spirit_remove = 1;
			}

			if(npc.combined_services & 0x4000000)
			{
				nt.npc_data->service_spirit_charge = 1;
			}

			if(npc.combined_services & 0x8000000)
			{
				nt.npc_data->service_spirit_decompose = 1;
			}

			if(npc.combined_services & 0x40000000)
			{
				nt.npc_data->service_arena_challenge = 1;
			}
			
			if(npc.combined_services2 & 0x0001)
			{
				nt.npc_data->service_change_style = 1;
			}

			if(npc.combined_services2 & 0x0002)
			{
				nt.npc_data->service_petequip_refine = 1;
			}

			if(npc.combined_services2 & 0x0020)
			{
				nt.npc_data->service_magic_refine = 1;
			}

			if(npc.combined_services2 & 0x0040)
			{
				nt.npc_data->service_magic_restore = 1;
			}

			if(npc.combined_services2 & 0x0080)
			{
				nt.npc_data->service_territory_challenge = 1;
			}
			
			if(npc.combined_services2 & 0x0100)
			{
				nt.npc_data->service_territory_enter = 1;
			}
			
			if(npc.combined_services2 & 0x0200)
			{
				nt.npc_data->service_territory_reward = 1;
			}
			
			if(npc.combined_services2 & 0x0400)
			{
				nt.npc_data->service_charge_telestation = 1;
			}
			
			if(npc.combined_services2 & 0x0800)
			{
				nt.npc_data->service_repair_damage = 1;
			}

			if(npc.combined_services2 & 0x1000)
			{
				nt.npc_data->service_equipment_upgrade = 1;
			}
			
			if(npc.combined_services2 & 0x2000)
			{
				nt.npc_data->service_crossservice_in = 1;
			}
			
			if(npc.combined_services2 & 0x4000)
			{
				nt.npc_data->service_crossservice_out = 1;
			}

			//Add by houjun 2010-03-11, 宝石镶嵌相关服务
			if(npc.combined_services2 & 0x10000)
			{
				nt.npc_data->service_identify_gem_slots = 1;
			}
			
			if(npc.combined_services2 & 0x20000)
			{
				nt.npc_data->service_rebuild_gem_slots = 1;
			}

			if(npc.combined_services2 & 0x40000)
			{
				nt.npc_data->service_customize_gem_slots = 1;
			}

			if(npc.combined_services2 & 0x80000)
			{
				nt.npc_data->service_embed_gems = 1;
			}

			if(npc.combined_services2 & 0x100000)
			{
				nt.npc_data->service_remove_gems = 1;
			}

			if(npc.combined_services2 & 0x200000)
			{
				nt.npc_data->service_upgrade_gem_level = 1;
			}

			if(npc.combined_services2 & 0x400000)
			{
				nt.npc_data->service_upgrade_gem_quality = 1;
			}
			
			if(npc.combined_services2 & 0x800000)
			{
				nt.npc_data->service_extract_gem = 1;
			}

			if(npc.combined_services2 & 0x1000000)
			{
				nt.npc_data->service_smelt_gem = 1;
			}

			if(npc.combined_services2 & 0x40000000)
			{
				nt.npc_data->service_change_name = 1;
			}

			if(npc.combined_services2 & 0x80000000)
			{
				nt.npc_data->service_change_faction_name = 1;
			}
			

			if(npc.combined_services3 & 0x00000001)
			{
				nt.npc_data->service_change_faction_name = 1;
			}

			if(npc.combined_services3 & 0x00000002)
			{
				nt.npc_data->service_talisman_holylevelup = 1;
			}

			if(npc.combined_services3 & 0x00000004)
			{
				nt.npc_data->service_talisman_embedskill = 1;
			}

			if(npc.combined_services3 & 0x00000008)
			{
				nt.npc_data->service_talisman_refineskill = 1;
			}

			if(npc.combined_services3 & 0x00000010)
			{
				nt.npc_data->service_equipment_upgrade2 = 1;
			}

			if(npc.combined_services3 & 0x00000020)
			{
				nt.npc_data->service_equipment_slot = 1;
			}

			if(npc.combined_services3 & 0x00000040)
			{
				nt.npc_data->service_install_astrology = 1;
			}

			if(npc.combined_services3 & 0x00000080)
			{
				nt.npc_data->service_uninstall_astrology= 1;
			}
			
			if(npc.combined_services3 & 0x00000100)
			{
				nt.npc_data->service_astrology_identify = 1;
			}

			if(npc.combined_services3 & 0x00000400)
			{
				nt.npc_data->service_crossservice_battle_out = 1;
			}

			if(npc.combined_services3 & 0x00000800)
			{
				nt.npc_data->service_crossservice_battle_sign_up = 1;
			}
			
			if(npc.combined_services3 & 0x00001000)
			{
				nt.npc_data->service_kingdom_enter = 1;
			}

			if(npc.combined_services3 & 0x00004000)
			{
				nt.npc_data->service_equipment_upgrade = 1;
			}
	
			if(npc.combined_services3 & 0x00010000)
			{
				nt.npc_data->service_produce_jinfashen = 1;
			}

			if(npc.combined_services3 & 0x00080000)
			{
				nt.npc_data->service_pet_reborn = 1;
			}

			if(npc.id_war_archer_service)
			{
				//城战购买弓箭手
				DATA_TYPE dt2;
				const NPC_WAR_BUY_ARCHER_SERVICE& service = *(const NPC_WAR_BUY_ARCHER_SERVICE*)dataman.get_data_ptr(npc.id_war_archer_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_WAR_BUY_ARCHER_SERVICE))
				{
					printf("发现了错误的id_war_archer_service  service %d 在NPC %d\n",npc.id_war_archer_service,nt.tid);
					continue;
				}
				nt.npc_data->battle_field_employ_service = 1;
				nt.npc_data->employ_service_data.price = service.price;
				nt.npc_data->employ_service_data.war_material_id = service.war_material_id;
				nt.npc_data->employ_service_data.war_material_count = service.war_material_count;
				nt.npc_data->employ_service_data.item_wanted = service.item_wanted;
				nt.npc_data->employ_service_data.remove_one_item = service.remove_one_item;
				memcpy(nt.npc_data->employ_service_data.controller_id,service.controller_id,sizeof(int) * 20);
			}

			if(npc.has_pkvalue_service)
			{
				nt.npc_data->service_reset_pkvalue.has_service = 1;
				nt.npc_data->service_reset_pkvalue.fee_per_unit  = npc.fee_per_pkvalue;
			}
			if(npc.id_equipbind_service)
			{
				DATA_TYPE dt2;
				const NPC_EQUIPBIND_SERVICE &service = *(const NPC_EQUIPBIND_SERVICE*)dataman.get_data_ptr(npc.id_equipbind_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_EQUIPBIND_SERVICE))
				{
					printf("发现了错误的bind service %d 在NPC %d\n",npc.id_equipbind_service,nt.tid)
						;
					continue;
				}
				nt.npc_data->service_equip_bind = 1;
				nt.npc_data->service_bind_prop.money_need = service.price;
				nt.npc_data->service_bind_prop.item_need = service.id_object_need;

			}               

			if(npc.id_equipdestroy_service)
			{               
				DATA_TYPE dt2;
				const NPC_EQUIPDESTROY_SERVICE &service = *(const NPC_EQUIPDESTROY_SERVICE*)dataman.get_data_ptr(npc.id_equipdestroy_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_EQUIPDESTROY_SERVICE))
				{
					printf("发现了错误的destroy service %d 在NPC %d\n",npc.id_equipdestroy_service,nt.tid);                         
					continue;
				}
				nt.npc_data->service_destroy_bind = 1;
				nt.npc_data->service_destroy_bind_prop.money_need = service.price;
				nt.npc_data->service_destroy_bind_prop.item_need = service.id_object_need;

			}	
			if(npc.id_equipundestroy_service)
			{
				DATA_TYPE dt2;
				const NPC_EQUIPUNDESTROY_SERVICE &service = *(const NPC_EQUIPUNDESTROY_SERVICE*)dataman.get_data_ptr(npc.id_equipundestroy_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_EQUIPUNDESTROY_SERVICE))
				{
					printf("发现了错误的undestroy service %d 在NPC %d\n",npc.id_equipundestroy_service,nt.tid);                     
					continue;
				}
				nt.npc_data->service_undestroy_bind = 1;
				nt.npc_data->service_undestroy_bind_prop.money_need = service.price;
				nt.npc_data->service_undestroy_bind_prop.item_need = service.id_object_need;

			}

			if(npc.id_resetprop_service)
			{
				DATA_TYPE dt2;
				const NPC_RESETPROP_SERVICE &service = *(const NPC_RESETPROP_SERVICE*)dataman.get_data_ptr(npc.id_resetprop_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_RESETPROP_SERVICE))
				{
					printf("发现了错误的resetprop service %d 在NPC %d\n",npc.id_resetprop_service,nt.tid);
					continue;
				}
				size_t t = 0;
				for(size_t i = 0; i < 6; i ++)
				{
					if(service.prop_entry[i].id_object_need <=0) continue;
					nt.npc_data->reset_prop[t].object_need= service.prop_entry[i].id_object_need;
					nt.npc_data->reset_prop[t].type  = service.prop_entry[i].resetprop_type;
					t++;
				}
				nt.npc_data->service_reset_prop_count = t;
			}

			if(npc.id_item_trade_service[0] > 0)
			{
				memcpy(&nt.npc_data->service_item_trade, npc.id_item_trade_service, sizeof(npc.id_item_trade_service));
			}	
			if(npc.id_consign_service)
			{
				DATA_TYPE dt2;
				const NPC_CONSIGN_SERVICE &service = *(const NPC_CONSIGN_SERVICE*)dataman.get_data_ptr(npc.id_consign_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_CONSIGN_SERVICE))
				{
					printf("发现了错误的undestroy service %d 在NPC %d\n",npc.id_consign_service,nt.tid);                     
					continue;
				}
				nt.npc_data->service_consign = 1;
				nt.npc_data->consign_margin = service.deposit;
			}
			if(npc.id_transcription)
			{
				DATA_TYPE dt2;
				const void* data = dataman.get_data_ptr(npc.id_transcription, ID_SPACE_CONFIG, dt2);
				if(!data || (dt2 != DT_TRANSCRIPTION_CONFIG && dt2 != DT_TOWER_TRANSCRIPTION_CONFIG))
				{
					printf("发现了错误的undestroy service %d 在NPC %d\n",npc.id_transcription,nt.tid);                     
					continue;
				}
				nt.npc_data->id_transcription = npc.id_transcription;
			}

			nt.npc_data->need_domain = npc.domain_related;
			if(npc.guard_npc)
			{
				//卫兵
				nt.npc_data->npc_type = npc_template::npc_statement::NPC_TYPE_GUARD;
			}
			else if(npc.is_vehicle)
			{
				//战船
				nt.npc_data->npc_type = npc_template::npc_statement::NPC_TYPE_CARRIER;
			}
			else
			{
				//普通
				nt.npc_data->npc_type = npc_template::npc_statement::NPC_TYPE_NORMAL;
			}
			nt.war_role_config = npc.war_role_config;
			nt.collision_in_server = npc.collision_in_server?1:0;
			Insert(nt);
		}
		if (dt == DT_INTERACTION_OBJECT_ESSENCE)
		{
			const INTERACTION_OBJECT_ESSENCE &interaction = *(const INTERACTION_OBJECT_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&interaction && dt == DT_INTERACTION_OBJECT_ESSENCE);
			//使用特殊的模板
			int mobs_id = interaction.monster_id;
			npc_template *mobs = NULL;
			npc_template nt;
			memset(&nt,0,sizeof(nt));
			if (mobs_id)
			{
				if ((mobs = Get(mobs_id)) == NULL)
				{
					printf("互动物品功能NPC模板%d，没有对应的怪物id %d\n",interaction.id,mobs_id);
				}
				else
				{
					nt = *mobs;
				}
			}
			nt.tid = interaction.id;
			nt.mob_active_data = (mob_active_statement *)malloc(sizeof(mob_active_statement));

			nt.mob_active_data->tid = interaction.id;
			nt.mob_active_data->active_mode = interaction.main_body;
			size_t hook_num = 0;
			for (int i = 0; i < 6; ++i)
			{
				if (interaction.hook[i]) hook_num++;
				nt.mob_active_data->active_point[i] = interaction.hook[i];
			}
			nt.mob_active_data->active_point_num = hook_num;
			nt.mob_active_data->active_times_max = interaction.max_interaction_num;
			nt.mob_active_data->active_no_disappear = interaction.disappear;
			nt.mob_active_data->need_equipment = interaction.require_item_id;
			nt.mob_active_data->task_in = interaction.require_task_id;
			nt.mob_active_data->level = interaction.require_level;
			nt.mob_active_data->require_gender = interaction.require_gender;
			nt.mob_active_data->character_combo_id = interaction.character_combo_id;
			nt.mob_active_data->character_combo_id2 = interaction.character_combo_id2;
			nt.mob_active_data->eliminate_equipment = interaction.consum_item_id;
			nt.mob_active_data->active_ready_time = interaction.open_time;
			nt.mob_active_data->active_running_time = interaction.interaction_time;
			nt.mob_active_data->active_ending_time = interaction.finish_time;
			nt.mob_active_data->path_id = interaction.path_id;
			nt.mob_active_data->speed = interaction.velocity;
			nt.mob_active_data->new_mob_active_id = interaction.new_item_id;
			nt.mob_active_data->move_can_active = interaction.move_enable;
			nt.mob_active_data->path_end_finish_active = interaction.auto_end;
			Insert(nt);
		}
	}

	return true;
}

bool 
mine_stubs_manager::LoadTemplate(itemdataman & dataman)
{
	DATA_TYPE dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_MINE_ESSENCE)
		{
			//是矿物
			const MINE_ESSENCE &mine=*(const MINE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&mine && dt == DT_MINE_ESSENCE);
			//使用特殊的模板
			mine_template mt;
			memset(&mt,0,sizeof(mt));
			mt.tid = mine.id;
			mt.role_in_war = mine.role_in_war;
			mt.std_amount = mine.num1;
			mt.bonus_amount = mine.num2;
			mt.bonus_prop = mine.probability2;
			mt.time_min = mine.time_min;
			mt.time_max = mine.time_max;
			mt.level = mine.level_required;
			mt.exp = mine.exp;
			if( (mt.std_amount <= 0 && 
				(!mine.task_in || !mine.task_out))||
				mt.time_min > 1024 ||
				mt.time_max > 1024 ||
				mt.time_min > mt.time_max ||
				mt.time_max <= 0 
				)
			{
				printf("错误的矿物参数，ID:%d\n",mt.tid);
				continue;
			}
			
			float prop = 0.f;
			int kinds = 0;
			for(size_t i = 0; i < 16; i ++)
			{
				if(mine.materials[i].probability > 0.f)
				{
					mt.id_produce[kinds] = mine.materials[i].id;
					mt.id_produce_prop[kinds] = mine.materials[i].probability;
					prop += mine.materials[i].probability;
					kinds ++;
				}
			}
			mt.produce_kinds = kinds;
			if(fabs(prop - 1.0f) >= 1e-5 || !mt.produce_kinds)
			{
				printf("错误的矿物产生的概率,矿物ID为%d\n",mt.tid);
				continue;
			}

			/* 去掉这个检查，现在允许
			if ((mine.task_in && !mine.task_out) || (!mine.task_in && mine.task_out))
			{
				printf("错误的任务矿%d\n",nt.tid);
				continue;
			}
			*/

			mt.need_equipment = mine.id_equipment_required;
			mt.task_in = mine.task_in;
			mt.task_out = mine.task_out;
			mt.no_interrupted = mine.uninterruptable;
			mt.gather_no_disappear = mine.permenent;
			mt.eliminate_tool = mine.eliminate_tool;
			mt.ask_help_faction = mine.aggros[0].monster_faction;
			mt.ask_help_range = mine.aggros[0].radius;
			mt.ask_help_aggro = mine.aggros[0].num;
			if(mt.ask_help_range > 30) mt.ask_help_range = 30.f;
			
			for(size_t i = 0; i < 4; i ++)
			{
				mt.monster_list[i].id_monster = mine.npcgen[i].id_monster;
				mt.monster_list[i].num = mine.npcgen[i].num;
				mt.monster_list[i].radius = mine.npcgen[i].radius;

				//mt.monster_list[i].remain_time = mine.npcgen[i].life;
			}
			mt.ctrl_info.actived_all	= (mine.activate_controller_mode == 0);
			mt.ctrl_info.deactived_all	= (mine.deactivate_controller_mode == 0);
			for(size_t i =0; i < 4; i ++)
			{
				if(mine.activate_controller_id[i])
				{
					mt.ctrl_info.active_cond[mt.ctrl_info.active_count++] = mine.activate_controller_id[i];
				}

				if(mine.deactivate_controller_id[i])
				{
					mt.ctrl_info.deactive_cond[mt.ctrl_info.deactive_count++] = mine.deactivate_controller_id[i];
				}
			}
			
			Insert(mt);
		}
		// Youshuang add
		else if( dt == DT_COMPOUND_MINE_ESSENCE)
		{
			const COMPOUND_MINE_ESSENCE &mine=*(const COMPOUND_MINE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&mine && dt == DT_COMPOUND_MINE_ESSENCE);
			//使用特殊的模板
			mine_template mt;
			memset(&mt,0,sizeof(mt));
			mt.tid = mine.id;
			mt.gather_count = mine.count;
			for( int i = 0; i < (int)( sizeof( mine.file_mine ) / sizeof( mine.file_mine[0] ) ); ++i )
			{
				if( mine.file_mine[i] > 0 )
				{
					mt.combine_tid.push_back( mine.file_mine[i] );
				}
			}
			mt.result_tid = mine.file_mine_spe;
			Insert( mt );
		}
		// end
	}
	return true;
}

bool
recipe_manager::LoadTemplate(itemdataman & dataman)
{
	//读取所有配方
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_RECIPE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_RECIPE,dt))
	{
		const RECIPE_ESSENCE &ess = *(const RECIPE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_RECIPE,dt);
		if(dt == DT_RECIPE_ESSENCE && &ess)
		{
			recipe_template rt;
			memset(&rt,0,sizeof(rt));
			
			rt.id = id;
			rt.fee = ess.price;
			rt.level = ess.level;
			if(rt.level < 1 || rt.level > 10)
			{
				printf("配方%d的级别超出范围\n",id);
				continue;
			}
			float prob = 0.f;
			int tc = 0;
			for(size_t  j = 0; j < 3; j ++)
			{
				float p = ess.products[j].probability;
				int iom = ess.products[j].id_to_make;
				int low = ess.products[j].min_num_make;
				int high = ess.products[j].max_num_make;
				if(p <= 0) continue;
				if(iom <= 0 || low <=0 || high <= 0) 
				{
					prob = -1;
					break;
				}
				
				rt.products[tc].probability = p;
				rt.products[tc].target_id = iom;
				rt.products[tc].target_count_low = low; 
				rt.products[tc].target_count_high =  high;
				tc ++;
				prob += p;
			}

			if(prob < 0 || tc <= 0)
			{
				printf("配方%d的数据有误\n",id);
				continue;
			}

			if(fabs(prob - 1.0f) > 1e-6)
			{
				printf("配方%d的归一化有误\n",id);
				continue;
			}

			int use_time = SECOND_TO_TICK(ess.duration);
			if(use_time<= 30) use_time = 30;
			if(use_time > 10000) use_time = 10000;
			rt.use_time = use_time;

			rt.cool_type = ess.cool_type;
			rt.cool_time = ess.cool_time * 1000;
			if(rt.cool_type > 100 || rt.cool_type < 0)
			{
				printf("配方%d的冷却类型不正确 %d\n", id, rt.cool_type);
				continue;
			}

			size_t total_meterail_count = 0;
			size_t num = 0;
			for(int i = 0; i < 6; ++i)
			{
				if(!ess.materials[i].id) continue;
				size_t meterail_count = ess.materials[i].num;
				if(!meterail_count)
				{
					printf("错误的配方 %d(制造%d) ,原料数目为0\n",ess.id,ess.products[0].id_to_make);
					continue;
				}
				total_meterail_count += meterail_count;
				rt.material_list[num].item_id = ess.materials[i].id;
				rt.material_list[num].count = meterail_count;
				num ++;
			}
			rt.material_num = num;
			rt.recipe_class = ess.recipe_class;
			memcpy(rt.required,ess.required,sizeof(rt.required));
			if(!total_meterail_count)
			{
				printf("错误的配方%d(制造%d) ，总原料数目为0\n", ess.id,ess.products[0].id_to_make);
				continue;
			}
			if(!Insert(rt))
			{
				printf("加入配方%d(制造%d)发生重复错误\n", ess.id,ess.products[0].id_to_make);
			}
		}
	}

	printf("总共加入了%d个生产配方\n",__GetInstance()._rt_map.size());
	return true;
}

bool
item_trade_manager::LoadTemplate(itemdataman & dataman)
{
	//读取所有配方
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_ITEM_TRADE_CONFIG)
		{
			const ITEM_TRADE_CONFIG &service = *(const ITEM_TRADE_CONFIG *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(dt == DT_ITEM_TRADE_CONFIG && &service);

			item_trade_template it;
			memset(&it, 0,sizeof(it));

			it.id = service.id; 
			bool err_flag = false;
			int num = 0;
			for(int i = 0; i < 4; ++i)
			{
				for(int j = 0; j < 48; ++j)
				{
					it.item_trade_goods[num].id = service.pages[i].goods[j].id_goods;
					it.item_trade_goods[num].item_num = service.pages[i].goods[j].goods_num;

					if(service.pages[i].goods[j].id_goods >0 && 
							(size_t)gmatrix::GetDataMan().get_item_pile_limit(service.pages[i].goods[j].id_goods) < service.pages[i].goods[j].goods_num)
					{
						//非法的
						printf("物物交易 service %d 里的物品数量超过堆叠上限\n",it.id);
						err_flag = true;
					}	

					if(service.pages[i].goods[j].id_goods &&
							( (service.pages[i].goods[j].item_required[0].id == 0 || service.pages[i].goods[j].item_required[0].count == 0) &&
							  (service.pages[i].goods[j].item_required[1].id == 0 || service.pages[i].goods[j].item_required[1].count == 0) &&
							  (service.pages[i].goods[j].repu_required[0].id == 0 || service.pages[i].goods[j].repu_required[0].count == 0) &&
							  (service.pages[i].goods[j].repu_required[1].id == 0 || service.pages[i].goods[j].repu_required[1].count == 0) &&
							  (service.pages[i].goods[j].special_required[0].type == 0 || service.pages[i].goods[j].special_required[0].count == 0)))
					{
						//非法的
						printf("物物交易 service %d 里有错误的价格\n", it.id);
						err_flag = true;
					}

					it.item_trade_goods[num].item_require[0].id          = service.pages[i].goods[j].item_required[0].id;
					it.item_trade_goods[num].item_require[1].id          = service.pages[i].goods[j].item_required[1].id;
					it.item_trade_goods[num].item_require[0].count       = service.pages[i].goods[j].item_required[0].count;
					it.item_trade_goods[num].item_require[1].count       = service.pages[i].goods[j].item_required[1].count;

					it.item_trade_goods[num].rep_require[0].id           = service.pages[i].goods[j].repu_required[0].id;
					it.item_trade_goods[num].rep_require[1].id           = service.pages[i].goods[j].repu_required[1].id;
					it.item_trade_goods[num].rep_require[0].count        = service.pages[i].goods[j].repu_required[0].count;
					it.item_trade_goods[num].rep_require[1].count        = service.pages[i].goods[j].repu_required[1].count;

					it.item_trade_goods[num].special_require.id           = service.pages[i].goods[j].special_required[0].type;                                  
					it.item_trade_goods[num].special_require.count        = service.pages[i].goods[j].special_required[0].count; 			
					num++;
				}
			}
			if(!err_flag)
			{
				item_trade_manager::Insert(it);
			}
		}
	}

	return true;
}

bool
reputation_shop_manager::LoadTemplate(itemdataman & dataman)
{
	//读取所有配方
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_NPC_REPUTATION_SHOP_SERVICE)
		{
			const NPC_REPUTATION_SHOP_SERVICE &service = *(const NPC_REPUTATION_SHOP_SERVICE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(dt == DT_NPC_REPUTATION_SHOP_SERVICE && &service);

			reputation_shop_template rs;
			memset(&rs, 0,sizeof(rs));

			rs.id = service.id;
			int number = 0;
			for(int i = 0; i < 4; ++i)
			{                       
				for(int j = 0; j < 48; ++j)
				{
					rs.reputation_shop_sell_goods[number].id_goods = service.pages[i].goods[j].id_goods;
					for(int k = 0; k < 3; ++k)
					{
						rs.reputation_shop_sell_goods[number].repu_required_pre[k].repu_id_req = service.pages[i].goods[j].repu_required_pre[k].repu_id_req;
						rs.reputation_shop_sell_goods[number].repu_required_pre[k].repu_req_value = service.pages[i].goods[j].repu_required_pre[k].repu_req_value;
						rs.reputation_shop_sell_goods[number].repu_required_pre[k].repu_id_consume = service.pages[i].goods[j].repu_required_pre[k].repu_id_consume;
						rs.reputation_shop_sell_goods[number].repu_required_pre[k].repu_consume_value = service.pages[i].goods[j].repu_required_pre[k].repu_consume_value
							;
					}
					number ++;
				}
			}		

			reputation_shop_manager::Insert(rs);
		}
	}

	return true;
}

bool
npc_produce_manager::LoadTemplate(itemdataman & dataman)
{
	//读取所有配方
	DATA_TYPE  dt;
	size_t id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_EQUIP_SOUL_MELD_SERVICE)
		{
			const EQUIP_SOUL_MELD_SERVICE &service = *(const EQUIP_SOUL_MELD_SERVICE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(dt == DT_EQUIP_SOUL_MELD_SERVICE && &service);

			npc_produce_template np;
			memset(&np, 0,sizeof(np));

			np.id = service.id;
			memcpy(&np.npc_produce_list, service.prescription, sizeof(service.prescription));
			npc_produce_manager::Insert(np);
		}
	}

	return true;
}

