#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include <common/protocol_imp.h>


DEFINE_SUBSTANCE(gplayer_controller,controller,CLS_PLAYER_CONTROLLER)

//just test

void 
gplayer_controller::Release(bool free_parent)
{
	//这里的free_parent参数不进行处理，一律按照true来处理
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gactive_imp * pImp = (gactive_imp*)_imp;
	pImp->ClearSession();
	slice * pPiece = pPlayer->pPiece;
	world * pPlane = _imp->_plane;
	if(pPiece)
	{
		pPlane->RemovePlayer(pPlayer);
	}

	pPlane->RemovePlayerToMan(pPlayer);
	gmatrix::UnmapPlayer(pPlayer->ID.id);
	
	//释放顺序必须这样，因为imp的析构里可能会使用_imp和_runner
	dispatcher * runner = _imp->_runner;
	delete _imp;
	delete runner;
	delete this;
	pPlayer->imp = NULL;
	pPlane->FreePlayer(pPlayer);
}

gplayer_controller::~gplayer_controller()
{
	if(_gm_auth) delete _gm_auth;
}

gplayer_controller::gplayer_controller():_cur_target(-1,-1),_load_stats(0),_peep_counter(0),_select_counter(0),_debug_command_enable(false),_banish_counter(0),_move_cmd_seq(0),_gm_auth(NULL)
{
	memset(_deny_cmd_list,0,sizeof(_deny_cmd_list));
	_pickup_counter = MAX_PICKUP_PER_SECOND;	//默认有5个最大
	_pickup_timestamp = g_timer.get_systime();
	_safe_lock_timestamp = 0;
	_max_safe_lock = 0;
}
	
void 
gplayer_controller::Init(gobject_imp * imp)
{
	controller::Init(imp);
	_last_pos = _imp->_parent->pos;
}

int 
gplayer_controller::MoveBetweenSlice(gobject * obj,slice * src, slice * dest)
{
	ASSERT(obj == _imp->_parent);
	return src->MovePlayer((gplayer*)obj,dest);
}

bool 
gplayer_controller::Save(archive & ar)
{
	_load_stats += 5;
	ar << _cur_target << _load_stats << _last_pos << _peep_counter 
		<< _debug_command_enable << _banish_counter << _move_cmd_seq <<_peep_counter;
	ar.push_back(_deny_cmd_list,sizeof(_deny_cmd_list));
	if(_gm_auth)
	{
		char buf[PRIVILEGE_NUM + 1];
		int authsize = _gm_auth->GetBuf(buf,sizeof(buf));
		ASSERT(authsize >= 0);
		if(authsize <0) authsize = 0;
		ar << authsize;
		ar.push_back(buf,authsize);
	}
	else
	{
		ar << (int)0;
	}
	return true;
}
bool 
gplayer_controller::Load(archive & ar)
{
	ar >> _cur_target >> _load_stats >> _last_pos >> _peep_counter
		>> _debug_command_enable >> _banish_counter >> _move_cmd_seq >>_peep_counter;
	ar.pop_back(_deny_cmd_list,sizeof(_deny_cmd_list));
	int authsize;
	ar >> authsize;
	ASSERT(authsize >=0);
	ASSERT(authsize <= PRIVILEGE_NUM);
	if(authsize > 0 && authsize <= PRIVILEGE_NUM)
	{
		char buf[PRIVILEGE_NUM + 1];
		ar.pop_back(buf,authsize);
		SetPrivilege(buf,authsize);
	}
	return true;
}

void
gplayer_controller::error_cmd(int msg)
{
//	ASSERT(msg != S2C::ERR_FATAL_ERR);
	return _imp->_runner->error_message(msg);
}

