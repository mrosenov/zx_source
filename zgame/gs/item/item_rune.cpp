#include "item_rune.h"
#include "../player_imp.h"


void
rune_essence::UpdateContent(item * parent)
{
	if(!_dirty_flag) return;
	packet_wrapper h1(256);

	//放入essence
	h1.push_back(&_enhanced_essence,sizeof(_enhanced_essence));

	//放入inner data
	size_t size = _inner_data.size() * sizeof(float);
	h1 << size;
	if(size > 0)
	{
		h1.push_back(&_inner_data[0], size);
	}
	
	//复制入输入
	parent->SetContent(h1.data(), h1.size());

	//将数据用回来
	_content= parent->GetContent(_size);

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
		rune_item * pItem = (rune_item*)GetAndTest(L,n,"__ITEM");\
		(void)(pItem);\
		rune_essence * pEss = (rune_essence*)GetAndTest(L,n,"__ESS");\
		if(!pEss) return 0;


int  
rune_essence::script_QueryLevel(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurLevel());
	return 1;
}

int  
rune_essence::script_QueryExp(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurExp());
	return 1;
}


int  
rune_essence::script_QueryInnerData(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);

	int index = lua_tointeger(L, -1);
	lua_pushnumber(L,pEss->QueryInnerData(index));
	return 1;
}

int  
rune_essence::script_GetInnerDataCount(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetInnerDataCount());
	return 1;

}

int  
rune_essence::script_SetInnerData(lua_State *L)
{
	RESTORE_RUNE_DATA(L,2);
	
	int index = lua_tointeger(L, -2);
	float value = lua_tonumber(L, -1);
	pEss->SetInnerData(index,value);
	return 0;
}

int  
rune_essence::script_ClearInnerData(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);
	
	pEss->ClearInnerDara();
	return 0;

}

int  
rune_essence::script_QueryQuality(lua_State *L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurQuality());
	return 1;
}

int  
rune_essence::script_SetQuality(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);
	
	int quality = lua_tointeger(L, -1);
	if(quality > 0 && quality <= MAX_QUALITY)
	{
		pEss->SetQuality(quality);
	}
	return 0;
}

int  
rune_essence::script_SetRefineQuality(lua_State *L)
{
	RESTORE_RUNE_DATA(L,1);
	
	int quality = lua_tointeger(L, -1);
	if(quality > 0 && quality <= MAX_QUALITY)
	{
		pEss->SetRefineQuality(quality);
	}
	return 0;
}


int
rune_essence::script_GetHole(lua_State * L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurHole());
	return 1;
}

int 
rune_essence::script_GetAttCnt(lua_State * L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetAttCnt());
	return 1;
}


int
rune_essence::script_GetAtt(lua_State * L)
{
	RESTORE_RUNE_DATA(L,1);

	int index = lua_tointeger(L, -1);
	int id = 0;
	int value = 0;
	int grade = 0;
	float grade_extra = 0.0f;

	if(index >= 0 && index < 8)
	{
		pEss->GetProp(index, id, value, grade, grade_extra);
	}
	lua_pushinteger(L, id);
	lua_pushinteger(L, value);
	lua_pushinteger(L, grade);
	lua_pushnumber(L, grade_extra);
	return 4;
}


int
rune_essence::script_SetAtt(lua_State * L)
{
	RESTORE_RUNE_DATA(L,5);
	int index = lua_tointeger(L, -5);
	int id = lua_tointeger(L, -4);
	int value = lua_tointeger(L, -3);
	int grade = lua_tointeger(L, -2);
	float grade_extra = lua_tonumber(L, -1);

	pEss->SetProp(index, id, value, grade, grade_extra);
	return 0;
}


int
rune_essence::script_SetRefineAtt(lua_State * L)
{
	RESTORE_RUNE_DATA(L,5);
	int index = lua_tointeger(L, -5);
	int id = lua_tointeger(L, -4);
	int value = lua_tointeger(L, -3);
	int grade = lua_tointeger(L, -2);
	float grade_extra = lua_tonumber(L, -1);

	pEss->SetRefineProp(index, id, value, grade, grade_extra);
	return 0;
}


int
rune_essence::script_GetRefineCnt(lua_State * L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetRefineCnt());
	return 1;
}

