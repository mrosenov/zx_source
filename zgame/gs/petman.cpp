#include "petman.h"
#include "world.h"
#include "player_imp.h"
#include "mount_filter.h"
#include "petdataman.h"
#include "pathfinding/pathfinding.h"
#include "petnpc.h"
#include "item/item_petbedge.h"
#include "item/item_petautofood.h"
#include "filter_man.h"
#include "pet_filter.h"
#include "template_loader.h"
#include "cooldowncfg.h"
#include "raid/raid_world_manager.h"

int pet_manager::CheckDBData(const void* buf,size_t size)
{
	if(0 == size)
	{
		//开放宠物后第一次上线的人
		return PCDDE_SUCCESS;
	}
	if(size > sizeof(pet_data_t) || size < sizeof(int))
	{
		//数据错误
		return PCDDE_SIZE_INVALID;
	}
	int version = *(int*)buf;
	if(version > PET_DB_DATA_VERSION)
	{
		//版本记错了?
		return PCDDE_VERSION_INVALID;
	}
	else if(PET_DB_DATA_VERSION == version)
	{
		if(size != sizeof(pet_data_t))
		{
			return PCDDE_VERSION_VALID_BUT_SIZE_INVALID;
		}
	}
	pet_data_t temp;
	memset(&temp,0,sizeof(pet_data_t));
	memcpy(&temp,buf,size);
	if(temp.pet_bedge_inv_size < INIT_PET_BEDGE_LIST_SIZE || temp.pet_bedge_inv_size > MAX_PET_BEDGE_LIST_SIZE)
	{
		//宠物牌栏记错了?
		return PCDDE_BEDGE_INV_SIZE_INVALID;
	}
	return PCDDE_SUCCESS;
}

const static int honor_level_list[] = {0,80,240,480,960,1800,3600,7200,14400,28800,57600};

int pet_manager::GetHonorLevel(int cur_honor_point)
{
	if(cur_honor_point <= 0) return pet_data::HONOR_LEVEL_0;
	for(int i = 0;i < pet_data::HONOR_LEVEL_COUNT;++i)
	{
		if(cur_honor_point <= honor_level_list[i]) return i;
	}
	return pet_data::HONOR_LEVEL_10;
}

const static float hunger_level_list[] = {0.25f,0.50f,0.75f,1.01f};

int pet_manager::GetHungerLevel(int cur_hunger_point,int max_hunger_point)
{
	if(cur_hunger_point <= 0 || max_hunger_point <= 0) return pet_data::HUNGER_LEVEL_0;
	float factor = (float)cur_hunger_point / max_hunger_point;
	for(int i = 0;i < pet_data::HUNGER_LEVEL_COUNT;++i)
	{
		if(factor < hunger_level_list[i]) return i;
	}
	return pet_data::HUNGER_LEVEL_3;
}

const static float hunger_level_damage_adjust[] = {-0.30f,-0.15f,0.0f,0.15f};

float  pet_manager::GetHungerLevelDamageAdjust(int hunger_level)
{
	if(hunger_level < pet_data::HUNGER_LEVEL_0) hunger_level = pet_data::HUNGER_LEVEL_0;
	if(hunger_level > pet_data::HUNGER_LEVEL_3) hunger_level = pet_data::HUNGER_LEVEL_3;
	return hunger_level_damage_adjust[hunger_level];
}

const static struct
{
	size_t civilization_need;
	int max_count;
	int level_need;
	float max_hp_add;
	float attack_add;
	int life_skill_add;
}rank_effect[] =
{
	{0,		0x7FFFFFF,	1,	0.0f,	0.0f,	0},
	{1,		32,		10,	0.01f,	0.01f,	0},
	{18000,		16,		30,	0.03f,	0.03f,	0},
	{144000,	8,		60,	0.05f,	0.05f,	0},
	{288000,	4,		90,	0.07f,	0.07f,	0},
	{1152000,	2,		120,	0.09f,	0.09f,	0},
	{2304000,	1,		150,	0.11f,	0.11f,	0},
	{1,		32,		30,	0,	0,	1},
	{9000,		16,		60,	0,	0,	3},
	{72000,		8,		60,	0,	0,	5},
	{144000,	4,		90,	0,	0,	10},
	{576000,	2,		120,	0,	0,	15},
	{1152000,	1,		150,	0,	0,	20},
};

size_t pet_manager::GetCivilizationNeed(int rank)
{
	if(rank < 0) rank = 0;
	if(rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) rank = pet_bedge_enhanced_essence::MAX_PET_RANK - 1;
	return rank_effect[rank].civilization_need;
}

int pet_manager::GetMaxCount(int rank)
{
	if(rank < 0) rank = 0;
	if(rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) rank = pet_bedge_enhanced_essence::MAX_PET_RANK - 1;
	return rank_effect[rank].max_count;
}

int pet_manager::GetLevelNeed(int rank)
{
	if(rank < 0) rank = 0;
	if(rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) rank = pet_bedge_enhanced_essence::MAX_PET_RANK - 1;
	return rank_effect[rank].level_need;
}

float pet_manager::GetMaxHPAdd(int rank)
{
	if(rank < 0) rank = 0;
	if(rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) rank = pet_bedge_enhanced_essence::MAX_PET_RANK - 1;
	return rank_effect[rank].max_hp_add;
}

float pet_manager::GetAttackAdd(int rank)
{
	if(rank < 0) rank = 0;
	if(rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) rank = pet_bedge_enhanced_essence::MAX_PET_RANK - 1;
	return rank_effect[rank].attack_add;
}

int pet_manager::GetLifeSkillAdd(int rank)
{
	if(rank < 0) rank = 0;
	if(rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) rank = pet_bedge_enhanced_essence::MAX_PET_RANK - 1;
	return rank_effect[rank].life_skill_add;
}

void pet_manager::GenerateBaseProp(const pet_bedge_essence& ess,const pet_bedge_data_temp& pbt,q_extend_prop& base_prop)
{
	base_prop.max_hp = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::MAX_HP);
	base_prop.max_mp = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::MAX_VIGOR);
	//这里出现负数是不应该的，0级别0属性的宠物参数才会出现这种情况
	if(base_prop.max_hp <= 0) base_prop.max_hp = 1;
	if(base_prop.max_mp <= 0) base_prop.max_mp = 1;
	memset(base_prop.hp_gen,0,4 * sizeof(int));
	base_prop.hp_gen[0] = pbt.hp_gen[0];
	base_prop.hp_gen[1] = pbt.hp_gen[1];
	memset(base_prop.mp_gen,0,4 * sizeof(int));
	base_prop.mp_gen[0] = pbt.vp_gen[0];
	base_prop.mp_gen[1] = pbt.vp_gen[1];
	base_prop.crit_rate = (int)(1000 * ess.GetCritRate() + 0.5f);
	base_prop.crit_damage = ess.GetCritDamage();
	base_prop.anti_crit = (int)(1000 * ess.GetAntiCritRate() + 0.5f);
	base_prop.anti_crit_damage = ess.GetAntiCritDamage();
	base_prop.skill_attack_rate = (int)(1000 * ess.GetSkillAttackRate() + 0.5f);
	base_prop.skill_armor_rate = (int)(1000 * ess.GetSkillArmorRate() + 0.5f);
	base_prop.damage_low = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::MIN_ATTACK);
	base_prop.damage_high = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::MAX_ATTACK);
	base_prop.attack = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::HIT);//命中,注意名字问题......
	base_prop.defense = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::DEFENCE);
	base_prop.armor = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::JOUK);
	int i = 0;
	for(i = 0;i < 6;++i)
	{
		base_prop.resistance[i] = ess.GetCombatAttrCur(pet_bedge_enhanced_essence::RESISTANCE1 + i);
	}
	base_prop.walk_speed = pbt.walk_speed;
	base_prop.run_speed = pbt.run_speed;
	base_prop.attack_range = pbt.attack_range;
	base_prop.attack_range += pbt.body_size;
}

void pet_manager::GenerateEnhancedProp(const pet_bedge_essence& ess,const pet_bedge_data_temp& pbt,q_enhanced_param& en_point)
{
	en_point.spec_damage = ess.GetExtraDamage();
	en_point.dmg_reduce = ess.GetExtraDamageReduce();
}

void pet_manager::AddEquipProp(gplayer_imp* pImp,const pet_bedge_essence& ess,q_extend_prop& base_prop,int& equip_mask,itemdataman::_pet_armor_essence* pAdded)
{
	equip_mask = 0;
	size_t pet_equip_count = pImp->GetPetEquipInventory().Size();
	for(size_t i = 0;i < pet_equip_count;++i)
	{
		if(!pImp->GetPetMan().CheckPetEquipExist(pImp,i)) continue;
		item& it_pet_equip = pImp->GetPetEquipInventory()[i];
		const pet_equip_data_temp* pEquipTemp = pet_equip_dataman::Get(it_pet_equip.type);
		if(!pEquipTemp) continue;
		if(pEquipTemp->level_require > ess.GetLevel()) continue;
		if(pEquipTemp->astro_level > ess.GetStar()) continue;
		int mask = 1;
		mask = mask << ess.GetRace();
		if(!(pEquipTemp->pet_type_mask & mask)) continue;
		mask = 1;
		mask = mask << ess.GetHoroscope();
		if(!(pEquipTemp->astro_type & mask)) continue;
		size_t len;
		const void* buf = it_pet_equip.GetContent(len);
		if(sizeof(itemdataman::_pet_armor_essence) != len) continue;
		itemdataman::_pet_armor_essence* pEss = (itemdataman::_pet_armor_essence*)buf;
		base_prop.max_hp += pEss->maxhp;
		base_prop.max_mp += pEss->maxmp;
		base_prop.defense += pEss->defence;
		base_prop.damage_low += pEss->attack;
		base_prop.damage_high += pEss->attack;
		for(int j = 0;j < 6; ++j )
		{
			base_prop.resistance[j] += pEss->resistance[j];
		}
		base_prop.attack += pEss->hit;
		base_prop.armor += pEss->jouk;
		base_prop.crit_rate += int(pEss->crit_rate * 1000 + 0.5f);
		base_prop.crit_damage += pEss->crit_damage;
		if(pAdded)
		{
			pAdded->maxhp += pEss->maxhp;
			pAdded->maxmp += pEss->maxmp;
			pAdded->defence += pEss->defence;
			pAdded->attack += pEss->attack;
			for(int k = 0; k < 6; ++k )
			{
				pAdded->resistance[k] += pEss->resistance[k];
			}
			pAdded->hit += pEss->hit;
			pAdded->jouk += pEss->jouk;
			pAdded->crit_rate += int(pEss->crit_rate * 1000 + 0.5f);
			pAdded->crit_damage += pEss->crit_damage;
		}
		equip_mask += (1 << i);
	}
}

void pet_manager::AddSkillProp(gplayer_imp* pImp,const pet_bedge_essence& ess,q_extend_prop& base_prop)
{
}

