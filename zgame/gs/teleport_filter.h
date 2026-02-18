#ifndef __ONLINEGAME_GS_TELEPORT_FILTER_H__
#define __ONLINEGAME_GS_TELEPORT_FILTER_H__

#include "filter.h"
#include "sfilterdef.h"

class teleport1_filter : public timeout_filter
{
public:
	enum 
	{
		MASK =  FILTER_MASK_HEARTBEAT | FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH 
	};

	DECLARE_SUBSTANCE(teleport1_filter);
	teleport1_filter(gactive_imp * imp,int filter_id,int timeout)
		:timeout_filter(object_interface(imp),timeout, MASK)
	{
		_filter_id = filter_id;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach(); 
	teleport1_filter(){}

};

class teleport2_filter : public timeout_filter
{
public:
	enum 
	{
		MASK =  FILTER_MASK_HEARTBEAT | FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH 
	};

	DECLARE_SUBSTANCE(teleport2_filter);
	teleport2_filter(gactive_imp * imp,int filter_id,int timeout)
		:timeout_filter(object_interface(imp),timeout, MASK)
	{
		_filter_id = filter_id;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach(); 
	teleport2_filter(){}

};


#endif
