#include "actobject.h"
#include "pmvsession.h"
#include "world.h"
#include "gmatrix.h"
#include "npc.h"
#include "ainpc.h"
#include "aipolicy.h"
#include "clstab.h"
#include <common/protocol.h>
#include "npcgenerator.h"
#include "player_imp.h"
#include "skill_filter.h"
#include "sfilterdef.h"
#include <gsp_if.h>
#include "invincible_filter.h"
#include <typeinfo>

DEFINE_SUBSTANCE(session_p_start_move, act_session , CLS_SESSION_P_START_MOVE);
DEFINE_SUBSTANCE(session_p_move, act_session ,       CLS_SESSION_P_MOVE);
DEFINE_SUBSTANCE(session_p_stop_move, act_session ,  CLS_SESSION_P_STOP_MOVE);

move_control & session_p_start_move::GetControl()
{
	return ((gplayer_imp *)_imp)->_mcontrol;
}

session_p_start_move::session_p_start_move(gplayer_imp * imp):act_timer_session(imp),_base_time(0)
{
}

bool session_p_start_move::StartSession(act_session * next_cmd) 
{
	move_control & ctrl = GetControl();
	gplayer_imp *pImp = (gplayer_imp *) _imp;
	g_timer.get_systime(ctrl.val_start);

	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = pImp->GetNextSessionID();
	_self_id = pImp->_parent->ID;

	ctrl.start_seq = pImp->_commander->GetCurMoveSeq();
	ctrl.session_id =  _session_id;
	int tick = MILLISEC_TO_TICK(STD_MOVE_USE_TIME);	
	//这里的时间是为了占用一定时间的session队列 实际后面的任务可能会超过此时间，不过也不会引起大碍
	if(next_cmd)
	{
		//如果后面存在需要时间的任务,则直接用这个时间来做延迟时间
		int next_time = next_cmd->GetMoveTime();
		if(next_time > 0)
		{
			//要注意防止客户端的欺骗
			tick = MILLISEC_TO_TICK(next_time);
			if(tick <= 0) tick = 1;
		}
	}
	SetTimer(g_timer,tick,1);
	return true;
}

bool session_p_start_move::RepeatSession()
{
	return false;
}

bool session_p_start_move::EndSession()
{
	if(_session_id != -1)
	{
		g_timer.get_systime(GetControl().val_end);
#ifdef __TEST_ATTACK_DELAY__
		timeval tv4;
		gettimeofday(&tv4,NULL);
		GLog::log(GLOG_INFO, "%d session_p_start_move EndSession %d -- %ld.%06ld",_self_id.id, GetControl().GetMilliRange(),tv4.tv_sec,tv4.tv_usec);
#endif
	}
	return true;
}

bool session_p_start_move::Mutable(act_session * next_cmd)
{
	if(next_cmd == NULL) return false;	//测试的话直接返回
	move_control & ctrl = GetControl();
	if(ctrl.start_seq < 0) return false;

	ASSERT(_session_id != -1);
	int next_time = next_cmd->GetMoveTime();
	if(next_time > 0)
	{
		//考虑一下是否需要停止
		timeval end;
		g_timer.get_systime(end);
		int t = move_control::MilliRange(ctrl.val_start, end) + _base_time;
		int t2 = next_time - t;
		if(t2 < FAST_MOVE_SKIP_TIME)
		{
			//如果时间差在容许范围之内 就直接范围 ,由下一个任务来处理时间延迟 
#ifdef __TEST_ATTACK_DELAY__
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d %s Mutable end -- %ld.%06ld",_self_id.id,typeid(*this).name(),tv4.tv_sec ,tv4.tv_usec);
#endif
			return true;
		}

		//重新开启新的定时器
		RemoveTimer();
		//换一个Session ID，避免消息冲突
		_session_id = _imp->GetNextSessionID();
		ctrl.session_id =  _session_id;
		SetTimer(g_timer,MILLISEC_TO_TICK(t2),1);

#ifdef __TEST_ATTACK_DELAY__
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d %s Mutable restart %d (%d %d) -- %ld.%06ld",_self_id.id, typeid(*this).name(), MILLISEC_TO_TICK(t2), t, next_time, tv4.tv_sec ,tv4.tv_usec);
		}
#endif
		return false;
	}
	return true;
}


