#ifndef __ONLINEGAME_GS_TALISMAN_ITEM_H__
#define __ONLINEGAME_GS_TALISMAN_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include <crc.h>
#include <vector>

class gplayer_imp;

//法宝数据分成两个部分，一部分供策划动态使用，一部分则供返回客户端使用
struct talisman_enhanced_essence
{
	int master_id;
	char name[20];
	int exp;
	int level;
//后面是脚本调整的 上面是脚本不能调整的	  
	int stamina;		//stamina一定是第一个脚本调整数据,因为上面的所有东西都要进行复制操作
	int quality;
	int hp;
	int mp;
	int attack_enhance;
	int damage_low;
	int damage_high;
	int mp_enhance;
	int hp_enhance;
	int resistance[6];
};
/*
	法宝数据结构:
	essence
	size_t addon_size;
	char [addon_size];
	size_t inner_size;
	char [inner_size];
	size_t inner_visible_size
	char [inner_visible_size]
*/

class talisman_essence
{
	const void * _content;
	size_t _size;

	abase::vector<float ,abase::fast_alloc<> > _inner_data;
	abase::vector<float ,abase::fast_alloc<> > _inner_data_visible;  //需要传给客户端使用

	talisman_enhanced_essence _enhanced_essence;
	abase::vector<int ,abase::fast_alloc<> > _addon_data;
	bool _dirty_flag;
	bool _content_valid;
	
	bool TestContentValid()
	{
		size_t size = _size;
		if(size < sizeof(talisman_enhanced_essence) + sizeof(int)*2) return false;

		size_t * pData = (size_t*)((char *)_content + sizeof(talisman_enhanced_essence));
		size -= sizeof(talisman_enhanced_essence);
		size_t tmpSize = pData[0];
		if(tmpSize > 0x7FFFFFFF) return false;
		if(size < sizeof(size_t) + tmpSize + sizeof(size_t)) return false;

		pData = (size_t*)((char *)_content + sizeof(talisman_enhanced_essence) + sizeof(size_t) + tmpSize);
		size -= sizeof(size_t)  + tmpSize;
		tmpSize = pData[0];

		if(size < sizeof(size_t) + tmpSize) return false;

		size -= sizeof(size_t) + tmpSize;	
		if(size > 0)
		{
			pData = (size_t*)((char*)pData + pData[0] + sizeof(size_t));
			tmpSize = pData[0];	
			if(size != sizeof(size_t) + tmpSize) return false;	
		}
		return true;
	}

	void DuplicateInnerData()
	{
		if(!_content_valid) return;

		size_t size = _size;
		int * pData = (int*)((char *)_content + sizeof(talisman_enhanced_essence));
		size -= sizeof(talisman_enhanced_essence) + pData[0] + sizeof(int);

		pData = (int*)(((char*)pData) + pData[0] + sizeof(int));
		size_t count =  pData[0]/sizeof(float);
		float * list = (float*)&pData[1];
		_inner_data.reserve(count);
		size -= sizeof(int);
		for(size_t i = 0; i < count && size >0 ; i ++)
		{
			_inner_data.push_back(list[i]);
			size -= sizeof(float);
		}

		if(size > 0)
		{	
			pData = (int*)((char*)pData + pData[0] + sizeof(int));
			size_t count2 = pData[0]/sizeof(float);
			float * list2 = (float*)&pData[1];
			_inner_data_visible.reserve(count2);
			size -= sizeof(int);
			for(size_t i = 0; i < count2 && size >0; i ++)
			{
				_inner_data_visible.push_back(list2[i]);
				size -= sizeof(float);
			}
		}
		memcpy(&_enhanced_essence ,_content,offsetof(talisman_enhanced_essence, stamina));

	}
public:
	talisman_essence(const void * buf, size_t size):_content(buf), _size(size),_dirty_flag(false)
	{
		_content_valid = TestContentValid();
	}

	bool IsValid()
	{
		return _content_valid;
	}

	bool IsDirty()
	{
		return _dirty_flag;
	}

	void SetDirty()
	{
		_dirty_flag = 1;
	}

	void UpdateContent(item * parent);
	
	//给程序用的获取函数
	talisman_enhanced_essence & InnerEssence() 
	{
		return _enhanced_essence;
	}

	talisman_enhanced_essence * QueryEssence() const
	{
		if(_content_valid) 
			return (talisman_enhanced_essence *)_content;
		else
			return NULL;
	}

