#include "item_petbedge.h"
#include "../clstab.h"
#include "../player_imp.h"

void pet_bedge_essence::UpdateContent(item* parent,bool force)
{
	if(!_dirty_flag && !force) return;
	packet_wrapper h1(2048);
	//·ÅÈëessence
	h1.push_back(&_enhanced_essence,sizeof(_enhanced_essence));
	//·ÅÈëparam inner data
	size_t size = _inner_data.size() * sizeof(float);
	h1 << size;
	if(size > 0)
	{
		h1.push_back(&_inner_data[0],size);
	}
	//¸´ÖÆÈëÊäÈë
	parent->SetContent(h1.data(), h1.size());
	//½«Êı¾İÓÃ»ØÀ´
	_content= parent->GetContent(_size);
}

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
	printf("lua µ÷ÓÃ²ÎÊı´íÎó\n");
	return false;
}
}

#define RESTORE_PET_BEDGE_DATA(L,n) \
	if(!CheckArgNum(L,n)) return 0;\
	item_pet_bedge* pItem = (item_pet_bedge*)GetAndTest(L,n,"__ITEM");\
	(void)(pItem);\
	pet_bedge_essence * pEss = (pet_bedge_essence*)GetAndTest(L,n,"__ESS");\
	if(!pEss) return 0;

int pet_bedge_essence::script_QueryInnerData(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushnumber(L,pEss->QueryInnerData(index));
	return 1;
}

int pet_bedge_essence::script_GetInnerDataCount(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetInnerDataCount());
	return 1;
}

int pet_bedge_essence::script_SetInnerData(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	float value = lua_tonumber(L,-1);
	pEss->SetInnerData(index,value);
	return 0;
}

int pet_bedge_essence::script_ClearInnerData(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	pEss->ClearInnerDara();
	return 0;
}

int pet_bedge_essence::script_QueryLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetLevel());
	return 1;
}

int pet_bedge_essence::script_SetLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetLevel(value);
	return 0;
}

int pet_bedge_essence::script_QueryExp(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetExp());
	return 1;
}

int pet_bedge_essence::script_SetExp(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetExp(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurHP(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurHP());
	return 1;
}

int pet_bedge_essence::script_SetCurHP(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurHP(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurVigor(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurVigor());
	return 1;
}

int pet_bedge_essence::script_SetCurVigor(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurVigor(value);
	return 0;
}

int pet_bedge_essence::script_QueryStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetStar());
	return 1;
}

int pet_bedge_essence::script_SetStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetStar(value);
	return 0;
}

int pet_bedge_essence::script_QueryRebornStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetRebornStar());
	return 1;
}

int pet_bedge_essence::script_SetRebornStar(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetRebornStar(value);
	return 0;
}

int pet_bedge_essence::script_QueryMainType(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetMainType());
	return 1;
}

int pet_bedge_essence::script_SetMainType(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetMainType(value);
	return 0;
}

int pet_bedge_essence::script_QueryElement(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetElement());
	return 1;
}

int pet_bedge_essence::script_SetElement(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetElement(value);
	return 0;
}

int pet_bedge_essence::script_QueryIdentify(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetIdentify());
	return 1;
}

int pet_bedge_essence::script_SetIdentify(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetIdentify(value);
	return 0;
}

int pet_bedge_essence::script_QueryRace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetRace());
	return 1;
}

int pet_bedge_essence::script_SetRace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetRace(value);
	return 0;
}

int pet_bedge_essence::script_QueryHoroscope(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetHoroscope());
	return 1;
}

int pet_bedge_essence::script_SetHoroscope(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetHoroscope(value);
	return 0;
}

int pet_bedge_essence::script_QueryCombatAttrInit(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetCombatAttrInit(index));
	return 1;
}

int pet_bedge_essence::script_SetCombatAttrInit(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetCombatAttrInit(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryCombatAttrPotential(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushnumber(L,pEss->GetCombatAttrPotential(index));
	return 1;
}

int pet_bedge_essence::script_SetCombatAttrPotential(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetCombatAttrPotential(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryCombatAttrCur(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetCombatAttrCur(index));
	return 1;
}

int pet_bedge_essence::script_SetCombatAttrCur(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetCombatAttrCur(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryRebornPropAdd(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushnumber(L,pEss->GetRebornPropAdd(index));
	return 1;
}

int pet_bedge_essence::script_SetRebornPropAdd(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	float value = lua_tonumber(L,-1);
	pEss->SetRebornPropAdd(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryExtraDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetExtraDamage());
	return 1;
}

int pet_bedge_essence::script_SetExtraDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetExtraDamage(value);
	return 0;
}

int pet_bedge_essence::script_QueryExtraDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetExtraDamageReduce());
	return 1;
}

int pet_bedge_essence::script_SetExtraDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetExtraDamageReduce(value);
	return 0;
}

int pet_bedge_essence::script_QueryCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetCritRate());
	return 1;
}

int pet_bedge_essence::script_SetCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetCritRate(value);
	return 0;
}

int pet_bedge_essence::script_QueryCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetCritDamage());
	return 1;
}

int pet_bedge_essence::script_SetCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetCritDamage(value);
	return 0;
}

int pet_bedge_essence::script_QueryAntiCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetAntiCritRate());
	return 1;
}

int pet_bedge_essence::script_SetAntiCritRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetAntiCritRate(value);
	return 0;
}

int pet_bedge_essence::script_QueryAntiCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetAntiCritDamage());
	return 1;
}

int pet_bedge_essence::script_SetAntiCritDamage(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetAntiCritDamage(value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillAttackRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetSkillAttackRate());
	return 1;
}

