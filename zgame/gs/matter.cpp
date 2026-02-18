#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "gmatrix.h"
#include "matter.h"
#include "world.h"
#include "usermsg.h"
#include "clstab.h"
#include <common/protocol.h>
#include "npcgenerator.h"
#include "pathfinding/pathfinding.h"
#include "template_loader.h"


DEFINE_SUBSTANCE(gmatter_imp,gobject_imp,CLS_MATTER_IMP)
DEFINE_SUBSTANCE(gmatter_dispatcher,dispatcher,CLS_MATTER_DISPATCHER)
DEFINE_SUBSTANCE(gmatter_controller,controller,CLS_MATTER_CONTROLLER)
DEFINE_SUBSTANCE_ABSTRACT(gmatter_item_base_imp,gmatter_imp,CLS_MATTER_ITEM_BASE_IMP)
DEFINE_SUBSTANCE(gmatter_money_imp,gmatter_item_base_imp,CLS_MATTER_MONEY_IMP)
DEFINE_SUBSTANCE(gmatter_mine_imp,gmatter_imp,CLS_MATTER_MINE_IMP)
DEFINE_SUBSTANCE(gmatter_dyn_imp,gmatter_imp,CLS_MATTER_DYN_IMP)
DEFINE_SUBSTANCE(gmatter_combine_mine_imp,gmatter_mine_imp,CLS_MATTER_COMBINE_MINE_IMP)

void gmatter_imp::ActiveCollision(bool active)
{
	if(active)
	{
		if(!_collision_actived)
		{
			_collision_actived = true;
			if(_parent->collision_id >0) GetWorldManager()->GetTraceMan().EnableElement(_parent->collision_id, true);
		}
	}
	else
	{
		if(_collision_actived)
		{
			_collision_actived = false;
			if(_parent->collision_id >0) GetWorldManager()->GetTraceMan().EnableElement(_parent->collision_id, false);
		}
	}
}

void gmatter_dispatcher::enter_world()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gmatter * pMatter = (gmatter*)_imp->_parent;
	CMD::Make<CMD::matter_enter_world>::From(h1,pMatter);
	slice * pPiece = pMatter->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void gmatter_dispatcher::disappear(char at_once)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gmatter * pObj = (gmatter*)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(h1,pObj,at_once);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void gmatter_dispatcher::matter_pickup(int id)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gmatter * pObj = (gmatter*)_imp->_parent;
	CMD::Make<CMD::matter_pickup>::From(h1,pObj,id);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

// Youshuang add
void gmatter_dispatcher::notify_combine_mine_state( int state )
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gmatter * pObj = (gmatter*)_imp->_parent;
	CMD::Make<CMD::combine_mine_state_change>::From(h1,pObj,state);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}
// end

void gmatter_controller::Release(bool free_parent)
{
	gmatter *pMatter = (gmatter *)_imp->_parent;
	world *pPlane = _imp->_plane;
	if(pMatter->pPiece) pPlane->RemoveMatter(pMatter);

	delete _imp->_runner;
	delete _imp;
	delete this;
	pMatter->Clear();
	pPlane->FreeMatter(pMatter);
}

/**
	物品基类的处理
*/
void
gmatter_item_base_imp::Init(world * pPlane,gobject*parent)
{
	gobject_imp::Init(pPlane,parent);
}

gmatter_item_base_imp::~gmatter_item_base_imp()
{
}

int 
gmatter_item_base_imp::MessageHandler(const MSG & msg)
{
	//
	switch(msg.message)
	{
		case GM_MSG_HEARTBEAT:
			if( (_life -= MATTER_HEARTBEAT_SEC) <=0)
			{
				//物品消失
				OnRelease();
				_runner->disappear();
				_commander->Release();
			}
			else
			{
				if(_owner_time >= 0)
				{
					if((_owner_time -=MATTER_HEARTBEAT_SEC) <=0)
					{
						_owner = 0;
						_team_owner = 0;
					}
				}
			}
			return 0;
		break;

		default:
		return gobject_imp::MessageHandler(msg);
	}
}

 

/**
 *	钱的消息处理,能够正确地处理捡取得消息
 */

void 
gmatter_money_imp::OnPickup(const XID & who,int team_id, bool is_team)
{
	MSG  msg;

	/*
	if(team_id > 0)
	{
		BuildMessage(msg,GM_MSG_PICKUP_TEAM_MONEY,XID(GM_TYPE_PLAYER,team_id),_parent->ID,
				_parent->pos,_money,&_drop_user,sizeof(_drop_user));
	}
	else
	{
		BuildMessage(msg,GM_MSG_PICKUP_MONEY,who,_parent->ID,
				_parent->pos,_money,&_drop_user,sizeof(_drop_user));
	}
	*/

	BuildMessage(msg,GM_MSG_PICKUP_MONEY,who,_parent->ID,
			_parent->pos,_money,&_drop_user,sizeof(_drop_user));
	gmatrix::SendMessage(msg);
}

