#ifndef __ONLINEGAME_GS_ACT_SESSION_H__
#define __ONLINEGAME_GS_ACT_SESSION_H__

#include <common/types.h>
#include <amemobj.h>
#include <timer.h>
#include "config.h"
#include "substance.h"
#include "pathfinding/chaseinfo.h"

typedef CChaseInfo chase_info;
class	dispatcher;
class	controller;
class 	gactive_imp;
class	world;
struct recipe_template;
class gplayer_imp;

bool CheckPlayerMove(gactive_imp * obj, const A3DVECTOR & target,const A3DVECTOR & offset, int mode, int use_time,int seq);

//暂定名 这是表明了物体现在的session的名称
//操作队列也可以直接以session的方式存在
class act_session : public substance
{
protected:
	act_session(): _imp(0), _session_id(-1){}
public:
	gactive_imp * _imp;
	int	_session_id;
	unsigned int _start_tick;		//session 开始时的tick数
	world	* _plane;
public:
	DECLARE_SUBSTANCE(act_session);
	explicit act_session(gactive_imp * imp);
	virtual void Restore(gactive_imp * imp,int session_id = -1);

	void NPCSessionStart(int task_id);
	void NPCSessionEnd(int task_id, int retcode);
	bool NPCGetNextWaypoint(A3DVECTOR & target);
	void NPCSessionUpdateChaseInfo(int task_id, const chase_info & info);

	virtual ~act_session(){}
	virtual bool StartSession(act_session * next_cmd = NULL) = 0; 	//session正式开始
	virtual bool EndSession() = 0;		//session完成
	virtual bool RepeatSession() = 0;	//重复完成session
	virtual bool TerminateSession(bool force = true) = 0;	//session中止
	virtual bool IsTimeSpent() = 0;		//这个session执行是否需要时间
	virtual int  GetMask() = 0;		//取得自己的mask，表明自己在队伍中的身分
	virtual int  GetExclusiveMask() = 0;	//设置排他的mask，表明会排除队列里的哪些session
	virtual bool OnAttacked() { return false;}
	virtual bool OnSilentSeal(){return false;}
	virtual bool OnBreakCast(){return false;}
	virtual bool IsSkillPerforming(int id) { return false; }
	virtual bool RestartSession()  { return true;}
	virtual bool Mutable(act_session * next_cmd) { return false;} //可以随意中断的协议 next_cmd == NULL 表示测试
	virtual int GetMoveTime() { return -1;}	 //只有p_move系列使用这个接口
	virtual int GetLimitTime() { return -1;} //只有p_move系列使用这个接口
	
	virtual void DoTimeoutLog() {}		//do nothing
public:
	void SendMsg(int msg,const XID & target,const XID & source);
	void SendRepeatMsg(const XID & self);
	void SendForceRepeat(const XID & self);
	void SendEndMsg(const XID & self);
	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}

	enum{

		SS_MASK_MOVE 	= 0x01,
		SS_MASK_ATTACK 	= 0x02,
		SS_MASK_SITDOWN	= 0x04,
		SS_MASK_RESURRECT = 0x08,
		SS_MASK_USE_ITEM = 0x10,
		SS_MASK_SKILL	= 0x20,
		SS_MASK_FOLLOW_TARGET = 0x40,
		SS_MASK_SPIRIT_SKILL = 0x80,
	};
	
};

class session_keeping_sitdown : public act_session
{
public:
	session_keeping_sitdown(gplayer_imp *pImp)
		:act_session((gactive_imp*)pImp)
	{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession();
	virtual bool RepeatSession() {return true;}
	virtual bool TerminateSession(bool force) {return EndSession();}
	virtual bool Mutable(act_session * next_cmd) { return true;} 
	virtual bool IsTimeSpent() { return true;}
	virtual bool OnAttacked();

	virtual int  GetMask() { return 0xFFFFFFFF;}
	virtual int  GetExclusiveMask()	{ return 0xFFFFFFFF;}
};

class session_empty : public act_session
{
public:
	DECLARE_SUBSTANCE(session_empty);
	explicit session_empty():act_session(NULL){}
	virtual bool StartSession(act_session * next_cmd = NULL) {return false;}
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	//取得自己的mask，表明自己在队伍中的身分
	virtual int  GetMask() { return 0; }
	//设置排他的mask，表明会排除队列里的哪些session
	virtual int  GetExclusiveMask()	{ return 0; }

	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}
};