	const int * QueryAddon(size_t * pCount) const
	{
		if(_content_valid) 
		{
			int * pData = (int*)((char *)_content + sizeof(talisman_enhanced_essence));
			*pCount = pData[0]/sizeof(int);
			return &pData[1];
		}
		else
		{
			pCount = 0;
			return NULL;
		}
	}
	const int * QueryInner(size_t * pCount) const
	{
		if(_content_valid) 
		{
			size_t addon_count;
			QueryAddon(&addon_count);

			int * pData = (int*)((char *)_content + sizeof(talisman_enhanced_essence) + sizeof(size_t) + (addon_count)*sizeof(int));
			*pCount = pData[0]/sizeof(int);
			return &pData[1];
		}
		else
		{
			pCount = 0;
			return NULL;
		}
	}

	const float* QueryNewAddon(size_t* pCount) const
	{
		if(_content_valid)
		{
			*pCount = 5;
			char* pBuf = (char*)_content;
			pBuf += sizeof(talisman_enhanced_essence);
			size_t addon_size = *(size_t*)pBuf;
			pBuf += sizeof(size_t) + addon_size;
			size_t inner_size = *(size_t*)pBuf;	
			if(inner_size == sizeof(float) * 121 || inner_size == sizeof(float) * 134)
			{
				float* pData = (float*)(pBuf + sizeof(size_t) + sizeof(float) * 114);
				return pData;
			}
			else
			{
				*pCount = 0;
				return NULL;
			}
		}
		else
		{
			*pCount = 0;
			return NULL;
		}
	}

	const float* QueryRefineSkill(int& level, size_t* pCount) const
	{
		if(_content_valid)
		{
			*pCount = 8;
			char* pBuf = (char*)_content;
			pBuf += sizeof(talisman_enhanced_essence);
			size_t addon_size = *(size_t*)pBuf;
			pBuf += sizeof(size_t) + addon_size;
			size_t inner_size = *(size_t*)pBuf;	
			if(inner_size == sizeof(float) * 121 || inner_size == sizeof(float) * 134)
			{
				level = (int)*(float*)(pBuf + sizeof(size_t) + sizeof(float) * 96);
				float* pData = (float*)(pBuf + sizeof(size_t) + sizeof(float) * 98);
				return pData;
			}
			else
			{
				*pCount = 0;
				return NULL;
			}
		}
		else
		{
			*pCount = 0;
			return NULL;
		}
	}

	bool IsHolyLevelup() const
	{
		if(_content_valid)
		{
			char* pBuf = (char*)_content;
			pBuf += sizeof(talisman_enhanced_essence);
			size_t addon_size = *(size_t*)pBuf;
			pBuf += sizeof(size_t) + addon_size;
			size_t inner_size = *(size_t*)pBuf;	
			if(inner_size >= sizeof(float) * 121 || inner_size == sizeof(float) * 134)
			{
				return (int)*(float*)(pBuf + sizeof(size_t) + sizeof(float) * 119) > 0;
			}
		}
		return false;
	}

	void GetClientData(packet_wrapper& ar);
	bool PackInnerData( packet_wrapper& ar, const char* pbuf, size_t inner_cnt, size_t start, size_t end ); // Youshuang add
	
public:
	
	//准备被脚本调用
	void PrepareForScript()
	{
		memset(&_enhanced_essence ,0,sizeof(_enhanced_essence));
		_inner_data.clear();
		_inner_data_visible.clear();
		_addon_data.clear();
		
		DuplicateInnerData();
	}

	//给脚本用的获取函数
	float QueryInnerData(int space, size_t index) const
	{
		if(space == 0)
		{
			if(index >= _inner_data.size()) return 0.f;
			return _inner_data[index];
		}
		else if(space == 1)
		{
			if(index >= _inner_data_visible.size()) return 0.f;
			return _inner_data_visible[index];
		}
		return 0.f;	
	}

	size_t GetInnerDataCount(int space) const
	{
		if(space == 0) return _inner_data.size();
		else if(space == 1) return _inner_data_visible.size();
		return 0;
	}

	int  GetCurLevel() 
	{
		return _enhanced_essence.level;
	}

	int GetCurExp()
	{
		return _enhanced_essence.exp;
	}

	void SetLevel(int level)
	{
		_enhanced_essence.level = level;
	}
	
