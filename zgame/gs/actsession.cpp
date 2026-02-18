#include "actobject.h"
#include "actsession.h"
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

DEFINE_SUBSTANCE_ABSTRACT(act_session,substance,CLS_SESSION_BASE)
DEFINE_SUBSTANCE(session_empty,act_session,CLS_SESSION_EMPTY)
DEFINE_SUBSTANCE(session_move,act_session,CLS_SESSION_MOVE)
DEFINE_SUBSTANCE(session_stop_move,act_session,CLS_SESSION_STOP_MOVE)
DEFINE_SUBSTANCE(session_normal_attack,act_session,CLS_SESSION_NORMAL_ATTACK)
DEFINE_SUBSTANCE(session_npc_zombie,act_session,CLS_SESSION_NPC_ZOMBIE)
DEFINE_SUBSTANCE(session_skill,act_session,CLS_SESSION_SKILL)
DEFINE_SUBSTANCE(session_produce,act_session,CLS_SESSION_PRODUCE)
DEFINE_SUBSTANCE(session_cancel_action,act_session,CLS_SESSION_CANCEL_ACTION)
DEFINE_SUBSTANCE(session_use_item,act_session,CLS_SESSION_USE_ITEM)
DEFINE_SUBSTANCE(session_use_item_with_target,session_use_item,CLS_SESSION_USE_ITEM_WITH_TARGET)
DEFINE_SUBSTANCE(session_sit_down,act_session,CLS_SESSION_PLAYER_SIT_DOWN)
DEFINE_SUBSTANCE(session_gather,act_session,CLS_SESSION_GATHER)
DEFINE_SUBSTANCE(session_use_trashbox,act_session,CLS_SESSION_USE_TRASHBOX)
DEFINE_SUBSTANCE(session_emote_action,act_session, CLS_SESSION_EMOTE)
DEFINE_SUBSTANCE(session_gather_prepare,act_session, CLS_SESSION_GATHER_PREPARE)
DEFINE_SUBSTANCE(session_resurrect, act_session ,CLS_SESSION_RESURRECT)
DEFINE_SUBSTANCE(session_resurrect_by_item, session_resurrect ,CLS_SESSION_RESURRECT_BY_ITEM)
DEFINE_SUBSTANCE(session_resurrect_in_town, session_resurrect ,CLS_SESSION_RESURRECT_IN_TOWN)
DEFINE_SUBSTANCE(session_enter_sanctuary, act_session,CLS_SESSION_ENTER_SANCTUARY)
DEFINE_SUBSTANCE(session_say_hello, act_session,CLS_SESSION_SAY_HELLO)
DEFINE_SUBSTANCE(session_instant_skill, act_session,CLS_SESSION_INSTANT_SKILL)
DEFINE_SUBSTANCE(session_region_transport, act_session,CLS_SESSION_REGION_TRANSPORT)
DEFINE_SUBSTANCE(session_resurrect_protect, act_session , CLS_SESSION_RESURRECT_PROTECT)
DEFINE_SUBSTANCE(session_pos_skill, act_session , CLS_SESSION_POS_SKILL)
DEFINE_SUBSTANCE(session_summon_pet, act_session , CLS_SESSION_SUMMON_PET)
DEFINE_SUBSTANCE(session_recall_pet, act_session , CLS_SESSION_RECALL_PET)
DEFINE_SUBSTANCE(session_combine_pet, act_session , CLS_SESSION_COMBINE_PET)
DEFINE_SUBSTANCE(session_uncombine_pet, act_session , CLS_SESSION_UNCOMBINE_PET)
DEFINE_SUBSTANCE(session_free_pet,act_session,CLS_SESSION_FREE_PET)
DEFINE_SUBSTANCE(session_adopt_pet,act_session,CLS_SESSION_ADOPT_PET)
DEFINE_SUBSTANCE(session_refine_pet,act_session,CLS_SESSION_REFINE_PET)
DEFINE_SUBSTANCE(session_refine_pet2,act_session,CLS_SESSION_REFINE_PET2)
DEFINE_SUBSTANCE(session_follow_target,act_session,CLS_SESSION_FOLLOW_TARGET)
DEFINE_SUBSTANCE(spirit_session_skill,act_session,CLS_SESSION_SPIRIT_SKILL)
DEFINE_SUBSTANCE(session_active_emote,act_session,CLS_SESSION_ACTIVE_EMOTE)

act_session::act_session(gactive_imp * imp):_imp(imp),_session_id(-1),_start_tick(0),_plane(0)
{
	if(_imp) _plane = _imp->_plane;
} 

void 
act_session::Restore(gactive_imp * imp,int session_id)
{
	_imp = imp;
	_plane = imp->_plane;
	_session_id = session_id;
}

bool 
act_timer_session::Save(archive & ar) 
{
	ar << _self_id;
	if(_timer_index != -1)
	{
		int interval;
		int rtimes;
		int next_interval = GetTaskData(interval,rtimes);
		ar << 0 << interval << next_interval << rtimes;
	}
	else
	{
		ar << -1;
	}
	return true;
}

bool 
act_timer_session::Load(archive & ar) 
{
	ar >> _self_id;
	int rst;
	ar >> rst;
	if(!rst)
	{
		//timer存在
		int interval;
		int rtimes;
		int next_interval;
		ar >> interval >> next_interval >> rtimes;
		if(next_interval < 0) { next_interval = 0;}
		SetTimer(g_timer,interval,rtimes,next_interval);
		return true;
	}
	return false;
}

void 
act_session::SendMsg(int message, const XID & target,const XID & source)
{
	MSG msg;
	BuildMessage(msg,message,target,source,A3DVECTOR(0.f,0.f,0.f),_session_id);
	gmatrix::SendMessage(msg);
}

void 
act_session::SendRepeatMsg(const XID & self)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_OBJ_SESSION_REPEAT,self,self,A3DVECTOR(0.f,0.f,0.f),_session_id);
	gmatrix::SendMessage(msg);
}

void 
act_session::SendForceRepeat(const XID & self)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_OBJ_SESSION_REP_FORCE,self,self,A3DVECTOR(0.f,0.f,0.f),_session_id);
	gmatrix::SendMessage(msg);
}
	
void 
act_session::SendEndMsg(const XID & self)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_OBJ_SESSION_END,self,self,A3DVECTOR(0.f,0.f,0.f),_session_id);
	gmatrix::SendMessage(msg);
}

//-------------------------------------------
// spirit_act_timer_session
//-------------------------------------------
bool spirit_act_timer_session::Save(archive & ar) 
{
	ar << _self_id;
	if(_timer_index != -1)
	{
		int interval;
		int rtimes;
		int next_interval = GetTaskData(interval,rtimes);
		ar << 0 << interval << next_interval << rtimes;
	}
	else
	{
		ar << -1;
	}
	return true;
}

bool spirit_act_timer_session::Load(archive & ar) 
{
	ar >> _self_id;
	int rst;
	ar >> rst;
	if(!rst)
	{
		//timer存在
		int interval;
		int rtimes;
		int next_interval;
		ar >> interval >> next_interval >> rtimes;
		if(next_interval < 0) { next_interval = 0;}
		SetTimer(g_timer,interval,rtimes,next_interval);
		return true;
	}
	return false;
}
void spirit_act_timer_session::SendSpiritRepeatMsg(const XID& self)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_SPIRIT_SESSION_REPEAT,self,self,A3DVECTOR(0.f,0.f,0.f),_session_id);
	gmatrix::SendMessage(msg);
}

void spirit_act_timer_session::Restore(gactive_imp * imp,int session_id)
{
	_imp = imp;
	_plane = imp->_plane;
}

void spirit_act_timer_session::SendSpiritEndMsg(const XID& self)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_SPIRIT_SESSION_END,self,self,A3DVECTOR(0.f,0.f,0.f),_session_id);
	gmatrix::SendMessage(msg);
}

void 
act_session::NPCSessionStart(int task_id)
{
	_imp->_commander->NPCSessionStart(task_id,_session_id);
}

