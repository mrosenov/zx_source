#include "world.h"
#include "playerbattle.h"
#include "clstab.h"
#include "faction.h"
#include "instance/battleground_ctrl.h"
#include "sfilterdef.h"

DEFINE_SUBSTANCE(gplayer_battleground,gplayer_imp,CLS_BATTLEGROUND_PLAYER_IMP)
int 
gplayer_battleground::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{

		case GM_MSG_TURRET_NOTIFY_LEADER:
			//每收到一次此种通知 在3秒钟之内都不能进行再次召唤
			turret_counter = 3;
		return 0;
		
	//攻击判定以帮派为最优
		case GM_MSG_ATTACK:
			{
				attack_msg ack_msg = *(attack_msg*)msg.content;
				_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
				XID attacker = ack_msg.ainfo.attacker;

				//这里无需区分玩家和NPC
				//自己不允许攻击
				if(attacker.id == _parent->ID.id) return 0;
				if(!(ack_msg.target_faction  & (GetFaction())))
				{
					//派系不正确则返回
					return 0;
				}
				ack_msg.target_faction = 0xFFFFFFFF;
				ack_msg.is_invader =  false;

				//试着选择对象
				((gplayer_controller*)_commander)->TrySelect(msg.source);
				HandleAttackMsg(msg,&ack_msg);
				return 0;
			}
			break;
		case GM_MSG_ENCHANT:
			{
				//进行处理
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
				if(!ech_msg.helpful)
				{
					XID attacker = ech_msg.ainfo.attacker;
					//自己不允许攻击
					if(attacker.id == _parent->ID.id) return 0;
					if(!(ech_msg.target_faction  & (GetFaction())))
					{
						//派系不正确则返回
						return 0;
					}
					
					ech_msg.target_faction = 0xFFFFFFFF;
					((gplayer_controller*)_commander)->TrySelect(msg.source);
				}
				else
				{
					//只有自己方可以使用有益法术
					if(!(ech_msg.attacker_faction  &  defense_faction))
					{
						//派系不正确则返回
						return 0;
					}
				}
				ech_msg.is_invader = false;
				HandleEnchantMsg(msg,&ech_msg);
				return 0;
			}

		case GM_MSG_ENCHANT_ZOMBIE:
		if(IsDead())
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if(!ech_msg.helpful)
			{
				return 0;
			}
			else
			{
				//只有自己方可以使用有益法术
				if(!(ech_msg.attacker_faction  &  defense_faction))
				{
					//派系不正确则返回
					return 0;
				}
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
		}
		return 0;
	}
	return gplayer_imp::MessageHandler(msg);
}


void
gplayer_battleground::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= defense_faction;
	attack.target_faction |= attack_faction;
}

void
gplayer_battleground::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= defense_faction;
	enchant.target_faction |= attack_faction;
}

void 
gplayer_battleground::PlayerEnterWorld()
{	
	gplayer_imp::PlayerEnterWorld();

	//正常情况下，这是不会出现的
	attack_faction = 0;
	defense_faction = 0;
}

void 
gplayer_battleground::OnHeartbeat(size_t tick)
{
	gplayer_imp::OnHeartbeat(tick);
	if(turret_counter) turret_counter --;
}

int 
gplayer_battleground::CheckUseTurretScroll()
{
	if(turret_counter >0)
	{
		return S2C::ERR_CONTROL_TOO_MANY_TURRETS;
	}
	return 0;
}

void 
gplayer_battleground::PlayerEnterMap()
{
	gplayer_imp::PlayerEnterMap();
	gplayer * pPlayer = GetParent();

	battleground_ctrl * pCtrl = (battleground_ctrl*)_plane->w_ctrl;
	//考虑完成攻方守方的判断
	if(pPlayer->IsBattleOffense())
	{
		//攻方
		attack_faction = FACTION_BATTLEDEFENCE;
		defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
//		_runner->enter_battleground(1,pCtrl->_data.battle_id,pCtrl->_data.end_timestamp,0,0,0,NULL);
		pCtrl->PlayerEnter(pPlayer,0x01);

	}
	else if(pPlayer->IsBattleDefence())
	{
		//守方
		attack_faction = FACTION_BATTLEOFFENSE;
		defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
//		_runner->enter_battleground(2,pCtrl->_data.battle_id,pCtrl->_data.end_timestamp,0,0,0,NULL);
		pCtrl->PlayerEnter(pPlayer,0x02);
	}
	else
	{
		attack_faction = 0;
		defense_faction = 0;
//		_runner->enter_battleground(0,pCtrl->_data.battle_id,pCtrl->_data.end_timestamp,0,0,0,NULL);
		pCtrl->PlayerEnter(pPlayer,0x0);
	}
	EnableFreePVP(true);

}

