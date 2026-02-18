#include "world.h"
#include "npcgenerator.h"
#include "clstab.h"
#include "npc.h"
#include "matter.h"
#include "ainpc.h"
#include "faction.h"
#include <arandomgen.h>
#include <glog.h>
#include "template/itemdataman.h"
#include "template/npcgendata.h"
#include "template/globaldataman.h"
#include "pathfinding/pathfinding.h"
#include "servicenpc.h"
#include "carriernpc.h"
#include "petman.h"
#include "petdataman.h"
#include "petnpc.h"
#include "player_imp.h"
#include "battleground/bg_world_manager.h"
#include "item/item_petbedge.h"
#include "summonnpc.h"
#include "forbid_manager.h"
#include "mobactivedataman.h"
#include "mobactivenpc.h"

int 
crontab_t::CheckTime(const tm &tt)
{
	int offset = 0;
	if(year >= 0 && year != (tt.tm_year + 1900)) 
	{
		//年份不符合
		if(tt.tm_year + 1901 != year) return 3600;
		if(tt.tm_mon != 11) return 3600;
		if(tt.tm_mday != 31) return 3600;
		return (23 - tt.tm_hour)*3600 + (59-tt.tm_min)+(60-tt.tm_sec);

	}
	if(month >= 0 && month != tt.tm_mon) 
	{
		if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

		//如果可能是月末的23点50分以后，则等待到0点的时间
		return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);
	}


	ASSERT(day_of_months < 0 || day_of_week < 0);
	if(day_of_months >= 0 && day_of_months != tt.tm_mday)
	{
		offset = (day_of_months - tt.tm_mday) * 24 * 3600;
	}
	if(day_of_week >= 0 && day_of_week != tt.tm_wday)
	{
		offset = (day_of_week - tt.tm_wday) * 24 * 3600;
	}

	if(hour >=0 && hour != tt.tm_hour)
	{
		offset += (hour - tt.tm_hour)*3600;
	}

	ASSERT(min >= 0);
	if(min >=0 && min != tt.tm_min)
	{
		offset += (min - tt.tm_min)*60;
	}

	offset -= tt.tm_sec;

	if(offset <= -60)
	{
		//时间超出了一分钟，考虑计算下一次可能事件
		if(hour < 0)
		{
			for(int h = tt.tm_hour; h < 23 && offset < 0; h++)
			{
				offset += 3600;
			}
			if(offset > 0)
				return offset;
			else
			{
				//靠小时调整无法完成，则需要调整更大的数值
				//所以将小时归0
				offset -= 3600*23;
			}
		}

		if(day_of_months < 0 || day_of_week < 0)
		{
			if(day_of_week >= 0)
			{
				//day of_month 是 *
				//进行周调整 
				offset %= 3600*24*7;
				offset += 3600*24*7;
				if(offset > 600) offset = 600;
				return offset;

			}
			else if(day_of_months >= 0)
			{
				//定日不定周，需要进行月份调整 
				//直接跳过10分钟
				if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

				//如果可能是月末的23点50分以后，则等待到0点的时间
				return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);

			}
			else
			{
				//任意一天，需要进行日调整
				offset %= 3600*24;
				offset += 3600*24;
				if(offset > 600) offset = 600;
				return offset;
			}
		}

		//定日定周的 进行月份调整
		if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

		//如果可能是月末的23点50分以后，则等待到0点的时间
		return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);

	}

	return offset;
}

static inline bool IsLeapYear(int year)
{
	if((year % 4) != 0)  return false;
	if((year % 400) == 0) return true;
	if((year % 100) == 0) return false;
	return true;
}

static inline int GetMDay(int year, int mon)
{
	static int mday[] = { 31,28,31,30,31,30,31,31,30,31,30,31};
	int d = mday[mon];
	if(mon == 1 && IsLeapYear(year)) d = d+1;
	return d;
}

int 
crontab_t::CheckTime2(time_t t1,int DUR)
{
	time_t t2 = t1 - DUR;
	struct tm tt;
	localtime_r(&t2,&tt);

	int offset = 0;
	if(year >= 0 && year != (tt.tm_year + 1900)) 
	{
		//年份不符合
		if(tt.tm_year+1901 != year) return DUR+3600;	//如果不是去年则一个小时后再说
		if(tt.tm_mon != 11) return DUR + 3600;		//如果不是12月则一个小时后再说

		//目标月份必须是一月份或者无要求
		if(month > 0) return DUR + 3600;

		//计算到年初的时间 
		offset = (31 - tt.tm_mday)* 24 * 3600 + (23 - tt.tm_hour)*3600 + (59-tt.tm_min)*60 +(60-tt.tm_sec);
		//修正日期
		tt.tm_year = year;		//今年
		tt.tm_mon = 0;			//1月
		tt.tm_yday = 0;
		tt.tm_mday = 1;			//1日 
		tt.tm_wday = (tt.tm_wday + (31 - tt.tm_mday + 1)) % 7;	//调整周
		tt.tm_hour = 0;
		tt.tm_min = 0;
		tt.tm_sec = 0;	//00:00:00
	}

	if(month >= 0 && month != tt.tm_mon) 
	{
		if((month != tt.tm_mon + 1) && (month != 0 || tt.tm_mon != 11))
		{
			//月份超过，或者相差超过了一个月，返回即可
			return DUR + 3600;
		}

		//计算月份相差时间 这里不会出现跨越年份的情况
		int d = GetMDay(tt.tm_year + 1900, tt.tm_mon);
		int d_adjust = d + 1 - tt.tm_mday;
		offset += d_adjust*24*3600;

		//修正日期致下一个月的1号
		if(tt.tm_mon == 11)
		{
			tt.tm_year ++;
			tt.tm_mon = 0;
		}
		else
		{
			tt.tm_mon ++;
		}
		tt.tm_yday += d_adjust;
		tt.tm_mday = 1;
		tt.tm_wday = (tt.tm_wday + d_adjust) % 7;	//调整周
	}

	int y_days = IsLeapYear(tt.tm_year + 1900)?366:365;
	int m_days = GetMDay(tt.tm_year + 1900, tt.tm_mon);

	ASSERT(day_of_months < 0 || day_of_week < 0);
	if(day_of_months >= 0 && day_of_months != tt.tm_mday)
	{
		//指定日子 修正日期
		offset += (day_of_months - tt.tm_mday) * 24 * 3600;
		if(offset < 0)
		{
			if(month < 0)
			{
				//时间不匹配，加入一个月的时间
				if(tt.tm_mon == 11 && year >= 0)
				{
					//无法加入
					return DUR + 3600;
				}
				offset += m_days*24*3600;
				//即使是12月，后面由于不需要再次调整了，所以不对tt进行修改了
			}
			else if(year < 0)
			{
				//加入一年的时间
				offset += y_days*24*3600;
			}
		}
	}

	if(day_of_week >= 0 && day_of_week != tt.tm_wday)
	{
		//指定周的日子(有多个时间点)
		int d = day_of_week - tt.tm_wday;
		if(d <0) d = 7 +d;
		if(offset == 0)
		{
			if((tt.tm_mday + d > m_days) && (month >= 0 || tt.tm_mon == 11))
			{
				//超过了月份或者年份要求
				return DUR + 3600;
			}

			offset += d * 24 * 3600;
		}
		else
		{
			//经过了月份和年份的调整，目前肯定是一号
			offset += d*24*3600;
		}
	}

	if(hour >=0 && hour != tt.tm_hour)
	{
		//小时的调整
		offset += (hour - tt.tm_hour)*3600;
	}

	ASSERT(min >= 0);
	if(min >=0 && min != tt.tm_min)
	{
		offset += (min - tt.tm_min)*60;
	}

	offset -= tt.tm_sec;

	if(offset > 0)
	{
		if(day_of_months < 0 && day_of_week < 0)
		{
			while(offset > 24*3600)
			{
				offset -= 24*3600;
			}
		} else if(day_of_week >= 0 && day_of_months < 0)
		{
			while(offset > 7 *24 *3600 && tt.tm_mday > 7)
			{
				offset -= 7*24*3600;
				tt.tm_mday -= 7;
			}
		}

		if(hour < 0)
		{
			while(offset > 3600 && tt.tm_hour > 0)
			{
				offset -= 3600;
				tt.tm_hour --;
			}
		}

		return offset;
	}
	if(offset < 0)
	{
		//能够如此的，肯定年份日期周都没有进行调整
		if(hour < 0)
		{
			for(int h = tt.tm_hour; h < 23 && offset < 0; h++)
			{
				offset += 3600;
			}       
			if(offset > 0)
				return offset;
			else    
			{
				//靠小时调整无法完成，则需要调整更大的数值
				//所以将小时归0
				offset -= 3600*23;
			}
		}

		if(day_of_months < 0 && day_of_week < 0)
		{
			//进行天数的调整
			int d = tt.tm_mday;
			while(offset < 0 && d < m_days)
			{
				offset += 24*3600;
				d ++;
			}
			if(offset < 0) //如果还是小于0
				offset -= d*24*3600;
			else
				return offset;
		}
		else if(day_of_week > 0)
		{
			//进行以周为单位的调整
			if((tt.tm_mday + 7 > m_days) && (month >= 0 || tt.tm_mon == 11))
			{
				//超过了月份或者年份要求
				return DUR + 3600;
			}

			offset += 7 * 24 * 3600;
			return offset;
		}
		return DUR + 3600;
	}
	return offset;
}


void 
npc_spawner::RegenAddon(gnpc_imp * pImp, int npc_id)
{
	npc_template * pTemplate = npc_stubs_manager::Get(npc_id);
	if(!pTemplate) return;
	pImp->_money_scale = 1.0f;
	pImp->_basic = pTemplate->bp;
	pImp->_base_prop = pTemplate->ep;
	pImp->_cur_item.attack_range = pTemplate->ep.attack_range;
	pImp->_base_prop.attack_range += pTemplate->body_size;
	pImp->_cur_prop = pImp->_base_prop;

	//现在没有addon了，所以只需无需做其它行为
}

gnpc* npc_spawner::CreatePetBase(gplayer_imp *pMaster,const A3DVECTOR& pos,const pet_data * pData,   
	const pet_bedge_essence* pEss,const int cid[3],unsigned char dir,                          
	int ai_policy_cid,int aggro_policy,int tid)
{
	const pet_bedge_data_temp* pTemplate = pet_bedge_dataman::Get(tid);
	if(NULL == pTemplate)
	{
		GLog::log(GLOG_ERR,"宠物信息: Invalid pet template id %d",tid);
		__PRINTF("宠物信息: Invalid pet template id %d",tid);
		return NULL;
	}
	//分配NPC设置npc全局数据
	gnpc* pNPC = NULL;
	world* pPlane = pMaster->_plane;
	pNPC = pPlane->AllocNPC();
	if(!pNPC) return NULL;
	pNPC->Clear();          //因为有可能zombie状态没有正确清除
	pNPC->SetActive();
	pNPC->ID.type = GM_TYPE_NPC;
	pNPC->ID.id= MERGE_ID<gnpc>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));
	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	gpet_imp* pImp = (gpet_imp*)pNPC->imp;
	pNPC->tid = tid;
	pNPC->vis_tid = tid;
	pImp->_faction = pMaster->GetFaction();
	pImp->_inhabit_mode = 0;
	pImp->_enemy_faction = pMaster->GetEnemyFaction();
	pImp->_base_prop.walk_speed = pTemplate->walk_speed;
	pImp->_base_prop.run_speed = pTemplate->run_speed;
	pImp->_base_prop.attack_range = pTemplate->attack_range;
	pImp->_base_prop.attack_range += pTemplate->body_size;

	//计算基本属性
	pet_manager::GenerateBaseProp(*pEss,*pTemplate,pImp->_base_prop);
	//计算增强属性
	pet_manager::GenerateEnhancedProp(*pEss,*pTemplate,pImp->_en_point);
	//需要加入装备效果
	int equip_mask = 0;
	pet_manager::AddEquipProp(pMaster,*pEss,pImp->_base_prop,equip_mask);

	pImp->_basic.hp = pEss->GetCurHP();
	if(pImp->_basic.hp <= 0) pImp->_basic.hp = 1;
	if(pImp->_basic.hp > pImp->_base_prop.max_hp) pImp->_basic.hp = pImp->_base_prop.max_hp;
	pImp->_basic.mp = pEss->GetCurVigor();
	if(pImp->_basic.mp < 0) pImp->_basic.mp = 0;
	if(pImp->_basic.mp > pImp->_base_prop.max_mp) pImp->_basic.mp = pImp->_base_prop.max_mp;
	pImp->_basic.level = pEss->GetLevel();

	pImp->_cur_prop = pImp->_base_prop;
	//设置宠物ai
	aggro_param aggp;
	ai_param aip;
	memset(&aggp,0,sizeof(aggp));
	memset(&aip,0,sizeof(aip));
	aip.trigger_policy = 0;
	aip.policy_class = ai_policy_cid;
	aggp.aggro_policy = aggro_policy;
	//模板里没有aggro_range
	//aggp.aggro_range = pTemplate->aggro_range;
	aggp.aggro_range = pTemplate->sight_range;
	aggp.sight_range = pTemplate->sight_range;
	//模板里没有aggro_time
	//aggp.aggro_time = (int)pTemplate->aggro_time;
	aggp.aggro_time = 0x7FFFFFF;

	//验证和复制技能
	aip.skills.as_count = 0;
	aip.skills.bs_count = 0;
	aip.skills.cs_count = 0;
	for(size_t i = 0;i < pet_bedge_enhanced_essence::MAX_PET_SKILL_COUNT;++i )
	{
		if(0 != pEss->GetSkillID(i))
		{
			aip.skills.attack_skills[i].id = pEss->GetSkillID(i);
			aip.skills.attack_skills[i].level = pEss->GetSkillLevel(i);
			pImp->AddSkill(aip.skills.attack_skills[i].id,aip.skills.attack_skills[i].level);
			++aip.skills.as_count;
		}
	}

	gnpc_controller* pCtrl = (gnpc_controller*)pImp->_commander;
	pCtrl->CreateAI(aggp,aip);
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);
	pImp->_base_prop.anti_invisible_rate = pImp->GetObjectLevel();
	pNPC->anti_invisible_rate = pImp->_base_prop.anti_invisible_rate;
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_imp)));
	pNPC->body_size = pTemplate->body_size;
	pImp->SetAttackHook( pMaster->GetHookAttackJudge(), pMaster->GetHookEnchantJudge(),
			pMaster->GetHookAttackFill(), pMaster->GetHookEnchantFill() );
	return pNPC;
}

