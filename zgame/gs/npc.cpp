#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include <arandomgen.h>
#include "gmatrix.h"
#include "npc.h"
#include "world.h"
#include "usermsg.h"
#include "clstab.h"
#include "ainpc.h"
#include "actsession.h"
#include "npcgenerator.h"
#include "item.h"
#include "playertemplate.h"
#include "npc_filter.h"
#include "pathfinding/pathfinding.h"
#include "global_drop.h"
#include "aipolicy.h"
#include "antiwallow.h"
#include "faction.h"
#include "servicenpc.h"

DEFINE_SUBSTANCE(gnpc_imp,gobject_imp,CLS_NPC_IMP)
DEFINE_SUBSTANCE(gnpc_controller,controller,CLS_NPC_CONTROLLER)
DEFINE_SUBSTANCE(gnpc_dispatcher,dispatcher,CLS_NPC_DISPATCHER)


void 
gnpc_dispatcher::enter_slice(slice *pPiece ,const A3DVECTOR &pos)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	CMD::Make<CMD::npc_enter_slice>::From(h1,(gnpc*)_imp->_parent,pos);
	
	//这里应该将自己的当前行动目标也告知
	send_msg_to_slice(pPiece,h1.data(),h1.size());
}

void 
gnpc_dispatcher::leave_slice(slice *pPiece ,const A3DVECTOR &pos)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	CMD::Make<CMD::leave_slice>::From(h1,_imp->_parent);
	send_msg_to_slice(pPiece,h1.data(),h1.size());
}

void
gnpc_dispatcher::enter_world()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	slice *pPiece = pObj->pPiece;
	CMD::Make<CMD::npc_enter_world>::From(h1,(gnpc*)_imp->_parent);
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1.data(),h1.size(),-1);
}

void
gnpc_dispatcher::move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::object_move>::From(h1,pObj,target,cost_time,speed,move_mode);
//	__PRINTF("npc move:(%f %f %f)\n",target.x,target.y,target.z);

	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void
gnpc_dispatcher::stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mode)
{
	if(_imp->_parent->IsZombie()) 
	{
		//如果死亡了就不必发送了
		return ;
	}

	if(((gnpc_imp*)_imp)->_direction.squared_magnitude() > 1e-3)
	{
		((gactive_imp*)_imp)->RecalcDirection();
	}
	else
	{
//		__PRINTF("最后一次移动的比例过小\n");
	}

	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj = _imp->_parent;
//	CMD::Make<CMD::npc_stop_move>::From(h1,pObj,speed,pObj->dir,move_mode);
	CMD::Make<CMD::object_stop_move>::From(h1,pObj,target,speed,pObj->dir,move_mode);

	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1,-1);
}

void            
gnpc_dispatcher::start_attack(const XID & target)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	gplayer * pObj = (gplayer*)pImp->_parent;
	CMD::Make<CMD::object_start_attack>::From(h1,pObj,target.id,pImp->GetAttackStamp());
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1.data(),h1.size(),-1);
}

void 
gnpc_dispatcher::on_death(const XID & attacker,bool is_delay, int time)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	if(is_delay)
	{
		CMD::Make<CMD::npc_dead_2>::From(h1,pObj,attacker);
	}
	else
	{
		CMD::Make<CMD::npc_dead>::From(h1,pObj,attacker);
	}
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1,-1);
}

void 
gnpc_dispatcher::dodge_attack(const XID &attacker, int skill_id, const attacker_info_t& info, char at_state,char stamp ,bool orange)
{
	// Youshuang add
	gnpc_imp* npc_imp = dynamic_cast<gnpc_imp*>( _imp );
	if( npc_imp && !npc_imp->_show_damage ){ return; }
	// end
	
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject *pObj = _imp->_parent;
	if(skill_id)
	{
		CMD::Make<CMD::object_skill_attack_result>::From(h1,attacker,pObj->ID,skill_id,0,0,at_state, stamp); 
		AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
	}
	else
	{
		CMD::Make<CMD::object_attack_result>::From(h1,attacker,pObj->ID,0,at_state , stamp);
		AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
	}

}

void 
gnpc_dispatcher::be_hurt(const XID & id, const attacker_info_t & info,int damage,bool invader)
{
	// Youshuang add
	gnpc_imp* npc_imp = dynamic_cast<gnpc_imp*>( _imp );
	if( npc_imp && !npc_imp->_show_damage ){ return; }
	// end
	
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
	{
		CMD::Make<CMD::hurt_result>::From(h1,pObj->ID,damage);
		send_ls_msg(info.cs_index,id.id,info.sid,h1.data(),h1.size());
		h1.clear();
	}
}
	
void
gnpc_dispatcher::be_damaged(const XID & id, int skill_id, const attacker_info_t & info,int damage,int dt_damage,int,char at_state,char stamp,bool orange)
{
	// Youshuang add
	gnpc_imp* npc_imp = dynamic_cast<gnpc_imp*>( _imp );
	if( npc_imp && !npc_imp->_show_damage ){ return; }
	// end
	
	packet_wrapper  h1(64);
	using namespace S2C;
//	__PRINTF (" be damaged\n");
	gobject * pObj = _imp->_parent;
	if(skill_id)
	{
		CMD::Make<CMD::object_skill_attack_result>::From(h1,id,pObj->ID,skill_id,damage,dt_damage,at_state , stamp);
		AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
		
	}
	else
	{
		CMD::Make<CMD::object_attack_result>::From(h1,id,pObj->ID,damage,at_state ,stamp);
		AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
	}
}

void
gnpc_dispatcher::cast_skill(int target_cnt, const XID* targets, int skill,unsigned short time, unsigned char level, unsigned char state, short cast_speed_rate, const A3DVECTOR& pos, char spirit_index)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gobject * pObj= (gobject*)pImp->_parent;
	CMD::Make<CMD::object_cast_skill>::From(h1,pObj->ID,target_cnt,targets,skill,time,level,pImp->GetAttackStamp(), 0, cast_speed_rate, pos, -1);
	AutoBroadcastCSMsg(pImp->_plane,pObj->pPiece,h1,-1);
}

void
gnpc_dispatcher::skill_interrupt(char reason)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gobject * pObj= (gobject*)pImp->_parent;
	CMD::Make<CMD::skill_interrupted>::From(h1,pObj->ID, -1);
	AutoBroadcastCSMsg(pImp->_plane,pObj->pPiece,h1,-1);
}

void
gnpc_dispatcher::notify_root(unsigned char type)
{
	if(type & 0x80) return;
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::notify_root>::From(h1,pObject);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1,-1);
}


void
gnpc_dispatcher::takeoff()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_takeoff>::From(h1,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
}

void 
gnpc_dispatcher::landing()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_landing>::From(h1,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
}

void 
gnpc_dispatcher::disappear(char at_once)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(h1,pObj, at_once);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::query_info00(const XID & target, int cs_index,int sid)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	//_back_up[1];
	CMD::Make<CMD::npc_info_00>::From(h1,pImp->_parent->ID,pImp->_basic.hp,pImp->_basic,pImp->_cur_prop);
	send_ls_msg(cs_index,target.id,sid,h1);
}

void 
gnpc_dispatcher::query_info_1(int uid,int cs_index, int cs_sid)
{
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<multi_data_header>::From(h1,NPC_INFO_LIST,1);
	CMD::Make<INFO::npc_info>::From(h1,(gnpc*)_imp->_parent);
	send_ls_msg(cs_index,uid,cs_sid,h1);
}

void 
gnpc_dispatcher::send_turrent_leader(int id)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::turret_leader_notify>::From(h1,pObj->ID, pObj->tid, id);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::level_up()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::level_up>::From(h1,pObj);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::enter_dim_state(bool state)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::enter_dim_state>::From(h1,pObj, state);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}
void gnpc_dispatcher::dir_visible_state(int id, int dir, bool on)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::dir_visible_state>::From(h1, pObj->ID.id, id, dir, on); 
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void gnpc_dispatcher::npc_invisible(bool on)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::npc_invisible>::From(h1, pObj->ID.id, on); 
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void gnpc_dispatcher::teleport_skill_info(int leader_id, int npc_tid, int skill_id, int teleport_count)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::teleport_skill_info>::From(h1, leader_id, npc_tid, skill_id, teleport_count); 
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void gnpc_dispatcher::player_mobactive_state_cancel(const XID& playerid,const XID & mobactive, int pos, const A3DVECTOR & player_pos, const A3DVECTOR & mobactive_pos)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::mob_active_cancel>::From(h1,playerid, mobactive, pos, player_pos, mobactive_pos);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1); 
}

gnpc_controller::gnpc_controller()
	:_svr_belong(-1),_extern_svr(-1),_ai_core(0),_cry_for_help_timestamp(0),_ignore_range(70.f)
{
}
int
gnpc_controller::CommandHandler(int cmd_type,const void * buf, size_t size)
{
	ASSERT(false && "普通NPC 怎么会接受到消息呢，赫赫");
	return 0;
}

bool 
gnpc_controller::Save(archive & ar)
{
	ar << _svr_belong ;
	//ar << _cur_target;
	ar << _ignore_range;
	return _ai_core->Save(ar);
}