int pet_bedge_essence::script_SetSkillAttackRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetSkillAttackRate(value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillArmorRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetSkillArmorRate());
	return 1;
}

int pet_bedge_essence::script_SetSkillArmorRate(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetSkillArmorRate(value);
	return 0;
}

int pet_bedge_essence::script_QueryDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetDamageReduce());
	return 1;
}

int pet_bedge_essence::script_SetDamageReduce(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetDamageReduce(value);
	return 0;
}

int pet_bedge_essence::script_QueryDamageReducePercent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushnumber(L,pEss->GetDamageReducePercent());
	return 1;
}

int pet_bedge_essence::script_SetDamageReducePercent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	float value = lua_tonumber(L,-1);
	pEss->SetDamageReducePercent(value);
	return 0;
}

int pet_bedge_essence::script_QueryMaxHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetMaxHungerPoint());
	return 1;
}

int pet_bedge_essence::script_SetMaxHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetMaxHungerPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurHungerPoint());
	return 1;
}

int pet_bedge_essence::script_SetCurHungerPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurHungerPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryMaxHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetMaxHonorPoint());
	return 1;
}

int pet_bedge_essence::script_SetMaxHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetMaxHonorPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryCurHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurHonorPoint());
	return 1;
}

int pet_bedge_essence::script_SetCurHonorPoint(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCurHonorPoint(value);
	return 0;
}

int pet_bedge_essence::script_QueryAge(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetAge());
	return 1;
}

int pet_bedge_essence::script_SetAge(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetAge(value);
	return 0;
}

int pet_bedge_essence::script_QueryLife(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetLife());
	return 1;
}

int pet_bedge_essence::script_SetLife(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetLife(value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillID(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetSkillID(index));
	return 1;
}

int pet_bedge_essence::script_SetSkillID(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetSkillID(index,value);
	return 0;
}

int pet_bedge_essence::script_QuerySkillLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetSkillLevel(index));
	return 1;
}

int pet_bedge_essence::script_SetSkillLevel(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetSkillLevel(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryTalent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int index = lua_tointeger(L,-1);
	lua_pushinteger(L,pEss->GetTalent(index));
	return 1;
}

int pet_bedge_essence::script_SetTalent(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,2);
	int index = lua_tointeger(L,-2);
	int value = lua_tointeger(L,-1);
	pEss->SetTalent(index,value);
	return 0;
}

int pet_bedge_essence::script_QueryAcuity(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetAcuity());
	return 1;
}

int pet_bedge_essence::script_SetAcuity(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetAcuity(value);
	return 0;
}

int pet_bedge_essence::script_QueryStamina(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetStamina());
	return 1;
}

int pet_bedge_essence::script_SetStamina(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetStamina(value);
	return 0;
}

int pet_bedge_essence::script_QuerySawy(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetSawy());
	return 1;
}

int pet_bedge_essence::script_SetSawy(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetSawy(value);
	return 0;
}

int pet_bedge_essence::script_QueryDowry(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetDowry());
	return 1;
}

int pet_bedge_essence::script_SetDowry(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetDowry(value);
	return 0;
}

int pet_bedge_essence::script_QueryWisdom(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetWisdom());
	return 1;
}

int pet_bedge_essence::script_SetWisdom(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetWisdom(value);
	return 0;
}

int pet_bedge_essence::script_QueryCNR(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetCNR());
	return 1;
}

int pet_bedge_essence::script_SetCNR(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetCNR(value);
	return 0;
}

int pet_bedge_essence::script_QueryFace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetFace());
	return 1;
}

int pet_bedge_essence::script_SetFace(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetFace(value);
	return 0;
}

int pet_bedge_essence::script_QueryShape(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetShape());
	return 1;
}

int pet_bedge_essence::script_SetShape(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetShape(value);
	return 0;
}

int pet_bedge_essence::script_QueryRebornCnt(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,0);
	lua_pushinteger(L,pEss->GetRebornCnt());
	return 1;
}

int pet_bedge_essence::script_SetRebornCnt(lua_State* L)
{
	RESTORE_PET_BEDGE_DATA(L,1);
	int value = lua_tointeger(L,-1);
	pEss->SetRebornCnt(value);
	return 0;
}

void item_pet_bedge::InitFromShop(gplayer_imp* pImp,item* parent,int origin) const
{
	size_t len;
	void* buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	ess.PrepareData();
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í InitFromShop NULL == L(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í InitFromShop NULL == L(_tid: %d)\n",_tid);
		return;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Init_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushinteger(L,origin);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,pImp->_basic.level);
	__PRINTF("level : %d\n",pImp->_basic.level);
	if(lua_pcall(L,5,1,0))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í InitFromShop lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í InitFromShop lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return;
	}
	if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í InitFromShop ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í InitFromShop ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)\n",_tid);
		lua_pop(L,1);
		return;
	}
	int ret = lua_tointeger(L,-1);
	lua_pop(L,1);
	if(_tid != ret)
	{
		GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(Éú³É): ³èÎïÅÆÉú³ÉÊ§°Ü(_tid: %d,pet: %d)",_tid,ret);
		__PRINTF("³èÎïĞÅÏ¢(Éú³É): ³èÎïÅÆÉú³ÉÊ§°Ü(_tid: %d,pet: %d)\n",_tid,ret);
		return;
	}
	if(ess.GetCurHP() <= 0) ess.SetCurHP(1);
	if(ess.GetCurHP() > ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP))
	{
		ess.SetCurHP(ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP));
	}
	ess.SetOrigin(origin);
	ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
	ess.UpdateContent(parent,true);
}