void 
gplayer_controller::OnHeartbeat(size_t tick)
{
	gplayer_imp *pImp =(gplayer_imp*)_imp;

	if(_safe_lock_timestamp > 0)
	{
		if(--_safe_lock_timestamp <= 0)
		{
			_imp->_runner->notify_safe_lock(0,0, _max_safe_lock );
		}
	}

	//测试是否发送自己的info00
	if(pImp->_refresh_state)
	{
		pImp->_runner->query_info00();
	}

	if(_cur_target.id != -1)
	{
		_select_counter ++;
		world::object_info info;
		gplayer * pPlayer = (gplayer*)_imp->_parent;
		if(_select_counter > 30 		//30秒超时
				|| !pImp->_plane->QueryObject(_cur_target,info) 
				|| info.pos.horizontal_distance(_imp->_parent->pos) > SELECT_IGNORE_RANGE * SELECT_IGNORE_RANGE
				|| (info.invisible_rate > pPlayer->anti_invisible_rate && _cur_target.id != pPlayer->ID.id
			        && !(_cur_target.IsPlayer() && pImp->IsInTeam() && pImp->IsMember(_cur_target))))
		{
			UnSelect();
		}
		else
		{
		}
	}
	TryPeepMobs();


	if(_banish_counter >0)
	{
		_banish_counter --;
	}
	//进行负载更新的工作
	if((_load_stats -= 10)<0)
	{
		_load_stats = 0;
	}

	if(_load_stats >= 450)
	{
		if(!_banish_counter)
		{
			GLog::log(GLOG_INFO,"玩家%d被暂时放逐\n",_imp->_parent->ID.id);
			error_cmd(S2C::ERR_YOU_HAS_BEEN_BANISHED);
		}
		//超过负载设计	
		//进入退避模式
		_banish_counter = 15;
	}

	return ;
}

void 
gplayer_controller::OnResurrect()
{
	_peep_counter = -PLAYER_REBORN_PROTECT;
}

void
gplayer_controller::TryPeepMobs()
{
	if(_peep_counter < 0)
	{
		++_peep_counter;
		return ;
	}
	gplayer_imp * pImp = (gplayer_imp *) _imp;
	gplayer *pPlayer = pImp->GetParent();
	if(pImp->_runner->is_invisible()) return;
	if(pImp->_parent->IsZombie()) return;
	if(pPlayer->bind_type == 2 || pPlayer->bind_type == 4) return;
	bool is_peep = false;
	if(pImp->_is_moved)
	{
		if(_last_pos.squared_distance(pImp->_parent->pos) >= 2.f*2.f )
		{
			pImp->_is_moved = false;
			is_peep = true;
		}
		else
		{
			//可以考虑用别的方法来减少这种广播的发送
			is_peep = ((++_peep_counter) >= 3);
			pImp->_is_moved = false;
		}
	}
	else
	{
		is_peep = ((++_peep_counter) >= 3);
	}

	if(is_peep)
	{
		_peep_counter = 0;
		_last_pos = pImp->_parent->pos;
		
		gplayer * pPlayer = (gplayer*)pImp->_parent;
		MSG msg;
		msg_watching_t mwt={pImp->GetFaction(),pPlayer->invisible_rate, pPlayer->id_family, pPlayer->id_mafia, pPlayer->id_zone};
		BuildMessage(msg,GM_MSG_WATCHING_YOU,XID(GM_TYPE_NPC,-1),pPlayer->ID,pPlayer->pos,0,&mwt,sizeof(mwt));
		world *pPlane = pImp->_plane;
		float tmp = pPlane->GetWorldManager()->GetMaxMobSightRange();
		pPlane->BroadcastMessage(msg,tmp,gobject::MSG_MASK_PLAYER_MOVE);
	}

}

/*
void TTTTT(gactive_imp *pImp)
{
	gobject * pObj  = (gplayer*)pImp->_parent;
	MSG msg;
	msg_watching_t mwt={pImp->_basic.level,pImp->GetFaction(),((gactive_object*)pImp->_parent)->invisible_rate};
	BuildMessage(msg,GM_MSG_WATCHING_YOU,XID(GM_TYPE_NPC,-1),pObj->ID,pObj->pos,0,&mwt,sizeof(mwt));
	world *pPlane = pImp->_plane;
	float tmp = pPlane->GetWorldManager()->GetMaxMobSightRange();
	pPlane->BroadcastMessage(msg,tmp,gobject::MSG_MASK_PLAYER_MOVE);
}
*/