bool 
gnpc_controller::Load(archive & ar)
{
	ar >> _svr_belong ;
	//ar >> _cur_target;
	ar >> _ignore_range;
	_extern_svr = -1;
	ASSERT(_ai_core == NULL);

	_ai_core = new gnpc_ai();
	return _ai_core->Load(ar);
}

int 
gnpc_controller::GetFactionAskHelp()
{
	if(_ai_core) 
		return _ai_core->GetFactionAskHelp();
	else
		return 0;
}

void 
gnpc_controller::SetLifeTime(int life)
{
	if(_ai_core) _ai_core->SetLife(life);
}

void 
gnpc_controller::SetDieWithLeader(bool val)
{
	if(_ai_core) _ai_core->SetDieWithLeader(val);
}

int 
gnpc_controller::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_WATCHING_YOU:
			if(msg.content_length == sizeof(msg_watching_t))
			{
				msg_watching_t * pMsg = (msg_watching_t *)msg.content;

				gnpc * pNPC = (gnpc*)_imp->_parent;
				if(pNPC->anti_invisible_rate < pMsg->invisible_rate) return 0;
				if((pMsg->faction & ((gactive_imp*)_imp)->GetEnemyFaction()) && _ai_core)
				{
					_ai_core->AggroWatch(msg);
				}
			}
			else
			{
				ASSERT(false);
			}
			return 0;
		break;
		case GM_MSG_GEN_AGGRO:
			if(_ai_core) _ai_core->AggroGen(msg);
			return 0;
		break;
		case GM_MSG_TRANSFER_AGGRO:
			if(_ai_core) _ai_core->AggroTransfer(msg);
			return 0;
		break;
		case GM_MSG_AGGRO_ALARM:
			if(_ai_core) _ai_core->AggroAlarm(msg);
			return 0;
		break;
		case GM_MSG_AGGRO_WAKEUP:
			if(_ai_core) _ai_core->AggroWakeUp(msg);
			return 0;
		break;
		case GM_MSG_AGGRO_TEST:
			if(_ai_core) _ai_core->AggroTest(msg);
			return 0;
		case GM_MSG_NPC_CRY_FOR_HELP:
			if(msg.content_length == sizeof(msg_cry_for_help_t)) 
			{
				msg_cry_for_help_t & mcht = *(msg_cry_for_help_t*)msg.content;
				if(mcht.helper_faction & ((gnpc*)(_imp->_parent))->monster_faction)
				{
					if(_ai_core) _ai_core->AggroHelp(mcht.attacker,mcht.lamb_faction);
				}
			}
			else
			{
				ASSERT(false);
			}
		return 0;

		case GM_MSG_PLAYER_KILLED_BY_NPC:
			if(_ai_core) _ai_core->KillTarget(msg.target);
		break;

		case GM_MSG_BECOME_TURRET_MASTER:
		{
			ASSERT(msg.content_length == sizeof(int));
			int tid  = msg.param;
			gnpc_imp * pImp = (gnpc_imp*)_imp;
			gnpc * pParent = (gnpc*)(pImp->_parent);
			int faction = *(int*)msg.content;
			if(pParent->pos.squared_distance(msg.pos) < 10.f*10.f &&
				((faction & FACTION_BATTLEOFFENSE) && (pImp->GetFaction() & FACTION_OFFENSE_FRIEND) ||
				 (faction & FACTION_BATTLEDEFENCE) && (pImp->GetFaction() & FACTION_DEFENCE_FRIEND)))
			{
				if(_ai_core)
				{
					if(_ai_core->ChangeTurretMaster(msg.source))
					{
						pImp->SendTo<0>(GM_MSG_REMOVE_ITEM,msg.source,tid);
					}
				}
			}
		}
		break;

		case GM_MSG_TRY_CLEAR_AGGRO:
		{
			gnpc * pNPC = (gnpc*)_imp->_parent;
			if(pNPC->anti_invisible_rate >= msg.param) break;
			if(_ai_core) _ai_core->AggroClear(msg.source);			
		}
		break;



	}
	return 0;
}

void 
gnpc_controller::AddAggro(const XID & who, int rage)
{
	if(_ai_core) _ai_core->AggroGen(who,rage);
}

void 
gnpc_controller::BeTaunted(const XID & who, int rage)
{
	if(_ai_core) _ai_core->BeTaunted(who,rage);
}

int 
gnpc_controller::MoveBetweenSlice(gobject *obj, slice * src, slice * dest)
{
	ASSERT(obj == _imp->_parent);
	return src->MoveNPC((gnpc *)obj,dest);
}

gnpc_controller::~gnpc_controller()
{
	if(_ai_core) ReleaseAI();
}

void 
gnpc_controller::Release(bool free_parent)
{
	gnpc *pNPC = GetParent();
	gactive_imp * pImp = (gactive_imp*)_imp;
	pImp->ResetSession();

	world *pPlane = _imp->_plane;
	if(pNPC->pPiece) pPlane->RemoveNPC(pNPC);
	ReleaseAI();
	dispatcher * runner = _imp->_runner;
	delete _imp;
	delete runner;
	delete this;
	pNPC->imp = NULL;
	if(free_parent) pPlane->FreeNPC(pNPC);
}


gnpc_imp::gnpc_imp():_native_notify(0),_npc_state(0),_dmg_list(20),_spawner(NULL),_money_scale(1.0f),_drop_rate(1.0f),_fast_regen(1),_regen_spawn(0),_drop_no_protected(0)
{
	_faction = 0;
	_enemy_faction = 0;
	_birth_place.y = -1e8;
	_leader_id = XID(-1,0);
	_first_attacker = XID(-1,-1);
	_corpse_delay = 20;
	_knocked_back = 0;
	_dead_time_count = 0;
	_collision_actived = false;
	_fix_drop_rate = false;
	_skill_drop_adjust = 0.f;
	_no_exp_punish = false;
	_player_cannot_attack = false;
	_disappear_life_expire = false;
	_init_path_id = 0;
	_init_path_type = 0;
	_share_npc_kill = false;
	_share_drop = false;
	_buffarea_tid = 0;
	_is_fly = false;
	_show_damage = true;
}

gnpc_imp::~gnpc_imp()
{
	if(_native_notify) delete _native_notify;
}

void 
gnpc_imp::SetFastRegen(bool b)
{
        _fast_regen = b?1:0;
	gnpc_controller * pCmd = (gnpc_controller*)_commander;
	pCmd->SetFastRegen(b);
}

void 
gnpc_imp::SetBattleNPC(bool b)
{
	_battle_npc = b;
}

void
gnpc_imp::SetDisappearLifeExpire(bool b)
{
	_disappear_life_expire = b;
}

bool
gnpc_imp::IsDisappearLifeExpire()
{
	return _disappear_life_expire;
}

void 
gnpc_imp::LifeExhaust()
{
	if(_collision_actived)
	{
		_collision_actived = false;
		if(_parent->collision_id >0) GetWorldManager()->GetTraceMan().EnableElement(_parent->collision_id, false);
	}

	if(_spawner)
	{
		//临时清除 订阅列表
		_buff.ClearSubscibeList(this);
		if(_spawner->Reclaim(_plane, (gnpc*)this->_parent,this,_regen_spawn))
		{
			//若返回false 则本对象应当已经被释放，这时不应当再修改npc_state
			_npc_state = NPC_STATE_SPAWNING;
		}
	}
	else
	{
		_commander->Release();
	}
}

int 
gnpc_imp::ZombieMessageHandler(const MSG & msg)
{	
	//__PRINTF("recv message in zombie state msg %d\n",msg.message);
	//暂时不处理任何消息，在死亡状态
	switch(msg.message)
	{
	case GM_MSG_OBJ_ZOMBIE_END:
		//如果有尸体残留时间才发送尸体消失信息
		if(_corpse_delay)
		{
			_runner->disappear();
		}
		LifeExhaust();
		break;
	case GM_MSG_HEARTBEAT:
		DoHeartbeat(msg.param);
		break;
	case GM_MSG_QUERY_OBJ_INFO00:
		MH_query_info00(msg);
		break;

	case GM_MSG_ENCHANT_ZOMBIE:
		{
			__PRINTF("recv zombie enchant\n");
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}

	case GM_MSG_SUBSCIBE_TARGET:
	case GM_MSG_UNSUBSCIBE_TARGET:
		return MessageHandler(msg);

	default:
		break;
	}
	return 0;
}

int 
gnpc_imp::DispatchMessage(const MSG & msg)
{
	switch(_npc_state)
	{
		case NPC_STATE_NORMAL:
		case NPC_STATE_WAITING_SWITCH:
		if(_parent->IsZombie())
			return ZombieMessageHandler(msg); 
		else
			return MessageHandler(msg);
		case NPC_STATE_SPAWNING:
		return 0;

		default:
		ASSERT(false);
		return 0;
	}
}