int item_pet_bedge::AdoptPet(gplayer_imp* imp,int inv_index)
{
	if(inv_index < 0 || (size_t)inv_index >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	/*
	if(ess.GetLevel() > imp->GetRebornCount() * 30 + imp->_basic.level)
	{
		//¼¶±ğ²»·ûºÏ
		return S2C::ERR_PET_LEVEL_TO_HIGH;
	}
	*/
	if(imp->_parent->ID.id == ess.GetOwnerID())
	{
		//ÒÑ¾­ÁìÑø¹ıÁË
		return S2C::ERR_PET_ALREADY_ADOPTED;
	}
	//½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ÁìÑø) Ö´ĞĞ½Å±¾³ö´í AdoptPet NULL == L(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(ÁìÑø) Ö´ĞĞ½Å±¾³ö´í AdoptPet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Adopt_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushinteger(L,ess.GetOrigin());
	lua_pushlightuserdata(L,&ess);
	if(lua_pcall(L,4,1,0))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ÁìÑø) Ö´ĞĞ½Å±¾³ö´í AdoptPet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("³èÎïĞÅÏ¢(ÁìÑø) Ö´ĞĞ½Å±¾³ö´í AdoptPet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || lua_tointeger(L,-1) == 0)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ÁìÑø) Ö´ĞĞ½Å±¾³ö´í AdoptPet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(ÁìÑø) Ö´ĞĞ½Å±¾³ö´í AdoptPet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)\n",_tid);
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	int ret = lua_tointeger(L,-1);
	lua_pop(L,1);
	if(_tid != ret)
	{
		GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(Éú³É): ³èÎïÅÆÁìÑøÊ§°Ü(_tid: %d,pet: %d)",_tid,ret);
		__PRINTF("³èÎïĞÅÏ¢(ÁìÑø): ³èÎïÅÆÁìÑøÊ§°Ü(_tid: %d,pet: %d)\n",_tid,ret);
		return S2C::ERR_FATAL_ERR;
	}
	//¿ÛÇ®
	imp->DecMoneyAmount((size_t)g_config.fee_adopt_pet);
	if (g_config.fee_adopt_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_adopt_pet,it.type);
	}
	//ÉèÖÃ³õÊ¼Ç×ÃÜ¶È
	//¸ÄÖ÷ÈËid
	ess.SetOwnerID(((gplayer*)(imp->_parent))->ID.id);
	ess.SetRank(0);
	ess.SetMainStatus(pet_bedge_enhanced_essence::PET_MAIN_STATUS_REST);
	const void* pname = imp->GetPlayerName(len);
	if(len > pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH) len = pet_bedge_enhanced_essence::MAX_OWNER_NAME_LENGTH;
	ess.SetOwnerName(pname,len);
	ess.UpdateContent(&it);
	//Í¨Öª¿Í»§¶ËĞÂµÄÊôĞÔ
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index);
	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(ÁìÑø): Íæ¼Ò %d Íê³ÉÁË³èÎïÁìÑø²Ù×÷(pet_tid: %d)",imp->_parent->ID.id,it.type);
	__PRINTF("³èÎïĞÅÏ¢(ÁìÑø): Íæ¼Ò %d Íê³ÉÁË³èÎïÁìÑø²Ù×÷(pet_tid: %d)\n",imp->_parent->ID.id,it.type);

	// ¸üĞÂ³èÎï²Ù×÷»îÔ¾¶È
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