void 
act_session::NPCSessionEnd(int task_id, int retcode)
{
	_imp->_commander->NPCSessionEnd(task_id,_session_id,retcode);
}

bool
act_session::NPCGetNextWaypoint(A3DVECTOR & target)
{
	return _imp->_commander->NPCGetNextWaypoint(target);
}

void 
act_session::NPCSessionUpdateChaseInfo(int task_id, const chase_info & info)
{
	_imp->_commander->NPCSessionUpdateChaseInfo(task_id,&info, sizeof(info));
}

bool CheckPlayerMove(gactive_imp * obj, const A3DVECTOR & target,const A3DVECTOR & offset, int mode, int use_time,int seq)
{
	gplayer_imp * pImp =  (gplayer_imp *)obj;
	ASSERT(pImp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gplayer_imp)));
	if(!pImp->NeedCheckMove()) return true;

	const A3DVECTOR & pos = pImp->_parent->pos;
	int rst = -1;
	float terrain_height;
	//检查一下高度信息
	if((terrain_height = obj->GetWorldManager()->GetTerrain().GetHeightAt(target.x,target.z)) <= target.y+0.5f)	 //允许有1.5 米的误差
	{
		rst = pImp->CheckPlayerMove(offset,mode,use_time);
	}
	else
	{
		GLog::log(GLOG_INFO,"用户%d的移动坐标低于地面(%f,%f,%f)" ,pImp->_parent->ID.id, target.x,target.y,target.z);
	}
	
	if(rst >= 0) 
	{
		//CheckPlayerMove 基本通过 ，进行碰撞检测的调用
		if(pImp->PhaseControl(target,terrain_height, mode, use_time))
		{
			if(rst == 0)
			{
				if(pImp->DecMoveCheckerError(1) < 10)
				{
					return true;
				}
			}
			else
			{
				GLog::log(GLOG_INFO,"用户%d的可疑移动数据(%f,%f,%f) pos:(%f,%f,%f) 时间%d,模式%d"
						,pImp->_parent->ID.id, offset.x,offset.y,offset.z,pos.x,pos.y,pos.z
						,use_time,mode);
				if(pImp->IncMoveCheckerError(rst) < 10)
				{
					return true;
				}
			}
		}
		else
		{
			rst = -3;
		}
	}

	//出现错误，记录特殊日志，
	GLog::log(GLOG_INFO,"用户%d被强行同步位置(%f,%f,%f)",pImp->_parent->ID.id,pos.x,pos.y,pos.z);

	//清除现在的移动错误计数
	pImp->ClrMoveCheckerError();
	
	//修正新的命令序号
	seq = (seq + 100) & 0xFFFF;
	pImp->_commander->SetNextMoveSeq(seq);

	//考虑重新发送玩家的速度数据
	pImp->_runner->get_extprop_move();
	//将玩家拉回原地
	pImp->_runner->trace_cur_pos(seq);
	//清空后面的所有session
	pImp->ClearNextSession();
	return false;
}

bool 
session_move::CheckCmdSeq()
{
	int seq = _imp->_commander->GetCurMoveSeq();
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

bool 
session_move::StartSession(act_session * next_cmd)
{
	if(!CheckCmdSeq()) return false;

	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	
	A3DVECTOR predict = _target;
	_target -= _imp->_parent->pos;

	if(_use_time < 100 ) _use_time = 100;
	if(!CheckPlayerMove(_imp,predict,_target,_move_mode,_use_time,_seq))
	{	
		//超速了
		//拉回来
		return false;
	}

	_imp->UpdateMoveMode(_move_mode);

	//播放自己移动的消息，可能需要有一定的简化策略
	//比如对距离远的位置减少播放的频率等等
	
	_imp->_runner->move(predict,_use_time,_speed,_move_mode);
	_imp->StepMove(_target);
	__PRINTF("MMMM MMMMM MMM:%f %f %f ---- %f %f %f\n",_target.x,_target.y,_target.z,
	_imp->_parent->pos.x,_imp->_parent->pos.y,_imp->_parent->pos.z);

	//测试延迟速度
	int tick = MILLISEC_TO_TICK(_use_time) - 1;
	if(next_cmd) tick --;			//如果后面有命令则加快一点
	if(tick <= 8) tick = 8;
	SetTimer(g_timer,tick,1);
	return true;
}

bool 
session_move::RepeatSession()
{
	ASSERT(false && "移动session不能重复");
	return false;
}


bool 
session_stop_move::StartSession(act_session * next_cmd)
{
	if(!CheckCmdSeq()) return false;

	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;


	A3DVECTOR pos = _target;
	_target -= _imp->_parent->pos;
	//每次stopmove 都要记录以下
	if(_use_time < 100 ) _use_time = 100;
	if(!CheckPlayerMove(_imp,pos,_target,_move_mode,_use_time,_seq))
	{	
		//超速了
		//拉回来
		return false;
	}

	_imp->UpdateStopMove(_move_mode);

	//发送停止移动的消息
	_imp->_runner->stop_move(pos,_speed,_dir,_move_mode);
	
	//进行真正的移动
	_imp->StepMove(_target);
	__PRINTF("MMMM SSSSS MMM:%f %f %f ---- %f %f %f\n",_target.x,_target.y,_target.z,
			_imp->_parent->pos.x,_imp->_parent->pos.y,_imp->_parent->pos.z);

	//进行移动的判定，如果前一个stopmove和现在的stop move 时间相差太近，则应该加入延时，
	//或者考虑如果后面跟随了攻击命令，则延时很小，否则进行正常延时
	act_session * pSession = _imp->GetNextSession();
	int t = MILLISEC_TO_TICK(_use_time);
	if(t < 8) t = 8;
	if(!pSession || !(pSession->GetMask() & SS_MASK_ATTACK))
	{
		SetTimer(g_timer,t,1);
		return true;
	}
	else
	{
		if(t > 10)
		{
			//时间太长则还是要延迟 只是将延迟缩短一些
			t = 8;
			SetTimer(g_timer,t,1);
			return true;
		}
	}
	
	//SetTimer(g_timer,1,1);
	return false;
}

#ifdef __TEST_ATTACK_DELAY__
static long co(timeval & t1, timeval & t2)
{
	return (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
}
#endif

session_normal_attack::session_normal_attack(gactive_imp * imp)
:act_timer_session(imp),_target(-1,-1),_stop_flag(0)
{
#ifdef __TEST_ATTACK_DELAY__
	if(imp->_parent->ID.IsPlayer())
	{
		gettimeofday(&tv1,NULL);
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO,"%d attack start -- %ld.%06ld", imp->_parent->ID.id,tv4.tv_sec, tv4.tv_usec);
		}
	}
#endif
}

bool 
session_normal_attack::StartSession(act_session * next_cmd)
{
	ASSERT(_target.id != -1);
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	if(!_imp->CheckAttack(_target))
	{
		return false;
	}
	if(!_imp->StandOnGround() && !_imp->InFlying()) return false;

	if(!_target.IsPlayer() && _target != _imp->_last_attack_target)
	{
		_imp->OnAntiCheatAttack();
		_imp->_last_attack_target = _target ;
	}

#ifdef __TEST_ATTACK_DELAY__
	if(_imp->_parent->ID.IsPlayer())
	{
		timeval tv2;
		gettimeofday(&tv2,NULL);
		{
			timeval tv4;
			gettimeofday(&tv4,NULL);
			GLog::log(GLOG_INFO,"%d attack DELAY %ld-- %ld.%06ld", _imp->_parent->ID.id,co(tv1,tv2), tv4.tv_sec, tv4.tv_usec);
		}
	}
#endif
	_imp->NotifyStartAttack(_target,_force_attack);
	_imp->_session_state = gactive_imp::STATE_SESSION_ATTACK;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	//__PRINTF("user attack\n");
	_imp->_runner->start_attack(_target);
	_imp->GetAttackCycle(_attack_restore,_attack_point);
	_attack_restore -= _attack_point;
	if(_attack_restore <=0) {
		ASSERT(false);
		_attack_restore = 17;
		_attack_point = 17;
	}
	SetTimer(g_timer,_attack_point,0);
	_mode = 0;
	return true;
}