class act_timer_session : public act_session, public abase::timer_task
{
protected:
	XID _self_id;
	act_timer_session(){}
public:
	explicit act_timer_session(gactive_imp * imp):act_session(imp){}
	virtual ~act_timer_session()
	{
		if(_timer_index >=0) RemoveTimer();
	}
	virtual bool EndSession()
	{
		RemoveTimer();
		return true;
	}
	virtual bool RepeatSession() = 0;	//重复完成session
	virtual bool TerminateSession(bool force)
	{
		//默认是不能非强制中断
		if(!force) return false;
		EndSession();
		return true;
	}
	virtual bool IsTimeSpent() { return true; }

	bool Save(archive & ar);
	bool Load(archive & ar);

	void OnTimer(int index,int rtimes, bool & is_delete)
	{
		if(rtimes)
			SendRepeatMsg(_self_id);
		else
			SendEndMsg(_self_id);
	}
};

class spirit_act_timer_session : public act_session, public abase::timer_task
{
protected:
	XID _self_id;
	spirit_act_timer_session(){}
public:
	explicit spirit_act_timer_session(gactive_imp * imp):act_session(imp){}
	virtual ~spirit_act_timer_session()
	{
		if(_timer_index >=0) RemoveTimer();
	}
	virtual bool EndSession()
	{
		RemoveTimer();
		return true;
	}
	virtual bool RepeatSession() = 0;
	virtual bool TerminateSession(bool force)
	{
		if(!force) return false;
		EndSession();
		return true;
	}
	virtual bool IsTimeSpent() { return true; }
	virtual void Restore(gactive_imp * imp,int session_id = -1);

	bool Save(archive & ar);
	bool Load(archive & ar);

	void OnTimer(int index,int rtimes, bool & is_delete)
	{
		if(rtimes)
		{
			SendSpiritRepeatMsg(_self_id);
		}
		else
		{
			SendSpiritEndMsg(_self_id);
		}
	}
	void SendSpiritRepeatMsg(const XID& self);
	void SendSpiritEndMsg(const XID& self);
};

class session_move:  public act_timer_session
{
protected:
	session_move(){}
protected:
	A3DVECTOR _target;
	A3DVECTOR _predict;
	int	_use_time;
	int	_seq;
	unsigned short _speed;
	unsigned char _move_mode;
	bool CheckCmdSeq();
public:
	DECLARE_SUBSTANCE(session_move);
	explicit session_move(gactive_imp * imp):act_timer_session(imp),_seq(-1),_speed(0),_move_mode(1){}
	~session_move()
	{
		if(_timer_index >=0) RemoveTimer();
	}
	void SetPredictPos(const A3DVECTOR &pos)
	{
		_predict = pos;
	}
	void SetDestination(unsigned short speed,unsigned char move_mode, 
				const A3DVECTOR & target,int use_time)
	{	
		_speed = speed;
		_move_mode = move_mode;
		_target = target;
		_use_time = use_time;
	}

	void SetCmdSeq(int seq)
	{
		_seq = seq;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _target << _predict <<_use_time 
		   << _speed << _move_mode;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _target >> _predict >>_use_time 
		   >> _speed >> _move_mode; 
		return true;
	}
};

class session_stop_move : public session_move
{
	unsigned short _dir;
protected:
	session_stop_move(){}
public:
	DECLARE_SUBSTANCE(session_stop_move);
	explicit session_stop_move(gactive_imp * imp):session_move(imp),_dir(0){}
	void SetDir(unsigned short dir)
	{
		_dir = dir;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	bool Save(archive & ar) 
	{
		session_move::Save(ar);
		ar << _dir;
		return true;
	}

	bool Load(archive & ar) 
	{
		session_move::Load(ar);
		ar >> _dir;
		return true;
	}

};

class session_normal_attack: public act_timer_session
{
#ifdef __TEST_ATTACK_DELAY__
	timeval tv1; 
#endif

protected:
	session_normal_attack(){}
protected:
	int _attack_restore;
	int _attack_point;

