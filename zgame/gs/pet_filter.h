#ifndef __ONLINEGAME_GS_PET_FILTERS_H__
#define __ONLINEGAME_GS_PET_FILTERS_H__

#include "filter.h"
#include "actobject.h"

class filter_pet_combine1 : public timeout_filter
{
	int _dmg_reduce;
	int _res_add;
	int _anti_crit_rate;
	enum 
	{
		MASK = FILTER_MASK_WEAK | FILTER_MASK_HEARTBEAT
	};
public:
	DECLARE_SUBSTANCE(filter_pet_combine1);

	filter_pet_combine1(){}

	filter_pet_combine1(gactive_imp* imp,int dmg_reduce,int res_add, int anti_crit_rate, int period): timeout_filter(object_interface(imp),period,MASK)
	{
		_filter_id = FILTER_INDEX_PET_COMBINE1;
		_dmg_reduce = dmg_reduce;
		_res_add = res_add;
		_anti_crit_rate = anti_crit_rate;
		if(_dmg_reduce < 20) _dmg_reduce = 20;
		if(_dmg_reduce > 250) _dmg_reduce = 250;
		if(_res_add < 5) _res_add = 5;
		if(_res_add > 100) _res_add = 100;
	}
	
private:
	virtual void OnAttach();
	virtual void OnDetach();
};

class filter_pet_combine2 : public timeout_filter
{
	int _effect;
	int _hp_add;
	int _mp_add;
	int _dmg_add;
	int _crit_rate;
	enum 
	{
		MASK = FILTER_MASK_WEAK | FILTER_MASK_HEARTBEAT
	};
public:
	DECLARE_SUBSTANCE(filter_pet_combine2);

	filter_pet_combine2(){}

	filter_pet_combine2(gactive_imp* imp,int effect, int crit_rate, int period): timeout_filter(object_interface(imp),period,MASK)
	{
		_filter_id = FILTER_INDEX_PET_COMBINE2;
		_effect = effect;
		_crit_rate = crit_rate;
		if(_effect < 3) _effect = 3;
		if(_effect > 100) _effect = 100;
		_hp_add = 0;
		_mp_add = 0;
		_dmg_add = 0;
	}
	
private:
	virtual void OnAttach();
	virtual void OnDetach();
};
#endif

