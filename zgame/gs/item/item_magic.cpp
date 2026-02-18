#include "item_magic.h"
#include "../item_list.h"
#include "../player_imp.h"
#include "item_equip.h"

void 
magic_essence::UpdateContent(item * parent)
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

	//放入inner data
	size = _inner_data.size() * sizeof(float);
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

#define RESTORE_MAGIC_DATA(L,n) \
		if(!CheckArgNum(L,n)) return 0;\
		magic_item * pItem = (magic_item*)GetAndTest(L,n,"__ITEM");\
		(void)(pItem);\
		magic_essence * pEss = (magic_essence*)GetAndTest(L,n,"__ESS");\
		if(!pEss) return 0;

int  
magic_essence::script_QueryLevel(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurLevel());
	return 1;
}

int  
magic_essence::script_SetLevel(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	int level = lua_tointeger(L, -1);
	if(level > 0 && level <= MAX_LEVEL)
	{
		pEss->SetLevel(level);
	}
	return 0;
}

int  
magic_essence::script_QueryExp(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurExp());
	return 1;
}


int  
magic_essence::script_SetExp(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	int exp = lua_tointeger(L, -1);
	if(exp >= 0)
	{
		pEss->SetExp(exp);
	}
	return 0;
}

int  
magic_essence::script_QueryMaxExp(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,0);
	lua_pushinteger(L,pEss->GetCurMaxExp());
	return 1;
}

int  
magic_essence::script_SetMaxExp(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	int max_exp = lua_tointeger(L, -1);
	if(max_exp >= 0 && max_exp <= MAX_EXP_VALUE)
	{
		pEss->SetMaxExp(max_exp);
	}
	return 0;
}

int  
magic_essence::script_SetExpSpeed(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	int exp_speed  = lua_tointeger(L, -1);
	if(exp_speed >= 0 && exp_speed < MAX_EXP_SPEED)
	{
		pEss->SetExpSpeed(exp_speed);
	}
	return 0;
}


int  
magic_essence::script_QueryExpLevel(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetCurExpLevel());
	return 1;
}

int  
magic_essence::script_SetExpLevel(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	int exp_level = lua_tointeger(L, -1);
	if(exp_level > 0 && exp_level <= MAX_EXP_LEVEL)
	{
		pEss->SetExpLevel(exp_level);
	}
	return 0;
}

int  
magic_essence::script_QueryInnerData(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);

	int index = lua_tointeger(L, -1);
	lua_pushnumber(L,pEss->QueryInnerData(index));
	return 1;
}

int  
magic_essence::script_GetInnerDataCount(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,0);
	
	lua_pushinteger(L,pEss->GetInnerDataCount());
	return 1;

}

int  
magic_essence::script_SetInnerData(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,2);
	
	int index = lua_tointeger(L, -2);
	float value = lua_tonumber(L, -1);
	pEss->SetInnerData(index,value);
	return 0;
}

int  
magic_essence::script_ClearInnerData(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	pEss->ClearInnerDara();
	return 0;

}

int  
magic_essence::script_AddAddon(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	
	int addon = lua_tointeger(L, -1);
	pEss->AddAddon(addon);
	return 0;

}

int  
magic_essence::script_SetDuration(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	if(value >= 0 && value <= MAX_DURATION_VALUE)
	{
		pEss->SetDuration(value);
	}
	return 0;
}

int  
magic_essence::script_SetMaxDuration(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetMaxDuration(value);
	return 0;
}

int  
magic_essence::script_SetQuality(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetQuality(value);
	return 0;
}

int  
magic_essence::script_SetHP(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetHP(value);
	return 0;
}

int  
magic_essence::script_SetMP(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetMP(value);
	return 0;

}

int  
magic_essence::script_SetDamage(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,2);
	int low = lua_tointeger(L, -2);
	int high = lua_tointeger(L, -1);
	pEss->SetDamage(low,high);
	return 0;

}

int  
magic_essence::script_SetAttack(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetAttack(value);
	return 0;

}

int  
magic_essence::script_SetDefense(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetDefense(value);
	return 0;

}

int  
magic_essence::script_SetArmor(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,1);
	int value = lua_tointeger(L, -1);
	pEss->SetArmor(value);
	return 0;

}

int  
magic_essence::script_SetResistance(lua_State *L)
{
	RESTORE_MAGIC_DATA(L,2);
	size_t index = lua_tointeger(L, -2);
	int value = lua_tointeger(L, -1);
	pEss->SetResistance(index, value);
	return 0;

}


void
magic_item::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
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

	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		//修改由于改名可能导致的名字没有更新
		if(epEss->master_id == obj->_parent->ID.id)
		{
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			if(memcmp(epEss->master_name, name, MAX_USERNAME_LENGTH) != 0)
			{
				OnChangeOwnerName(item::BODY, index, obj, parent);
			}

		}	
	}
}