	XID _target;
	char _stop_flag;
	char _force_attack;
	unsigned char _mode;	//mode 0: d1, mode 0 d2
public:
	DECLARE_SUBSTANCE(session_normal_attack);
	explicit session_normal_attack(gactive_imp * imp);
	//			:act_timer_session(imp),_target(-1,-1),_stop_flag(0)
	//{}
	~session_normal_attack()
	{
		if(_timer_index >=0) RemoveTimer();
	}

	void SetTarget(const XID & target,char force_attack)
	{
		_target = target;
		_force_attack = force_attack;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	
	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_ATTACK;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE);
	}
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _target << _stop_flag <<_force_attack << _attack_restore << _attack_point;

		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _target >> _stop_flag >> _force_attack >> _attack_restore >> _attack_point;
		return true;
	}
	virtual void OnTimer(int index,int rtimes, bool & is_delete);
	virtual bool TerminateSession(bool force);
};


class session_npc_zombie : public act_timer_session
{
	int _delay_time;
protected:
	session_npc_zombie(){}
public:
	DECLARE_SUBSTANCE(session_npc_zombie);
	explicit session_npc_zombie(gactive_imp * imp)
				:act_timer_session(imp),_delay_time(500)
	{}

	~session_npc_zombie()
	{
		if(_timer_index >=0) RemoveTimer();
	}

	void SetDelay(int delay)
	{
		_delay_time = delay;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual void OnTimer(int index,int rtimes, bool & is_delete);

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return 0;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~0;
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _delay_time;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _delay_time;
		return true;
	}
};

class session_skill : public act_timer_session
{
protected:
	session_skill():_data(0){}
	SKILL::Data _data;
	int  _next_interval;
	bool _end_flag;
	char _force_attack;
	int  _skill_skip_time;
	int  _curr_cast_speed_rate;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
public:
	DECLARE_SUBSTANCE(session_skill);
	explicit session_skill(gactive_imp * imp)
				:act_timer_session(imp),_data(0),
				_next_interval(TICK_PER_SECOND),_end_flag(false),_force_attack(0), _skill_skip_time(0), _curr_cast_speed_rate(0)
	{
	}
	void SetTarget(int skill_id, char force_attack,int target_num,int * targets, int level = 0, int item_id = -1, short item_index = -1, bool consumable = false, const A3DVECTOR& pos = A3DVECTOR());
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RestartSession();
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes, bool & is_delete);

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_SKILL;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE);
	}

	bool OnAttacked();		// 返回真表示要中断
	bool OnSilentSeal();
	bool OnBreakCast();
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _next_interval
		   << _end_flag << _force_attack << _skill_skip_time;
		size_t size = _target_list.size();
		ar << size;
		ar.push_back(_target_list.begin(), sizeof(XID)*_target_list.size());
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _next_interval
		   >> _end_flag >> _force_attack >> _skill_skip_time;
		size_t size;
		ar >> size;
		XID id;
		for(size_t i = 0; i < size; i ++)
		{
			ar.pop_back(&id,sizeof(id));
			_target_list.push_back(id);
		}
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
	bool IsSkillPerforming(int id);
};

class spirit_session_skill : public spirit_act_timer_session
{
protected:
	spirit_session_skill():_data(0){}
	SKILL::Data _data;
	int  _next_interval;
	bool _end_flag;
	char _force_attack;
	int  _skill_skip_time;
	int  _curr_cast_speed_rate;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
public:
	DECLARE_SUBSTANCE(spirit_session_skill);
	explicit spirit_session_skill(gactive_imp * imp)
				:spirit_act_timer_session(imp),_data(0),
				_next_interval(TICK_PER_SECOND),_end_flag(false),_force_attack(0), _skill_skip_time(0), _curr_cast_speed_rate(0)
	{
	}
	void SetTarget(int skill_id, char force_attack,int target_num,int * targets, int level, int item_id, short item_index, bool consumable, const A3DVECTOR& pos, char spirit_index);
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RestartSession();
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes, bool & is_delete);

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_SPIRIT_SKILL;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return 0;
	}

	bool OnAttacked();		// 返回真表示要中断
	bool OnSilentSeal();
	bool OnBreakCast();
	bool Save(archive & ar) 
	{
		spirit_act_timer_session::Save(ar);
		ar << _next_interval
		   << _end_flag << _force_attack << _skill_skip_time;
		size_t size = _target_list.size();
		ar << size;
		ar.push_back(_target_list.begin(), sizeof(XID)*_target_list.size());
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		spirit_act_timer_session::Load(ar);
		ar >> _next_interval
		   >> _end_flag >> _force_attack >> _skill_skip_time;
		size_t size;
		ar >> size;
		XID id;
		for(size_t i = 0; i < size; i ++)
		{
			ar.pop_back(&id,sizeof(id));
			_target_list.push_back(id);
		}
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
};