int
rune_essence::script_GetResetCnt(lua_State * L)
{
	RESTORE_RUNE_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetResetCnt());
	return 1;
}


int
rune_essence::script_GetAvgGrade(lua_State * L)
{
	RESTORE_RUNE_DATA(L,0);
	lua_pushinteger(L,pEss->GetAvgGrade());
	return 1;
}

int
rune_essence::script_SetAvgGrade(lua_State * L)
{
	RESTORE_RUNE_DATA(L,1);

	int grade = lua_tointeger(L, -1);
	pEss->SetAvgGrade(grade);
	return 0;
}


void
rune_item::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

void
rune_item::OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
			Deactivate(index,obj,parent);
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

bool
rune_item::VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const
{
	if(list.GetLocation() == item::BODY)
	{
		size_t len;
		const void * buf = parent->GetContent(len);
		if(len == 0) return false;
		
		rune_essence ess(buf,len);

		//判断等级
		gplayer_imp * pImp = (gplayer_imp*)obj;
		int r_level = pImp->GetRebornCount() * 150 + pImp->GetObjectLevel();

		if(r_level < _level_required + _require_reborn_count * 150) return false;
		if(r_level < player_template::GetRuneRequireLevel(ess.GetCurLevel()) + player_template::GetRuneRequireReborn(ess.GetCurLevel()) * 150) return false;
		

		//元魂装备位是否已经开启
		if(!pImp->IsRuneActive()) return false;
		
		return true; 
	}
	else
	{
		return false;
	}
}


bool
rune_item::CallScript(gactive_imp *imp, rune_essence & ess, const char * function) const
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_RUNE);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;
	gplayer_imp * pImp = (gplayer_imp*)imp;
	
	lua_getfield(L, LUA_GLOBALSINDEX, function);
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushinteger(L, pImp->GetRuneScore());

	if(lua_pcall(L, 4, 1,0))
	{
		printf("rune %d error when calling %s %s\n", _tid, function, lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("rune %d error when calling %s\n", _tid, function);
	}
	lua_pop(L,1);

	return true;
}

void 
rune_item::OnActivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp*)obj;
	OnActivateProp(index, obj, parent);
	pImp->ActiveRuneProp();
}

void 
rune_item::OnDeactivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp*)obj;
	OnDeactivateProp(index, obj, parent);
	pImp->DeactiveRuneProp();
}

void
rune_item::OnActivateProp(size_t index, gactive_imp *imp, item *parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return;

	int cur_level = ess.GetCurLevel();
	if(cur_level <= 0) return;

	//洗炼属性
	for(size_t i = 0; i < 8; ++i)
	{
		int id = 0;
		int value = 0;
		int grade = 0;
		float grade_extra = 0.0f;
		ess.GetProp(i, id, value, grade, grade_extra);

		if(id > 0 && id <= MAX_PROP_ID && value > 0)
		{
			EnhanceProp(id, value, cur_level, imp); 
		}
	}

	//符文之语
	if(ess.GetCurHole() <= 0) return;

	abase::vector<int> stone_list;
	for(int i = 0; i < ess.GetCurHole(); ++i)
	{
		int stone_id = ess.GetStoneID(i);
		if(stone_id > 0)
		{
			//不能有重复的符文, 重复的符文无法达成符文之语
			for(size_t i = 0; i < stone_list.size(); ++i)
			{
				if(stone_id == stone_list[i]) return;
			}
		       	stone_list.push_back(stone_id);
		}
	}

	if(stone_list.size() != (size_t)ess.GetCurHole()) return;

	unsigned int stone_combo_id = player_template::GetRuneComboID(stone_list);
	if(stone_combo_id > 0)
	{
		abase::vector<int> addon_list;
		player_template::GetRuneComboAddon(stone_combo_id, addon_list);

		for(size_t i = 0; i < addon_list.size(); ++i)
		{
			int id = addon_list[i];
			const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
			if(pSpec)
			{
				pSpec->handler->Activate(pSpec->data,this,imp,parent);
			}
		}	

	}

}