int 
gmatter_money_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_PICKUP:
			if(msg.content_length == sizeof(msg_pickup_t))
			{
				msg_pickup_t * mpt = (msg_pickup_t*)msg.content;
				Pickup<0>(msg.source,msg.param, mpt->team_id, mpt->team_seq,msg.pos, mpt->who,true);
			}
			else
			{
				ASSERT(false);
			}
			return 0;
		case GM_MSG_FORCE_PICKUP:
			if(msg.content_length == sizeof(XID))
			{
				Pickup<0>(msg.source,0, msg.param,0,msg.pos,*(XID*)msg.content,false);
			}
			return 0;
		default:
			return gmatter_item_base_imp::MessageHandler(msg);
	}
}


void gmatter_mine_imp::BeMined()
{
	ActiveCollision(false);		//里面会检查是否重复调用
	if(_spawner)
	{
		if(_spawner->Reclaim(_plane,(gmatter*)_parent,this))
		{
			//若返回false 则本对象已经在Reclaim内释放
		}
	}
	else
	{
		_commander->Release();
	}
}


void 
gmatter_mine_imp::SetMonsterParam(void * buf, size_t count)
{
	if(count < 4) return ;
	memcpy(produce_monster,buf,sizeof(produce_monster));
}

int gmatter_mine_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_HEARTBEAT:
			ActiveCollision(true);		//里面会检查是否重复调用
			if(_lock)
			{
				//有锁定
				if((_lock_time_out -= MATTER_HEARTBEAT_SEC) <= 0)
				{
					_lock = false;
					_lock_id = 0;
					__PRINTF("采集超时\n");
				}
			}
			else if(_life >0)
			{
				if( (_life -= MATTER_HEARTBEAT_SEC) <=0)
				{
					ActiveCollision(false);
					_runner->disappear();
					_commander->Release();
				}
			}
			return 0;

		
		case GM_MSG_GATHER_REQUEST:
			{
				ASSERT(msg.content_length == sizeof(int)*6);
				int level = *(((int*)msg.content) +0);
				int tool = *(((int*)msg.content) +1); 
				int task_id = *(((int*)msg.content) +2);
				int role_id = *(((int*)msg.content) +3);
				int team_id = *(((int*)msg.content) +4);
				int master_id = *(((int*)msg.content) +5);
				if(_lock) // && _lock_id != msg.source.id)
				{
					//已经被锁定,返回错误
					//但如果发来消息的人就是锁定的自己，那么可以进行此操作
					//即使是自己也返回错误
					SendErrMessage<0>(msg.source,S2C::ERR_MINE_HAS_BEEN_LOCKED);
					return 0;
				}

				if(tool != _produce_tool)
				{
					//工具不正确,返回错误
					SendErrMessage<0>(msg.source,S2C::ERR_MINE_HAS_INVALID_TOOL);
					return 0;
				}
				
				if(level < _level)
				{
					//级别不够
					SendErrMessage<0>(msg.source,S2C::ERR_LEVEL_NOT_MATCH);
					return 0;
				}

				if(_task_id != task_id)
				{
					//任务不匹配返回错误
					SendErrMessage<0>(msg.source,S2C::ERR_MINE_HAS_INVALID_TOOL);
					return 0;
				}
				if(!IsMineOwner(role_id, team_id, master_id))
				{
					SendErrMessage<0>(msg.source,S2C::ERR_MINE_NOT_OWNER);
					return 0;
				}

				if(msg.pos.squared_distance(_parent->pos) >= GATHER_RANGE_LIMIT*GATHER_RANGE_LIMIT)
				{
					//工具不正确,返回错误
					SendErrMessage<0>(msg.source,S2C::ERR_OUT_OF_RANGE);
					return 0;
				}

				_lock = true;
				//是否考虑采集时间为0则不进行锁定操作?
				_lock_time_out = _gather_time + 15;
				_lock_id = msg.source.id;
				//发送数据
				{
					gather_reply rpy;
					rpy.can_be_interrupted = _can_be_interrupted;
					if(_eliminate_tool)
					{
						rpy.eliminate_tool = _produce_tool;
					}
					else
					{
						rpy.eliminate_tool = -1;
					}
					PrepareMine(msg);

					MSG message;
					BuildMessage(message,GM_MSG_GATHER_REPLY,msg.source,_parent->ID,_parent->pos,_gather_time,&rpy,sizeof(rpy));
					gmatrix::SendMessage(message);
				}
				if(_broadcast_aggro)
				{
					MSG message;
					msg_aggro_alarm_t  alarm;
					alarm.attacker	= msg.source;
					alarm.rage	= _aggro_count;
					alarm.faction	= msg.param;
					alarm.target_faction = _ask_help_faction;
					BuildMessage(message,GM_MSG_AGGRO_ALARM,XID(GM_TYPE_NPC,-1),msg.source,_parent->pos,0,&alarm,sizeof(alarm));
					_plane->BroadcastMessage(message,_aggro_range, 0xFFFFFFFF);
				}
			}
			return 0;

		case GM_MSG_GATHER_CANCEL:
			if(_lock && _lock_id == msg.source.id)
			{
				_lock = false;
				_lock_id = 0;
			}
			return 0;

		case GM_MSG_GATHER:
			if(_lock && _lock_id == msg.source.id)
			{
				gather_result data;
				data.amount = _produce_amount;
				data.task_id = _produce_task_id;
				data.eliminate_tool = 0;
				if(_eliminate_tool)
				{
					data.eliminate_tool = _produce_tool;
				}

				//返回数据
				MSG message;
				BuildMessage(message,GM_MSG_GATHER_RESULT,msg.source,_parent->ID,_parent->pos,
						_produce_id,&data,sizeof(data));
				gmatrix::SendMessage(message);

				//给点经验
				if(_exp)
				{
					msg_exp_t expdata = {_level,_exp, true};
					message.message = GM_MSG_EXPERIENCE;
					message.param = 0;
					message.content = & expdata;
					message.content_length = sizeof(expdata);
					gmatrix::SendMessage(message);
				}

				//试着创建怪物
				object_interface::minor_param param;
				memset(&param,0,sizeof(param));
				param.exp_factor = 1.0f;
				param.drop_rate = 1.f;
				param.money_scale = 1.f;
				param.spec_leader_id = XID(-1,-1);
				param.parent_is_leader = false;
				param.use_parent_faction = false;
				param.die_with_leader = false;
				for(size_t i =0; i < 4; i ++)
				{
					if(!produce_monster[i].mob_id || produce_monster[i].num <=0) continue;
					float radius = produce_monster[i].radius;
					param.mob_id = produce_monster[i].mob_id;
					param.remain_time = produce_monster[i].remain_time;
					for(int j= 0; j < produce_monster[i].num;j ++)
					{
						int n = 0;
						A3DVECTOR pos;
						do 
						{
							pos = _parent->pos;
							pos.x += abase::Rand(-radius,radius);
							pos.z += abase::Rand(-radius,radius);
							if(path_finding::GetValidPos(GetWorldManager()->GetMoveMap(), pos))
							{
								break;
							}
							else
							{
								pos.y = _parent->pos.y;
							} 
						} while((n++) < 3);
						object_interface::CreateMob(_plane,pos,param);
					}
					
				}


				//试着创建和取消控制器
				if(ctrl_info.actived_all)
				{
					for(size_t i = 0; i < ctrl_info.active_count; i ++)
					{
						world_manager::ActiveSpawn(GetWorldManager(), ctrl_info.active_cond[i], true);
					}
				}
				else
				{
					if(ctrl_info.active_count)
					{
						int i = abase::Rand(0, ctrl_info.active_count - 1);
						world_manager::ActiveSpawn(GetWorldManager(), ctrl_info.active_cond[i], true);
					}
				}

				if(ctrl_info.deactived_all)
				{
					for(size_t i = 0; i < ctrl_info.deactive_count; i ++)
					{
						world_manager::ActiveSpawn(GetWorldManager(), ctrl_info.deactive_cond[i], false);
					}
				}
				else
				{
					if(ctrl_info.deactive_count)
					{
						int i = abase::Rand(0, ctrl_info.deactive_count - 1);
						world_manager::ActiveSpawn(GetWorldManager(), ctrl_info.deactive_cond[i], false);
					}
				}
				
				//让自己消失
				//现在先不管
				OnMined();
				if(!_gather_no_disappear)
				{
					_runner->disappear();
					BeMined();
				}
				else
				{
					_lock = false;
				}
			}
			else
			{
				//返回错误
				SendErrMessage<0>(msg.source,S2C::ERR_MINE_HAS_BEEN_LOCKED);
			}
			return 0;

		case GM_MSG_SPAWN_DISAPPEAR:
		{
			ActiveCollision(false);
			_runner->disappear();
			_commander->Release();
		}
		return 0;

		case GM_MSG_MINE_DISAPPEAR:
		{
			ActiveCollision(false);
			_runner->disappear();
			_commander->Release();
		}
		return 0;
		
		default:
			return gobject_imp::MessageHandler(msg);
	}
	return 0;
}

