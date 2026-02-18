
#ifndef __ONLINEGAME_GS_ITEM_FUWEN_H__
#define __ONLINEGAME_GS_ITEM_FUWEN_H__

#include "../item.h"


struct fuwen_essence
{
	int exp;
	int reserver1;	
	int reserver2;	
};

class item_fuwen: public item_body
{

public:
	item_fuwen(int quality, int prop_type, float value, int self_exp)
	{
		_quality = quality;
		_prop_type = prop_type;
		_value = value;
		_self_exp = self_exp;
	}

	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_FUWEN;}
	virtual void OnActivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual void OnDeactivate(size_t index,gactive_imp* obj, item * parent) const;
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const{ return true;}
	virtual void OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const;
	virtual void OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const;

	void UpgradeFuwen(item *parent, gactive_imp *pImp, size_t main_fuwen_index, int main_fuwen_where, size_t assist_count, int assist_fuwen_index[]); 

	int GetFuwenSelfExp() const { return _self_exp;}
	int GetFuwenPropType() const {return _prop_type;}
	int GetFuwenLevel(item * parent) const;

private:
	int _quality;
	int _prop_type;
	float _value;
	int _self_exp;

};

#endif //__ONLINEGAME_GS_ITEM_FUWEN_H__