pet_manager::pet_manager()
{
	memset(_pet_list,0,sizeof(pet_data) * MAX_PET_BEDGE_LIST_SIZE);
	memset(&_data,0,sizeof(pet_data_t));
	_cur_summon_pet_index = -1;
	_change_flag = false;
}

pet_manager::~pet_manager()
{

}

void pet_manager::Swap(pet_manager & rhs)
{
	for(size_t i = 0;i < MAX_PET_BEDGE_LIST_SIZE;++i)
	{
		abase::swap(_pet_list[i],rhs._pet_list[i]);
	}
	abase::swap(_data,rhs._data);
	abase::swap(_cur_summon_pet_index,rhs._cur_summon_pet_index);
	abase::swap(_change_flag,rhs._change_flag);
}

void pet_manager::DoSummonPet(gplayer_imp* pImp,size_t pet_index,const A3DVECTOR& pos)
{
	if(IsPetActive(pet_index)) return;
	if(pImp->GetParent()->IsInvisible()) pImp->_filters.RemoveFilter(FILTER_INDEX_INVISIBLE);

	if(PetBedgeRebuild(pImp,pet_index))
	{
		RefreshPetInfo(pImp,pet_index);
		pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
	}

	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	//只有战斗宠物
	pData->pet_id = XID(-1,-1);

	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(ess.GetCurHP() <= 0)
	{
		ess.SetCurHP(1);
	}
	if(ess.GetCurHungerPoint() <= 0)
	{
		ess.SetCurHungerPoint(SUMMON_INIT_HUNGER_POINT);
	}
	XID who;
	//宠物summon逻辑接口
	object_interface oi(pImp);

	_cooldown.ClearExpiredCoolDown();
	raw_wrapper cd_data;
        _cooldown.Save(cd_data);	
	bool bRst = oi.CreatePet(pos,pData,&ess,it.type,pet_index,who, ess.GetRebornCnt(), cd_data.data(), cd_data.size());
	if(!bRst)
	{
		pImp->_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	ASSERT(who.IsActive());
	//设置激活标志
	pData->active_state = pet_data::PET_STATE_SUMMON;
	pData->pet_id = who;
	pData->summon_timer = 0;
	pData->summon_timestamp = g_timer.get_systime();
	pImp->_runner->player_pet_ai_state(pet_index,pData->stay_state,pData->aggro_state);
	pImp->_runner->player_summon_pet(pet_index,it.type,pData->pet_id.id);
	ess.UpdateContent(&it);
	RefreshPetInfo(pImp,pet_index);
	//自动设置能设置的最高战斗等级
	int rank = pet_bedge_enhanced_essence::PET_RANK_COMBAT6;
	for(rank = pet_bedge_enhanced_essence::PET_RANK_COMBAT6;rank >= pet_bedge_enhanced_essence::PET_RANK_BASE;--rank)
	{
		if(GetPetCivilization() >= GetCivilizationNeed(rank)) break;
	}
	SetPetRank(pImp,pet_index,it.type,rank);

	if(ess.GetRebornCnt() > 0)
	{
		gplayer* pPlayer = pImp->GetParent();
		int star = ess.GetStar();
		int rebornstar = ess.GetRebornStar();
		if(ess.GetSubStatus() == pet_bedge_enhanced_essence::PET_SUB_STATUS_FIGHT_TONGLING)
		{
			int dmg_reduce = int((0.0004 * star * star + 0.001 * star + 0.02) * 1000 + 0.5f);
			int res_add = int(0.1 * star * star + 5 + 0.5f);
			int anti_crit_rate = int(0.4*rebornstar* rebornstar+0.5* rebornstar+ 5);	
			pImp->_filters.AddFilter(new filter_pet_combine1(pImp,dmg_reduce,res_add, anti_crit_rate,28800));
			pPlayer->SetObjectState(gactive_object::STATE_PET_COMBINE1);

			if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE2))
			{
				pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE2);
				pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE2);
			}
		}
		else if(ess.GetSubStatus() == pet_bedge_enhanced_essence::PET_SUB_STATUS_FIGHT_YUBAO)
		{
			int effect = int((0.001 * star * star + 0.007 * star + 0.06) * 100 + 0.5f);
			int crit_rate = int(0.4*rebornstar* rebornstar+0.5* rebornstar+ 5);	
			pImp->_filters.AddFilter(new filter_pet_combine2(pImp,effect, crit_rate,28800));
			pPlayer->SetObjectState(gactive_object::STATE_PET_COMBINE2);

			if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE1))
			{
				pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE1);
				pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE1);
			}
		}
	}
}

void pet_manager::DoRecallPet(gplayer_imp* pImp,size_t pet_index)
{
	if(!IsPetSummon(pet_index)) return;
	__PRINTF("DoRecallPet\n");
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	pData->active_state = pet_data::PET_STATE_NONE;
	//召回后修改召唤的时间戳
	++pData->summon_stamp;
	pImp->SendTo2<0>(GM_MSG_PET_DISAPPEAR,pData->pet_id,pet_index,pData->summon_stamp);
	pImp->_runner->player_recall_pet(pet_index,it.type,pData->pet_id.id);
	pData->pet_id = XID(-1,-1);
	RefreshPetInfo(pImp,pet_index);
}

void pet_manager::DoCombinePet(gplayer_imp* pImp,size_t pet_index,int type, bool need_cooldown)
{
	if(IsPetActive(pet_index)) return;

	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	pData->active_state = pet_data::PET_STATE_COMBINE;
	pData->combine_counter = 7200;
	gplayer* pPlayer = pImp->GetParent();
	int star = ess.GetStar();
	pData->combine_type = type;
	if(0 == pData->combine_type)
	{
		int dmg_reduce = int((0.0004 * star * star + 0.001 * star + 0.02) * 1000 + 0.5f);
		int res_add = int(0.1 * star * star + 5 + 0.5f);
		pImp->_filters.AddFilter(new filter_pet_combine1(pImp,dmg_reduce,res_add,0,7200));
		pPlayer->SetObjectState(gactive_object::STATE_PET_COMBINE1);
		if(need_cooldown) pImp->SetCoolDown(COOLDOWN_INDEX_PET_COMBINE1,PET_COMBINE1_COOLDOWN_TIME * 1000);

		if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE2))
		{
			pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE2);
			pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE2);
		}
	}
	else
	{
		int effect = int((0.001 * star * star + 0.007 * star + 0.06) * 100 + 0.5f);
		pImp->_filters.AddFilter(new filter_pet_combine2(pImp,effect,0,7200));
		pPlayer->SetObjectState(gactive_object::STATE_PET_COMBINE2);
		if(need_cooldown) pImp->SetCoolDown(COOLDOWN_INDEX_PET_COMBINE2,PET_COMBINE2_COOLDOWN_TIME * 1000);

		if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE1))
		{
			pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE1);
			pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE1);
		}
	}
	pImp->_runner->player_combine_pet(pet_index,it.type,pData->combine_type);
}

void pet_manager::DoRebornCombinePet(gplayer_imp * pImp, size_t pet_index, int type)
{
	gplayer* pPlayer = pImp->GetParent();
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();

	if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE1))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE1);
		pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE1);
	}

	if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE2))
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE2);
		pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE2);
	}

	ess.SetSubStatus(type);
	ess.UpdateContent(&it);

	int star = ess.GetStar();
	int rebornstar = ess.GetRebornStar();
	if(ess.GetSubStatus() == pet_bedge_enhanced_essence::PET_SUB_STATUS_FIGHT_TONGLING)
	{
		int dmg_reduce = int((0.0004 * star * star + 0.001 * star + 0.02) * 1000 + 0.5f);
		int res_add = int(0.1 * star * star + 5 + 0.5f);
		int anti_crit_rate = int(0.4*rebornstar* rebornstar+0.5* rebornstar+ 5);	
		pImp->_filters.AddFilter(new filter_pet_combine1(pImp,dmg_reduce,res_add,anti_crit_rate,28800));
		pPlayer->SetObjectState(gactive_object::STATE_PET_COMBINE1);

		if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE2))
		{
			pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE2);
			pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE2);
		}
	}
	else if(ess.GetSubStatus() == pet_bedge_enhanced_essence::PET_SUB_STATUS_FIGHT_YUBAO)
	{
		int effect = int((0.001 * star * star + 0.007 * star + 0.06) * 100 + 0.5f);
		int crit_rate = int(0.4*rebornstar* rebornstar+0.5* rebornstar+ 5);	
		pImp->_filters.AddFilter(new filter_pet_combine2(pImp,effect,crit_rate,28800));
		pPlayer->SetObjectState(gactive_object::STATE_PET_COMBINE2);

		if(pImp->_filters.IsFilterExist(FILTER_INDEX_PET_COMBINE1))
		{
			pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE1);
			pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE1);
		}
	}
}


void pet_manager::DoUncombinePet(gplayer_imp* pImp,size_t pet_index)
{
	if(!IsPetCombine(pet_index)) return;
	if(pImp->GetParent()->IsInvisible()) pImp->_filters.RemoveFilter(FILTER_INDEX_INVISIBLE);

	__PRINTF("DoUncombinePet\n");
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	pData->active_state = pet_data::PET_STATE_NONE;
	pData->combine_counter = 0;
	gplayer* pPlayer = pImp->GetParent();
	if(0 == pData->combine_type)
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE1);
		pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE1);
	}
	else
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_PET_COMBINE2);
		pPlayer->ClrObjectState(gactive_object::STATE_PET_COMBINE2);
	}
	pImp->_runner->player_uncombine_pet(pet_index,it.type);
}