int item_pet_bedge::FreePet(gplayer_imp* imp,int inv_index)
{
	if(inv_index < 0 || (size_t)inv_index >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//³èÎïÎ´ÁìÑø²»¿ÉÒÔ²Ù×÷
		return S2C::ERR_PET_NOT_ADOPTED;
	}
	if(0 == imp->GetInventory().GetEmptySlotCount())
	{
		//ÕâÀïĞèÒª¿´ÏÂ°ü¹üÓĞÃ»ÓĞ¿Õ¼ä
		return S2C::ERR_INVENTORY_IS_FULL;
	}
	//ÕâÀïĞèÒªµ÷ÓÃ½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(·ÅÉú) Ö´ĞĞ½Å±¾³ö´í FreePet NULL == L(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(·ÅÉú) Ö´ĞĞ½Å±¾³ö´í FreePet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Free_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	if(lua_pcall(L,3,5,0))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(·ÅÉú) Ö´ĞĞ½Å±¾³ö´í FreePet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("³èÎïĞÅÏ¢(·ÅÉú) Ö´ĞĞ½Å±¾³ö´í FreePet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3) || !lua_isnumber(L,-4) || !lua_isnumber(L,-5))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í FreePet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(Éú³É) Ö´ĞĞ½Å±¾³ö´í FreePet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)\n",_tid);
		lua_pop(L,5);
		return S2C::ERR_FATAL_ERR;
	}
	//È¡µÃ·µ»ØÖµ
	int item_count = lua_tointeger(L,-1);
	int item_tid = lua_tointeger(L,-2);
	int msg_id = lua_tointeger(L,-3);
	int channel_id = lua_tointeger(L,-4);
	int pet_tid = lua_tointeger(L,-5);
	lua_pop(L,5);
	if(pet_tid != _tid)
	{
		__PRINTF("³èÎïĞÅÏ¢(·ÅÉú) Ê§°Ü(pet_tid: %d,channel_id: %d,msg_id: %d,item_tid: %d,item_count: %d)",
			pet_tid,channel_id,msg_id,item_tid,item_count);
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return 0;
	}
	if(0 == item_count || 0 == item_tid)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(·ÅÉú) Ê§°Ü(pet_tid: %d,channel_id: %d,msg_id: %d,item_tid: %d,item_count: %d)",
			pet_tid,channel_id,msg_id,item_tid,item_count);
		__PRINTF("³èÎïĞÅÏ¢(·ÅÉú) Ê§°Ü(pet_tid: %d,channel_id: %d,msg_id: %d,item_tid: %d,item_count: %d)\n",
			pet_tid,channel_id,msg_id,item_tid,item_count);
		return S2C::ERR_FATAL_ERR;
	}
	//ÅĞ¶Ïitem_tidÊÇ²»ÊÇ³èÎïÁ¶»¯µ¤Ò©ÀàĞÍ
	DATA_TYPE dt2;
	const PET_REFINE_ESSENCE& ess2 = *(const PET_REFINE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(item_tid,ID_SPACE_ESSENCE,dt2);
	if(DT_PET_REFINE_ESSENCE != dt2 || NULL == &ess2)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(·ÅÉú): Íæ¼Ò %d ·ÅÉútid: %d,»ñµÃÎïÆ·tid: %d ´íÎó,²»ÊÇ³èÎïÁ¶»¯µ¤Ò©ÀàĞÍ",imp->_parent->ID.id,pet_tid,item_tid);
		__PRINTF("³èÎïĞÅÏ¢(·ÅÉú): Íæ¼Ò %d ·ÅÉútid: %d,»ñµÃÎïÆ·tid: %d ´íÎó,²»ÊÇ³èÎïÁ¶»¯µ¤Ò©ÀàĞÍ\n",imp->_parent->ID.id,pet_tid,item_tid);
		return S2C::ERR_FATAL_ERR;
	}
	//¿ÛÇ®
	imp->DecMoneyAmount((size_t)g_config.fee_free_pet);
	if (g_config.fee_free_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_free_pet,it.type);
	}
	//ÈÓ¶«Î÷
	imp->UseItemLog(it,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index,pet_tid,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index,1);
	//¸ø¶«Î÷
	const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_tid);
	if(!pItem || pItem->pile_limit <= 0) return S2C::ERR_FATAL_ERR;
	item_data * data = DupeItem(*pItem);
	if((size_t)item_count > data->pile_limit) item_count = data->pile_limit;
	data->count = item_count;
	if(imp->ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(·ÅÉú): Íæ¼Ò %d Íê³ÉÁË³èÎï·ÅÉú²Ù×÷(pet_tid: %d,item_tid: %d,item_count: %d)",
		imp->_parent->ID.id,it.type,item_tid,item_count);
	__PRINTF("³èÎïĞÅÏ¢(·ÅÉú): Íæ¼Ò %d Íê³ÉÁË³èÎï·ÅÉú²Ù×÷(pet_tid: %d,item_tid: %d,item_count: %d)\n",
		imp->_parent->ID.id,it.type,item_tid,item_count);
	//³É¹¦ÁËÒ²Òª¸æËß¿Í»§¶Ë¸öĞÅÏ¢
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);

	// ¸üĞÂ³èÎï²Ù×÷»îÔ¾¶È
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

