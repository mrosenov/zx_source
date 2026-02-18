#ifndef __ONLINEGAME_GS_KINGDOM_BATH_FILTER_H__
#define __ONLINEGAME_GS_KINGDOM_BATH_FILTER_H__

#include "../filter.h"
#include "../actobject.h"

class kingdom_bath_filter : public filter
{
protected:
	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT, 
	};
	kingdom_bath_filter(){}

public:
	DECLARE_SUBSTANCE(kingdom_bath_filter);
	kingdom_bath_filter(gactive_imp * imp,  int filter_id, int timeout,  bool is_leader, int award_period, int bath_count)
		: filter(object_interface(imp), FILTER_MASK)
	{
		_timeout = timeout;
		_filter_id = filter_id;
		_is_leader = is_leader;
		_award_period = award_period;
		_bath_count = bath_count;
		_counter = 0;
	}	     

	virtual void OnAttach();
	virtual void OnDetach(); 
	virtual void Heartbeat(int tick);

private:
	bool _is_leader;
	int _timeout;
	int _award_period;
	int _counter;
	int _bath_count;
};


#endif