class session_produce : public act_timer_session
{
protected:
	session_produce(){}
	const recipe_template * _rt;
	size_t _count;

public:
	DECLARE_SUBSTANCE(session_produce);
	explicit session_produce(gplayer_imp * imp,const recipe_template * rt, size_t count)
				:act_timer_session((gactive_imp*)imp),_rt(rt),_count(count)
	{
	}
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_MOVE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};


class session_use_item : public  act_timer_session
{
protected:
	session_use_item(){}
	int _where;
	int _index;
	int _type;
	size_t _count;
	size_t _usetime;

public:
	DECLARE_SUBSTANCE(session_use_item); 
	session_use_item(gplayer_imp * imp,int where,int index,int type, size_t count,size_t usetime)
		:act_timer_session((gactive_imp*)imp), _where(where),_index(index),
		_type(type),_count(count),_usetime(usetime)
	{
	}
	virtual void OnTimer(int index,int rtimes, bool & is_delete);
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_ITEM;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_use_item_with_target : public  session_use_item
{
protected:
	session_use_item_with_target(){}
	XID _target;
	char _force_attack;

public:
	DECLARE_SUBSTANCE(session_use_item_with_target); 
	session_use_item_with_target(gplayer_imp * imp,int where,int index,int type, size_t count,size_t usetime)
		:session_use_item(imp,where,index,type,count,usetime),_target(-1,-1),_force_attack(0)
	{
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	void SetTarget(const XID & target, char force_attack)
	{
		_target = target;
		_force_attack = force_attack;
	}

	virtual bool RepeatSession();
	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_cancel_action: public act_session
{
public:
	DECLARE_SUBSTANCE(session_cancel_action);
	explicit session_cancel_action():act_session(NULL){}
	virtual bool StartSession(act_session * next_cmd = NULL) {return false;}
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	//取得自己的mask，表明自己在队伍中的身分
	virtual int  GetMask() { return 0; }
	//设置排他的mask，表明会排除队列里的哪些session
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE); }
};

class session_sit_down: public  act_session
{
protected:
	session_sit_down(){}
public:
	DECLARE_SUBSTANCE(session_sit_down); 
	session_sit_down(gplayer_imp * imp)
		:act_session((gactive_imp*)imp)
	{
	}
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession(); 
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}

	virtual int  GetMask() { return SS_MASK_SITDOWN;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}
};

class session_gather_prepare : public act_session
{
public:
	session_gather_prepare(){}
	int   _target;
	short _where;
	short _index;
	int _tool_type;
	int _task_id;
	int _role_id;
	int _team_id;
	int _master_id;
public:
	DECLARE_SUBSTANCE(session_gather_prepare);
	explicit session_gather_prepare(gplayer_imp * imp)
		:act_session((gactive_imp*)imp)
	{}

	void SetTarget(int target, short where, short index, int tool_type,int task_id, int role_id, int team_id, int master_id)
	{
		_target = target;
		_where = where;
		_index = index;
		_tool_type = tool_type;
		_task_id = task_id;
		_role_id = role_id;
		_team_id = team_id;
		_master_id = master_id;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession() { return false;}
	virtual bool EndSession() {return true;}
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() {return false;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//和SitDown同级的
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}


	bool Save(archive & ar) 
	{
		ar << _target << _where << _index << _tool_type << _task_id;
		return true;
	}

	bool Load(archive & ar) 
	{ 
		ar >> _target >> _where >> _index >> _tool_type >> _task_id;
		return true;
	}
};

class session_gather : public  act_timer_session
{
public:
	session_gather(){}
	int _mine;
	unsigned short _gather_time;
	bool _gather_flag;
	bool _can_be_interruputed;
	bool _lock_inventory;
public:
	DECLARE_SUBSTANCE(session_gather);
	explicit session_gather(gplayer_imp * imp)
		:act_timer_session((gactive_imp*)imp),_mine(0),_can_be_interruputed(true),_lock_inventory(false)
	{}

