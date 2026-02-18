#ifndef __ONLINEGAME_GS_ASTROLOGY_ITEM_H__
#define __ONLINEGAME_GS_ASTROLOGY_ITEM_H__

#include "../item.h"
#include "../config.h"

#pragma pack(1)


enum
{
	MAX_ASTROLOGY_PROP_NUM  = 5,
	MAX_ASTROLOGY_LEVEL = 10,
};

struct astrology_prop
{
	int id;
	int value;
};

struct astrology_essence
{
	int level;			//等级
	int score;			//评分
	int energy_consume;		//精力消耗
	astrology_prop prop[6];		//属性值(和元魂类似)
	int reserver1;
	int reserver2;

public:
	int GetCurLevel(){ return level;}
	void SetCurLevel(int value) { level = value;}

	int GetScore(){ return score;}
	void SetScore(int value) { score = value;} 

	int GetEnergyConsume() { return energy_consume;}
	void SetEnergyConsume(int value) { energy_consume = value;}

	bool GetProp(int index, int & id, int & value) 
	{
		if(index < 0 || index >= MAX_ASTROLOGY_PROP_NUM) return false;

		id = prop[index].id;
		value = prop[index].value;
		return true;
	}	
	
	void SetProp(int index, int id, int value)
	{
		if(index < 0 || index >= MAX_ASTROLOGY_PROP_NUM) return;

		prop[index].id = id;
		prop[index].value = value;
	}
	
	static int script_GetLevel(lua_State * L);
	static int script_SetLevel(lua_State * L);

	static int script_GetScore(lua_State * L);
	static int script_SetScore(lua_State * L);
	
	static int script_GetEnergyConsume(lua_State * L);
	static int script_SetEnergyConsume(lua_State * L);
	
	static int script_GetProp(lua_State * L);
	static int script_SetProp(lua_State * L);
		
};

#pragma pack()


class item_astrology : public item_body
{
private:
	int _equip_mask;
	int _reborn_count;

public:
	item_astrology(int equip_mask, int reborn_count) : _equip_mask(equip_mask), _reborn_count(reborn_count){}

	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_ASTROLOGY;}
	virtual int OnGetEquipMask() const { return _equip_mask; }
	int GetRebornCount() const { return _reborn_count;}
	

	astrology_essence * GetDataEssence(item * parent) const 
	{
		size_t len;
		void * buf = parent->GetContent(len);
		if(len == sizeof(astrology_essence))
		{
			return (astrology_essence*)buf;
		}
		return NULL;
	}
	

	virtual bool AstrologyIdentify(gactive_imp * obj, item * parent, size_t item_index, int item_id); 
	virtual bool AstrologyUpgrade(gactive_imp * obj, item * parent, size_t item_index, int item_id, size_t stone_index, int stone_id); 
	virtual bool AstrologyDestroy(gactive_imp * obj, item * parent, size_t item_index, int item_id); 
	


};

class item_astrology_energy : public item_body
{
private:
	int energy;

public:
	item_astrology_energy(int en) : energy(en){}
	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_ASTROLOGY_ENERGY;}

};


#endif