bool pet_manager::RecvExp(gplayer_imp* pImp,size_t pet_index,int exp_added,bool medicine)
{
	if(exp_added <= 0) return false;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	int cur_exp = ess.GetExp() + exp_added;
	int cur_level = ess.GetLevel();
	if(cur_exp <= 0) return false; //防止经验冒了
	bool lp = false;
	do
	{
		if(cur_level >= MAX_PET_LEVEL) break;
		if(0 == pImp->GetRebornCount())
		{
			if(cur_level >= pImp->_basic.level + 5)
			{
				cur_exp = 0;
				break;
			}
		}
		int lvl_exp = player_template::GetPetLvlupExp(cur_level);
		if(cur_exp < lvl_exp) break; //加的经验不够升级的
		lp = true;
		cur_exp -= lvl_exp;
		AddPetCivilization(size_t(0.02f * cur_level * cur_level + cur_level + 0.5f));
		cur_level += 1;
		
	}while(true);
	ess.SetExp(cur_exp);
	ess.SetLevel(cur_level);
	if(lp)
	{
		pImp->_runner->player_pet_level_up(pet_index,pData->pet_id.id,ess.GetLevel(),ess.GetExp());
		//调用脚本刷新宠物信息
		gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
		lua_State* L = keeper.GetState();
		if(NULL == L)
		{
			GLog::log(LOG_ERR,"宠物信息(升级) 执行脚本出错 PetLevelUp NULL == L(tid: %d)",it.type);
			__PRINTF("宠物信息(升级) 执行脚本出错 PetLevelUp NULL == L(tid: %d)\n",it.type);
			return false;
		} 
		lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_LevelUp_Entrance");
		lua_pushinteger(L,it.type);
		lua_pushlightuserdata(L,&it);
		lua_pushinteger(L,ess.GetOrigin());
		lua_pushlightuserdata(L,&ess);
		if(lua_pcall(L,4,1,0))
		{
			GLog::log(LOG_ERR,"宠物信息(升级) 执行脚本出错 PetLevelUp lua_pcall failed(tid: %d),when calling %s",it.type,lua_tostring(L,-1));
			__PRINTF("宠物信息(升级) 执行脚本出错 PetLevelUp lua_pcall failed(tid: %d),when calling %s\n",it.type,lua_tostring(L,-1));
			lua_pop(L,1);
			return false;
		}
		if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
		{
			GLog::log(LOG_ERR,"宠物信息(升级) 执行脚本出错 PetLevelUp 返回值数据错误(tid: %d)",it.type);
			__PRINTF("宠物信息(升级) 执行脚本出错 PetLevelUp 返回值数据错误(tid: %d)\n",it.type);
			lua_pop(L,1);
			return false;
		}
		int ret = lua_tointeger(L,-1);
		lua_pop(L,1);
		if(it.type != ret)
		{
			GLog::log(LOG_ERR,"宠物信息(升级) 升级失败(tid: %d,ret: %d)",it.type,ret);
			__PRINTF("宠物信息(升级) 升级失败(tid: %d,ret: %d)\n",it.type,ret);
			return false;
		}
		ess.UpdateContent(&it);
		//通知宠物属性更新一下
		RefreshPetInfo(pImp,pet_index);
		pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
		pImp->_runner->player_pet_civilization(GetPetCivilization());
	}
	else
	{
		ess.UpdateContent(&it);
		pImp->_runner->player_pet_recv_exp(pet_index,pData->pet_id.id,cur_exp);
	}
	return true;
}

void pet_manager::SetDBData(gplayer_imp* pImp,const void* buf,size_t size)
{
	if(0 == size)
	{
		//开放宠物后第一次上线的人
		_data.version = PET_DB_DATA_VERSION;
		_data.pet_bedge_inv_size = INIT_PET_BEDGE_LIST_SIZE;
		_data.pet_civilization = 0;
		_data.pet_construction = 0;
		_data.reserved1 = 0;
		_data.reserved2 = 0;
		_data.reserved3 = 0;
		_data.reserved4 = 0;
		pImp->GetPetBedgeInventory().SetSize(_data.pet_bedge_inv_size);
		return;
	}
	if(size > sizeof(pet_data_t) || size < sizeof(int))
	{
		ASSERT(false && "size > sizeof(pet_data_t) || size < sizeof(int)");
	}
	int version = *(int*)buf;
	if(version > PET_DB_DATA_VERSION)
	{
		ASSERT(false && "version > PET_DB_DATA_VERSION");
	}
	else if(PET_DB_DATA_VERSION == version)
	{
		if(size != sizeof(pet_data_t))
		{
			ASSERT(false && "PET_DB_DATA_VERSION == version && size != sizeof(pet_data_t)");
		}
	}
	memset(&_data,0,sizeof(pet_data_t));
	memcpy(&_data,buf,size);
	if(_data.pet_bedge_inv_size < INIT_PET_BEDGE_LIST_SIZE) _data.pet_bedge_inv_size = INIT_PET_BEDGE_LIST_SIZE;
	if(_data.pet_bedge_inv_size > MAX_PET_BEDGE_LIST_SIZE) _data.pet_bedge_inv_size = MAX_PET_BEDGE_LIST_SIZE;
	pImp->GetPetBedgeInventory().SetSize(_data.pet_bedge_inv_size);
}

void pet_manager::AddPetConstruction(gplayer_imp* pImp,int added)
{
	if(added < 0) return;
	size_t old = _data.pet_construction;
	_data.pet_construction += added;
	if(old > _data.pet_construction) _data.pet_construction = 0xFFFFFFFF;
	pImp->_runner->player_pet_construction(GetPetConstruction());
}

void pet_manager::SetPetBedgeInventorySize(gplayer_imp* pImp,size_t size)
{
	_data.pet_bedge_inv_size = size;
	if(_data.pet_bedge_inv_size < INIT_PET_BEDGE_LIST_SIZE) _data.pet_bedge_inv_size = INIT_PET_BEDGE_LIST_SIZE;
	if(_data.pet_bedge_inv_size > MAX_PET_BEDGE_LIST_SIZE) _data.pet_bedge_inv_size = MAX_PET_BEDGE_LIST_SIZE;
	pImp->GetPetBedgeInventory().SetSize(_data.pet_bedge_inv_size);
}

bool pet_manager::GetPetEffectProp(gplayer_imp* pImp,size_t pet_index,msg_pet_info_changed_t& prop,itemdataman::_pet_armor_essence& equip_added,int& equip_mask)
{
	memset(&prop,0,sizeof(prop));
	memset(&equip_added,0,sizeof(equip_added));
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	prop.level = ess.GetLevel();
	prop.honor_level = GetHonorLevel(ess.GetCurHonorPoint());
	prop.hunger_level = GetHungerLevel(ess.GetCurHungerPoint(),ess.GetMaxHungerPoint());
	const pet_bedge_data_temp* pTemplate = pet_bedge_dataman::Get(it.type);
	if(NULL == pTemplate)
	{
		GLog::log(GLOG_ERR,"宠物信息: Invalid pet template id %d",it.type);
		__PRINTF("宠物信息: Invalid pet template id %d\n",it.type);
		return false;
	}
	GenerateBaseProp(ess,*pTemplate,prop.base_prop);
	GenerateEnhancedProp(ess,*pTemplate,prop.en_point);
	prop.damage_reduce = ess.GetDamageReduce();
	prop.damage_reduce_percent = ess.GetDamageReducePercent();
	AddEquipProp(pImp,ess,prop.base_prop,equip_mask,&equip_added);
	//加入阶级影响值
	prop.base_prop.max_hp = int(prop.base_prop.max_hp * (1 + GetMaxHPAdd(ess.GetRank())) + 0.5f);
	prop.base_prop.damage_low = int(prop.base_prop.damage_low * (1 + GetAttackAdd(ess.GetRank())) + 0.5f);
	prop.base_prop.damage_high = int(prop.base_prop.damage_high * (1 + GetAttackAdd(ess.GetRank())) + 0.5f);
	if(GetRankPetCount(pImp,pet_bedge_enhanced_essence::PET_RANK_COMBAT6) > 0)
	{
		//有战神存在
		//加抗性
		for(int i = 0;i < 6;++i)
		{
			prop.base_prop.resistance[i] += 20;
		}
	}
	return true;
}

void pet_manager::RefreshPetInfo(gplayer_imp* pImp,size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	memset(&pData->effect_prop,0,sizeof(pData->effect_prop));
	memset(&pData->equip_added,0,sizeof(pData->equip_added));
	GetPetEffectProp(pImp,pet_index,pData->effect_prop,pData->equip_added,pData->equip_mask);
	pImp->_runner->player_pet_prop_added(pet_index,pData->equip_added.maxhp,pData->equip_added.maxmp,pData->equip_added.defence,pData->equip_added.attack,
		pData->equip_added.resistance,pData->equip_added.hit,pData->equip_added.jouk,pData->equip_added.crit_rate,pData->equip_added.crit_damage,pData->equip_mask);
	if(pData->pet_id.IsActive())
	{
		pImp->SendTo2<0>(GM_MSG_PET_INFO_CHANGED,pData->pet_id,pet_index,pData->summon_stamp,&pData->effect_prop,sizeof(msg_pet_info_changed_t));
	}
}

void pet_manager::GetPetPropAddedInfo(gplayer_imp* pImp, size_t pet_index, S2C::CMD::player_pet_prop_added * data_prop_added)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) 
	{
		return;
	}

	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	memset(&pData->effect_prop,0,sizeof(pData->effect_prop));
	memset(&pData->equip_added,0,sizeof(pData->equip_added));
	GetPetEffectProp(pImp,pet_index,pData->effect_prop,pData->equip_added,pData->equip_mask);

	data_prop_added->pet_index			= pet_index;
	data_prop_added->maxhp				= pData->equip_added.maxhp;
	data_prop_added->maxmp				= pData->equip_added.maxmp;
	data_prop_added->defence			= pData->equip_added.defence;
	data_prop_added->attack				= pData->equip_added.attack;
	for(int i = 0; i < 6; ++i)
	{
		data_prop_added->resistance[i]	= pData->equip_added.resistance[i];
	}
	data_prop_added->hit				= pData->equip_added.hit;
	data_prop_added->jouk				= pData->equip_added.jouk;
	data_prop_added->crit_rate			= pData->equip_added.crit_rate;
	data_prop_added->crit_damage		= pData->equip_added.crit_damage;
	data_prop_added->equip_mask			= pData->equip_mask;
}

void pet_manager::ModifyHungerPoint(gplayer_imp* pImp,size_t pet_index,pet_bedge_essence& ess,int value)
{
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	int old_level = GetHungerLevel(ess.GetCurHungerPoint(),ess.GetMaxHungerPoint());
	ess.SetCurHungerPoint(ess.GetCurHungerPoint() + value);
	if(ess.GetCurHungerPoint() < 0) ess.SetCurHungerPoint(0);
	if(ess.GetCurHungerPoint() > ess.GetMaxHungerPoint()) ess.SetCurHungerPoint(ess.GetMaxHungerPoint());
	int new_level = GetHungerLevel(ess.GetCurHungerPoint(),ess.GetMaxHungerPoint());
	pImp->_runner->player_pet_hunger_point(pet_index,ess.GetCurHungerPoint(),ess.GetMaxHungerPoint());
	if(old_level != new_level && pData->pet_id.IsActive())
	{
		pImp->SendTo<0>(GM_MSG_PET_HUNGER_LEVEL_CHANGED,pData->pet_id,new_level);
	}
	ess.UpdateContent(&it);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
}

void pet_manager::ModifyHonorPoint(gplayer_imp* pImp,size_t pet_index,pet_bedge_essence& ess,int value)
{
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	int old_level = GetHonorLevel(ess.GetCurHonorPoint());
	ess.SetCurHonorPoint(ess.GetCurHonorPoint() + value);
	if(ess.GetCurHonorPoint() < 0) ess.SetCurHonorPoint(0);
	if(ess.GetCurHonorPoint() > ess.GetMaxHonorPoint()) ess.SetCurHonorPoint(ess.GetMaxHonorPoint());
	int new_level = GetHonorLevel(ess.GetCurHonorPoint());
	pImp->_runner->player_pet_honor_point(pet_index,ess.GetCurHonorPoint(),ess.GetMaxHonorPoint());
	if(old_level != new_level)
	{
		if(PetBedgeRebuild(pImp,pet_index))
		{
			RefreshPetInfo(pImp,pet_index);
		}
	}
	ess.UpdateContent(&it);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
}

