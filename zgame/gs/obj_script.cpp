//
// player脚本，可以查询玩家信息,调用前需要压入player对象
//

#include "obj_script.h"
#include "actobject.h"
#include "world.h"
#include "player_imp.h"


namespace
{
void* GetLuaPointer(lua_State* L,int n,const char* name)
{
	lua_pushstring(L,name);
	lua_gettable(L,-2 - n);
	void *p = lua_touserdata(L,-1);
	lua_pop(L,1);
	return p;
}

void* GetAndTest(lua_State* L ,int n,const char* name)
{
	void *p = GetLuaPointer(L,n, name);
	if(!p)
	{
		return NULL;
	}
	return p;
}

bool CheckArgNum(lua_State* L,int n)
{
	int nArg = lua_gettop(L);    /* number of arguments */
	if(nArg == n + 1) return true;
	ASSERT(false);
	printf("lua 调用参数错误\n");
	return false;
}
}

#define RESTORE_OBJECT_DATA(L,n) \
	if(!CheckArgNum(L,n)) return 0;\
	gactive_imp* pImp = (gactive_imp*)GetAndTest(L,n,"__IMP");\
	if(!pImp) return 0;


//等级
int 
gactive_script::script_GetLevel(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->GetObjectLevel());
	return 1;
}

//Max HP
int 
gactive_script::script_GetHP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->GetMaxHP());
	return 1;
}

//Max MP
int 
gactive_script::script_GetMP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->GetMaxMP());
	return 1;
}

//Cur HP
int 
gactive_script::script_GetCurHP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->GetHP());
	return 1;
}

//Cur MP
int 
gactive_script::script_GetCurMP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->GetMP());
	return 1;
}


//Speed
int 
gactive_script::script_GetSpeed(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushnumber(L, pImp->_cur_prop.run_speed);
	return 1;

}

//致命一击率 致命伤害
int 
gactive_script::script_GetCrit(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.crit_rate);
	lua_pushnumber(L, pImp->_cur_prop.crit_damage);
	return 2;
}

//攻击 (high, low)
int 
gactive_script::script_GetDamage(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.damage_high);
	lua_pushinteger(L, pImp->_cur_prop.damage_low);
	return 2;
}


//普攻命中
int 
gactive_script::script_GetAttack(lua_State *L) 
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.attack);
	return 1;
}

//防御
int 
gactive_script::script_GetDefense(lua_State *L) 	
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.defense);
	return 1;
}

//普攻防御
int 
gactive_script::script_GetArmor(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.armor);
	return 1;
}

//6个抗性
int 
gactive_script::script_GetResistance(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.resistance[0]);
	lua_pushinteger(L, pImp->_cur_prop.resistance[1]);
	lua_pushinteger(L, pImp->_cur_prop.resistance[2]);
	lua_pushinteger(L, pImp->_cur_prop.resistance[3]);
	lua_pushinteger(L, pImp->_cur_prop.resistance[4]);
	lua_pushinteger(L, pImp->_cur_prop.resistance[5]);
	return 6;
}

//减免暴击
int 
gactive_script::script_GetAttackRange(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushnumber(L, pImp->_cur_prop.attack_range);
	return 1;
}

//减免暴击
int 
gactive_script::script_GetAntiCrit(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.anti_crit);
	lua_pushnumber(L, pImp->_cur_prop.anti_crit_damage);
	return 2;
}

//技能命中和技能躲闪
int 
gactive_script::script_GetSkillRate(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 0);
	lua_pushinteger(L, pImp->_cur_prop.skill_attack_rate);
	lua_pushinteger(L, pImp->_cur_prop.skill_armor_rate);
	return 2;
}


//技能等级
int 
gactive_script::script_GetSkillLevel(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int skill_id = lua_tointeger(L, -1);

	int base_level = pImp->GetSkillBaseLevel(skill_id);
	int total_level = pImp->GetSkillLevel(skill_id);
	
	lua_pushinteger(L, base_level);
	lua_pushinteger(L, total_level);
	return 2;
}

