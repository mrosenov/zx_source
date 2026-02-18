#include "item_talisman.h"
#include "../clstab.h"
#include "../actobject.h"
#include "../item_list.h"
#include "../item_manager.h"
#include "../player_imp.h"
#include "item_equip.h"
#include "../topic_site.h"

void 
talisman_essence::UpdateContent(item * parent)
{
	if(!_dirty_flag) return;
	packet_wrapper h1(256);

	//放入essence
	h1.push_back(&_enhanced_essence,sizeof(_enhanced_essence));

	//放入addon
	size_t size = _addon_data.size() * sizeof(int);
	h1 << size;
	if(size > 0)
	{
		h1.push_back(&_addon_data[0], size);
	}

	//放入param inner data
	size = _inner_data.size() * sizeof(float);
	h1 << size;
	if(size > 0)
	{
		h1.push_back(&_inner_data[0], size);
	}
	
	//放入param inner visible data 
	size = _inner_data_visible.size() * sizeof(float);
	h1 << size;
	if(size > 0)
	{
		h1.push_back(&_inner_data_visible[0], size);
	}

	//复制入输入
	parent->SetContent(h1.data(), h1.size());

	//将数据用回来
	_content= parent->GetContent(_size);

}

// Youshuang add
bool talisman_essence::PackInnerData( packet_wrapper& ar, const char* pbuf, size_t inner_cnt, size_t start, size_t end )
{
	if(inner_cnt >= start)
	{
		if(inner_cnt <= end)
		{
			end = inner_cnt - 1;
		}
		ar << end - start + 1;
		for(size_t i = start; i <= end; i ++)
		{
			float value = *(float*)(pbuf + sizeof(size_t) + i * sizeof(float));
			ar << value;
		}
		return true;
	}
	return false;
}
// end

void talisman_essence::GetClientData(packet_wrapper& ar)
{
	if(!_content_valid)
	{
		return;
	}
	const char* pbuf = (const char*)_content;
	//放入essence
	ar.push_back(pbuf,sizeof(_enhanced_essence));
	pbuf += sizeof(_enhanced_essence);

	//放入addon
	size_t size = *(size_t*)(pbuf);
	pbuf += sizeof(size_t);
	ar << size;
	if(size > 0)
	{
		ar.push_back(pbuf, size);
	}
	pbuf += size;
	
	size = *(size_t*)(pbuf); 
	size_t inner_cnt = size / sizeof(float);

	if( !PackInnerData( ar, pbuf, inner_cnt, 96, 119 ) )
	{
		ar << (size_t)0;
		return;
	}
	PackInnerData( ar, pbuf, inner_cnt, 121, 127 );
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

#define RESTORE_TALISMAN_DATA(L,n) \
		if(!CheckArgNum(L,n)) return 0;\
		talisman_item * pItem = (talisman_item*)GetAndTest(L,n,"__ITEM");\
		(void)(pItem);\
		talisman_essence * pEss = (talisman_essence*)GetAndTest(L,n,"__ESS");\
		if(!pEss) return 0;

int  
talisman_essence::script_QueryLevel(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurLevel());
	return 1;
}

int  
talisman_essence::script_SetLevel(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	
	int level = lua_tointeger(L, -1);
	if(level > 0 && level < 200)
	{
		pEss->SetLevel(level);
	}
	return 0;
}

int  
talisman_essence::script_QueryExp(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurExp());
	return 1;
}

int  
talisman_essence::script_SetExp(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	
	int exp = lua_tointeger(L, -1);
	if(exp > 0)
	{
		pEss->SetExp(exp);
	}
	return 0;
}

int  
talisman_essence::script_QueryInnerData(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,2);

	int space = lua_tointeger(L, -2);	
	int index = lua_tointeger(L, -1);
	lua_pushnumber(L,pEss->QueryInnerData(space, index));
	return 1;
}

int  
talisman_essence::script_GetInnerDataCount(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	
	int space = lua_tointeger(L, -1);	
	lua_pushinteger(L,pEss->GetInnerDataCount(space));
	return 1;

}

int  
talisman_essence::script_SetInnerData(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,3);
	
	int space= lua_tointeger(L, -3);
	int index = lua_tointeger(L, -2);
	float value = lua_tonumber(L, -1);
	pEss->SetInnerData(space, index,value);
	return 0;
}

int  
talisman_essence::script_ClearInnerData(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	
	int space= lua_tointeger(L, -1);
	pEss->ClearInnerDara(space);
	return 0;

}

int  
talisman_essence::script_AddAddon(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	
	int addon = lua_tointeger(L, -1);
	pEss->AddAddon(addon);
	return 0;

}

int  
talisman_essence::script_SetQuality(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetQuality(value);
	return 0;
}

int  
talisman_essence::script_SetStamina(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetStamina(value);
	return 0;
}

int  
talisman_essence::script_SetHP(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetHP(value);
	return 0;
}

int  
talisman_essence::script_SetMP(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetMP(value);
	return 0;

}

int  
talisman_essence::script_SetDamage(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,2);
	int low = lua_tointeger(L, -2);
	int high = lua_tointeger(L, -1);
	pEss->SetDamage(low,high);
	return 0;

}

int  
talisman_essence::script_SetAttackEnhance(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetAttackEnhance(value);
	return 0;

}

int  
talisman_essence::script_SetResistance(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,2);
	size_t index = lua_tointeger(L, -2);
	int value = lua_tointeger(L, -1);
	pEss->SetResistance(index, value);
	return 0;

}

int  
talisman_essence::script_SetMPEnhance(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetMPEnhance(value);
	return 0;

}

int  
talisman_essence::script_SetHPEnhance(lua_State *L)
{
	RESTORE_TALISMAN_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetHPEnhance(value);
	return 0;

}