gnpc * 
npc_spawner::CreateMobBase(npc_spawner * __this,world * pPlane,const entry_t & et, 
		int spawn_index, int phase_id, const A3DVECTOR & pos,const int cid[3],
		unsigned char dir, int ai_policy_cid,int aggro_policy, gnpc * origin_npc,int life,
		const void *script_data,size_t script_size)
{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);

	if(pTemplate == NULL)
	{
		GLog::log(GLOG_ERR,"Invalid npc template id %d",et.npc_tid);
		ASSERT(false && "Invlid npc template id");
		return NULL;
	}

	if(forbid_manager::IsForbidNPC(et.npc_tid))
	{
		return NULL;
	}
	
	if(pTemplate->is_boss)
	{
		aggro_policy = AGGRO_POLICY_BOSS;
	}
	if(pTemplate->role_in_war == ROLE_IN_WAR_TURRET)//  && ai_policy_cid == CLS_NPC_AI_POLICY_BASE)
	{
		//如果是工程车那么更改成攻城车策略
		ai_policy_cid =  CLS_NPC_AI_POLICY_TURRET;
		aggro_policy = AGGRO_POLICY_TURRET;
	}
	//分配NPC设置npc全局数据
	gnpc * pNPC = NULL;
	if(!origin_npc)
	{
		pNPC = pPlane->AllocNPC();
		if(!pNPC) return NULL;
		pNPC->Clear();		//因为有可能zombie状态没有正确清除
		pNPC->SetActive();
		pNPC->ID.type = GM_TYPE_NPC;
		pNPC->ID.id= MERGE_ID<gnpc>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));
	}
	else
	{
		pNPC = origin_npc;
	}

	//如果是城战的npc心跳加快
	if(pTemplate->role_in_war || pTemplate->no_idle) pNPC->npc_idle_hearbeat = 1;

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->spawn_index = spawn_index;
	//根据怪物是否会进入idle模式设置timer
	pNPC->idle_timer = 0;
	pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
	pNPC->tid = et.npc_tid; 
	pNPC->vis_tid = et.npc_tid; 
	pNPC->monster_faction = pTemplate->monster_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);
	pNPC->phase_id = phase_id;
	if(pNPC->phase_id > 0)
	{
		pNPC->SetObjectState(gactive_object::STATE_PHASE);
	}

	//设置npc基础属性
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_imp)));
	gnpc_imp * pImp = (gnpc_imp *)pNPC->imp;
	pImp->_basic = pTemplate->bp;
	pImp->_base_prop = pTemplate->ep;
	pImp->_base_prop.attack_range += pTemplate->body_size;
	pImp->_cur_item=  pTemplate->ip;
	pImp->_en_point.spec_damage = pTemplate->spec_damage;
	pImp->_en_point.dmg_reduce = pTemplate->dmg_reduce;
	pImp->_base_prop.anti_invisible_rate = pTemplate->bp.level;
	if(pTemplate->is_boss)
	{
		pImp->_base_prop.anti_invisible_rate += 200;
	}
	pNPC->anti_invisible_rate = pImp->_base_prop.anti_invisible_rate;
	
	pImp->_is_fly = pTemplate->isfly;  // Youshuang add
	pImp->_show_damage = pTemplate->show_damage;  // Youshuang add
	pImp->_spawner = __this;
	pImp->_faction = pTemplate->faction;
	pImp->_enemy_faction = pTemplate->enemy_faction;
	pImp->_after_death = pTemplate->after_death;
	pImp->_immune_state = pTemplate->immune_type;
	pImp->_inhabit_mode = 0;
	pImp->_corpse_delay = et.corpse_delay;
	pImp->_drop_no_protected = pTemplate->drop_no_protected;
	pImp->_is_boss = pTemplate->is_boss;
	pImp->_fix_drop_rate = pTemplate->is_skill_drop_adjust ? 0:1;
	pImp->_no_exp_punish = pTemplate->no_exp_punish;
	pImp->_player_cannot_attack = pTemplate->player_cannot_attack;
	pImp->_init_path_id = et.path_id; 
	pImp->_init_path_type = et.path_type;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTemplate->body_size;
	pNPC->anti_invisible_rate = pImp->_base_prop.anti_invisible_rate;
	pImp->_share_npc_kill = pTemplate->task_share;		
	pImp->_share_drop = pTemplate->item_drop_share;			
	pImp->_buffarea_tid = pTemplate->buff_area_id;

	if(pTemplate->role_in_war)
	{
		pImp->SetFastRegen(0);
		pImp->SetBattleNPC(true);
	}
	else
	{
		pImp->SetBattleNPC(false);
	}

	//设置主动和被动
	if(pTemplate->aggressive_mode)
	{
		pNPC->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}
	pNPC->msg_mask &= et.msg_mask_and;
	pNPC->msg_mask |= et.msg_mask_or;

	//创建AI控制的对象  还未引入仇恨度策略
	aggro_param aggp;
	ai_param aip;
	memset(&aggp,0,sizeof(aggp));
	memset(&aip,0,sizeof(aip));
	if(ai_policy_cid)
	{
		aip.trigger_policy = pTemplate->trigger_policy;
		aggp.aggro_policy = aggro_policy;	

		aggp.aggro_range = pTemplate->aggro_range;
		aggp.aggro_time = pTemplate->aggro_time; 
		aggp.sight_range = pTemplate->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;
		aggp.faction_ask_help = et.ask_for_help?et.monster_faction_ask_help:pTemplate->monster_faction_ask_help;
		aggp.faction_accept_help = et.accept_ask_for_help?et.monster_faction_accept_for_help:pTemplate->monster_faction_can_help;

		if(aggp.faction_accept_help)
		{
			pNPC->msg_mask |= gobject::MSG_MASK_CRY_FOR_HELP;
		}

		aip.policy_class = ai_policy_cid;
		
		aip.patrol_mode = pTemplate->patrol_mode;
		aip.primary_strategy = pTemplate->id_strategy; 

		//验证和复制技能
		ASSERT(sizeof(aip.skills) >= sizeof(pTemplate->skills));
		memcpy(&aip.skills,&pTemplate->skills,sizeof(pTemplate->skills));

		aip.path_id = et.path_id;
		aip.path_type = et.path_type;
		aip.speed_flag = et.speed_flag;
		aip.script_data = (const char *)script_data; //-_-!
		aip.script_size = script_size;

	}
	if(pTemplate->role_in_war)
	{
		pPlane->GetWorldManager()->AdjustNPCAttrInBattlefield(pImp,aip,ai_policy_cid);
		pPlane->GetWorldManager()->RecordBattleNPC(pImp);
	}
	if(ai_policy_cid)
	{
		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);
		//创建AI之后才能设定寿命
		pCtrl->SetLifeTime(life);
	}
	pImp->_cur_prop = pImp->_base_prop;
	return pNPC;
}

gnpc *
npc_spawner::CreateSummonBase(gplayer_imp *pCaster, const A3DVECTOR& pos, const int cid[3], unsigned char dir,
	       	int ai_policy_cid, int aggro_policy, int tid, int life, bool is_clone)
{
	npc_template * pTemplate = npc_stubs_manager::Get(tid);
	if(pTemplate == NULL)
	{
		GLog::log(GLOG_ERR,"Invalid npc template id %d",tid);
		ASSERT(false && "Invlid npc template id");
		return NULL;
	}

	//分配NPC设置npc全局数据
	gnpc * pNPC = NULL;
	world* pPlane = pCaster->_plane;
	pNPC = pPlane->AllocNPC();
	if(!pNPC) return NULL;
	pNPC->Clear();		//因为有可能zombie状态没有正确清除
	pNPC->SetActive();
	pNPC->ID.type = GM_TYPE_NPC;
	if(!is_clone)
	{
		pNPC->ID.id= MERGE_ID<gnpc>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));
	}
	else
	{
		pNPC->ID.id= MERGE_ID<gclonenpc>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));
	}

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->tid = tid; 
	pNPC->vis_tid = tid; 
	pNPC->monster_faction = pTemplate->monster_faction;
	pNPC->dir = dir;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTemplate->body_size;

	//设置主动和被动
	if(pTemplate->aggressive_mode)
	{
		pNPC->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}

	//设置npc基础属性
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_imp)));
	gsummon_imp * pImp = (gsummon_imp *)pNPC->imp;
	pImp->_basic = pTemplate->bp;
	pImp->_base_prop = pTemplate->ep;
	pImp->_base_prop.attack_range += pTemplate->body_size;
	pImp->_cur_item=  pTemplate->ip;
	pImp->_en_point.spec_damage = pTemplate->spec_damage;
	pImp->_en_point.dmg_reduce = pTemplate->dmg_reduce;
	pImp->_inhabit_mode = 0;
	pImp->_immune_state = pTemplate->immune_type;
	pImp->_faction = pCaster->GetFaction();
	pImp->_enemy_faction = pCaster->GetEnemyFaction();
	pImp->_player_cannot_attack = pTemplate->player_cannot_attack;
	pImp->_share_npc_kill = pTemplate->task_share;		
	pImp->_share_drop = pTemplate->item_drop_share;			
	pImp->_buffarea_tid = pTemplate->buff_area_id;

	//创建AI控制的对象  还未引入仇恨度策略
	aggro_param aggp;
	ai_param aip;
	memset(&aggp,0,sizeof(aggp));
	memset(&aip,0,sizeof(aip));
	if(ai_policy_cid)
	{
		aggp.aggro_policy = aggro_policy;	
		aggp.aggro_range = pTemplate->aggro_range;
		aggp.aggro_time = 0x7FFFFFF;
		aggp.sight_range = pTemplate->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;

		aip.policy_class = ai_policy_cid;
		aip.trigger_policy = pTemplate->trigger_policy;
		aip.primary_strategy = pTemplate->id_strategy; 

		//验证和复制技能
		ASSERT(sizeof(aip.skills) >= sizeof(pTemplate->skills));
		memcpy(&aip.skills,&pTemplate->skills,sizeof(pTemplate->skills));

	}
	if(ai_policy_cid)
	{
		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);
		//创建AI之后才能设定寿命
		pCtrl->SetLifeTime(life);
	}
	
	pImp->SetAttackHook( pCaster->GetHookAttackJudge(), pCaster->GetHookEnchantJudge(),
			pCaster->GetHookAttackFill(), pCaster->GetHookEnchantFill() );
	return pNPC;
}