bool pet_manager::CheckPetBedgeExist(gplayer_imp* pImp,size_t pet_index)
{
	if(pet_index >= pImp->GetPetBedgeInventory().Size()) return false;
	if(-1 == pImp->GetPetBedgeInventory()[pet_index].type) return false;
	const item& it = pImp->GetPetBedgeInventory()[pet_index];
	if(item_body::ITEM_TYPE_PET_BEDGE != it.GetItemType()) return false;
	return true;
}

bool pet_manager::CheckPetEquipExist(gplayer_imp* pImp,size_t pet_equip_index)
{
	if(pet_equip_index >= pImp->GetPetEquipInventory().Size()) return false;
	if(-1 == pImp->GetPetEquipInventory()[pet_equip_index].type) return false;
	const item& it = pImp->GetPetEquipInventory()[pet_equip_index];
	if(item_body::ITEM_TYPE_PET_EQUIP != it.GetItemType()) return false;
	return true;
}

size_t pet_manager::GetBattleStatusPetCount(gplayer_imp* pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	size_t fight_count = 0;
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		const item& it = pImp->GetPetBedgeInventory()[i];
		size_t len;
		const void* buf = it.GetContent(len);
		pet_bedge_essence ess(buf,len);
		ess.PrepareData();
		if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT == ess.GetMainStatus())
		{
			fight_count ++;
		}
	}
	return fight_count;
}

int pet_manager::TestCanEquipPetBedge(gplayer_imp* pImp,size_t inv_index,size_t pet_index)
{
	if(inv_index >= pImp->GetInventory().Size()) return S2C::ERR_ITEM_CANNOT_EQUIP;
	if(-1 == pImp->GetInventory()[inv_index].type) return S2C::ERR_ITEM_CANNOT_EQUIP;
	item& it = pImp->GetInventory()[inv_index];
	if(item_body::ITEM_TYPE_PET_BEDGE != it.GetItemType()) return S2C::ERR_ITEM_CANNOT_EQUIP;
	size_t len;
	void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	const pet_bedge_data_temp* pTemplate = pet_bedge_dataman::Get(it.type);
	if(NULL == pTemplate)
	{
		GLog::log(GLOG_ERR,"宠物信息: Invalid pet template id %d",it.type);
		__PRINTF("宠物信息: Invalid pet template id %d\n",it.type);
		return false;
	}
	if(pTemplate->init_level <= 200)
	{
		if(ess.GetOwnerID() != pImp->GetParent()->ID.id) return S2C::ERR_PET_NOT_ADOPTED;
	}
	ess.SetOwnerID(pImp->GetParent()->ID.id);
	ess.UpdateContent(&it);
	return 0;
}

int pet_manager::TestCanEquipPetEquip(gplayer_imp* pImp,size_t inv_index,size_t pet_equip_index)
{
	if(inv_index >= pImp->GetInventory().Size()) return S2C::ERR_ITEM_CANNOT_EQUIP;
	if(-1 == pImp->GetInventory()[inv_index].type) return S2C::ERR_ITEM_CANNOT_EQUIP;
	const item& it = pImp->GetInventory()[inv_index];
	if(item_body::ITEM_TYPE_PET_EQUIP != it.GetItemType()) return S2C::ERR_ITEM_CANNOT_EQUIP;
	if(sizeof(itemdataman::_pet_armor_essence) != it.content.size) return S2C::ERR_FATAL_ERR;
	const pet_equip_data_temp* pTemplate = pet_equip_dataman::Get(it.type);
	if(!pTemplate) return S2C::ERR_FATAL_ERR;
	if((size_t)pTemplate->equip_location != pet_equip_index) return S2C::ERR_ITEM_CANNOT_EQUIP;
	return 0;
}

void pet_manager::OnEquipPetBedge(gplayer_imp* pImp,size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	//刚刚装备的设置为休息状态
	ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
	ess.SetRank(pet_bedge_enhanced_essence::PET_RANK_BASE);
	//刚装备时采集标志清0
	memset(pData,0,sizeof(pet_data));
	pImp->_runner->player_set_pet_status(pet_index,it.type,ess.GetMainStatus(),ess.GetSubStatus());
	ess.UpdateContent(&it);
	RefreshPetInfo(pImp,pet_index);
}

void pet_manager::OnEquipPetEquip(gplayer_imp* pImp,size_t pet_equip_index)
{
	//更新属性
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		RefreshPetInfo(pImp,i);
	}
}

int pet_manager::SetPetStatus(gplayer_imp* pImp,size_t pet_index,int pet_tid,int main_status,int sub_status)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return S2C::ERR_NEED_EQUIP_PET_BEDGE;
	if(main_status < pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT || main_status > pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST) return S2C::ERR_FATAL_ERR;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	if(it.type != pet_tid) return S2C::ERR_FATAL_ERR;
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(GetBattleStatusPetCount(pImp) > 0 && main_status == pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT && ess.GetMainStatus() != pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT)
	{
		return S2C::ERR_CAN_NOT_SET_MORE_PET_FOR_COMBET_STATUS;
	}
	//看看是不是主人
	if(ess.GetOwnerID() != pImp->GetParent()->ID.id) return S2C::ERR_PET_NOT_ADOPTED;
	//状态没变化忽略掉
	if(ess.GetMainStatus() == main_status && ess.GetSubStatus() == sub_status) return 0;
	pData->collect_counter = 0;
	if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT == ess.GetMainStatus() && pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT != main_status)
	{
		//战斗换到别的需要把宠物收了
		DoRecallPet(pImp,pet_index);
		DoUncombinePet(pImp,pet_index);
	}
	switch(main_status)
	{
		case pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT:
		{
			if(0 == ess.GetLife()) return 0;
			if(ess.GetAge() >= ess.GetLife()) return 0;

			if(ess.GetRebornCnt() > 0)
			{
				if(sub_status != pet_bedge_enhanced_essence::PET_SUB_STATUS_FIGHT_TONGLING && sub_status != pet_bedge_enhanced_essence::PET_SUB_STATUS_FIGHT_YUBAO)
				{
					return 0;
				}
			}
			else 
			{
				if(sub_status != 0) return 0;
			}
		}
		break;

		case pet_bedge_enhanced_essence::PET_MAIN_STATUS_COLLECT:
		{
			//要设定为采集状态,额外判断
			if(sub_status < pet_bedge_enhanced_essence::PET_SUB_STATUS_PLANTING || sub_status > pet_bedge_enhanced_essence::PET_SUB_STATUS_ARCHAEOLOGY) return S2C::ERR_FATAL_ERR;
			//采集
			if(0 == ess.GetLife()) return 0;
			if(ess.GetAge() >= ess.GetLife()) return S2C::ERR_AGE_LIFE_INVALID;
			if(!ess.GetIdentify()) return 0;
			if(ess.GetLevel() < 30) return 0;
		}
		break;

		case pet_bedge_enhanced_essence::PET_MAIN_STATUS_MAKE:
		{
			return 0;
			if(0 == ess.GetLife()) return 0;
			if(ess.GetAge() >= ess.GetLife()) return 0;
			if(!ess.GetIdentify()) return 0;
			if(ess.GetLevel() < 60) return 0;
		}
		break;

		case pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST:
		{
		}
		break;

		default:
			break;
	}
	ess.SetMainStatus(main_status);
	ess.SetSubStatus(sub_status);
	ess.UpdateContent(&it);
	pImp->_runner->player_set_pet_status(pet_index,pet_tid,main_status,sub_status);
	return 0;
}

int pet_manager::GetRankPetCount(gplayer_imp* pImp,int rank)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	int count = 0;
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		item& it = pImp->GetPetBedgeInventory()[i];
		size_t len;
		const void* buf = it.GetContent(len);
		pet_bedge_essence ess(buf,len);
		ess.PrepareData();
		if(ess.GetRank() == rank) ++count;
	}
	return count;
}

int pet_manager::SetPetRank(gplayer_imp* pImp,size_t pet_index,int pet_tid,int rank)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return S2C::ERR_NEED_EQUIP_PET_BEDGE;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	if(it.type != pet_tid) return S2C::ERR_FATAL_ERR;
	if(rank < 0 || rank >= pet_bedge_enhanced_essence::MAX_PET_RANK) return S2C::ERR_FATAL_ERR;
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	//看看是不是主人
	if(ess.GetOwnerID() != pImp->GetParent()->ID.id) return S2C::ERR_PET_NOT_ADOPTED;
	if(ess.GetRank() == rank) return 0;
	//阶级设定判断
	if(GetPetCivilization() < GetCivilizationNeed(rank)) return S2C::ERR_NOT_ENOUGH_CIVILIZATION;
	if(ess.GetLevel() < GetLevelNeed(rank)) return S2C::ERR_NOT_ENOUGH_PET_LEVEL;
	if(GetRankPetCount(pImp,rank) >= GetMaxCount(rank)) return S2C::ERR_CUR_PET_RANK_MAX;
	ess.SetRank(rank);
	ess.UpdateContent(&it);
	pImp->_runner->player_set_pet_rank(pet_index,pet_tid,rank);
	RefreshPetInfo(pImp,pet_index);
	return 0;
}

int pet_manager::TestCanSummonPet(gplayer_imp* pImp,size_t pet_index,A3DVECTOR& pos)
{
	if(PET_SUMMON_ALLOWED_COUNT <= GetActivePetCount()) return S2C::ERR_PET_ACTIVE_TOO_MUCH;
	if(!CheckPetBedgeExist(pImp,pet_index)) return S2C::ERR_NEED_EQUIP_PET_BEDGE;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	if(IsPetActive(pet_index)) return S2C::ERR_PET_IS_ALEARY_ACTIVE;
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT != ess.GetMainStatus()) return S2C::ERR_PET_NOT_IN_BATTLE_STATUS;
	if(ess.GetOwnerID() != pImp->GetParent()->ID.id) return S2C::ERR_PET_NOT_ADOPTED;//其实装备的时候已经判断过了
	if(ess.GetCurHP() < 0) return S2C::ERR_CANNOT_SUMMON_DEAD_PET;
	if(ess.GetAge() >= ess.GetLife()) return S2C::ERR_CANNOT_SUMMON_DEAD_PET;
	if(ess.GetRebornCnt() > pImp->GetRebornCount()) return S2C::ERR_CANNOT_SUMMON_REBORN_PET;
	if(ess.GetRebornCnt() > 0 && pImp->IsCombatState()) 
	{
		pet_data* pData = &_pet_list[pet_index];
		if(g_timer.get_systime() - pData->summon_timestamp < 30)
		{
			return S2C::ERR_PET_SUMMON_COOLDOWN;
		}
	}
	
	//招出普通宠物的条件需要满足环境 水下不能招出所有宠物 空中无法招出地面宠物 
	pos = pImp->GetParent()->pos;

	if(IsRebornPet(pImp,pet_index))
	{
		if((!pImp->OI_IsFlying()) && !FindGroundPos(pImp->GetWorldManager(),pos))
		{
			//召唤失败,因为没有可以立足的地点
			return S2C::ERR_SUMMON_PET_INVALID_POS;
		}
	}
	else
	{
		if(!FindGroundPos(pImp->GetWorldManager(),pos))
		{
			//召唤失败,因为没有可以立足的地点
			return S2C::ERR_SUMMON_PET_INVALID_POS;
		}
	}
	return 0;
}