int 
gnpc_imp::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_WATCHING_YOU:
		case GM_MSG_GEN_AGGRO:
		case GM_MSG_TRANSFER_AGGRO:
		case GM_MSG_AGGRO_ALARM:
		case GM_MSG_AGGRO_WAKEUP:
		case GM_MSG_AGGRO_TEST:
		case GM_MSG_NPC_CRY_FOR_HELP:
		case GM_MSG_PLAYER_KILLED_BY_NPC:
		case GM_MSG_BECOME_TURRET_MASTER:
		case GM_MSG_TRY_CLEAR_AGGRO:
		//这里把有关于仇恨的消息转发到controller里去
		return _commander->MessageHandler(msg);

		return 0;

		case GM_MSG_HATE_YOU:
		return 0;

		case GM_MSG_ATTACK:
			{
				ASSERT(msg.content_length >= sizeof(attack_msg));
				//这里由于都要进行复制操作，有一定的耗费存在
				attack_msg ack_msg = *(attack_msg*)msg.content;
				if(_player_cannot_attack && ack_msg.ainfo.attacker.IsPlayerClass()) return 0;
				//if(msg.source.IsPlayerClass() && ack_msg.is_flying) return 0;
				if(msg.source.IsPlayerClass() && !_is_fly && ack_msg.is_flying) return 0;  // Youshuang add
				if(!msg.source.IsPlayerClass())
				{
					if(!ack_msg.force_attack && !(GetFaction() & ack_msg.target_faction))
					{
						return 0;
					}
				}
					
				//对怪物无效
				if(!IsNPCOwner(ack_msg.ainfo.attacker.id, ack_msg.ainfo.team_id, ack_msg.ainfo.master_id)) return 0;

				_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
				HandleAttackMsg(msg,&ack_msg);
				return 0;
			}
		case GM_MSG_ENCHANT:
			{
				//__PRINTF("recv enchant\n");
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				//这里由于都要进行复制操作，有一定的耗费存在
				if(_player_cannot_attack && ech_msg.ainfo.attacker.IsPlayerClass()) return 0;
				//if(msg.source.IsPlayerClass() && ech_msg.is_flying) return 0;
				if(msg.source.IsPlayerClass() && !_is_fly && ech_msg.is_flying) return 0;  // Youshuang add
				if(!msg.source.IsPlayerClass())
				{
					if(!ech_msg.force_attack && !(GetFaction() & ech_msg.target_faction))
					{
						return 0;
					}
				}
				if(!IsNPCOwner(ech_msg.ainfo.attacker.id, ech_msg.ainfo.team_id, ech_msg.ainfo.master_id)) return 0;

				_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
				HandleEnchantMsg(msg,&ech_msg);
				return 0;
			}
		case GM_MSG_SPAWN_DISAPPEAR:
			{
				if(_buffarea_tid > 0)
				{
					GetWorldManager()->DeactiveBuffArea(_buffarea_tid);
				}
				gnpc * pNPC = (gnpc*)_parent;
				_runner->disappear();
				pNPC->b_zombie = true;
				if(_collision_actived)
				{
					_collision_actived = false;
					if(_parent->collision_id >0) GetWorldManager()->GetTraceMan().EnableElement(_parent->collision_id, false);
				}
				_commander->Release();
			}
			return 0;

		case GM_MSG_NPC_TRANSFORM2:
		{
			if(msg.param)
			{
				gnpc_imp * __this = TransformNPC(msg.param, msg.param2);
				if(__this == NULL)
				{
					GLog::log(GLOG_ERR,"NPC在照妖镜转换的时候发生错误");
					return 0;
				}

				//发送更换状态数据
				__this->_runner->disappear(true);
				__this->_runner->enter_world();
			}
		}	
		return 0;

		case GM_MSG_MONSTER_TRANSFORM2:
		{
			if(msg.param)
			{
				gnpc_imp * __this = TransformMob(msg.param, msg.param2);
				if(__this == NULL)
				{
					GLog::log(GLOG_ERR,"NPC在照妖镜转换的时候发生错误");
					return 0;
				}

				//发送更换状态数据
				__this->_runner->disappear(true);
				__this->_runner->enter_world();
			}
		}
		return 0;

		case GM_MSG_MINE_TRANSFORM2:
		{
			if(msg.param)
			{
				TransformMine(msg.param, msg.param2);
				
			}
		}
		return 0;

		case GM_MSG_BATTLE_NPC_DISAPPEAR:
		{
			_runner->disappear();
			_parent->b_zombie = true;
			LifeExhaust();
		}
		return 0;


		case GM_MSG_NPC_DISAPPEAR:
		{
			_runner->disappear();
			_parent->b_zombie = true;
			LifeExhaust();
		}
		return 0;

		case GM_MSG_FORBID_NPC:
		{
			int npc_id = msg.param;
			if(GetNPCID() != npc_id) return 0;

			_runner->disappear();
			_parent->b_zombie = true;
			LifeExhaust();
		}
		return 0;
		

		case GM_MSG_NPC_BE_CATCHED:
		{
			if(CatchedByPlayer())
			{
				SendTo<0>(GM_MSG_NPC_BE_CATCHED_CONFIRM, msg.source, msg.param); 
			}
		}
		return 0;

		case GM_MSG_NPC_BORN:
		{
			OnBorn();
		}
		return 0;

		case GM_MSG_EXCHANGE_STATUS:
		{
			int newhp = msg.param;
			if(newhp < _basic.hp)
			{
				if(newhp > GetMaxHP()) newhp = GetMaxHP();
				_basic.hp = newhp;
				SetRefreshState();
			}
		}
		return 0;

		case GM_MSG_EXCHANGE_POS:
		{
			A3DVECTOR destPos = msg.pos;
			float height = GetWorldManager()->GetTerrain().GetHeightAt(destPos.x,destPos.z);
			if(destPos.y < height) destPos.y = height;

			A3DVECTOR start = _parent->pos; 
			A3DVECTOR offset(destPos.x - start.x, destPos.y-start.y, destPos.z - start.z);
			StepMove(offset);
			_runner->object_charge(msg.source.id, 0, destPos);
		}	
		return 0;
		
		//处理国王战里面防守方的生命立柱被辅地图NPC死亡影响的血量
		case GM_MSG_KINGDOM_CHANGE_KEY_NPC:
		{
			if(!_battle_npc) return 0;
			if(_parent->IsZombie()) return 0;
			int type = msg.param;
			int old_hp = _basic.hp;
			int delta_hp = player_template::GetKingdomChangeHP(type);

			//为国王战的生命柱体加血
			if(type == 1)
			{
				IncHP(delta_hp);
			}
			//为国王战的生命柱体减血
			else if(type == 2)
			{
				DecHP(-delta_hp);
			}
			
			int change_hp = _basic.hp - old_hp;
			if(change_hp != 0)
			{
				msg_kingdom_update_key_npc key_npc;
				key_npc.cur_hp = _basic.hp;
				key_npc.max_hp = GetMaxHP();
				key_npc.cur_op_type = type;
				key_npc.change_hp = change_hp;
					
				MSG msg;
				BuildMessage(msg,GM_MSG_KINGDOM_UPDATE_KEY_NPC, XID(GM_TYPE_MANAGER,GetWorldTag()),_parent->ID,_parent->pos,0,&key_npc,sizeof(key_npc));
				gmatrix::SendWorldMessage(GetWorldTag(), msg);
			}
		}
		return 0;

		case GM_MSG_KINGDOM_QUERY_KEY_NPC:
		{
			if(_parent->IsZombie()) return 0;

			msg_kingdom_key_npc_info key_npc;
			key_npc.cur_hp = _basic.hp;
			key_npc.max_hp = GetMaxHP();
			
			MSG msg;
			BuildMessage(msg,GM_MSG_KINGDOM_KEY_NPC_INFO, XID(GM_TYPE_MANAGER,GetWorldTag()),_parent->ID,_parent->pos,0,&key_npc,sizeof(key_npc));
			gmatrix::SendWorldMessage(GetWorldTag(), msg);

		}
		return 0;

		case GM_MSG_KINGDOM_CHANGE_NPC_FACTION:
		{
			if(_parent->IsZombie()) return 0;

			int faction = msg.param;
			int enemy_faction = msg.param2;

			_faction = faction;
			_enemy_faction = enemy_faction;
		}
		return 0;

		case GM_MSG_REQUEST_BUFFAREA_BUFF:
		{
			int areaTid = msg.param;
			if(_buffarea_tid <= 0 || _buffarea_tid != areaTid)
			{
				GLog::log(GLOG_ERR,"NPC上没有挂buff区域， areaTid=%d, param=%d\n", _buffarea_tid, areaTid);
				return 0;
			}
			DATA_TYPE dt;
			const BUFF_AREA_CONFIG& ess = *(BUFF_AREA_CONFIG*)gmatrix::GetDataMan().get_data_ptr(areaTid, ID_SPACE_CONFIG, dt);
			if(dt != DT_BUFF_AREA_CONFIG || &ess == NULL)
			{
				__PRINTF("无效的buff区域模板ID=%d\n", areaTid);
				return 0;
			}
			if(ess.transfigure_id > 0)
			{
				int sType = GNET::SkillWrapper::GetType(ess.transfigure_id);
				if(sType == 2 && sType == 3)
				{
					enchant_msg	ent;
					memset(&ent,0,sizeof(ent));
					ent.skill        = ess.transfigure_id;
					ent.skill_level	 = 1;
					ent.force_attack = true;
					if(sType == 2)
					{
						ent.helpful  = 1;
					}
					ent.attack_range = 256;
					FillEnchantMsg(msg.source,ent);
					TranslateEnchant(msg.source,ent);
					SendEnchantMsg(GM_MSG_ENCHANT,msg.source,ent);
				}
			}

			for(size_t i = 0; i < sizeof(ess.skill_ids) / sizeof(int); i ++)
			{
				if(ess.skill_ids[i] <= 0)
				{
					continue;
				}
				int sType = GNET::SkillWrapper::GetType(ess.skill_ids[i]);
				if(sType != 2 && sType != 3)
				{
					continue;
				}
				enchant_msg	ent;
				memset(&ent,0,sizeof(ent));
				ent.skill        = ess.skill_ids[i];
				ent.skill_level	 = 1;
				ent.force_attack = true;
				if(sType == 2)
				{
					ent.helpful  = 1;
				}
				ent.attack_range = 256;
				FillEnchantMsg(msg.source,ent);
				TranslateEnchant(msg.source,ent);
				SendEnchantMsg(GM_MSG_ENCHANT,msg.source,ent);
			}	
		}
		return 0;

	default:
		return gactive_imp::MessageHandler(msg);
	}
	return 0;
}