void
rune_item::OnDeactivateProp(size_t index, gactive_imp *imp, item *parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return;

	int cur_level = ess.GetCurLevel();
	if(cur_level <= 0) return;

	for(size_t i = 0; i < 8; ++i)
	{
		int id = 0;
		int value = 0;
		int grade = 0;
		float grade_extra = 0.0f;
		ess.GetProp(i, id, value, grade, grade_extra);

		if(id > 0 && id <= MAX_PROP_ID && value > 0)
		{
			ImpairProp(id, value, cur_level, imp); 
		}
	}
	

	//符文之语
	if(ess.GetCurHole() <= 0) return;

	abase::vector<int> stone_list;
	for(int i = 0; i < ess.GetCurHole(); ++i)
	{
		int stone_id = ess.GetStoneID(i);
		if(stone_id > 0)
		{
			//不能有重复的符文, 重复的符文无法达成符文之语
			for(size_t i = 0; i < stone_list.size(); ++i)
			{
				if(stone_id == stone_list[i]) return;
			}
		       	stone_list.push_back(stone_id);
		}
	}

	if(stone_list.size() != (size_t)ess.GetCurHole()) return;

	unsigned int stone_combo_id = player_template::GetRuneComboID(stone_list);
	if(stone_combo_id > 0)
	{
		abase::vector<int> addon_list;
		player_template::GetRuneComboAddon(stone_combo_id, addon_list);

		for(size_t i = 0; i < addon_list.size(); ++i)
		{
			int id = addon_list[i];
			const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
			if(pSpec)
			{
				pSpec->handler->Deactivate(pSpec->data,this,imp,parent);
			}
		}	

	}
}

void
rune_item::EnhanceProp(int prop_id, int value, int cur_level, gactive_imp * imp) const
{
	object_interface oif(imp);
	int total_value = value * cur_level;
	oif.EnhanceProp(prop_id, total_value);
}


void
rune_item::ImpairProp(int prop_id, int value, int cur_level, gactive_imp * imp) const
{
	object_interface oif(imp);
	int total_value = value * cur_level;
	oif.ImpairProp(prop_id, total_value);
}


int 
rune_item::GetClientSize(const void * buf, size_t len) const 
{ 
	if (len == 0)
		return 0;

	rune_essence ess(buf,len);
	if(ess.IsValid())
	{
		return sizeof(rune_enhanced_essence); 
	}
	else
	{
		return 0;
	}
}

/*void 
rune_item::GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const
{
	if(len == 0)
	{
		*data = buf;
		size = len;
	}
	else
	{
		rune_essence ess(buf,len);
		if(ess.IsValid())
		{
			rune_enhanced_essence * epEss = ess.QueryEssence();

			*data = epEss;
			size = sizeof(rune_enhanced_essence); 
		}
		else
		{
			*data = NULL;
			size = 0;
		}
	}
}*/

void 
rune_item::GetItemDataForClient(item_data_client& data, const void * buf, size_t len) const
{
	if(len == 0)
	{
		data.item_content = (char*)buf;
		data.content_length = len;
	}
	else
	{
		rune_essence ess(buf,len);
		if(ess.IsValid())
		{
			rune_enhanced_essence * epEss = ess.QueryEssence();

			data.item_content = (char*)epEss;
			data.content_length = sizeof(rune_enhanced_essence); 
		}
		else
		{
			data.item_content = NULL;
			data.content_length = 0;
		}
	}
}

bool
rune_item::Identify(item *parent, gactive_imp *imp, size_t index1, size_t index2)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);

	if(len != 0) return false;
	int assist_id = pImp->GetInventory()[index2].type;
	if(!player_template::IsRuneIdentifyItemID(assist_id)) return false;
	
	ess.SetDirty();
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_RUNE);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	lua_getfield(L, LUA_GLOBALSINDEX, "Rune_Init_Entrance");
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushinteger(L, pImp->GetRuneScore());
	lua_pushinteger(L, 0);

	if(lua_pcall(L, 5, 1,0))
	{
		printf("rune %d error when calling %s %s\n", _tid, "Rune_Init_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("rune %d error when calling %s\n", _tid, "Rune_Init_Entrance");
	}
	lua_pop(L,1);

	ess.AddExp(player_template::GetRuneIdentifyExp());
	ess.SetCurLevel(1);
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index1);
	return true;
}