bool
session_normal_attack::EndSession()
{
	if(_session_id != -1)
	{
		_imp->_runner->stop_attack(_stop_flag);
		_session_id = -1;
		RemoveTimer();
	}
	return true;
}

void 
session_normal_attack::OnTimer(int index,int rtimes, bool & is_delete)
{
	if(_mode == 0)
	{
		//第一段为force
		SendForceRepeat(_self_id);
		ChangeIntervalInCallback(_attack_restore);
	}
	else if(_mode == 1)
	{
		//第二段为非force
		SendRepeatMsg(_self_id);
		ChangeIntervalInCallback(_attack_point);
	}
}

bool 
session_normal_attack::TerminateSession(bool force)
{
	if(_mode == 0) 
	{
		EndSession();
		return true;
	}
	return act_timer_session::TerminateSession(force);
}

bool 
session_normal_attack::RepeatSession()
{
	if(_mode == 0)
	{
		//第一段 此时进行攻击行为
		_imp->NormalAttack(_target,_force_attack);
		_mode = 1;
		return true;
	}

	//mode 1
	float dis;
	A3DVECTOR pos;
	int flag;
	if(!_imp->CheckAttack(_target,&flag, &dis,pos))
	{	
		//现在不给客户端回馈错误原因了
		_stop_flag = flag;
		return false;
	}
	_imp->_runner->start_attack(_target);

	//重新更新攻击速度
	int interval1,interval2;
	_imp->GetAttackCycle(interval1,interval2);
	interval1 -= interval2;
	if(interval1 <=0) {
		ASSERT(false);
		interval1 = 17;
		interval2 = 17;
	}
	if(interval1 != _attack_restore || interval2 != _attack_point)
	{
		_attack_restore = interval1;
		_attack_point = interval2;
	}
	
	//回到第一段状态
	_mode = 0;
	return true;
}


bool 
session_npc_zombie::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_ZOMBIE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	SetTimer(g_timer,_delay_time,1);
	return true;
}

bool 
session_npc_zombie::RepeatSession()
{
	ASSERT(false);
	return false;
}

void 
session_npc_zombie::OnTimer(int index,int rtimes, bool & is_delete)
{
	SendMsg(GM_MSG_OBJ_ZOMBIE_END,_self_id,_self_id);
}


void 
session_skill::SetTarget(int skill_id, char force_attack,int target_num,int * targets, int level, int item_id, short item_index, bool consumable, const A3DVECTOR& pos)
{
	_data.id = skill_id;
	_data.level = level;
	_data.item_id = item_id;
	_data.item_index = item_index;
	_data.consumable = consumable;
	_data.forceattack = force_attack;
	_force_attack = force_attack;
	_data.pos = pos;
	_data.spirit_index = -1;

	if(target_num > 0)
	{
		XID id;
		_target_list.reserve(target_num);
		for(int i = 0; i < target_num; i ++,targets ++)
		{
			MAKE_ID(id,*targets);
			_target_list.push_back(id);
		}
	}
}


bool 
session_skill::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_SKILL;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	_curr_cast_speed_rate = _imp->_skill.GetCurrCastSpeedRate();
	__PRINTF("use skill................. %d\n", _data.id);
	if(!_imp->StandOnGround() && !_imp->InFlying()) return false;
	if(!_imp->CheckSendSkillAttack(_data.id)) return false;

	int rangeType = GNET::SkillWrapper::RangeType(_data.id);
	if(!(rangeType == 6 || rangeType == 11 || rangeType == 12 || rangeType == 13 || rangeType == 14 || rangeType == 15) && _target_list.size())
	{
		int skilltype = GNET::SkillWrapper::GetType(_data.id);
		XID target = _target_list[0];
		if( 1 == skilltype || 3 == skilltype )
		{
			if(target != _imp->GetCurTarget()) return false;
			_imp->NotifyStartAttack(_target_list[0],_force_attack);
		}
		if(!target.IsPlayer() && target != _imp->_last_attack_target)
		{
			_imp->OnAntiCheatAttack();
			_imp->_last_attack_target = target ;
		}
	}

	if(GNET::SkillWrapper::IsTalisman(_data.id))
	{
		int level = 0;
		_imp->GetTalismanRefineSkills(level, _data.talismanRefineSkillList);
	}

	int first_interval = _imp->StartSkill(_data,_target_list.begin(),_target_list.size(),_next_interval);
	if(first_interval < 0)
	{
		_imp->_runner->error_message(S2C::ERR_SKILL_NOT_AVAILABLE);
		return false;
	}
	_imp->GetWorldManager()->OnCastSkill(_imp, _data.id);
	if(first_interval < TICK_MILLISEC)
	{
		__PRINTF("玩家瞬发技能\n");
		//瞬发技能
		int next_interval;
		_imp->RunSkill(_data,_target_list.begin(),_target_list.size(),next_interval);
		return false;
	}

	//将时间转换成tick
	__PRINTF("skill session start: first_interval %d, next_interval %d\n",first_interval, _next_interval);
	first_interval = MILLISEC_TO_TICK(first_interval);
	_next_interval = MILLISEC_TO_TICK(_next_interval);
	ASSERT(first_interval > 0);

	if(GNET::SkillWrapper::IsWarmup(_data.id)) 
	{
		_skill_skip_time = g_timer.get_tick();
		_next_interval += 20; //蓄力技能最多可以有20tick的延迟
	}

	
	SetTimer(g_timer,SECOND_TO_TICK(1),0,first_interval);

	//注册一个filter
	_imp->_filters.AddFilter(new skill_interrupt_filter(_imp,_session_id,FILTER_INDEX_SKILL_SESSION));
	return true;
}

bool 
session_skill::RestartSession()
{
	if(!_data.skippable) return true;
	ASSERT(_session_id != -1);
	
	//重新分配session id
	_session_id = _imp->GetNextSessionID();

	//蓄力技能重新开始
	int tick = g_timer.get_tick() - _skill_skip_time;
	if(tick <= 0) tick = 0;
	
	//停止当前定时器
	RemoveTimer();

	int next_interval;
	
	int interval = _imp->ContinueSkill(_data,_target_list.begin(),_target_list.size(),next_interval,TICK_TO_MILLISEC(tick));
	//将时间转换成tick
	interval = MILLISEC_TO_TICK(interval);
	if(interval <= 0) return false;
	_next_interval = MILLISEC_TO_TICK(next_interval);

	if(_data.skippable) 
	{
		_skill_skip_time = g_timer.get_tick();
	}
	
	SetTimer(g_timer,SECOND_TO_TICK(1),0,interval);
	return true;
}

bool
session_skill::EndSession()
{
	if(_session_id != -1)
	{
		if(_imp->IsInCircleOfDoom() && GNET::SkillWrapper::IsCycle(_data.id) && !_imp->_filters.IsFilterExist(FILTER_CYCLE))
		{
			_imp->SetCircleOfDoomStop();
		}
		//并非强制终止技能，试图终止之
		__PRINTF("skill session end \n");
		_session_id = -1;
		RemoveTimer();
		_imp->_filters.RemoveFilter(FILTER_INDEX_SKILL_SESSION);
		_imp->_runner->stop_skill(_data.spirit_index);
		if(_curr_cast_speed_rate != 0)
		{
			_curr_cast_speed_rate = 0;
			_imp->_runner->player_stop_skill(_data.spirit_index);
		}
		int rangeType = GNET::SkillWrapper::RangeType(_data.id);
		if(rangeType == 10)
		{
			_imp->_runner->player_stop_skill(_data.spirit_index);
		}
		_imp->IncAttackStamp();
		timeval tv;
		gettimeofday(&tv,NULL);
		__PRINTF("player %6d stop skill at %ld.%06ld\n",_imp->_parent->ID.id,tv.tv_sec,tv.tv_usec);
	}
	return true;
}

