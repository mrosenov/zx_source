#ifndef __ONLINEGAME_GS_RUNE_ITEM_H__
#define __ONLINEGAME_GS_RUNE_ITEM_H__

#include "../item.h"

enum
{
	MAX_RUNE_LEVEL = 20, 
	MAX_QUALITY = 100,
	MAX_HOLE = 5,
	MAX_PROP_ID = 33,
	RUNE_DECOMPOSE_LEVEL = 10,
	RUNE_DECOMPOSE_EXP = 200,
	RUNE_OPENSLOT_LEVEL = 10,
	REFINE_CASH_ITEM1 = 52258,
	REFINE_CASH_ITEM2 = 52378,
};

/*
	元魂数据结构:
	essence
	size_t inner_size;
	char [inner_size];
*/

#pragma pack(1)
struct rune_prop
{
	int id;
	int value;
	int grade;		//大挡位
	float grade_extra;	//小挡位
};

struct rune_enhanced_essence 
{
	int exp;
	int level;
	int quality;		//品质
	int hole;		//孔数
	bool refine_active;	//是否有洗炼属性
	int refine_quality;
	rune_prop prop[8];
	rune_prop refine_prop[8];	//洗练属性
	int rune_stone[5];
	int reserver[8];	//预留字段
	int cash_refine_cnt;	//使用收费道具次数洗练次数
	int refine_remain;	//不消耗道具洗练次数剩余次数
	int refine_cnt;		//总洗练次数
	int reset_cnt;		//归元次数
	int avg_grade;		//平均挡位
};

#pragma pack()

class rune_essence
{
	const void * _content;
	size_t _size;

	rune_enhanced_essence _enhanced_essence;
	abase::vector<float ,abase::fast_alloc<> > _inner_data;

	bool _dirty_flag;
	bool _content_valid;
	
	bool TestContentValid()
	{
		size_t size = _size;
		if(size < sizeof(rune_enhanced_essence) + sizeof(int)) return false;

		int * pData = (int*)((char *)_content + sizeof(rune_enhanced_essence));
		size -= sizeof(rune_enhanced_essence);

		size_t tmpSize = pData[0];
		ASSERT(tmpSize < 0xFFFF);
		if(size != sizeof(size_t) + tmpSize) return false;

		return true;
	}
	
