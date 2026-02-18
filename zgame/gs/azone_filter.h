#ifndef __ONLINEGAME_GS_AZONE_FILTER_H__
#define __ONLINEGAME_GS_AZONE_FILTER_H__
#include "filter.h"

class azone_filter : public filter
{
	int _skill_id;
	int _skill_level;
	float _radis;
	int _count;
	int _mp_cost;
	char _force;
	char _is_helpful;
	int _var1;
	int _var2;
	int _visible_state;

	enum 
	{
		MASK = FILTER_MASK_UNIQUE | FILTER_MASK_BUFF | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_HEARTBEAT | FILTER_MASK_ON_MOVED 
	};

public:
	DECLARE_SUBSTANCE(azone_filter);

	azone_filter(gactive_imp * imp, int filter_id, int skill_id, int skill_level, float radis, int count, int mp_cost, char force, char is_helpful, int var1, int var2, int visible_state)
		: filter(object_interface(imp), MASK), _skill_id(skill_id),_skill_level(skill_level),_radis(radis),_count(count),_mp_cost(mp_cost),_force(force),_is_helpful(is_helpful),
		_var1(var1), _var2(var2),_visible_state(visible_state)
	{
		_filter_id = filter_id;
	}

protected:	
	virtual void OnAttach();
	virtual void OnDetach(); 
	virtual void OnMoved()
	{
		_is_deleted = true;
	}
	
	virtual void Heartbeat(int tick);
	virtual bool Save(archive & ar)
	{
		filter::Save(ar);
		ar << _skill_id << _skill_level << _radis << _count << _mp_cost << _force << _is_helpful << _var1 << _var2 << _visible_state; 
		return true;
	}

	virtual bool Load(archive & ar)
	{
		filter::Load(ar);
		ar >> _skill_id >> _skill_level >> _radis >> _count >> _mp_cost >> _force >> _is_helpful >> _var1 >> _var2 >> _visible_state;
		return true;
	}



	azone_filter(){}
};	



#endif