int pet_manager::TestCanRecallPet(gplayer_imp* pImp,size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return S2C::ERR_NEED_EQUIP_PET_BEDGE;
	if(!IsPetSummon(pet_index)) return S2C::ERR_PET_NOT_SUMMON;
	return 0;
}

int pet_manager::TestCanCombinePet(gplayer_imp* pImp,size_t pet_index,int type, bool need_cooldown)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return S2C::ERR_NEED_EQUIP_PET_BEDGE;
	const item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT != ess.GetMainStatus()) return S2C::ERR_PET_NOT_IN_BATTLE_STATUS;
	if(need_cooldown)
	{
		if(0 == type)
		{
			if(!pImp->CheckCoolDown(COOLDOWN_INDEX_PET_COMBINE1)) return S2C::ERR_PET_COMBINE1_COOLDOWN;
		}
		else
		{
			if(!pImp->CheckCoolDown(COOLDOWN_INDEX_PET_COMBINE2)) return S2C::ERR_PET_COMBINE2_COOLDOWN;
		}
	}
	return 0;
}

int pet_manager::TestCanUncombinePet(gplayer_imp* pImp,size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return S2C::ERR_NEED_EQUIP_PET_BEDGE;
	if(!IsPetCombine(pet_index)) return S2C::ERR_PET_NOT_COMBINE;
	return 0;
}

//召唤宠物
int pet_manager::SummonPet(gplayer_imp* pImp,size_t pet_index)
{
	//player 的状态在外面完成判断
	//首先进行基础判断
	A3DVECTOR pos;
	int rst = TestCanSummonPet(pImp,pet_index,pos);
	if(0 != rst)
	{
		return rst;
	}
	DoSummonPet(pImp,pet_index,pos);
	return 0;
}

//进行
int pet_manager::RecallPet(gplayer_imp* pImp,size_t pet_index)
{
	int rst = TestCanRecallPet(pImp,pet_index);
	if(0 != rst)
	{
		return rst;
	}
	DoRecallPet(pImp,pet_index);
	return 0;
}

int pet_manager::CombinePet(gplayer_imp* pImp,size_t pet_index,int type, bool need_cooldown)
{
	int rst = TestCanCombinePet(pImp,pet_index,type, need_cooldown);
	if(0 != rst)
	{
		return rst;
	}

	if(IsRebornPet(pImp,pet_index))
	{
		DoRebornCombinePet(pImp, pet_index, type);
	}
	else
	{
		DoRecallPet(pImp,pet_index);
		DoCombinePet(pImp,pet_index,type, need_cooldown);
	}
	return 0;
}

int pet_manager::UncombinePet(gplayer_imp* pImp,size_t pet_index,int type)
{
	int rst = TestCanUncombinePet(pImp,pet_index);
	if(0 != rst)
	{
		return rst;
	}
	DoUncombinePet(pImp,pet_index);
	A3DVECTOR pos;
	rst = TestCanSummonPet(pImp,pet_index,pos);
	if(0 != rst)
	{
		return rst;
	}
	DoSummonPet(pImp,pet_index,pos);
	return 0;
}

void pet_manager::ClearPetBehavior(gplayer_imp* pImp,size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return;
	DoRecallPet(pImp,pet_index);
	DoUncombinePet(pImp,pet_index);
}

void pet_manager::ClearAllPetBehavior(gplayer_imp* pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		DoRecallPet(pImp,i);
		DoUncombinePet(pImp,i);
	}
}

void pet_manager::RecordCurSummonPet(gplayer_imp* pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(IsPetSummon(i))
		{
			_cur_summon_pet_index = i;
			return;
		}
	}
}


void pet_manager::TrySummonLastSummonPet(gplayer_imp* pImp)
{
	if(-1 != _cur_summon_pet_index)
	{
		SummonPet(pImp,_cur_summon_pet_index);
		_cur_summon_pet_index = -1;
	}
}

bool pet_manager::PetRelocatePos(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,bool force_disappear)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	if(!IsPetSummon(pet_index)) return false;
	pet_data* pData = &_pet_list[pet_index];
	if(pData->pet_id != who) return false;
	if(pData->summon_stamp != stamp) return false;
	A3DVECTOR pos = pImp->_parent->pos;

	//飞升宠物如果在空中不召回
	if(force_disappear || ( (!IsRebornPet(pImp, pet_index) || (IsRebornPet(pImp, pet_index) && !pImp->OI_IsFlying()))  && !FindGroundPos(pImp->GetWorldManager(),pos)))
	{
		//无法找到合适的内容 或者宠物要求消失
		DoRecallPet(pImp,pet_index);
		return true;
	}
	//找到了合适的坐标发送给对方
	pImp->SendTo2<0>(GM_MSG_PET_CHANGE_POS,who,pet_index,stamp,&pos,sizeof(pos));
	return true;
}

bool pet_manager::PetNotifyPetHPVPState(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,const msg_pet_notify_hp_vp_t& info)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	if(!IsPetSummon(pet_index)) return false;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	pet_data* pData = &_pet_list[pet_index];
	if(pData->pet_id != who) return false;
	if(pData->summon_stamp != stamp) return false;
	//宠物条件满足才能继续
	//宠物通知计数器加一
	++pData->cur_state;
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(ess.GetCurHP() != info.cur_hp || ess.GetCurVigor() != info.cur_vp)
	{
		ess.SetCurHP(info.cur_hp);
		ess.SetCurVigor(info.cur_vp);
		//通知客户端
		pImp->_runner->player_pet_hp_notify(pet_index,info.cur_hp,info.max_hp,info.cur_vp,info.max_vp);
		pData->effect_prop.base_prop.max_hp = info.max_hp;
		pData->effect_prop.base_prop.max_mp = info.max_vp;
	}
	if(pData->aggro_state != info.aggro_state || pData->stay_state != info.stay_state)
	{
		pData->aggro_state = info.aggro_state;
		pData->stay_state = info.stay_state;
		//通知客户端
		pImp->_runner->player_pet_ai_state(pet_index,pData->stay_state,pData->aggro_state);
	}
	ess.UpdateContent(&it);
	return true;
}

bool pet_manager::PetDeath(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,int kill_by_npc)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	if(!IsPetSummon(pet_index)) return false;
	pet_data* pData = &_pet_list[pet_index];
	if(pData->pet_id != who) return false;
	if(pData->summon_stamp != stamp) return false;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	//宠物条件满足才能继续
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	//更新宠物的血值
	ess.SetCurHP(0);
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"宠物信息(死亡) 执行脚本出错 PetDeath NULL == L(tid: %d)",it.type);
		__PRINTF("宠物信息(死亡) 执行脚本出错 PetDeath NULL == L(tid: %d)\n",it.type);
	} 
	else
	{
		if(pImp->GetWorldManager() && pImp->GetWorldManager()->IsRaidWorld() && pImp->GetWorldManager()->GetRaidType() == RT_TOWER)
		{
			kill_by_npc = 1;
		}
		lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Dead_Entrance");
		lua_pushinteger(L,it.type);
		lua_pushlightuserdata(L,&it);
		lua_pushlightuserdata(L,&ess);
		lua_pushinteger(L,kill_by_npc);
		if(lua_pcall(L,4,1,0))
		{
			GLog::log(LOG_ERR,"宠物信息(死亡) 执行脚本出错 PetDeath lua_pcall failed(tid: %d),when calling %s",it.type,lua_tostring(L,-1));
			__PRINTF("宠物信息(死亡) 执行脚本出错 PetDeath lua_pcall failed(tid: %d),when calling %s\n",it.type,lua_tostring(L,-1));
			lua_pop(L,1);
		}
		else
		{
			if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
			{
				GLog::log(LOG_ERR,"宠物信息(死亡) 执行脚本出错 PetDeath 返回值数据错误(tid: %d)",it.type);
				__PRINTF("宠物信息(死亡) 执行脚本出错 PetDeath 返回值数据错误(tid: %d)\n",it.type);
				lua_pop(L,1);
			}
			else
			{
				int ret = lua_tointeger(L,-1);
				lua_pop(L,1);
				if(it.type != ret)
				{
					GLog::log(LOG_ERR,"宠物信息(死亡) 脚本操作失败(tid: %d,ret: %d)",it.type,ret);
					__PRINTF("宠物信息(死亡) 脚本操作失败(tid: %d,ret: %d)\n",it.type,ret);
				}
			}
		}
	}
	ess.UpdateContent(&it);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
	//通知客户端
	pImp->_runner->player_pet_honor_point(pet_index,ess.GetCurHonorPoint(),ess.GetMaxHonorPoint());
	pImp->_runner->player_pet_hunger_point(pet_index,ess.GetCurHungerPoint(),ess.GetMaxHungerPoint());
	pImp->_runner->player_pet_age_life(pet_index,ess.GetAge(),ess.GetLife());
	pImp->_runner->player_pet_dead(pet_index);
	if(ess.GetAge() >= ess.GetLife())
	{
		ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
		ess.SetSubStatus(0);
		pImp->_runner->player_set_pet_status(pet_index,it.type,ess.GetMainStatus(),ess.GetSubStatus());
	}
	//召回宠物
	DoRecallPet(pImp,pet_index);
	return true;
}

bool pet_manager::PetSetAutoSkill(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,int skill_id,size_t set_flag)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	if(!IsPetSummon(pet_index)) return false;
	pet_data* pData = &_pet_list[pet_index];
	if(pData->pet_id != who) return false;
	if(pData->summon_stamp != stamp) return false;
	pImp->_runner->player_pet_set_auto_skill(pet_index,pData->pet_id.id,skill_id,set_flag);
	return true;
}

bool pet_manager::PetSetCoolDown(gplayer_imp* pImp,const XID& who,size_t pet_index,int stamp,int cooldown_index,int cooldown_time)
{
	/*
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	if(!IsPetSummon(pet_index)) return false;
	pet_data* pData = &_pet_list[pet_index];
	if(pData->pet_id != who) return false;
	if(pData->summon_stamp != stamp) return false;
	*/
	pImp->_runner->player_pet_set_skill_cooldown(pet_index,who.id,cooldown_index,cooldown_time);
	_cooldown.SetCoolDown(cooldown_index, cooldown_time);
	return true;
}


