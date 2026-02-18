#ifndef __ONLINE_GAME_GS_PLAYER_MOVE_SESSION_H__
#define __ONLINE_GAME_GS_PLAYER_MOVE_SESSION_H__
#include "actsession.h"
/*
	这是行走逻辑的低延迟版本
*/

struct move_control;
class session_p_start_move : public act_timer_session
{
protected:
	session_p_start_move(){}
	move_control & GetControl();
	int _base_time;
public:
	DECLARE_SUBSTANCE(session_p_start_move);
	explicit session_p_start_move(gplayer_imp * imp);
	~session_p_start_move()
	{
		if(_timer_index >=0) RemoveTimer();
	}
	virtual bool StartSession(act_session * next_cmd);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool Mutable(act_session * next_cmd);

	virtual int  GetMask()          //取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_MOVE;
	}

	virtual int  GetExclusiveMask() //设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}
};

class session_p_move: public session_p_start_move
{
public:
	session_p_move() {}
	DECLARE_SUBSTANCE(session_p_move);
protected:
	A3DVECTOR _target;
	A3DVECTOR _predict;
	int	_use_time;
	int	_seq;
	int	_cur_seq;
	unsigned short _speed;
	unsigned char _move_mode;
	bool 	_on_carrier;
	template <int>
	bool CheckCmdSeq()
	{
		int seq = _imp->_commander->GetCurMoveSeq();
		_cur_seq = seq;
		int seq_offset = (_seq - seq) & 0xFFFF;
		if(_seq != -1 && seq_offset > 2000)
		{
			//忽略本session
			return false;
		}

		if(_seq != -1  && seq_offset)
		{
			//是否应该记录一下日志
			_imp->_commander->SetNextMoveSeq(_seq);
		}
		else
		{
			//命令号进一
			_imp->_commander->GetNextMoveSeq();
		}
		return true;
	}

public:
	explicit session_p_move(gplayer_imp * imp):session_p_start_move(imp),_seq(-1),_cur_seq(-1),_speed(0),_move_mode(1), _on_carrier(false){}
	void SetPredictPos(const A3DVECTOR &pos) { _predict = pos; }
	void SetCmdSeq(int seq) { _seq = seq; }
	void SetDestination(unsigned short speed,unsigned char move_mode, 
				const A3DVECTOR & target,int use_time)
	{	
		_speed = speed;
		_move_mode = move_mode;
		_target = target;
		_use_time = use_time;
	}

	void SetCarrierMode(bool mode)
	{
		_on_carrier = mode;
	}
	
	virtual bool StartSession(act_session * next_cmd);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual int GetMoveTime()
	{
		int t = _use_time;
		if(t <= 0) t = 100;
		if(t > 1000) t = 1000;
		return t;
	}
};

class session_p_stop_move: public session_p_move
{
	unsigned short _dir;
	session_p_stop_move() {}
	int _tick;
public:
	DECLARE_SUBSTANCE(session_p_stop_move);
	explicit session_p_stop_move(gplayer_imp * imp):session_p_move(imp), _tick(-1){}
	void SetDir(unsigned short dir)
	{
		_dir = dir;
	}

	virtual int GetLimitTime();
	virtual void DoTimeoutLog();

	virtual bool StartSession(act_session * next_cmd);
	virtual bool EndSession();
	virtual bool Mutable(act_session * next_cmd) { return false;}
};
#endif

