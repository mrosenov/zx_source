#include <stdio.h>
#include <sys/types.h>
#include "../clstab.h"
#include "../task/taskman.h"
#include "bf_player_imp.h"
#include "bf_world_manager.h"
#include "../userlogin.h"
#include "../usermsg.h"

DEFINE_SUBSTANCE(bf_player_imp,gplayer_imp,CLS_BF_PLAYER_IMP)

bf_player_imp::bf_player_imp()
{
	_attack_faction = 0;
	_defense_faction = 0;
	_old_world_tag = 0;
	_old_pos = A3DVECTOR(0,0,0);
	_timeout = -1;
	_kickout = 0;
	_battle_result_sent = false;
	_turret_counter = 0;
	_battle_info_seq = 0;
}

void bf_player_imp::FillAttackMsg(const XID& target,attack_msg& attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction |= _attack_faction;
}

void bf_player_imp::FillEnchantMsg(const XID& target,enchant_msg& enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	enchant.target_faction |= _attack_faction;
}

int bf_player_imp::GetFaction()
{
	return _faction | _defense_faction;
}

int bf_player_imp::GetEnemyFaction()
{
	return _enemy_faction | _attack_faction;
}

int bf_player_imp::MessageHandler(const MSG& msg)
{
	switch(msg.message)
	{
		case GM_MSG_TURRET_NOTIFY_LEADER:
		{
			//每收到一次此种通知 在3秒钟之内都不能进行再次召唤
			_turret_counter = 3;
			return 0;
		}
		break;
		
		case GM_MSG_ATTACK:
		{
			attack_msg ack_msg = *(attack_msg*)msg.content;
			_filters.EF_TranslateRecvAttack(msg.source, ack_msg);
			XID attacker = ack_msg.ainfo.attacker;
			//这里无需区分玩家和NPC
			//自己不允许攻击
			if(attacker.id == _parent->ID.id) return 0;
			if(!(ack_msg.target_faction & (GetFaction())))
			{
				//派系不正确则返回
				return 0;
			}
			ack_msg.is_invader =  false;
			//试着选择对象
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(msg,&ack_msg);
			return 0;
		}
		break;
		
		case GM_MSG_ENCHANT:
		{       
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TranslateRecvEnchant(msg.source, ech_msg);
			if(!ech_msg.helpful )
			{       
				XID attacker = ech_msg.ainfo.attacker;
				if(attacker.id == _parent->ID.id) return 0;
				if(!(ech_msg.target_faction & (GetFaction())))
				{       
					return 0;
				}
				((gplayer_controller*)_commander)->TrySelect(msg.source);
			}
			else
			{       
				if(!(ech_msg.attacker_faction & (GetFaction())))
				{       
					return 0;
				}
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;

		case GM_MSG_ENCHANT_ZOMBIE:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if(!ech_msg.helpful )
			{
				return 0;
			}
			if(!(ech_msg.attacker_faction & (GetFaction())))
			{
				return 0;
			}
			ech_msg.is_invader = false;
			HandleEnchantMsg(msg,&ech_msg);
			return 0;
		}
		break;

		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			//这个处理交给delivery了
		}
		return 0;
	}
	return gplayer_imp::MessageHandler(msg);
}

void bf_player_imp::OnHeartbeat(size_t tick)
{
	gplayer_imp::OnHeartbeat(tick);
	if(_turret_counter) --_turret_counter;
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	if(GetParent()->b_disconnect)
	{
		//已经被清除了,断线了等待处理中
		return;
	}
	if(_timeout >= 0)
	{
		//已经在踢出准备状态下,_timeout后一定会踢出,不会在做后面面判断
		//加入了副本盒子,一样需要踢出
		--_timeout;
		if(_timeout <= 0)
		{
			++_kickout;
		}
		if(_kickout && _timeout <= 0)
		{
			if(_kickout > 5)
			{
				LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			}
			else
			{
				LeaveAbnormalState();
				_timeout = 3;
				PlayerLogout(GMSV::PLAYER_LOGOUT_HALF,0);
			}
		}
		return;
	}
	//以下进行判断是否需要踢出玩家
	if(CheckGMPrivilege())
	{
		//gm不踢
		return;
	}
	if(!pManager->IsBattleStart())
	{
		//非正常状态
		//xx秒后踢出该玩家,发送消息
		_timeout = abase::Rand(pManager->GetKickoutTimeMin(),pManager->GetKickoutTimeMax());
		__PRINTF("非正常战场状态: %d,%d 秒后踢出玩家\n",pManager->GetBattleStatus(),_timeout);
		_runner->kickout_instance(_timeout);
		return;
	}
	if(pManager->IsBattlePreparing())
	{
		//准备状态时检测位置
		gplayer* pPlayer = GetParent();
		if(!pManager->CheckValidPreparePos(_battle_faction,pPlayer->pos))
		{
			_timeout = DEFAULT_KICKOUT_TIME;
			_runner->kickout_instance(_timeout);
			return;
		}
	} 
	//全局检测位置,防外挂
	gplayer* pPlayer = GetParent();
	if(pManager->CheckInvalidPos(pPlayer->pos))
	{
		_timeout = DEFAULT_KICKOUT_TIME;
		_runner->kickout_instance(_timeout);
		return;
	}
	if(OI_GetMafiaID() != pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER)
		&& OI_GetMafiaID() != pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER)
		&& OI_GetMafiaID() != pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT) )
	{
		_timeout = DEFAULT_KICKOUT_TIME;
		__PRINTF("玩家: %d 帮派信息不对: %d 秒后踢出\n",_parent->ID.id, _timeout );
		_runner->kickout_instance(_timeout);
		return;
	}
        if(pManager->IsBattleInfoRefresh(_battle_info_seq))
        {       
		pManager->SendBattlefieldInfo(this);
        } 
}