//查询位置
int 
gactive_script::script_GetPosition(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 0);
/*liuyue-facbase	
	int map_id = pImp->_plane->GetWorldIndex();
	if(pImp->GetWorldManager()->IsRaidWorld())
	{
		map_id = pImp->GetWorldManager()->GetRaidID();
	}
*/	
	int map_id = pImp->GetClientTag();
	lua_pushinteger(L, map_id);
        lua_pushnumber(L, pImp->_parent->pos.x);
	lua_pushnumber(L, pImp->_parent->pos.y);
	lua_pushnumber(L, pImp->_parent->pos.z);	
	return 4;
}

int
gactive_script::script_GetCultivation(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 0);
	int cul = pImp->OI_GetCultivation();
	lua_pushinteger(L, cul);
	return 1;
}

int
gactive_script::script_GetCircleMemberCnt(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 0);
	int count = pImp->GetCircleMemberCnt();
	lua_pushinteger(L, count);
	return 1;
}

//Level
int 
gactive_script::script_SetLevel(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	ASSERT(!pImp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gplayer_imp)));

	int level = lua_tointeger(L, -1);
	if(level > 0)
	{
		pImp->_basic.level = level;
	}
	return 0;
}

//HP
int 
gactive_script::script_SetHP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int hp = lua_tointeger(L, -1);	
	if(hp >= 0)
	{
		pImp->_base_prop.max_hp = hp;
		pImp->_basic.hp = hp;
	}
	return 0;
}

//MP
int 
gactive_script::script_SetMP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int mp = lua_tointeger(L, -1);	
	if(mp >= 0)
	{
		pImp->_base_prop.max_mp = mp;
		pImp->_basic.mp = mp;
	}
	return 0;
}


//Cur HP
int 
gactive_script::script_SetCurHP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int hp = lua_tointeger(L, -1);	
	if(hp >= 0)
	{
		if(hp > pImp->_base_prop.max_hp) hp = pImp->_base_prop.max_hp;
		pImp->_basic.hp = hp;
	}
	return 0;
}

//Cur MP
int 
gactive_script::script_SetCurMP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int mp = lua_tointeger(L, -1);	
	if(mp >= 0)
	{
		if(mp > pImp->_base_prop.max_mp) mp = pImp->_base_prop.max_mp;
		pImp->_basic.mp = mp;
	}
	return 0;

}


//Speed
int 
gactive_script::script_SetSpeed(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	float speed = lua_tointeger(L, -1);	
	if(speed >= 0)
	{
		pImp->_base_prop.run_speed = speed;
	}
	return 0;

}

//致命一击率 致命伤害
int 
gactive_script::script_SetCrit(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int crit_rate = lua_tointeger(L, -2);	
	float crit_damage = lua_tonumber(L, -1);	
	if(crit_rate >= 0 && crit_damage >=0 )
	{
		pImp->_base_prop.crit_rate = crit_rate;
		pImp->_base_prop.crit_damage = crit_damage;
	}
	return 0;
}

//攻击 (high, low)
int 
gactive_script::script_SetDamage(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int damage_high = lua_tointeger(L, -2);	
	int damage_low = lua_tointeger(L, -1);	
	if(damage_high >= 0 && damage_low >=0)
	{
		pImp->_base_prop.damage_high = damage_high;
		pImp->_base_prop.damage_low = damage_low;
	}
	return 0;
}

//普攻命中
int 
gactive_script::script_SetAttack(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int attack = lua_tointeger(L, -1);	
	if(attack >= 0)
	{
		pImp->_base_prop.attack = attack;
	}
	return 0;
}

//防御
int 
gactive_script::script_SetDefense(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int defense = lua_tointeger(L, -1);	
	if(defense >= 0)
	{
		pImp->_base_prop.defense = defense;
	}
	return 0;
}

