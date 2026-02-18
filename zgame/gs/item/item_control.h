#ifndef __ONLINEGAME_GS_ITEM_CONTROL_H__
#define __ONLINEGAME_GS_ITEM_CONTROL_H__

#include "../item.h"
#include "../config.h"

class item_control_mob : public item_body
{
public:
	virtual bool ArmorDecDurability(int) { return false;}
	virtual ITEM_TYPE GetItemType() const {return ITEM_TYPE_TANK_CONTROL; }
private:
	virtual unsigned short GetDataCRC() { return 0; }
	virtual bool Load(archive & ar)
	{
		return true;
	}
	virtual void GetItemData(const void ** data, size_t &len)
	{
		*data = "";
		len = 0;
	}
	virtual item_body* Clone() const { return  new item_control_mob (*this); }
	virtual int OnUseWithTarget(item::LOCATION l,size_t index, gactive_imp * obj,const XID & target,char force_attack,item * parent) const;
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj) { return false;}
	virtual bool IsItemCanUseWithTarget(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return SECOND_TO_TICK(5); }
public:

};
#endif

