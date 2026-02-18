#include "aei_filter.h"
#include "clstab.h"
#include "world.h"
#include "player_imp.h"
#include "sfilterdef.h"

DEFINE_SUBSTANCE(aei_filter,filter,CLS_FILTER_CHECK_INSTANCE_KEY)
DEFINE_SUBSTANCE(aebf_filter,filter,CLS_FILTER_CHECK_BATTLEFIELD_KEY)

void 
aei_filter::OnAttach()
{
	_key =_parent.GetImpl()->_plane->w_ins_key;
	_worldtag = _parent.GetImpl()->GetWorldTag();
}

void 
aei_filter::OnDetach()
{
	//do nothing
}

void 
aei_filter::Heartbeat(int tick)
{
	bool is_dead = _parent.IsDead();
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	bool is_kick = pImp->_plane->w_ins_kick;
	if(_state == NORMAL && (_timeout -= tick) < 0)
	{
		instance_key key;
		pImp->GetInstanceKey(_worldtag, key);
		key.target = key.essence;
		//这里的比较key是否一致是否应该由world_manager来判断？
		if(!pImp->GetWorldManager()->CompareInsKey(key,_key) && !is_dead && is_kick)
		{
			//需要踢出，检查是否GM
			if(pImp->GetWorldManager()->GetWorldLimit().gmfree && _parent.CheckGMPrivilege())
			{
				//GM 且本世界允许GM随意穿越 ，则不会被踢出
				_timeout = 3;
			}
			else
			{
				_state = WAIT_ESCAPE;
				_timeout = 60;
				pImp->_runner->kickout_instance(_timeout);
			}
		}
		else
		{
			//每三秒查一次
			_timeout = 3;
		}
	}
	else if(_state == WAIT_ESCAPE)
	{
		if((_timeout -= tick) <= 0)
		{
			//到时间了，踢出副本
			__PRINTF("副本该踢出辣\n");
			pImp->LeaveAbnormalState();
			if(!pImp->ReturnToTown())
			{
				const struct world_pos& pos = pImp->GetWorldManager()->GetSavePoint();
				if(pos.tag > 0)
				{
					pImp->LongJump(pos.pos,pos.tag);
				}
			}
			//重置一下timeout
			_timeout = 5;
		}
		else
		{
			//再检查是否恢复了
			instance_key key;
			pImp->GetInstanceKey(_worldtag, key);
			key.target = key.essence;
			if(pImp->GetWorldManager()->CompareInsKey(key,_key) || is_dead || !is_kick)
			{
				_state = NORMAL;
				_timeout = 3;
				pImp->_runner->kickout_instance(-1);
			}
		}
	}
}

/*--------------------------------------------------------*/
void 
aebf_filter::OnAttach()
{
}

void 
aebf_filter::OnDetach()
{
	//do nothing
}

void  
aebf_filter::OnModify(int ctrlname,void * ctrlval,size_t ctrllen)
{
	if(ctrlname == FMID_CLEAR_AEBF)
	{
		_origin_mafia = -1;
	}
}

void 
aebf_filter::Heartbeat(int tick)
{
	int cur_mafia = _parent.GetMafiaID();
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	world * pPlane = pImp->_plane;

//计算和统计分数
	int attacker_score = pPlane->w_offense_cur_score;
	int defender_score = pPlane->w_defence_cur_score;

	if(attacker_score != _attacker_score || defender_score != _defender_score)
	{
		//攻击和防守的分数要反过来
//		pImp->_runner->battle_score( defender_score,pPlane->w_defence_goal, attacker_score,pPlane->w_offense_goal);

		_attacker_score = attacker_score;
		_defender_score = defender_score;
	}

//分析结果是否出现
	if(!_battle_result)
	{
		if(pImp->_plane->w_battle_result != _battle_result)
		{
			_battle_result = pImp->_plane->w_battle_result;
			pImp->_runner->battle_result(_battle_result, 0, 0);

			//激活准备离开的操作
			_battle_end_timer = 4;
			_timeout = 60;
		}
	}

//准备退出倒数
	if(_battle_end_timer)
	{
		_battle_end_timer --;
		if(_battle_end_timer <= 0)
		{
			_timeout = 60;
			pImp->_runner->kickout_instance(_timeout);
		}
	}

	
	
	if(_battle_result)
	{
		if(_timeout > 0)  _timeout --;
		if(_timeout <=0)
		{
			_kickout ++;
		}
	
	}
	else
	{
		if(cur_mafia != _origin_mafia || _origin_mafia <= 0)
		if(_origin_mafia <= 0 || 
		   	cur_mafia != _origin_mafia &&
		    	!(pImp->GetWorldManager()->GetWorldLimit().gmfree && _parent.CheckGMPrivilege()) )
		{
			_origin_mafia = -1;
			if(_timeout > 0)  _timeout --;
			if(_timeout <=0)
			{
				_kickout ++;
			}
		}
	}

	if(_kickout && _timeout <=0)
	{
		if(_kickout > 5)
		{
			//如果多次都不能离开副本，则断线之
			_is_deleted = true;
			pImp->LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
		}
		else
		{
			pImp->LeaveAbnormalState();
			_timeout = 3;
			//使用这里的存盘点来处理
			A3DVECTOR pos;
			int tag;
			pImp->GetWorldManager()->GetLogoutPos(pImp,tag,pos);
			if(tag > 0)
			{
				pImp->LongJump(pos,tag);
			}
		}
	}
}