bool
rune_item::Customize(item * parent, gactive_imp * imp, int count, int prop[], int index, int type)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);

	if(len != 0) return false;

	ess.SetDirty();

	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_RUNE);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	lua_getfield(L, LUA_GLOBALSINDEX, "Rune_Init_Entrance");
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushinteger(L, pImp->GetRuneScore());
	lua_pushinteger(L, type);

	if(lua_pcall(L, 5, 1,0))
	{
		printf("rune %d error when calling %s %s\n", _tid, "Rune_Init_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("rune %d error when calling %s\n", _tid, "Rune_Init_Entrance");
	}
	lua_pop(L,1);

	ess.SetCurLevel(1);

	int prop_used[8];	//该属性位置已经处理，不能再替换
	memset(prop_used, 0, sizeof(prop_used));

	int prop_id = 0;
	int prop_value = 0;
	int prop_grade = 0;
	float grade_extra = 0.0f;
	for(int i = 0; i < count; ++i)
	{
		bool is_handle = false;
		for(size_t j = 0; j < 8; ++j)
		{
			ess.GetProp(j, prop_id, prop_value, prop_grade, grade_extra);
			if(prop_id > 0 && prop_id == prop[i] && prop_used[j] == 0)
			{
				prop_used[j] = 1;
				ess.SetProp(j, prop[i], 0, 0, 0.0); //把属性重置为0
				is_handle = true;
				break;
			}
		}
		if(is_handle) continue;

		for(size_t j = 0; j < 8; ++j)
		{
			ess.GetProp(j, prop_id, prop_value, prop_grade, grade_extra);
			if(prop_id > 0 && prop_id != prop[i] && prop_used[j] == 0)
			{
				prop_used[j] = 1;
				ess.SetProp(j, prop[i], 0, 0, 0.0); //把属性重置为0
				break;
			}
		}
	}

	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
	return true;

}

bool
rune_item::Combine(item *parent, gactive_imp *imp, size_t index1, size_t index2)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len1;
	void * buf1 = parent->GetContent(len1);
	rune_essence ess1(buf1,len1);
	if(len1 == 0) return false;


	item & it2 = pImp->GetInventory()[index2];
	size_t len2;
	void * buf2 = it2.GetContent(len2);
	rune_essence ess2(buf2,len2);
	if(len2 == 0) return false;

	int level = ess2.GetCurLevel();
	int exp = ess2.GetCurExp();
	exp += player_template::GetRuneLevelExp(level);


	ess1.SetDirty();
	if(exp > 0) ess1.AddExp(exp);
	ess1.AddCashRefineCnt(ess2.GetCashRefineCnt());
	ess1.AddRefineRemain(ess2.GetCashRefineCnt());
	ess1.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index1);
	return true;
}

bool
rune_item::RefineRune(item *parent, gactive_imp *imp, size_t rune_index, int assist_index)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;

	bool no_assist = false;
	if(assist_index < 0 && assist_index != -1) return false;
	if(assist_index == -1)
	{
		if(ess.GetRefineRemain() <= 0) return false;
		no_assist = true;
	}
	else
	{
		size_t inv_size = pImp->GetInventory().Size();
		if(assist_index < 0 || (size_t)assist_index >= inv_size) return false;
		item & it2 = pImp->GetInventory()[assist_index];
		if(it2.type == -1) return false;

		int assist_id = pImp->GetInventory()[assist_index].type;
		if(!player_template::IsRuneRefineItemID(assist_id)) return false;

		no_assist = false;
	}

	ess.SetDirty();
	bool active = ess.GetRefineActive();
	if(active) 
	{
		ess.ClrRefineProp();
		ess.SetRefineActive(false);
	}
	
	bool rst = CallScript(pImp, ess, "Rune_Refine_Entrance");
	if(rst)
	{
		ess.IncRefineCnt();
		ess.SetRefineActive(true);

		if(no_assist)
		{
			ess.DecRefineRemain();
		}
		else
		{
			int assist_id = pImp->GetInventory()[assist_index].type;
			//if(assist_id == REFINE_CASH_ITEM1 || assist_id == REFINE_CASH_ITEM2)
			if(assist_id == REFINE_CASH_ITEM1) 
			{
				ess.AddCashRefineCnt(1);
			}	
			ess.AddExp(player_template::GetRuneRefineExp());
		}
	}
	
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,rune_index);
	return rst;
}