gnpc * 
npc_spawner::CreateMobActiveBase(npc_spawner * __this,world * pPlane,const entry_t & et, 
		int spawn_index, const A3DVECTOR & pos,const int cid[3],
		unsigned char dir, int ai_policy_cid,int aggro_policy, gnpc * origin_npc,int life,
		const void *script_data,size_t script_size)
{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);

	if(pTemplate == NULL)
	{
		GLog::log(GLOG_ERR,"Invalid mob active npc template id %d",et.npc_tid);
		ASSERT(false && "Invlid mob active npc template id");
		return NULL;
	}

	if(forbid_manager::IsForbidNPC(et.npc_tid))
	{
		return NULL;
	}
	
	//分配NPC设置npc全局数据
	gnpc * pNPC = NULL;
	if(!origin_npc)
	{
		pNPC = pPlane->AllocNPC();
		if(!pNPC) return NULL;
		pNPC->Clear();		//因为有可能zombie状态没有正确清除
		pNPC->SetActive();
		pNPC->ID.type = GM_TYPE_NPC;
		pNPC->ID.id= MERGE_ID<gnpc>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));
	}
	else
	{
		pNPC = origin_npc;
	}

	if (pTemplate->no_idle) pNPC->npc_idle_hearbeat = 1;
	if (pTemplate->mob_active_data->path_id)
	{
		pNPC->npc_idle_hearbeat = 1;
		ai_policy_cid = CLS_NPC_AI_POLICY_MOB_ACTIVE;
	}

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->spawn_index = spawn_index;
	//根据怪物是否会进入idle模式设置timer
	pNPC->idle_timer = 0;
	pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
	pNPC->tid = et.npc_tid; 
	pNPC->vis_tid = et.npc_tid; 
	pNPC->monster_faction = pTemplate->monster_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);

	//设置npc基础属性
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_imp)));
	mob_active_imp * pImp = (mob_active_imp *)pNPC->imp;
	pImp->_basic = pTemplate->bp;
	pImp->_base_prop = pTemplate->ep;
	pImp->_base_prop.attack_range += pTemplate->body_size;
	if (pTemplate->mob_active_data->speed > 1e-3)
	{
		pImp->_base_prop.walk_speed = pTemplate->mob_active_data->speed;
		pImp->_base_prop.run_speed = pTemplate->mob_active_data->speed;
	}
	pImp->_cur_item=  pTemplate->ip;
	pImp->_en_point.spec_damage = pTemplate->spec_damage;
	pImp->_en_point.dmg_reduce = pTemplate->dmg_reduce;
	pImp->_base_prop.anti_invisible_rate = pTemplate->bp.level;
	pNPC->anti_invisible_rate = pImp->_base_prop.anti_invisible_rate;
	
	pImp->_is_fly = pTemplate->isfly;
	pImp->_spawner = __this;
	pImp->_faction = pTemplate->faction;
	pImp->_enemy_faction = pTemplate->enemy_faction;
	pImp->_after_death = pTemplate->after_death;
	pImp->_immune_state = pTemplate->immune_type;
	pImp->_inhabit_mode = 0;
	pImp->_corpse_delay = et.corpse_delay;
	pImp->_drop_no_protected = pTemplate->drop_no_protected;
	pImp->_is_boss = pTemplate->is_boss;
	pImp->_fix_drop_rate = pTemplate->is_skill_drop_adjust ? 0:1;
	pImp->_no_exp_punish = pTemplate->no_exp_punish;
	pImp->_player_cannot_attack = pTemplate->player_cannot_attack;
	pImp->_init_path_id = 0; 
	pImp->_init_path_type = et.path_type;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTemplate->body_size;
	pNPC->anti_invisible_rate = pImp->_base_prop.anti_invisible_rate;
	pImp->_share_npc_kill = pTemplate->task_share;		
	pImp->_share_drop = pTemplate->item_drop_share;			
	pImp->_buffarea_tid = pTemplate->buff_area_id;

	pImp->SetBattleNPC(false);

	//设置主动和被动
	if(pTemplate->aggressive_mode)
	{
		pNPC->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}
	pNPC->msg_mask &= et.msg_mask_and;
	pNPC->msg_mask |= et.msg_mask_or;

	//创建AI控制的对象  还未引入仇恨度策略
	aggro_param aggp;
	ai_param aip;
	memset(&aggp,0,sizeof(aggp));
	memset(&aip,0,sizeof(aip));
	if(ai_policy_cid)
	{
		aip.trigger_policy = pTemplate->trigger_policy;
		aggp.aggro_policy = aggro_policy;	

		aggp.aggro_range = pTemplate->aggro_range;
		aggp.aggro_time = pTemplate->aggro_time; 
		aggp.sight_range = pTemplate->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;
		aggp.faction_ask_help = et.ask_for_help?et.monster_faction_ask_help:pTemplate->monster_faction_ask_help;
		aggp.faction_accept_help = et.accept_ask_for_help?et.monster_faction_accept_for_help:pTemplate->monster_faction_can_help;

		if(aggp.faction_accept_help)
		{
			pNPC->msg_mask |= gobject::MSG_MASK_CRY_FOR_HELP;
		}

		aip.policy_class = ai_policy_cid;
		
		aip.patrol_mode = pTemplate->patrol_mode;
		aip.primary_strategy = pTemplate->id_strategy; 

		//验证和复制技能
		ASSERT(sizeof(aip.skills) >= sizeof(pTemplate->skills));
		memcpy(&aip.skills,&pTemplate->skills,sizeof(pTemplate->skills));

		aip.path_id = 0;
		aip.path_type = et.path_type;
		aip.speed_flag = et.speed_flag;
		aip.script_data = (const char *)script_data; //-_-!
		aip.script_size = script_size;

	}

	mobactive_spawner::GenerateMobActiveParam(pImp,pTemplate);

	pImp->_mob_active_life = life;
	if(ai_policy_cid)
	{
		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);
		//创建AI之后才能设定寿命
		pCtrl->SetLifeTime(life);
	}
	pImp->_cur_prop = pImp->_base_prop;
	return pNPC;
}

void
npc_spawner::ClearObjects(world * pPlane)
{
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gnpc * pHeader = _npc_pool[i];
		if(pHeader == NULL) continue;

		//将所有在等待池里的ID删掉，并且将整个对象也删掉

		gnpc * tmp = pHeader;
		do
		{
			_xid_list.erase(tmp->ID);
			gnpc * pNext = (gnpc*)tmp->pNext;
			tmp->Lock();
			tmp->imp->_commander->Release();
			tmp->Unlock();
			tmp = pNext;
		}while(tmp != pHeader);
		_npc_pool[i] = NULL;
	}

	//给所有尚未删除的对象发送消息 让其消失
	if(_xid_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_SPAWN_DISAPPEAR,XID(-1,-1),XID(-1,-1),A3DVECTOR(0,0,0),0);
		abase::vector<XID> list;
		abase::static_multimap<XID,int, abase::fast_alloc<> >::iterator it = _xid_list.begin();
		list.reserve(_xid_list.size());
		for(;it != _xid_list.end(); ++it)
		{
			list.push_back(it->first);
		}
		gmatrix::SendMessage(list.begin(),list.end(), msg);
		_xid_list.clear();
	}
}

gnpc *
mobs_spawner::CreateMob(world * pPlane,int index,const entry_t & et, bool auto_detach)
{
	A3DVECTOR pos;
	char dir;
	GeneratePos(pos,dir,et.offset_terrain, pPlane->GetWorldManager(),NULL);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC = CreateMobBase(NULL,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_BASE,0,NULL,_mob_life);
	}
	else
	{
		pNPC = CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_BASE,0,NULL,_mob_life);
	}
	if(pNPC)
	{
		pPlane->InsertNPC(pNPC);
		pNPC->imp->_runner->enter_world();
		pNPC->imp->OnCreate();
		pNPC->Unlock();
	}
	return pNPC;
}

void 
mobs_spawner::ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR &pos, char dir, int index)
{
	XID oldID = pNPC->ID;
	pNPC->Clear();
	pNPC->SetActive();
	pNPC->ID = oldID;

	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	entry_t &et = _entry_list[index];
	gnpc * pNPC2 = CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_BASE,0,pNPC,_mob_life);
	ASSERT(pNPC2 == pNPC);
	pNPC2 = NULL;

	pPlane->InsertNPC(pNPC);
	pNPC->imp->_runner->enter_world();
	pNPC->imp->OnCreate();
}

bool 
mobs_spawner::CreateMobs(world * pPlane,bool auto_detach)
{
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		size_t count = _entry_list[i].mobs_count;
		for(size_t j = 0; j < count; j ++)
		{
			gnpc * pNPC = CreateMob(pPlane,i,_entry_list[i], auto_detach);
			if(pNPC == NULL) return false;
			if(!auto_detach)
			{
				_xid_list[pNPC->ID] = 1;
				_mobs_cur_gen_num ++;
			}
			
		}
	}
	return true;
}

bool
mobs_spawner::Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * imp, bool is_reset)
{
	if(!_auto_respawn || !_is_spawned || IsMobActiveReclaim())
	{
		imp->_commander->Release();
		return false;
	}
	slice *pPiece = pNPC->pPiece;
	if(pPiece)
	{
		pPlane->RemoveNPC(pNPC);
	}       
	else
	{
		//没有piece，也肯定在管理器中
		pPlane->RemoveNPCFromMan(pNPC);
	}
	pNPC->ClrActive();

	pNPC->npc_reborn_flag = is_reset?1:0;

	size_t index = pNPC->spawn_index;
	ASSERT(index < _entry_list.size());
	if(index >= _entry_list.size())
	{
		//做日志，致命错误 
		GLog::log(GLOG_ERR,"回收NPC时发现错误的生成器索引");
		ASSERT(false);
		return false;
	}

	spin_autolock  keeper(_spool_lock); //$$LOCK$$
	if(!_is_spawned || (_mobs_total_gen_num && _mobs_cur_gen_num >= _mobs_total_gen_num))
	{
		_xid_list.erase(pNPC->ID);
		//check again
		imp->_commander->Release();
		return false;
	}

	//如果是封禁的NPC则直接回收
	if(forbid_manager::IsForbidNPC(pNPC->tid))
	{
		_xid_list.erase(pNPC->ID);
		//check again
		imp->_commander->Release();
		return false;
	}
	
	_mobs_cur_gen_num ++;
	gnpc * pTmp = _npc_pool[index];
	int tick = g_timer.get_tick();
	pNPC->native_state = tick + _entry_list[index].reborn_time;
	if(pTmp)
	{
		pNPC->pNext= pTmp;
		pNPC->pPrev= pTmp->pPrev;
		pTmp->pPrev->pNext = pNPC;
		pTmp->pPrev = pNPC;
	}
	else
	{
		_npc_pool[index] = pNPC;
		pNPC->pPrev = pNPC->pNext = pNPC;
	}
	__PRINTF("回收了npc %d at %d/%d\n",pNPC->ID.id & 0x7FFFFFFF,tick,pNPC->native_state);
	return true;
}

void 
mobs_spawner::Release()
{
	spin_autolock  keeper(_spool_lock);
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gnpc* header = _npc_pool[i];
		if(!header) continue;

		gnpc * pTmp = (gnpc *)header;
		do
		{
			gnpc * pNext = (gnpc*)pTmp->pNext;
			spin_autolock keeper(pTmp->spinlock);
			pTmp->imp->_commander->Release();
			pTmp = pNext;
		} while(pTmp!= header);
		_npc_pool[i] = NULL;
	}
	_entry_list.clear();
	_npc_pool.clear();
}

void 
mobs_spawner::OnHeartbeat(world * pPlane)
{
	int tick = g_timer.get_tick();
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gnpc* header = _npc_pool[i];
		if(!header) continue;

		if(header->native_state - tick > 0) continue;
		gnpc * pTmp =(gnpc*) header->pNext;
		while(pTmp != header && pTmp->native_state - tick <= 0)
		{
			pTmp = (gnpc*)pTmp->pNext;
		}
		if(pTmp == header)
		{
			_npc_pool[i] = NULL;
		}
		else
		{
			_npc_pool[i] = pTmp;
			header->pPrev->pNext = pTmp;
			pTmp ->pPrev = header->pPrev;
		}

		//这时候其实可以不加锁，不过看在这个事件不是那么频繁上，估计无大碍
		//开始重生
		Reborn(pPlane,header,pTmp,_entry_list[i].offset_terrain,_entry_list[i].npc_tid);
	}
}

void 
mobs_spawner::Reborn(world * pPlane, gnpc * header, gnpc * tail,float offset_terrain,int tid)
{
	int tick = g_timer.get_tick();
	do
	{
		gnpc * pNPC = header;
		header = (gnpc*)header->pNext;
		ASSERT(pNPC->b_zombie == true);
		ASSERT(pNPC->pPiece == NULL);

		pNPC->Lock();
		char dir;
		int collision_id;
		GeneratePos(pNPC->pos,dir, offset_terrain,pPlane->GetWorldManager(),&collision_id);

		int tmp_tick = pNPC->native_state;

		pNPC->native_state = gnpc::TYPE_NATIVE;
		pNPC->object_state = 0; 
		pNPC->dir = dir;
		pNPC->collision_id = collision_id;

		if(_is_spawned)
		{
			if(pNPC->npc_reborn_flag)
			{
				//需要彻底重新生成
				 size_t index = pNPC->spawn_index;
				 ReCreate(pPlane,pNPC,pNPC->pos,dir, index);
			}
			else
			{
				ASSERT(pNPC->tid == tid);
				pNPC->SetActive();
				pNPC->pPrev = pNPC->pNext = NULL;
				pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
				pPlane->InsertNPC(pNPC);
				if(tick - pNPC->pPiece->idle_timestamp > NPC_IDLE_TIMER * TICK_PER_SEC)
				{
					pNPC->idle_timer = 0;
				}
				else
				{
					pNPC->idle_timer = NPC_IDLE_TIMER;
				}
				//现在每次都试图生成随机属性
				RegenAddon((gnpc_imp*)pNPC->imp,tid);

				pNPC->imp->Reborn();
				((gnpc_imp*)(pNPC->imp))->SetLifeTime(_mob_life);
				ASSERT(pNPC->b_zombie == false);
				ASSERT(((gnpc_imp*)(pNPC->imp))->_spawner == this);
			}
		}
		else
		{
			//不处于生成中的spawner 直接释放不符合的NPC
			pNPC->imp->_commander->Release();
		}
		pNPC->Unlock();
		__PRINTF("npc %d(%d/%d)重新站起来啦\n",pNPC->ID.id & 0x7FFFFFFF,tmp_tick,tick);

	} while(header != tail);
}



bool 
npc_generator::InitIncubator(world * pPlane)		//初始化所有的spawner，去掉不属于自己的部分
{
	_plane = pPlane;
	return true;
}

bool
npc_generator::BeginSpawn()
{
	for(size_t i = 0; i < _ctrl_list.size();i ++)
	{
		if(!_ctrl_list[i]->IsAutoSpawn()) continue;
		if(!_ctrl_list[i]->BeginSpawn(_plane)) return false;
	}
	return true;
}

bool
npc_generator::TriggerSpawn(int condition)
{
	//加个锁意思意思
	spin_autolock keeper(_tlock);

	spawner_ctrl * ctrl = _ctrl_map[condition];
	if(ctrl == NULL) return false;
	ctrl->BeginSpawn(_plane);
	return true;
}

void
npc_generator::ClearSpawn(int condition)
{
	//加个锁意思意思
	spin_autolock keeper(_tlock);

	spawner_ctrl * ctrl = _ctrl_map[condition];
	if(ctrl == NULL) return ;
	ctrl->EndSpawn(_plane);
}


