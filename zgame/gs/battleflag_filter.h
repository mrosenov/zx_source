#ifndef __ONLINEGAME_GS_BATTLEFLAG_FILTER_H__
#define __ONLINEGAME_GS_BATTLEFLAG_FILTER_H__

#include "filter.h"
#include "actobject.h"
#include "config.h"
#include "statedef.h"

class battleflag_filter : public timeout_filter
{
protected:
	enum
	{
		FILTER_MASK = FILTER_MASK_UNIQUE | FILTER_MASK_HEARTBEAT
	};
	battleflag_filter(){}

public:
	int hp;
	int mp;
	int attack;
	int defence;
	int dec_damage;
	int crit;
	int crit_damage;
	int resistance[6];					//6个特殊魔法抗性 (眩晕 虚弱 定身 魅惑 昏睡 减速)
	int cult_defense[CULT_DEF_NUMBER];	//造化防御 仙 魔 佛 备选1 2 3
	int cult_attack[CULT_DEF_NUMBER];	//造化阵营相克 仙 魔 佛 备选1 2 3

public:
	DECLARE_SUBSTANCE(battleflag_filter);
	battleflag_filter(gactive_imp * imp, int time) : timeout_filter(object_interface(imp), time, FILTER_MASK)
	{
		_filter_id = FILTER_INDEX_BATTLEFLAG;
		_buff_id = GNET::HSTATE_BATTLEFLAG;
		hp = 0;
		mp = 0;
		attack = 0;
		defence = 0;
		dec_damage = 0;
		crit = 0;
		crit_damage = 0;
		memset(resistance, 0, sizeof(resistance));
		memset(cult_defense, 0, sizeof(cult_defense));
		memset(cult_attack, 0, sizeof(cult_attack));
	}
	
	virtual bool Save(archive & ar)
	{
		timeout_filter::Save(ar);
		ar << hp << mp << attack << defence << dec_damage << crit << crit_damage;
		ar.push_back(resistance, sizeof(resistance));
		ar.push_back(cult_defense, sizeof(cult_defense));
		ar.push_back(cult_attack, sizeof(cult_attack));
		return true;
	}

	virtual bool Load(archive & ar)
	{
		timeout_filter::Load(ar);
		timeout_filter::Save(ar);
		ar >> hp >> mp >> attack >> defence >> dec_damage >> crit >> crit_damage;
		ar.pop_back(resistance, sizeof(resistance));
		ar.pop_back(cult_defense, sizeof(cult_defense));
		ar.pop_back(cult_attack, sizeof(cult_attack));
		return true;
	}

private:
	virtual void OnAttach();
	virtual void OnDetach();
};

#endif