bool
rune_item::RefineAction(item *parent, gactive_imp *imp, size_t index, bool accept_result)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;

	bool active = ess.GetRefineActive();
	if(!active) return false;

	OnDeactivateProp(index, pImp, parent);
	if(accept_result)
	{
		ess.ReplaceRefineProp();
	}
	ess.SetDirty();
	ess.ClrRefineProp();
	ess.SetRefineActive(false);
	ess.UpdateContent(parent);
	OnActivateProp(index, pImp, parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index);
	property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
	if(pImp->_basic.hp > pImp->GetMaxHP()) pImp->_basic.hp = pImp->GetMaxHP();
	if(pImp->_basic.mp > pImp->GetMaxMP()) pImp->_basic.mp = pImp->GetMaxMP();
	return true;
}

bool
rune_item::Reset(item *parent, gactive_imp *imp, size_t index1, size_t index2)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;

	int assist_id = pImp->GetInventory()[index2].type;
	if(!player_template::IsRuneResetItemID(assist_id)) return false;
	
	OnDeactivateProp(index1, pImp, parent);
	ess.SetDirty();
	bool rst = CallScript(pImp, ess, "Rune_Reset_Entrance");
	if(rst)
	{
		ess.AddExp(player_template::GetRuneResetExp());
		ess.IncResetCnt();
		ess.ClrRefineProp();
		ess.SetRefineActive(false);
		ess.SetRefineRemain(ess.GetCashRefineCnt());
		ess.UpdateContent(parent);
		pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index1);

	}
	OnActivateProp(index1, pImp, parent);
	property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
	if(pImp->_basic.hp > pImp->GetMaxHP()) pImp->_basic.hp = pImp->GetMaxHP();
	if(pImp->_basic.mp > pImp->GetMaxMP()) pImp->_basic.mp = pImp->GetMaxMP();
	return rst;
}

bool
rune_item::Decompose(item *parent, gactive_imp *imp, int & output_id, size_t index) 
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;

	if(ess.GetCurLevel() < RUNE_DECOMPOSE_LEVEL) return false; 
	if(ess.GetCurExp() < RUNE_DECOMPOSE_EXP) return false;
	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_RUNE);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;
	
	lua_getfield(L, LUA_GLOBALSINDEX, "Rune_Decompose_Entrance");
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushinteger(L, pImp->GetRuneScore());

	if(lua_pcall(L, 4, 1,0))
	{
		printf("rune %d error when calling %s %s\n", _tid, "Rune_Decompose_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}

	if(!lua_isnumber(L, -1) || (output_id = lua_tointeger(L,-1)) <= 0) 
	{
		printf("rune %d error when calling %s\n", _tid, "Rune_Decompose_Entrance");
		lua_pop(L,1);
		return false;
	}
	lua_pop(L,1);
	
	ess.SetDirty();
	ess.DecExp(RUNE_DECOMPOSE_EXP);
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index);
	return true;
}

bool
rune_item::LevelUp(item *parent, gactive_imp *imp, size_t rune_index, int & level) 
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;

	int cur_level = ess.GetCurLevel();
	level = cur_level;
	if(cur_level >= MAX_RUNE_LEVEL) return false; 

	int r_level = pImp->GetRebornCount() * 150 + pImp->GetObjectLevel();
	if(r_level < _level_required + _require_reborn_count * 150) return false;
	if(r_level < player_template::GetRuneRequireLevel(cur_level+1) + player_template::GetRuneRequireReborn(cur_level+1) * 150) return false;
	
	int cur_exp = ess.GetCurExp();
	int lvlup_exp = player_template::GetRuneLevelUpExp(cur_level);

	//一次只升一级
	if(cur_exp >= lvlup_exp)
	{
		OnDeactivateProp(rune_index, pImp, parent);
		cur_exp -= lvlup_exp;
		cur_level++;

		ess.SetDirty();
		if(cur_level == RUNE_OPENSLOT_LEVEL && ess.GetCurHole() == 0)
		{
			ess.SetHole(abase::Rand(2, 5));
		}	
		ess.SetCurExp(cur_exp);
		ess.SetCurLevel(cur_level);
		ess.UpdateContent(parent);
		pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,rune_index);
		OnActivateProp(rune_index, pImp, parent);
		property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
		if(pImp->_basic.hp > pImp->GetMaxHP()) pImp->_basic.hp = pImp->GetMaxHP();
		if(pImp->_basic.mp > pImp->GetMaxMP()) pImp->_basic.mp = pImp->GetMaxMP();
	}

	level = cur_level;
	return true;
}