void
npc_generator::Release()
{	
	for(size_t i = 0; i < _ctrl_list.size();i ++)
	{
		delete _ctrl_list[i];
	}
	_ctrl_list.clear();

	for(size_t i = 0; i < _sp_list.size();i ++)
	{
		_sp_list[i]->ReleaseSelf();
	}
	_sp_list.clear();
	
	_ctrl_map.clear();
	_ctrl_idx_map.clear();

	_task_offset = 0;
	_tcount = 0; 
	_tlock = 0; 
	_plane = 0;
	
}

static int first_load_gen = 0;

bool 
npc_generator::LoadGenData(world_manager* manager,CNPCGenMan & npcgen, rect & world_region)
{
	//读取控制对象数据
	size_t count = npcgen.GetNPCCtrlNum();

	for(size_t i = 0; i < count; i ++)
	{
		const CNPCGenMan::CONTROLLER & ctrl = npcgen.GetController(i);
		int id = ctrl.id;
		if(id == 0) 
		{
			printf("控制器%d里发现了非法的ID 0\n",id);
			return false;
		}

		crontab_t::entry_t act_date,stop_date;
		act_date.min		= ctrl.ActiveTime.iMinutes;
		act_date.hour		= ctrl.ActiveTime.iHours;
		act_date.month		= ctrl.ActiveTime.iMouth;
		act_date.year		= ctrl.ActiveTime.iYear;
		act_date.day_of_months	= ctrl.ActiveTime.iDay;
		act_date.day_of_week	= ctrl.ActiveTime.iWeek;

		stop_date.min		= ctrl.StopTime.iMinutes;
		stop_date.hour		= ctrl.StopTime.iHours;
		stop_date.month		= ctrl.StopTime.iMouth;
		stop_date.year		= ctrl.StopTime.iYear;
		stop_date.day_of_months	= ctrl.StopTime.iDay;
		stop_date.day_of_week	= ctrl.StopTime.iWeek;
		
		int tempcond = manager->TranslateCtrlID(ctrl.iControllerID);
		InsertSpawnControl(id, tempcond, ctrl.bActived, ctrl.iWaitTime, ctrl.iStopTime, 
					ctrl.iActiveTimeRange,
					!ctrl.bActiveTimeInvalid?&act_date:NULL,
					!ctrl.bStopTimeInvalid?&stop_date:NULL , 
					ctrl.bRepeatActived);

		//---------------------时间段控制器---------------------------------------
		std::vector<time_segment::entry_t> atimeseg;

		//调用时InsertSpawnCtrlTimeSeg不能放在InsertSpawnControl之前
		for(size_t i = 0; i < ctrl.aTimeSeg.size(); i += 2)
		{
			time_segment::entry_t start_date, end_date;
			start_date.year        = ctrl.aTimeSeg[i].iYear;
			start_date.month       = ctrl.aTimeSeg[i].iMouth;
			start_date.day_of_months = ctrl.aTimeSeg[i].iDay;
			start_date.day_of_week = ctrl.aTimeSeg[i].iWeek;
			start_date.hour        = ctrl.aTimeSeg[i].iHours;
			start_date.min         = ctrl.aTimeSeg[i].iMinutes;

			//结束时间跟在开始时间之后
			end_date.year        = ctrl.aTimeSeg[i+1].iYear;
			end_date.month       = ctrl.aTimeSeg[i+1].iMouth;
			end_date.day_of_months = ctrl.aTimeSeg[i+1].iDay;
			end_date.day_of_week = ctrl.aTimeSeg[i+1].iWeek;
			end_date.hour        = ctrl.aTimeSeg[i+1].iHours;
			end_date.min         = ctrl.aTimeSeg[i+1].iMinutes;

			atimeseg.push_back(start_date);
			atimeseg.push_back(end_date);
		}

		if( atimeseg.size() > 0 )
		{
			bool is_intersection = false;
			if( ctrl.GetTimeSegLogic() )
			{
				is_intersection = true;
			}

			InsertSpawnCtrlTimeSeg(id, atimeseg, is_intersection);
		}
	}
	
	//加入一个默认的控制器
	InsertSpawnControl(0, 0, true,0,0,0);


	//读取NPC和怪物数据
	count = npcgen.GetGenAreaNum();
	for(size_t i = 0; i <count; i ++)
	{
		const CNPCGenMan::AREA & area = npcgen.GetGenArea(i);
		//__PRINTINFO("ai gen:(%f,%f,%f) (%f,%f,%f)\n",area.vPos[0],area.vPos[1],area.vPos[2],area.vExts[0],area.vExts[1],area.vExts[2]);
		rect rt(area.vPos[0]-area.vExts[0]*.5f, area.vPos[2]-area.vExts[2]*.5f, area.vPos[0]+area.vExts[0]*.5f, area.vPos[2]+area.vExts[2]*.5f);
		if(!world_region.IsOverlap(rt) && !world_region.IsIn(rt))
		{
			continue;
		}
		npc_spawner * pSp;
		if(area.iNPCType == 1)
		{
			pSp = new server_spawner();
		}
		else if (area.iNPCType == 2)
		{
			pSp = new mobactive_spawner();
		}
		else
		{
			if(area.iGroupType == 0)
			{
				pSp = new mobs_spawner();
			}
			else if(area.iGroupType == 1)
			{
				pSp = new group_spawner();
			}
			else if(area.iGroupType == 2)
			{
				pSp = new boss_spawner();
			}
			else
			{
				pSp = new mobs_spawner();
			}
		}
		pSp->SetRespawn(area.cRevive);
		pSp->SetRegion(area.iType,area.vPos,area.vExts,a3dvector_to_dir(area.vDir),0,0);
		pSp->SetGenLimit(area.iLifeTime,area.iMaxNum);
		pSp->SetPhaseID(area.iPhase);


		if(!area.aAttachAreas.empty())
		{
			//存在额外附属区域
			for(size_t m = 0; m < area.aAttachAreas.size(); m ++)
			{
				const CNPCGenMan::AREA & na = *npcgen.GetGenAttachArea(area.aAttachAreas[m]);
				ASSERT(&na);
				pSp->SetRegion(na.iType,na.vPos,na.vExts,a3dvector_to_dir(na.vDir),0,0);
			}
		}
		
		size_t entry_count =0;
		for(size_t j = (size_t)area.iFirstGen; j < (size_t)area.iFirstGen + area.iNumGen; j ++)
		{
			const CNPCGenMan::NPCGEN & gen = npcgen.GetGenerator(j);
			npc_spawner::entry_t ent;
			memset(&ent,0,sizeof(ent));
			ent.npc_tid = gen.dwID;
			npc_template * pTemplate = npc_stubs_manager::Get(ent.npc_tid);

			if(pTemplate == NULL)
			{
				printf("在生成器中找不到指定的npc entry. npc id %d at pos(%f,%f,%f)\n",ent.npc_tid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}
			if(ROLE_IN_WAR_TOWN_POS == pTemplate->role_in_war)
			{
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				manager->RecordTownPos(pos,pTemplate->faction);
				continue;
			}
			else if(ROLE_IN_WAR_REBORN_POS == pTemplate->role_in_war)
			{
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				manager->RecordRebornPos(pos,pTemplate->faction,_ctrl_idx_map[area.idCtrl]->GetConditionID());
				continue;
			}
			else if(ROLE_IN_WAR_BATTLE_ENTRY == pTemplate->role_in_war)
			{
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				manager->RecordEntryPos(pos,pTemplate->faction);
				continue;
			}
			else if( ROLE_IN_WAR_BATTLE_FLAG == pTemplate->role_in_war )
			{
				continue;
			}
			else if( ROLE_IN_WAR_BATTLE_NORMAL_BUILDING == pTemplate->role_in_war )
			{
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				manager->RecordBattleBuilding( pos, pTemplate->faction, pTemplate->tid ); 
			}
			else if( ROLE_IN_WAR_BATTLE_KEY_BUILDING == pTemplate->role_in_war )
			{
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				manager->RecordBattleBuilding( pos, pTemplate->faction, pTemplate->tid, true ); 
			}

			if(first_load_gen == 0)
			{
				int cnt = gen.dwNum;
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				if(pTemplate->role_in_war == ROLE_IN_WAR_MAIN_BUILDING)
				{
					//标志性建筑
					manager->RecordMob(0,ent.npc_tid,pos,pTemplate->faction,cnt);
				}
				else if(area.iNPCType)
				{
					//NPC
					manager->RecordMob(1,ent.npc_tid,pos,pTemplate->faction,cnt);
				}
				else 
				{
					if(pTemplate->role_in_war)
					{
						//城战怪
						manager->RecordMob(2,ent.npc_tid,pos,pTemplate->faction,cnt);
					}
					else
					{
						//普通怪
						manager->RecordMob(3,ent.npc_tid,pos,pTemplate->faction,cnt);
					}
				}

			}

			switch(gen.dwAggressive)
			{
				default:
					ASSERT(false);
				case 0:
					ent.msg_mask_or = 0;
					ent.msg_mask_and = ~0;
					break;
				case 1:
					ent.msg_mask_or = gobject::MSG_MASK_PLAYER_MOVE;
					ent.msg_mask_and = ~0;
					break;
				case 2:
					ent.msg_mask_or = 0;
					ent.msg_mask_and = ~gobject::MSG_MASK_PLAYER_MOVE;;
					break;
			}
			ent.alarm_mask = 0;
			ent.enemy_faction = 0;		//not use
			ent.has_faction = !gen.bDefFaction;
			ent.faction = gen.dwFaction;
			ent.ask_for_help = !gen.bDefFacHelper;
			ent.monster_faction_ask_help = gen.dwFacHelper;
			ent.accept_ask_for_help = !gen.bDefFacAccept;
			ent.monster_faction_accept_for_help = gen.bDefFacAccept;

			ent.corpse_delay = gen.iDeadTime;
			if(ent.corpse_delay)
			{
				bool flag = (ent.corpse_delay < 0) || (ent.corpse_delay > 1800);
				if(ent.corpse_delay < 5) ent.corpse_delay = 5;
				if(ent.corpse_delay > 1800) ent.corpse_delay = 1800;
				if(flag) printf("发现错误的尸体残留时间 npc id %d at pos(%f,%f,%f) 时间%d\n",
						ent.npc_tid,
						area.vPos[0],area.vPos[1],area.vPos[2] ,gen.iDeadTime);
			}
			
			
			//加入固定复活时间以合理之
			int reborn_time = BASE_REBORN_TIME + gen.dwRefresh ;
			if(reborn_time < 15) reborn_time = 15;
			if(reborn_time > 2592000) reborn_time = 2592000;
			ent.reborn_time = reborn_time* 20;
			ent.mobs_count = gen.dwNum;
			ent.offset_terrain = gen.fOffsetTrn;
			ent.path_id = gen.iPathID;
			ent.path_type = gen.iLoopType;
			ent.speed_flag = gen.iSpeedFlag;

			pSp->AddEntry(ent);
			if(j == (size_t)area.iFirstGen)
			{
				//为群怪准备的，如果是群怪，使用第一个怪物的复生时间
				pSp->SetRebornTime(ent.reborn_time);
			}
			entry_count  ++;

			if(pTemplate->collision_in_server) pSp->BuildRegionCollision(manager, gen.dwID,0);

		}
		if(entry_count)
		{
			InsertSpawner(area.idCtrl,pSp);
		}
		else 
			delete pSp;
	}

	//加入矿物
	count = npcgen.GetResAreaNum();
	for(size_t i = 0; i <count; i ++)
	{
		const CNPCGenMan::RESAREA & area = npcgen.GetResArea(i);
		//rect rt(area.vPos[0]-area.vExts[0]*.5f, area.vPos[2]-area.vExts[2]*.5f, area.vPos[0]+area.vExts[0]*.5f, area.vPos[2]+area.vExts[2]*.5f);
		rect rt(area.vPos[0]-area.fExtX*0.5f, area.vPos[2]-area.fExtZ*0.5f, area.vPos[0]+area.fExtX*.5f, area.vPos[2]+area.fExtZ*.5f);
		if(!world_region.IsOverlap(rt) && !world_region.IsIn(rt))
		{
			continue;
		}
		mine_spawner * pSp;
		pSp = new mine_spawner();
		float vExt[3] = {0,0,0};
		pSp->SetRegion(1,area.vPos,vExt,area.dir[0],area.dir[1], area.rad);
		pSp->SetRespawn(area.bAutoRevive); 
		pSp->SetGenLimit(area.iMaxNum);
		pSp->SetPhaseID(area.iPhase);

		if(!area.aAttachAreas.empty())
		{
			//存在额外附属区域
			for(size_t m = 0; m < area.aAttachAreas.size(); m ++)
			{
				const CNPCGenMan::RESAREA& na = *npcgen.GetResAttachArea(area.aAttachAreas[m]);
				ASSERT(&na);
				float vExt2[3] = {0,0,0};
				pSp->SetRegion(1,na.vPos,vExt2,na.dir[0],na.dir[1], na.rad);
			}
		}

		size_t entry_count = 0;
		for(size_t j = (size_t)area.iFirstRes; j < (size_t)area.iFirstRes+ area.iResNum; j ++)
		{
			const CNPCGenMan::RES & res = npcgen.GetRes(j);
			mine_spawner::entry_t ent;
			ent.mid = res.idTemplate;
			if(mine_stubs_manager::Get(ent.mid) == NULL)
			{
				printf("在生成器中找不到指定的mine entry. id %d at pos(%f,%f,%f)\n",ent.mid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}
			ent.mine_count = res.dwNumber;
			int reborn_time = BASE_REBORN_TIME + res.dwRefreshTime;
			if(reborn_time < 15) reborn_time = 15;
			ent.reborn_time = reborn_time * 20;
			pSp->SetOffsetTerrain(res.fHeiOff);
			pSp->AddEntry(ent);
			entry_count ++;

			pSp->BuildRegionCollision(manager, ent.mid,0);
			break;
		}
		if(entry_count)
		{
			InsertSpawner(area.idCtrl,pSp);
		}
		else 
			delete pSp;
	}

	//加入动态物品
	count = npcgen.GetDynObjectNum();
	for(size_t i = 0; i <count; i ++)               
	{
		const CNPCGenMan::DYNOBJ & obj = npcgen.GetDynObject(i);
		if(!world_region.IsIn(obj.vPos[0], obj.vPos[2]))
		{
			continue;
		}
		dyn_object_spawner * pSp = new dyn_object_spawner();
		float vExt[3] = {1e-3,1e-3,1e-3}; 
		pSp->SetRegion(1,obj.vPos,vExt,obj.dir[0],obj.dir[1], obj.rad);
		pSp->SetRespawn(true); 
		pSp->BuildRegionCollision(manager, 0, obj.dwDynObjID);
		pSp->SetPhaseID(obj.iPhase);

		mine_spawner::entry_t ent;
		memset(&ent,0,sizeof(ent));
		ent.mid = obj.dwDynObjID;
		ent.mine_count = 1;
		ent.reborn_time = 100;
		pSp->AddEntry(ent); 
		InsertSpawner(obj.idCtrl,pSp);
	}       
	first_load_gen = 1;
	return true;
}
namespace
{
	class terrain_gen_pos : public npc_spawner::generate_pos
	{
		public:
		terrain_gen_pos(const A3DVECTOR &pos_min, const A3DVECTOR &pos_max, char dir,char dir1,char rad):generate_pos(pos_min,pos_max,dir,dir1,rad){}
		virtual void Generate(A3DVECTOR &pos, float offset, world_manager* manager)
		{
			int n = 0;
			do
			{
				pos.x = abase::Rand(_pos_min.x,_pos_max.x);
				pos.z = abase::Rand(_pos_min.z,_pos_max.z);
				pos.y = (_pos_min.y + _pos_max.y)*0.5f;
				if(path_finding::GetValidPos(manager->GetMoveMap(), pos))
				{
					return;
				}
				n ++;
			}while(n < 5);

			pos.y = manager->GetTerrain().GetHeightAt(pos.x,pos.z);
		}
		virtual float GenerateY(float x, float y, float z,float offset, world_manager* manager)
		{
			y = manager->GetTerrain().GetHeightAt(x,z);
			return y + offset;
		}
	};

	class box_gen_pos : public npc_spawner::generate_pos
	{
		public:
		box_gen_pos(const A3DVECTOR &pos_min, const A3DVECTOR &pos_max, char dir,char dir1,char rad):generate_pos(pos_min,pos_max,dir,dir1,rad){}
		virtual void Generate(A3DVECTOR &pos, float offset,world_manager* manager)
		{
			pos.x = abase::Rand(_pos_min.x,_pos_max.x);
			pos.y = abase::Rand(_pos_min.y,_pos_max.y);
			pos.z = abase::Rand(_pos_min.z,_pos_max.z);
			float height = manager->GetTerrain().GetHeightAt(pos.x,pos.z);
			if(pos.y < height) pos.y = height;
			pos.y += offset;
		}

		virtual float GenerateY(float x, float y, float z, float offset, world_manager* manager)
		{
			float height = manager->GetTerrain().GetHeightAt(x,z);
			if(y < height) y = height;
			return y+offset;
		}
	};
}
void base_spawner::SetRegion(int region_type,const float vPos[3],const float vExts[3],
			unsigned char dir0,unsigned char dir1, unsigned char rad)
{
	A3DVECTOR pos_min = A3DVECTOR(vPos[0]-vExts[0]*.5f,vPos[1]-vExts[1]*.5f,vPos[2]-vExts[2]*.5f);
	A3DVECTOR pos_max = A3DVECTOR(vPos[0]+vExts[0]*.5f,vPos[1]+vExts[1]*.5f,vPos[2]+vExts[2]*.5f);

	_region = rect(vPos[0]-vExts[0]*.5f, vPos[2]-vExts[2]*.5f, vPos[0]+vExts[0]*.5f, vPos[2]+vExts[2]*.5f);

	generate_pos * pGen = NULL;
	switch(region_type)
	{
		case 0:
			pGen = new terrain_gen_pos(pos_min,pos_max,dir0,dir1, rad);
			break;
		case 1:
			pGen = new box_gen_pos(pos_min,pos_max,dir0,dir1,rad);
			break;
		default:
			printf("错误的区域类型\n");
			ASSERT(false);
			break;
	}
	_pos_generator.push_back(pGen);
}

void base_spawner::BuildRegionCollision(world_manager * manager, int tid1,int did)
{
	
	for(size_t i =0; i < _pos_generator.size(); i ++)
	{
		A3DVECTOR pos;
		char dir0, dir1,rad;
		_pos_generator[i]->GeneratePos(pos,dir0, 0 ,manager, &dir1,&rad);
		int id = manager->GetTraceMan().RegisterElement(tid1,did, pos, dir0 *(2*3.1415926535/256.f),dir1 *(2*3.1415926535/256.f), (unsigned char)rad * ( 2 * 3.1415926535/256.f));
		_pos_generator[i]->SetCollisionID(id);
	}
}



void 
server_spawner::ReCreate(world * pPlane, gnpc * pNPC,const A3DVECTOR & pos, char dir,int index)
{
	XID oldID = pNPC->ID;
	pNPC->Clear();
	pNPC->SetActive();
	pNPC->ID = oldID;
	CreateNPC(pPlane,index,_entry_list[index],pos,pNPC, false,dir);
}

gnpc * 
server_spawner::CreateNPC(world * pPlane, int index,const entry_t & et, bool auto_detach)
{
	A3DVECTOR pos;
	char dir;
	int collision_id;
	GeneratePos(pos,dir,et.offset_terrain, pPlane->GetWorldManager(),&collision_id);
	gnpc * pNPC = CreateNPC(pPlane,index,et,pos,NULL, auto_detach, dir);
	if(pNPC) pNPC->collision_id = collision_id;
	return pNPC;
}
	
gnpc *
server_spawner::CreateNPC(world * pPlane, int index,const entry_t & et, const A3DVECTOR & pos, gnpc * origin_npc, bool auto_detach, char dir)
{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);
	ASSERT(pTemplate);
	if(!pTemplate) return NULL; 
	if(!pTemplate->npc_data) return NULL;

	int imp_type;
	if(pTemplate->npc_data->npc_type == npc_template::npc_statement::NPC_TYPE_CARRIER)
	{
		imp_type = CLS_CARRIER_NPC_IMP;
	}
	else
	{
		imp_type = CLS_SERVICE_NPC_IMP;
	}

	const int cid[3] = {imp_type,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};

	int aipolicy = 0;
	int aggro_policy = AGGRO_POLICY_BOSS;
	switch(pTemplate->npc_data->npc_type)
	{
		case npc_template::npc_statement::NPC_TYPE_GUARD:
			aipolicy = CLS_NPC_AI_POLICY_GUARD;
			break;
		case npc_template::npc_statement::NPC_TYPE_NORMAL:
			aipolicy = CLS_NPC_AI_POLICY_SERVICE;
			break;
		case npc_template::npc_statement::NPC_TYPE_CARRIER:
			aipolicy = CLS_NPC_AI_POLICY_SERVICE;
			break;
		default:
			//普通的不做任何处理即可
			break;
	}
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC = CreateMobBase(NULL,pPlane,et, index, _phase_id, pos,cid,dir,aipolicy,aggro_policy,origin_npc,_mob_life);
	}
	else
	{
		pNPC = CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,aipolicy,aggro_policy,origin_npc,_mob_life);
	}
	if(!pNPC) return NULL;
	
	//加入各种服务
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(service_npc)) ||
	       pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(carrier_npc)));

	service_npc * pImp = (service_npc *)pNPC->imp;
	pImp->SetTaxRate(pTemplate->npc_data->tax_rate);
	pImp->SetNeedDomain(pTemplate->npc_data->need_domain);
	pImp->SetAttackRule(pTemplate->npc_data->attack_rule);
	pImp->SetTerritoryID(pTemplate->npc_data->id_territory);
	
	AddNPCServices(pImp, pTemplate);
	
	pPlane->InsertNPC(pNPC);
	pNPC->imp->_runner->enter_world();
	pNPC->imp->OnCreate();
	pImp->OnCreate();
	if(!origin_npc)
	{
		//只有当外部没有NPC的时候才这么做
		//因为如果有外部的，那么应该已经加锁了
		pNPC->Unlock();
	}
	return pNPC;
}

