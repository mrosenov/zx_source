#ifndef __ONLINEGAME_GS_SKILL_ITEM_H__
#define __ONLINEGAME_GS_SKILL_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_skill : public item_body
{
	int _skill_id;
	int _skill_level;
	int _cooldown_type;
	int _level_require;
	bool _battleground_only;
	bool _inconsumable;
	int _require_cultivation;
public:
	item_skill(int id,int level, int ctype, int level_require, bool battleground_only, bool inconsumable, int require_cultivation)
	{
		_skill_id	= id;
		_skill_level	= level;
		_cooldown_type	= ctype;
		_level_require	= level_require;
		_battleground_only  = battleground_only;
		_inconsumable = inconsumable;
		_require_cultivation = require_cultivation;
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_SKILL;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const;
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(1.5f);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual int GetItemSkillLevel(int skill_id, gactive_imp *pImp) const; 
	virtual bool IsConsumableItemSkill() const {return !_inconsumable;}
};
#endif


