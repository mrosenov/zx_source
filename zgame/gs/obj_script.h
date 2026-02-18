#ifndef __ONLINEGAME_GS_PLAYER_SCRIPT_H
#define __ONLINEGAME_GS_PLAYER_SCRIPT_H

#include <lua.hpp>

class gactive_script
{
public:
	//获得当前属性
	static int script_GetLevel(lua_State * L);
	static int script_GetHP(lua_State * L);
	static int script_GetMP(lua_State * L);
	static int script_GetCurHP(lua_State * L);
	static int script_GetCurMP(lua_State * L);
	static int script_GetSpeed(lua_State *L);
	static int script_GetCrit(lua_State * L);
	static int script_GetDamage(lua_State * L);
	static int script_GetAttack(lua_State * L);
	static int script_GetDefense(lua_State * L);
	static int script_GetArmor(lua_State * L);
	static int script_GetResistance(lua_State * L);
	static int script_GetAttackRange(lua_State *L);
	static int script_GetAntiCrit(lua_State * L);
	static int script_GetSkillRate(lua_State * L);

	static int script_GetSkillLevel(lua_State * L);
	static int script_GetPosition(lua_State * L);
	static int script_GetCultivation(lua_State * L);
	static int script_GetCircleMemberCnt(lua_State * L);

	//重新设置属性	
	static int script_SetLevel(lua_State * L);
	static int script_SetHP(lua_State * L);
	static int script_SetMP(lua_State * L);
	static int script_SetCurHP(lua_State * L);
	static int script_SetCurMP(lua_State * L);
	static int script_SetSpeed(lua_State *L);
	static int script_SetCrit(lua_State * L);
	static int script_SetDamage(lua_State * L);
	static int script_SetAttack(lua_State * L);
	static int script_SetDefense(lua_State * L);
	static int script_SetArmor(lua_State * L);
	static int script_SetResistance(lua_State * L);
	static int script_SetAttackRange(lua_State *L);
	static int script_SetAntiCrit(lua_State * L);
	static int script_SetSkillRate(lua_State * L);
	static int script_SetAntiInvisibleRate(lua_State *L);


	//改变属性
	static int script_AddHP(lua_State * L);
	static int script_AddMP(lua_State * L);
	static int script_AddDefense(lua_State *L);
	static int script_AddAttack(lua_State *L);
	static int script_AddArmor(lua_State *L);
	static int script_AddDamage(lua_State *L);
	static int script_AddResistance(lua_State *L);
	static int script_AddCrit(lua_State *L);
	static int script_AddSpeed(lua_State *L);
	static int script_AddAntiCrit(lua_State *L);
	static int script_AddSkillRate(lua_State *L);
	static int script_AddAttackRange(lua_State *L);
	static int script_AddScaleHP(lua_State *L);
	static int script_AddScaleMP(lua_State *L);
	static int script_AddScaleDefense(lua_State *L);
	static int script_AddScaleAttack(lua_State *L);
	static int script_AddScaleArmor(lua_State *L);
	static int script_AddScaleDamage(lua_State *L);
	static int script_AddScaleSpeed(lua_State *L);
	static int script_AddScaleResistance(lua_State *L);
	
	//改变玩家属性
	static int script_AddReputation(lua_State * L);
	static int script_AddSkillProficiency(lua_State *L);
	static int script_AddTransformSkill(lua_State *L);

};

#endif