void
server_spawner::AddNPCServices(service_npc * pImp, npc_template * pTemplate)
{
	ASSERT(pTemplate->npc_data);
	if(int num = pTemplate->npc_data->service_sell_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_NPC_VENDOR);
		pImp->AddProvider(provider,pTemplate->npc_data->service_sell_goods,sizeof(npc_template::npc_statement::goods_t)*num);
	}

	if(pTemplate->npc_data->service_purchase)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_NPC_PURCHASE);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_heal)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_HEAL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(int num = pTemplate->npc_data->service_transmit_target_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(5);
		pImp->AddProvider(provider,pTemplate->npc_data->transmit_entry, sizeof(npc_template::npc_statement::__st_ent)* num);
	}

	if(pTemplate->npc_data->service_task_in_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_IN);
		pImp->AddProvider(provider,pTemplate->npc_data->service_task_in_list,pTemplate->npc_data->service_task_in_num*sizeof(int));
	}

	if(pTemplate->npc_data->service_task_out_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_OUT);
		pImp->AddProvider(provider,pTemplate->npc_data->service_task_out_list,pTemplate->npc_data->service_task_out_num*sizeof(int));
	}

	if(pTemplate->npc_data->service_task_matter_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_MATTER);
		pImp->AddProvider(provider,pTemplate->npc_data->service_task_matter_list,pTemplate->npc_data->service_task_matter_num*sizeof(int));
	}

	
	if(pTemplate->npc_data->service_install)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_INSTALL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_uninstall)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_UNINSTALL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_reset_pkvalue.has_service)
	{
		service_provider * provider=service_manager::CreateProviderInstance(service_ns::SERVICE_ID_RESET_PK);
		int fee = pTemplate->npc_data->service_reset_pkvalue.fee_per_unit;
		pImp->AddProvider(provider,&fee,sizeof(fee));
	}

	if(pTemplate->npc_data->service_storage)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_PASS);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_OPEN);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_storage_mafia)
	{

		service_provider * provider;
		if(!pTemplate->npc_data->service_storage)
		{
			provider= service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_PASS);
			pImp->AddProvider(provider,NULL,0);
		}

		provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_MAFIA_TB_OPEN);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_waypoint_id > 0)
	{
		service_provider * provider=service_manager::CreateProviderInstance(service_ns::SERVICE_ID_WAYPOINT);
		int wp = pTemplate->npc_data->service_waypoint_id ;
		pImp->AddProvider(provider,&wp,sizeof(wp));
	}

	if(pTemplate->npc_data->service_faction)
	{
		service_provider * provider = service_manager::CreateProviderInstance(18);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_mail)
	{
		service_provider * provider = service_manager::CreateProviderInstance(25);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data->service_auction)
	{
		service_provider * provider = service_manager::CreateProviderInstance(26);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_double_exp)
	{
		service_provider * provider = service_manager::CreateProviderInstance(27);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_hatch_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(28);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_recover_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(29);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_cash_trade)
	{
		//点卡交易系统
		service_provider * provider = service_manager::CreateProviderInstance(38);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(39);
		pImp->AddProvider(provider,NULL,0);
	}

	
	if(pTemplate->npc_data->service_equip_bind)
	{
		int m[] = {     pTemplate->npc_data->service_bind_prop.money_need,
			pTemplate->npc_data->service_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(35);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if(pTemplate->npc_data->service_destroy_bind)
	{
		int m[] = {     pTemplate->npc_data->service_destroy_bind_prop.money_need,
			pTemplate->npc_data->service_destroy_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(36);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if(pTemplate->npc_data->service_undestroy_bind)
	{
		int m[] = {     pTemplate->npc_data->service_undestroy_bind_prop.money_need,
			pTemplate->npc_data->service_undestroy_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(37);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if(pTemplate->npc_data->service_reset_prop_count)
	{               
		size_t size = pTemplate->npc_data->service_reset_prop_count;
		service_provider * provider = service_manager::CreateProviderInstance(33);
		pImp->AddProvider(provider,pTemplate->npc_data->reset_prop,size * sizeof(npc_template::npc_statement::__reset_prop));
	}  

	if(pTemplate->npc_data->service_talisman)
	{               
		service_provider * provider;
		provider = service_manager::CreateProviderInstance(40);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(41);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(42);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(43);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(44);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->battle_field_challenge_service)
	{
		service_provider * provider = service_manager::CreateProviderInstance(45);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data->battle_field_construct_service)
	{
		service_provider * provider = service_manager::CreateProviderInstance(46);
		pImp->AddProvider(provider,NULL,0);
		//pImp->AddProvider(provider,pTemplate->npc_data->npc_tower_build,sizeof(pTemplate->npc_data->npc_tower_build));
	}

	if(pTemplate->npc_data->battle_field_employ_service)
	{
		service_provider * provider = service_manager::CreateProviderInstance(47);
		pImp->AddProvider(provider,&pTemplate->npc_data->employ_service_data,sizeof(pTemplate->npc_data->employ_service_data));
	}

	if(pTemplate->npc_data->pet_service_adopt)
	{
		service_provider* provider = service_manager::CreateProviderInstance(48);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->pet_service_free)
	{
		service_provider* provider = service_manager::CreateProviderInstance(49);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->pet_service_combine)
	{
		service_provider* provider = service_manager::CreateProviderInstance(50);
		pImp->AddProvider(provider,NULL,0);
		
		provider = service_manager::CreateProviderInstance(88);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->pet_service_rename)
	{
		service_provider* provider = service_manager::CreateProviderInstance(51);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->renew_mount_service)
	{
		service_provider* provider = service_manager::CreateProviderInstance(52);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_item_trade[0] > 0)
	{
		service_provider * provider = service_manager::CreateProviderInstance(53);
		pImp->AddProvider(provider,&pTemplate->npc_data->service_item_trade, sizeof(pTemplate->npc_data->service_item_trade));
	}

	if(pTemplate->npc_data->lock_item_service)
	{
		service_provider * provider = service_manager::CreateProviderInstance(54);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(55);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_restore_broken)
	{
		service_provider * provider = service_manager::CreateProviderInstance(56);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_blood_enchant)
	{
		service_provider * provider = service_manager::CreateProviderInstance(57);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_spirit_addon)
	{
		service_provider * provider = service_manager::CreateProviderInstance(58);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_spirit_remove)
	{
		service_provider * provider = service_manager::CreateProviderInstance(59);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_spirit_charge)
	{
		service_provider * provider = service_manager::CreateProviderInstance(60);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_spirit_decompose)
	{
		service_provider * provider = service_manager::CreateProviderInstance(61);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_npc_produce_id)
	{
		service_provider * provider = service_manager::CreateProviderInstance(62);
		pImp->AddProvider(provider,&pTemplate->npc_data->service_npc_produce_id, sizeof(int));
	}
	if(pTemplate->npc_data->service_petequip_refine)
	{
		service_provider * provider = service_manager::CreateProviderInstance(63);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_change_style)
	{
		service_provider * provider = service_manager::CreateProviderInstance(64);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_magic_refine)
	{
		service_provider * provider = service_manager::CreateProviderInstance(65);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_magic_restore)
	{
		service_provider * provider = service_manager::CreateProviderInstance(66);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_arena_challenge)
	{
		service_provider * provider = service_manager::CreateProviderInstance(67);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_territory_challenge)
	{
		service_provider * provider = service_manager::CreateProviderInstance(68);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_territory_enter)
	{
		service_provider * provider = service_manager::CreateProviderInstance(69);
		int id = pTemplate->npc_data->id_territory;
		pImp->AddProvider(provider,&id, sizeof(id));
	}
	if(pTemplate->npc_data->service_territory_reward)
	{
		service_provider * provider = service_manager::CreateProviderInstance(70);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_charge_telestation)
	{
		service_provider * provider = service_manager::CreateProviderInstance(71);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_repair_damage)
	{
		int m[] = {    500000000, 0 }; 
		service_provider * provider = service_manager::CreateProviderInstance(72);
		pImp->AddProvider(provider,m ,sizeof(m));
	}
	if(pTemplate->npc_data->service_equipment_upgrade)
	{
		service_provider * provider = service_manager::CreateProviderInstance(73);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_consign)
	{
		service_provider * provider = service_manager::CreateProviderInstance(74);
		int margin = pTemplate->npc_data->consign_margin;
		pImp->AddProvider(provider,&margin,sizeof(margin));
	}
	if(pTemplate->npc_data->service_crossservice_in)
	{
		service_provider * provider = service_manager::CreateProviderInstance(75);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_crossservice_out)
	{
		service_provider * provider = service_manager::CreateProviderInstance(76);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data->service_identify_gem_slots)
	{
		service_provider * provider = service_manager::CreateProviderInstance(77);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_rebuild_gem_slots)
	{
		service_provider * provider = service_manager::CreateProviderInstance(78);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_customize_gem_slots)
	{
		service_provider * provider = service_manager::CreateProviderInstance(79);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_embed_gems)
	{
		service_provider * provider = service_manager::CreateProviderInstance(80);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_remove_gems)
	{
		service_provider * provider = service_manager::CreateProviderInstance(81);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_upgrade_gem_level)
	{
		service_provider * provider = service_manager::CreateProviderInstance(82);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_upgrade_gem_quality)
	{
		service_provider * provider = service_manager::CreateProviderInstance(83);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_extract_gem)
	{
		service_provider * provider = service_manager::CreateProviderInstance(84);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_smelt_gem)
	{
		service_provider * provider = service_manager::CreateProviderInstance(85);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->id_transcription)
	{
		service_provider * provider1 = service_manager::CreateProviderInstance(86);
		pImp->AddProvider(provider1,&pTemplate->npc_data->id_transcription,sizeof(pTemplate->npc_data->id_transcription));

		service_provider * provider2 = service_manager::CreateProviderInstance(87);
		pImp->AddProvider(provider2,&pTemplate->npc_data->id_transcription,sizeof(pTemplate->npc_data->id_transcription));
	}
	if(pTemplate->npc_data->service_change_name)
	{
		service_provider * provider = service_manager::CreateProviderInstance(89);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_change_faction_name)
	{
		service_provider* provider = service_manager::CreateProviderInstance(90);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data->service_talisman_holylevelup)
	{
		service_provider* provider = service_manager::CreateProviderInstance(91);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_talisman_embedskill)
	{
		service_provider* provider = service_manager::CreateProviderInstance(92);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_talisman_refineskill)
	{
		service_provider* provider = service_manager::CreateProviderInstance(93);
		pImp->AddProvider(provider,NULL,0);

		service_provider* provider2 = service_manager::CreateProviderInstance(94);
		pImp->AddProvider(provider2,NULL,0);
	}
	
	//声望商店
	if(pTemplate->npc_data->reputation_shop_id)
	{
		service_provider * provider = service_manager::CreateProviderInstance(95);
		pImp->AddProvider(provider,&pTemplate->npc_data->reputation_shop_id,sizeof(int));
	}
	if(pTemplate->npc_data->service_equipment_upgrade2)
	{
		service_provider * provider = service_manager::CreateProviderInstance(96);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_equipment_slot)
	{
		service_provider * provider = service_manager::CreateProviderInstance(97);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_install_astrology)
	{
		service_provider * provider = service_manager::CreateProviderInstance(98);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_uninstall_astrology)
	{
		service_provider * provider = service_manager::CreateProviderInstance(99);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_astrology_identify)
	{
		service_provider * provider = service_manager::CreateProviderInstance(100);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->id_open_ui_trans) // 上古世界传送，激活传送点服务
	{
		service_provider * provider = service_manager::CreateProviderInstance(103);
		pImp->AddProvider(provider, &(pTemplate->npc_data->id_open_ui_trans), sizeof(int));
	}
	if(pTemplate->npc_data->id_use_ui_trans) // 上古世界传送，使用传送点服务
	{
		service_provider * provider = service_manager::CreateProviderInstance(104);
		pImp->AddProvider(provider, 0, 0);
	}
	if(pTemplate->npc_data->service_crossservice_battle_sign_up)
	{
		service_provider * provider = service_manager::CreateProviderInstance(105);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_crossservice_battle_out)
	{
		service_provider * provider = service_manager::CreateProviderInstance(106);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_kingdom_enter)
	{
		service_provider * provider = service_manager::CreateProviderInstance(107);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data->service_produce_jinfashen)
	{
		service_provider * provider = service_manager::CreateProviderInstance(108);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data->service_pet_reborn)
	{
		service_provider * provider = service_manager::CreateProviderInstance(109);
		pImp->AddProvider(provider,NULL,0);
	}
}

void 
server_spawner::OnHeartbeat(world * pPlane)
{
	//先什么也不做
	mobs_spawner::OnHeartbeat(pPlane);
	return;
}

bool 
server_spawner::CreateMobs(world * pPlane,bool auto_detach)
{
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		size_t count = _entry_list[i].mobs_count;
		for(size_t j = 0; j < count; j ++)
		{
			gnpc *pNPC = CreateNPC(pPlane,i,_entry_list[i], auto_detach);
			if(pNPC == NULL) return false;
			if(!auto_detach)
			{
				_xid_list[pNPC->ID] = 1;
				_mobs_cur_gen_num ++;
			}
		}
	}
	return true;
}

bool
server_spawner::Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp,bool is_reset)
{
	//要处理之
	//pImp->_commander->Release();
	__PRINTF("特殊NPC死亡了\n");
	return mobs_spawner::Reclaim(pPlane,pNPC,pImp, is_reset);
}

void 
mine_spawner::Reborn(world * pPlane,gmatter * header, gmatter * tail,int mid,int index)
{
	do
	{
		gmatter * pMatter = header;
		header = (gmatter*)header->pNext;
		ASSERT(pMatter->pPiece == NULL);
		ASSERT(pMatter->matter_type == mid);
		char dir,dir1,rad;
		int collision_id;
		GeneratePos(pMatter->pos,dir,_offset_terrain,pPlane->GetWorldManager(),&collision_id, &dir1,&rad);	//这里不改方向即可
		pMatter->spawn_index = index;

		pMatter->Lock();

		pMatter->collision_id = collision_id;
		pMatter->dir = dir;
		pMatter->dir1 = dir1;
		pMatter->rad = rad;

		if(_is_spawned)
		{
			pMatter->SetActive();
			pMatter->pPrev = pMatter->pNext = NULL;

			mine_template * pTemplate = mine_stubs_manager::Get(mid);
			((gmatter_mine_imp*)pMatter->imp)->InitTemplate( pTemplate );
			//GenerateMineParam((gmatter_mine_imp*)pMatter->imp, pTemplate);

			pPlane->InsertMatter(pMatter);
			pMatter->imp->Reborn();
			ASSERT(((gmatter_mine_imp*)(pMatter->imp))->_spawner == this);
		}
		else
		{
			//如果本spawner已经处于非生成状态，那么直接释放矿物而不是重生
			pMatter->imp->_commander->Release();
		}
		pMatter->Unlock();
		__PRINTF("矿藏%d 重新产生了\n", pMatter->ID.id & 0x7FFFFFF);

	} while(header != tail);
}

void 
mine_spawner::Release()
{
	spin_autolock  keeper(_spool_lock);
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gmatter * header = _mine_pool[i];
		if(!header) continue;

		gmatter * pTmp = (gmatter *)header;
		do
		{
			gmatter * pNext = (gmatter*)pTmp->pNext;
			spin_autolock keeper(pTmp->spinlock);
			pTmp->imp->_commander->Release();
			pTmp = pNext;
		}
		while(pTmp!= header);
		_mine_pool[i] = NULL;
	}
	_entry_list.clear();
	_mine_pool.clear();
}

void 
mine_spawner::OnHeartbeat(world * pPlane)
{
	int tick = g_timer.get_tick();
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gmatter * header = _mine_pool[i];
		if(!header) continue;

		if(header->spawn_index - tick > 0) continue;
		gmatter * pTmp =(gmatter*) header->pNext;
		while(pTmp != header && header->spawn_index - tick <= 0)
		{
			pTmp = (gmatter*)pTmp->pNext;
		}
		if(pTmp == header)
		{
			_mine_pool[i] = NULL;
		}
		else
		{
			_mine_pool[i] = pTmp;
			header->pPrev->pNext = pTmp;
			pTmp ->pPrev = header->pPrev;
		}

		//开始重生
		Reborn(pPlane,header,pTmp,_entry_list[i].mid,i);
	}
}

bool
mine_spawner::Reclaim(world * pPlane,gmatter * pMatter, gmatter_mine_imp * imp)
{
	if(!_auto_respawn || !_is_spawned)
	{
		imp->_commander->Release();
		return false;
	}

	slice *pPiece = pMatter->pPiece;
	if(pPiece)
	{
		pPlane->RemoveMatter(pMatter);
	}       
	else
	{
		//没有piece，也肯定在管理器中
		pPlane->RemoveMatterFromMan(pMatter);
	}
	pMatter->ClrActive();

	size_t index = pMatter->spawn_index;
	ASSERT(index < _entry_list.size());
	if(index >= _entry_list.size())
	{
		//做日志，致命错误 
		GLog::log(GLOG_ERR,"回收矿物时发现错误的生成器索引");
		ASSERT(false);
		return false;
	}

	spin_autolock  keeper(_spool_lock); //$$LOCK$$
	if(!_is_spawned || (_mine_total_gen_num && _mine_cur_gen_num >= _mine_total_gen_num))
	{
		_xid_list.erase(pMatter->ID);
		imp->_commander->Release();
		return false;
	}
	_mine_cur_gen_num ++;
	gmatter * pTmp = _mine_pool[index];
	pMatter->spawn_index = g_timer.get_tick() + _entry_list[index].reborn_time;
	if(pTmp)
	{
		pMatter->pNext= pTmp;
		pMatter->pPrev= pTmp->pPrev;
		pTmp->pPrev->pNext = pMatter;
		pTmp->pPrev = pMatter;
	}
	else
	{
		_mine_pool[index] = pMatter;
		pMatter->pPrev = pMatter->pNext = pMatter;
	}
	__PRINTF("回收了矿物%d\n",pMatter->ID.id);
	return true;
}

void 
mine_spawner::GenerateMineParam(gmatter_mine_imp * imp, mine_template * pt)
{
	mine_template & mine = *pt;
	int index = abase::RandSelect(mine.id_produce_prop,mine.produce_kinds);
	int id = mine.id_produce[index];
	int amount = abase::Rand(0.f,1.f) < mine.bonus_prop?mine.bonus_amount:mine.std_amount;
	imp->SetParam(id,amount,abase::Rand(mine.time_min, mine.time_max) ,mine.need_equipment,mine.level,mine.exp);
	imp->SetCtrlInfo( mine.ctrl_info.actived_all, mine.ctrl_info.active_count, mine.ctrl_info.active_cond, mine.ctrl_info.deactived_all, mine.ctrl_info.deactive_count, mine.ctrl_info.deactive_cond);

	imp->SetTaskParam(mine.task_in, mine.task_out, 
			mine.no_interrupted,mine.gather_no_disappear,mine.eliminate_tool,0,
			mine.ask_help_faction,mine.ask_help_range,mine.ask_help_aggro);
	imp->SetMonsterParam(mine.monster_list, 4);		
}

gmatter *
mine_spawner::CreateMine(mine_spawner* __this,const A3DVECTOR & pos, world * pPlane,int index,const entry_t & et,char dir,char dir1,char rad, int lifetime)
{
	mine_template * pTemplate = mine_stubs_manager::Get(et.mid);
	if(pTemplate == NULL )
	{
		GLog::log(GLOG_ERR,"Invalid mine template id %d",et.mid);
		return NULL;
	}
	if(pTemplate->std_amount == 0 && pTemplate->task_out == 0 && !pTemplate->IsCombineMine() )
	{
		printf("矿物数据有错\n");
		GLog::log(GLOG_ERR,"Invalid mine template id %d",et.mid);
		return NULL;
	}

	//分配NPC设置npc全局数据
	gmatter * pMatter = pPlane->AllocMatter();
	if(!pMatter) return NULL;
	pMatter->ID.type = GM_TYPE_MATTER;
	pMatter->pos = pos;
	pMatter->ID.id= MERGE_ID<gmatter>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetMatterIndex(pMatter)));

	gmatter_mine_imp *imp;
	//普通矿物
	imp = ( ( pTemplate->IsCombineMine() ) ? new gmatter_combine_mine_imp() : new gmatter_mine_imp() );

	imp->Init(pPlane,pMatter);
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_controller();
	imp->_commander->Init(imp);
	//矿物先不管帮派基地里的reset内容
	pMatter->imp = imp;
	pMatter->matter_type = et.mid;
	if(__this)
	{
		pMatter->SetDirUp(dir,dir1,rad);
	}
	else
	{
		pMatter->SetDirUp(0,0,abase::Rand(0,255));
	}

	//初始化必要的参数
	//GenerateMineParam(imp, pTemplate);
	imp->InitTemplate( pTemplate );

	pMatter->spawn_index = index;

	if(__this)
	{
		pMatter->phase_id = __this->_phase_id; 
		if(pMatter->phase_id > 0)
		{
			pMatter->matter_state |= gmatter::STATE_MASK_PHASE;
		}
	}

	imp->_spawner = __this;
	imp->SetLife(lifetime);

