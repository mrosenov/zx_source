#ifndef __ONLINEGAME_GS_CIRCLEOFDOOM_FILTER_H__
#define __ONLINEGAME_GS_CIRCLEOFDOOM_FILTER_H__

#include "filter.h"
#include <common/types.h>
#include <set>


//阵法发起者的filter，cod --- circle_of_doom
class cod_sponsor_filter : public filter
{
	enum
	{
		MASK = FILTER_MASK_HEARTBEAT | FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH 
			  | FILTER_MASK_NOSAVE,
	};

public:
	struct prepare_param
	{
		int faction;
		int faction_id;
		float radius;
		bool is_flymode;
		int skill_id;
	};

public:
	DECLARE_SUBSTANCE(cod_sponsor_filter);
	cod_sponsor_filter(gactive_imp * imp, float radius, int faction, int self_faction_id, bool is_flymode, int filter_id, int skill_id);
	~cod_sponsor_filter();
	
	virtual void Heartbeat(int tick);
	virtual void OnAttach();
	virtual void OnDetach();

private:
	cod_sponsor_filter() {}
	int BroadcastCODPrepareMassge();
	bool CheckTeamChange();

	float _radius;
	int _circle_max_count;
	int _faction;
	int _self_faction_id;
	bool _is_flymode;
	int _skill_session_id;
	int _team_member_count;
	int _skill_id;
	XID _team_list[TEAM_MEMBER_CAPACITY];
};


//阵法成员的filter，cod --- circle_of_doom
class cod_member_filter : public filter
{
	enum
	{
		MASK = FILTER_MASK_HEARTBEAT | FILTER_MASK_WEAK | FILTER_MASK_REMOVE_ON_DEATH 
			  | FILTER_MASK_NOSAVE,
	};

public:
	struct cod_param
	{
		float radius;
		bool is_flymode;
		int faction;
	};

public:
	DECLARE_SUBSTANCE(cod_member_filter);
	cod_member_filter(gactive_imp * imp, const XID & sponsor , const A3DVECTOR &pos, cod_param &param, int filter_id);
	~cod_member_filter();

	virtual void Heartbeat(int tick);
	virtual void OnAttach();
	virtual void OnDetach();

private:
	cod_member_filter() {}
	void CheckDistance();
	inline void LeaveCircleOfDoom();

	XID _sponsor_xid;
	A3DVECTOR _sponsor_pos;
	int _sponsor_faction;
	float _radius;
	bool _is_flymode;
};

class cod_cooldown_filter : public timeout_filter
{
	enum
	{
		MASK = FILTER_MASK_HEARTBEAT | FILTER_MASK_WEAK 
	};

public:
	DECLARE_SUBSTANCE(cod_cooldown_filter);
	cod_cooldown_filter(gactive_imp * imp, int timeout, int filter_id); 
	~cod_cooldown_filter(){}

	virtual void OnAttach();
	virtual void OnDetach();

private:
	cod_cooldown_filter() {}
};	

#endif