//普攻防御
int 
gactive_script::script_SetArmor(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int armor= lua_tointeger(L, -1);	
	if(armor >= 0)
	{
		pImp->_base_prop.armor= armor;
	}
	return 0;
}

//6个抗性
int 
gactive_script::script_SetResistance(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 6);
	int resistance[6];
	resistance[0] = lua_tointeger(L, -6);	
	resistance[1] = lua_tointeger(L, -5);	
	resistance[2] = lua_tointeger(L, -4);	
	resistance[3] = lua_tointeger(L, -3);	
	resistance[4] = lua_tointeger(L, -2);	
	resistance[5] = lua_tointeger(L, -1);	
	if(resistance[0] >= 0 && resistance[1] >= 0 && resistance[2] >= 0 &&
	   resistance[3] >= 0 && resistance[4] >= 0 && resistance[5] >= 0)
	{
		pImp->_base_prop.resistance[0]= resistance[0];
		pImp->_base_prop.resistance[1]= resistance[1];
		pImp->_base_prop.resistance[2]= resistance[2];
		pImp->_base_prop.resistance[3]= resistance[3];
		pImp->_base_prop.resistance[4]= resistance[4];
		pImp->_base_prop.resistance[5]= resistance[5];
	}
	return 0;
}

//攻击范围
int 
gactive_script::script_SetAttackRange(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int range = lua_tointeger(L, -1);	
	if(range >= 0) 
	{
		pImp->_base_prop.attack_range = range;
	}
	return 0;
}

//减免暴击
int 
gactive_script::script_SetAntiCrit(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int anti_crit = lua_tointeger(L, -2);	
	float anti_crit_damage = lua_tointeger(L, -1);	
	if(anti_crit >= 0 && anti_crit_damage >=0 )
	{
		pImp->_base_prop.anti_crit = anti_crit;
		pImp->_base_prop.anti_crit_damage = anti_crit_damage;
	}
	return 0;
}

//技能命中和技能躲闪
int 
gactive_script::script_SetSkillRate(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int skill_attack_rate = lua_tointeger(L, -2);	
	int skill_armor_rate = lua_tointeger(L, -1);	
	if(skill_attack_rate >= 0 && skill_armor_rate >=0 )
	{
		pImp->_base_prop.skill_attack_rate = skill_attack_rate;
		pImp->_base_prop.skill_armor_rate = skill_armor_rate;
	}
	return 0;
}

//反隐能力
int 
gactive_script::script_SetAntiInvisibleRate(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int anit_invisible_rate = lua_tointeger(L, -1);	
	if(anit_invisible_rate >= 0)
	{
		pImp->_base_prop.anti_invisible_rate = anit_invisible_rate;
		((gactive_object*)pImp->_parent)->anti_invisible_rate = anit_invisible_rate;
	}
	return 0;
}



int 
gactive_script::script_AddReputation(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int index = lua_tointeger(L, -2);	
	int rep = lua_tointeger(L, -1);	
	pImp->OI_ModifyRegionReputation(index, rep);
	return 0;
}

int
gactive_script::script_AddSkillProficiency(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int id = lua_tointeger(L, -2);	
	int add = lua_tointeger(L, -1);	
	pImp->OI_AddProficiency(id, add);
	return 0;
}

int
gactive_script::script_AddTransformSkill(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int skill_id = lua_tointeger(L, -2);
	int skill_level  = lua_tointeger(L, -1);	
	pImp->OI_AddTransformSkill(skill_id, skill_level);
	return 0;
}

int
gactive_script::script_AddHP(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int hp = lua_tointeger(L, -1);
	pImp->OI_AddHP(hp);
	return 0;
}

int
gactive_script::script_AddMP(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int mp = lua_tointeger(L, -1);
	pImp->OI_AddMP(mp);
	return 0;
}

int
gactive_script::script_AddDefense(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int defense = lua_tointeger(L, -1);
	pImp->OI_AddDefense(defense);
	return 0;
}