bool 
gnpc_controller::CreateAI(const aggro_param & aggp, const ai_param & aip)
{
	if(_ai_core) {
		ASSERT(false && "原来的AI对象没有删除");
		return false;
	}
	gnpc_ai *pTmp = new gnpc_ai();
	pTmp->Init(this,aggp,aip);
	_ai_core = pTmp;
	return true;
}

void 
gnpc_controller::Init(gobject_imp * imp) 
{
	_imp = imp;
	if(_ai_core)
	{
		_ai_core->ReInit(this);
	}
}

void
gnpc_controller::ReleaseAI()
{
	if(_ai_core) delete _ai_core;
	_ai_core = NULL;
}


class gnpc_notify_native_npc : public gnpc_notify
{
	int _timeout;
public:
	virtual int MoveOutHeartbeat(gnpc_imp * impl, int tick)
	{
		_timeout -= tick;
		if(_timeout <= 0)
		{
			//自己必须消失
			return -1;
		}
		return 0;
	}
	
	virtual void OnUpdate(gnpc_imp * impl) 
	{
		//刷新一下
		_timeout = 60;
	} 
};

void 
gnpc_imp::Init(world * pPlane,gobject*parent)
{
	gactive_imp::Init(pPlane,parent);
	((gnpc*)parent)->idle_timer = NPC_IDLE_TIMER;
	_npc_state = NPC_STATE_NORMAL;
	
	//第一次初始化才更新出生地
	if(_birth_place.y < -1e3) 
	{
		_birth_place = _parent->pos;
		_birth_dir = _parent->dir;
	}

	ASSERT(_native_notify == NULL);
	_native_notify = new gnpc_notify_native_npc;
}


void 
gnpc_imp::OnDeath(const XID & attacker,bool is_invader, bool no_drop, bool is_hostile_duel, int time )
{
	if(_parent->IsZombie())
	{
		//已经是zombie了
		return ;
	}
	switch(_after_death)
	{
	case 0:
	case 2:
		//重生
		break;
	case 1:
		//自爆
		{
			int next;
			if(NPCStartSkill(SUICIDE_ATTACK_SKILL_ID,attacker,next) >= 0)
			{
				NPCEndSkill(SUICIDE_ATTACK_SKILL_ID,1,attacker);
			}
		}
		break;
	}
	//死亡的操作，是，进入zombie状态，
	_parent->b_zombie = true;
	if(_corpse_delay)
	{
		((gnpc*)_parent)->object_state |= gactive_object::STATE_STATE_CORPSE;
	}
	else
	{
		((gnpc*)_parent)->object_state &= ~gactive_object::STATE_STATE_CORPSE;
	}

	GetWorldManager()->OnMobDeath(_plane,GetFaction(), ((gnpc*)_parent)->tid,_parent->pos, attacker.id);
	if((GetWorldManager()->IsRaidWorld() || GetWorldManager()->IsBattleWorld()) && attacker.IsPlayerClass())
	{
		SendTo2<0>(GM_MSG_KILL_MONSTER_IN_BATTLEGROUND,attacker,((gnpc*)_parent)->tid, GetFaction()); 	
	}
	
	int team_id = 0;
	int team_seq = -1;
	XID owner(-1,-1);
	XID task_owner(-1,-1);
	int owner_level = 99;
	int wallow_level = 0;

	_buff.ClearBuff();

	//重新设置first_attacker
	gnpc_ai * pAI = ((gnpc_controller*)_commander)->_ai_core;
	if(pAI)
	{
		XID first;
		if(pAI->GetAggroCtrl()->GetFirst(first))
		{
			if(GM_TYPE_NPC != first.type)
			{
				_first_attacker = first;
			}
		}
	}

	DispatchExp(owner,team_id,team_seq,owner_level,task_owner, wallow_level);	//分发经验值
	//执行策略的OnDeath
	((gnpc_controller*)_commander) -> OnDeath(attacker);
	//清除当前Session 注意必须在commander的OnDeath之后调用，否则可能由于策略中排队的任务会引发新的session和任务的产生
	ClearSession();

	_native_notify->OnDeath(this);
	
	//发送死亡消息
	_runner->on_death(attacker,_corpse_delay);

	if(_share_npc_kill)
	{
		DAMAGE_MAP::iterator it = _dmg_list.begin();
		for(;it != _dmg_list.end(); ++it)
		{
			if(it->first.type == GM_TYPE_PLAYER)
			{
				int level = _basic.level;
				SendTo<0>(GM_MSG_TASK_SHARE_NPC_BE_KILLED,it->first,GetNPCID(),&level,sizeof(level));
			}
			if(_is_boss)
			{
				//玩家杀死的boss要记录日志
				GLog::action("killmon,bossid=%d:rid=%d", GetNPCID(), it->first.id);
			}
		}
	}

	{
		//目前task_owner和owner肯定是一个
		//发送被杀死的信息到特定Player
		if(task_owner.type == GM_TYPE_PLAYER) 
		{
			int level = _basic.level;
			SendTo<0>(GM_MSG_NPC_BE_KILLED,task_owner,GetNPCID(),&level,sizeof(level));

			if(_is_boss)
			{
				//玩家杀死的boss要记录日志
				GLog::action("killmon,bossid=%d:rid=%d", GetNPCID(), task_owner.id );
			}
		}
	}

	//要考虑宠物杀死的怪物要计算在玩家身上

	//开始生成金钱 只有玩家或者队伍杀死的怪物才会掉落金钱和物品
	bool is_drop = ((owner.type != GM_TYPE_NPC && owner.IsActive()) || team_id > 0);

	//如果是组队的内容，那么标记为组队内容 清除原有的owner
	if(team_id>0) owner = XID(-1,0);

	if(is_drop) 
	{
		if(_drop_no_protected)
		{
			owner = XID(-1,0);
			team_id = 0;
		}
		if(_share_drop && GetWorldManager()->IsRaidWorld())
		{
			owner = XID(-1,0);
			team_id = 0;
		}
		DropItem(owner, owner_level, team_id, team_seq, wallow_level);
	}

	ClearDamageList(); //应该在dropitem之后，因为清除了_skill_drop_rate
//-------------------------------掉落结束-----------------------------------------

	//发送消失的消息，延时发送，
	//因为现在消失的话，可能在心跳中死亡 会有很多额外的处理
	if(_corpse_delay == 0)
	{
		//如果没有死亡延迟就立刻发送消失代码否则等待心跳
		SendTo<0>(GM_MSG_OBJ_ZOMBIE_END, _parent->ID,0);
	}

	_dead_time_count = 0;
}

namespace
{
	struct TempDmgNode 
	{
		//这个类的顺序不要随意调整
		//它必须和msg_grpexp_t一致
		msg_grpexp_t content;
		TempDmgNode(const XID & id, int dmg)
		{
			content.who = id;
			content.damage = dmg;
		}
	};
	struct  TempDmgEntry
	{
		int64_t total_damage;
		abase::vector<TempDmgNode, abase::fast_alloc<> > list;
		TempDmgEntry():total_damage(0)
		{}
	};
};