bool 
session_skill::RepeatSession()
{

	if(_end_flag) return false;	//结束
	
	if(_target_list.size())
	{
		int skilltype = GNET::SkillWrapper::GetType(_data.id);
		char rangetype = GNET::SkillWrapper::RangeType(_data.id); 
		if( (1 == skilltype || 3 == skilltype ) && 0 == rangetype )
		{
			if(_target_list[0] != _imp->GetCurTarget()) return false;
		}
	}


	int new_interval = -1;
	int rst = _imp->RunSkill(_data,_target_list.begin(),_target_list.size(),new_interval);
	if(rst <= 0 || _next_interval <= 0) return false;

	__PRINTF("skill repeat , next interval %d\n",new_interval);
	if(_data.skippable) 
	{
		_skill_skip_time = g_timer.get_tick();
	}
	if(new_interval <= 0)
	{
		_next_interval  = new_interval;
		return true;
	}
	_next_interval  = MILLISEC_TO_TICK(new_interval);
	return true;
}

bool 
session_skill::TerminateSession(bool force)
{
	if(force) 
	{
		return EndSession();
	}

	if(_session_id != -1) 
	{
		if(_imp->CancelSkill(_data))
		{
			return EndSession();
		}
		return false;
	}
	return true;
}

void 
session_skill::OnTimer(int index,int rtimes, bool & is_delete)
{
	int interval = _next_interval;
	__PRINTF("%d skill change timer %d %d\n",_self_id.id,interval,_end_flag);
	if(interval <= 0 || _end_flag)
	{
		//结束自己 
		SendForceRepeat(_self_id);
		if(_timer_index != -1) //试图结束自己的定时器
		{
			RemoveSelf();
		}
	}
	else
	{
		ChangeIntervalInCallback(interval);
		_next_interval = SECOND_TO_TICK(1);	//这个操作理论上可能由于时序引发错误，但并无大碍
		SendForceRepeat(_self_id);
	}
}

bool 
session_skill::OnAttacked()
{
	ASSERT(_session_id == _imp->GetCurSessionID());
	/*if(_session_id != _imp->GetCurSessionID())
	{
		return false;
	}*/
	if(_imp->SkillOnAttacked(_data))
	{
		_end_flag = true;
		RemoveTimer();
		SendEndMsg(_self_id);
		return true;
	}
	return false;
}

bool
session_skill::OnSilentSeal()
{
	ASSERT(_session_id == _imp->GetCurSessionID());
	/*if(_session_id != _imp->GetCurSessionID())
	{
		return false;
	}*/

	_end_flag = true;
	RemoveTimer();
	SendEndMsg(_self_id);
	return true;
}

bool
session_skill::OnBreakCast()
{
	ASSERT(_session_id == _imp->GetCurSessionID());
	/*if(_session_id != _imp->GetCurSessionID())
	{
		return false;
	}*/

	_end_flag = true;
	RemoveTimer();
	SendEndMsg(_self_id);
	return true;
}

bool session_skill::IsSkillPerforming(int id)
{
	return (int)_data.id == id && _data.skillstate == SKILL::SKILL_STATE_PERFORM;
}

//--------------------------------------------------
// spirit_session_skill
//--------------------------------------------------
void spirit_session_skill::SetTarget(int skill_id, char force_attack,int target_num,int * targets, int level, int item_id, short item_index, bool consumable, const A3DVECTOR& pos, char spirit_index)
{
	_data.id = skill_id;
	_data.level = level;
	_data.item_id = item_id;
	_data.item_index = item_index;
	_data.consumable = consumable;
	_data.forceattack = force_attack;
	_force_attack = force_attack;
	_data.pos = pos;
	_data.spirit_index = spirit_index;

	if(target_num > 0)
	{
		XID id;
		_target_list.reserve(target_num);
		for(int i = 0; i < target_num; i ++,targets ++)
		{
			MAKE_ID(id,*targets);
			_target_list.push_back(id);
		}
	}
}

bool spirit_session_skill::StartSession(act_session * next_cmd)
{
	_self_id = _imp->_parent->ID;
	_curr_cast_speed_rate = _imp->_skill.GetCurrCastSpeedRate();
	if(!_imp->StandOnGround() && !_imp->InFlying()) return false;
	if(!_imp->CheckSendSkillAttack(_data.id)) return false;

	int rangeType = GNET::SkillWrapper::RangeType(_data.id);
	if(!(rangeType == 6 || rangeType  == 11 || rangeType == 12 || rangeType == 13 || rangeType == 14 || rangeType == 15) && _target_list.size())
	{
		int skilltype = GNET::SkillWrapper::GetType(_data.id);
		XID target = _target_list[0];
		if( 1 == skilltype || 3 == skilltype )
		{
			if(target != _imp->GetCurTarget()) return false;
			_imp->NotifyStartAttack(_target_list[0],_force_attack);
		}
		if(!target.IsPlayer() && target != _imp->_last_attack_target)
		{
			_imp->OnAntiCheatAttack();
			_imp->_last_attack_target = target ;
		}
	}

	int first_interval = _imp->StartSkill(_data,_target_list.begin(),_target_list.size(),_next_interval);
	if(first_interval < 0)
	{
		_imp->_runner->error_message(S2C::ERR_SKILL_NOT_AVAILABLE);
		return false;
	}
	_imp->GetWorldManager()->OnCastSkill(_imp, _data.id);
	if(first_interval < TICK_MILLISEC)
	{
		__PRINTF("玩家瞬发技能\n");
		//瞬发技能
		int next_interval;
		_imp->RunSkill(_data,_target_list.begin(),_target_list.size(),next_interval);
		return false;
	}

	//将时间转换成tick
	__PRINTF("@@@@@@@@@@@@@@@spirit skill=%d, session=%d, spirit_index=%d, start: first_interval %d, next_interval %d\n", _data.id, _session_id, _data.spirit_index, first_interval, _next_interval);
	first_interval = MILLISEC_TO_TICK(first_interval);
	_next_interval = MILLISEC_TO_TICK(_next_interval);
	ASSERT(first_interval > 0);

	if(GNET::SkillWrapper::IsWarmup(_data.id)) 
	{
		_skill_skip_time = g_timer.get_tick();
		_next_interval += 20; //蓄力技能最多可以有20tick的延迟
	}

	
	SetTimer(g_timer,SECOND_TO_TICK(1),0,first_interval);

	//注册一个filter
	//_imp->_filters.AddFilter(new skill_interrupt_filter(_imp,_session_id,FILTER_INDEX_SKILL_SESSION));
	return true;
}

bool spirit_session_skill::RestartSession()
{
	if(!_data.skippable) return true;
	ASSERT(_session_id != -1);
	
	//重新分配session id
	_session_id = _imp->GetNextSpiritSessionID();

	//蓄力技能重新开始
	int tick = g_timer.get_tick() - _skill_skip_time;
	if(tick <= 0) tick = 0;
	
	//停止当前定时器
	RemoveTimer();

	int next_interval;
	
	int interval = _imp->ContinueSkill(_data,_target_list.begin(),_target_list.size(),next_interval,TICK_TO_MILLISEC(tick));
	//将时间转换成tick
	interval = MILLISEC_TO_TICK(interval);
	if(interval <= 0) return false;
	_next_interval = MILLISEC_TO_TICK(next_interval);

	if(_data.skippable) 
	{
		_skill_skip_time = g_timer.get_tick();
	}
	
	SetTimer(g_timer,SECOND_TO_TICK(1),0,interval);
	return true;
}