void 
gplayer_battleground::PlayerLeaveMap()
{
	gplayer * pPlayer = GetParent();
	battleground_ctrl * pCtrl = (battleground_ctrl*)_plane->w_ctrl;

	//进行攻守双方人员记数的减少
	//这个人数的增加是在battleground_world_message_handler的PlayerPreEnterServer里做的
	if(pPlayer->IsBattleOffense())
	{
		//攻方
		pCtrl->DelAttacker();
		pCtrl->PlayerLeave(pPlayer,0x01);

	}
	else if(pPlayer->IsBattleDefence())
	{
		//守方
		pCtrl->DelDefender();
		pCtrl->PlayerLeave(pPlayer,0x02);
	}
	
	EnableFreePVP(false);
	pPlayer->ClrBattleMode();

	//如果玩家死亡则复活
	if(_parent->b_zombie)
	{
		_parent->b_zombie = false;
		_basic.hp = (int)(_cur_prop.max_hp * 0.1f + 0.5f);
		_basic.mp = (int)(_cur_prop.max_mp * 0.1f + 0.5f);

		SetRefreshState();
		_enemy_list.clear();

		((gplayer_controller*)_commander)->OnResurrect();

		//清除后继所有的session
		ClearNextSession();

		_runner->resurrect(0);
	}

	//清除所有的负面状态
	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

//	_runner->enter_battleground(0,0,0,0,0,0,NULL);
}

int
gplayer_battleground::Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int target_tag)
{
	gplayer_imp::Resurrect(pos,nomove,exp_reduce, target_tag);

	return 0;
}

void 
gplayer_battleground::PlayerLeaveWorld()
{
	PlayerLeaveMap();
}

int 
gplayer_battleground::GetFaction()
{
	return _faction | defense_faction;
}

int 
gplayer_battleground::GetEnemyFaction()
{
	return _enemy_faction | attack_faction;
}

gactive_imp::attack_judge 
gplayer_battleground::GetHookAttackJudge()
{
	return gplayer_battleground::__GetHookAttackJudge;
	
}

gactive_imp::enchant_judge 
gplayer_battleground::GetHookEnchantJudge()
{
	return gplayer_battleground::__GetHookEnchantJudge;
}

gactive_imp::attack_fill 
gplayer_battleground::GetHookAttackFill()
{
	return __GetHookAttackFill;
}

gactive_imp::enchant_fill 
gplayer_battleground::GetHookEnchantFill()
{
	return __GetHookEnchantFill;
}

bool 
gplayer_battleground::__GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//派系不正确则返回
	if(!(amsg.target_faction  & (__this->GetFaction()))) return false;
	amsg.target_faction = 0xFFFFFFFF;
	amsg.is_invader =  false;
	return true;
}

bool 
gplayer_battleground::__GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	if(!emsg.helpful)
	{
		//派系不正确则返回
		if(!(emsg.target_faction  & (__this->GetFaction()))) return false;
		emsg.target_faction = 0xFFFFFFFF;
	}
	else
	{
		//只有对方不视自己是敌人才能够使用有益法术 这里和player_battle的并不完全一致
		if(emsg.target_faction  &  __this->GetFaction()) return false;
	}
	emsg.is_invader = false;
	return true;
}

void
gplayer_battleground::__GetHookAttackFill(gactive_imp * __this, attack_msg & attack)
{
	gplayer_imp::__GetHookAttackFill(__this,attack);
	attack.force_attack = 0;
}

void
gplayer_battleground::__GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	gplayer_imp::__GetHookEnchantFill(__this,enchant);
	enchant.force_attack = 0;
}