bool session_p_move::StartSession(act_session * next_cmd)
{
	if(!CheckCmdSeq<0>()) 
	{
		return false;
	}

	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;


	gplayer_imp *pImp = (gplayer_imp *) _imp;
	A3DVECTOR predict = _target;
	if(!_on_carrier)
	{
		_target -= _imp->_parent->pos;
	}
	else
	{
		_target -= pImp->GetParent()->rpos;
	}

	if(_use_time < MIN_MOVE_USE_TIME ) _use_time = MIN_MOVE_USE_TIME;
	if(!_on_carrier && !CheckPlayerMove(_imp,predict,_target,_move_mode,_use_time,_seq))
	{
		//超速了
		//拉回来
		return false;
	}

	//Add by Houjun 2011-07-05, 检查移动限制区域
	if(!_imp->CheckMoveArea(predict))
	{
		printf("超过玩家允许移动的范围，错误错误错误哦！！！！！！！\n");
		_imp->UpdateStopMove(_move_mode);

		int dir = _imp->_parent->dir;
		if(!_on_carrier)
		{
			_imp->_runner->stop_move(predict, _speed, dir, _move_mode);
		}
		else
		{
			_imp->_runner->player_stop_move_on_carrier(predict,_speed,dir,_move_mode);
		}
		return false;
	}

	_imp->UpdateMoveMode(_move_mode);

	//播放自己移动的消息，可能需要有一定的简化策略
	//比如对距离远的位置减少播放的频率等等

	if(!_on_carrier)
	{
		_imp->_runner->move(predict,_use_time,_speed,_move_mode);
	}
	else
	{
		_imp->_runner->player_move_on_carrier(predict,_use_time,_speed,_move_mode);
	}
	_imp->StepMove(_target);
	__PRINTF("MMMM MMMMM MMM:%f %f %f ---- %f %f %f\n",_target.x,_target.y,_target.z,
			_imp->_parent->pos.x,_imp->_parent->pos.y,_imp->_parent->pos.z);

	//计算延迟速度		
	move_control & ctrl = GetControl();
	if(ctrl.IsInValid(_cur_seq,_session_id))
	{
#ifdef __TEST_ATTACK_DELAY__
		GLog::log(GLOG_INFO, "%d session_p_move NormalSession",_self_id.id);
#endif
		//不是正常的start_move模式， 使用通常的方式
		ctrl.start_seq = -1;
		_cur_seq = -1;
		//测试延迟速度
		int tick = MILLISEC_TO_TICK(_use_time) - 1;
		if(tick <= 8) tick = 8;
		SetTimer(g_timer,tick,1);
	}
	else
	{
		//新的低延迟方式
		timeval end;
		g_timer.get_systime(end);
		int t = move_control::MilliRange(GetControl().val_start, end);
		t = t - _use_time;
		if(t > 0)
		{
			//进行空间限制和调整
			//前面的时间存在着可用的内容
			//最多使用其100ms的额度
			if(t > 100) t = 100;
			_base_time = t;
		}
		else
		{
			//前面session的时间不足，所以需要在这个时间内使用掉
			//考虑到
			if(t < -MAX_MOVE_USE_TIME) t = -MAX_MOVE_USE_TIME;
			_base_time = t;
		}
		ctrl.val_start = end;	//从现在开始计时  前面的结果已经纪入base_time
		ctrl.session_id = _session_id;
		ctrl.start_seq = _imp->_commander->GetCurMoveSeq();

		//按照base_time来设定延迟时间
		int tick = -_base_time;
		//base_time时间后，后面的命令就可以准备执行了

		//如果现在后面的命令已经到达并且可以获得该命令的执行时间
		//那么就用这个时间来进行延迟(这个延迟就是多久以后执行下一个命令)
		//如果后面的命令还没有到达，那么用一个估计的时间， 使用移动的平均时间来作为执行时间。
		int next_tick = STD_MOVE_USE_TIME; //若无后续命令，按照标准时间预设
		if(next_cmd)
		{
			//考虑后续的命令是否需要时间
			int next_time = next_cmd->GetMoveTime();
			if(next_time > 0) next_tick = next_time;	//此项操作是为了避免客户端欺骗
		}

		tick += next_tick;
		tick = MILLISEC_TO_TICK(tick);
		if(tick <= 0) tick = 1;

		SetTimer(g_timer,tick,1);

	}
	return true;
}

bool session_p_move::RepeatSession()
{
	return false;
}

bool session_p_move::EndSession()
{
	if(_session_id != -1)
	{
		move_control & ctrl = GetControl();
		if(ctrl.start_seq >= 0)
		{
			ctrl.val_start.tv_usec -= _base_time*1000;
			g_timer.get_systime(ctrl.val_end);
#ifdef __TEST_ATTACK_DELAY__
			{
				timeval tv4;
				gettimeofday(&tv4,NULL);
				GLog::log(GLOG_INFO, "%d session_p_move EndSession %d(%d) usetime:%d -- %ld.%06ld", _self_id.id, ctrl.GetMilliRange(),_base_time, _use_time , tv4.tv_sec ,tv4.tv_usec);
			}
#endif
		}
	}
	return true;
}