int item_pet_bedge::RefinePet(gplayer_imp* imp,int inv_index_pet,int inv_index_c,int inv_index_a)
{
	if(inv_index_pet < 0 || (size_t)inv_index_pet >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	if(inv_index_c < 0 || (size_t)inv_index_c >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index_pet];
	bool is_lock = it.IsLocked();
	//ÕâÀïÅĞ¶Ïpet_tid;
	DATA_TYPE dt2;
	const PET_BEDGE_ESSENCE& ess2 = *(const PET_BEDGE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt2);
	if(DT_PET_BEDGE_ESSENCE != dt2 || NULL == &ess2)
	{
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d pet_tid: %d ÀàĞÍ´íÎó,²»ÊÇ³èÎïÅÆÀàĞÍ\n",imp->_parent->ID.id,it.type);
		return S2C::ERR_FATAL_ERR;
	}
	int pet_tid_in = it.type;
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//³èÎïÎ´ÁìÑø²»¿ÉÒÔ²Ù×÷
		return S2C::ERR_PET_NOT_ADOPTED;
	}
	item& it_c = imp->GetInventory()[inv_index_c];
	//ÕâÀïÅĞ¶Ïcid
	DATA_TYPE dt3;
	const PET_REFINE_ESSENCE& ess3 = *(const PET_REFINE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it_c.type,ID_SPACE_ESSENCE,dt3);
	if(DT_PET_REFINE_ESSENCE != dt3 || NULL == &ess3)
	{
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d cid: %d ÀàĞÍ´íÎó,²»ÊÇ³èÎïÁ¶»¯µ¤Ò©ÀàĞÍ\n",imp->_parent->ID.id,it_c.type);
		return S2C::ERR_FATAL_ERR;
	}
	int cid = it_c.type;
	//Èç¹ûÓĞaidÅĞ¶Ïaid
	int aid = 0;
	if(-1 != inv_index_a)
	{
		if(inv_index_a < 0 || (size_t)inv_index_a >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
		item& it_a = imp->GetInventory()[inv_index_a];
		DATA_TYPE dt4;
		const PET_ASSIST_REFINE_ESSENCE& ess4 = *(const PET_ASSIST_REFINE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it_a.type,ID_SPACE_ESSENCE,dt4);
		if(DT_PET_ASSIST_REFINE_ESSENCE != dt4 || NULL == &ess4)
		{
			__PRINTF("³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d aid: %d ÀàĞÍ´íÎó,²»ÊÇ³èÎïĞŞÁ¶¸¨ÖúµÀ¾ßÍ\n",imp->_parent->ID.id,it_a.type);
			return S2C::ERR_FATAL_ERR;
		}
		aid = it_a.type;
	}
	pet_bedge_essence output(NULL,0);
	output.PrepareData();
	output.SetOwnerID(ess.GetOwnerID());
	size_t name_len;
	const void* name_buf = ess.GetName(name_len);
	output.SetName(name_buf,name_len);
	name_buf = ess.GetOwnerName(name_len);
	output.SetOwnerName(name_buf,name_len);
	//ÕâÀïĞèÒªµ÷ÓÃ½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet NULL == L(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Combine_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushinteger(L,cid);
	lua_pushinteger(L,aid);
	lua_pushlightuserdata(L,&ess);
	lua_pushlightuserdata(L,&output);
	lua_pushinteger(L,imp->GetCultivation());
	if(lua_pcall(L,6,3,0))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢() Ö´ĞĞ½Å±¾³ö´í RefinePet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)\n",_tid);
		lua_pop(L,3);
		return S2C::ERR_FATAL_ERR;
	}
	
	object_interface oif(imp);
	imp->GetAchievementManager().OnRefinePet(oif, ess.GetStar());

	//È¡µÃ·µ»ØÖµ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid_out = lua_tointeger(L,-3);
	lua_pop(L,3);
	//¼ì²âĞÂpetÉú³É½á¹û
	item outitem;
	if(pet_tid_out > 0)
	{
		const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(pet_tid_out);
		if(!pItem) return S2C::ERR_FATAL_ERR;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem,*pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem,*pItem);
		}
	        DATA_TYPE dt2;
	        const void* no_use = gmatrix::GetDataMan().get_data_ptr(pet_tid_out,ID_SPACE_ESSENCE,dt2);
		if(DT_PET_BEDGE_ESSENCE!= dt2 || NULL == no_use)
		{
			//Éú³ÉµÄÎïÆ·²»ÊÇ³èÎïÅÆ×Ó ÌØÊâ´¦Àí
		}
		else
		{
			if(output.IsDirty())
			{
				if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
				//¸üĞÂÎïÆ·
				output.SetOrigin(ITEM_INIT_TYPE_COMBINE);
				if(ess.GetCurHP() <= 0) ess.SetCurHP(1);
				if(ess.GetCurHP() > ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP))
				{
					ess.SetCurHP(ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP));
				}
				output.UpdateContent(&outitem);
			}
			else
			{
				//Èç¹ûÊÇpetbedge¾Í±¨´í,ÒòÎªÃ»ÓĞĞ´ÈëÎïÆ·(½Å±¾Î´½øĞĞÈÎºÎ²Ù×÷)
				if(outitem.GetItemType() == item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
			}
		}
	}
	else
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return 0;
	}
	//¿ÛÇ®
	imp->DecMoneyAmount((size_t)g_config.fee_refine_pet);
	if (g_config.fee_refine_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_refine_pet,it.type);
	}
	//¿Û¶«Î÷
	imp->UseItemLog(it,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_pet,it.type,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index_pet,1);
	//¼ì²éÊÇ·ñĞèÒª¼ÇÂ¼Ïû·ÑÖµ
	imp->CheckSpecialConsumption(it_c.type, 1);
	imp->UseItemLog(it_c,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_c,cid,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index_c,1);
	if(-1 != inv_index_a)
	{
		item& it_a = imp->GetInventory()[inv_index_a];
		imp->UseItemLog(it_a,1);
		imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_a,aid,1,S2C::DROP_TYPE_USE);
		imp->GetInventory().DecAmount(inv_index_a,1);
	}
	//ÕâÀïÔö¼ÓĞÂ¶«Î÷
	if(-1 != outitem.type)
	{
		//½«ĞÂÉú³ÉµÄ³èÎïÅÆ·ÅÔÚÔ­À´µÄµØ·½·½±ã¿Í»§¶Ë×öÁ¬Ğø²Ù×÷
//		int rst = imp->GetInventory().Push(outitem);
		imp->GetInventory().Put(inv_index_pet, outitem);
		if(is_lock)
		{
			imp->GetInventory()[inv_index_pet].Lock();
		}
		int state = item::Proctype2State(outitem.proc_type);
		imp->_runner->put_item(pet_tid_out,inv_index_pet,0,1,imp->GetInventory()[inv_index_pet].count,gplayer_imp::IL_INVENTORY,state);
	}
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	//Í¨Öª¿Í»§¶ËĞÂµÄÊôĞÔ
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index_pet);


	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d Íê³ÉÁË³èÎïÁ¶»¯²Ù×÷(pet_tid_in: %d,cid: %d,aid: %d,pet_tid_out: %d,channel_id: %d,msg_id: %d",
		imp->_parent->ID.id,pet_tid_in,cid,aid,pet_tid_out,channel_id,msg_id);
	__PRINTF("³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d Íê³ÉÁË³èÎïÁ¶»¯²Ù×÷(pet_tid_in: %d,cid: %d,aid: %d,pet_tid_out: %d,channel_id: %d,msg_id: %d\n",
		imp->_parent->ID.id,pet_tid_in,cid,aid,pet_tid_out,channel_id,msg_id);

	// ¸üĞÂ³èÎï²Ù×÷»îÔ¾¶È
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