void pet_manager::Heartbeat(gplayer_imp* pImp)
{
	//这里对所有的宠物轮寻
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		item& it = pImp->GetPetBedgeInventory()[i];
		const pet_bedge_data_temp* pTemplate = pet_bedge_dataman::Get(it.type);
		if(!pTemplate)
		{
			__PRINTF("宠物数据错误,心跳中(it.type: %d)在宠物模板中不存在\n",it.type);
			continue;
		}
		pet_data* pData = &_pet_list[i];
		size_t len;
		const void* buf = it.GetContent(len);
		pet_bedge_essence ess(buf,len);
		ess.PrepareData();

		//饱食度
		++pData->hunger_counter;
		if(0 == pData->hunger_counter % HUNGER_TRIGGER_TIME)
		{
			//只要装备了就要变化
			pData->hunger_counter = 0;
			int hunger_dec = 0;
			if(IsPetCombine(i) || IsPetSummon(i))
			{
				//战斗状态的
				hunger_dec = pTemplate->hunger_point_dec_speed[1];
			}
			else
			{
				//非战斗状态
				hunger_dec = pTemplate->hunger_point_dec_speed[0];
			}
			if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST != ess.GetMainStatus() && ess.GetRebornCnt() == 0)
			{
				ModifyHungerPoint(pImp,i,ess,-hunger_dec);
			}
			if(ess.GetCurHungerPoint() <= 0)
			{
				//饱食度没了,收
				DoRecallPet(pImp,i);
				DoUncombinePet(pImp,i);
			}
		}
		//亲密度
		if(IsPetSummon(i) && ess.GetRebornCnt() == 0)
		{
			++pData->honor_counter;
			if(0 == pData->hunger_counter % HONOR_TRIGGER_TIME)
			{
				//只有放出来跟着跑的才加
				pData->honor_counter = 0;
				ModifyHonorPoint(pImp,i,ess,pTemplate->honor_point_inc_speed);
			}
		}
		//年龄, 2012-11-20，策划需求去除宠物年龄的自动增长
		/*if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST != ess.GetMainStatus())
		{
			//只要不是休息的就加
			/++pData->age_counter;
			if(0 == pData->age_counter % AGE_TRIGGER_TIME)
			{
				pData->age_counter = 0;
				ess.SetAge(ess.GetAge() + 1);
				pImp->_runner->player_pet_age_life(i,ess.GetAge(),ess.GetLife());
				if(ess.GetAge() >= ess.GetLife())
				{
					ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
					ess.SetSubStatus(0);
					pImp->_runner->player_set_pet_status(i,it.type,ess.GetMainStatus(),ess.GetSubStatus());
					DoRecallPet(pImp,i);
					DoUncombinePet(pImp,i);
				}
			}
		}*/
		//采集
		if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_COLLECT == ess.GetMainStatus())
		{
			++pData->collect_counter;
			if(0 == pData->collect_counter % COLLECT_TRIGGER_TIME)
			{
				pData->collect_counter = 0;
				int collect_type = ess.GetSubStatus();
				int item_tid = 0;
				int item_count = 0;
				//获得需要的物品和数量
				AskCollectRequuirement(it,ess,collect_type,item_tid,item_count);
				int collect_mode = 0;
				if(item_tid == 0 || item_count == 0)
				{
					//一个都没
					collect_mode = 2;
				}
				else if(!pImp->CheckItemExist(item_tid,item_count))
				{
					//有一部分
					collect_mode = 1;
				}
				else
				{
					pImp->UseItemByID(pImp,gplayer_imp::IL_INVENTORY,item_tid,item_count,S2C::DROP_TYPE_TAKEOUT);
				}
				//调用脚本触发采集
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
				lua_State* L = keeper.GetState();
				if(NULL == L)
				{
					GLog::log(LOG_ERR,"宠物信息(采集) 执行脚本出错 PetCollect NULL == L(tid: %d)",it.type);
					__PRINTF("宠物信息(采集) 执行脚本出错 PetCollect NULL == L(tid: %d)\n",it.type);
				} 
				else if(!pImp->CanProduceItem())
				{
					//交易状态，无法产出	
				}	      
				else
				{
					lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Collect_Entrance");
					lua_pushinteger(L,it.type);
					lua_pushlightuserdata(L,&it);
					lua_pushlightuserdata(L,&ess);
					lua_pushinteger(L,collect_type);
					lua_pushinteger(L,collect_mode);
					if(lua_pcall(L,5,5,0))
					{
						//GLog::log(LOG_ERR,"宠物信息(采集) 执行脚本出错 PetCollect lua_pcall failed(tid: %d),when calling %s",it.type,lua_tostring(L,-1));
						__PRINTF("宠物信息(采集) 执行脚本出错 PetCollect lua_pcall failed(tid: %d),when calling %s\n",it.type,lua_tostring(L,-1));
						lua_pop(L,1);
					}
					else
					{
						if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3) || !lua_isnumber(L,-4) || !lua_isnumber(L,-5))
						{
							//GLog::log(LOG_ERR,"宠物信息(采集) 执行脚本出错 PetCollect 返回值数据错误(tid: %d)",it.type);
							__PRINTF("宠物信息(采集) 执行脚本出错 PetCollect 返回值数据错误(tid: %d)\n",it.type);
							lua_pop(L,1);
							continue;
						}
						int item_count = lua_tointeger(L,-1);
						int item_tid = lua_tointeger(L,-2);
						int msg_id = lua_tointeger(L,-3);
						int channel_id = lua_tointeger(L,-4);
						int pet_tid = lua_tointeger(L,-5);
						lua_pop(L,1);
						pImp->_runner->script_message(pImp->_parent->ID.id,pData->pet_id.id,channel_id,msg_id);
						if(it.type != pet_tid)
						{
							__PRINTF("宠物信息(采集) 采集失败(tid: %d,ret: %d)\n",it.type,pet_tid);
						}
						else
						{
							//给东西
							const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_tid);
							if(pItem && pItem->pile_limit > 0)
							{
								bool bPocketSlot = pImp->GetPocketInventory().HasSlot(pItem->type);
								bool bInvSlot = pImp->GetInventory().HasSlot(pItem->type);
								bool bCanPutPocket = pItem->CanMoveToPocket();	
								if(!bInvSlot && !(bPocketSlot && bCanPutPocket))
								{
									pImp->_runner->script_message(pImp->_parent->ID.id,pData->pet_id.id,channel_id,msg_id);
									pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
								}
								else
								{
									item_data* data = DupeItem(*pItem);
									if((size_t)item_count > data->pile_limit) item_count = data->pile_limit;
									data->count = item_count;
									//放入随身包裹
									if(bPocketSlot && bCanPutPocket) 
									{
										if(pImp->ObtainItem(gplayer_imp::IL_POCKET, data)) FreeItem(data);

									}
									else if(bInvSlot)
									{
										if(pImp->ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
									}
									else
									{
										FreeItem(data);
									}
									ess.UpdateContent(&it);
									pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,i);
								}
							}
						}
					}
				}
			}
		}
		
		//3太分离的
		if(IsPetCombine(i))
		{
			//处于combine状态下的宠物
			--pData->combine_counter;
			if(pData->combine_counter <= 0)
			{
				DoUncombinePet(pImp,i);
				A3DVECTOR pos;
				int rst = TestCanSummonPet(pImp,i,pos);
				if(0 == rst)
				{
					DoSummonPet(pImp,i,pos);
				}
			}
		}
		else if(IsPetSummon(i))
		{
			//处于summon状态下的宠物
			++pData->pet_counter;
			if(pData->pet_counter >= 15)
			{
				if(pData->cur_state <= 0)
				{
					//超过15秒没有数据到来,则将宠物自动收回
					DoRecallPet(pImp,i);
				}
				else
				{
					pData->pet_counter = 0;
					pData->cur_state = 0;
				}
			}

			++pData->cur_notify_counter;
			if(pData->cur_notify_counter > NOTIFY_MASTER_TIME)
			{
				//发送自己的数据给宠物
				leader_prop data;
				pImp->SetLeaderData(data);
				if(pData->pet_id.IsValid())
				{
					pImp->SendTo2<0>(GM_MSG_MASTER_INFO,pData->pet_id,i,pData->summon_stamp,&data,sizeof(leader_prop));

				}
				pData->cur_notify_counter = 0;
			}
			if(pData->need_refresh)
			{
				pData->need_refresh = false;
				RefreshPetInfo(pImp,i);
			}
			++pData->talk_counter;
			if(pData->talk_counter >= TALK_TRIGGER_TIME)
			{
				pData->talk_counter = 0;
				gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
				lua_State* L = keeper.GetState();
				if(NULL == L)
				{
					GLog::log(LOG_ERR,"宠物信息(说话) 执行脚本出错 NULL == L(tid: %d)",it.type);
					__PRINTF("宠物信息(说话) 执行脚本出错 NULL == L(tid: %d)\n",it.type);
					continue;
				} 
				lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Talk_Entrance");
				lua_pushinteger(L,it.type);
				lua_pushlightuserdata(L,&it);
				lua_pushlightuserdata(L,&ess);
				if(lua_pcall(L,3,3,0))
				{
					//GLog::log(LOG_ERR,"宠物信息(说话) 执行脚本出错 lua_pcall failed(tid: %d),when calling %s",it.type,lua_tostring(L,-1));
					__PRINTF("宠物信息(说话) 执行脚本出错 lua_pcall failed(tid: %d),when calling %s\n",it.type,lua_tostring(L,-1));
					lua_pop(L,1);
					continue;
				}
				if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
				{
					//GLog::log(LOG_ERR,"宠物信息(说话) 执行脚本出错 返回值数据错误(tid: %d)",it.type);
					__PRINTF("宠物信息(说话) 执行脚本出错 返回值数据错误(tid: %d)\n",it.type);
					lua_pop(L,1);
					continue;
				}
				int ret = lua_tointeger(L,-3);
				int channel_id = lua_tointeger(L,-2);
				int msg_id = lua_tointeger(L,-1);
				lua_pop(L,1);
				if(it.type != ret)
				{
					GLog::log(LOG_ERR,"宠物信息(说话) 失败(tid: %d,ret: %d)",it.type,ret);
					__PRINTF("宠物信息(说话) 失败(tid: %d,ret: %d)\n",it.type,ret);
					continue;
				}
				pImp->_runner->script_message(pImp->_parent->ID.id,pData->pet_id.id,channel_id,msg_id);
			}
		}
		else
		{
			//处于none状态
		}
		ess.UpdateContent(&it);
	}
}

void pet_manager::OnMasterDeath(gplayer_imp* pImp)
{
	ClearAllPetBehavior(pImp);
	//现在主人死了什么也不扣了
}