	void SetTarget(int mine,unsigned short gather_time, bool cbi)
	{
		_mine = mine;
		_gather_time = gather_time;
		_can_be_interruputed = cbi;
	}

	void LockInventory()
	{
		_lock_inventory = true;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//和SitDown同级的
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}
	bool OnAttacked();		// 返回真表示要中断

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _mine << _gather_time <<  _gather_flag << _can_be_interruputed << _lock_inventory;
		return true;
	}

	bool Load(archive & ar) 
	{ 
		act_timer_session::Load(ar);
		ar >> _mine >> _gather_time >>  _gather_flag >> _can_be_interruputed >> _lock_inventory;
		return true;
	}
};

class session_use_trashbox : public  act_timer_session
{
	int _trash_type;
public:
	session_use_trashbox() {}
public:
	DECLARE_SUBSTANCE(session_use_trashbox);
	explicit session_use_trashbox(gplayer_imp * imp,int trash_type)
		:act_timer_session((gactive_imp*)imp),_trash_type(trash_type)
	{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//和SitDown同级的
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

};


class session_emote_action : public  act_timer_session
{
	unsigned char _action;
public:
	session_emote_action() {}
public:
	DECLARE_SUBSTANCE(session_emote_action);
	explicit session_emote_action(gplayer_imp * imp)
		:act_timer_session((gactive_imp*)imp),_action(0)
	{}

	void SetAction(unsigned char action) 
	{
		_action = action;
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _action;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _action;
		return true;
	}
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//和SitDown同级的
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}
};


class session_resurrect : public act_timer_session
{
protected:
	float _exp_reduce;
	int _time;
	session_resurrect():_exp_reduce(0){}
public:
	DECLARE_SUBSTANCE(session_resurrect);
	explicit session_resurrect(gplayer_imp * imp,int t= 17)
		:act_timer_session((gactive_imp*)imp),_exp_reduce(0),_time(t){}


	void SetExpReduce(float reduce)
	{
		_exp_reduce = reduce;
	}

	bool Save(archive & ar)
	{
		act_timer_session::Save(ar);
		ar << _exp_reduce;
		return true;
	}

	bool Load(archive & ar)
	{
		act_timer_session::Load(ar);
		ar >> _exp_reduce;
		return true;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession(); 
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return true; }
	//取得自己的mask，表明自己在队伍中的身分
	virtual int  GetMask() { return 0; }
	//设置排他的mask，表明会排除队列里的哪些session
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE); }

	virtual void OnTimer(int index,int rtimes, bool & is_delete);
};

class session_resurrect_by_item : public session_resurrect
{
	session_resurrect_by_item(){}
public:
	DECLARE_SUBSTANCE(session_resurrect_by_item);
	explicit session_resurrect_by_item(gplayer_imp * imp,int time=17):session_resurrect(imp,time){}
	virtual bool EndSession();
};

class session_resurrect_in_town: public session_resurrect
{
	session_resurrect_in_town(){}
public:
	DECLARE_SUBSTANCE(session_resurrect_in_town);
	explicit session_resurrect_in_town(gplayer_imp * imp,int time=17):session_resurrect(imp,time){}
	virtual bool EndSession();
};

class session_enter_sanctuary : public act_session
{
	session_enter_sanctuary(){}
public:
	DECLARE_SUBSTANCE(session_enter_sanctuary);
	explicit session_enter_sanctuary(gplayer_imp * imp):act_session((gactive_imp*)imp){}
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return 0;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}
};

class session_say_hello : public act_session
{
	session_say_hello(){}
	XID _target;
public:
	DECLARE_SUBSTANCE(session_say_hello);
	explicit session_say_hello(gplayer_imp * imp):act_session((gactive_imp*)imp),_target(-1,-1){}
	void SetTarget(const XID & target)
	{
		_target = target;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return SS_MASK_USE_ITEM;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { 
		ar << _target;
		return true;
	}
	bool Load(archive & ar) 
	{ 
		ar >> _target;
		return true;
	}
};

class session_instant_skill : public act_session
{
protected:
	session_instant_skill():_data(0){}
	SKILL::Data _data;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
	A3DVECTOR _chargePos;
	XID _chargeTarget;

public:
	DECLARE_SUBSTANCE(session_instant_skill);
	explicit session_instant_skill(gactive_imp * imp)
				:act_session(imp),_data(0),_chargePos(0.0f, 0.0f, 0.0f),_chargeTarget(-1,-1)
	{}