void 
gnpc_imp::DispatchExp(XID & owner, int &team_id, int &team_seq, int &level, XID & task_owner, int &wallow_level)
{
	int64_t exp = _basic.exp;
	if(exp < 0) exp = 0;
	if(!_dmg_list.size()) return;
	int64_t total_damage = 0;

	player_template::AdjustGlobalExpSp(exp);

	typedef abase::hash_map<XID,TempDmgEntry,XID_HashFunc, abase::fast_alloc<> >  TempDmgMap;
	TempDmgMap dlist(_dmg_list.size());
	
	DAMAGE_MAP::iterator it = _dmg_list.begin();
	int sig_max_damage = -1;		//单体最大伤害造成的伤害值
	int team_max_damage = -1;		//最大伤害造成的总伤害
	int max_team_id = 0;			//最大伤害的team_id
	int max_team_seq = -1;			//最大伤害的team_seq
	int first_team_id = 0;			//第一攻击者所属的team_id
	int first_team_seq = -1;		//第一攻击者所属的team_seq
	int max_wallow = 0;
	for(;it != _dmg_list.end(); ++it)
	{
		int64_t damage = it->second.damage;
		if(it->first == _first_attacker)
		{
			first_team_id = it->second.team_id;
			first_team_seq = it->second.team_seq;
		}

		if(max_wallow < it->second.wallow_level)
		{
			max_wallow = it->second.wallow_level;
		}

		total_damage += damage;
		if(sig_max_damage < damage)
		{
			sig_max_damage = damage;
			owner = it->first;
		}
		if(it->second.team_id > 0)
		{
			TempDmgEntry &ent = dlist[XID(-it->second.team_id,it->second.team_seq)];
			ent.total_damage += damage;
			/*
			if(ent.list.empty())
			{
				//留出两个空位，保存经验值和sp等数据，这样可以一起传送过去
				ent.list.push_back(TempDmgNode(XID(0,0),0));
				ent.list.push_back(TempDmgNode(XID(0,0),0));
			}
			//现在用索引0的元素保存最高级别和最高级别对应的玩家
			TempDmgNode & node = ent.list[0];
			if(node.content.damage < it->second.level) 
			{
				node.content.damage = it->second.level;
				node.content.who = it->first;
			}
			*/

			ent.list.push_back(TempDmgNode(it->first,damage));

			if(team_max_damage < ent.total_damage) 
			{
				team_max_damage = ent.total_damage;
				max_team_id = it->second.team_id;
				max_team_seq = it->second.team_seq;
			}
		}
		else
		{
			dlist[it->first].total_damage += damage;
			if(team_max_damage < damage) 
			{
				team_max_damage = damage;
				max_team_id = 0;
				max_team_seq = -1;
			}
		}
	}

	if(total_damage < GetMaxHP() ) total_damage = GetMaxHP();
	float factor = 1.f / total_damage;

	if(!_is_boss)
	{
		//如果不是boss 则第一攻击者就是所属者
		owner = _first_attacker;
		task_owner = _first_attacker;
		max_team_id = first_team_id;
		max_team_seq = first_team_seq;
	}

	TempDmgMap::iterator it2 = dlist.begin();
	msg_exp_t exp_data;
	exp_data.level = _basic.level;
	exp_data.no_exp_punish = _no_exp_punish;
		
	int group_level = exp_data.level;
	int world_tag = _plane->GetTag();
	for(;it2 != dlist.end(); ++it2)
	{
		int64_t damage = it2->second.total_damage;
		exp_data.exp = (int64_t)(exp * factor * damage + 0.5f);
		if(it2->first.type > 0)
		{
			//非组队
			if(exp_data.exp > 0 && it2->first.type != GM_TYPE_NPC)
			{
				//非NPC
				SendTo<0>(GM_MSG_EXPERIENCE,it2->first,0,&exp_data,sizeof(exp_data));
			}
		}
		else
		{
			TempDmgEntry &ent = it2->second; 
			int id = -it2->first.type;
		        msg_grpexp_info exp_team_info;
		        memset(&exp_team_info, 0, sizeof(exp_team_info));
			
			if(ent.list.size())
			{
				if(exp_data.exp > 0 )
				{
					exp_team_info.exp = exp_data.exp;
					exp_team_info.team_damage = damage;
					exp_team_info.team_seq = it2->first.id;
					exp_team_info.npc_level = group_level; 
					exp_team_info.npc_tag = world_tag; 
					exp_team_info.no_exp_punish = _no_exp_punish;

					if(id == max_team_id) 
					{
						exp_team_info.npc_id = GetNPCID(); 
						float r = abase::RandUniform();
						exp_team_info.r = r; 
					}

					int size = sizeof(msg_grpexp_info) + ent.list.size()*sizeof(TempDmgNode);
					char buf[size];
					memcpy(buf, &exp_team_info, sizeof(msg_grpexp_info));
					memcpy(buf+sizeof(msg_grpexp_info), ent.list.begin(), ent.list.size() * sizeof(TempDmgNode));
					XID team_leader(GM_TYPE_PLAYER,id);
					SendTo<0>(GM_MSG_GROUP_EXPERIENCE,team_leader,0,buf, size);

					//组队
					/*
					ent.list[0].content.who.type = exp_data.exp;
					ent.list[0].content.who.id =  group_level;
					ent.list[0].content.damage= it2->first.id;	//这个值实际上是seq
					ent.list[0].content.no_exp_punish = _no_exp_punish;
					
					if(id == max_team_id) 
					{
						//如果是造成了最大伤害的队伍，会进行任务和其他内容的发送
						//需要附带的内容有，杀死了哪个NPC
						//该 NPC的级别
						//看来需要额外再占用一位
						ent.list[1].content.who.type = GetNPCID() ;
						float r = abase::RandUniform();
						ent.list[1].content.who.id = *(int*)&r;


					}
					ent.list[1].content.damage = world_tag;

					XID team_leader(GM_TYPE_PLAYER,id);
					SendTo<0>(GM_MSG_GROUP_EXPERIENCE,team_leader,damage,
							ent.list.begin(), ent.list.size() * sizeof(TempDmgNode));
					*/

				}
			}

		}
	}

	//等级按照单人最大攻击力计算
	level = _dmg_list[owner].level;

	//设置组队杀怪标志
	team_id = max_team_id;
	team_seq = max_team_seq;
	
	//现在任务所属就是攻击力最大者(不计算组队累计)
	task_owner = owner;

	wallow_level = max_wallow;
	
	ASSERT(level > 0);
}

void 
gnpc_imp::AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry &dmg)
{
	if(IS_HUMANSIDE(msg.source))
	{
		float adjust = 1.0f;
		player_template::GetAttackLevelPunishment(attack->ainfo.level - _basic.level, adjust);
		dmg.adjust *= adjust;
	}
}

void 
gnpc_imp::OnAttacked(const MSG & msg, attack_msg * attack, damage_entry & dmg, bool is_hit)
{
	if(!is_hit)
	{
		//若未击中才会进行仇恨度的计算 因为后面没有仇恨了
		AddAggroEntry(msg.source,attack->attacker_faction,attack->ainfo.level,3);
	}

	if(attack->skill_id != 0)
	{
		((gnpc_controller*)_commander) -> OnSkillAttack(attack->skill_id);
	}
}

void
gnpc_imp::OnCreate()
{
	if(_buffarea_tid > 0)
	{
		GetWorldManager()->ActiveBuffArea(_buffarea_tid, _parent->ID.id);
	}
	//延迟20个tick发送，解决客户端同步问题
	LazySendTo<0>(GM_MSG_NPC_BORN, _parent->ID,0, 20);
}

void
gnpc_imp::OnBorn()
{
	((gnpc_controller*)_commander) -> OnBorn();
}

void  
gnpc_imp::AddHurtEntry(const XID & attacker, int damage, int team_id,int team_seq,int level, int wallow_level)
{
	if(_dmg_list.empty())
	{
		//考虑记录第一人的数据
		_first_attacker = attacker;
	}
	DAMAGE_MAP::iterator it = _dmg_list.find(attacker);
	if(it != _dmg_list.end())
	{
		hurt_entry & ent = it->second;
		ent.team_id = team_id;
		ent.team_seq = team_seq;
		ent.damage += damage;
		ent.level = level;
		ent.wallow_level = wallow_level;
	}
	else
	{
		if(_dmg_list.size() >= MAX_HURT_ENTRY)
		{	
			//人数满了，不再加入新的数据了
			return ;
		}
		hurt_entry & ent = _dmg_list[attacker];
		ent.team_id = team_id;
		ent.team_seq= team_seq;
		ent.damage = damage;
		ent.level = level;
		ent.wallow_level = wallow_level;
	}
}

void 
gnpc_imp::OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader)
{	
	//首先发送受伤数据
	_runner->be_hurt(info.attacker,info, damage,invader);
	//使用info里的attacker，而不是原来的，这是为了区分宠物的攻击
	if(info.attacker.IsValid() && info.level > 0)
	{
		int level = info.level;
		if(info.eff_level) level = info.eff_level;
		AddHurtEntry(info.attacker,damage,info.team_id,info.team_seq,level,info.wallow_level);
	}
}

void 
gnpc_imp::OnDamage(const XID & attacker,int skill_id,const attacker_info_t & info,int damage, int dt_damage, char at_state,char stamp,bool orange)
{
	// 普攻，修改技能掉落概率
	if (skill_id == 0) _skill_drop_adjust = 0;

	//首先发送受伤数据 数据发给主人，而不是宠物
	_runner->be_damaged(attacker,skill_id, info, damage, dt_damage, 0,at_state,stamp,orange);

	//伤害比例按照内部值计算
	int level = info.level;
	if(info.eff_level) level = info.eff_level;
	AddHurtEntry(info.attacker,damage,info.team_id,info.team_seq,level, info.wallow_level);

	//加入仇恨度
	AddAggroEntry(attacker,0xFFFFFFFF,info.level,damage+2);
	
	if(_basic.hp < damage )
	{
		//自己死亡了，试着发送求救信息
		//由于仇恨消息会被延迟，所以这里需要发送一次
		//考虑在死亡的时候直接发送 仇恨第一位的求助
		((gnpc_controller*)_commander)->TryCryForHelp(attacker);
	}
}

bool 
gnpc_imp::CheckInvader(world * pPlane, const XID & source)
{
	return false;
}


void
gnpc_controller::OnHeartbeat(size_t tick)
{
	if(_ai_core) _ai_core->Heartbeat();
}

void 
gnpc_controller::OnDeath(const XID & attacker)
{
	if(_ai_core) _ai_core->OnDeath(attacker);
}

void
gnpc_controller::OnSkillAttack(int skill_id)
{
	if(_ai_core) _ai_core->OnSkillAttack(skill_id);
}