bool spirit_session_skill::EndSession()
{
	if(_session_id != -1)
	{
		if(_imp->IsInCircleOfDoom() && GNET::SkillWrapper::IsCycle(_data.id) && !_imp->_filters.IsFilterExist(FILTER_CYCLE))
		{
			_imp->SetCircleOfDoomStop();
		}
		//并非强制终止技能，试图终止之
		__PRINTF("@@@@@@@@@@@@spirit skill %d session %d , spirit_index=%d, end \n", _data.id, _session_id, _data.spirit_index);
		_session_id = -1;
		RemoveTimer();
		_imp->_filters.RemoveFilter(FILTER_INDEX_SKILL_SESSION);
		_imp->_runner->stop_skill(_data.spirit_index);
		if(_curr_cast_speed_rate != 0)
		{
			_curr_cast_speed_rate = 0;
		}
		_imp->_runner->player_stop_skill(_data.spirit_index);
		_imp->IncAttackStamp();
		timeval tv;
		gettimeofday(&tv,NULL);
		//__PRINTF("@@@@@@@@@@@@player %6d stop spirit skill %d session %d, spirit %d, at %ld.%06ld\n",_imp->_parent->ID.id, _data.id, _session_id, _data.spirit_index, tv.tv_sec,tv.tv_usec);
	}
	return true;
}

bool spirit_session_skill::RepeatSession()
{

	if(_end_flag) 
	{
		return false;	//结束
	}
	
	/*if(_target_list.size())
	{
		int skilltype = GNET::SkillWrapper::GetType(_data.id);
		char rangetype = GNET::SkillWrapper::RangeType(_data.id); 
		if( (1 == skilltype || 3 == skilltype ) && 0 == rangetype )
		{
			//if(_target_list[0] != _imp->GetCurTarget()) 
			{
				//return false;
			}
		}
	}*/

	int new_interval = -1;
	int rst = _imp->RunSkill(_data,_target_list.begin(),_target_list.size(),new_interval);
	if(rst <= 0 || _next_interval <= 0) 
	{
		return false;
	}

	__PRINTF("spirit skill %d repeat, session_id=%d, next interval %d\n", _data.id, _session_id, new_interval);
	if(_data.skippable) 
	{
		_skill_skip_time = g_timer.get_tick();
	}
	if(new_interval <= 0)
	{
		_next_interval  = new_interval;
		return true;
	}
	_next_interval  = MILLISEC_TO_TICK(new_interval);
	return true;
}

bool spirit_session_skill::TerminateSession(bool force)
{
	if(force) 
	{
		return EndSession();
	}

	if(_session_id != -1) 
	{
		if(_imp->CancelSkill(_data))
		{
			return EndSession();
		}
		return false;
	}
	return true;
}

void spirit_session_skill::OnTimer(int index,int rtimes, bool & is_delete)
{
	int interval = _next_interval;
	__PRINTF("%d spirit skill change timer %d %d\n",_self_id.id,interval,_end_flag);
	if(interval <= 0 || _end_flag)
	{
		//结束自己 
		SendSpiritRepeatMsg(_self_id);
		if(_timer_index != -1) //试图结束自己的定时器
		{
			RemoveSelf();
		}
	}
	else
	{
		ChangeIntervalInCallback(interval);
		_next_interval = SECOND_TO_TICK(1);	//这个操作理论上可能由于时序引发错误，但并无大碍
		SendSpiritRepeatMsg(_self_id);
	}
}

bool spirit_session_skill::OnAttacked()
{
	//ASSERT(_session_id == _imp->GetCurSessionID());
	/*if(_imp->SkillOnAttacked(_data))
	{
		_end_flag = true;
		RemoveTimer();
		SendSpiritEndMsg(_self_id);
		return true;
	}*/
	return false;
}

bool spirit_session_skill::OnSilentSeal()
{
	//ASSERT(_session_id == _imp->GetCurSessionID());

	/*_end_flag = true;
	RemoveTimer();
	SendSpiritEndMsg(_self_id);*/
	return true;
}

bool spirit_session_skill::OnBreakCast()
{
	//ASSERT(_session_id == _imp->GetCurSessionID());

	/*_end_flag = true;
	RemoveTimer();
	SendSpiritEndMsg(_self_id);*/
	return true;
}

bool 
session_produce::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_PRODUCE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	_imp->_runner->produce_start(_rt->id,_rt->use_time,_count);
	SetTimer(g_timer,_rt->use_time,0);
	return true;
}

bool 
session_produce::RepeatSession()
{
	if(_count <= 0) return false;

	gplayer_imp * pImp = ((gplayer_imp*)_imp);
	if(!pImp->ProduceItem(*_rt))
	{
		return false;
	}
	if(--_count == 0) return false;
	return true;
}

bool 
session_produce::EndSession()
{
	if(_session_id != -1)
	{
		_session_id = -1;
		RemoveTimer();
		_imp->_runner->produce_end();
		//发送停止制造的指令
	}
	return true;
}


bool 
session_produce::TerminateSession(bool force)
{
	//随时都可以中断
	return EndSession();
}

bool 
session_produce::Save(archive & ar) 
{
	act_timer_session::Save(ar);
	ar << _rt->id << _count;
	return true;
}

bool 
session_produce::Load(archive & ar) 
{
	act_timer_session::Load(ar);
	int id;
	ar >> id >> _count;
	_rt = recipe_manager::GetRecipe(id);
	ASSERT(_rt);
	return true;
}


bool 
session_use_item::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_USE_ITEM;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	if(_usetime == 0)
	{
		RepeatSession();
		return false;
	}
	_imp->_runner->start_use_item(_type,TICK_TO_MILLISEC(_usetime));

	SetTimer(g_timer,_usetime,1);
	return true;
}

bool 
session_use_item::RepeatSession()
{
	gplayer_imp * pImp = ((gplayer_imp*)_imp);
	//使用指定的物品
	if(!pImp->UseItem(_where,_index,_type))
	{
		//无法使用，发送终止使用的消息
		if(_usetime) 
			_imp->_runner->cancel_use_item();
		else
		{	
			//这个错误报告改在物品实现里了
			//_imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		}
	}
	return false;
}

bool 
session_use_item::EndSession()
{
	if(_session_id != -1)
	{
		_session_id = -1;
		RemoveTimer();
	}
	return true;
}

bool 
session_use_item::TerminateSession(bool force)
{
	//随时都可以中断
	_imp->_runner->cancel_use_item();
	return EndSession();
}

bool 
session_use_item::Save(archive & ar)
{
	act_timer_session::Save(ar);
	ar << _where << _index << _type << _count << _usetime;
	return true;
}

bool 
session_use_item::Load(archive & ar)
{
	act_timer_session::Load(ar);
	ar >> _where >> _index >> _type >> _count >> _usetime;
	return true;
}

void 
session_use_item::OnTimer(int index,int rtimes, bool & is_delete)
{
	SendForceRepeat(_self_id);
}

bool 
session_use_item_with_target::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_USE_ITEM;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	if(_usetime == 0)
	{
		RepeatSession();
		return false;
	}
	_imp->_runner->start_use_item_with_target(_type,TICK_TO_MILLISEC(_usetime),_target);

	SetTimer(g_timer,_usetime,1);
	return true;
}
bool 
session_use_item_with_target::Save(archive & ar)
{
	session_use_item::Save(ar);
	ar << _target << _force_attack;
	return true;
}

bool 
session_use_item_with_target::Load(archive & ar)
{
	session_use_item::Load(ar);
	ar >> _target >> _force_attack;
	return true;
}

bool 
session_use_item_with_target::RepeatSession()
{
	gplayer_imp * pImp = ((gplayer_imp*)_imp);
	//使用指定的物品
	if(!pImp->UseItemWithTarget(_where,_index,_type,_target,_force_attack))
	{	
		//无法使用，发送终止使用的消息
		if(_usetime) 
			_imp->_runner->cancel_use_item();
		else
			_imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	}
	return false;
}

bool
session_sit_down::StartSession(act_session * next_cmd)
{
	((gplayer_imp*)_imp)->SitDown();
	return false;
}

bool
session_sit_down::EndSession()
{
	return true;
}

bool 
session_sit_down::TerminateSession(bool force) 
{
	return true;
}