	void SetExp(int exp)
	{
		_enhanced_essence.exp = exp;
	}

/*
	int QueryAddonData(size_t index) const
	{
		if(!_content_valid) return -1;

		int * pData = (int*)((char *)_content + sizeof(talisman_enhanced_essence));
		size_t max_index = pData[0]/sizeof(int);
		if(index >= max_index) return -1;
		return pData[1 + index];
	}

	size_t GetAddonCount() const
	{
		if(!_content_valid) return 0;

		int * pData = (int*)((char *)_content + sizeof(talisman_enhanced_essence));
		return pData[0]/sizeof(int);
	}
*/
public:
	//给脚本用的添加函数

	void ClearInnerDara(int space)
	{
		_dirty_flag = 1;
		if(space == 0) _inner_data.clear();
		else if(space == 1) _inner_data_visible.clear();
	}
	
	void SetInnerData(int space, size_t index, float value)
	{
		_dirty_flag = 1;
		if(space == 0)
		{
			if(index >= _inner_data.size())
			{
				_inner_data.reserve((index < 5?10:(int)((index + 1)*1.5f)));
				//_inner_data.reserve(index + 1);
				for(size_t i = _inner_data.size(); i <=index; i ++)
				{
					_inner_data.push_back(0.f);
				}
			}
			_inner_data[index] = value;
		}
		else if(space == 1)
		{
			if(index >= _inner_data_visible.size())
			{
				_inner_data_visible.reserve((index < 5?10:(int)((index + 1)*1.5f)));
				//_inner_data.reserve(index + 1);
				for(size_t i = _inner_data_visible.size(); i <=index; i ++)
				{
					_inner_data_visible.push_back(0.f);
				}
			}
			_inner_data_visible[index] = value;

		}
	}

	void AddAddon(int addon_id)
	{
		_dirty_flag = 1;
		_addon_data.push_back(addon_id);
	}

	void SetStamina(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.stamina = value;
	}

	void SetQuality(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.quality = value;
	}

	void SetHP(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.hp = value;
	}

	void SetMP(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.mp = value;
	}

	void SetAttackEnhance(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.attack_enhance = value;
	}
	
	void SetDamage(int low,int high)
	{
		_dirty_flag = 1;
		_enhanced_essence.damage_low  = low;
		_enhanced_essence.damage_high = high;
	}

	void SetMPEnhance(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.mp_enhance= value;
	}
	
	void SetHPEnhance(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.hp_enhance= value;
	}

	void SetResistance(size_t index, int  value)
	{
		if(index >= 6) return;
		_dirty_flag = 1;
		_enhanced_essence.resistance[index] = value;
	}

public:
	static int  script_QueryLevel(lua_State *L);
	static int  script_QueryExp(lua_State *L);
	static int  script_QueryInnerData(lua_State *L);
	static int  script_SetLevel(lua_State *L);
	static int  script_SetExp(lua_State *L);
	static int  script_GetInnerDataCount(lua_State *L);
	static int  script_SetInnerData(lua_State *L);
	static int  script_ClearInnerData(lua_State *L);
	static int  script_AddAddon(lua_State *L);
	static int  script_SetStamina(lua_State *L);
	static int  script_SetQuality(lua_State *L);
	static int  script_SetHP(lua_State *L);
	static int  script_SetMP(lua_State *L);
	static int  script_SetDamage(lua_State *L);
	static int  script_SetResistance(lua_State *L);
	static int  script_SetAttackEnhance(lua_State *L);
	static int  script_SetHPEnhance(lua_State *L);
	static int  script_SetMPEnhance(lua_State *L);
};

class talisman_item : public item_body
{
	int64_t _class_required;
	int64_t _class_required1;
	int _level_required;
	int _require_gender;		//女 1 ， 男0 
	int _equip_mask; //$$$$
	int _is_aircraft;	//是否飞行器
	float _fly_speed;	//飞行消耗
	int _fly_cost;		//飞行消耗
	int _fly_exp_add;	//飞行增加经验
	int _max_level;
	int _max_level2;
	int _init_level;
	float _energy_recover_speed;
	float _energy_recover_factor;
	float _energy_drop_speed;
	int _require_reborn_count;
	int64_t _require_reborn_prof[3];     //1 2 3转时的职业要求
	int64_t _require_reborn_prof1[3];	 //1 2 3转时的职业要求
	bool _change_color;
	
	abase::vector<addon_data_spec> _addon_list;

