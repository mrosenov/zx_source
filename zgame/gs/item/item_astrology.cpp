#include "item_astrology.h"
#include "../player_imp.h"

bool
item_astrology::AstrologyIdentify(gactive_imp * obj, item * parent, size_t item_index, int item_id)
{
	size_t len;
	parent->GetContent(len);
	if(len != 0) return false;
		
	astrology_essence ess; 
	memset(&ess, 0, sizeof(ess));
	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_ASTROLOGY);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;
	
	lua_getfield(L, LUA_GLOBALSINDEX, "Astrology_Identify_Entrance");
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushinteger(L, _reborn_count);
	lua_pushinteger(L, _equip_mask);
	
	if(lua_pcall(L, 5, 1,0))
	{
		printf("astrology %d error when calling %s %s\n", _tid, "Astrology_Identify_Entrance", lua_tostring(L,-1));
		return false;
	}
	parent->SetContent(&ess, sizeof(ess));	

	gplayer_imp * pImp = (gplayer_imp*)obj;
	size_t need_money = g_config.fee_astrology_identify; 
	if(need_money > 0)
	{
		pImp->SpendMoney(need_money);
		pImp->_runner->spend_money(need_money);
	}

	if(!pImp->IsAstrologyClientActive())
	{
		pImp->ActiveAstrologyClient();
		pImp->_runner->notify_astrology_energy();
	}

	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,item_index);
	GLog::log(LOG_INFO, "用户%d执行了星座鉴定操作, 星座id为%d", pImp->_parent->ID.id, item_id);
	return true;
}


static float upgrade_prob[10] = {1.001, 0.5, 0.35, 0.25, 0.2, 0.16, 0.12, 0.1, 0.08, 0.08};

bool
item_astrology::AstrologyUpgrade(gactive_imp * obj, item * parent, size_t item_index, int item_id, size_t stone_index, int stone_id)
{
	astrology_essence * pEss = GetDataEssence(parent);
	if(!pEss) return false;
	if(pEss->level >= MAX_ASTROLOGY_LEVEL || pEss->level < 0) return false;
	if(stone_id <= 0 || (stone_id != g_config.item_astrology_upgrade_id[0] && 
	   stone_id != g_config.item_astrology_upgrade_id[1] && stone_id != g_config.item_astrology_upgrade_id[2]) ) return false;


	gplayer_imp * pImp = (gplayer_imp*)obj;
	item_list &inv = pImp->GetInventory();

	bool success = false;
	if(abase::Rand(0.0f, 1.0f) < upgrade_prob[pEss->level])
	{
		pEss->level++;
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, item_index);
		success = true;
	}

	//模具消失	
	pImp->UseItemLog(inv[stone_index], 1);
	pImp->GetInventory().DecAmount(stone_index, 1);
	pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,stone_index,stone_id, 1 ,S2C::DROP_TYPE_USE);
	pImp->_runner->astrology_upgrade_result(success, pEss->level);
	GLog::log(LOG_INFO, "用户%d执行了星座升级操作, 结果为%d, 星座id为%d, 道具id为%d", pImp->_parent->ID.id, success,  item_id, stone_id);
	return true;
}

bool
item_astrology::AstrologyDestroy(gactive_imp * obj, item * parent, size_t item_index, int item_id)
{
	astrology_essence * pEss = GetDataEssence(parent);
	if(!pEss) return false;

	//返回的精力值等于评分*12
	int energy = pEss->score * 12;
	
	gplayer_imp * pImp = (gplayer_imp*)obj;
	item_list &inv = pImp->GetInventory();
	//星座消失
	pImp->UseItemLog(inv[item_index], 1);
	pImp->GetInventory().DecAmount(item_index, 1);
	pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,item_index,item_id, 1 ,S2C::DROP_TYPE_USE);

	pImp->GainAstrologyEnergy(energy);
	pImp->_runner->astrology_destroy_result(energy);
	GLog::log(LOG_INFO, "用户%d执行了星座粉碎操作, 星座id为%d, 返回的精力为%d", pImp->_parent->ID.id, item_id, energy);
	return true;
}



namespace
{
	void * GetLuaPointer(lua_State *L, int n, const char * name)
	{
		lua_pushstring(L,name);
		lua_gettable(L,-2 - n);
		void *p = lua_touserdata(L,-1);
		lua_pop(L,1);
		return p;
	}

	void * GetAndTest(lua_State *L ,int n, const char * name)
	{
		void *p = GetLuaPointer(L,n, name);
		if(!p) 
		{
			return NULL;
		}
		return p;
	}

	bool CheckArgNum(lua_State *L, int n)
	{
		int nArg = lua_gettop(L);    /* number of arguments */	
		if(nArg == n + 1) return true;
		printf("lua 调用参数错误\n");
		return false;
	}
}

#define RESTORE_RUNE_DATA(L,n) \
		if(!CheckArgNum(L,n)) return 0;\
		item_astrology * pItem = (item_astrology*)GetAndTest(L,n,"__ITEM");\
		(void)(pItem);\
		astrology_essence * pEss = (astrology_essence*)GetAndTest(L,n,"__ESS");\
		if(!pEss) return 0;

int  
astrology_essence::script_GetLevel(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurLevel());
	return 1;
}

int  
astrology_essence::script_SetLevel(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);
	
	int level = lua_tointeger(L, -1);
	if(level > 0 && level <= MAX_ASTROLOGY_LEVEL)
	{
		pEss->SetCurLevel(level);
	}
	return 0;
}

int  
astrology_essence::script_GetScore(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetScore());
	return 1;
}

int  
astrology_essence::script_SetScore(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);
	
	int score = lua_tointeger(L, -1);
	if(score > 0) 
	{
		pEss->SetScore(score);
	}
	return 0;
}

int  
astrology_essence::script_GetEnergyConsume(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetEnergyConsume());
	return 1;
}

int  
astrology_essence::script_SetEnergyConsume(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);
	
	int energy_consume = lua_tointeger(L, -1);
	if(energy_consume > 0) 
	{
		pEss->SetEnergyConsume(energy_consume);
	}
	return 0;
}



int
astrology_essence::script_SetProp(lua_State * L)
{
	RESTORE_RUNE_DATA(L,3);
	int index = lua_tointeger(L, -3);
	int id = lua_tointeger(L, -2);
	int value = lua_tointeger(L, -1);

	pEss->SetProp(index, id, value);
	return 0;
}

int
astrology_essence::script_GetProp(lua_State * L)
{
	RESTORE_RUNE_DATA(L,1);

	int index = lua_tointeger(L, -1);
	int id = 0;
	int value = 0;

	if(index >= 0 && index < MAX_ASTROLOGY_PROP_NUM)
	{
		pEss->GetProp(index, id, value); 
	}
	lua_pushinteger(L, id);
	lua_pushinteger(L, value);
	return 2;
}



/*****************************************************************/


int
item_astrology_energy::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp * ) imp;

	if(pImp->GetAstrologyEnergy() >= MAX_ASTROLOGY_VALUE) return -1;  

	pImp->GainAstrologyEnergy(energy);
	return 1;
}