bool 
session_sit_down::RepeatSession() 
{
	return false;
}


bool 
session_gather_prepare::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	if(_tool_type > 0)
	{
		if(!pImp->IsItemExist(_where,_index,_tool_type,1))
		{
			pImp->_runner->error_message(S2C::ERR_MINE_HAS_INVALID_TOOL);
			return false;
		}
	}
	if(_task_id > 0)
	{
		//检查任务条件是否满足.................
	}

	int rst = pImp->CheckCanMineInCaptureRaid(_target);
	if(rst)
	{
		if (rst == 1)
			pImp->_runner->error_message(S2C::ERR_MINE_CANNOT_GATHER);
		else 	
			pImp->_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	if(pPlayer->IsInvisible())
	{
		pImp->_filters.RemoveFilter(FILTER_INDEX_INVISIBLE);
	}

	struct 
	{
		int level;
		int tool;
		int task;
		int roleid;
		int teamid;
		int masterid;
	}data;
	data.level = pImp->_basic.level;
	data.tool = _tool_type;
	data.task = _task_id;
	data.roleid = _role_id;
	data.teamid = _team_id;
	data.masterid = _master_id;
	
	pImp->SendTo<0>(GM_MSG_GATHER_REQUEST,XID(GM_TYPE_MATTER,_target),pImp->GetFaction(),&data,sizeof(data));
	return false;
}

bool 
session_gather::OnAttacked()
{
	ASSERT(_session_id == _imp->GetCurSessionID());
	if(_gather_flag)
	{
		RemoveTimer();
		SendEndMsg(_self_id);
		_gather_flag = false;
		return true;
	}
	return false;
}

bool session_gather::StartSession(act_session * next_cmd)
{
	if(!_imp->StandOnGround() && !_imp->InFlying()) return false;
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_GATHERING;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;


	//计算采集时间
	int use_time  = _gather_time & 0xFF;
	_imp->_runner->gather_start(_mine,use_time);
	//开始等待采集完成 
	SetTimer(g_timer,SECOND_TO_TICK(use_time),1);
	_gather_flag = true;

	if(_lock_inventory)
	{
		_imp->LockInventory(true);
	}

	if(_can_be_interruputed)
	{
		//注册一个filter
		_imp->_filters.AddFilter(new gather_interrupt_filter(_imp,_session_id,FILTER_INDEX_GATHER_SESSION));
	}
	((gplayer_imp*)_imp)->OnMineStart(_mine);
	return true;
}

bool session_gather::RepeatSession()
{
	return false;
}

bool session_gather::EndSession()
{
	if(_session_id != -1)
	{
		_session_id = -1;
		RemoveTimer();
		if(_gather_flag)
		{
			//这是正常采集完毕 发送采集消息
			SendMsg(GM_MSG_GATHER,XID(GM_TYPE_MATTER,_mine),_imp->_parent->ID);
		}
		else
		{
			//采集中断  发送中断采集消息
			SendMsg(GM_MSG_GATHER_CANCEL,XID(GM_TYPE_MATTER,_mine),_imp->_parent->ID);
		}
		if(_lock_inventory)
		{
			_imp->LockInventory(false);
		}
		//发送采集结束的消息
		_imp->_runner->gather_stop();
		if(_can_be_interruputed)
		{
			_imp->_filters.RemoveFilter(FILTER_INDEX_GATHER_SESSION);
		}
	}
	return true;
}

bool session_gather::TerminateSession(bool force)
{
	_gather_flag = false;
	EndSession();
	return true;
}


bool session_use_trashbox::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_TRAHSBOX;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	((gplayer_imp*)_imp)->TrashBoxOpen(_trash_type);

	//等待仓库使用完毕
	SetTimer(g_timer,47,0);
	return true;
}

bool session_use_trashbox::RepeatSession()
{
	return true;
}

bool session_use_trashbox::EndSession()
{
	if(_session_id != -1)
	{
		_session_id = -1;
		RemoveTimer();
		((gplayer_imp*)_imp)->TrashBoxClose();
	}
	return true;
}

bool session_use_trashbox::TerminateSession(bool force)
{
	return EndSession();
}


bool session_emote_action::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	ASSERT(_action != 0);

	_imp->_session_state = gactive_imp::STATE_SESSION_EMOTE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	_imp->SetEmoteState(_action);
	_imp->_runner->do_emote(_action);

	//开始等待采集完成 
	SetTimer(g_timer,29,0);
	return true;
}

bool session_emote_action::RepeatSession()
{
	return true;
}

bool session_emote_action::EndSession()
{
	if(_session_id != -1)
	{
		_session_id = -1;
		RemoveTimer();
		_imp->ClearEmoteState();
		_imp->_runner->do_emote_restore(_action);
	}
	return true;
}

bool session_emote_action::TerminateSession(bool force)
{
	return EndSession();
}

bool 
session_resurrect::StartSession(act_session * next_cmd)
{
	if(!_imp->_parent->IsZombie()) return false;
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_EMOTE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	SetTimer(g_timer,_time,1);
	return true;
}

void 
session_resurrect::OnTimer(int index,int rtimes, bool & is_delete)
{
	SendMsg(GM_MSG_OBJ_ZOMBIE_SESSION_END,_self_id,_self_id);
}

bool 
session_resurrect::EndSession()
{
	RemoveTimer();
	if(!_imp->_parent->IsZombie()) return true;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->Resurrect(pImp->_parent->pos,true,_exp_reduce,1);
	return true;
}

bool 
session_resurrect_by_item::EndSession()
{
	RemoveTimer();
	if(!_imp->_parent->IsZombie()) return true;
	gplayer_controller * pCtrl = (gplayer_controller *)(_imp->_commander);
	pCtrl->ResurrectByItem(_exp_reduce);
	return true;
}

bool 
session_resurrect_in_town::EndSession()
{
	RemoveTimer();
	gplayer_controller * pCtrl = (gplayer_controller *)(_imp->_commander);
	pCtrl->ResurrectInTown(_exp_reduce);
	return false;
}

bool
session_enter_sanctuary::StartSession(act_session * next_cmd)
{
	((gplayer_imp*)_imp)->TestSanctuary();
	return false;
}

bool
session_say_hello::StartSession(act_session * next_cmd)
{
	((gplayer_imp*)_imp)->SayHelloToNPC(_target);
	return false;
}

void 
session_instant_skill::SetTarget(int skill_id, char force_attack,int target_num,int * targets, int level, int item_id, short item_index, bool consumable, const A3DVECTOR& pos, char spirit_index)
{
	_data.id = skill_id;
	_data.level = level;
	_data.item_id = item_id;
	_data.item_index = item_index;
	_data.consumable = consumable;
	_data.forceattack = force_attack;
	_data.pos = pos;
	_data.spirit_index = spirit_index;

	if(target_num > 0)
	{
		XID id;
		_target_list.reserve(target_num);
		for(int i = 0; i < target_num; i ++,targets ++)
		{
			MAKE_ID(id,*targets);
			_target_list.push_back(id);
		}
	}
}

void 
session_instant_skill::SetCharge(A3DVECTOR & pos, XID & target)
{
	_chargePos = pos;
	_chargeTarget = target;
}

bool 
session_instant_skill::StartSession(act_session * next_cmd)
{
	if(!_imp->StandOnGround() && !_imp->InFlying()) return false;
	//为变身副本随机技能添加
	if(!_imp->SetSeekRaidRandomSkillAttack(_data.id)) return false;
	if(!_imp->CheckSendSkillAttack(_data.id)) return false;
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	__PRINTF("use instant skill................. %d\n", _data.id);
	int skilltype = GNET::SkillWrapper::GetType(_data.id);
	int rangeType = GNET::SkillWrapper::RangeType(_data.id);
	if(!(rangeType == 6 || rangeType == 11 || rangeType == 12 || rangeType == 13 || rangeType == 14 || rangeType == 15) && _target_list.size())
	{
		if( 1 == skilltype || 3 == skilltype )
		{
			if(_target_list[0] != _imp->GetCurTarget()) return false;
			_imp->NotifyStartAttack(_target_list[0], _data.forceattack);
		}
	}

	int rst = _imp->CastInstantSkill(_data,_target_list.begin(),_target_list.size(),_chargePos,_chargeTarget);
	_imp->IncAttackStamp();
	if(rst < 0)
	{
		_imp->_runner->error_message(S2C::ERR_SKILL_NOT_AVAILABLE);
		return false;
	}
	_imp->GetWorldManager()->OnCastSkill(_imp, _data.id);
	__PRINTF("use instant skill startsession................. %d\n", _data.id);
	return false;
}