//	pPlane->InsertMatter(pMatter);
//	imp->_runner->enter_world();
	return pMatter;
}

bool 
mine_spawner::CreateMines(world *pPlane) 
{
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		size_t count = _entry_list[i].mine_count;
		for(size_t j = 0; j < count; j ++)
		{
			A3DVECTOR pos;
			char dir0,dir1,rad;
			int collision_id;
			GeneratePos(pos,dir0,_offset_terrain,pPlane->GetWorldManager(),&collision_id, &dir1,&rad);
			gmatter * pMatter;
			if((pMatter = CreateMine(this,pos,pPlane,i,_entry_list[i],dir0,dir1,rad)))
			{
				pPlane->InsertMatter(pMatter);
				//pMatter->imp->_runner->enter_world();
				((gmatter_mine_imp*)pMatter->imp)->MineEnterWorld();
				pMatter->collision_id = collision_id;
				_xid_list[pMatter->ID] = 1;
				_mine_cur_gen_num ++;
				pMatter->Unlock();
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void 
mine_spawner::ClearObjects(world * pPlane)
{
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gmatter * pHeader = _mine_pool[i];
		if(pHeader == NULL) continue;

		//将所有在等待池里的ID删掉，并且将整个对象也删掉
		gmatter * tmp = pHeader;
		do
		{
			_xid_list.erase(tmp->ID);
			gmatter * pNext = (gmatter*)tmp->pNext;
			tmp->Lock();
			tmp->imp->_commander->Release();
			tmp->Unlock();
			tmp = pNext;
		}while(tmp != pHeader);
		_mine_pool[i] = NULL;
	}

	//给所有尚未删除的对象发送消息 让其消失
	if(_xid_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_SPAWN_DISAPPEAR,XID(-1,-1),XID(-1,-1),A3DVECTOR(0,0,0),0);
		abase::vector<XID> list;
		abase::static_multimap<XID,int, abase::fast_alloc<> >::iterator it = _xid_list.begin();
		list.reserve(_xid_list.size());
		for(;it != _xid_list.end(); ++it)
		{
			list.push_back(it->first);
		}
		gmatrix::SendMessage(list.begin(),list.end(), msg);
		_xid_list.clear();
	}
}

gmatter * 
dyn_object_spawner::CreateDynObject(mine_spawner* __this,const A3DVECTOR & pos,size_t index, world * pPlane,const entry_t & ent, char dir, char dir1, char rad)
{
	//分配NPC设置npc全局数据
	gmatter * pMatter = pPlane->AllocMatter();
	if(!pMatter) return NULL;
	pMatter->ID.type = GM_TYPE_MATTER;
	pMatter->pos = pos;
	pMatter->ID.id= MERGE_ID<gmatter>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetMatterIndex(pMatter)));
	pMatter->matter_type = ent.mid;

	gmatter_dyn_imp *imp = new gmatter_dyn_imp();
	imp->Init(pPlane,pMatter);
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_controller();
	imp->_commander->Init(imp);

	pMatter->imp = imp;
	if(__this)
	{
		pMatter->SetDirUp(dir,dir1, rad);
	}
	else
	{
		pMatter->SetDirUp(0,0,abase::Rand(0,255));
	}

	pMatter->spawn_index = index;
	pMatter->phase_id = __this->GetPhaseID(); 
	if(pMatter->phase_id > 0)
	{
		pMatter->matter_state |= gmatter::STATE_MASK_PHASE;
	}
	imp->_spawner = __this;

	pPlane->InsertMatter(pMatter);
	imp->_runner->enter_world();
	return pMatter;
	
}

