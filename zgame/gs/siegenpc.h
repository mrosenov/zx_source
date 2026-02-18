#ifndef __ONLINE_GAME_GS_SIEGE_NPC_H__
#define __ONLINE_GAME_GS_SIEGE_NPC_H__

#include "aipolicy.h"
#include "guardnpc.h"

#define TURRET_RANGE_A 3.0f
#define TURRET_RANGE_B 47.0f
#define TURRET_HP_DEC_PER_SECOND 0.025f

class turret_policy : public guard_policy
{
	virtual float GetReturnHomeRange() { return 1e30;}
	int _leader_counter;
	int _send_notify_counter;
public:
	enum {
		SEND_NOTIFY_COUNTER = 3,
		SEND_NOTIFY_COUNTER_NO_LEADER = 5,
	};
public:
	DECLARE_SUBSTANCE(turret_policy);
	turret_policy():_leader_counter(0),_send_notify_counter(0)
	{}
	virtual ~turret_policy() {}
	virtual void OnHeartbeat();
	virtual void Init(const ai_object & self, const ai_param & aip);
	virtual bool ChangeTurretMaster(const XID & target);
	virtual void OnAggro();			//仇恨度变化事件
	virtual bool KeepMagicCastRange() { return false;}
	virtual void OnDeath(const XID & attacker);

protected:
	void ClearLeader();
	void SendNotify(int id);
};

#endif