void
magic_item::OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const
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
magic_item::VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const
{
	if(list.GetLocation() == item::BODY)
	{
		bool base = obj->GetObjectLevel() >= _level_required
			&& object_base_info::CheckCls(obj->GetObjectClass(), _class_required, _class_required1)
			&& ( (obj->IsObjectFemale()?0x02:0x01) & _require_gender);
		if(!base) return false;

		gplayer_imp * pImp = (gplayer_imp *)obj;
		//判断转生
		if(_require_reborn_count > 0 )
		{
			int reborn_count = pImp->GetRebornCount();
			if(_require_reborn_count > reborn_count ) return false;
		}
		
		//判断声望
		if(_require_rep_idx >= 0 && _require_rep_val > 0 && pImp->GetRegionReputation(_require_rep_idx) < _require_rep_val)
		{
			return false;
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
magic_item::CallScript(gactive_imp *pImp, magic_essence & ess, const char * function) const
{
	gmatrix::ScriptKeeper keeper(*gmatrix::Instance(), LUA_ENV_MAGIC);
	lua_State * L = keeper.GetState();
	if(L == NULL) return false;
	
	lua_getfield(L, LUA_GLOBALSINDEX, function);
	lua_pushinteger(L, _tid);
	lua_pushlightuserdata(L, (void*)this);
	lua_pushlightuserdata(L, &ess);
	lua_pushlightuserdata(L, (void*)pImp);

	if(lua_pcall(L, 4, 1,0))
	{
		printf("magic %d error when calling %s %s\n", _tid, function, lua_tostring(L,-1));
		lua_pop(L,1);
		return false;
	}
	if(!lua_isnumber(L, -1) || lua_tointeger(L,-1) == 0) 
	{
		printf("magic %d error when calling %s\n", _tid, function);
	}
	lua_pop(L,1);

	return true;
}

void 
magic_item::InitFromShop(gplayer_imp* pImp,item * parent, int value) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);

	ess.PrepareForScript();
	ess.SetDirty();
	CallScript(pImp, ess, "Magic_Init_Entrance");
	ess.UpdateContent(parent);
	return ;
}

bool
magic_item::GetTransformInfo(item * parent, unsigned char & level, unsigned char & exp_level, float & energy_drop_speed, int & exp_speed, int & duration) 
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		level = epEss->level;
		exp_level = epEss->exp_level;
		energy_drop_speed = _energy_drop_speed;
	       	exp_speed = epEss->exp_speed;	
		duration = epEss->duration;
		return true;
	}
	return false;
}

void 
magic_item::OnActivate(size_t index,gactive_imp* obj, item * parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(epEss->master_id == 0)
		{
			epEss->master_id = obj->_parent->ID.id;
			size_t len;
			const void * name = pImp->GetPlayerName(len);
			if(len > 20) len = 20;
			memcpy(epEss->master_name, name, len);
		}
		
	}
}

void 
magic_item::OnDeactivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp= (gplayer_imp *)obj;
	if(index == item::EQUIP_INDEX_MAGIC)
	{
		if(pImp->GetParent()->IsTransformMode() && pImp->GetParent()->transform_id == _transform_id)
		{
			OnDeactivateProp(index, obj, parent);
			pImp->PlayerStopTransform();
		}
	}
}

void
magic_item::OnActivateProp(size_t index, gactive_imp *obj, item *parent) const
{
	ASSERT(index == item::EQUIP_INDEX_MAGIC);

	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		obj->EquipItemEnhance(*epEss);
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
	}
	
	for(size_t i = 0;i < _addon_list.size(); i ++)
	{
		const addon_data_spec & spec = _addon_list[i];
		spec.handler->Activate(spec.data,this, obj,parent);
	}
}


void
magic_item::OnDeactivateProp(size_t index, gactive_imp *obj, item *parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		obj->EquipItemImpair(*epEss);
		obj->ResistanceImpair(epEss->resistance);

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
	}

	for(size_t i = 0;i < _addon_list.size(); i ++)
	{
		const addon_data_spec & spec = _addon_list[i];
		spec.handler->Deactivate(spec.data,this,obj,parent);
	}

}
	
int 
magic_item::GetClientSize(const void * buf, size_t len) const 
{ 
	if (len == 0)
		return 0;

	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		size_t addon_count;
		ess.QueryAddon(&addon_count);

		return sizeof(magic_enhanced_essence) + sizeof(size_t) + addon_count*sizeof(int);
	}
	else
	{
		return 0;
	}
}