void
talisman_item::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
			//      Activate(index,obj);  现在不作了，在外面统一扫描
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};

	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		talisman_enhanced_essence * epEss = ess.QueryEssence();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		//修改由于改名可能导致的名字没有更新
		if(epEss->master_id == obj->_parent->ID.id)
		{
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			if(memcmp(epEss->name, name, MAX_USERNAME_LENGTH) != 0)
			{
				OnChangeOwnerName(item::BODY, index, obj, parent);
			}
				
		}	
	}

}

void
talisman_item::OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const
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
talisman_item::VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const
{
	if(list.GetLocation() == item::BODY)
	{
		bool base = obj->GetObjectLevel() >= _level_required
			&& object_base_info::CheckCls(obj->GetObjectClass(), _class_required, _class_required1)
			&& ( (obj->IsObjectFemale()?0x02:0x01) & _require_gender);
		if(!base) return false;

		//判断转生
		if(_require_reborn_count > 0 )
		{
			gplayer_imp * pImp = (gplayer_imp *)obj;
			int reborn_count = pImp->GetRebornCount();
			if(_require_reborn_count > reborn_count ) return false;

			ASSERT(reborn_count <= MAX_REBORN_COUNT);
			/*for(int i = 0; i < reborn_count; i ++)
			{
				if(!object_base_info::CheckCls(pImp->GetRebornProf(i), _require_reborn_prof[i], _require_reborn_prof1[i])) return false;
			}*/
		}

		
		size_t len;
		const void * buf = parent->GetContent(len);
		if(len < sizeof(int)) return false;
		int id = *(int*)buf;
		return id ==0 || id == obj->_parent->ID.id;
	
	}
	else
	{
		return false;
	}
}

bool
talisman_item::CallScript(talisman_essence & ess, const char * function,int value) const
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;
	
	lua_getfield(L, LUA_GLOBALSINDEX, "Talisman_Entrance");
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushstring(L,function);
	lua_pushinteger(L, value);

	if(lua_pcall(L, 5, 1,0))
	{
		printf("talisman %d error when calling %s %s\n", _tid, function, lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("talisman %d error when calling %s\n", _tid, function);
	}
	lua_pop(L,1);

	return true;
}

void 
talisman_item::InitFromShop(gplayer_imp* pImp,item * parent, int value) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);

	ess.PrepareForScript();
	ess.InnerEssence().level = _init_level;
	ess.SetDirty();
	CallScript(ess, "Init",value);
	ess.UpdateContent(parent);
	return ;
}

void 
talisman_item::OnActivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp= (gplayer_imp *)obj;
	if(!_is_aircraft && index == item::EQUIP_INDEX_TALISMAN1)
	{
		size_t len;
		void * buf = parent->GetContent(len);
		talisman_essence ess(buf,len);
		if(ess.IsValid())
		{
			talisman_enhanced_essence * epEss = ess.QueryEssence();
			pImp->SetTalismanInfo(epEss->stamina, _energy_recover_speed,_energy_recover_factor, _energy_drop_speed);
		}
	}
	else if (_is_aircraft && index == item::EQUIP_INDEX_WING)
	{
		pImp->ActiveAircraft(_fly_speed, _fly_cost, _fly_exp_add);
	}

	OnActivateProp(index, obj, parent);

}

void
talisman_item::OnActivateProp(size_t index, gactive_imp *obj, item *parent) const
{
	gplayer_imp * pImp= (gplayer_imp *)obj;
	for(size_t i = 0;i < _addon_list.size(); i ++)
	{
		const addon_data_spec & spec = _addon_list[i];
		spec.handler->Activate(spec.data,this, obj, parent);
	}

	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		//增强本体
		talisman_enhanced_essence * epEss = ess.QueryEssence();
		
		if(epEss->master_id == 0)
		{
			epEss->master_id = obj->_parent->ID.id;
			size_t len;
			const void * name = pImp->GetPlayerName(len);
			if(len > 20) len = 20;
			memcpy(epEss->name, name, len);

		//	pImp->PlayerGetItemInfo(gplayer_imp::IL_EQUIPMENT,index);
		}
		
		obj->TalismanItemEnhance(*epEss);
		obj->ResistanceEnhance(epEss->resistance);


		//增强附加属性
		size_t addon_count;
		const int * addon_list = ess.QueryAddon(&addon_count);
		for(size_t i =0; i < addon_count; i ++)
		{
			int id = addon_list[i];
			const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
			if(pSpec)
			{
				pSpec->handler->Activate(pSpec->data,this,obj,parent);
			}
		}

		const float* new_addon_list = ess.QueryNewAddon(&addon_count);
		for(size_t i = 0; i < addon_count; i ++)
		{
			int id = (int)new_addon_list[i];
			const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
			if(pSpec)
			{
				pSpec->handler->Activate(pSpec->data,this,obj,parent);
			}	
		}
	}
}

void 
talisman_item::OnDeactivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp= (gplayer_imp *)obj;
	if(!_is_aircraft && index == item::EQUIP_INDEX_TALISMAN1)
	{
		pImp->ClrTalismanInfo();
	}
	else if (_is_aircraft && index == item::EQUIP_INDEX_WING)
	{
		pImp->DisableAircraft();
		pImp->_filters.RemoveFilter(FILTER_INDEX_AIRCRAFT);

		if(_change_color)
		{
			pImp->PlayerClearWingColor();
		}
	}
	OnDeactivateProp(index, obj, parent);
}


