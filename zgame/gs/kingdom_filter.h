#ifndef __ONLINEGAME_GS_KINGDOM_FILTER_H__
#define __ONLINEGAME_GS_KINGDOM_FILTER_H__
#include "filter.h"

class kingdom_exp_filter : public filter
{

public:
	DECLARE_SUBSTANCE(kingdom_exp_filter);
	kingdom_exp_filter(gactive_imp * imp, int filter_id) : filter(object_interface(imp), 0)
	{
		_filter_id = filter_id;
	}

protected:	
	virtual void OnAttach();
	virtual void OnDetach(); 
	kingdom_exp_filter(){}
};


#endif