bool 
dyn_object_spawner::CreateMines(world *pPlane) 
{
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		gmatter * pMatter;
		A3DVECTOR pos;
		char dir,dir1,rad;
		int collision_id = 0;
		GeneratePos(pos,dir,0,pPlane->GetWorldManager(),&collision_id, &dir1,&rad);
		if((pMatter = CreateDynObject(this,pos,i, pPlane,_entry_list[i],dir,dir1,rad)))
		{
			pMatter->collision_id = collision_id;
			_xid_list[pMatter->ID] = 1;
			_mine_cur_gen_num ++;
			pMatter->Unlock();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void 
group_spawner::OnHeartbeat(world * pPlane)
{
	if(_next_time <= 0) return; 
	mutex_spinlock(&_lock);
	if((_next_time-=20) > 0) 
	{
		mutex_spinunlock(&_lock);
		return;
	}
	mutex_spinunlock(&_lock);

	gnpc * pLeader;
	{
		//创建队长
		ASSERT(_npc_pool[0]);
		pLeader = _npc_pool[0];
		if(!pLeader) 
		{
			//考虑是不是报告一个错误
			ASSERT(false);
			return;
		}
		ASSERT(pLeader->pNext == pLeader);
		_npc_pool[0] = NULL;

		//重生队长
		_gen_pos_mode = false;
		Reborn(pPlane,pLeader,pLeader,_entry_list[0].offset_terrain,_entry_list[0].npc_tid);
		_gen_pos_mode = true;
		_leader_pos = pLeader->pos;
	}

	for(size_t i =1;i < _entry_list.size();i ++)
	{
		gnpc* header = _npc_pool[i];
		if(!header) continue;
		_npc_pool[i] = NULL;

		//开始重生
		Reborn(pPlane,header,header,_entry_list[i].offset_terrain,_entry_list[i].npc_tid);
	}
}

void 
group_spawner::GeneratePos(A3DVECTOR &pos,char & dir, float offset_terrain, world_manager* manager, int * collision_id, char * dir1, char * rad)
{
	if(_gen_pos_mode)
	{
		do
		{
			extern float __sctab[256][2];
			pos = _leader_pos;
			//加入随机的位置分布
			int index = abase::Rand(0,65536) & 0xFF;
			pos.x += __sctab[index][0]*7.0f;
			pos.z += __sctab[index][1]*7.0f;

			//重新决定高度值
			pos.y = _pos_generator[0]->GenerateY(pos.x,pos.y,pos.z,offset_terrain, manager);

			//加入可到达判定
		}while(0);
		if(collision_id) *collision_id = 0;

	}
	else
	{
		//标准方式
		base_spawner::GeneratePos(pos,dir,offset_terrain, manager,collision_id, dir1,rad);
	}
}


gnpc*
group_spawner::CreateMasterMob(world * pPlane,int index, const entry_t &et, bool auto_detach)
{
	A3DVECTOR pos;
	char dir;
	base_spawner::GeneratePos(pos,dir,et.offset_terrain,pPlane->GetWorldManager(),NULL);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC=CreateMobBase(NULL,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_MASTER,0,NULL,_mob_life);
	}
	else
	{
		_leader_pos = pos;
		pNPC=CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_MASTER,0,NULL,_mob_life);
	}
	if(pNPC)
	{
		pPlane->InsertNPC(pNPC);
		pNPC->imp->_runner->enter_world();
		pNPC->imp->OnCreate();
		pNPC->Unlock();
	}
	return pNPC;
}

gnpc *
group_spawner::CreateMinorMob(world * pPlane,int index, int leader_id,const A3DVECTOR & leaderpos, const entry_t &et, bool auto_detach)
{
	A3DVECTOR pos;
//	GenerateMinorPos(pos,leaderpos,et.offset_terrain);
	char dir;
	GeneratePos(pos,dir,et.offset_terrain, pPlane->GetWorldManager(),NULL );
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC=CreateMobBase(NULL,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_MINOR,0,NULL,_mob_life);
	}
	else
	{
		pNPC=CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_MINOR,0,NULL,_mob_life);
	}
	if(pNPC)
	{
		((gnpc_imp*)pNPC->imp)->_leader_id = XID(GM_TYPE_NPC,leader_id);
		pPlane->InsertNPC(pNPC);
		pNPC->imp->_runner->enter_world();
		pNPC->imp->OnCreate();
		pNPC->Unlock();
	}
	return pNPC;
}

bool
group_spawner::CreateMobs(world * pPlane,bool auto_detach)
{
	if(auto_detach)
	{
		int leader_id;
		A3DVECTOR pos;
		for(size_t i =0;i < _entry_list.size();i ++)
		{
			size_t count = _entry_list[i].mobs_count;
			if(i == 0) 
			{
				if(count > 1) count = 1;
				gnpc *pNPC = CreateMasterMob(pPlane,i,_entry_list[0],auto_detach);
				if(pNPC == NULL) return false;
				leader_id = pNPC->ID.id;
				pos = pNPC->pos;
			}
			else
			{
				for(size_t j = 0; j < count; j ++)
				{
					CreateMinorMob(pPlane,i,leader_id,pos,_entry_list[i], auto_detach);
				}
			}
		}
		return true;

	}
	
	if(_leader_id) return true;	//跳过已经生成的群怪
	_next_time = g_timer.get_tick() + _reborn_time;
	//开始生成怪物
	gnpc * pLeader = NULL;
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		size_t count = _entry_list[i].mobs_count;
		if(i == 0) 
		{
			if(count > 1)
			{
				printf("发现不正确的群怪，队长数目过多，自动被清除为1\n");
				count = 1;
			}
			_gen_pos_mode = false;
			gnpc *pNPC = CreateMasterMob(pPlane,i,_entry_list[0]);
			if(pNPC == NULL) return false;
			_leader_id = pNPC->ID.id;
			pLeader = pNPC;
			_xid_list[pNPC->ID] = 1;
			_mobs_cur_gen_num ++;
		}
		else
		{
			_gen_pos_mode = true;
			for(size_t j = 0; j < count; j ++)
			{
				gnpc * pNPC = CreateMinorMob(pPlane,i,_leader_id,pLeader->pos,_entry_list[i]);
				if(pNPC)
				{
					_xid_list[pNPC->ID] = 1;
					_mobs_cur_gen_num ++;
				}
			}
		}
	}
	_next_time = -1;
	return true;
}

bool
group_spawner::Reclaim(world * pPlane, gnpc * pNPC, gnpc_imp * imp, bool is_reset) 
{
	int id = pNPC->ID.id;
	if(mobs_spawner::Reclaim(pPlane,pNPC,imp,is_reset))
	{
		if(id == _leader_id)
		{
			//是_leader_id
			mutex_spinlock(&_lock);
			_next_time  = _reborn_time;
			mutex_spinunlock(&_lock);
		}
		return true;
	}
	else
	{
		//由于不会重生，所以考虑在适当的时候清除leader_id
		if(id == _leader_id) _leader_id = 0;
		return false;
	}
}

