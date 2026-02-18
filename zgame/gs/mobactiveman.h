#ifndef __ONLINE_GAME_GS_MOB_ACTIVE_MAN_H__
#define __ONLINE_GAME_GS_MOB_ACTIVE_MAN_H__

#include <common/types.h>
#include <common/protocol.h>

class gplayer_imp;
class mob_active_imp;
class mobactive_manager
{
	XID _mobactive_bind_target;
	int _interrupt_condition;
	int _mobactive_state;
	int _active_mode; // 主体操作: 0 - 物品, 1 - 人物
	int _active_time[3];
	int _time_out;
	int _active_mount_point; // 挂载的模板
	int _active_pos; // 挂载的序号
	int _path_id;
	int _player_can_be_finish; // 互动物品玩家是否可以点击主动完成


protected:

	void SendErrorMessage(gplayer_imp *pImp, int message);

public:
	enum
	{
		MOB_ACTIVE_STATE_START = 0, //互动开始
		MOB_ACTIVE_STATE_RUN = 1, //互动过程
		MOB_ACTIVE_STATE_ENDING = 2, //互动结束
		MOB_ACTIVE_STATE_FINISH = 3, //互动结束完成
	};

	enum
	{
		MOB_ACTIVE_INTERRUPT_BY_ATTACK = 0x01,
		MOB_ACTIVE_INTERRUPT_BY_MOVE = 0x02,
	};

	enum
	{
		MONSTER_ACTIVE = 0,
		PLAYER_ACTIVE = 1,
	};

	mobactive_manager():_mobactive_bind_target(-1,-1),_interrupt_condition(-1),_mobactive_state(-1),_active_mode(-1),
	_time_out(-1),_active_mount_point(-1),_active_pos(-1),_path_id(-1),_player_can_be_finish(-1)
	{
		memset(_active_time,0,sizeof(_active_time));
	}

	void Swap(mobactive_manager& rhs)
	{
		mobactive_manager tmp = rhs;
		rhs = *this;
		*this = tmp;
	}
	bool CheckPlayerMobActiveRequest(gplayer_imp *pImp);

	void PlayerBeAttacked(gplayer_imp* pImp);
	void PlayerBeMoved(gplayer_imp* pImp);
	void PlayerMobActive(gplayer_imp * pImp, const C2S::CMD::mobactive_start &ma);
	void PlayerMobActiveFinish(gplayer_imp * pImp);
	void PlayerMobActiveCancel(gplayer_imp *pImp);

	void SetBindTarget(gplayer_imp * pImp, const XID & target);

	void SetMobActiveState(int state)
	{
		_mobactive_state = state;
		_time_out = _active_time[state] ;
	}

	char GetMobActiveState()
	{
		return _mobactive_state;
	}

	void SetMobActiveMode(int mode)
	{
		_active_mode = mode;
	}

	void SetMobActiveMountPoint(int active_mount_point)
	{
		_active_mount_point = active_mount_point;
	}

	void SetMobActivePos(int active_pos)
	{
		_active_pos = active_pos;
	}

	void SetMobActiveTime(int* active_time,int num)
	{
		for (int cnt = 0; cnt < num; cnt++)
		{
			_active_time[cnt] = active_time[cnt];
		}
	}

	void SetMobActivePathId(int path_id)
	{
		_path_id = path_id;
	}

	void Reset()
	{
		_mobactive_bind_target = XID(-1,-1);
		_interrupt_condition = -1;
		_mobactive_state = -1;
		_active_mode = -1;
		_time_out = -1;
		_active_mount_point = -1;
		_active_pos = -1;
		_path_id = -1;
		_player_can_be_finish = -1;

		memset(_active_time,0,sizeof(_active_time));
	}

	bool IsActive()
	{
		return _mobactive_bind_target != XID(-1,-1);
	}

	void SetMobActiveInterruptCondition();

	void Heartbeat(gplayer_imp* pImp);

	void MsgMobActiveStateStart(gplayer_imp* pImp);

	void MsgMobActiveStateRun(gplayer_imp* pImp);

	void MsgMobActiveStateEnding(gplayer_imp* pImp);

	void MsgMobActiveStateFinish(gplayer_imp* pImp);

	void MsgMobActiveStateCancel(gplayer_imp* pImp);	

	void MobActiveStateStartCtrl(gplayer_imp* pImp);

	void MobActiveStateRunCtrl(gplayer_imp* pImp);

	void MobActiveStateEndingCtrl(gplayer_imp* pImp);

	void MobActiveSyncPos(gplayer_imp* pImp);

	void DenyCmd(gplayer_imp* pImp);

	void AllowCmd(gplayer_imp* pImp);

	void DeliverMobActiveTask(gplayer_imp* pImp, int state);

	void SetPlayerMobActiveLeavePos(gplayer_imp* pImp);
};
#endif