void
talisman_item::OnDeactivateProp(size_t index, gactive_imp *obj, item *parent) const
{
	for(size_t i = 0;i < _addon_list.size(); i ++)
	{
		const addon_data_spec & spec = _addon_list[i];
		spec.handler->Deactivate(spec.data,this,obj,parent);
	}

	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		//增强本体
		talisman_enhanced_essence * epEss = ess.QueryEssence();
		obj->TalismanItemImpair(*epEss);
		obj->ResistanceImpair(epEss->resistance);


		//增强附加属性
		size_t addon_count;
		const int * addon_list = ess.QueryAddon(&addon_count);
		for(size_t i =0; i < addon_count; i ++)
		{
			int id = addon_list[i];
			const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
			if(pSpec)
			{
				pSpec->handler->Deactivate(pSpec->data,this,obj,parent);
			}
		}

		const float* new_addon_list = ess.QueryNewAddon(&addon_count);
		for(size_t i = 0; i < addon_count; i ++)
		{
			int id = (int)new_addon_list[i];
			const addon_data_spec *pSpec  = addon_data_man::Instance().GetAddon(id);
			if(pSpec)
			{
				pSpec->handler->Deactivate(pSpec->data,this,obj,parent);
			}	
		}
	}
}

int 
talisman_item::GetClientSize(const void * buf, size_t len) const 
{ 
	if(len < 0)
	{
		return 0;
	}
	return len;
	/*if (len == 0)
		return 0;

	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		size_t addon_count;
		ess.QueryAddon(&addon_count);

		return sizeof(talisman_enhanced_essence) + sizeof(size_t) + addon_count*sizeof(int) + (119-96+1) * sizeof(float);
	}
	else
	{
		return 0;
	}*/
}

/*void 
talisman_item::GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const
{
	if(len == 0)
	{
		*data = buf;
		size = len;
	}
	else
	{
		talisman_essence ess(buf,len);
		if(ess.IsValid())
		{
			packet_wrapper ar;
			ess.GetClientData(ar);
			size = ar.size();
			*data = ar.data();
			printf("\n");
			//talisman_enhanced_essence * epEss = ess.QueryEssence();
			//size_t addon_count;
			//ess.QueryAddon(&addon_count);

			// *data = epEss;
			//size = sizeof(talisman_enhanced_essence) + sizeof(size_t) + addon_count*sizeof(int);
		}
		else
		{
			*data = NULL;
			size = 0;
		}
	}
}*/

void 
talisman_item::GetItemDataForClient(item_data_client& data, const void * buf, size_t len) const
{
	if(len == 0)
	{
		data.item_content = (char*)buf;
		data.content_length = len;
	}
	else
	{
		talisman_essence ess(buf,len);
		if(ess.IsValid())
		{
			data.use_wrapper = true;

			ess.GetClientData(data.ar);
			/*talisman_enhanced_essence * epEss = ess.QueryEssence();
			size_t addon_count;
			ess.QueryAddon(&addon_count);

			*data = epEss;
			size = sizeof(talisman_enhanced_essence) + sizeof(size_t) + addon_count*sizeof(int);*/
		}
		else
		{
			data.item_content = NULL;
			data.content_length = 0;
		}
	}
}

int 
talisman_item::GainExp(item::LOCATION l, int exp, item * parent, gactive_imp * obj, int index, bool & level_up) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(!ess.IsValid()) return -1;

	talisman_enhanced_essence * epEss = ess.QueryEssence();
	int eff_level = _max_level2;
	if(epEss->master_id != obj->_parent->ID.id) return -1;
	if(epEss->level >=  GetMaxCurLevel(obj)) return -1;	//人物级别要求不能获得再多经验了
	if(epEss->level >= eff_level) return -1;			//达到了最大级别
	gplayer_imp * pImp = (gplayer_imp*)obj;
	int max_exp = player_template::GetTalismanLvlupExp(epEss->level);
	int new_exp = epEss->exp + exp;

	while(new_exp >= max_exp)
	{
		int level = ++ (ess.QueryEssence()->level);
		level_up = true;

		GLog::log( LOG_INFO, "用户%d获得法宝经验成功升级法宝, item_id=%d, cur_level=%d", pImp->_parent->ID.id, _tid, level); 

		//升级先让装备失效
		if(l == item::BODY && (index == item::EQUIP_INDEX_WING || index == item::EQUIP_INDEX_TALISMAN1 ))
		{
			OnDeactivateProp(index, obj, parent);
		}

		//调用升级脚本
		ess.PrepareForScript();
		CallScript(ess, "LevelUp");
		ess.UpdateContent(parent);
		//重新加上装备
		if(l == item::BODY && (index == item::EQUIP_INDEX_WING || index == item::EQUIP_INDEX_TALISMAN1 ))
		{
			OnActivateProp(index, obj, parent);
		} 

		if(level >= GetMaxCurLevel(obj) || level >= eff_level) 
		{
			//达到最大级别了
			new_exp = 0;
			break;
		}

		//尚未到最大级别
		new_exp -= max_exp;
		max_exp = player_template::GetTalismanLvlupExp(level);
		
	}
	ess.QueryEssence()->exp = new_exp;

	if(!_is_aircraft && index == item::EQUIP_INDEX_TALISMAN1 && level_up && epEss != NULL)
	{
		pImp->SetTalismanMaxStamina(epEss->stamina);
	}
	return new_exp;
}
	


bool
talisman_item::LevelUp(item * parent, gactive_imp * pImp) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(!ess.IsValid()) return false;

	talisman_enhanced_essence * epEss = ess.QueryEssence();
	if(epEss->level >= GetMaxCurLevel(pImp)) return false;	//人物级别要求不能获得再多经验了
	if(epEss->level >= _max_level2) return false;	//达到了最大级别

	int max_exp = player_template::GetTalismanLvlupExp(epEss->level);
	if(epEss->exp >= max_exp)
	{
		//达到了升级要求 进行升级操作
		epEss->exp = 0;
		epEss->level ++;
		//调用升级脚本
		ess.PrepareForScript();
		CallScript(ess, "LevelUp");
		ess.UpdateContent(parent);

		GLog::log( LOG_INFO, "用户%d执行了法宝升级操作, item_id=%d, cur_level=%d", pImp->_parent->ID.id, _tid, epEss->level); 
	}
	return true;
}

