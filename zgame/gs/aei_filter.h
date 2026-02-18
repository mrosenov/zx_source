#ifndef __ONLINE_GAME_AUTO_ESCAPE_INSTANCE_FILTER_H__
#define __ONLINE_GAME_AUTO_ESCAPE_INSTANCE_FILTER_H__

#include "filter.h"
#include "worldmanager.h"
#include "faction.h"

//用于判断玩家是否应该脱出副本的filter

class aei_filter : public filter
{
	int _state;
	int _timeout;
	int _worldtag;
	instance_hash_key _key;

	enum
	{	
		NORMAL,
		WAIT_ESCAPE,
	};

	enum 
	{
		MASK = FILTER_MASK_HEARTBEAT | FILTER_MASK_NOSAVE | FILTER_MASK_UNIQUE 
	};
public:
	DECLARE_SUBSTANCE(aei_filter);
	aei_filter(gactive_imp * imp,int filter_id)
		:filter(object_interface(imp),MASK),_state(0), _timeout(0),_worldtag(-1)
	{
		_filter_id = filter_id;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach();
	virtual void Heartbeat(int tick);
	virtual bool Save(archive & ar)
	{
		ASSERT(false);
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ASSERT(false);
		return false;
	}
	
	aei_filter() {}
};

class aebf_filter : public filter
{
	int _timeout;
	int _origin_mafia;
	int _battle_result;
	int _battle_end_timer;
	int _attacker_score;
	int _defender_score;
	int _faction;
	int _kickout;


	enum 
	{
		MASK = FILTER_MASK_HEARTBEAT | FILTER_MASK_NOSAVE | FILTER_MASK_UNIQUE 
	};
public:
	DECLARE_SUBSTANCE(aebf_filter);
	aebf_filter(gactive_imp * imp,int filter_id, int origin_mafia)
		:filter(object_interface(imp),MASK),_timeout(0),_origin_mafia(origin_mafia)
	{
		_filter_id = filter_id;
		_battle_result = 0;
		_battle_end_timer = 0;
		_attacker_score = -1;
		_defender_score = -1;
		_faction = _parent.GetFaction();
		_kickout = 0;
	}

protected:
	virtual void OnAttach();
	virtual void OnDetach(); 
	virtual void Heartbeat(int tick);
	virtual bool Save(archive & ar)
	{
		ASSERT(false);
		return true;
	}

	virtual bool Load(archive & ar)
	{
		ASSERT(false);
		return false;
	}
	
	aebf_filter():_timeout(0),_origin_mafia(0) {}
	virtual void  OnModify(int ctrlname,void * ctrlval,size_t ctrllen);
};

#endif