//Ö±½ÓÊ¹ÓÃÏÉ¶¹ĞŞÁ¶ (refinepetÊÇÊ¹ÓÃÏÉ¶¹¶Ò»»³ÉÓñ)
int item_pet_bedge::RefinePet2(gplayer_imp* imp,int inv_index_pet,int type)
{
	if(inv_index_pet < 0 || (size_t)inv_index_pet >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	if(type < 0 || type >= 36) return S2C::ERR_FATAL_ERR;

	item& it = imp->GetInventory()[inv_index_pet];
	bool is_lock = it.IsLocked();
	//ÕâÀïÅĞ¶Ïpet_tid;
	DATA_TYPE dt2;
	const PET_BEDGE_ESSENCE& ess2 = *(const PET_BEDGE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt2);
	if(DT_PET_BEDGE_ESSENCE != dt2 || NULL == &ess2)
	{
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d pet_tid: %d ÀàĞÍ´íÎó,²»ÊÇ³èÎïÅÆÀàĞÍ\n",imp->_parent->ID.id,it.type);
		return S2C::ERR_FATAL_ERR;
	}
	int pet_tid_in = it.type;
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("³èÎïĞÅÏ¢: ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//³èÎïÎ´ÁìÑø²»¿ÉÒÔ²Ù×÷
		return S2C::ERR_PET_NOT_ADOPTED;
	}

	int cid = 0;
	int aid = 0;	//Ö±½ÓÊ¹ÓÃÏÉ¶¹µÄ»°aidÎª0
	int need_count = 0;
	if(!GetRefineInfoByType(type, cid, need_count)) return S2C::ERR_FATAL_ERR;
	if(imp->GetInventory().CountItemByID(18794) < need_count) return S2C::ERR_FATAL_ERR;

	pet_bedge_essence output(NULL,0);
	output.PrepareData();
	output.SetOwnerID(ess.GetOwnerID());
	size_t name_len;
	const void* name_buf = ess.GetName(name_len);
	output.SetName(name_buf,name_len);
	name_buf = ess.GetOwnerName(name_len);
	output.SetOwnerName(name_buf,name_len);
	//ÕâÀïĞèÒªµ÷ÓÃ½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet NULL == L(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet NULL == L(_tid: %d)\n",_tid);
		return S2C::ERR_FATAL_ERR;
	}
	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Combine_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushinteger(L,cid);
	lua_pushinteger(L,aid);
	lua_pushlightuserdata(L,&ess);
	lua_pushlightuserdata(L,&output);
	lua_pushinteger(L,imp->GetCultivation());
	if(lua_pcall(L,6,3,0))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet lua_pcall failed(_tid: %d),when calling %s",_tid,lua_tostring(L,-1));
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet lua_pcall failed(_tid: %d),when calling %s\n",_tid,lua_tostring(L,-1));
		lua_pop(L,1);
		return S2C::ERR_FATAL_ERR;
	}
	if(!lua_isnumber(L,-1) || !lua_isnumber(L,-2) || !lua_isnumber(L,-3))
	{
		GLog::log(LOG_ERR,"³èÎïĞÅÏ¢() Ö´ĞĞ½Å±¾³ö´í RefinePet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)",_tid);
		__PRINTF("³èÎïĞÅÏ¢(ºÏ³É) Ö´ĞĞ½Å±¾³ö´í RefinePet ·µ»ØÖµÊı¾İ´íÎó(_tid: %d)\n",_tid);
		if(type < 0 || type >= 36) return S2C::ERR_FATAL_ERR;

		lua_pop(L,3);
		return S2C::ERR_FATAL_ERR;
	}
	
	object_interface oif(imp);
	imp->GetAchievementManager().OnRefinePet(oif, ess.GetStar());

	//È¡µÃ·µ»ØÖµ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid_out = lua_tointeger(L,-3);
	lua_pop(L,3);
	//¼ì²âĞÂpetÉú³É½á¹û
	item outitem;
	if(pet_tid_out > 0)
	{
		const item_data* pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(pet_tid_out);
		if(!pItem) return S2C::ERR_FATAL_ERR;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem,*pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem,*pItem);
		}
	        DATA_TYPE dt2;
	        const void* no_use = gmatrix::GetDataMan().get_data_ptr(pet_tid_out,ID_SPACE_ESSENCE,dt2);
		if(DT_PET_BEDGE_ESSENCE!= dt2 || NULL == no_use)
		{
			//Éú³ÉµÄÎïÆ·²»ÊÇ³èÎïÅÆ×Ó ÌØÊâ´¦Àí
		}
		else
		{
			if(output.IsDirty())
			{
				if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
				//¸üĞÂÎïÆ·
				output.SetOrigin(ITEM_INIT_TYPE_COMBINE);
				if(ess.GetCurHP() <= 0) ess.SetCurHP(1);
				if(ess.GetCurHP() > ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP))
				{
					ess.SetCurHP(ess.GetCombatAttrInit(pet_bedge_enhanced_essence::MAX_HP));
				}
				output.UpdateContent(&outitem);
			}
			else
			{
				//Èç¹ûÊÇpetbedge¾Í±¨´í,ÒòÎªÃ»ÓĞĞ´ÈëÎïÆ·(½Å±¾Î´½øĞĞÈÎºÎ²Ù×÷)
				if(outitem.GetItemType() == item_body::ITEM_TYPE_PET_BEDGE)
				{
					outitem.Release();
					return S2C::ERR_FATAL_ERR;
				}
			}
		}
	}
	else
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return 0;
	}
	//¿ÛÇ®
	imp->DecMoneyAmount((size_t)g_config.fee_refine_pet);
	if (g_config.fee_refine_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_refine_pet,it.type);
	}
	//¿Û¶«Î÷
	imp->UseItemLog(it,1);
	imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index_pet,it.type,1,S2C::DROP_TYPE_USE);
	imp->GetInventory().DecAmount(inv_index_pet,1);

	imp->TakeOutItem(18794, need_count);

	//ÕâÀïÔö¼ÓĞÂ¶«Î÷
	if(-1 != outitem.type)
	{
		//½«ĞÂÉú³ÉµÄ³èÎïÅÆ·ÅÔÚÔ­À´µÄµØ·½·½±ã¿Í»§¶Ë×öÁ¬Ğø²Ù×÷
//		int rst = imp->GetInventory().Push(outitem);
		imp->GetInventory().Put(inv_index_pet, outitem);
		if(is_lock)
		{
			imp->GetInventory()[inv_index_pet].Lock();
		}
		int state = item::Proctype2State(outitem.proc_type);
		imp->_runner->put_item(pet_tid_out,inv_index_pet,0,1,imp->GetInventory()[inv_index_pet].count,gplayer_imp::IL_INVENTORY,state);
	}
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	//Í¨Öª¿Í»§¶ËĞÂµÄÊôĞÔ
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index_pet);


	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(ºÏ³É): Íæ¼Ò %d Íê³ÉÁË³èÎïÁ¶»¯²Ù×÷(pet_tid_in: %d,cid: %d,aid: %d,pet_tid_out: %d,channel_id: %d,msg_id: %d, type: %d",
		imp->_parent->ID.id,pet_tid_in,cid,aid,pet_tid_out,channel_id,msg_id,type);

	// ¸üĞÂ³èÎï²Ù×÷»îÔ¾¶È
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);

	return 0;
}

