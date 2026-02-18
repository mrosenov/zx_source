#ifndef __ONLINEGAME_GS_TRAVEL_FILTER_H__
#define __ONLINEGAME_GS_TRAVEL_FILTER_H__

#include "filter.h"

class travel_filter : public timeout_filter
{
	enum
	{
		MASK =  FILTER_MASK_HEARTBEAT 
	};

	int _travel_vehicle_id;
	float _travel_speed;
	int _travel_path_id;
	int _travel_max_time;
	int _travel_map_id;

public:
	travel_filter(gactive_imp * imp, int filter_id, int travel_vehicle_id, float travel_speed, int travel_path_id, 
			int travle_max_time, int travel_map_id) : timeout_filter(object_interface(imp),travle_max_time, MASK)
	{
		_travel_vehicle_id = travel_vehicle_id;
		_travel_speed = travel_speed;
		_travel_path_id = travel_path_id;
		_travel_map_id = travel_map_id;
		_travel_max_time = travle_max_time;
		_filter_id = filter_id;
	}

	virtual void OnAttach();
	virtual void OnDetach();
	virtual void Heartbeat(int);

};

class charge_filter : public timeout_filter
{
	enum
	{
		MASK =  FILTER_MASK_HEARTBEAT 
	};
public:
	charge_filter(gactive_imp * imp, int filter_id, int timeout) : timeout_filter(object_interface(imp), timeout, MASK)
	{
		_filter_id = filter_id;
	}

	virtual void OnAttach();
	virtual void OnDetach();
	
};

#endif