void
gnpc_controller::OnBorn()
{
	if(_ai_core) _ai_core->OnBorn();
}

void 
gnpc_controller::RefreshAggroTimer(const XID & target)
{
	if(_ai_core) _ai_core->RefreshAggroTimer(target);
}

void 
gnpc_controller::SetIdleMode(bool isIdle)
{
	if(_ai_core) _ai_core->SetIdleMode(isIdle);
}

void 
gnpc_controller::SetSealMode(int seal_flag)
{
	if(_ai_core) _ai_core->SetSealMode(seal_flag);
}


void 
gnpc_imp::OnHeartbeat(size_t tick)
{
	if(_parent->IsZombie())
	{
		if(_corpse_delay > 0)
		{
			_dead_time_count ++;
			if(_dead_time_count >= _corpse_delay) 
			{
				//注意可能会在心跳的时候死亡
				//如果死亡时间超过了死亡等待时间， 则发送消失消息
				SendTo<0>(GM_MSG_OBJ_ZOMBIE_END, _parent->ID,0);
				_dead_time_count = 0;
			}
		}
	}
	if(!_collision_actived)
	{
		_collision_actived = true;
		if(_parent->collision_id >0)
		{
			GetWorldManager()->GetTraceMan().EnableElement(_parent->collision_id, true);
		}
	}

	_filters.Heartbeat(tick);

	//宠物和召唤物不一定遵循这种规则，需要作出考虑（可能让这些对象处于另外一个特殊状态，调
	//用一个特殊的心跳函数或者npc的回血由ai逻辑来控制
	//这里到时候需要区分是否idle状态 ，
	//这个idle状态是指一个特殊的idle状态，即血满并且没有仇恨度时的idle状态，需要考虑一下
	//auto gen hp/mp
	if(!_parent->IsZombie())
	{
		if(!IsCombatState())
		{
			bool sitdown = IsSitDownMode();	//sitdown true是慢速回血
			int hpgen = netgame::GetHPGen(_cur_prop,sitdown);
			int mpgen = netgame::GetMPGen(_cur_prop,sitdown);
			GenHPandMP(hpgen,mpgen);
		}
	}

	//进行时间的减少，用于判断是否进入idle状态  
	if(((gnpc*)_parent)->idle_timer > 0 )
	{
		((gnpc*)_parent)->idle_timer -= 1;
	}
	_native_notify->OnHeartbeat(this,tick);
	if(_battle_npc)
	{
		if(GetRefreshState())
		{
			GetWorldManager()->OnBattleNPCNotifyHP(GetFaction(),((gnpc*)_parent)->tid,_basic.hp,GetMaxHP());
		}
	}
}

void 
gnpc_imp::Reborn()
{
	ASSERT(_npc_state == NPC_STATE_SPAWNING);
	_npc_state = NPC_STATE_NORMAL;
	_birth_place = _parent->pos;
	_birth_dir = _parent->dir;
	//还没有清除所有不利状态 
	//但是所有不利状态应该在死亡和恢复时就清除了
	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

	_basic.hp = GetMaxHP();
	_basic.mp = GetMaxMP();

	_silent_seal_flag = _diet_seal_flag = _melee_seal_flag = _root_seal_flag = 0;

	_npc_state = NPC_STATE_NORMAL;
	_parent->b_zombie = false;
	ClearDamageList();
	ClearSession();
	
	_commander->Reborn();

	_filters.AddFilter(new npc_passive_filter(this));
	_runner->enter_world();
	GetWorldManager()->OnMobReborn( GetNPCID() );  // Youshuang add
}

void 
gnpc_controller::Reborn()
{
	if(_ai_core)
	{
		_ai_core->Reborn();
	}
}


bool 
gnpc_imp::StepMove(const A3DVECTOR &offset)
{
	_direction = offset;
	_knocked_back = 0;
	if(_root_seal_flag) return false;		//定身模式无法移动
	bool bRst = gobject_imp::StepMove(offset);
	_native_notify->OnMove(this);
	return bRst;
}

bool 
gnpc_imp::CanMove()
{
	return !_root_seal_flag;
}


bool 
gnpc_imp::Save(archive & ar)
{
	gactive_imp::Save(ar);
	ar << _npc_state << _money_scale << _birth_place << _leader_id << _inhabit_type << _inhabit_mode << _after_death << _first_attacker << _corpse_delay << _fast_regen << _regen_spawn;
	ar << _dmg_list.size();
	DAMAGE_MAP::iterator it = _dmg_list.begin();
	for(;it != _dmg_list.end(); ++it)
	{
		const DAMAGE_MAP::value_type & val = *it;
		ar << val.first.type << val.first.id;
		ar.push_back(&(val.second), sizeof(val.second));
	}
	return true;
}

bool 
gnpc_imp::Load(archive & ar)
{
	gactive_imp::Load(ar);
	ar >> _npc_state >> _money_scale >> _birth_place >> _leader_id >> _inhabit_type >> _inhabit_mode >> _after_death >> _first_attacker >> _corpse_delay >> _fast_regen >> _regen_spawn;
	size_t size;
	ar >> size;
	ASSERT(_dmg_list.size() == 0);
	for(size_t i = 0; i < size; i ++)
	{
		XID id;
		ar >> id.type >> id.id;
		hurt_entry ent;
		ar.pop_back(&ent,sizeof(ent));
		_dmg_list[id] = ent;
	}
	return true;
}

void 
gnpc_controller::NPCSessionStart(int task_id, int session_id)
{
	if(_ai_core) _ai_core->SessionStart(task_id, session_id);
}

void 
gnpc_controller::NPCSessionEnd(int task_id,int session_id, int retcode)
{
	if(_ai_core) _ai_core->SessionEnd(task_id, session_id,retcode);
}

void 
gnpc_controller::NPCSessionUpdateChaseInfo(int task_id,const void * buf ,size_t size)
{
	if(_ai_core) _ai_core->SessionUpdateChaseInfo(task_id, buf, size);
}

bool 
gnpc_controller::NPCGetNextWaypoint(A3DVECTOR & target)
{
	if(_ai_core) return _ai_core->GetNextWaypoint(target);
	return false;
}

int
gnpc_imp::DoAttack(const XID & target,char force_attack)
{
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack);

	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,0,&attack,sizeof(attack));
	TranslateAttack(target,attack,0);
	gmatrix::SendMessage(msg);
	return 0;
}

// Youshuang add
void gnpc_imp::FillAttackMsg(const XID & target, attack_msg & attack, int dec_arrow )
{
	gactive_imp::FillAttackMsg(target,attack);
	if( _is_fly ){ attack.is_flying = 1; }
}
// end

int 
gnpc_imp::GetComboSealMode()
{
	int state = _silent_seal_flag?ai_object::SEAL_MODE_SILENT:0;
	state += _melee_seal_flag?ai_object::SEAL_MODE_MELEE:0;
	state += _root_seal_flag?ai_object::SEAL_MODE_ROOT:0;
	return state;
}

void 
gnpc_imp::OnSetSealMode()
{
	int state = _silent_seal_flag?ai_object::SEAL_MODE_SILENT:0;
	state += _melee_seal_flag?ai_object::SEAL_MODE_MELEE:0;
	state += _root_seal_flag?ai_object::SEAL_MODE_ROOT:0;
	((gnpc_controller*)_commander)->SetSealMode(state);
	if(_silent_seal_flag && _melee_seal_flag && _root_seal_flag)
	{
		((gnpc_controller*)_commander)->SetIdleMode(0xFF);
		ClearSession();
	}
	else
	{
		((gnpc_controller*)_commander)->SetIdleMode(0);
	}
}

void 
gnpc_controller::TryCryForHelp(const XID & attacker)
{
	if(_ai_core)
	{
		_ai_core->TryCryForHelp(attacker);
	}
}

void 
gnpc_controller::CryForHelp(const XID & attacker,int faction_ask_help,float sight_range)
{
	//现在没有呼救了
	return ; 
}

void
gnpc_imp::SetCombatState()
{
	//do nothing
	//npc的战斗状态由仇恨控制
}

void 
gnpc_imp::AddNPCAggro(const XID & who, int rage)
{
	((gnpc_controller*)_commander)->AddAggro(who,rage);
}

void 
gnpc_imp::BeTaunted(const XID & who, int rage)
{
	((gnpc_controller*)_commander)->BeTaunted(who,rage);
}

void 
gnpc_imp::BeTaunted2(const XID & who, int rage)
{
	((gnpc_controller*)_commander)->BeTaunted(who,rage);
}

void 
gnpc_imp::SendInfo00(link_sid *begin, link_sid *end)
{
	ASSERT(begin != end);
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<CMD::npc_info_00>::From(h1,_parent->ID,_basic.hp,_basic,_cur_prop);
	send_ls_msg(begin, end, h1.data(), h1.size());
}

void 
gnpc_imp::SendBuffData(link_sid *begin, link_sid *end)
{
	if(begin != end)
	{
		packet_wrapper h1(64);
		using namespace S2C;
		h1.clear();
		size_t count;
		const buff_t_s * pBuff = _buff.GetBuffForElse(count);
		CMD::Make<CMD::object_buff_notify>::From(h1,_parent->ID,pBuff,count);
		send_ls_msg(begin, end, h1.data(),h1.size());
	}
}