	void SetTarget(int skill_id, char force_attack,int target_num,int * targets, int level, int item_id, short item_index, bool consumable, const A3DVECTOR& pos, char spirit_index);
	void SetCharge(A3DVECTOR & pos, XID & target);
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession() { return false;}
	virtual bool EndSession() { return true;}
	virtual bool TerminateSession(bool force) { return true;}
	virtual bool IsTimeSpent() { return false;}

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE);
	}

	bool Save(archive & ar) 
	{
		act_session::Save(ar);
		size_t size = _target_list.size();
		ar << size;
		ar.push_back(_target_list.begin(), sizeof(XID)*_target_list.size());
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_session::Load(ar);
		size_t size;
		ar >> size;
		XID id;
		for(size_t i = 0; i < size; i ++)
		{
			ar.pop_back(&id,sizeof(id));
			_target_list.push_back(id);
		}
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
};

class session_region_transport : public act_session
{
	session_region_transport(){}
	int _ridx;
	int _tag;
public:
	DECLARE_SUBSTANCE(session_region_transport);
	explicit session_region_transport(gplayer_imp * imp,int ridx, int tag):act_session((gactive_imp*)imp),_ridx(ridx),_tag(tag)
	{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession() {return true;}
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return SS_MASK_MOVE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { 
		ar << _ridx << _tag;
		return true;
	}
	bool Load(archive & ar) 
	{ 
		ar >> _ridx >> _tag;
		return true;
	}
};

class session_resurrect_protect : public act_timer_session
{
protected:
	session_resurrect_protect(){}
public:
	DECLARE_SUBSTANCE(session_resurrect_protect);
	explicit session_resurrect_protect(gplayer_imp * imp):act_timer_session((gactive_imp*)imp){}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession(); 
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return false; }
	virtual bool IsTimeSpent() { return true; }
	//取得自己的mask，表明自己在队伍中的身分
	virtual int  GetMask() { return 0; }
	//设置排他的mask，表明会排除队列里的哪些session
	virtual int  GetExclusiveMask()	{ return 0xFFFFFFFF; }

};

class session_pos_skill : public act_timer_session
{
protected:
	session_pos_skill():_data(0){}
	SKILL::Data _data;
	int  _next_interval;
	bool _end_flag;
	A3DVECTOR _target_pos;
public:
	DECLARE_SUBSTANCE(session_pos_skill);
	explicit session_pos_skill(gactive_imp * imp)
				:act_timer_session(imp),_data(0), _next_interval(TICK_PER_SECOND),_end_flag(false) 
	{
	}
	void SetTarget(int skill_id, const A3DVECTOR & pos)
	{
		_data.id = skill_id;
		_target_pos = pos;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes, bool & is_delete);

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _next_interval
		   << _end_flag << _target_pos;
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _next_interval
		   >> _end_flag >> _target_pos;
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
};

class session_general : public act_timer_session
{
protected:
	session_general(){}
	int _delay;
public:
	explicit session_general(gactive_imp * imp)
				:act_timer_session(imp),_delay(60)
	{
	}

	void SetDelay(int delay)
	{
		_delay = delay;
	}
	
	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes, bool & is_delete);

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_SITDOWN;
	}
	
	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_MOVE);
	}


	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _delay;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _delay;
		return true;
	}
	virtual void OnStart() =0;
	virtual void OnRepeat() = 0;
	virtual void OnEnd() = 0;
};

class session_pet_operation : public session_general
{
protected:
	session_pet_operation(){}
	int _pet_index;
	int _pet_id;
	int _op;
public:
	explicit session_pet_operation(gactive_imp * imp,int op)
				:session_general(imp),_op(op)
	{}

	void SetTarget(size_t pet_index,int pet_id = -1)
	{
		_pet_index = pet_index;
		_pet_id = pet_id;
	}

	bool Save(archive & ar) 
	{
		session_general::Save(ar);
		ar << _pet_index << _pet_id << _op;
		return true;
	}

