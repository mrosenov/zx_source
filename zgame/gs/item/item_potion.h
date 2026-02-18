#ifndef __ONLINEGAME_GS_POTION_ITEM_H__
#define __ONLINEGAME_GS_POTION_ITEM_H__

#include "../item.h"
#include "../config.h"
#include "../cooldowncfg.h"
#include <common/types.h>


class item_base_potion : public item_body
{
protected:
	int _cooldown_time;
	bool _battleground_only;
	int _map_limit[5];
protected:
	item_base_potion(int cd_time):_cooldown_time(cd_time), _battleground_only(false){}
	
	virtual int GetCoolDownIndex() const = 0; 
	virtual void SetCoolDown(gactive_imp *imp) const;

public:
	item_body * SetBattleGroundFlag(bool flag)
	{
		_battleground_only = flag;
		return this;
	}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const ;

};

class item_step_healing_potion : public item_base_potion
{
	int _require_level;
	int _life[4];
	int _require_reborn_count;
public:
	item_step_healing_potion(int cd_time, int level, const int life[4], int reborn_cnt, const int map_limit[5]):item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 4; i++)
		{
			_life[i] = life[i];
		}
		
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
	virtual bool NeedUseLog() const { return false;}

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_STEP_HEALING; }
};

class item_step_mana_potion : public item_base_potion
{
	int _require_level;
	int _mana[4];
	int _require_reborn_count;
public:
	item_step_mana_potion(int cd_time, int level, const int mana[4], int reborn_cnt, const int map_limit[5])
		:item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 4; i++)
		{
			_mana[i] = mana[i];
		}
		
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
	virtual bool NeedUseLog() const { return false;}

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_STEP_MANA; }
};

class item_step_rejuvenation_potion : public item_base_potion
{
	int _require_level;
	int _life[4];
	int _mana[4];
	int _require_reborn_count;
public:
	item_step_rejuvenation_potion(int cd_time, int level, const int life[4], const int mana[4], int reborn_cnt, const int map_limit[5])
		:item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 4; i++)
		{
			_life[i] = life[i];
			_mana[i] = mana[i];
		}
		
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_STEP_REJU; }
};

struct healing_potion_essence
{
	int hp;
};

class item_healing_potion : public item_base_potion
{
	int _require_level;
	int _require_reborn_count;
public:
	item_healing_potion(int cd_time, int level, int reborn_cnt, const int map_limit[5]):item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}

	virtual void SetPotionCapacity(item* parent, int v);
	virtual int GetPotionCapacity(item* parent) const;
	virtual bool CanFillToBottle() const { return true; }

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_HEALING_POTION; }
};

struct mana_potion_essence
{
	int mp;
};

class item_mana_potion : public item_base_potion
{
	int _require_level;
	int _require_reborn_count;
public:
	item_mana_potion(int cd_time, int level, int reborn_cnt, const int map_limit[5]):item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}

	virtual void SetPotionCapacity(item* parent, int v);
	virtual int GetPotionCapacity(item* parent) const;
	virtual bool CanFillToBottle() const { return true; }

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_MANA_POTION; }
};

struct rejuvenation_potion_essence
{
	int hp;
	int mana;
};

class item_rejuvenation_potion : public item_base_potion
{
	int _require_level;
	int _require_reborn_count;
public:
	item_rejuvenation_potion(int cd_time, int level, int reborn_cnt, const int map_limit[5]):item_base_potion(cd_time),_require_level(level),_require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_REJU_POTION; }
};

struct deity_potion_essence
{
	int dp;
};

class item_deity_potion : public item_base_potion
{
	int _require_level;
	int _require_reborn_count;
public:
	item_deity_potion(int cd_time, int level, int reborn_cnt, const int map_limit[5]):item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}

	virtual void SetPotionCapacity(item* parent, int v);
	virtual int GetPotionCapacity(item* parent) const;
	virtual bool CanFillToBottle() const { return true; }

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_DEITY_POTION; }
};

class item_step_deity_potion : public item_base_potion
{
	int _require_level;
	int _dp[4];
	int _require_reborn_count;
public:
	item_step_deity_potion(int cd_time, int level, const int dp[4], int reborn_cnt, const int map_limit[5]):item_base_potion(cd_time), _require_level(level), _require_reborn_count(reborn_cnt)
	{
		for(size_t i = 0; i < 4; i++)
		{
			_dp[i] = dp[i];
		}
		
		for(size_t i = 0; i < 5; i++)
		{
			_map_limit[i] = map_limit[i];
		}
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_POTION;}

	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
	virtual bool NeedUseLog() const { return false;}

protected:
	virtual int GetCoolDownIndex() const {return COOLDOWN_INDEX_STEP_DEITY; }
};

#endif


