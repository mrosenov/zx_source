#include "clstab.h"
#include "npc.h"
#include "ainpc.h"
#include "world.h"
#include <common/protocol.h>
#include "invincible_filter.h"
#include "usermsg.h"
#include "aitrigger.h"

DEFINE_SUBSTANCE_ABSTRACT(aggro_policy,substance, CLS_NPC_AGGRO_POLICY)
DEFINE_SUBSTANCE(aggro_minor_policy,aggro_policy, CLS_NPC_MINOR_AGGRO_POLICY)
DEFINE_SUBSTANCE(aggro_mobs_policy,aggro_policy, CLS_NPC_MOBS_AGGRO_POLICY)
DEFINE_SUBSTANCE(aggro_boss_policy,aggro_policy, CLS_NPC_BOSS_AGGRO_POLICY)
DEFINE_SUBSTANCE(pet_aggro_policy,aggro_policy, CLS_PET_AGGRO_POLICY)
DEFINE_SUBSTANCE(turret_aggro_policy,aggro_policy, CLS_TURRET_AGGRO_POLICY)
DEFINE_SUBSTANCE(summon_aggro_policy,aggro_policy, CLS_SUMMON_AGGRO_POLICY)

ai_trigger::policy * 
ai_object::CreatePolicy(int id)
{
	const ai_trigger::policy * pTmp = gmatrix::GetTriggerMan().GetPolicy(id);
	if(pTmp)
	{
		return new ai_trigger::policy(*pTmp);
	}
	return NULL;
}

int 
ai_npcobject::GetSealMode()
{
	return ((gnpc_imp*)_imp)->GetComboSealMode();
}

bool
ai_npcobject::IsReturnHome(A3DVECTOR & pos, float offset_range)
{
	pos = ((gnpc_imp*)_imp)->_birth_place;
	if(pos.squared_distance(_imp->_parent->pos) > offset_range)
	{
		return true;
	}
	return false;
}

void
ai_npcobject::BeHurt(int hp)
{
	attacker_info_t info ={XID(-1,-1),0,0,0,0,0};
	_imp->BeHurt(XID(-1,-1),info,hp,false,0);
}

void ai_npcobject::Say(const char * msg)
{
	_imp->Say(msg);
}

void 
ai_npcobject::Say(const XID & target, const void * msg, size_t size, int level)
{
	gnpc_imp * pImp = (gnpc_imp*)_imp;
	gnpc * pNPC = (gnpc*)pImp->_parent;
	if(pNPC->pPiece)
	{
		SaySomething(pImp->_plane,pNPC->pPiece,msg,size,GMSV::CHAT_CHANNEL_LOCAL,pNPC->ID.id);
	}
}

void ai_npcobject::BattleFactionSay(const void * msg, size_t size)
{
	gnpc_imp * pImp = (gnpc_imp*)_imp;
	pImp->_plane->BattleFactionSay(pImp->GetFaction(), msg,size);
}

void ai_npcobject::BattleSay(const void * msg, size_t size)
{
	gnpc_imp * pImp = (gnpc_imp*)_imp;
	pImp->_plane->BattleSay(msg,size);
}

void ai_npcobject::BroadcastSay(const void * msg, size_t size, char channel)
{
	if(channel < 0)
	{
		channel = GMSV::CHAT_CHANNEL_FARCRY;
	}
	broadcast_chat_msg(0,msg, size, channel,0, 0, 0);
}

void 
ai_npcobject::AddInvincibleFilter(const XID & who)
{
	filter * f = new invincible_filter_to_spec_id(_imp,1000,20,who);
	_imp->_filters.AddFilter(f);
}

void 
ai_npcobject::ReturnHome(const A3DVECTOR & target,float range)
{
	gnpc_imp * pImp = (gnpc_imp*)_imp;
	gnpc * pNPC = (gnpc*)(_imp->_parent);
	A3DVECTOR pos(target);
	pos.x += abase::Rand(-range,range);
	pos.z += abase::Rand(-range,range);
	if(range < 1e-3)
	{
		pImp->ClearDirection();
		pNPC->dir = pImp->_birth_dir;
	}

	//超过距离10米的时候即回到出生位置
	_imp->_runner->stop_move(pos,0x500,1,C2S::MOVE_MODE_RETURN);
	pos -= _imp->_parent->pos;
	_imp->StepMove(pos);
}

const XID & 
ai_npcobject::GetLeaderID()
{
	return ((gnpc_imp*)_imp)->_leader_id;
}

void 
ai_npcobject::RelocatePos(bool is_disappear)
{
	((gnpc_imp*)_imp)->RelocatePos(is_disappear);
}

void
ai_npcobject::SetLeaderID(const XID & leader)
{
	((gnpc_imp*)_imp)->_leader_id  = leader;
}

void 
ai_npcobject::ForwardFirstAggro(const XID & id,int rage)
{
	gnpc_imp * pImp = (gnpc_imp*)_imp;
	pImp->ForwardFirstAggro(id,rage);
	return ;
}

bool
ai_npcobject::CheckWorld()
{
	gnpc_imp * pImp = (gnpc_imp*)_imp;
	return pImp->_plane->w_activestate == 1;
	
}

float 
ai_npcobject::GetSightRange()
{
	return	_ctrl->GetAI()->GetSightRange();
}