bool session_p_stop_move::StartSession(act_session * next_cmd)
{
	if(!CheckCmdSeq<0>()) 
	{
		return false;
	}

	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	gplayer_imp *pImp = (gplayer_imp *) _imp;
	A3DVECTOR pos = _target;
	if(!_on_carrier)
	{
		_target -= _imp->_parent->pos;
	}
	else
	{
		_target -= pImp->GetParent()->rpos;
	}
	//每次stopmove 都要记录以下
	if(_use_time < MIN_MOVE_USE_TIME ) _use_time = MIN_MOVE_USE_TIME;
	if(!_on_carrier && !CheckPlayerMove(_imp,pos,_target,_move_mode,_use_time,_seq))
	{	
		//超速了
		//拉回来
		return false;
	}

	//Add by Houjun 2011-07-05, 检查移动限制区域
	if(!_imp->CheckMoveArea(pos))
	{
		printf("Stop Move 超过玩家允许移动的范围，错误错误错误哦！！！！！！！\n");
		_imp->UpdateStopMove(_move_mode);

		if(!_on_carrier)
		{
			_imp->_runner->stop_move(pos, _speed, _dir, _move_mode);
		}
		else
		{
			_imp->_runner->player_stop_move_on_carrier(pos,_speed,_dir,_move_mode);
		}
		return false;
	}

	_imp->UpdateStopMove(_move_mode);

	if(!_on_carrier)
	{
		_imp->_runner->stop_move(pos,_speed,_dir,_move_mode);
	}
	else
	{
		_imp->_runner->player_stop_move_on_carrier(pos,_speed,_dir,_move_mode);
	}
	//发送停止移动的消息
	
	//进行真正的移动
	_imp->StepMove(_target);
	__PRINTF("MMMM SSSSS MMM:%f %f %f ---- %f %f %f\n",_target.x,_target.y,_target.z,
			_imp->_parent->pos.x,_imp->_parent->pos.y,_imp->_parent->pos.z);

	move_control & ctrl = GetControl();
	if(ctrl.IsInValid(_cur_seq,_session_id))
	{
#ifdef __TEST_ATTACK_DELAY__
		GLog::log(GLOG_INFO, "%d session_p_stop_move NormalSession",_self_id.id);
#endif
		//不是正常的start_move模式， 使用通常的方式
		ctrl.start_seq = -1;
		_cur_seq = -1;
		//测试延迟速度
		int t = MILLISEC_TO_TICK(_use_time);
		if(t < 8) t = 8;
		SetTimer(g_timer,t,1);
		return true;
	}
	else
	{
		//新的低延迟方式
		timeval end;
		g_timer.get_systime(end);
		int t = move_control::MilliRange(GetControl().val_start, end);
		t = t - _use_time;
		ctrl.start_seq = -1;
		_cur_seq = -1;
		
#ifdef __TEST_ATTACK_DELAY__
		int t1 = t;
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d session_p_stop_move t:%d usetime:%d -- %ld.%06ld", _self_id.id, t1,_use_time , tv4.tv_sec ,tv4.tv_usec);
		}
#endif

		if(t > 0) return false;
		if(t < -MAX_MOVE_USE_TIME) t = -MAX_MOVE_USE_TIME;

		//按照base_time来设定延迟时间
		int tick = -t;
#ifdef __TEST_ATTACK_DELAY__
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO, "%d session_p_stop_move t1:%d tick %d -- %ld.%06ld", _self_id.id, t, MILLISEC_TO_TICK(tick), tv4.tv_sec ,tv4.tv_usec);
		}
#endif
		tick = MILLISEC_TO_TICK(tick);
		if(tick <= 0) return false;

		SetTimer(g_timer,tick,1);

		//设置本操作需要多少tick，供超时判定之用
		_tick = tick;
		return true;

	}
	return false;
}

int 
session_p_stop_move::GetLimitTime()
{
	return _tick;
}


bool 
session_p_stop_move::EndSession()
{
	if(_session_id != -1)
	{
		move_control & ctrl = GetControl();
		ctrl.start_seq = -1;
		ctrl.session_id = -1;
	}
	return true;
}

void 
session_p_stop_move::DoTimeoutLog()
{
	std::string str;
	if(DumpContent(str))
	{
		GLog::log(GLOG_ERR, "%s", str.c_str());
	}
}

