#ifndef __ONLINEGAME_GS_PUPPET_FILTER_H__
#define __ONLINEGAME_GS_PUPPET_FILTER_H__


#include "filter.h"
#include "sfilterdef.h"

class puppet_filter : public timeout_filter
{
public:
	enum 
	{
		MASK =  FILTER_MASK_HEARTBEAT | FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH 
	};

	DECLARE_SUBSTANCE(puppet_filter);

	puppet_filter(gactive_imp * imp,int filter_id,int timeout)
		:timeout_filter(object_interface(imp),timeout, MASK)
	{
		_filter_id = filter_id;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach(); 
	puppet_filter(){}

};

class immune_death_filter : public timeout_filter
{
public:
	enum 
	{
		MASK =  FILTER_MASK_HEARTBEAT | FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH 
	};
	DECLARE_SUBSTANCE(immune_death_filter);

	immune_death_filter(gactive_imp * imp,int filter_id,int timeout)
		:timeout_filter(object_interface(imp),timeout, MASK)
	{
		_filter_id = filter_id;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach(); 
	immune_death_filter(){}
};



#endif