int item_pet_bedge::RenamePet(gplayer_imp* imp,int inv_index,const char* name_buf,int name_size)
{
	if(inv_index < 0 || (size_t)inv_index >=imp->GetInventory().Size()) return S2C::ERR_FATAL_ERR;
	item& it = imp->GetInventory()[inv_index];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid())
	{
		GLog::log(LOG_INFO,"ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		__PRINTF("ÓÃ»§ %d ³èÎïÅÆÎïÆ·Êı¾İ´íÎó item:%d,GUID(%d,%d)",
				imp->_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);
		return S2C::ERR_INVALID_ITEM;
	}
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID())
	{
		//³èÎïÎ´ÁìÑø²»¿ÉÒÔ¸ÄÃû×Ö
		return S2C::ERR_PET_NOT_ADOPTED;
	}
	//¿ÛÇ®
	imp->DecMoneyAmount((size_t)g_config.fee_rename_pet);
	if (g_config.fee_rename_pet)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=12:hint=%d",imp->GetParent()->ID.id,imp->GetUserID(),g_config.fee_rename_pet,it.type);
	}
	//¸ÄÃû×Ö
	ess.SetName(name_buf,name_size);
	ess.UpdateContent(&it);
	//Í¨Öª¿Í»§¶ËĞÂµÄÊôĞÔ
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,inv_index);
	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(¸ÄÃû): Íæ¼Ò %d Íê³ÉÁË³èÎï¸ÄÃû²Ù×÷(pet_tid: %d)",imp->_parent->ID.id,it.type);
	__PRINTF("³èÎïĞÅÏ¢(¸ÄÃû): Íæ¼Ò %d Íê³ÉÁË³èÎï¸ÄÃû²Ù×÷(pet_tid: %d)\n",imp->_parent->ID.id,it.type);

	// ¸üĞÂ³èÎï²Ù×÷»îÔ¾¶È
	imp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CHONGWU_XIULIAN);
	return 0;
}

bool item_pet_bedge::RebornPet(gplayer_imp* imp, size_t inv_index_pet)
{
	item& it = imp->GetInventory()[inv_index_pet];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid()) return false;
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID()) return false;
	if(ess.GetRebornCnt() > 0) return false;
	if(ess.GetStar() < 12) return false;
	if(ess.GetLevel() < 150) return false;

	//ÕâÀïĞèÒªµ÷ÓÃ½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false; 
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_Reborn_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	if(lua_pcall(L,3,3,0))
	{
		lua_pop(L,1);
		return false; 
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3)) 
	{
		lua_pop(L,3);
		return false; 
	}

	//È¡µÃ·µ»ØÖµ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid = lua_tointeger(L,-3);

	lua_pop(L,3);
	if(pet_tid != _tid)
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return false;
	}
	ess.SetRebornCnt(1);
	ess.UpdateContent(&it);

	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(·ÉÉı) ú): Íæ¼Ò %d Íê³ÉÁË·ÉÉı²Ù×÷, pet_id=%d", imp->_parent->ID.id,it.type);
	//³É¹¦ÁËÒ²Òª¸æËß¿Í»§¶Ë¸öĞÅÏ¢
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, inv_index_pet);
	return true;
}

bool item_pet_bedge::ChangeShape(gplayer_imp* imp, size_t inv_index_pet, int shape_id)
{
	item& it = imp->GetPetBedgeInventory()[inv_index_pet];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid()) return false;
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID()) return false;
	if(ess.GetRebornCnt() <= 0) return false;

	//ÕâÀïĞèÒªµ÷ÓÃ½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false; 
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_ChangeShape_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,shape_id);
	if(lua_pcall(L,4,3,0))
	{
		lua_pop(L,1);
		return false; 
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3)) 
	{
		lua_pop(L,3);
		return false; 
	}

	//È¡µÃ·µ»ØÖµ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid = lua_tointeger(L,-3);

	lua_pop(L,3);
	if(pet_tid != _tid)
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return false;
	}
	ess.UpdateContent(&it);

	GLog::log(LOG_INFO,"Íæ¼Ò%d¸Ä±ä³èÎïÍâĞÎ³É¹¦: pet_id=%d, shape_id=%d", imp->_parent->ID.id, it.type, shape_id);
	//³É¹¦ÁËÒ²Òª¸æËß¿Í»§¶Ë¸öĞÅÏ¢
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	imp->PlayerGetItemInfo(gplayer_imp::IL_PET_BEDGE, inv_index_pet);
	return true;
}

