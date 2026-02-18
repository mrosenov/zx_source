#ifndef __ONLINEGAME_GS_TELEPORT_ITEM_H__
#define __ONLINEGAME_GS_TELEPORT_ITEM_H__

#include "../item.h"
#include "../config.h"
#include <common/types.h>

class item_teleport : public item_body
{
	int _tag;
	A3DVECTOR _pos;
	int _reborn_cnt;
	int _require_level;
	int _use_time;
public:
	item_teleport(int tag,float x, float y ,float z, int reborn_cnt, int require_level, float use_time=0.f):_tag(tag),_pos(x,y,z), _reborn_cnt(reborn_cnt),
		_require_level(require_level),_use_time(SECOND_TO_TICK(use_time))
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_TELEPORT;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return _use_time;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
};
#endif



