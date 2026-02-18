#ifndef __ONLINEGAME_GS_PROTECTED_NPC_H__
#define __ONLINEGAME_GS_PROTECTED_NPC_H__

#include "servicenpc.h"

class protected_npc : public service_npc
{
	int _owner_id;
	int _task_id;

	enum
	{
		enumTaskProtectNPCSuccess = 0,	// 护送成功
		enumTaskProtectNPCDie,		// 护送的NPC死亡
		enumTaskProtectNPCTooFar,		// 玩家距离过远
		enumTaskProtectNPCTimeOut,	// 超时
	};

public:
	DECLARE_SUBSTANCE(protected_npc);

public:
	protected_npc() : _owner_id(0), _task_id(0){}
	~protected_npc(){}

	void SetProtectedID(int id)
	{
		_owner_id = id;
	}

	int GetProtectedID() const
	{
		return _owner_id;
	}

	void SetTaskID(int taskid)
	{
		_task_id = taskid;
	}

	int GetTaskID() const 
	{
		return _task_id;
	}
	  

	virtual void OnDeath(const XID & lastattack,bool is_invader, bool no_drop, bool is_hostile_duel, int time);
	virtual void OnProtectedFinish();

};


#endif