void gmatter_mine_imp::Reborn()
{
	_lock = false;
	_lock_id = 0;
	_runner->enter_world();
}

bool gmatter_mine_imp::IsMineOwner(int roleid, int teamid, int masterid)
{
	return ((gmatter*)_parent)->IsMineOwner(roleid, teamid, masterid);
}

int gmatter_dyn_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_HEARTBEAT:
			ActiveCollision(true);		//里面会检查是否重复调用
			return 0;

		case GM_MSG_SPAWN_DISAPPEAR:
		{
			ActiveCollision(false);		//里面会检查是否重复调用
			_runner->disappear();
			_commander->Release();
		}
		return 0;
		
		default:
			return gobject_imp::MessageHandler(msg);
	}
	return 0;
}

void gmatter_dyn_imp::Reborn()
{
	_runner->enter_world();
}

// Youshuang add
void gmatter_mine_imp::InitTemplate( mine_template* pTemplate )
{
	mine_spawner::GenerateMineParam( this, pTemplate );
}

void gmatter_mine_imp::MineEnterWorld()
{
	_runner->enter_world();
}

gmatter_combine_mine_imp::gmatter_combine_mine_imp() : state(0), result_tid(0)
{
}

void gmatter_combine_mine_imp::Init(world * pPlane,gobject*parent)
{
	gmatter_imp::Init( pPlane, parent );
	gmatter * pMatter = (gmatter *) parent; 
	pMatter->matter_state |= gmatter::STATE_MASK_COMBINE_MINE;
} 