	bool CallScript(talisman_essence & ess, const char * function,int value = -1) const ;
public:
	talisman_item( int64_t class_required, int64_t class_required1, int level_required, int require_gender)
		:_class_required(class_required), _class_required1(class_required1), _level_required(level_required), _require_gender(require_gender)
		{
			_max_level=0;
			_max_level2=0;
			_init_level=0;
			_energy_recover_speed = 0;
			_energy_recover_factor = 0;
			_energy_drop_speed = 0;
			_is_aircraft = 0;
			_fly_speed = 0;
			_fly_cost = 0;
			_fly_exp_add = 0;
			_change_color = false;
		}

	void Init(int mlevel,int mlevel2, float enery_recover, float enery_recover_factor, float enery_drop, int init_level)
	{
		_max_level = mlevel;
		_max_level2 = mlevel2;
		_energy_recover_speed = enery_recover;
		_energy_recover_factor = enery_recover_factor;
		_energy_drop_speed = enery_drop;
		_init_level = init_level;
	}

	void InitRebornRequire(int require_reborn_count, int64_t require_prof[3], int64_t require_prof1[3])
	{
		_require_reborn_count = require_reborn_count;
		for(int i = 0; i < 3; i ++)
		{
			_require_reborn_prof[i] = require_prof[i];
			_require_reborn_prof1[i] = require_prof1[i];
		}
	}

	void SetAircraft(int aircraft, float speed, int cost, int exp_add, bool change_color)
	{
		_is_aircraft = aircraft;
		_fly_speed = speed;
		_fly_cost = cost;
		_fly_exp_add = exp_add;
		_change_color = change_color;
	}

	void InsertAddon(const addon_data & addon);

	int GetMaxCurLevel(gactive_imp * obj) const;
public:
	static bool Combine(gplayer_imp * pImp, size_t index1, size_t index2, int cid);
	static bool Enchant( gplayer_imp * pImp, size_t index1, size_t index2 );
	// Youshuang add
	static bool GenNewTalisman( talisman_essence& output, int output_id, item& outitem );
	static bool ConfirmEnchant( gplayer_imp* pImp, size_t index1, int confirm );
	// end

	static bool HolyLevelup(gplayer_imp * pImp, size_t index1, int cid);
	static bool EmbedSkill(gplayer_imp * pImp, size_t index1, size_t index2, int cid1, int cid2);
	static bool SkillRefine(gplayer_imp* pImp, size_t index1, size_t index2, int cid);
	static bool SkillRefineResult(gplayer_imp* pImp, size_t index1, int result);

public:
	virtual void OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const;
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const;
	virtual void InitFromShop(gplayer_imp* pImp,item * parent, int value) const;
	virtual void OnActivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnDeactivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnActivateProp(size_t index, gactive_imp *obj, item *parent) const;
	virtual void OnDeactivateProp(size_t index, gactive_imp *obj, item *parent) const;
	virtual void OnChangeOwnerName(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnChangeOwnerID(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TALISMAN;}
	virtual bool OnCheckAttack(item_list & list) const {return true;}
	virtual void OnAfterAttack(item_list & list,bool * pUpdate) const { *pUpdate = false;} 
	//virtual void GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const;
	virtual void GetItemDataForClient(item_data_client& data, const void * buf, size_t len) const;
	virtual int GainExp(item::LOCATION l, int exp, item * parent,gactive_imp * pImp,int index, bool & level_up) const ;
	virtual bool LevelUp(item * parent, gactive_imp * pImp) const;
	virtual bool ItemReset(item * parent, gactive_imp * pImp) const;
	virtual int64_t GetIdModify(const item * parent) const;
	virtual int GetClientSize(const void * buf, size_t len) const;

	virtual void GetRefineSkills(item* parent, int& level, std::vector<short>& skills) const;
	virtual void GetEmbedAddons(item* parent, std::vector<int>& addons) const;
	virtual bool IsHolyLevelup(item* parent) const;

private:
	virtual int OnGetEquipMask() const 
	{
		return _is_aircraft?item::EQUIP_MASK_WING:item::EQUIP_MASK_TALISMAN1;
	}
};

class item_talisman_stamina_potion : public item_body
{
	int _min_cost;
	int _max_stamina;
public:
	item_talisman_stamina_potion(int min_cost, int max_stamina):_min_cost(min_cost),_max_stamina(max_stamina)
	{
		ASSERT(min_cost >= 0);
	}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return false;}

};
#endif