void 
gnpc_imp::ForwardFirstAggro(const XID & id,int rage)
{
	if(_spawner)
	{
		//生成器存在，直接通知 
		_spawner->ForwardFirstAggro(_plane,id,rage);
	}
	else
	{	
		
		//试图进行转发之
	}
	return ;
}

int 
gnpc_imp::GetMonsterFaction()
{
	return ((gnpc*)_parent)->monster_faction;
}

int 
gnpc_imp::GetFactionAskHelp()
{
	return ((gnpc_controller*)_commander)->GetFactionAskHelp();
}

void 
gnpc_imp::SetLifeTime(int life)
{
	((gnpc_controller*)_commander)->SetLifeTime(life);
}

void 
gnpc_imp::KnockBack(const XID & target, const A3DVECTOR & source, float distance)
{
	A3DVECTOR offset = _parent->pos;
	offset -= source;
	float sq = offset.squared_magnitude();
	if(sq <= 1e-6)
	{
		//这种情况下不击退
		return ;
	}
	
	offset *= distance/sqrt(sq);

	offset += _parent->pos;
	path_finding::GetKnockBackPos(GetWorldManager()->GetMoveMap(), _parent->pos,offset,_inhabit_mode);
	offset -= _parent->pos;

	StepMove(offset);
	_direction.x = -_direction.x;
	_direction.y = -_direction.y;
	_direction.z = -_direction.z;
	_knocked_back = 1;

	_runner->stop_move(_parent->pos,0x0f00,1,C2S::MOVE_MODE_KNOCK);
}

void 
gnpc_imp::BounceTo(const  A3DVECTOR & target_pos, float distance)
{
	A3DVECTOR dest_pos = _parent->pos;
	dest_pos -= target_pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= distance/sqrt(sq);
	dest_pos += _parent->pos;
	if(!path_finding::GetValidPos(GetWorldManager()->GetMoveMap(), dest_pos)) return;

	_runner->object_charge(0, 3, dest_pos); 

	dest_pos -= _parent->pos;
	StepMove(dest_pos);

}

//吸引
void 
gnpc_imp::DrawTo(const XID & target, const A3DVECTOR & dest_pos)
{
 	A3DVECTOR offset = dest_pos;
	if(!path_finding::GetValidPos(GetWorldManager()->GetMoveMap(), offset)) return;

	_runner->object_charge(target.id, 4, dest_pos); 

	offset -= _parent->pos;
	StepMove(offset);
}

//击退
void 
gnpc_imp::Repel2(const  A3DVECTOR & target_pos, float distance)
{
	A3DVECTOR dest_pos = _parent->pos;
	dest_pos -= target_pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= distance/sqrt(sq);
	dest_pos += _parent->pos;

	if(!path_finding::GetValidPos(GetWorldManager()->GetMoveMap(), dest_pos)) return;

	_runner->object_charge(0, 5, dest_pos);
	dest_pos -= _parent->pos;
	StepMove(dest_pos);
}

//背摔
void 
gnpc_imp::Throwback(const A3DVECTOR & target_pos, float dist)
{
	A3DVECTOR dest_pos = target_pos;
	dest_pos -= _parent->pos;
	float sq = dest_pos.squared_magnitude();
	dest_pos *= dist/sqrt(sq);
	dest_pos += target_pos; 
	if(!path_finding::GetValidPos(GetWorldManager()->GetMoveMap(), dest_pos)) return;

	_runner->object_charge(0, 6, dest_pos);
	dest_pos -= _parent->pos;
	StepMove(dest_pos);
}

void
gnpc_imp::Flee(const XID & target, float distance)
{
	world::object_info info;
	if(_plane->QueryObject(target,info))
	{
		A3DVECTOR offset = _parent->pos;
		offset -= info.pos;

		if(offset.squared_magnitude() < distance * distance)
		{
			gnpc_ai* pAI = ((gnpc_controller*)_commander)->GetAI();
			ai_policy * pPolicy = pAI->GetAICtrl();
			ai_task* cur_task = pPolicy->GetCurTask();
			if(!cur_task || cur_task->GetTaskType() != ai_task::AI_TASK_TYPE_RUNAWAY)
			{
				pPolicy->AddTargetTask<ai_runaway_task>(target);
			}
		}
	}
}

void 
gnpc_imp::AddAggroEntry(const XID & who , int faction, int level, int rage)
{
	//通过类似消息的方式增加aggro
	msg_aggro_info_t info = { who,rage,0,faction,level};
	MSG newmsg;
	BuildMessage(newmsg,GM_MSG_GEN_AGGRO,_parent->ID,who,_parent->pos,1,&info,sizeof(info));
	MessageHandler(newmsg);
}

gnpc_imp * 
gnpc_imp::TransformMob(int target_id, int lifetime)
{
	gnpc *pNPC = (gnpc*)_parent;
	//暂时将自己从世界中移出
	_plane->RemoveNPC(pNPC);

	gnpc npc = *pNPC;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = target_id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC2 = npc_spawner::CreateMobBase(_spawner,_plane,ent,npc.spawn_index,npc.phase_id, npc.pos,cid,npc.dir,CLS_NPC_AI_POLICY_BASE,0,&npc, lifetime);
	if(pNPC2 == NULL) 
	{
		//无法创建中间对象
		_plane->InsertNPC(pNPC);
		return  NULL;
	}

	ASSERT(pNPC2 == &npc);

	gnpc_imp *__this = (gnpc_imp *)npc.imp;

	//记录pPlane在自身释放以后就不能使用_plane了
	world * pPlane = _plane;

	//清除当前的选中对象
	_buff.ClearSubscibeList(this);
	//释放自己 但不释放 pNPC本身
	_commander->Release(false);

	//清空原有NPC的数据
	pNPC->Clear();

	//将新数据注入
	*pNPC = npc;

	//修正新数据里的parent信息和生成数据
	__this->_parent = pNPC;
	__this->_regen_spawn = 1;

	//将新NPC加入到世界中
	pPlane->InsertNPC(pNPC);

	return __this;
}


service_npc * 
gnpc_imp::TransformNPC(int target_id, int lifetime)
{
	npc_template * pTemplate = npc_stubs_manager::Get(target_id);
	ASSERT(pTemplate);
	ASSERT(pTemplate->npc_data);
	if(!pTemplate) return NULL; 

	gnpc *pNPC = (gnpc*)_parent;
	//暂时将自己从世界中移出
	_plane->RemoveNPC(pNPC);

	gnpc npc = *pNPC;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = target_id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;
	const int cid[3] = {CLS_SERVICE_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int aipolicy = 0;
	int aggro_policy = AGGRO_POLICY_BOSS;
	switch(pTemplate->npc_data->npc_type)
	{
		case npc_template::npc_statement::NPC_TYPE_GUARD:
			aipolicy = CLS_NPC_AI_POLICY_GUARD;
			break;
		case npc_template::npc_statement::NPC_TYPE_NORMAL:
			aipolicy = CLS_NPC_AI_POLICY_SERVICE;
			break;
		default:
			//普通的不做任何处理即可
			break;
	}
	gnpc * pNPC2 = npc_spawner::CreateMobBase(_spawner,_plane,ent,npc.spawn_index,npc.phase_id,npc.pos,cid,npc.dir,aipolicy,aggro_policy,&npc, lifetime);
	if(pNPC2 == NULL) 
	{
		//无法创建中间对象
		_plane->InsertNPC(pNPC);
		return  NULL;
	}

	ASSERT(pNPC2->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(service_npc)));
	service_npc * pImp = (service_npc *)pNPC2->imp;
	pImp->SetTaxRate(pTemplate->npc_data->tax_rate);
	pImp->SetNeedDomain(pTemplate->npc_data->need_domain);
	pImp->SetAttackRule(pTemplate->npc_data->attack_rule);
	
	server_spawner::AddNPCServices(pImp, pTemplate);
	ASSERT(pNPC2 == &npc);

	//记录pPlane在自身释放以后就不能使用_plane了
	world * pPlane = _plane;

	//清除当前的选中对象
	_buff.ClearSubscibeList(this);
	//释放自己 但不释放 pNPC本身
	_commander->Release(false);

	//清空原有NPC的数据
	pNPC->Clear();

	//将新数据注入
	*pNPC = npc;

	//修正新数据里的parent信息和生成数据
	pImp->_parent = pNPC;
	pImp->_regen_spawn = 1;

	//将新NPC加入到世界中
	pPlane->InsertNPC(pNPC);

	return pImp;
}

void
gnpc_imp::TransformMine(int target_id, int lifetime)
{
	object_interface oif(this);
	oif.CreateMines(target_id, 1, lifetime, 0);

	_runner->disappear(true);
	_parent->b_zombie = true;
	LifeExhaust();
}


void 
gnpc_controller::SetFastRegen(bool b)
{
	if(_ai_core) _ai_core->SetFastRegen(b);
}

void 
gnpc_imp::RelocatePos(bool )
{
}