	bool Load(archive & ar) 
	{
		session_general::Load(ar);
		ar >> _pet_index >> _pet_id >> _op;
		return true;
	}
	virtual void OnStart();
	virtual void OnRepeat() {}
	virtual void OnEnd();
};

class session_summon_pet : public session_pet_operation
{
protected:
	session_summon_pet(){}
public:
	DECLARE_SUBSTANCE(session_summon_pet);
	explicit session_summon_pet(gactive_imp * imp)
				:session_pet_operation(imp,0)
	{}
	virtual void OnRepeat();
};

class session_recall_pet : public session_pet_operation
{
protected:
	session_recall_pet(){}
public:
	DECLARE_SUBSTANCE(session_recall_pet);
	explicit session_recall_pet(gactive_imp * imp)
				:session_pet_operation(imp,1)
	{}
	virtual void OnRepeat();
};

class session_combine_pet : public session_pet_operation
{
	int _type;
protected:
	session_combine_pet(){} 
public: 
	DECLARE_SUBSTANCE(session_combine_pet);
	explicit session_combine_pet(gactive_imp * imp,int type)
		:session_pet_operation(imp,-1),_type(type)
		{
			if(0 == type)
			{
				_op = 2;
			}
			else
			{
				_op = 4;
			}
		}      
	virtual void OnRepeat();
}; 

class session_uncombine_pet : public session_pet_operation
{
	int _type;
protected:
	session_uncombine_pet(){}
public:
	DECLARE_SUBSTANCE(session_uncombine_pet);
	explicit session_uncombine_pet(gactive_imp * imp,int type)
		:session_pet_operation(imp,-1),_type(type)
		{
			if(0 == type)
			{
				_op = 3;
			}
			else
			{
				_op = 5;
			}
		}
	virtual void OnRepeat();
};

class session_free_pet : public session_pet_operation
{
protected:
	session_free_pet(){}
public:
	DECLARE_SUBSTANCE(session_free_pet);
	explicit session_free_pet(gactive_imp * imp)
				:session_pet_operation(imp,6)
	{}
	virtual void OnRepeat();
};

class session_adopt_pet : public session_pet_operation
{
protected:
	session_adopt_pet(){}
public:
	DECLARE_SUBSTANCE(session_adopt_pet);
	explicit session_adopt_pet(gactive_imp * imp)
				:session_pet_operation(imp,7)
	{}
	virtual void OnRepeat();
};

class session_refine_pet : public session_pet_operation
{
	size_t _index_c;
	size_t _index_a;
protected:
	session_refine_pet(){}
public:
	DECLARE_SUBSTANCE(session_refine_pet);
	explicit session_refine_pet(gactive_imp * imp)
				:session_pet_operation(imp,8)
	{}
	void SetTarget(size_t pet_index,size_t index_c,size_t index_a)
	{
		_pet_index = pet_index;
		_index_c = index_c;
		_index_a = index_a;
	}
	virtual void OnRepeat();
};

class session_refine_pet2 : public session_pet_operation
{
	int _type;
protected:
	session_refine_pet2(){}
public:
	DECLARE_SUBSTANCE(session_refine_pet2);
	explicit session_refine_pet2(gactive_imp * imp)
				:session_pet_operation(imp,8)
	{}
	void SetTarget(size_t pet_index,int type)
	{
		_pet_index = pet_index;
		_type = type;
	}
	virtual void OnRepeat();
};

class session_general_operation : public session_general
{
public:
	class operation : public abase::ASmallObject
	{
	friend class session_general_operation;
	protected:
		int _session_id;
	public:
		virtual ~operation() {}
		virtual int GetID() = 0;
		virtual bool NeedBoardcast() = 0;
		virtual void OnStart(gactive_imp * pImp) = 0;
		virtual void OnEnd(gactive_imp * pImp) = 0;
		virtual bool OnAttacked(gactive_imp * pImp) { return false;}
		virtual void PostEnd(gactive_imp * pImp) {}
		virtual int GetDuration() = 0; 
	};
protected:
	operation * _op;

public:
	session_general_operation(gactive_imp * imp, operation * op)
				:session_general(imp),_op(op)
	{
		SetDelay(_op->GetDuration());
	}

	~session_general_operation()
	{
		delete _op;
	}

	bool Save(archive & ar) 
	{
		ASSERT(false);
		return true;
	}

