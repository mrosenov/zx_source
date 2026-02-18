#ifndef __ONLINEGAME_GS_COUPLE_JUMP_H__
#define __ONLINEGAME_GS_COUPLE_JUMP_H__

#include "../item.h"
#include "../config.h"

class item_remove_cd : public item_body
{
	enum 
	{
		SKILL_MAX_COUNT = 4
	};
	int _cooltime;
	int _skills[SKILL_MAX_COUNT];
	size_t _skills_count;

	int CheckUseCondition(gactive_imp* imp) const;
public:
	item_remove_cd(int cooltime, int skills[], size_t count);

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_CD_ERASER;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
};
#endif