gnpc*
boss_spawner::CreateMasterMob(world * pPlane,int index, const entry_t &et, bool auto_detach)
{
	A3DVECTOR pos;
	char dir;
	base_spawner::GeneratePos(pos,dir,et.offset_terrain,pPlane->GetWorldManager(), NULL);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC=CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_BOSS,0,NULL,_mob_life);
	}
	else
	{
		_leader_pos = pos;
		pNPC=CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_BOSS,0,NULL,_mob_life);
	}
	if(pNPC)
	{
		pPlane->InsertNPC(pNPC);
		pNPC->imp->_runner->enter_world();
		pNPC->imp->OnCreate();
		pNPC->Unlock();
	}
	return pNPC;
}

gnpc *
boss_spawner::CreateMinorMob(world * pPlane,int index, int leader_id,const A3DVECTOR & leaderpos, const entry_t &et, bool auto_detach)
{
	A3DVECTOR pos;
	char dir;
	GeneratePos(pos,dir,et.offset_terrain,pPlane->GetWorldManager(),NULL);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC = CreateMobBase(NULL,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_MINOR,AGGRO_POLICY_BOSS_MINOR,NULL,_mob_life);
	}
	else
	{
		pNPC = CreateMobBase(this,pPlane,et, index, _phase_id, pos,cid,dir,CLS_NPC_AI_POLICY_MINOR,AGGRO_POLICY_BOSS_MINOR,NULL,_mob_life);
	}
	if(pNPC)
	{
		((gnpc_imp*)pNPC->imp)->_leader_id = XID(GM_TYPE_NPC,leader_id);
		pPlane->InsertNPC(pNPC);
		pNPC->imp->_runner->enter_world();
		pNPC->imp->OnCreate();
		pNPC->Unlock();
	}
	return pNPC;
}

bool
boss_spawner::CreateMobs(world * pPlane,bool auto_detach)
{
	if(auto_detach)
	{
		int leader_id = 0;
		A3DVECTOR pos;
		for(size_t i =0;i < _entry_list.size();i ++)
		{
			size_t count = _entry_list[i].mobs_count;
			if(i == 0) 
			{
				if(count > 1) count = 1;
				gnpc *pNPC = CreateMasterMob(pPlane,i,_entry_list[0],auto_detach);
				if(pNPC == NULL) return false;
				leader_id = pNPC->ID.id;
				pos = pNPC->pos;
			}
			else
			{
				for(size_t j = 0; j < count; j ++)
				{
					CreateMinorMob(pPlane,i,leader_id,pos,_entry_list[i],auto_detach);
				}
			}
		}
		return true;
	}

	if(_leader_id) return true;
	_next_time = g_timer.get_tick() + _reborn_time;
	//开始生成怪物
	gnpc * pLeader = NULL;
	spin_autolock keeper(_mobs_list_lock);
	_mobs_list.reserve(_entry_list.size());
	for(size_t i =0;i < _entry_list.size();i ++)
	{
		size_t count = _entry_list[i].mobs_count;
		if(i == 0) 
		{
			if(count > 1)
			{
				printf("发现不正确的群怪，队长数目过多，自动被清除为1\n");
				count = 1;
			}
			_gen_pos_mode = false;
			gnpc *pNPC = CreateMasterMob(pPlane,i,_entry_list[0]);
			if(pNPC == NULL) return false;
			_leader_id = pNPC->ID.id;
			pLeader = pNPC;
			_mobs_list.push_back(pNPC->ID);
			_xid_list[pNPC->ID] = 1;
			_mobs_cur_gen_num ++;
		}
		else
		{
			_gen_pos_mode = true;
			for(size_t j = 0; j < count; j ++)
			{
				gnpc * pNPC = CreateMinorMob(pPlane,i,_leader_id,pLeader->pos,_entry_list[i]);
				ASSERT(pNPC);
				if(pNPC)
				{
					_mobs_list.push_back(pNPC->ID);
					_xid_list[pNPC->ID] = 1;
					_mobs_cur_gen_num ++;
				}
			}
		}
	}
	_next_time = -1;
	return true;
}

void 
boss_spawner::ForwardFirstAggro(world * pPlane, const XID & id, int rage)
{
	spin_autolock keeper(_mobs_list_lock);
	if(_mobs_list.size() > 1)
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_TRANSFER_AGGRO,XID(-1,-1),_mobs_list[0],A3DVECTOR(0,0,0),rage,&id,sizeof(id));
		gmatrix::SendMessage(_mobs_list.begin() + 1, _mobs_list.end(), msg);

	}
}

void 
spawner_ctrl::Spawn(world * pPlane)
{
	if(IsSpawned()) return;
	for(size_t i = 0; i < _list.size();i ++)
	{
		if(!_list[i]->BeginSpawn(pPlane)) break;
	}
	SetSpawnFlag(true);
	return ;
}

void 
spawner_ctrl::Active(world * pPlane)
{
	if(IsActived()) return ;
	_active_flag = true;
	_cur_active_life = _active_life;

	if(_spawn_after_active <= 0) 
	{
		Spawn(pPlane);
	}
	else
	{	
		_time_before_spawn = _spawn_after_active;
	}
}

void 
spawner_ctrl::SingleActive(world * pPlane)
{
	for(size_t i = 0; i < _list.size();i ++)
	{
		_list[i]->SingleSpawn(pPlane);
	}
}


void 
spawner_ctrl::Stop(world * pPlane)
{
	if(!IsActived()) return ;
	_active_flag = false;
	if(IsSpawned())
	{
		for(size_t i = 0; i < _list.size();i ++)
		{
			_list[i]->EndSpawn(pPlane);
		}
		SetSpawnFlag(false);
	}
}

bool 
spawner_ctrl::BeginSpawn(world * pPlane)
{
	spin_autolock keeper(_lock);

	//可重复激活的情况
	if(_detach_after_spawn)
	{
		//如果有时间段范围，还要先判断是否在时间段范围之内，不在则直接return
		if( _timeseg.HasTimeSeg() )
		{
			time_t t = (time_t)g_timer.get_systime();
			struct tm tt;
			localtime_r(&t, &tt);

			//检查是否在时间段内
			if( !_timeseg.IsInTimeSegment(tt) )
			{
				return true;
			}
		}

		SingleActive(pPlane);
		return true;
	}

	if(IsActived()) 
		return false;

	Active(pPlane);

	return true; 
}


void
spawner_ctrl::OnHeartbeat(world * pPlane)
{
	spin_autolock keeper(_lock);
	time_t t = (time_t)g_timer.get_systime();

	//已经处于激活状态
	if(_active_flag)
	{
		//若未激活
		if(!_spawn_flag)
		{
			//未生成
			_time_before_spawn --;
			if(_time_before_spawn <= 0)
			{
				//生成....
				Spawn(pPlane);
				return ;
			}
		}

		if(_active_life > 0)
		{
			_cur_active_life --;
			if(_cur_active_life <= 0)
			{
				//超出了寿命
				//取消激活
				Stop(pPlane);
				return ;
			}
		}

		if(_has_stop_date)
		{
			_date_counter_down --;
			if(_date_counter_down <= 0)
			{
				struct tm tt;
				localtime_r(&t,&tt);
				//再次检查时间
				int rst = _stop_date.CheckTime(tt);
				if(rst > 0)
				{
					if(rst > 600) rst = 600;
					_date_counter_down = rst;
				}
				else
				{
					//达到停止的时间点
					//停止激活
					Stop(pPlane);
					return ;
				}
			}
		}
		else if( _timeseg.HasTimeSeg() && !_has_active_date ) //时间段和原来的指定时间点不能兼容
		{
			_timeseg_counter_down--;
			if(_timeseg_counter_down <= 0)
			{
				int countdown = abase::Rand(600, 1200);
				struct tm tt;
				localtime_r(&t, &tt);

				if( _timeseg.IsInTimeSegment(tt) )
				{
					int tempcountdown = _timeseg.GetNextTimeToStop(tt);
					if(tempcountdown > 0)
					{
						if(tempcountdown < countdown) 
						{
							countdown = tempcountdown;
						}
					}
					else//这个地方没有Stop是因为可能存在互相重叠的时间段,并集不允许有交，理论上不会走这个分支。
					{
						countdown = 60 + tempcountdown;
					}

					_timeseg_counter_down = countdown;
				}
				else
				{
					Stop(pPlane);
					_timeseg_counter_down = 0;
					return;
				}
			}
		}
	}//end _active_flag
	else //未激活状态
	{
		if(_has_active_date)
		{
			_date_counter_down --;
			if(_date_counter_down <= 0)
			{
				int rst;
				if(_active_date_duration <= 60)
				{
					struct tm tt;
					localtime_r(&t,&tt);
					//再次检查时间
					rst = _active_date.CheckTime(tt);
					if(rst > 0)
					{
						if(rst > 600) rst = 600;
						_date_counter_down = rst;
					}
					else
					{
						//到达开始的时间点
						//开始激活
						Active(pPlane);
					}
					return ;
				}

				//使用第二种起始激活方法
				rst = _active_date.CheckTime2(t,_active_date_duration);
				if(rst >= 0 && rst <= _active_date_duration)
				{
					Active(pPlane);
					return ;
				}
				else
				{
					if(rst > _active_date_duration)
					{
						_date_counter_down = rst - _active_date_duration;
					}
				}
			}
		}
		else if( _timeseg.HasTimeSeg() && !_has_stop_date) //时间段和原来的指定时间点不能兼容
		{
			_timeseg_counter_down--;
			if(_timeseg_counter_down <= 0)
			{
//				int countdown = abase::Rand(600, 1200);
				int countdown = 30; 
				struct tm tt;
				localtime_r(&t, &tt);

				if( _timeseg.IsInTimeSegment(tt) )
				{
					Active(pPlane);
					_timeseg_counter_down = 0;
					return ;
				}
				else
				{
					int tempcountdown = _timeseg.GetNextTimeToStart(tt);
					if(tempcountdown > 0)   
					{
						if(tempcountdown < countdown)
						{
							countdown = tempcountdown;
						}
					}
					else
					{
						//到达时间点，开始激活
						Active(pPlane);
						_timeseg_counter_down = 0;
						return ;
					}

					//在多个时间段里找到最近的countdown重新赋值
					_timeseg_counter_down = countdown;
				}
			}
		}
	}
}

void npc_generator::CheckCtrlID( int id )
{
	if( id >= CTRL_CONDISION_ID_BATTLE_MIN && id <= CTRL_CONDISION_ID_BATTLE_MAX )
	{
		__PRINTF( "不要在非战场服务器中使用战场控制器区间的id=%d!!!!!!!!!!!!\n", id );
	}
}

void
mobactive_spawner::ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR & pos, char dir, int index)
{
	XID oldID = pNPC->ID;
	pNPC->Clear();
	pNPC->SetActive();
	pNPC->ID = oldID;
	CreateMobActive(pPlane,index,_entry_list[index],pos,pNPC,false,dir);

}

gnpc *
mobactive_spawner::CreateMobActive(world * pPlane, int index, const entry_t & et, bool auto_detach)
{
	A3DVECTOR pos;
	char dir;
	int collision_id;
	GeneratePos(pos,dir,et.offset_terrain,pPlane->GetWorldManager(),&collision_id);
	gnpc * pNPC = CreateMobActive(pPlane,index,et,pos,NULL,auto_detach,dir);
	if (pNPC) pNPC->collision_id = collision_id;
	return pNPC;
}

gnpc * 
mobactive_spawner::CreateMobActive(world * pPlane, int index,const entry_t & et, const A3DVECTOR & pos, gnpc * origin_npc, bool auto_detach, char dir)
{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);
	ASSERT(pTemplate);
	if(!pTemplate) return NULL; 
	if(!pTemplate->mob_active_data) return NULL;

	const int cid[3] = {CLS_MOBACTIVE_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int ai_policy_cid = CLS_NPC_AI_POLICY_BASE;
	gnpc * pNPC;
	if(auto_detach)
	{
		pNPC = CreateMobActiveBase(NULL,pPlane,et, index,pos,cid,dir,ai_policy_cid,0,origin_npc,_mob_life);
	}
	else
	{
		pNPC = CreateMobActiveBase(this,pPlane,et, index,pos,cid,dir,ai_policy_cid,0,origin_npc,_mob_life);
	}
	if (!pNPC) return NULL;

	pPlane->InsertNPC(pNPC);
	pNPC->imp->_runner->enter_world();
	pNPC->imp->OnCreate();
	if(!origin_npc)
	{
		//只有当外部没有NPC的时候才这么做
		//因为如果有外部的，那么应该已经加锁了
		pNPC->Unlock();
	}
	return pNPC;
}

bool
mobactive_spawner::CreateMobs(world *pPlane,bool auto_detach)
{
	for (size_t i = 0; i < _entry_list.size(); i++)
	{
		size_t count = _entry_list[i].mobs_count;
		for (size_t j = 0; j < count; j++)
		{
			gnpc *pNPC = CreateMobActive(pPlane,i,_entry_list[i],auto_detach);
			if (pNPC == NULL) return false;
			if (!auto_detach)
			{
				_xid_list[pNPC->ID] = 1;
				_mobs_cur_gen_num++;
			}
		}
	}
	return true;
}

bool
mobactive_spawner::Reclaim(world * pPlane, gnpc * pNPC,gnpc_imp * pImp, bool is_reset)
{
	return mobs_spawner::Reclaim(pPlane,pNPC,pImp,is_reset);
}

void
mobactive_spawner::OnHeartbeat(world * pPlane)
{
	mobs_spawner::OnHeartbeat(pPlane);
	return;
}

void
mobactive_spawner::GenerateMobActiveParam(mob_active_imp * imp, npc_template * pt)
{
	imp->SetParam(pt->mob_active_data);
}