void bf_player_imp::PostLogin(const userlogin_t& user, char trole, char loginflag)
{                       
	//设置登出使用的原坐标
	_logout_tag = user._origin_tag;
	_logout_pos = user._origin_pos;
	_logout_pos_flag = 1;
}               

void bf_player_imp::PlayerEnterWorld()
{
	__PRINTF("玩家: %d 进入战场world %d 啦!!!\n",_parent->ID.id,_plane->GetTag());
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	if(OI_GetMafiaID() == pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER))
	{
		_battle_faction = bf_world_manager_base::BF_ATTACKER;
		_attack_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		_defense_faction = FACTION_BATTLEOFFENSE;
		//协议
		_runner->enter_battlefield(pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER),
			pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER),
			pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT),
			pManager->GetBattleID(),pManager->GetBattleEndTimestamp());
		pManager->PlayerEnter(pPlayer,bf_world_manager_base::BF_ATTACKER);
	}
	else if(OI_GetMafiaID() == pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER))
	{
		_battle_faction = bf_world_manager_base::BF_DEFENDER;
		_attack_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
		_defense_faction = FACTION_BATTLEDEFENCE;
		_runner->enter_battlefield(pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER),
			pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER),
			pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT),
			pManager->GetBattleID(),pManager->GetBattleEndTimestamp());
		pManager->PlayerEnter(pPlayer,bf_world_manager_base::BF_DEFENDER);
	}
	else if(OI_GetMafiaID() == pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT))
	{
		_battle_faction = bf_world_manager_base::BF_ATTACKER_ASSISTANT;
		_attack_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		_defense_faction = FACTION_BATTLEOFFENSE;
		_runner->enter_battlefield(pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER),
			pManager->GetMafiaID(bf_world_manager_base::BI_DEFENDER),
			pManager->GetMafiaID(bf_world_manager_base::BI_ATTACKER_ASSISTANT),
			pManager->GetBattleID(),pManager->GetBattleEndTimestamp());
		pManager->PlayerEnter(pPlayer,bf_world_manager_base::BF_ATTACKER_ASSISTANT);
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}
	pManager->SendBattlefieldInfo(this);
	EnableFreePVP(true);
	gplayer_imp::PlayerEnterWorld();
}

void bf_player_imp::PlayerLeaveWorld()
{
	__PRINTF("玩家: %d 离开战场world %d 啦!!!\n",_parent->ID.id,_plane->GetTag());
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();
	pManager->PlayerLeave(pPlayer,_battle_faction);
	_runner->leave_battlefield();
	gplayer_imp::PlayerLeaveWorld();
}

void bf_player_imp::PlayerEnterMap(int old_tag,const A3DVECTOR& old_pos)
{
	__PRINTF("战场中不应该出现切换场景\n");
	ASSERT(false);
}

void bf_player_imp::PlayerLeaveMap()
{
	__PRINTF("战场中不应该出现切换场景\n");
	ASSERT(false);
}

void bf_player_imp::OnDeath(const XID& lastattack,bool is_pariah,bool faction_battle,bool is_hostile_duel, int time)
{
	__PRINTF("战场处理死亡消息: 玩家 %d 被 %d 杀死了\n",_parent->ID.id,lastattack.id);
	gplayer* pPlayer = GetParent();
	if(pPlayer->IsZombie()) return;
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	//杀人被杀记录逻辑
	if(lastattack.IsPlayerClass() && pManager->IsBattleStart())
	{
		//如果是被人杀了,并且战场在进行中,告诉delivery谁被谁杀了
		bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
                GMSV::SendBattlefieldDeath(OI_GetMafiaID(),pPlayer->ID.id,lastattack.id,pManager->GetBattleID());
	}
	gplayer_imp::OnDeath(lastattack,false,true);
	_resurrect_hp_recover = 1.0f;
}

void bf_player_imp::GetLogoutPos(int& world_tag,A3DVECTOR& pos)
{
	world_tag = _old_world_tag;
	pos = _old_pos;
}

void bf_player_imp::SendNormalChat(char channel,const void* buf,size_t len)
{
	if(GMSV::CHAT_CHANNEL_TRADE != channel && GMSV::CHAT_CHANNEL_LOCAL != channel)
	{
		return;
	}
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	pManager->BattleChat(GetFaction(),buf,len,channel,GetParent()->ID.id);
}

bool bf_player_imp::QueryBattlefieldConstructInfo(int type,int page)
{
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	return pManager->QueryBattlefieldConstructInfo(this,type,page);
}

bool bf_player_imp::BattlefieldContribute(int res_type)
{
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	return pManager->BattlefieldContribute(this,res_type);
}

bool bf_player_imp::BattlefieldConstruct(int key,int type,int cur_level)
{
	bf_world_manager_base* pManager = (bf_world_manager_base*)_plane->GetWorldManager();
	return pManager->BattlefieldConstruct(this,key,type,cur_level);
}

int bf_player_imp::CheckUseTurretScroll()
{
	if(_turret_counter >0)
	{
		return S2C::ERR_CONTROL_TOO_MANY_TURRETS;
	}
	return 0;
}