/*void 
magic_item::GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const
{
	if(len == 0)
	{
		*data = buf;
		size = len;
	}
	else
	{
		magic_essence ess(buf,len);
		if(ess.IsValid())
		{
			magic_enhanced_essence * epEss = ess.QueryEssence();
			size_t addon_count;
			ess.QueryAddon(&addon_count);

			*data = epEss;
			size = sizeof(magic_enhanced_essence) + sizeof(size_t) + addon_count*sizeof(size_t);
		}
		else
		{
			*data = NULL;
			size = 0;
		}
	}
}*/

void 
magic_item::GetItemDataForClient(item_data_client& data, const void * buf, size_t len) const
{
	if(len == 0)
	{
		data.item_content = (char*)buf;
		data.content_length = len;
	}
	else
	{
		magic_essence ess(buf,len);
		if(ess.IsValid())
		{
			magic_enhanced_essence * epEss = ess.QueryEssence();
			size_t addon_count;
			ess.QueryAddon(&addon_count);

			data.item_content = (char*)epEss;
			data.content_length = sizeof(magic_enhanced_essence) + sizeof(size_t) + addon_count*sizeof(size_t);
		}
		else
		{
			data.item_content = NULL;
			data.content_length = 0;
		}
	}
}

int 
magic_item::GainExp(item::LOCATION l, int exp, item * parent, gactive_imp * obj, int index, bool & level_up) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(!ess.IsValid()) return -1;

	magic_enhanced_essence * epEss = ess.QueryEssence();
	if(epEss->master_id != obj->_parent->ID.id) return -1;
	if(epEss->exp_level >=  MAX_EXP_LEVEL) return -1;

	int max_exp = epEss->max_exp; 
	int new_exp = epEss->exp + exp;
	while(new_exp >= max_exp)
	{
		level_up = true;
		//升级先让变身效果失效
		if(l == item::BODY && (index == item::EQUIP_INDEX_MAGIC))
		{
			OnDeactivateProp(index, obj, parent);
		}
		//调用升级脚本
		ess.PrepareForScript();
		CallScript(obj, ess, "Magic_ExpLevelUp_Entrance");
		ess.UpdateContent(parent);
		//重新加上变身效果
		if(l == item::BODY && index == item::EQUIP_INDEX_MAGIC)
		{
			OnActivateProp(index, obj, parent);
		} 
		int level = ess.QueryEssence()->exp_level;
		if(level >= MAX_EXP_LEVEL) 
		{
			//达到最大级别了
			new_exp = 0;
			break;
		}

		//尚未到最大级别
		new_exp -= max_exp;
		max_exp = epEss->max_exp; 

	}
	ess.QueryEssence()->exp = new_exp;
	return new_exp;
}

int 
magic_item::DecMagicDuration(gactive_imp * obj, item* parent, int count, int index) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(!ess.IsValid()) return false;

	magic_enhanced_essence * epEss = ess.QueryEssence();
	if(epEss->duration <= 0) return -1;
	epEss->duration -= count;
	if(epEss->duration < 0) epEss->duration = 0;
	ess.UpdateContent(parent);
	return epEss->duration;
}

//恢复
bool
magic_item::RestoreDuration(item * parent, gactive_imp * imp) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(!ess.IsValid()) return false;

	magic_enhanced_essence * epEss = ess.QueryEssence();
	if(epEss->master_id == 0) return false;	
	if(epEss->master_id != imp->_parent->ID.id) return false;	
	if(epEss->duration >= epEss->max_duration) return false;

	size_t need_money = (epEss->max_duration - epEss->duration) * _recover_duration_fee;

	gplayer_imp * pImp = (gplayer_imp*)imp;
	if(pImp->GetMoney() < need_money)
	{
		pImp->_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	epEss->duration = epEss->max_duration;
	pImp->SpendMoney(need_money);
	if (need_money)
	{
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=14:hint=%d",pImp->GetParent()->ID.id,pImp->GetUserID(),need_money,parent->type);
	}
	pImp->_runner->spend_money(need_money);
	ess.UpdateContent(parent);
	return true;
}