bool
rune_item::OpenSlot(item *parent, gactive_imp *imp, size_t index)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;

	if(ess.GetCurLevel() != MAX_RUNE_LEVEL) return false; 
	if(ess.GetCurHole() > 0) return false;

	ess.SetDirty();
	ess.SetHole(abase::Rand(2, 5));
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index);
	return true;
}

bool
rune_item::ChangeSlot(item * parent, gactive_imp * imp, size_t index1, size_t index2)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;
	
	if(ess.GetCurLevel() < RUNE_OPENSLOT_LEVEL) return false; 
	if(ess.GetCurHole() <= 0) return false;
	
	int assist_id = pImp->GetInventory()[index2].type;
	if(!player_template::IsRuneChangeSlotItemID(assist_id)) return false;

	for(size_t i = 0; i < 5; ++i)
	{
		if(ess.GetStoneID(i) != 0) return false;
	}
		
	ess.SetDirty();
	ess.SetHole(abase::Rand(2, 5));
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index1);
	return true;
}

bool
rune_item::EraseSlot(item * parent, gactive_imp * imp, int rune_index, int slot_index, int & stone_id)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;
	
	if(ess.GetCurLevel() < RUNE_OPENSLOT_LEVEL) return false; 
	if(ess.GetCurHole() <= 0) return false;
	if(slot_index < 0 || slot_index > ess.GetCurHole()) return false;

	if(ess.GetStoneID(slot_index) <= 0) return false;

	OnDeactivateProp(rune_index, pImp, parent);
	ess.SetDirty();
	stone_id = ess.GetStoneID(slot_index);
	ess.SetStoneID(slot_index, 0);
	
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,rune_index);
	OnActivateProp(rune_index, pImp, parent);
	property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
	if(pImp->_basic.hp > pImp->GetMaxHP()) pImp->_basic.hp = pImp->GetMaxHP();
	if(pImp->_basic.mp > pImp->GetMaxMP()) pImp->_basic.mp = pImp->GetMaxMP();

	return true;
}

bool
rune_item::InstallSlot(item * parent, gactive_imp * imp, int rune_index, int slot_index, int stone_id)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return false;
	
	if(ess.GetCurLevel() < RUNE_OPENSLOT_LEVEL) return false; 
	if(ess.GetCurHole() <= 0) return false;
	if(slot_index < 0 || slot_index > ess.GetCurHole()) return false;
	if(ess.GetStoneID(slot_index) > 0) return false;
	
	OnDeactivateProp(rune_index, pImp, parent);
	ess.SetDirty();
	ess.SetStoneID(slot_index, stone_id);
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,rune_index);
	OnActivateProp(rune_index, pImp, parent);
	property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
	if(pImp->_basic.hp > pImp->GetMaxHP()) pImp->_basic.hp = pImp->GetMaxHP();
	if(pImp->_basic.mp > pImp->GetMaxMP()) pImp->_basic.mp = pImp->GetMaxMP();

	return true;
}


int
rune_item::GainExp(item::LOCATION l, int exp, item * parent, gactive_imp * imp, int index, bool& level_up)  const
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	if(l != item::BODY) return -1; 
	if(index != item::EQUIP_INDEX_RUNE) return -1;
	
	size_t len;
	void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(!ess.IsValid()) return -1;

	ess.SetDirty();
	ess.AddExp(exp);
	ess.UpdateContent(parent);
	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT, index);

	return 0;
}


int64_t rune_item::GetIdModify(const item * parent) const
{
	size_t len;
	const void * buf = parent->GetContent(len);
	rune_essence ess(buf,len);
	if(len == 0) return 0;

	return ((int64_t)(ess.GetCurQuality() & 0x00FF)) << 32;
}