void pet_manager::KillMob(gplayer_imp* pImp,int mob_level)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(!IsPetSummon(i)) continue;
		item& it = pImp->GetPetBedgeInventory()[i];
		size_t len;
		const void* buf = it.GetContent(len);
		pet_bedge_essence ess(buf,len);
		ess.PrepareData();
		if(ess.GetLevel() >= MAX_PET_LEVEL) continue; //上限
		if(0 == pImp->GetRebornCount())
		{
			if(ess.GetLevel() >= pImp->_basic.level + 5) continue;
		}
		//宠物条件满足才能继续
		int base_exp = mob_level * 3 + 30;
		int dis = abs(mob_level - ess.GetLevel());
		float rate = 1.0f;
		if(dis > 15 && dis <= 30)
		{
			rate = 0.5f;
		}
		else if(dis > 30 && dis <= 60)
		{
			rate = 0.25f;
		}
		else if(dis > 60)
		{
			rate = 0.1f;
		}
		int count = pImp->GetTeamCtrl().GetNearMemberCount(pImp->_parent->tag,pImp->_parent->pos);
		if(count < 1) count = 1;
		int exp_added = (int)(base_exp * rate * (0.9f + 0.1f * count) + 0.5f);
		if(exp_added > 0)
		{
			RecvExp(pImp,i,exp_added);
		}
	}
}

void pet_manager::PlayerBeAttacked(gplayer_imp* pImp,const XID& attacker)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(!IsPetSummon(i)) continue;
		pet_data* pData = &_pet_list[i];
		pImp->SendTo2<0>(GM_MSG_MASTER_ASK_HELP,pData->pet_id,i,pData->summon_stamp,&attacker,sizeof(attacker));
	}
}

void pet_manager::NotifyMasterInfo(gplayer_imp* pImp, leader_prop & data)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(!IsPetSummon(i)) continue;
                pet_data* pData = &_pet_list[i];
		pImp->SendTo2<0>(GM_MSG_MASTER_INFO,pData->pet_id,i,pData->summon_stamp,&data,sizeof(leader_prop));
	}
}

int pet_manager::AutoFeedPet(gplayer_imp* pImp,size_t pet_index,item *parent,float hp_gen,float mp_gen,float hunger_gen,int mask,int& cur_value)
{
	if(-1 != (int)pet_index) return AutoFeedOnePet(pImp,pet_index,parent,hp_gen,mp_gen,hunger_gen,mask,cur_value);
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		int ret = AutoFeedOnePet(pImp,i,parent,hp_gen,mp_gen,hunger_gen,mask,cur_value);
		if(0 != ret) return ret;
	}
	return 0;
}

int pet_manager::AutoFeedOnePet(gplayer_imp* pImp,size_t pet_index,item *parent,float hp_gen,float mp_gen,float hunger_gen,int mask,int& cur_value)
{
	if(cur_value <= 0) return 1;
	if(!CheckPetBedgeExist(pImp,pet_index)) return -1;
	pet_data* pData = &_pet_list[pet_index];
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(mask & item_pet_auto_food::HP)
	{
		if(0 == hp_gen)  return -1;
		int need = pData->effect_prop.base_prop.max_hp - ess.GetCurHP();
		if(need > 0)
		{
			int need2 = (int)(need/hp_gen + 1.0f);
			if(need2 <= 0) need2 = 1;
			if(IsPetSummon(pet_index))
			{
				//召唤出来的
				if(cur_value > need2)
				{
					cur_value = cur_value - need2;
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,need,0);
				}
				else
				{
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,(int)(0.5f + cur_value * hp_gen),0);
					return 1;
				}
			}
			else
			{
				//没召唤
				if(cur_value > need2)
				{
					cur_value = cur_value - need2;
					ess.SetCurHP(ess.GetCurHP() + need);
					ess.UpdateContent(&it);
				}
				else
				{
					ess.SetCurHP(ess.GetCurHP() + (int)(0.5f + cur_value * hp_gen));
					ess.UpdateContent(&it);
					return 1;
				}
			}
		}
	}
	if(mask & item_pet_auto_food::VP)
	{
		if(0 == mp_gen)  return -1;
		int need = pData->effect_prop.base_prop.max_mp - ess.GetCurVigor();
		if(need > 0)
		{
			int need2 = (int)(need/mp_gen + 1.0f);
			if(need2 <= 0) need2 = 1;
			if(IsPetSummon(pet_index))
			{
				//召唤
				if(cur_value > need2)
				{
					cur_value = cur_value - need2;
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,0,need);
				}
				else
				{
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,0,(int)(0.5f + cur_value * mp_gen));
					return 1;
				}
			}
			else
			{
				//没召唤
				if(cur_value > need2)
				{
					cur_value = cur_value - need2;
					ess.SetCurVigor(ess.GetCurVigor() + need);
					ess.UpdateContent(&it);
				}
				else
				{
					ess.SetCurVigor(ess.GetCurVigor() + (int)(0.5f + cur_value * mp_gen));
					ess.UpdateContent(&it);
					return 1;
				}
			}
		}
	}
	if(mask & item_pet_auto_food::HUNGER)
	{
		if(0 == hunger_gen) return -1;
		int need = ess.GetMaxHungerPoint() - ess.GetCurHungerPoint();
		if(need > 0)
		{
			int need2 = (int)(need/hunger_gen + 1.0f);
			if(need2 <= 0) need2 = 1;
			if(cur_value > need2)
			{
				cur_value = cur_value - need2;
				ModifyHungerPoint(pImp,pet_index,ess,need);
			}
			else
			{
				ModifyHungerPoint(pImp,pet_index,ess,(int)(0.5f + cur_value * hunger_gen));
				return 1;
			}
		}
	}
	return 0;
}

int pet_manager::RecoverPetFull(gplayer_imp* pImp, size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return -1;
	if(!IsPetSummon(pet_index))
	{
		pImp->_runner->error_message(S2C::ERR_PET_IS_NOT_ACTIVE);
		return -1;
	}
	pet_data* pData = &_pet_list[pet_index];
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();

	//补满HP和VP
	int hp_need = pData->effect_prop.base_prop.max_hp - ess.GetCurHP();
	int mp_need = pData->effect_prop.base_prop.max_mp - ess.GetCurVigor();
	if(hp_need > 0 || mp_need > 0)
	{
		pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id, hp_need, mp_need);
	}
	return 0;
}

int pet_manager::FeedPet(gplayer_imp* pImp,size_t pet_index,item* parent,int pet_level_min,int pet_level_max,
	unsigned int pet_type_mask,int food_usage,unsigned int food_type,int food_value,int& cur_value)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return -1;
	if(!IsPetSummon(pet_index))
	{
		pImp->_runner->error_message(S2C::ERR_PET_IS_NOT_ACTIVE);
		return -1;
	}
	pet_data* pData = &_pet_list[pet_index];
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	if(ess.GetLevel() > pet_level_max || ess.GetLevel() < pet_level_min)
	{
		pImp->_runner->error_message(S2C::ERR_USE_ITEM_FAILED);
		return -1;
	}
	const pet_bedge_data_temp* pTemplate = pet_bedge_dataman::Get(it.type);
	if(!pTemplate || !(1 << pTemplate->pet_type & pet_type_mask))
	{
		pImp->_runner->error_message(S2C::ERR_PET_FOOD_TYPE_NOT_MATCH);
		return -1;
	}
	switch(food_usage)
	{
		case 0://饱食度
		{
			if(ess.GetCurHungerPoint() >= ess.GetMaxHungerPoint()) return -1;//饱食度满的不用吃
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					ModifyHungerPoint(pImp,pet_index,ess,food_value);
				}
				return 1;
			}
			else
			{
				int need = ess.GetMaxHungerPoint() - ess.GetCurHungerPoint();
				if(need < 200) need = 200;
				if(cur_value > need)
				{
					cur_value = cur_value - need;
					ModifyHungerPoint(pImp,pet_index,ess,need);
					return 0;
				}
				else
				{
					ModifyHungerPoint(pImp,pet_index,ess,cur_value);
					return 1;
				}
			}
		}
		break;

		case 1://hp
		{
			if(ess.GetCurHP() >= pData->effect_prop.base_prop.max_hp) return -1; //满血不用吃
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,food_value,0);
				}
				return 1;
			}
			else
			{
				int need = pData->effect_prop.base_prop.max_hp - ess.GetCurHP();
				if(need < 200) need = 200;
				if(cur_value > need)
				{
					cur_value = cur_value - need;
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,need,0);
					return 0;
				}
				else
				{
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,cur_value,0);
					return 1;
				}
			}
		}
		break;

		case 2://vp
		{
			if(ess.GetCurVigor() >= pData->effect_prop.base_prop.max_mp) return -1; //满精力不用吃
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,0,food_value);
				}
				return 1;
			}
			else
			{
				int need = pData->effect_prop.base_prop.max_mp - ess.GetCurVigor();
				if(need < 200) need = 200;
				if(cur_value > need)
				{
					cur_value = cur_value - need;
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,0,need);
					return 0;
				}
				else
				{
					pImp->SendTo2<0>(GM_MSG_FEED_PET,pData->pet_id,0,cur_value);
					return 1;
				}
			}
		}
		break;

		case 3:
		{
			if(ess.GetCurHonorPoint() >= ess.GetMaxHonorPoint()) return -1;//亲密度满的不用吃
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					ModifyHonorPoint(pImp,pet_index,ess,food_value);
				}
				return 1;
			}
			return -1;
		}
		break;

		case 4:
		{
			if(ess.GetAge() <= 0) return -1; //年龄够小了
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					ess.SetAge(ess.GetAge() - food_value);
					if(ess.GetAge() < 0) ess.SetAge(0);
					ess.UpdateContent(&it);
					pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
				}
				return 1;
			}
			return -1;
		}
		break;

		case 5:
		{
			if(ess.GetLife() >= MAX_LIFE) return -1;//寿命太大了
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					int temp = food_value + ess.GetLife();
					if(temp <= 0)
					{
						//吃冒了?数据错误?
						return -1;
					}
					else
					{
						if(temp > MAX_LIFE) temp = MAX_LIFE;
						ess.SetLife(temp);
					}
					ess.UpdateContent(&it);
					pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
				}
				return 1;
			}
			return -1;
		}
		break;

		case 6:
		{
			if(1 == food_type)
			{
				if(food_value > 0)
				{
					if(ess.GetLevel() >= MAX_PET_LEVEL)
					{
						pImp->_runner->error_message(S2C::ERR_PET_LEVEL_TOO_HIGH_CAN_NOT_USE_ITEM);
						return -1;
					}
					if(0 == pImp->GetRebornCount())
					{
						if(ess.GetLevel() >= pImp->_basic.level + 5)
						{
							pImp->_runner->error_message(S2C::ERR_PET_LEVEL_TOO_HIGH_CAN_NOT_USE_ITEM);
							return -1;
						}
					}
					if(!RecvExp(pImp,pet_index,food_value,true))
					{
						return -1;
					}
				}
				return 1;
			}
			return -1;
		}
		break;

		default:
			return -1;
	}
	ess.UpdateContent(&it);
	//清空当前的喂食记数
	//pData->feed_period = 0;
	return -1;
}

