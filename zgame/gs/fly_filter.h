#ifndef __ONLINEGAME_GS_FLY_FILTER_H__
#define __ONLINEGAME_GS_FLY_FILTER_H__
#include "filter.h"

class aircraft_filter : public filter
{
	float _speed;
	int   _cost;
	int   _exp_add;
	int   _exp_counter;
	enum
	{
		MASK = FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT
	};
public:
	aircraft_filter(gactive_imp * imp,int filter_id, float speed, int cost_per_sec, int exp_add)
		:filter(object_interface(imp),MASK)
	{
		_filter_id = filter_id;
		_speed = speed;
		_cost = cost_per_sec;
		_exp_add = exp_add;
		_exp_counter = 0;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void Heartbeat(int);
};

#endif


