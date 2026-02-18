#ifndef __ONLINEGAME_GS_DOUBLE_EXP_ITEM_H__
#define __ONLINEGAME_GS_DOUBLE_EXP_ITEM_H__

#include "../item.h"
#include "../config.h"

class item_dbl_exp : public item_body
{
	int _dbl_time;
	int _multi_exp;
public:
	item_dbl_exp(int time, int multi_exp):_dbl_time(time), _multi_exp(multi_exp)
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_DBL_EXP;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(3);}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
};
#endif