bool pet_manager::PlayerPetCtrl(gplayer_imp* pImp,size_t pet_index,int cur_target,int pet_cmd,const void* buf,size_t size)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	if(!IsPetSummon(pet_index)) return false;
	pet_data* pData = &_pet_list[pet_index];
	if(size > 120) return false;
	if(pData->pet_id.IsValid())
	{
		char dbuf[128];
		*(int*)dbuf = pet_cmd;
		memcpy(dbuf + sizeof(int),buf,size);
		pImp->SendTo<0>(GM_MSG_PET_CTRL_CMD,pData->pet_id,cur_target,dbuf,size + sizeof(int));
	}
	return true;
}

void pet_manager::NotifyStartAttack(gplayer_imp* pImp,const XID& target,char force_attack)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(!IsPetSummon(i)) continue;
		pet_data* pData = &_pet_list[i];
		if(gpet_imp::PET_AGGRO_AUTO != pData->aggro_state) continue;
		pImp->SendTo<0>(GM_MSG_MASTER_START_ATTACK,pData->pet_id,force_attack,&target,sizeof(target));
	}
}

void pet_manager::NotifyDuelStop(gplayer_imp *pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(!IsPetSummon(i)) continue;
		pet_data* pData = &_pet_list[i];
		pImp->SendTo<0>(GM_MSG_MASTER_DUEL_STOP,pData->pet_id,0);
	}
}

int pet_manager::GetMakeAttr(gplayer_imp* pImp,int index)
{
	int sum = 0;
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		const item& it = pImp->GetPetBedgeInventory()[i];
		size_t len;
		const void* buf = it.GetContent(len);
		pet_bedge_essence ess(buf,len);
		ess.PrepareData();
		//if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_MAKE != ess.GetMainStatus()) continue;
		sum += ess.GetMakeAttr(index);
		sum += GetLifeSkillAdd(ess.GetRank());
	}
	if(GetRankPetCount(pImp,pet_bedge_enhanced_essence::PET_RANK_WORKSHOP6) > 0)
	{
		//有神匠存在
		//加能力值
		sum = int(1.15f * sum + 0.5f);
	}
	return sum;
}

bool pet_manager::CheckProduceItem(gplayer_imp* pImp,const recipe_template& rt)
{
	for(int i = 0;i < 6;++i)
	{
		if(GetMakeAttr(pImp,i) < rt.required[i])
		{
			//属性不够
			pImp->_runner->error_message(S2C::ERR_NOT_HAVE_ENOUGH_PET_ABILITY);
			return false;
		}
	}
	if(GetPetConstruction() < (size_t)rt.required[6])
	{
		//工坊建设度不够
		pImp->_runner->error_message(S2C::ERR_NOT_HAVE_ENOUGH_PET_CONSTRUCTION);
		return false;
	}
	return true;
}

bool pet_manager::FindGroundPos(world_manager* manager,A3DVECTOR& pos)
{
	size_t i = 0;
	for(i = 0;i < 10;++i)
	{
		A3DVECTOR t = pos;
		float offsetx = abase::Rand(1.f,4.f);
		float offsetz = abase::Rand(1.f,4.f);
		t.x += abase::Rand(0,1)?offsetx:-offsetx;
		t.z += abase::Rand(0,1)?offsetz:-offsetz;
		//t.y = manager->GetTerrain().GetHeightAt(t.x,t.z);
		if(!path_finding::GetValidPos(manager->GetMoveMap(), t)) continue;
		if(fabs(t.y - pos.y) >= 6.8f)
		{
			//高度差过大，重新寻找
			continue;
		}
		pos = t;
		return true;
	}
	return false;
}

void pet_manager::DbgChangePetLevel(gplayer_imp* pImp,size_t pet_index,int level)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return;
	if(level < 1)level = 1;
	if(level > MAX_PET_LEVEL) level = MAX_PET_LEVEL;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	ess.SetLevel(level);
	ess.UpdateContent(&it);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE,pet_index);
}

void pet_manager::AskCollectRequuirement(item& it,pet_bedge_essence& ess,int collect_type,int& item_tid,int& item_count)
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"宠物信息(采集) 执行脚本出错 AskCollectRequuirement NULL == L(tid: %d)",it.type);
		__PRINTF("宠物信息(采集) 执行脚本出错 AskCollectRequuirement NULL == L(tid: %d)\n",it.type);
		return;
	} 
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_AskCollectRequirement_Entrance");
	lua_pushinteger(L,it.type);
	lua_pushlightuserdata(L,&it);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,collect_type);
	if(lua_pcall(L,4,2,0))
	{
		__PRINTF("宠物信息(采集) 执行脚本出错 AskCollectRequuirement lua_pcall failed(tid: %d),when calling %s\n",it.type,lua_tostring(L,-1));
		lua_pop(L,1);
		return;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2))
	{
		__PRINTF("宠物信息(采集) 执行脚本出错 AskCollectRequuirement 返回值数据错误(tid: %d)\n",it.type);
		lua_pop(L,1);
		return;
	}
	item_count = lua_tointeger(L,-1);
	item_tid = lua_tointeger(L,-2);
}

void pet_manager::OnPlayerLogin(gplayer_imp* pImp)
{
	//这里对所有的宠物轮寻
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		item& it = pImp->GetPetBedgeInventory()[i];
		size_t len;
		const void* buf = it.GetContent(len);
		pet_bedge_essence ess(buf,len);
		ess.PrepareData();
		RefreshPetInfo(pImp,i);
	}
}

void pet_manager::SomePetBedgeExpired(gplayer_imp* pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i))
		{
			DoRecallPet(pImp,i);
			DoUncombinePet(pImp,i);
		}
	}
}

void pet_manager::SomePetEquipExpired(gplayer_imp* pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		RefreshPetInfo(pImp,i);
	}
}

bool pet_manager::PetBedgeRebuild(gplayer_imp* pImp,size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false;
	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"宠物信息(rebuild) 执行脚本出错 PetBedgeRebuild NULL == L(tid: %d)",it.type);
		__PRINTF("宠物信息(rebuild) 执行脚本出错 PetBedgeRebuild NULL == L(tid: %d)\n",it.type);
		return false;
	} 
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Rebuild_Entrance");
	lua_pushinteger(L,it.type);
	lua_pushlightuserdata(L,&it);
	lua_pushinteger(L,ess.GetOrigin());
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,pImp->_basic.level);
	lua_pushinteger(L,pImp->GetCultivation());
	if(lua_pcall(L,6,1,0))
	{
		GLog::log(LOG_ERR,"宠物信息(rebuild) 执行脚本出错 PetBedgeRebuild lua_pcall failed(tid: %d),when calling %s",it.type,lua_tostring(L,-1));
		__PRINTF("宠物信息(rebuild) 执行脚本出错 PetBedgeRebuild lua_pcall failed(tid: %d),when calling %s\n",it.type,lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
	{
		GLog::log(LOG_ERR,"宠物信息(rebuild) 执行脚本出错 PetBedgeRebuild 返回值数据错误(tid: %d)",it.type);
		__PRINTF("宠物信息(rebuild) 执行脚本出错 PetBedgeRebuild 返回值数据错误(tid: %d)\n",it.type);
		lua_pop(L,1);
		return false;
	}
	int ret = lua_tointeger(L,-1);
	lua_pop(L,1);
	if(it.type != ret)
	{
		GLog::log(LOG_ERR,"宠物信息(rebuild) 失败(tid: %d,ret: %d)",it.type,ret);
		__PRINTF("宠物信息(rebuild) 失败(tid: %d,ret: %d)\n",it.type,ret);
		return false;
	}
	ess.UpdateContent(&it);
	return true;
}

int pet_manager::GetActivePetIndex(gplayer_imp* pImp)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(CheckPetBedgeExist(pImp,i))
		{
			const item& it = pImp->GetPetBedgeInventory()[i];
			size_t len;
			const void* buf = it.GetContent(len);
			pet_bedge_essence ess(buf,len);
			ess.PrepareData();
			if(pet_bedge_enhanced_essence::PET_MAIN_STATUS_FIGHT == ess.GetMainStatus()) return i;
		}
	}
	return -1;
}

int pet_manager::GetPetCombineType(gplayer_imp* pImp,size_t pet_index)
{
	if(!IsPetCombine(pet_index)) return 0;
	pet_data* pData = &_pet_list[pet_index];
	return pData->combine_type;
}

void pet_manager::OnSetInvisible(gplayer_imp *pImp)
{
	int summon_index = -1;
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(IsPetSummon(i))
		{
			summon_index = i;
			break;
		}
	}

	if(summon_index != -1)
	{
		if(IsRebornPet(pImp,summon_index))
		{
			DoRecallPet(pImp,summon_index);
		}
		else
		{
			CombinePet(pImp, summon_index, 1, false);
			_change_flag = true;
		}
	}
}

void pet_manager::OnClearInvisible(gplayer_imp *pImp)
{
	int combine_index = -1;
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(IsPetCombine(i))
		{
			combine_index = i;
			break;
		}
	}

	if(combine_index != -1 && _change_flag)
	{
		int type = GetPetCombineType(pImp, combine_index);
		UncombinePet(pImp, combine_index , type);
		_change_flag = false;
	}
}


bool pet_manager::IsRebornPet(gplayer_imp * pImp, size_t pet_index)
{
	if(!CheckPetBedgeExist(pImp,pet_index)) return false; 

	item& it = pImp->GetPetBedgeInventory()[pet_index];
	size_t len;
	const void* buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();

	return ess.GetRebornCnt() > 0;
}

void pet_manager::ClearPetSkillCoolDown(gplayer_imp * pImp)
{
	abase::vector<int> list;
	_cooldown.GetCoolDownList(list);

	for(size_t i = 0; i < list.size(); ++i)
	{
		if(_cooldown.ClrCoolDown(list[i]))
		{
			pImp->_runner->set_cooldown(list[i], 0);
		}
	}
}

void pet_manager::GetSummonPetProp(gplayer_imp * pImp, int cs_index, int uid, int sid)
{
	size_t pet_bedge_count = pImp->GetPetBedgeInventory().Size();
	for(size_t i = 0;i < pet_bedge_count;++i)
	{
		if(!CheckPetBedgeExist(pImp,i)) continue;
		if(!IsPetSummon(i)) continue;
		if(!IsRebornPet(pImp,i)) return;

		pet_data* pData = &_pet_list[i];
		msg_master_get_pet_prop gpp;
		gpp.cs_index = cs_index;
		gpp.uid = uid;
		gpp.sid = sid;
		gpp.pet_index = i;

		pImp->SendTo<0>(GM_MSG_MASTER_GET_PET_PROP,pData->pet_id, 0, &gpp, sizeof(gpp)); 
		return;
	}
}