void gmatter_combine_mine_imp::Reborn()
{
	MineEnterWorld();
}

void gmatter_combine_mine_imp::MineEnterWorld()
{
	_runner->enter_world();
	_runner->notify_combine_mine_state( state );
}

void gmatter_combine_mine_imp::NotifyMineState()
{
	_runner->notify_combine_mine_state( state );
}

void gmatter_combine_mine_imp::OnMined()
{
	if( gather_count == 0 )
	{
		state = result_tid;
	}
	else if( gather_count < 0 )
	{
		_gather_no_disappear = false;
		return;
	}
	GenCombineMineData();
	--gather_count;
	_gather_no_disappear = true;
	_runner->notify_combine_mine_state( state );
}

void gmatter_combine_mine_imp::GenCombineMineData()
{
	if( state != result_tid )
	{
		int sz = tids.size() - 1;
		int tmp = abase::Rand( 0, sz );
		state = tids[tmp];
	}
	gmatter * pMatter = (gmatter *) _parent;
	pMatter->combine_state = state;
	mine_template * pTemplate2 = mine_stubs_manager::Get( state );
	if( !pTemplate2 )
	{
		GLog::log(GLOG_ERR,"Invalid mine template id %d", state);
		return;
	}
	mine_spawner::GenerateMineParam( ( gmatter_mine_imp* )this, pTemplate2 );
}
	
void gmatter_combine_mine_imp::InitTemplate( mine_template* pTemplate )
{
	gather_count = pTemplate->gather_count;
	tids = pTemplate->combine_tid;
	result_tid = pTemplate->result_tid;
	GenCombineMineData();
}
// end

void DropMoneyItem(world * pPlane, const A3DVECTOR & pos, size_t amount,const XID &owner,int owner_team, int seq,int drop_id)
{
	gmatter * matter = pPlane->AllocMatter();
	if(matter == NULL) return ;
	matter->SetActive();
	matter->pos = pos;
	matter->ID.type = GM_TYPE_MATTER;
	matter->ID.id= MERGE_ID<gmatter>(MKOBJID(pPlane->GetWorldIndex(),pPlane->GetMatterIndex(matter)));
	matter->dir = abase::Rand(0,255);
	gmatter_money_imp *imp = new gmatter_money_imp(amount);
	imp->SetOwner(owner,owner_team,seq);
	imp->SetDrop(drop_id);
	imp->Init(pPlane,matter);
	matter->imp = imp;
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_controller();
	imp->_commander->Init(imp);
	
	pPlane->InsertMatter(matter);
	imp->_runner->enter_world();
	matter->Unlock();
}