int
gactive_script::script_AddAttack(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int attack = lua_tointeger(L, -1);
	pImp->OI_AddAttack(attack);
	return 0;
}

int
gactive_script::script_AddArmor(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int armor = lua_tointeger(L, -1);
	pImp->OI_AddArmor(armor);
	return 0;
}

int
gactive_script::script_AddDamage(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int damage = lua_tointeger(L, -1);	
	pImp->OI_AddDamage(damage);
	return 0;
}

int
gactive_script::script_AddResistance(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 6);
	int resistance[6];
	resistance[0] = lua_tointeger(L, -6);	
	resistance[1] = lua_tointeger(L, -5);	
	resistance[2] = lua_tointeger(L, -4);	
	resistance[3] = lua_tointeger(L, -3);	
	resistance[4] = lua_tointeger(L, -2);	
	resistance[5] = lua_tointeger(L, -1);	
	pImp->OI_AddResistance(resistance);
	return 0;
}

int
gactive_script::script_AddCrit(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int crit_rate = lua_tointeger(L, -2);	
	float crit_damage = lua_tonumber(L, -1);	
	pImp->OI_AddCrit(crit_rate, crit_damage);
	return 0;
}

int
gactive_script::script_AddSpeed(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 1);
	float speed = lua_tonumber(L, -1);	
	pImp->OI_AddSpeed(speed);
	return 0;
}

int
gactive_script::script_AddAntiCrit(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int anti_crit = lua_tointeger(L, -2);	
	float anti_crit_damage = lua_tonumber(L, -1);	
	pImp->OI_AddAntiCrit(anti_crit, anti_crit_damage);
	return 0;
}

int 
gactive_script::script_AddSkillRate(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 2);
	int skill_attack_rate = lua_tointeger(L, -2);	
	int skill_armor_rate = lua_tointeger(L, -1);	
	pImp->OI_AddSkillRate(skill_attack_rate, skill_armor_rate);
	return 0;
}

int
gactive_script::script_AddAttackRange(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	float range = lua_tonumber(L, -1);	
	pImp->OI_AddAttackRange(range);
	return 0;
}

int 
gactive_script::script_AddScaleHP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int hp = lua_tointeger(L, -1); 
	pImp->OI_AddScaleHP(hp);
	return 0;
}

int 
gactive_script::script_AddScaleMP(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int mp = lua_tointeger(L, -1); 
	pImp->OI_AddScaleMP(mp);
	return 0;
}

int
gactive_script::script_AddScaleDefense(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int defense = lua_tointeger(L, -1); 
	pImp->OI_AddScaleDefense(defense);
	return 0;
}

int
gactive_script::script_AddScaleAttack(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int attack = lua_tointeger(L, -1); 
	pImp->OI_AddScaleAttack(attack);
	return 0;
}

int
gactive_script::script_AddScaleArmor(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int armor = lua_tointeger(L, -1); 
	pImp->OI_AddScaleArmor(armor);
	return 0;
}

int
gactive_script::script_AddScaleDamage(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int damage = lua_tointeger(L, -1); 
	pImp->OI_AddScaleDamage(damage);
	return 0;
}


int
gactive_script::script_AddScaleSpeed(lua_State *L)
{
	RESTORE_OBJECT_DATA(L, 1);
	int speed = lua_tointeger(L, -1); 
	pImp->OI_AddScaleSpeed(speed);
	return 0;
}

int
gactive_script::script_AddScaleResistance(lua_State * L)
{
	RESTORE_OBJECT_DATA(L, 6);
	int resistance[6];
	resistance[0] = lua_tointeger(L, -6);	
	resistance[1] = lua_tointeger(L, -5);	
	resistance[2] = lua_tointeger(L, -4);	
	resistance[3] = lua_tointeger(L, -3);	
	resistance[4] = lua_tointeger(L, -2);	
	resistance[5] = lua_tointeger(L, -1);	
	pImp->OI_AddScaleResistance(resistance);
	return 0;
}