void gplayer_controller::UnSelect()
{
	if(_cur_target.type != -1)
	{
		((gplayer_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_TARGET,_cur_target,0);
		_cur_target.type = -1;
		_cur_target.id = -1;
	}
	_select_counter = 0;
	_imp->_runner->unselect();
}


void gplayer_controller::SelectTarget(int id,bool client_cmd)
{
	if(id == -1)
	{
		UnSelect();
		return ;
	}
	if(id == _cur_target.id) 
	{
		_imp->_runner->player_select_target(id);
		return;
	}

	XID target;
	MAKE_ID(target,id);
	//查询对象是否存在和距离是否过远

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp *pImp =(gplayer_imp*)_imp;
	world::object_info info;
	if(!_imp->_plane->QueryObject(target,info)
			|| info.pos.squared_distance(_imp->_parent->pos) > SELECT_IGNORE_RANGE * SELECT_IGNORE_RANGE  
			|| (info.invisible_rate > pPlayer->anti_invisible_rate && target.id != pPlayer->ID.id
		        && !(target.IsPlayer() && pImp->IsInTeam() && pImp->IsMember(target))))
	{
		//不满足选择条件,并且是客户端发来的命令
		if(client_cmd) ((gplayer_dispatcher*)_imp->_runner)->object_is_invalid(id);
		return;
	}

	_select_counter = 0;
	if(_cur_target.id != -1)
	{
		((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_TARGET,_cur_target,0);
	}
	_cur_target = target;

	//发送数据
	link_sid ld;
	ld.cs_id = pPlayer->cs_index;
	ld.cs_sid = pPlayer->cs_sid;
	ld.user_id = pPlayer->ID.id;
	((gactive_imp*)_imp)->SendTo<0>(GM_MSG_SUBSCIBE_TARGET,target,0,&ld,sizeof(ld));
	_imp->_runner->player_select_target(target.id);
}

void gplayer_controller::SubscibeConfirm(const XID & who)
{
	if(who != _cur_target)
	{
		((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_TARGET,who,0);
	}
	else
	{
		_select_counter = 0;
	}
}

void 
gplayer_controller::ReInit()
{
	gactive_object * pParent= (gactive_object*)(_imp->_parent);
	if(_gm_auth)
		pParent->object_state |= gactive_object::STATE_GAMEMASTER;
	else
		pParent->object_state &= ~gactive_object::STATE_GAMEMASTER;
}

void gplayer_controller::SetPrivilege(const void * data, size_t size)
{
//	ASSERT(!_gm_auth);
	gactive_imp * pImp = (gactive_imp*)_imp;
	if(size && data)
	{
		_gm_auth = new GNET::Privilege();
		_gm_auth->Init(data,size);
		
		if(pImp && pImp->_parent) ((gactive_object*)pImp->_parent)->object_state |= gactive_object::STATE_GAMEMASTER;
	}
	else
	{
		if(_gm_auth)
		{
			delete _gm_auth;
			_gm_auth = NULL;
		}
		if(pImp && pImp->_parent) ((gactive_object*)pImp->_parent)->object_state &= ~gactive_object::STATE_GAMEMASTER;
	}
}


bool 
gplayer_controller::HasGMPrivilege()
{
	return  _gm_auth;
}

void 
gplayer_controller::DenyCmd(size_t cmd_type)
{
	if(cmd_type >= CMD_MAX) return;
	_deny_cmd_list[cmd_type] ++;
}

void 
gplayer_controller::AllowCmd(size_t cmd_type)
{
	if(cmd_type >= CMD_MAX) return;
	_deny_cmd_list[cmd_type] --;
	if(_deny_cmd_list[cmd_type] < 0)
	{
		_deny_cmd_list[cmd_type]  = 0;
		GLog::log(LOG_ERR,"用户%d AllowCmd发生错误 %d",_imp->_parent->ID.id, cmd_type);
	
	}
}