bool
gnpc_imp::DropItemFromGlobal(const XID & owner, int owner_level , int team_id, int team_seq, int wallow_level)
{
	//检查是否存在全局掉落表
	drop_template::drop_entry * pEntry = drop_template::GetDropList(GetNPCID());
	if(!pEntry) return true;
	float money_adj;
	float drop_adj;
	player_template::GetDropPunishment(owner_level - _basic.level, &money_adj, &drop_adj);
	drop_adj *= _drop_rate;

	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustNormalMoneyItem(wallow_level, money_adj,drop_adj);
	}

	if(!_fix_drop_rate)
	{
		if(_skill_drop_adjust > 0)
		{
			drop_adj *= (1 - _skill_drop_adjust);
		}
	}

	//获得掉落次数
	int item_more_times = gmatrix::GetWorldParam().double_drop?2:1;

	for(int t= 0; t < item_more_times; t ++)
	{
		if(abase::RandUniform() <= drop_adj)
		{
			int drop_list[48];
			//生成物品 
			int count = sizeof(drop_list)/sizeof(int)-4;
			int rst = drop_template::GenerateItem(pEntry ,drop_list + 4 ,count);
			if(rst > 0)
			{
				drop_list[0] = team_id;
				drop_list[1] = team_seq;
				drop_list[2] = GetNPCID();
				drop_list[3] = rst;
				MSG msg;
				BuildMessage(msg,GM_MSG_PRODUCE_MONSTER_DROP, XID(GM_TYPE_PLANE,_plane->GetTag()),
						owner ,_parent->pos,0,drop_list,(rst + 4)*sizeof(int));
				gmatrix::SendPlaneMessage(_plane, msg);

				//记录boss 的掉落log
				if(_is_boss)
				{
					for(int i = 0; i < rst; ++i)
					{
						int item_id = drop_list[i+4];
						GLog::action("boss drop, boss_id=%d, item_id=%d", GetNPCID(), item_id); 
					}
				}
			}
		}
	}

	return pEntry->type != EDT_TYPE_REPLACE;
} 

void
gnpc_imp::DropItemFromData(const XID & owner, int owner_level, int team_id,int team_seq, int wallow_level)
{
	float money_adj,drop_adj;
	player_template::GetDropPunishment(owner_level - _basic.level, &money_adj, &drop_adj);
	drop_adj *= _drop_rate;

	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustNormalMoneyItem(wallow_level, money_adj,drop_adj);
	}

	if(!_fix_drop_rate)
	{
		if(_skill_drop_adjust > 0)
		{
			drop_adj *= (1.0f  - _skill_drop_adjust);
			money_adj *= (1.0f - _skill_drop_adjust);
		}
	}

	//获得掉落次数
	int item_more_times = gmatrix::GetWorldParam().double_drop?2:1;

	for(int t= 0; t < item_more_times; t ++)
	{
		if(abase::RandUniform() <= drop_adj)
		{
			int drop_list[480];
			int rst = gmatrix::GetDataMan().generate_item_from_monster(((gnpc*)_parent)->tid,drop_list+4,sizeof(drop_list)/sizeof(int) - 4);
			if(rst > 0)
			{
				drop_list[0] = team_id;
				drop_list[1] = team_seq;
				drop_list[2] = GetNPCID();
				drop_list[3] = rst;
				MSG msg;
				BuildMessage(msg,GM_MSG_PRODUCE_MONSTER_DROP, XID(GM_TYPE_PLANE,_plane->GetTag()),
						owner ,_parent->pos,0,drop_list,(rst + 4)*sizeof(int));
				gmatrix::SendPlaneMessage(_plane, msg);

				//记录boss 的掉落log
				if(_is_boss)
				{
					for(int i = 0; i < rst; ++i)
					{
						int item_id = drop_list[i+4];
						GLog::action("boss drop, boss_id=%d, item_id=%d", GetNPCID(), item_id); 
					}
				}
			}
		}
	}
}

void 
gnpc_imp::DropMoney(const XID & owner, int owner_level, int team_id, int team_seq, int wallow_level)
{
	if(abase::RandUniform() > g_config.money_drop_rate) return;
	float money_adj,drop_adj;
	int low,high;
	player_template::GetDropPunishment(owner_level - _basic.level, &money_adj, &drop_adj);
	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustNormalMoneyItem(wallow_level, money_adj,drop_adj);
	}

	if(!_fix_drop_rate)
	{
		if(_skill_drop_adjust > 0)
		{
			drop_adj *= (1.0f  - _skill_drop_adjust);
			money_adj *= (1.0f - _skill_drop_adjust);
		}
	}

	gmatrix::GetDataMan().get_monster_drop_money(GetNPCID(),low,high);
	int money = (int)(abase::Rand(low,high) * _money_scale * money_adj + 0.5f);
	if(money <= 0) return;
	if(gmatrix::GetWorldParam().double_money) money *= 2;

	if(team_id > 0)
	{
		//是队伍的，发给队长 
		SendTo2<0>(GM_MSG_MONSTER_GROUP_MONEY,XID(GM_TYPE_PLAYER,team_id), money,GetWorldTag());
	}
	else if(owner.IsPlayer())
	{
		SendTo<0>(GM_MSG_MONSTER_MONEY,owner, money);
	}
}

void 
gnpc_imp::DropItem(const XID & owner, int owner_level, int team_id,int team_seq, int wallow_level)
{

	if(DropItemFromGlobal(owner, owner_level , team_id, team_seq,wallow_level))
	{
		DropItemFromData(owner, owner_level, team_id,team_seq, wallow_level);
	}
	DropMoney(owner, owner_level, team_id, team_seq,wallow_level);
}

void 
gnpc_imp::OI_UpdateAllProp()
{
	property_policy::UpdateNPC(this);
}

bool 
gnpc_imp::OI_GetLeader(XID & leader)
{       
	if(_leader_id.IsValid())
	{
		leader = _leader_id;
		return true;
	}
	return false;
}       

int
gnpc_imp::OI_GetTemplateID()
{
	return ((gnpc*)_parent)->tid;
}

void 
gnpc_imp::PlayerCatchPet(const XID catcher)
{
	if(_parent->IsZombie())
	{
		return;
	}

	//查询对象	
	world::object_info info;
	if(!_plane->QueryObject(catcher,info))
	{
		__PRINTF("目标不存在\n");
		return;
	}

	gnpc *pNPC = GetParent();
	npc_template * pTemplate = npc_stubs_manager::Get(pNPC->tid);
	if(!pTemplate) return;

	if(!pTemplate->can_catch) return; 

	if(!catcher.IsPlayerClass() || !catcher.IsActive()) return;

	msg_catch_pet catch_msg;

	catch_msg.monster_id = pNPC->tid;
	catch_msg.monster_level = _basic.level;
	catch_msg.monster_raceinfo = pTemplate->monster_faction;
	catch_msg.monster_catchdifficulty = pTemplate->can_catch;
	catch_msg.monster_hp_ratio = (float)_basic.hp / (float)GetMaxHP();
	
	SendTo<0>(GM_MSG_PLAYER_CATCH_PET,catcher,0, &catch_msg, sizeof(catch_msg));
}
		

bool 
gnpc_imp::CatchedByPlayer()
{	
	if(_parent->b_zombie) return false;

	//通知自己消失
	_runner->stop_move(_parent->pos,0x500,1,C2S::MOVE_MODE_RETURN);
	_parent->b_zombie = true;
	((gnpc*)_parent)->object_state &= ~gactive_object::STATE_STATE_CORPSE;
	ClearSession();
	SendTo<0>(GM_MSG_OBJ_ZOMBIE_END, _parent->ID,0);
	_corpse_delay = 1;
	_dead_time_count = 0;
	return true;
}

bool
gnpc_imp::CanTransform(int template_id)
{
	int monster_id = ((gnpc*)_parent)->tid; 
	return player_template::CheckOriginalInfo(template_id, monster_id);
}

//TRANSFORM2: 照妖镜
void
gnpc_imp::TransformMonster(int template_id, int lifetime)
{
	int monster_id = ((gnpc*)_parent)->tid; 
	int target_type = 0;
	int target_id = player_template::GetOriginalID(template_id, monster_id, target_type);
	if(target_id <= 0) return;
	if(target_type == 0)
	{
		SendTo2<0>(GM_MSG_NPC_TRANSFORM2, _parent->ID, target_id, lifetime);
	}
	else if(target_type == 1)
	{
		SendTo2<0>(GM_MSG_MONSTER_TRANSFORM2, _parent->ID, target_id, lifetime);
	}
	else if(target_type == 2)
	{
		SendTo2<0>(GM_MSG_MINE_TRANSFORM2, _parent->ID, target_id, lifetime);
	}
}

bool
gnpc_imp::IsNPCOwner(int roleid, int teamid, int master_id)
{
	return ((gnpc*)_parent)->IsNPCOwner(roleid, teamid, master_id);

}

void
gnpc_imp::SetDimState(bool is_dim)
{
	gnpc * pNPC = (gnpc*)_parent;
	if(is_dim)
	{
		pNPC->SetObjectState(gactive_object::STATE_NPC_DIM);
		_runner->enter_dim_state(is_dim);
	}
	else
	{
		pNPC->ClrObjectState(gactive_object::STATE_NPC_DIM);
		_runner->enter_dim_state(is_dim);
	}
}

void
gnpc_imp::SetFrozenState(bool on)
{
	gnpc * pNPC = (gnpc*)_parent;
	if(on)
	{
		pNPC->SetObjectState(gactive_object::STATE_NPC_FROZEN);
	}
	else
	{
		pNPC->ClrObjectState(gactive_object::STATE_NPC_FROZEN);
	}
	_runner->object_special_state(S2C::STATE_FROZEN, on);
}

