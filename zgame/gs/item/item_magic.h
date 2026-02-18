#ifndef __ONLINEGAME_GS_MAGIC_ITEM_H__
#define __ONLINEGAME_GS_MAGIC_ITEM_H__

#include "../item.h"

//需要存盘的数据
struct magic_enhanced_essence
{
	int master_id;
	char master_name[20];		//主人名称
	int level;			//变身石等级
	int exp_level;			//熟练度等级
	int duration;			//灵气, 耐久度
	int max_duration;		//灵气, 耐久度
	int quality;			//品级
	int exp;			//熟练度
	int max_exp;			//当前等级最大熟练度
	int exp_speed;			//熟练度成长速度
	int hp;
	int mp;
	int damage_low;			//最低攻击	
	int damage_high;		//最高攻击
	int attack;			//普攻命中
	int defense;			//防御
	int armor;			//普攻闪躲
	int resistance[6];
	int reserver1;
	int reserver2;
	int reserver3;
	int reserver4;
	int reserver5;
};


/*
	幻灵石数据结构:
	essence
	size_t addon_size;
	char [addon_size];
	size_t inner_size;
	char [inner_size];
*/

enum
{
	MAX_LEVEL = 20, 
	MAX_EXP_LEVEL = 5,
	MAX_EXP_VALUE = 65535, 
	MAX_EXP_SPEED = 128, 
	MAX_DURATION_VALUE = 65535,

};

enum
{
	REFINE_TYPE_RESET, 	//归元
	REFINE_TYPE_LEVELUP,	//提升等级
	REFINE_TYPE_EXPUP,	//提升契合值
	REFINE_TYPE_RENEW,	//重置
};

class magic_essence
{
	const void * _content;
	size_t _size;

	magic_enhanced_essence _enhanced_essence;
	abase::vector<int ,abase::fast_alloc<> > _addon_data;
	abase::vector<float ,abase::fast_alloc<> > _inner_data;

	bool _dirty_flag;
	bool _content_valid;

	bool TestContentValid()
	{
		size_t size = _size;
		if(size < sizeof(magic_enhanced_essence) + sizeof(int)*2) return false;

		int * pData = (int*)((char *)_content + sizeof(magic_enhanced_essence));
		size -= sizeof(magic_enhanced_essence);

		size_t tmpSize = pData[0];
		ASSERT(tmpSize < 0xFFFF);
		if(size < sizeof(size_t) + tmpSize + sizeof(size_t)) return false;
		size -= sizeof(size_t)  + tmpSize;

		pData = (int*)(((char*)pData) + pData[0] + sizeof(int));
		tmpSize = pData[0];
		ASSERT(tmpSize < 0xFFFF);
		if(size != sizeof(size_t) + tmpSize) return false;

		return true;
	}

	void DuplicateInnerData()
	{
		if(!_content_valid) return;

		size_t size = _size;
		int * pData = (int*)((char *)_content + sizeof(magic_enhanced_essence));
		size -= sizeof(magic_enhanced_essence) + pData[0] + sizeof(int);
		pData = (int*)(((char*)pData) + pData[0] + sizeof(int));

		size_t count =  pData[0]/sizeof(float);
		float * list = (float*)&pData[1];
		_inner_data.reserve(count);
		size -= sizeof(int);
		for(size_t i = 0; i < count && size >0 ; i ++)
		{
			_inner_data.push_back(list[i]);
		}

		memcpy(&_enhanced_essence ,_content,sizeof(magic_enhanced_essence));

	}

public:
	magic_essence(const void * buf, size_t size):_content(buf), _size(size),_dirty_flag(false)
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

	magic_enhanced_essence & InnerEssence() 
	{
		return _enhanced_essence;
	}

	magic_enhanced_essence * QueryEssence() const
	{
		if(_content_valid) 
			return (magic_enhanced_essence *)_content;
		else
			return NULL;
	}

	const int * QueryAddon(size_t * pCount) const
	{
		if(_content_valid) 
		{
			int * pData = (int*)((char *)_content + sizeof(magic_enhanced_essence)); 
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
			int * pData = (int*)((char *)_content + sizeof(magic_enhanced_essence) + sizeof(size_t) + (addon_count)*sizeof(int));

			*pCount = pData[0]/sizeof(int);
			return &pData[1];
		}
		else
		{
			pCount = 0;
			return NULL;
		}
	}

public:

	//准备被脚本调用
	void PrepareForScript()
	{
		memset(&_enhanced_essence ,0,sizeof(_enhanced_essence));
		_addon_data.clear();
		_inner_data.clear();

		DuplicateInnerData();
	}

	//给脚本用的获取函数
	float QueryInnerData(size_t index) const
	{
		if(index >= _inner_data.size()) return 0.f;
		return _inner_data[index];
	}

	size_t GetInnerDataCount() const
	{
		return _inner_data.size();
	}

	int  GetCurLevel() 
	{
		return _enhanced_essence.level;
	}

	int GetCurExp()
	{
		return _enhanced_essence.exp;
	}
	
	int GetCurMaxExp()
	{
		return _enhanced_essence.max_exp;
	}

	int GetCurExpLevel()
	{
		return _enhanced_essence.exp_level;
	}

	void SetLevel(int level)
	{
		_enhanced_essence.level = level;
	}

	void SetExp(int exp)
	{
		_enhanced_essence.exp = exp;
	}

