#ifndef __ONLINEGAME_GS_RAGE_FILTER_H__
#define __ONLINEGAME_GS_RAGE_FILTER_H__

#include "filter.h"

class rage_filter : public filter
{
	enum
	{
		MASK =  FILTER_MASK_HEARTBEAT 
	};
	int _heartbeat_counter; 

public:
	rage_filter(gactive_imp * imp, int filter_id) : filter(object_interface(imp), MASK)
	{
		_heartbeat_counter = 0;
	}

	virtual void OnAttach()
	{
	}

	virtual void OnDetach()
	{
	}

	virtual void Heartbeat(int);
};


#endif