bool
session_region_transport::StartSession(act_session * next_cmd)
{
	gplayer_imp * pImp = (gplayer_imp*)_imp;
	if(!pImp->RegionTransport(_ridx, _tag))
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_ENTER_INSTANCE);
	}
	return false;
}

bool 
session_resurrect_protect::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_EMOTE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	SetTimer(g_timer,SECOND_TO_TICK(PLAYER_REBORN_PROTECT),1);
	_imp->_filters.AddFilter(new invincible_banish_filter(_imp,FILTER_INVINCIBLE_BANISH));
	_imp->_runner->resurrect(1);
	return true;

}

bool 
session_resurrect_protect::EndSession()
{
	RemoveTimer();
	//发一个消息给玩家 告诉玩家可以移动啦 
	_imp->_filters.RemoveFilter(FILTER_INVINCIBLE_BANISH);
	_imp->_runner->resurrect(2);
	return true;
}


bool 
session_pos_skill::StartSession(act_session * next_cmd)
{
	if(!_imp->StandOnGround() && !_imp->InFlying()) return false;
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_SKILL;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	__PRINTF("use pos skill................. %d\n", _data.id);

	int first_interval = _imp->StartSkill(_data,_target_pos,_next_interval);
	if(first_interval < 0)
	{
		_imp->_runner->error_message(S2C::ERR_SKILL_NOT_AVAILABLE);
		return false;
	}
	_imp->GetWorldManager()->OnCastSkill(_imp, _data.id);
	if(first_interval < TICK_MILLISEC)
	{
		__PRINTF("玩家瞬发技能\n");
		//瞬发技能
		int next_interval;
		first_interval = _imp->RunSkill(_data,_target_pos,next_interval);
		if(first_interval > TICK_MILLISEC)
		{	
			_next_interval = MILLISEC_TO_TICK(next_interval);
			SetTimer(g_timer,SECOND_TO_TICK(1),0,MILLISEC_TO_TICK(first_interval));
			return true;
		}
		return false;
	}

	//将时间转换成tick
	__PRINTF("skill session start: first_interval %d, next_interval %d\n",first_interval, _next_interval);
	first_interval = MILLISEC_TO_TICK(first_interval);
	_next_interval = MILLISEC_TO_TICK(_next_interval);
	ASSERT(first_interval > 0);

	
	SetTimer(g_timer,SECOND_TO_TICK(1.0f),0,first_interval);

	//注册一个filter
	return true;
}

bool
session_pos_skill::EndSession()
{
	if(_session_id != -1)
	{
		//并非强制终止技能，试图终止之
		_session_id = -1;
		RemoveTimer();
		_imp->_runner->stop_skill(-1);
		_imp->IncAttackStamp();
		__PRINTF("pos skill end\n");
	}
	return true;
}

bool 
session_pos_skill::RepeatSession()
{
	if(_end_flag) return false;	//结束

	int new_interval = -1;
	int rst = _imp->RunSkill(_data,_target_pos,new_interval);
	if(rst <= 0 || _next_interval <= 0) return false;

	__PRINTF("skill repeat , next interval %d\n",new_interval);
	if(new_interval <= 0)
	{
		_next_interval  = new_interval;
		return true;
	}
	_next_interval  = MILLISEC_TO_TICK(new_interval);
	return true;
}

bool 
session_pos_skill::TerminateSession(bool force)
{
	if(force) 
	{
		return EndSession();
	}

	if(_session_id != -1) 
	{
		if(_imp->CancelSkill(_data))
		{
			return EndSession();
		}
		return false;
	}
	return true;
}

void 
session_pos_skill::OnTimer(int index,int rtimes, bool & is_delete)
{
	__PRINTF("skill change timer %d\n",_next_interval);
	if(_next_interval <= 0 || _end_flag)
	{
		//结束自己 
		SendForceRepeat(_self_id);
		if(_timer_index != -1) //试图结束自己的定时器
		{
			RemoveSelf();
		}
	}
	else
	{
		ChangeIntervalInCallback(_next_interval);
		_next_interval = SECOND_TO_TICK(1.0f);
		SendForceRepeat(_self_id);
	}
}


bool 
session_general::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);

	_imp->_session_state = gactive_imp::STATE_SESSION_GENERAL;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	if(_delay <=  0) _delay = 100;

	SetTimer(g_timer,_delay,1);
	OnStart();
	return true;
}

bool 
session_general::RepeatSession()
{
	OnRepeat();
	return false;
}

void 
session_general::OnTimer(int index,int rtimes, bool & is_delete)
{
	SendForceRepeat(_self_id);
}

bool 
session_general::EndSession()
{
	if(_session_id != -1)
	{
		_session_id = -1;
		RemoveTimer();
		OnEnd();
	}
	return true;
}

bool 
session_general::TerminateSession(bool force)
{
	return EndSession();
}

void 
session_pet_operation::OnStart()
{
	_imp->_runner->player_start_pet_operation(_pet_index,_pet_id,_delay,_op);
}

void session_pet_operation::OnEnd()
{
	_imp->_runner->player_end_pet_operation(_op);
}

void session_summon_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->SummonPet(_pet_index);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_recall_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->RecallPet(_pet_index);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_combine_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->CombinePet(_pet_index,_type);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_uncombine_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->UncombinePet(_pet_index,_type);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_free_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->FreePet(_pet_index);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_adopt_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->AdoptPet(_pet_index);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_refine_pet::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->RefinePet(_pet_index,_index_c,_index_a);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

void session_refine_pet2::OnRepeat()
{
	gplayer_imp* pImp = (gplayer_imp*)_imp;
	int rst = pImp->RefinePet2(_pet_index,_type);
	if(0 != rst)
	{
		pImp->_runner->error_message(rst);
	}
}

bool 
session_keeping_sitdown::StartSession(act_session * next_cmd)
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->_session_state = gactive_imp::STATE_SESSION_SITDOWN;
	_session_id = pImp->GetNextSessionID();
	pImp->_filters.AddFilter(new sitdown_filter(pImp,_session_id,FILTER_INDEX_SITDOWN));
	pImp->EnterStayInState();
	return true;
}

bool 
session_keeping_sitdown::EndSession()
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->LeaveStayInState();
	pImp->_filters.RemoveFilter(FILTER_INDEX_SITDOWN);
	return true;
}

bool 
session_keeping_sitdown::OnAttacked() 
{ 
	SendEndMsg(_imp->_parent->ID);
	return true;
}

void 
session_general_operation::OnStart()
{
	_imp->_runner->start_general_operation(_op->GetID(),_delay,_op->NeedBoardcast());
	_op->_session_id = _session_id;
	_op->OnStart(_imp);
}

void 
session_general_operation::OnRepeat()
{
	_op->OnEnd(_imp);
}

void 
session_general_operation::OnEnd()
{
	_op->PostEnd(_imp);
	_imp->_runner->stop_general_operation(_op->GetID(),_op->NeedBoardcast());
}

bool 
session_general_operation::OnAttacked()
{
	return _op->OnAttacked(_imp);
}

bool 
session_online_agent::StartSession(act_session * next_cmd)
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->_session_state = gactive_imp::STATE_SESSION_SITDOWN;
	_session_id = pImp->GetNextSessionID();
	pImp->_filters.AddFilter(new online_agent_filter(pImp,_session_id, FILTER_INDEX_ONLINE_AGENT));
	pImp->EnterStayInState();
	return true;
}