	void SetMaxExp(int max_exp)
	{
		_enhanced_essence.max_exp = max_exp;
	}

	void SetExpSpeed(int exp_speed)
	{
		_enhanced_essence.exp_speed = exp_speed;
	}
	void SetExpLevel(int level)
	{
		_enhanced_essence.exp_level = level;
	}

public:
	//给脚本用的添加函数

	void ClearInnerDara()
	{
		_dirty_flag = 1;
		_inner_data.clear();
	}

	void SetInnerData(size_t index, float value)
	{
		_dirty_flag = 1;
		if(index >= _inner_data.size())
		{
			_inner_data.reserve((index < 5?10:(int)((index + 1)*1.5f)));
			for(size_t i = _inner_data.size(); i <=index; i ++)
			{
				_inner_data.push_back(0.f);
			}
		}
		_inner_data[index] = value;
	}

	void AddAddon(int addon_id)
	{
		_dirty_flag = 1;
		_addon_data.push_back(addon_id);
	}

	void SetDuration(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.duration = value;
	}

	void SetMaxDuration(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.max_duration = value;
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

	void SetDamage(int low,int high)
	{
		_dirty_flag = 1;
		_enhanced_essence.damage_low  = low;
		_enhanced_essence.damage_high = high;
	}

	void SetAttack(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.attack = value;
	}


	void SetDefense(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.defense = value;
	}

	void SetArmor(int value)
	{
		_dirty_flag = 1;
		_enhanced_essence.armor = value;
	}

	void SetResistance(size_t index, int  value)
	{
		if(index >= 6) return;
		_dirty_flag = 1;
		_enhanced_essence.resistance[index] = value;
	}

public:
	static int  script_QueryLevel(lua_State *L);
	static int  script_SetLevel(lua_State *L);
	static int  script_QueryExp(lua_State *L);
	static int  script_SetExp(lua_State *L);
	static int  script_QueryMaxExp(lua_State *L);
	static int  script_SetMaxExp(lua_State *L);
	static int  script_QueryExpLevel(lua_State *L);
	static int  script_SetExpLevel(lua_State *L);
	static int  script_SetExpSpeed(lua_State *L);

	static int  script_QueryInnerData(lua_State *L);
	static int  script_GetInnerDataCount(lua_State *L);
	static int  script_SetInnerData(lua_State *L);
	static int  script_ClearInnerData(lua_State *L);
	static int  script_AddAddon(lua_State *L);

	static int  script_SetDuration(lua_State *L);
	static int  script_SetMaxDuration(lua_State *L);
	static int  script_SetQuality(lua_State *L);
	static int  script_SetHP(lua_State *L);
	static int  script_SetMP(lua_State *L);
	static int  script_SetDamage(lua_State *L);
	static int  script_SetAttack(lua_State *L);
	static int  script_SetDefense(lua_State *L);
	static int  script_SetArmor(lua_State *L);
	static int  script_SetResistance(lua_State *L);

};

class magic_item : public item_body
{
	int64_t _class_required;
	int64_t _class_required1;
	int _level_required;
	int _require_gender;		//女 1 ， 男0 
	int _equip_mask; //$$$$
	int _require_reborn_count;
	float _energy_drop_speed;
	int _recover_duration_fee;
	int _require_rep_idx;
	int _require_rep_val;
	int _transform_id;
	
	abase::vector<addon_data_spec> _addon_list;
	
	bool CallScript(gactive_imp * pImp, magic_essence & ess, const char * function) const ;
	
public:
	magic_item( int64_t class_required, int64_t class_required1, int level_required, int require_gender, float energy_drop_speed, int recover_duration_fee, int reborn_count, int repu_index, int require_repu, int transform_id) 
	{
		_class_required = class_required;
		_class_required1 = class_required1;
		_level_required = level_required;
		_require_gender = require_gender;
		_require_reborn_count = reborn_count;
		_energy_drop_speed = energy_drop_speed;
		_recover_duration_fee = recover_duration_fee;
		_require_rep_idx = repu_index;
		_require_rep_val = require_repu;
		_transform_id = transform_id;

	}
	void InsertAddon(const addon_data & addon);

public:
	static int GetMaxCurLevel(gactive_imp * obj);

public:
	virtual bool GetTransformInfo(item *parent,  unsigned char & level, unsigned char & exp_level, float & energy_drop_speed, int & exp_speed, int & duration); 
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

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_MAGIC;}
	virtual bool OnCheckAttack(item_list & list) const {return true;}
	virtual void OnAfterAttack(item_list & list,bool * pUpdate) const { *pUpdate = false;} 
	//virtual void GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const;
	virtual void GetItemDataForClient(item_data_client& data, const void * buf, size_t len) const;
	virtual int GainExp(item::LOCATION l, int exp, item * parent,gactive_imp * obj, int index, bool & level_up) const ;
	virtual int64_t GetIdModify(const item * parent) const;
	virtual int GetClientSize(const void * buf, size_t len) const;
	virtual int DecMagicDuration(gactive_imp * obj, item* parent, int count,int index) const; 
	virtual bool Refine(item *parent, gactive_imp *imp, size_t index1, size_t index2);
	virtual bool RestoreDuration(item * parent, gactive_imp * imp) const;

private:
	virtual int OnGetEquipMask() const 
	{
		return item::EQUIP_MASK_MAGIC;
	}
};

#endif