bool 
gnpc_ai::Init(gnpc_controller * pControl,const aggro_param & aggp, const ai_param & aip)
{	
	_commander= pControl;
	ASSERT(_aggro == NULL);
	switch(aggp.aggro_policy)
	{
		case AGGRO_POLICY_NULL:
			_aggro = new aggro_mobs_policy;
			break;
		case AGGRO_POLICY_BOSS:
			_aggro = new aggro_boss_policy;
			break;
		case AGGRO_POLICY_BOSS_MINOR:
			_aggro = new aggro_mobs_policy;
			break;
		case AGGRO_POLICY_MOBS:
			_aggro = new aggro_mobs_policy;
			break;
		case AGGRO_POLICY_PET:
			_aggro = new pet_aggro_policy;
			break;
		case AGGRO_POLICY_TURRET:
			_aggro = new turret_aggro_policy;
			break;
		case AGGRO_POLICY_SUMMON:
			_aggro = new summon_aggro_policy;
			break;

		default:
			ASSERT(false);
	}
	_aggro->Init(aggp);
	substance * pSub = substance::CreateInstance(aip.policy_class);
	ASSERT(pSub->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(ai_policy)));
	_core = (ai_policy*)pSub;
	ai_npcobject self((gactive_imp*)pControl->_imp,pControl,_aggro);
	_core->Init(self,aip);

	_core->SetAITrigger(aip.trigger_policy);

	float body_size = self.GetBodySize();

	//初始化自己的参数
	_sight_range = aggp.sight_range;
	_squared_sight_range = (_sight_range + body_size)* (_sight_range + body_size);
	_faction_ask_help = aggp.faction_ask_help;
	_faction_accept_help = aggp.faction_accept_help;
	return true;
}


void 
ai_npcobject::SendClientTurretNotify(int id)
{
	_imp->_runner->send_turrent_leader(id);
}

int
ai_npcobject::GetState()
{
	//还有移出世界的状态
	if(_imp->_parent->IsZombie())
	{	
		return STATE_ZOMBIE;
	}
	return STATE_NORMAL;
}


bool 
ai_npcobject::CanRest()
{
	gnpc * pNPC = (gnpc*)_imp->_parent;
	if(pNPC->idle_timer <= 0 ) return false;
	pNPC->cruise_timer = (pNPC->cruise_timer - 1) & (32 - 1);
	return (pNPC->cruise_timer == 0);
}

bool 
ai_npcobject::IsInIdleHeartbeat()
{
	gnpc * pNPC = (gnpc*)_imp->_parent;
	return (pNPC->idle_timer <= 0 );
}

bool
ai_npcobject::IsDisappearLifeExpire()
{
	return ((gnpc_imp*)_imp)->IsDisappearLifeExpire();
}

void 
ai_npcobject::GetPatrolPos(A3DVECTOR & pos)
{
	pos = ((gnpc_imp*)_imp)->_birth_place;

}

void 
ai_npcobject::GetPatrolPos(A3DVECTOR & pos,float range)
{
	pos.x += abase::Rand(-range,range);
	pos.z += abase::Rand(-range,range);
	pos.y = _imp->GetWorldManager()->GetTerrain().GetHeightAt(pos.x, pos.z);
	pos.y = 0;
}

bool 
gnpc_ai::Save(archive & ar)
{
	ar << _sight_range << _squared_sight_range << _faction_ask_help << _faction_accept_help;
	if(_aggro) 
		_aggro->SaveInstance(ar);
	else 
		ar << -1;

	if(_core)
		_core->SaveInstance(ar);
	else
		ar << -1;
	return true;
}

bool 
gnpc_ai::Load(archive & ar)
{
	ar >> _sight_range >> _squared_sight_range >> _faction_ask_help >> _faction_accept_help;
	int guid;
	ar >> guid;
	ASSERT(_aggro == NULL && _core == NULL);
	if(guid >= 0)
	{
		_aggro = substance::LoadSpecInstance<aggro_policy>(guid,ar);
	}
	ar >> guid;
	if(guid >= 0)
	{
		_core = substance::LoadSpecInstance<ai_policy>(guid,ar);
	}
	_commander = NULL;
	ASSERT(_aggro && _core);
	return _aggro && _core;
}

void 
gnpc_ai::Heartbeat()
{
	_aggro->OnHeartbeat();
	if(_aggro->Size() && g_timer.get_systime() % 2 == 0) //每两秒才会发一次
	{
		XID target;
		_aggro->GetFirst(target);
		if(target.type != GM_TYPE_NPC)
		{
			ai_npcobject obj((gactive_imp*)_commander->_imp,_commander,_aggro);
			obj.HateTarget(target);
		}

		/*
		//给所有敌人发送HATE消息
		abase::vector<XID> list;
		list.reserve(_aggro->Size());
		_aggro->GetAll(list);
		gactive_imp * imp = (gactive_imp*)_commander->_imp;
		MSG msg;
		BuildMessage(msg,GM_MSG_HATE_YOU,XID(-1,-1),imp->_parent->ID,imp->_parent->pos);
		imp->_plane->SendMessage(list.begin(),list.end(),msg);
		*/
	}
	_core->OnHeartbeat();
}