bool 
session_online_agent::EndSession()
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->LeaveStayInState();
	pImp->_filters.RemoveFilter(FILTER_INDEX_ONLINE_AGENT);
	return true;
}

bool 
session_online_agent::OnAttacked() 
{ 
	SendEndMsg(_imp->_parent->ID);
	return true;
}

bool 
session_logon_invincible::StartSession(act_session * next_cmd)
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->_session_state = gactive_imp::STATE_SESSION_LOGON;
	_session_id = pImp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	pImp->_filters.AddFilter(new invincible_logon_filter(pImp,FILTER_INDEX_LOGON_INVINCIBLE,_time));
	SetTimer(g_timer,SECOND_TO_TICK(_time),1);
	return true;
}

bool 
session_logon_invincible::EndSession()
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->_filters.RemoveFilter(FILTER_INDEX_LOGON_INVINCIBLE);
	return true;
}



//-------------------------------------------------------------
// session_follow_target implementation
//-------------------------------------------------------------

session_follow_target::~session_follow_target()
{
	stop = true;
	_timeout = 0;
	if(_agent)
	{
		delete _agent;
	}
	_imp->_runner->stop_move(_imp->_parent->pos,_speed,1, C2S::MOVE_MODE_PULL);
}

bool 
session_follow_target::StartSession(act_session * next_cmd)
{
	//后面有操作就不继续了，因为这样就无法进行正确的处理了
	if(next_cmd) return false;
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gplayer_controller)));
	ASSERT(_target.id != -1);
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	stop = false;

	Run();
	SetTimer(g_timer, SECOND_TO_TICK(NPC_FOLLOW_TARGET_TIME),0);
	return true;
}

bool 
session_follow_target::RepeatSession()
{
	if(--_timeout <= 0) return false;
	return Run();
}

bool 
session_follow_target::EndSession()
{
	stop = true;
	if(_session_id != -1)
	{
		//发送NPC停止的命令，暂时   
		if(!_imp->GetRootMode() && !_stop_flag) 
		{
			_imp->_runner->stop_move(_imp->_parent->pos,_speed,1, C2S::MOVE_MODE_PULL);
		}

		_session_id = -1;
		RemoveTimer();
	}
	return true;
}

void 
session_follow_target::TrySendStop()
{
	if(!_imp->GetRootMode() && !_stop_flag) 
	{
		_stop_flag = true;
		_imp->_runner->stop_move(_imp->_parent->pos,_speed,1,C2S::MOVE_MODE_PULL);
	}
}

int 
session_follow_target::Run()
{
#define TEST_GETTOGOAL()  if(_agent->GetToGoal()) \
	{\
		if(++_reachable_count>= 3) \
		{\
			return 0; \
		}\
		else\
		{\
			TrySendStop();\
			return 1;\
		}\
	}

	if(stop)
	{
		return -1;
	}
	
	float range;
	world::object_info info;
	if(!_imp->_plane->QueryObject(_target,info)
			|| (info.state & world::QUERY_OBJECT_STATE_ZOMBIE)
			|| (range= info.pos.squared_distance(_imp->_parent->pos)) >= _range_max)
	{
		// 追寻目标失败
		return 0;
	}
	//if(_imp->GetRootMode()) return 0;

	if(range < _range_min)
	{
		_retcode = 0;
		return 0;
	}

	const A3DVECTOR & tmpPos = _imp->_parent->pos;
	float speed = GetSpeed() * NPC_FOLLOW_TARGET_TIME;
	bool first_call = false;
	if(!_agent)
	{
		first_call = true;
		_agent = new path_finding::follow_target();
		_agent->CreateAgent(_imp->GetWorldManager()->GetMoveMap(), 0);
		_agent->Start(tmpPos,info.pos,speed,_range_target,range);
		TEST_GETTOGOAL();
	}
	else
	{
		bool is_knocked_back = ((gnpc_imp*)_imp)->TestKnockBackFlag();
		if(_agent->GetToGoal() || is_knocked_back) 
		{
			//这里肯定没有达到能够结束的要求，因为前面判断过了
			_agent->Start(tmpPos,info.pos,speed,_range_target*0.6f,range);
			//检测是否可达，如是，则存在着高度差 
			//如果存在那么应该按照无法到达来进行
			TEST_GETTOGOAL();
		}
		else
		{
			//这里需要判断一下是否目标移动过多，以至于需要重新计算位置
			const A3DVECTOR & oldtarget = _agent->GetTarget();
			float x = oldtarget.x - info.pos.x;
			float z = oldtarget.z - info.pos.z;
			float dis = x*x + z*z;
			if( dis > 49.f || (dis > 16.f && !_agent->IsSearching()))
			{
				//重新进行Start
				_agent->Start(tmpPos,info.pos,speed,_range_target,range);
				TEST_GETTOGOAL();
			}
		}
	}

	// 不断的移动！
	if(!_agent->MoveOneStep(speed))
	{
		if(first_call) 
		{
			return 1;
		}
		return 0;
	}

	
	// 移动后得到的新位置！传出给客户端！ 应该再判断一下是否真的移动了
	A3DVECTOR targetpos;
	_agent->GetCurPos(targetpos);	
	//提出地面一点防止人物卡住
	targetpos.y += 0.3;

	//确认玩家的目标点是否在空中
	trace_manager & man = _imp->GetWorldManager()->GetTraceMan();
	if(!man.Valid()) return true;
	bool is_solid;
	float ratio;
	bool bRst = man.AABBTrace(targetpos, A3DVECTOR(0,-10,0), A3DVECTOR(0.075,0.1125,0.075), is_solid,ratio);
	//bool bRst = man.AABBTrace(targetpos, A3DVECTOR(0,-10,0), A3DVECTOR(0.3,0.9,0.3), is_solid,ratio);
	if(bRst && is_solid) 
	{
		printf("玩家%d跟随目标%d嵌入了建筑，直接退出\n", _imp->_parent->ID.id, _target.id);
		return 0;	//目标点嵌入了建筑 直接返回即可
	}
	
	A3DVECTOR offset= targetpos;
	offset -= _imp->_parent->pos;
	if(offset.squared_magnitude() < 1e-3)
	{
		//用距离判断是否真正发生了移动
		TrySendStop();
		return 1;
	}
	_stop_flag = false;
	
//	printf("Player Follow target: %d, (x=%3f, y=%3f, z=%f)\n", _target.id, targetpos.x, targetpos.y, targetpos.z);
	if(_imp->StepMove(offset)) 
	{
		//广播给other players
		_imp->_runner->move(targetpos, 
				(int)(NPC_FOLLOW_TARGET_TIME*1000.f + 0.1f),
				(unsigned short)(GetSpeed()* 256.0f + 0.5f),
				C2S::MOVE_MODE_PULL);
		//发给自己的特殊处理
		_imp->_runner->be_moved(targetpos, 
				(int)(NPC_FOLLOW_TARGET_TIME*1000.f + 0.1f),
				(unsigned short)(GetSpeed()* 256.0f + 0.5f),
				C2S::MOVE_MODE_PULL);
	}
	return 1;

#undef TEST_GETTOGOAL
}

bool
session_active_emote::StartSession(act_session * next_cmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->_session_state = gactive_imp::STATE_SESSION_EMOTE;
	_session_id = pImp->GetNextSessionID();
	pImp->_filters.AddFilter(new active_emote_filter(pImp,_session_id,FILTER_INDEX_ACTIVE_EMOTE));
	return true;
}

bool
session_active_emote::EndSession()
{
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	pImp->_filters.RemoveFilter(FILTER_INDEX_ACTIVE_EMOTE);
	pImp->PlayerActiveEmoteCancel();
	return true;
}

bool
session_active_emote::OnAttacked()
{
	SendEndMsg(_imp->_parent->ID);
	return true;
}