	bool Load(archive & ar) 
	{
		ASSERT(false);
		return true;
	}

	virtual void OnStart();
	virtual void OnRepeat();
	virtual void OnEnd();
	virtual bool OnAttacked();
};

class session_online_agent : public act_session
{
public:
	session_online_agent(gplayer_imp *pImp)
		:act_session((gactive_imp*)pImp)
	{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession();
	virtual bool RepeatSession() {return true;}
	virtual bool TerminateSession(bool force) {return EndSession();}
	virtual bool Mutable(act_session * next_cmd) { return true;} 
	virtual bool IsTimeSpent() { return true;}
	virtual bool OnAttacked();

	virtual int  GetMask() { return 0xFFFFFFFF;}
	virtual int  GetExclusiveMask()	{ return 0xFFFFFFFF;}
};

class session_logon_invincible : public act_timer_session 
{
protected:
	size_t _time;
public:
	session_logon_invincible(gplayer_imp *pImp, size_t time)
		:act_timer_session((gactive_imp*)pImp), _time(time)
	{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession();
	virtual bool RepeatSession() {return true;}
	virtual bool TerminateSession(bool force) {return EndSession();}
	virtual bool Mutable(act_session * next_cmd) { return true;} 
	virtual bool IsTimeSpent() { return true;}

	virtual int  GetMask() { return 0xFFFFFFFF;}
	virtual int  GetExclusiveMask()	{ return 0xFFFFFFFF;}
};

namespace path_finding
{
	class follow_target;
	class cruise;
	class keep_out;
};

class session_follow_target :  public act_timer_session
{
protected:
	float _range_min;
	float _range_max;
	float _range_target;
	XID _target;
	int _retcode;
	bool stop;
	unsigned short _speed;
	float _player_speed;
	bool _stop_flag;
	char _reachable_count;
	int _timeout;
	int Run();
	path_finding::follow_target * _agent;
	session_follow_target():_agent(0){}
public:
	DECLARE_SUBSTANCE(session_follow_target);
	explicit session_follow_target(gactive_imp *imp)
				:act_timer_session(imp),_target(-1,-1),_retcode(0),_stop_flag(true),_reachable_count(0),_timeout(20),_agent(0)
	{
	}
	~session_follow_target();

	float GetSpeed()
	{
		return _player_speed;
	}

	virtual int  GetMask()		//取得自己的mask，表明自己在队伍中的身分
	{
		return SS_MASK_FOLLOW_TARGET;
	}

	virtual int  GetExclusiveMask()	//设置排他的mask，表明会排除队列里的哪些session
	{
		return ~(SS_MASK_FOLLOW_TARGET|SS_MASK_MOVE);
	}

	void SetTarget(const XID & target, float range_min,float range_max,float range_target,float speed, int timeout=(int)(10/NPC_FOLLOW_TARGET_TIME))
	{
		_target = target;
		_range_max = range_max*range_max;
		_range_min = range_min*range_min;
		_range_target = range_target;
		_speed = (unsigned short)(speed * 256.0f + 0.5f);
		_player_speed = speed;
		_timeout = timeout;
	}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force) {return EndSession();}
	virtual bool IsTimeSpent() { return true; }

	void TrySendStop();
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _target << _timeout
		   << _range_min << _range_max << _range_target << _retcode
		   << _speed << _stop_flag << _reachable_count;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _target >> _timeout
		   >> _range_min >> _range_max >> _range_target >> _retcode
		   >> _speed >> _stop_flag >> _reachable_count;
		return true;
	}
};

class session_active_emote : public act_session
{
public:
	DECLARE_SUBSTANCE(session_active_emote);
	session_active_emote():act_session(NULL) {}
	explicit session_active_emote(gplayer_imp *pImp)
		:act_session((gactive_imp*)pImp)
	{}

	virtual bool StartSession(act_session * next_cmd = NULL);
	virtual bool EndSession();
	virtual bool RepeatSession() {return true;}
	virtual bool TerminateSession(bool force) {return EndSession();}
	virtual bool Mutable(act_session * next_cmd) { return true;} 
	virtual bool IsTimeSpent() { return true;}
	virtual bool OnAttacked();

	virtual int  GetMask() { return 0xFFFFFFFF;}
	virtual int  GetExclusiveMask()	{ return 0xFFFFFFFF;}
};

#endif