bool
talisman_item::ItemReset(item * parent, gactive_imp * pImp) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(!ess.IsValid()) return false;

	talisman_enhanced_essence * epEss = ess.QueryEssence();
	if(epEss->master_id == 0) return false;	//不能重复洗
	if(epEss->master_id == pImp->_parent->ID.id) return false;	//自己不应该洗

	//洗法宝
	epEss->exp = 0;
	epEss->level = 1;
	epEss->master_id = 0;
	memset(epEss->name, 0 , sizeof(epEss->name));
	//调用升级脚本
	ess.PrepareForScript();
	CallScript(ess, "Reset");
	ess.UpdateContent(parent);
	return true;
}

bool talisman_item::Enchant( gplayer_imp * pImp, size_t index1, size_t index2 )
{
	// 法宝灌魔
	item_list &inv = pImp->GetInventory();
	item &it1 = inv[index1];
	item &it2 = inv[index2];
	ASSERT(it1.GetItemType() == ITEM_TYPE_TALISMAN);
	size_t len;
	void * buf;
	buf = it1.GetContent( len );
	talisman_essence ess1( buf,len );
	if( !ess1.IsValid() ) return false;
	talisman_essence output( NULL, 0 );
	ess1.PrepareForScript();
	output.PrepareForScript();
	talisman_enhanced_essence & oess = output.InnerEssence(); //这是 _content的副本，由于 output没有content
								//加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝
						//注意可能会在内部修改法宝的等级
	output.InnerEssence().exp = 0;		// 经验同level规则,初始为0

	bool is_locked = it1.IsLocked();
	int expire_time = it1.expire_date;
	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper( *gmatrix::Instance(), LUA_ENV_TALISMAN );
	lua_State * L = keeper.GetState();
	if( NULL == L ) return false;
	lua_getfield( L, LUA_GLOBALSINDEX, "Talisman_Enchant_Entrance" );
	lua_pushinteger(L, it1.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushinteger(L, it2.type);
	lua_pushlightuserdata(L, &output);
	lua_pushboolean(L, is_locked);
	
	if( lua_pcall( L, 5, 1, 0 ) )
	{
		printf( "enchant talisman error when calling %s %s\n", "Talisman_Enchant_Entrance", lua_tostring( L, -1 ) );
		lua_pop( L, 1 );
		return false;
	}
	int output_id = 0;
	if( !lua_isnumber( L, -1 ) || ( output_id = lua_tointeger( L, -1 ) ) == -1 ) 
	{
		lua_pop( L, 1 );
		printf( "enchant talisman error return \n");
		return false;
	}
	lua_pop( L, 1 );
	//脚本调用结束，判断返回值
	item outitem;
	if( !GenNewTalisman( output, output_id, outitem ) ){ return false; }
	
	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(it1,1);
	pImp->UseItemLog(it2,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=it1.type, type2=it2.type;
	pImp->_runner->player_drop_item(where,index1, type1, 1, S2C::DROP_TYPE_USE);
	pImp->_runner->player_drop_item(where,index2, type2, 1, S2C::DROP_TYPE_USE);
	inv.DecAmount( index1, 1 );
	inv.DecAmount( index2, 1 );
	//检查是否需要记录消费值
	pImp->CheckSpecialConsumption(type2, 1);
	if(outitem.type != -1)
	{
		if(is_locked) outitem.Lock();
		outitem.expire_date = expire_time;

		//将新生成的法宝放入原来的位置方便连续操作
		pImp->GetInventory().Put(index1, outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->put_item(output_id,index1,expire_time,1,pImp->GetInventory()[index1].count,where, state);
	}
	GLog::log( LOG_INFO, "用户%d精炼灌魔了法宝(%d,%d),使用灵媒(%d,%d) 产出了 %d", pImp->_parent->ID.id, index1, type1, index2, type2, output_id );

	if( type2 == 62752 || type2 == 62754 )
	{
		pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index1);
		pImp->_runner->send_talisman_enchant_addon( output_id, index1 );
	}
	return true;
}

// Youshuang add
bool talisman_item::GenNewTalisman( talisman_essence& output, int output_id, item& outitem )
{
	if( output_id > 0 )
	{
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
		if( !pItem ) return false;
		if( ( pItem->proc_type & item::ITEM_PROC_TYPE_GUID ) && pItem->pile_limit == 1 )
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem, *pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem, *pItem);
		}
		if(output.IsDirty())
		{
			if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
			//更新物品
			output.UpdateContent(&outitem);
		}
		else
		{
			//如果是法宝就报错，因为没有写入物品
			if(outitem.GetItemType() == item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
		}
	}
	return true;
}

bool talisman_item::ConfirmEnchant( gplayer_imp* pImp, size_t index1, int confirm )
{
	// 法宝灌魔
	item_list &inv = pImp->GetInventory();
	item &it1 = inv[index1];
	ASSERT(it1.GetItemType() == ITEM_TYPE_TALISMAN);
	size_t len;
	void * buf;
	buf = it1.GetContent( len );
	talisman_essence ess1( buf,len );
	if( !ess1.IsValid() ) return false;
	ess1.PrepareForScript();

	talisman_essence output( NULL, 0 );
	output.PrepareForScript();
	talisman_enhanced_essence& oess = output.InnerEssence(); //这是 _content的副本，由于 output没有content 加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝 注意可能会在内部修改法宝的等级
	output.InnerEssence().exp = 0;		// 经验同level规则,初始为0
	bool is_locked = it1.IsLocked();
	int expire_time = it1.expire_date;
	
	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper( *gmatrix::Instance(), LUA_ENV_TALISMAN );
	lua_State * L = keeper.GetState();
	if( NULL == L ) return false;
	lua_getfield( L, LUA_GLOBALSINDEX, "Talisman_Enchant_Result_Entrance" );
	lua_pushinteger(L, it1.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushinteger(L, confirm);
	lua_pushlightuserdata(L, &output);
	if( lua_pcall( L, 4, 1, 0 ) )
	{
		printf( "enchant talisman error when calling %s %s\n", "Talisman_Enchant_Entrance", lua_tostring( L, -1 ) );
		lua_pop( L, 1 );
		return false;
	}
	int output_id = 0;
	if( !lua_isnumber( L, -1 ) || ( output_id = lua_tointeger( L, -1 ) ) == -1 ) 
	{
		lua_pop( L, 1 );
		printf( "enchant talisman error return \n");
		return false;
	}
	lua_pop( L, 1 );
	//脚本调用结束，判断返回值

	item outitem;
	if( !GenNewTalisman( output, output_id, outitem ) ){ return false; }
	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(it1,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=it1.type;
	pImp->_runner->player_drop_item(where,index1, type1, 1, S2C::DROP_TYPE_USE);
	inv.DecAmount( index1, 1 );
	if(outitem.type != -1)
	{
		if(is_locked) outitem.Lock();
		outitem.expire_date = expire_time;

		//将新生成的法宝放入原来的位置方便连续操作
		pImp->GetInventory().Put(index1, outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->put_item(output_id,index1,expire_time,1,pImp->GetInventory()[index1].count,where, state);
	}

	GLog::log( LOG_INFO, "用户%d精炼灌魔了法宝(%d,%d),结果%d 产出了 %d", pImp->_parent->ID.id, index1, type1, confirm, output_id );
	return true;
}
// end

bool 
talisman_item::Combine(gplayer_imp * pImp, size_t index1, size_t index2, int cid)
{
	item_list &inv = pImp->GetInventory();
	item &i1 = inv[index1];
	item &i2 = inv[index2];
	ASSERT(i1.GetItemType() == ITEM_TYPE_TALISMAN && i2.GetItemType() == ITEM_TYPE_TALISMAN);
	
	size_t len;
	void * buf;
	
	buf= i1.GetContent(len);
	talisman_essence ess1(buf,len);

	buf = i2.GetContent(len);
	talisman_essence ess2(buf,len);

	if(!ess1.IsValid() || !ess2.IsValid()) return false;

	talisman_essence output(NULL,0);

	ess1.PrepareForScript();
	ess2.PrepareForScript();
	output.PrepareForScript();


	talisman_enhanced_essence & oess = output.InnerEssence();	//这是 _content的副本，由于 output没有content
									//加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝
						//注意可能会在内部修改法宝的等级

	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	bool i1_locked = i1.IsLocked();	
	bool i2_locked = i2.IsLocked();	
	int expire_time = i1.expire_date;

	lua_getfield(L, LUA_GLOBALSINDEX, "Talisman_Combine_Entrance");
	lua_pushinteger(L, i1.type);
	lua_pushinteger(L, i2.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushlightuserdata(L, &ess2);
	lua_pushinteger(L, cid);
	lua_pushlightuserdata(L, &output);
	lua_pushinteger(L, pImp->GetTalismanValue());
	lua_pushboolean(L, i1_locked);
	lua_pushboolean(L, i2_locked);

	if(lua_pcall(L, 9, 2,0))
	{
		printf("combine talisman error when calling %s %s\n", "Talisman_Combine_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	int output_id = 0;
	int talisman_value = 0;
	if(!lua_isnumber(L, -2) || (output_id = lua_tointeger(L,-2)) == -1
			|| !lua_isnumber(L, -1) || (talisman_value = lua_tointeger(L,-1)) < 0) 
	{
		lua_pop(L,2);
		printf("combine talisman error return \n");
		return false;
	}
	lua_pop(L,2);

	//脚本调用结束，判断返回值
	item outitem;
	if(output_id > 0)
	{
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
		if(!pItem) return false;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem, *pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem, *pItem);
		}
		if(output.IsDirty())
		{
			if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}

			//更新物品
			output.UpdateContent(&outitem);
		}
		else
		{
			//如果是法宝就报错，因为没有写入物品
			if(outitem.GetItemType() == item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
		}
	}

	//设置熔炼修正值
	pImp->SetTalismanValue(talisman_value);

	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(i1,1);
	pImp->UseItemLog(i2,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=i1.type, type2=i2.type;
	pImp->_runner->player_drop_item(where,index1, type1, i1.count,S2C::DROP_TYPE_USE);
	pImp->_runner->player_drop_item(where,index2, type2, i2.count,S2C::DROP_TYPE_USE);
	inv.Remove(index1);
	inv.Remove(index2);
	//检查是否需要记录消费值
	pImp->CheckSpecialConsumption(type2, 1);


	if(outitem.type != -1)
	{
		if(i1_locked) outitem.Lock();
		outitem.expire_date = expire_time;
		int rst = inv.Push(outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,where,rst,state);
	}
	pImp->_runner->talisman_combine(type1,type2,output_id>0?output_id:0);
	GLog::log(LOG_INFO,"用户%d精炼血炼了法宝(%d,%d) (%d,%d) 产出了 %d", pImp->_parent->ID.id, index1, type1, index2,type2,output_id);
	return true;
}

bool talisman_item::HolyLevelup(gplayer_imp * pImp, size_t index1, int cid)
{
	item_list &inv = pImp->GetInventory();
	item &i1 = inv[index1];
	ASSERT(i1.GetItemType() == ITEM_TYPE_TALISMAN);
	
	size_t len;
	void * buf;
	
	buf= i1.GetContent(len);
	talisman_essence ess1(buf,len);

	if(!ess1.IsValid()) return false;

	talisman_essence output(NULL,0);

	ess1.PrepareForScript();
	output.PrepareForScript();

	talisman_enhanced_essence & oess = output.InnerEssence();	//这是 _content的副本，由于 output没有content
																//加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝
										//注意可能会在内部修改法宝的等级
	bool i1_locked = i1.IsLocked();	

	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	int expire_time = i1.expire_date;

	lua_getfield(L, LUA_GLOBALSINDEX, "Talisman_HolyLevelup_Entrance");
	lua_pushinteger(L, i1.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushinteger(L, cid);
	lua_pushlightuserdata(L, &output);

	if(lua_pcall(L,4,1,0))
	{
		printf("Holy levelup talisman error when calling %s %s\n", "Talisman_HolyLevelup_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	int output_id = 0;
	if(!lua_isnumber(L, -1) || (output_id = lua_tointeger(L,-1)) == -1)
	{
		lua_pop(L,1);
		printf("Holy levelup talis error return \n");
		return false;
	}
	lua_pop(L,1);

	//脚本调用结束，判断返回值
	item outitem;
	if(output_id > 0)
	{
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
		if(!pItem) return false;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem, *pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem, *pItem);
		}
		if(output.IsDirty())
		{
			if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}

			//更新物品
			output.UpdateContent(&outitem);
		}
		else
		{
			//如果是法宝就报错，因为没有写入物品
			if(outitem.GetItemType() == item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
		}
	}

	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(i1,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=i1.type;
	pImp->_runner->player_drop_item(where,index1, type1, i1.count,S2C::DROP_TYPE_USE);
	inv.Remove(index1);

	if(outitem.type != -1)
	{
		if(i1_locked) outitem.Lock();
		outitem.expire_date = expire_time;
		int rst = inv.Push(outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,where,rst,state);
	}
	//pImp->_runner->talisman_combine(type1,type2,output_id>0?output_id:0);
	GLog::log(LOG_INFO,"用户%d飞升法宝(%d,%d) 产出了 %d", pImp->_parent->ID.id, index1, type1, output_id);
	return true;
}

bool talisman_item::EmbedSkill(gplayer_imp * pImp, size_t index1, size_t index2, int cid1, int cid2)
{
	item_list &inv = pImp->GetInventory();
	item &i1 = inv[index1];
	item &i2 = inv[index2];
	ASSERT(i1.GetItemType() == ITEM_TYPE_TALISMAN && i2.GetItemType() == ITEM_TYPE_TALISMAN);
	
	size_t len;
	void * buf;
	
	buf= i1.GetContent(len);
	talisman_essence ess1(buf,len);

	buf = i2.GetContent(len);
	talisman_essence ess2(buf,len);

	if(!ess1.IsValid() || !ess2.IsValid()) return false;

	talisman_essence output(NULL,0);

	ess1.PrepareForScript();
	ess2.PrepareForScript();
	output.PrepareForScript();


	talisman_enhanced_essence & oess = output.InnerEssence();	//这是 _content的副本，由于 output没有content
																//加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝
										//注意可能会在内部修改法宝的等级

	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	bool i1_locked = i1.IsLocked();	
	int expire_time = i1.expire_date;

	lua_getfield(L, LUA_GLOBALSINDEX, "Talisman_EmbedSkill_Entrance");
	lua_pushinteger(L, i1.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushinteger(L, i2.type);
	lua_pushlightuserdata(L, &ess2);
	lua_pushinteger(L, cid1);
	lua_pushinteger(L, cid2);
	lua_pushlightuserdata(L, &output);

	if(lua_pcall(L,7,1,0))
	{
		printf("embed skill talisman error when calling %s %s\n", "Talisman_EmbedSkill_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}

	int output_id = 0;
	if(!lua_isnumber(L, -1) || (output_id = lua_tointeger(L,-1)) == -1)
	{
		lua_pop(L,1);
		printf("embed skill talisman error return \n");
		return false;
	}
	lua_pop(L,1);

	//脚本调用结束，判断返回值
	item outitem;
	if(output_id > 0)
	{
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
		if(!pItem) return false;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem, *pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem, *pItem);
		}
		if(output.IsDirty())
		{
			if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}

			//更新物品
			output.UpdateContent(&outitem);
		}
		else
		{
			//如果是法宝就报错，因为没有写入物品
			if(outitem.GetItemType() == item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
		}
	}

	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(i1,1);
	pImp->UseItemLog(i2,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=i1.type, type2=i2.type;
	pImp->_runner->player_drop_item(where,index1, type1, i1.count,S2C::DROP_TYPE_USE);
	pImp->_runner->player_drop_item(where,index2, type2, i2.count,S2C::DROP_TYPE_USE);
	inv.Remove(index1);
	inv.Remove(index2);
	//检查是否需要记录消费值
	pImp->CheckSpecialConsumption(type2, 1);


	if(outitem.type != -1)
	{
		if(i1_locked) outitem.Lock();
		outitem.expire_date = expire_time;
		int rst = inv.Push(outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,where,rst,state);
	}
	GLog::log(LOG_INFO,"用户%d镶嵌法宝技能(%d,%d) (%d,%d) 产出了 %d", pImp->_parent->ID.id, index1, type1, index2,type2,output_id);
	return true;
}

bool talisman_item::SkillRefine(gplayer_imp* pImp, size_t index1, size_t index2, int cid)
{
	item_list &inv = pImp->GetInventory();
	item &i1 = inv[index1];
	item &i2 = inv[index2];
	ASSERT(i1.GetItemType() == ITEM_TYPE_TALISMAN && i2.GetItemType() == ITEM_TYPE_TALISMAN);
	
	size_t len;
	void * buf;
	
	buf= i1.GetContent(len);
	talisman_essence ess1(buf,len);

	buf = i2.GetContent(len);
	talisman_essence ess2(buf,len);

	if(!ess1.IsValid() || !ess2.IsValid()) return false;

	talisman_essence output(NULL,0);

	ess1.PrepareForScript();
	ess2.PrepareForScript();
	output.PrepareForScript();


	talisman_enhanced_essence & oess = output.InnerEssence();	//这是 _content的副本，由于 output没有content
																//加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝
										//注意可能会在内部修改法宝的等级

	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	bool i1_locked = i1.IsLocked();	
	int expire_time = i1.expire_date;

	lua_getfield(L, LUA_GLOBALSINDEX, "Talisman_RefineSkill_Entrance");
	lua_pushinteger(L, i1.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushinteger(L, i2.type);
	lua_pushlightuserdata(L, &ess2);
	lua_pushinteger(L, cid);
	lua_pushlightuserdata(L, &output);

	if(lua_pcall(L,6,1,0))
	{
		printf("Refine skill talisman error when calling %s %s\n", "Talisman_RefineSkill_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}

	int output_id = 0;
	if(!lua_isnumber(L, -1) || (output_id = lua_tointeger(L,-1)) == -1)
	{
		lua_pop(L,1);
		printf("Refine skill talisman error return \n");
		return false;
	}
	lua_pop(L,1);

	//脚本调用结束，判断返回值
	item outitem;
	if(output_id > 0)
	{
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
		if(!pItem) return false;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem, *pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem, *pItem);
		}
		if(output.IsDirty())
		{
			if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}

			//更新物品
			output.UpdateContent(&outitem);
		}
		else
		{
			//如果是法宝就报错，因为没有写入物品
			if(outitem.GetItemType() == item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
		}
	}

	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(i1,1);
	pImp->UseItemLog(i2,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=i1.type, type2=i2.type;
	pImp->_runner->player_drop_item(where,index1, type1, i1.count,S2C::DROP_TYPE_USE);
	pImp->_runner->player_drop_item(where,index2, type2, i2.count,S2C::DROP_TYPE_USE);
	inv.Remove(index1);
	inv.Remove(index2);
	//检查是否需要记录消费值
	pImp->CheckSpecialConsumption(type2, 1);


	if(outitem.type != -1)
	{
		if(i1_locked) outitem.Lock();
		outitem.expire_date = expire_time;
		int rst = inv.Push(outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,where,rst,state);
		pImp->_runner->talisman_refineskill(rst, output_id);
	}
	GLog::log(LOG_INFO,"用户%d法宝技能洗练(%d,%d) (%d,%d) 产出了 %d", pImp->_parent->ID.id, index1, type1, index2, type2, output_id);
	return true;
}

bool talisman_item::SkillRefineResult(gplayer_imp* pImp, size_t index1, int result)
{
	item_list &inv = pImp->GetInventory();
	item &i1 = inv[index1];
	ASSERT(i1.GetItemType() == ITEM_TYPE_TALISMAN);
	
	size_t len;
	void * buf;
	
	buf= i1.GetContent(len);
	talisman_essence ess1(buf,len);

	if(!ess1.IsValid()) return false;

	talisman_essence output(NULL,0);

	ess1.PrepareForScript();
	output.PrepareForScript();

	talisman_enhanced_essence & oess = output.InnerEssence();	//这是 _content的副本，由于 output没有content
																//加之如果是法宝会把这个副本写回content，所以直接修改这个副本就好了

	oess.master_id = ess1.InnerEssence().master_id;	//复制主人ID和名称
	memcpy(oess.name, ess1.InnerEssence().name, sizeof(oess.name));

	output.InnerEssence().level = 1;	//熔炼出来的东西 默认是一级 这里不设置DirtyFlag，因为还不一定是法宝
										//注意可能会在内部修改法宝的等级
	bool i1_locked = i1.IsLocked();	

	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_TALISMAN);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;

	int expire_time = i1.expire_date;

	lua_getfield(L, LUA_GLOBALSINDEX, "Talisman_RefineSkillResult_Entrance");
	lua_pushinteger(L, i1.type);
	lua_pushlightuserdata(L, &ess1);
	lua_pushinteger(L, result);
	lua_pushlightuserdata(L, &output);

	if(lua_pcall(L,4,1,0))
	{
		printf("Refine skill result talisman error when calling %s %s\n", "Talisman_RefineSkillResult_Entrance", lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	int output_id = 0;
	if(!lua_isnumber(L, -1) || (output_id = lua_tointeger(L,-1)) == -1)
	{
		lua_pop(L,1);
		printf("Ralisman refine skill result error return \n");
		return false;
	}
	lua_pop(L,1);

	//脚本调用结束，判断返回值
	item outitem;
	if(output_id > 0)
	{
		const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(output_id);
		if(!pItem) return false;
		if((pItem->proc_type & item::ITEM_PROC_TYPE_GUID) && pItem->pile_limit == 1)
		{
			item_data * pItem2 = DupeItem(*pItem);
			generate_item_guid(pItem2);
			MakeItemEntry(outitem, *pItem2);
			FreeItem(pItem2);
		}
		else
		{
			MakeItemEntry(outitem, *pItem);
		}
		if(output.IsDirty())
		{
			if(!outitem.body || outitem.GetItemType() != item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}

			//更新物品
			output.UpdateContent(&outitem);
		}
		else
		{
			//如果是法宝就报错，因为没有写入物品
			if(outitem.GetItemType() == item_body::ITEM_TYPE_TALISMAN) 
			{
				outitem.Release();
				return false;
			}
		}
	}

	//判断结束， 删除法宝物品 加入新物品
	pImp->UseItemLog(i1,1);
	int where = gplayer_imp::IL_INVENTORY;
	int type1=i1.type;
	pImp->_runner->player_drop_item(where,index1, type1, i1.count,S2C::DROP_TYPE_USE);
	inv.Remove(index1);

	if(outitem.type != -1)
	{
		if(i1_locked) outitem.Lock();
		outitem.expire_date = expire_time;
		int rst = inv.Push(outitem);
		int state = item::Proctype2State(outitem.proc_type);
		pImp->_runner->obtain_item(output_id,0, 1 ,inv[rst].count,where,rst,state);
	}
	//pImp->_runner->talisman_combine(type1,type2,output_id>0?output_id:0);
	GLog::log(LOG_INFO,"用户%d法宝技能精练(%d,%d) 结果%d, 产出了 %d", pImp->_parent->ID.id, index1, type1, result, output_id);
	return true;
}

int64_t talisman_item::GetIdModify(const item * parent) const
{
	size_t len;
	const void * buf = parent->GetContent(len);
	if(len >= sizeof(talisman_enhanced_essence))
	{
		const talisman_enhanced_essence * ess = (const talisman_enhanced_essence *) buf;
		return ((int64_t)(ess->quality &0x00FF)) << 32;
	}
	return 0;
}

void            
talisman_item::InsertAddon(const addon_data & addon)
{               
	addon_handler * handler = addon_manager::QueryHandler(addon.id);
	if(!handler) return;

	addon_data_spec spec;
	spec.handler = handler;
	spec.data = addon;

	equip_data tmpdata;
	int rst = addon_manager::CheckAndUpdate(addon,&tmpdata);
	switch(rst)
	{       
		case addon_manager::ADDON_MASK_STATIC: 
			//由于是静态的，所以无需再保存了
			break;
		case addon_manager::ADDON_MASK_ACTIVATE:
			//此addon需要保留，动态生效 
			_addon_list.push_back(spec);
			break;
		case addon_manager::ADDON_MASK_USE:
			break;
	}       

	ASSERT(_addon_list.size() < 128);
}       

int 
item_talisman_stamina_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	if(!pImp->IsTalismanAvailable())
	{
		//不报错误了 
		imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	
	size_t len;
	void * buf = parent->GetContent(len);
	if(len == sizeof(int))
	{
		int & stamina = *(int*)buf;
		if(stamina <= 0) return 1;
		if(stamina > _max_stamina) stamina = _max_stamina;

		int need = pImp->ChargeTalismanStamina(stamina);
		if(need < _min_cost) need = _min_cost;
		if(need > stamina) need = stamina;
		stamina -= need;
		if(stamina <= 0) 
			return 1;
		else
		{
			pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
			return 0;
		}
	}

	imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}


int 
talisman_item::GetMaxCurLevel(gactive_imp * obj) const
{
	gplayer_imp * pImp = (gplayer_imp*)obj;
	if(_is_aircraft) 
	{
		return 150;
	}
	else
	{
		//飞升前法宝最大等级30, 当前等级为人物等级除以5+2
		if(pImp->GetRebornCount() == 0)
		{
			int max_level = pImp->GetObjectLevel() / 5 + 2;
			if(max_level > 30) max_level = 30;
			return max_level;
		}
		//飞升后人物等级155之前，法宝最大等级35, 当前等级为人物等级除以5+2+飞升次数*5
		//155-159 法宝最大等级36
		//160法宝最大等级37
		else if(pImp->GetRebornCount() > 0)
		{
			int max_level = pImp->GetObjectLevel() / 5 + 2 + pImp->GetRebornCount() * 5;
			if(pImp->GetObjectLevel() < 155)
			{
				if(max_level > 35) max_level = 35;
				return max_level;
			}
			else if(pImp->GetObjectLevel() < 160)
			{
				if(max_level > 36) max_level = 36;
				return max_level;
			}
			else if(pImp->GetObjectLevel() == 160)
			{
				max_level = 37;
				return max_level;
			}
		}
	}
	return 0;
}


void
talisman_item::OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{	
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		gplayer_imp * pImp= (gplayer_imp *)obj;
		talisman_enhanced_essence * epEss = ess.QueryEssence();
		if(epEss->master_id == pImp->_parent->ID.id)
		{
			memset(epEss->name, 0, sizeof(epEss->name));
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			if(name_len > MAX_USERNAME_LENGTH) name_len = MAX_USERNAME_LENGTH;
			memcpy(epEss->name, name, name_len);
			pImp->PlayerGetItemInfo(l,index);
		}
	}
}

void
talisman_item::OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{	
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		gplayer_imp * pImp= (gplayer_imp *)obj;
		talisman_enhanced_essence * epEss = ess.QueryEssence();
		epEss->master_id = pImp->_parent->ID.id;
		pImp->PlayerGetItemInfo(l,index);
	}
}

void talisman_item::GetRefineSkills(item* parent, int& level, std::vector<short>& skills) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		size_t count = 0;
		const float* ss = ess.QueryRefineSkill(level, &count);	
		if(ss && count > 0)
		{
			for(size_t i = 0; i < count; i ++)
			{
				int skillid = (int)ss[i];
				if(skillid > 0)
				{
					skills.push_back(skillid);
				}
			}
		}	
	}
}

void talisman_item::GetEmbedAddons(item* parent, std::vector<int>& addons) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		size_t count = 0;
		const float* addon = ess.QueryNewAddon(&count);
		if(count > 0 && addon)
		{
			for(size_t i = 0; i < count; i ++)
			{
				addons.push_back((int)addon[i]);
			}
		}
	}
}

bool talisman_item::IsHolyLevelup(item* parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	talisman_essence ess(buf,len);
	return ess.IsValid() && ess.IsHolyLevelup();
}