	void DuplicateInnerData()
	{
		if(!_content_valid) return;

		size_t size = _size;
		int * pData = (int*)((char *)_content + sizeof(rune_enhanced_essence));

		size_t count =  pData[0]/sizeof(float);
		float * list = (float*)&pData[1];
		_inner_data.reserve(count);
		size -= sizeof(int);
		for(size_t i = 0; i < count && size >0 ; i ++)
		{
			_inner_data.push_back(list[i]);
		}

		memcpy(&_enhanced_essence,_content,sizeof(rune_enhanced_essence));
	}

public:
	rune_essence(const void * buf, size_t size):_content(buf), _size(size),_dirty_flag(false)
	{
		_content_valid = TestContentValid();

		memset(&_enhanced_essence ,0,sizeof(_enhanced_essence));
		_inner_data.clear();
		DuplicateInnerData();
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
	
	rune_enhanced_essence & InnerEssence() 
	{
		return _enhanced_essence;
	}

	rune_enhanced_essence * QueryEssence() const
	{
		if(_content_valid) 
			return (rune_enhanced_essence *)_content;
		else
			return NULL;
	}


	const int * QueryInner(size_t * pCount) const
	{
		if(_content_valid) 
		{
			int * pData = (int*)((char *)_content + sizeof(rune_enhanced_essence) );

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


	int  GetCurLevel() 
	{
		return _enhanced_essence.level;
	}

	void SetCurLevel(int cur_level)
	{
		_enhanced_essence.level = cur_level;
	}
	
	int GetCurExp()
	{
		return _enhanced_essence.exp;
	}

	void SetCurExp(int cur_exp)
	{
		_enhanced_essence.exp = cur_exp;
	}
	
	void AddExp(int exp)
	{
		_enhanced_essence.exp += exp;
	}

	void DecExp(int exp)
	{
		_enhanced_essence.exp -= exp;
	}

	int GetCurQuality()
	{
		return _enhanced_essence.quality;
	}

	void SetQuality(int quality)
	{
		_enhanced_essence.quality = quality; 
	}
	
	void SetRefineQuality(int quality)
	{
		_enhanced_essence.refine_quality = quality; 
	}

	int GetCurHole()
	{
		return _enhanced_essence.hole;
	}

	void SetHole(int hole)
	{
		_enhanced_essence.hole = hole;
	}

	int GetStoneID(int index)
	{
		if(index < 0 || index > MAX_HOLE) return 0;
		if(index > _enhanced_essence.hole) return 0;

	       return _enhanced_essence.rune_stone[index];	
	}

	void SetStoneID(int index, int stone_id)
	{
		if(index < 0 || index > MAX_HOLE) return ;
		if(index > _enhanced_essence.hole) return;

		_enhanced_essence.rune_stone[index] = stone_id;
	}

	int GetAttCnt()
	{
		int count = 0;
		for(size_t i = 0; i < 8; ++i)
		{
			if(_enhanced_essence.prop[i].id > 0) count++;
		}
		return count;
	}

	bool GetProp(int index, int & id, int & value, int & grade, float & grade_extra)
	{
		if(index < 0 || index >= 8) return false;

		id = _enhanced_essence.prop[index].id;
		value = _enhanced_essence.prop[index].value;
		grade = _enhanced_essence.prop[index].grade;
		grade_extra = _enhanced_essence.prop[index].grade_extra;

		return true;
	}	

	void SetProp(int index, int id, int value, int grade, float grade_extra)
	{
		if(index < 0 || index >= 8) return;

		_enhanced_essence.prop[index].id = id;
		_enhanced_essence.prop[index].value = value;
		_enhanced_essence.prop[index].grade = grade;
		_enhanced_essence.prop[index].grade_extra = grade_extra;
	}
	
	void SetRefineProp(int index, int id, int value, int grade, float grade_extra)
	{
		if(index < 0 || index >= 8) return;

		_enhanced_essence.refine_prop[index].id = id;
		_enhanced_essence.refine_prop[index].value = value;
		_enhanced_essence.refine_prop[index].grade = grade;
		_enhanced_essence.refine_prop[index].grade_extra = grade_extra;
	}

	int GetCashRefineCnt()
	{
		return _enhanced_essence.cash_refine_cnt;
	}
	
	int GetRefineCnt()
	{
		return _enhanced_essence.refine_cnt;
	}

	int GetRefineRemain()
	{
		return _enhanced_essence.refine_remain;
	}
	
	void AddRefineRemain(int cnt)
	{
		_enhanced_essence.refine_remain += cnt;
	}

	void DecRefineRemain()
	{
		_enhanced_essence.refine_remain --;
	}

	void SetRefineRemain(int cnt)
	{
		_enhanced_essence.refine_remain = cnt;
	}
	
	void IncRefineCnt()
	{
		_enhanced_essence.refine_cnt++;
	}

	void AddCashRefineCnt(int cnt)
	{
		_enhanced_essence.cash_refine_cnt += cnt;
	}

	int GetResetCnt()
	{
		return _enhanced_essence.reset_cnt;
	}

	void IncResetCnt()
	{
		_enhanced_essence.reset_cnt++;
	}

	int GetAvgGrade()
	{
		return _enhanced_essence.avg_grade;
	}

	void SetAvgGrade(int grade)
	{
		_enhanced_essence.avg_grade = grade;
	}

	bool GetRefineActive()
	{
		return _enhanced_essence.refine_active;
	}
	
	void SetRefineActive(bool active)
	{
		_enhanced_essence.refine_active = active;
	}

	void ReplaceRefineProp()
	{
		memcpy(_enhanced_essence.prop, _enhanced_essence.refine_prop, sizeof(_enhanced_essence.refine_prop));
		_enhanced_essence.quality = _enhanced_essence.refine_quality;
	}	

	void ClrRefineProp()
	{
		memset(_enhanced_essence.refine_prop, 0, sizeof(_enhanced_essence.refine_prop));
		_enhanced_essence.refine_quality = 0;
	}
public:
	static int script_QueryLevel(lua_State * L);
	static int script_QueryExp(lua_State * L);

	static int script_QueryQuality(lua_State * L);
	static int script_SetQuality(lua_State * L);
	static int script_SetRefineQuality(lua_State * L);
	static int script_GetHole(lua_State * L);
	
	static int script_GetAttCnt(lua_State * L);
	static int script_GetAtt(lua_State * L);
	static int script_SetAtt(lua_State * L);
	static int script_SetRefineAtt(lua_State * L);

	static int script_GetRefineCnt(lua_State * L);
	static int script_GetResetCnt(lua_State * L);

	static int script_GetAvgGrade(lua_State * L);
	static int script_SetAvgGrade(lua_State * L);

	static int script_QueryInnerData(lua_State * L);
	static int script_GetInnerDataCount(lua_State * L);
	static int script_SetInnerData(lua_State * L);
	static int script_ClearInnerData(lua_State * L);

};

class rune_item : public item_body
{
	int _tpl_quality;
	int _max_hole;
	int _level_required;
	int _require_reborn_count;

	bool CallScript(gactive_imp * pImp, rune_essence & ess, const char * function) const ;
public:
	rune_item(int quality, int max_hole, int level_required, int renascence_count)
	{
		_tpl_quality = quality;
		_max_hole = max_hole;
		_level_required = level_required;
		_require_reborn_count = renascence_count;
	}

	
	virtual void OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const;
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const;
	virtual void OnActivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnDeactivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnActivateProp(size_t index, gactive_imp *obj, item *parent) const;
	virtual void OnDeactivateProp(size_t index, gactive_imp *obj, item *parent) const;
	
	virtual int GetClientSize(const void * buf, size_t len) const;
	//virtual void GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len) const;
	virtual void GetItemDataForClient(item_data_client& data, const void * buf, size_t len) const;

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_RUNE;}


	virtual bool Identify(item *parent, gactive_imp *imp, size_t index1, size_t index2);
	virtual bool Customize(item * parent, gactive_imp * imp, int count, int prop[], int index, int type);
	virtual bool Combine(item *parent, gactive_imp *pImp, size_t index1, size_t index2);
	virtual bool RefineRune(item *parent, gactive_imp *pImp, size_t index1, int index2);
	virtual bool RefineAction(item *parent, gactive_imp *pImp, size_t index, bool accept_result);
	virtual bool Reset(item *parent, gactive_imp *pImp, size_t index1, size_t index2);
	virtual bool Decompose(item *parent, gactive_imp *imp, int & output_id, size_t index); 
	virtual bool LevelUp(item * parent, gactive_imp * pImp, size_t index, int & level);
	virtual bool OpenSlot(item * parent, gactive_imp * pImp,size_t index);
	virtual bool ChangeSlot(item * parent, gactive_imp * pImp, size_t index1, size_t index2);
	virtual bool EraseSlot(item * parent, gactive_imp * pImp, int rune_index, int slot_index, int & stone_id);
	virtual bool InstallSlot(item * parent, gactive_imp * pImp, int rune_index, int slot_index, int stone_id);

	//测试用
	virtual int GainExp(item::LOCATION, int exp, item * parent, gactive_imp * pImp, int index, bool& level_up)  const;
	virtual int64_t GetIdModify(const item * parent) const;


private:
	virtual int OnGetEquipMask() const 
	{
		return item::EQUIP_MASK_RUNE;
	}

	void EnhanceProp(int prop_id, int value, int cur_level, gactive_imp * imp) const;
	void ImpairProp(int prop_id, int value, int cur_level, gactive_imp * imp) const;
};


#endif 