bool item_pet_bedge::RefineAttr(gplayer_imp* imp, size_t inv_index_pet, size_t attr_type, size_t assist_id, size_t assist_count) 
{
	item& it = imp->GetInventory()[inv_index_pet];
	size_t len;
	void * buf = it.GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(!ess.IsValid()) return false;
	ess.PrepareData();
	if(imp->_parent->ID.id != ess.GetOwnerID()) return false;
	if(ess.GetRebornCnt() <= 0) return false;

	//ÕâÀïĞèÒªµ÷ÓÃ½Å±¾
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(),LUA_ENV_PET_BEDGE);
	lua_State* L = keeper.GetState();
	if(NULL == L)
	{
		return false; 
	}

	lua_getfield(L,LUA_GLOBALSINDEX,"PetBedge_RefineAttr_Entrance");
	lua_pushinteger(L,_tid);
	lua_pushlightuserdata(L,(void*)this);
	lua_pushlightuserdata(L,&ess);
	lua_pushinteger(L,ess.GetOrigin());
	lua_pushinteger(L,imp->_basic.level);
	lua_pushinteger(L,attr_type);
	lua_pushinteger(L,assist_id);
	lua_pushinteger(L,assist_count);
	if(lua_pcall(L,8,3,0))
	{
		lua_pop(L,1);
		return false; 
	}

	if(!lua_isnumber(L,-1) || !lua_isnumber(L, -2) || !lua_isnumber(L, -3)) 
	{
		lua_pop(L,3);
		return false; 
	}

	//È¡µÃ·µ»ØÖµ
	int msg_id = lua_tointeger(L,-1);
	int channel_id = lua_tointeger(L,-2);
	int pet_tid = lua_tointeger(L,-3);

	lua_pop(L,3);
	if(pet_tid != _tid)
	{
		imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
		return false;
	}
	ess.UpdateContent(&it);

	GLog::log(LOG_INFO,"³èÎïĞÅÏ¢(ÊôĞÔÏ´Á·) Íæ¼Ò%dÍê³ÉÊôĞÔÏ´Á·²Ù×÷ pid=%d, attr_type=%d, assist_id=%d, assist_num=%d", imp->_parent->ID.id,it.type, attr_type, assist_id, assist_count);
	//³É¹¦ÁËÒ²Òª¸æËß¿Í»§¶Ë¸öĞÅÏ¢
	imp->_runner->script_message(imp->_parent->ID.id,0,channel_id,msg_id);
	imp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, inv_index_pet);
	return true;
}

void item_pet_bedge::GetItemDataForClient(item_data_client& data,const void* buf,size_t len) const
{
	if(len == 0)
	{
		data.item_content = (char*)buf;
		data.content_length = len;
	}
	else
	{
		pet_bedge_essence ess(buf,len);
		if(ess.IsValid())
		{
			ess.PrepareData();
			//Ã»ÓĞ¼ø¶¨µÄ³èÎïÊı¾İ×ö¼Ù
			ess.MakeFakeData();
			pet_bedge_enhanced_essence * epEss = ess.QueryEssence();
			data.item_content = (char*)epEss;
			data.content_length = sizeof(pet_bedge_enhanced_essence);
		}
		else
		{
			data.item_content = NULL;
			data.content_length = 0;
		}
	}
}

bool item_pet_bedge::GetRefineInfoByType(int type, int & c_id, int & count)
{
	if(type < 0 || type >= 36) return false;

	static int refine_table[][2] = {{16030, 1}, {16031, 2}, {16032, 5}, {16050, 1}, {16051, 2}, {16052, 5}, {16070, 1}, {16071, 2}, {16072, 5},
		{16090, 1}, {16091, 2}, {16092, 5}, {16110, 1}, {16111, 2}, {16112, 5}, {16130, 1}, {16131, 1}, {16132, 2}, {16150, 1}, {16151, 1},
		{16152, 2}, {16170, 1}, {16171, 1}, {16172, 2}, {16190, 1}, {16191, 1}, {16192, 2}, {16250, 1}, {16251, 1}, {16252, 2},
		{16210, 1}, {16211, 1}, {16212, 2}, {16230, 1}, {16231, 1}, {16232, 2}};

	c_id = refine_table[type][0];
	count = refine_table[type][1];
	return true;
}

void item_pet_bedge::TryConvertData(item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != 0 && len > sizeof(int))
	{
		size_t magic = *(size_t*)buf;
		if(magic != pet_bedge_enhanced_essence::PET_MAGIC_NUMBER)
		{
			pet_bedge_essence ess;
			if(ess.ConvertData(buf, len))
			{
				ess.UpdateContent(parent,true);
			}
		}
	}
}


void item_pet_bedge::OnInit(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	TryConvertData(parent);

}


void item_pet_bedge::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	TryConvertData(parent);

	size_t len;
	void * buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(ess.IsValid())
	{
		ess.PrepareData();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(ess.GetOwnerID() == pImp->_parent->ID.id)
		{
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			size_t ess_name_len;
			if(memcmp(ess.GetOwnerName(ess_name_len), name, MAX_USERNAME_LENGTH) != 0)
			{
				OnChangeOwnerName(item::BODY, index, obj, parent);
			}
		}
	}
}

void item_pet_bedge::OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(ess.IsValid())
	{
		ess.PrepareData();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(ess.GetOwnerID() == pImp->_parent->ID.id)
		{
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			if(name_len > MAX_USERNAME_LENGTH) name_len = MAX_USERNAME_LENGTH;
			ess.SetOwnerName(name,name_len);
			ess.UpdateContent(parent,true);
			pImp->PlayerGetItemInfo(l,index);
		}
	}
}


void item_pet_bedge::OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	pet_bedge_essence ess(buf,len);
	if(ess.IsValid())
	{
		ess.PrepareData();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		ess.SetOwnerID(pImp->_parent->ID.id);
		ess.UpdateContent(parent,true);
		pImp->PlayerGetItemInfo(l,index);
	}
}