//炼化
bool 
magic_item::Refine(item * parent, gactive_imp *imp, size_t index1, size_t index2)
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	item_list &inv = pImp->GetInventory();
	item &it1 = inv[index1];
	item &it2 = inv[index2];
	ASSERT(it1.GetItemType() == ITEM_TYPE_MAGIC);

	DATA_TYPE dt;
	const CHANGE_SHAPE_STONE_ESSENCE & ess_stone = *(CHANGE_SHAPE_STONE_ESSENCE*)gmatrix::GetDataMan().get_data_ptr(it2.type,ID_SPACE_ESSENCE,dt);
	if(dt != DT_CHANGE_SHAPE_STONE_ESSENCE || &ess_stone == NULL) return false;

	
	size_t len;
	void * buf = parent->GetContent( len );
	magic_essence ess( buf,len );
	if( !ess.IsValid() ) return false;
	magic_enhanced_essence * epEss = ess.QueryEssence();

	switch(ess_stone.refine_type)
	{
		case REFINE_TYPE_LEVELUP:
		{
			if(epEss->level >= MAX_LEVEL ) return false;
			if(epEss->level >= GetMaxCurLevel(pImp)) return false;
			if(epEss->exp_level != MAX_EXP_LEVEL ) return false;
			if(epEss->master_id != pImp->_parent->ID.id) return false;	
		}
		break;
		case REFINE_TYPE_EXPUP:
		{
			if(epEss->exp_level >= MAX_EXP_LEVEL ) return false;
			if(epEss->exp == epEss->max_exp) return false;
			if(epEss->master_id != pImp->_parent->ID.id) return false;	
		}
		break;
		case REFINE_TYPE_RESET:
		{
			if(epEss->master_id == 0) return false;
			if(epEss->master_id == pImp->_parent->ID.id) return false;
		}
		break;
		default:
			return false;
		break;
	}

	if(ess_stone.refine_type == REFINE_TYPE_RESET)
	{
		epEss->master_id = 0;
		memset(epEss->master_name, 0 , sizeof(epEss->master_name));
	}

	ess.PrepareForScript();
	//开始脚本调用逻辑	
	gmatrix::ScriptKeeper keeper( *gmatrix::Instance(), LUA_ENV_MAGIC);
	lua_State * L = keeper.GetState();
	if( NULL == L ) return false;
	lua_getfield( L, LUA_GLOBALSINDEX, "Magic_Refine_Entrance" );
	lua_pushinteger(L, it1.type);
	lua_pushlightuserdata(L, &ess);
	lua_pushinteger(L, it2.type);
	lua_pushlightuserdata(L, (void*)pImp);
	lua_pushinteger(L, ess_stone.refine_type);
	lua_pushnumber(L, ess_stone.ext_succeed_prob);
	lua_pushnumber(L, ess_stone.ext_match_value);
	
	if( lua_pcall( L, 7, 3, 0 ) )
	{
		printf( "enchant magic error when calling %s %s\n", "Magic_Enchant_Entrance", lua_tostring( L, -1 ) );
		lua_pop( L, 1 );
		return false;
	}

	int channel_id = lua_tointeger( L, -2 );
	int msg_id = lua_tointeger( L, -1 );
	if(msg_id > 0 && channel_id >=0)
	{
		pImp->_runner->script_message(pImp->_parent->ID.id,0,channel_id,msg_id);
	}

	if( !lua_isnumber( L, -3 ) || ( lua_tointeger( L, -3 ) ) == 0 ) 
	{
		lua_pop( L, 3 );
		return false;
	}
	lua_pop( L, 3);

	int type1=it1.type, type2=it2.type;
	ess.UpdateContent(parent);
	GLog::log( LOG_INFO, "用户%d精炼了幻灵石(%d,%d),使用道具(%d,%d)", 
		pImp->_parent->ID.id, index1, type1, index2, type2);
	return true;
}

int64_t magic_item::GetIdModify(const item * parent) const
{
	size_t len;
	const void * buf = parent->GetContent(len);
	if(len >= sizeof(magic_enhanced_essence))
	{
		const magic_enhanced_essence * ess = (const magic_enhanced_essence *) buf;
		return ((int64_t)(ess->quality &0x00FF)) << 32;
	}
	return 0;
}



void            
magic_item::InsertAddon(const addon_data & addon)
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
magic_item::GetMaxCurLevel(gactive_imp * obj) 
{
	gplayer_imp * pImp = (gplayer_imp*)obj;
	return pImp->GetObjectLevel()/10 + 3*pImp->GetRebornCount() +1;
}

void
magic_item::OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{	
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		if(epEss->master_id == pImp->_parent->ID.id)
		{
			memset(epEss->master_name, 0, sizeof(epEss->master_name));
			size_t name_len;
			const void * name = pImp->GetPlayerName(name_len);
			if(name_len > MAX_USERNAME_LENGTH) name_len = MAX_USERNAME_LENGTH;
			memcpy(epEss->master_name, name, name_len);
			pImp->PlayerGetItemInfo(l,index);
		}
	}
}

void
magic_item::OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{	
	size_t len;
	void * buf = parent->GetContent(len);
	magic_essence ess(buf,len);
	if(ess.IsValid())
	{
		magic_enhanced_essence * epEss = ess.QueryEssence();
		gplayer_imp * pImp= (gplayer_imp *)obj;
		epEss->master_id = pImp->_parent->ID.id;
		pImp->PlayerGetItemInfo(l,index);
	}
}

