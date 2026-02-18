#include <stdio.h>
#include "cs_team_player.h"
#include "../actsession.h"
#include "../clstab.h"
#include "bg_world_manager.h"
#include <vector>
#include "../skill_filter.h"
#include "../cooldowncfg.h"
#include "../invincible_filter.h"


DEFINE_SUBSTANCE(cs_team_player_imp,  bg_player_imp,  CLS_CS_TEAM_PLAYER_IMP )

cs_team_player_imp::cs_team_player_imp()
{
	_is_in_combat_zone = false;
	_revive_counter = 0;	
	_cs_team_info_seq = -1;
}

cs_team_player_imp::~cs_team_player_imp()
{
	_is_in_combat_zone = false;
	_revive_counter = 0;
	_cs_team_info_seq = -1;
}


int 
cs_team_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length )
			{
				++_kill_count;
				gplayer* pPlayer = GetParent();
				bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
				MSG msg_send;
				BuildMessage( msg_send, GM_MSG_MODIFIY_BATTLE_KILL, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
					pPlayer->ID, pPlayer->pos, _kill_count );
				gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );
				return 0;
			}
		}
		break;


		case GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER:
		{
			_kill_count += msg.param;
			_death_count += msg.param2;
		}
		break;


		case GM_MSG_ROUND_START_IN_BATTLE:
		{
			if( !RoundStart() )
			{
				gplayer* pPlayer = GetParent();
				__PRINTF("小场开始！玩家跳地图失败 player->ID.id = %d\n", pPlayer->ID.id);
			}
		}
		break;


		case GM_MSG_ROUND_END_IN_BATTLE:
		{
			if( !RoundEnd() )
			{
				gplayer* pPlayer = GetParent();
				__PRINTF("小场结束！玩家跳地图失败 player->ID.id = %d\n", pPlayer->ID.id);
			}
		}
		break;

				
		//禁止多人骑乘	
		case GM_MSG_PLAYER_LINK_RIDE_INVITE: 
		case GM_MSG_PLAYER_LINK_RIDE_INV_REPLY:
	
		//禁止相依相偎
		case GM_MSG_PLAYER_BIND_REQUEST:
		case GM_MSG_PLAYER_BIND_INVITE:
		case GM_MSG_PLAYER_BIND_REQ_REPLY:
		case GM_MSG_PLAYER_BIND_INV_REPLY:		
		{
			return 0;
		}
		break;
	}

	return bg_player_imp::MessageHandler(msg);	
}

void 
cs_team_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsZombie() ) return;
	__PRINTF( "战场处理死亡消息: 玩家 %d 被 %d 杀死了, is_pariah=%d, faction_battle=%d\n",
			_parent->ID.id, lastattack.id, is_pariah, faction_battle );
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	
	// 杀人被杀记录逻辑
	if( lastattack.IsPlayerClass() && pManager->IsRoundRunning() )
	{
		// 如果是被人杀了，并且战场在进行中
		++_death_count;
		SendTo<0>( GM_MSG_KILL_PLAYER_IN_BATTLEGROUND, lastattack, 0);
		MSG msg;
		BuildMessage( msg, GM_MSG_MODIFIY_BATTLE_DEATH, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, _death_count );
		gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg );
		
		if(_is_in_combat_zone)
		{
			pManager->UpdateRoundScore(_battle_faction, -1);
			_is_in_combat_zone = false;
		}
	}
	
	gplayer_imp::OnDeath( lastattack, is_pariah, true);
	_resurrect_hp_recover = 1.0f;

	DisableAttackInBattle();
}


void 
cs_team_player_imp::HandleDeath()
{
	if(IsDead())
	{
		++_revive_counter;
		if(_revive_counter >= REVIVE_FORCE_COUNTER)
		{
			//强制复活
			_revive_counter = 0;
			ClearSession();
			session_resurrect_in_town *pSession= new session_resurrect_in_town(this);
			pSession->SetExpReduce(0);
			AddStartSession(pSession);

			_is_in_combat_zone = false;
		}
	}
}	


void 
cs_team_player_imp::OnHeartbeat( size_t tick )
{
	bg_player_imp::OnHeartbeat(tick);

	GetCrossServerTeamBattleInfo();
}


void 
cs_team_player_imp::PlayerEnter()
{
	if(CheckGMPrivilege() )
	{
		_runner->set_invisible(true);	
		gplayer * pPlayer = GetParent();
		pPlayer->invisible = true;
		_runner->toggle_invisible(0);
	}
	
	EnableFreePVP( true );
	//回收战场专用物品
	RecycleBattleItem();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	gplayer* pPlayer = GetParent();

	SetDisAttackCount(0);
	if( BF_ATTACKER == _battle_faction )
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_OFFENSE );
		_attack_faction = FACTION_BATTLEDEFENCE;
		_defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
		DisableAttackInBattle();
	}
	else if( BF_DEFENDER == _battle_faction ) 
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_DEFENCE );
		_attack_faction = FACTION_BATTLEOFFENSE;
		_defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
		DisableAttackInBattle();
	}
	else if( BF_VISITOR == _battle_faction ) 
	{
		pPlayer->SetObjectState( gactive_object::STATE_BATTLE_VISITOR);
		SetMeleeSealMode(true);
		SetDietSealMode(true);
		SetSilentSealMode(true);
		_attack_faction = 0;
		_defense_faction = 0;
		_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, 7200));
	}
	else if( BF_MELEE == _battle_faction ) 
	{
		_attack_faction = 0;
		_defense_faction = 0xFFFFFFFF;
	}
	else
	{
		_attack_faction = 0;
		_defense_faction = 0;
	}

	A3DVECTOR temp(0,0,0);
	int tag;
	if(BF_VISITOR == _battle_faction || BF_NONE == _battle_faction)
	{
		pManager->GetRandomEntryPos(temp, _battle_faction);
	}
	else
	{
		pManager->GetRebornPos(this, temp, tag);
	}
	_parent->pos = temp;
	pManager->PlayerEnter( pPlayer, _battle_faction );

	// 通知客户端
	_runner->enter_battleground( _battle_faction, (unsigned char)(pManager->GetBattleType()), pManager->GetBattleID(), pManager->GetBattleEndstamp() );

	//发送战场信息
	GetCrossServerTeamBattleInfo();
}


void
cs_team_player_imp::PlayerLeave()
{
	EnableAttackInBattle();
	//ASSERT(0 == _disable_attack_count);

	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(_is_in_combat_zone)
	{
		pManager->UpdateRoundScore(_battle_faction, -1);
		_is_in_combat_zone = false;
	}
	bg_player_imp::PlayerLeave();
}


void 
cs_team_player_imp::GetBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	unsigned char attacker_building_left;
	unsigned char defender_building_left;
	std::vector<S2C::CMD::battle_info::player_info_in_battle> info;
	if( pManager->GetCommonBattleInfo( _battle_info_seq, attacker_building_left, defender_building_left, info ) )
	{
		if( info.size() )
		{
			_runner->battle_info( attacker_building_left, defender_building_left,
				(const char*)&info[0], sizeof(S2C::CMD::battle_info::player_info_in_battle) * info.size() );
		}
	}
}


void 
cs_team_player_imp::GetCrossServerTeamBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	S2C::CMD::crossserver_team_battle_info info;
	
	if( pManager->GetCrossServerTeamBattleInfo(_cs_team_info_seq, info, _history_info) )
	{
		int size = sizeof(S2C::CMD::crossserver_team_battle_info::history_score) * _history_info.size(); 
		_runner->crossserver_team_battle_info(info, (const char*)&_history_info[0], size);
	}
}


bool 
cs_team_player_imp::RoundStart()
{
	if(BF_ATTACKER == _battle_faction || BF_DEFENDER == _battle_faction)	
	{
		EnableAttackInBattle();
		//ASSERT( 0 == _disable_attack_count );

		bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
		if(NULL == pManager)
		{
			return false;
		}

		A3DVECTOR pos;
		pManager->GetRandomEntryPos(pos, _battle_faction);
		PlayerGoto(pos);

		//加无敌filter
		int timeout = g_timer.get_systime() - pManager->GetRoundStartstamp();
		if(timeout >= 0 && timeout < INVICIBLE_TIME)
		{
			_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, INVICIBLE_TIME - timeout));
		}

		//ASSERT(false == _is_in_combat_zone);
		_is_in_combat_zone = true;
		pManager->UpdateRoundScore(_battle_faction, 1);
	}

	return true;
}


bool
cs_team_player_imp::RoundEnd()
{
	if(BF_ATTACKER == _battle_faction || BF_DEFENDER == _battle_faction)	
	{
		DisableAttackInBattle();

		if(_is_in_combat_zone)
		{
			bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
			if(NULL == pManager)
			{
				return false;
			}

			A3DVECTOR pos;
			int tag;
			pManager->GetRebornPos(this, pos, tag);
			PlayerGoto(pos);

			_is_in_combat_zone = false;
		}
	}

	return true;
}


void
cs_team_player_imp::SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data , size_t dsize )
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(pManager == NULL) return;

	pManager->BattleFactionSay(GetFaction(), buf, len, channel, aux_data, dsize, _chat_emote,  _parent->ID.id);	
}


void 
cs_team_player_imp::HandleBattleResult()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// 判断是否需要发送结果,给奖励
	if( !_battle_result_sent && pManager->GetBattleResult() )
	{
		_runner->battle_result(pManager->GetBattleResult(), pManager->GetAttackerScore(), pManager->GetDefenderScore());
		_battle_result_sent = true;
	}
}


bool 
cs_team_player_imp::IsInBattleCombatZone()
{
	return _is_in_combat_zone;
}


void 
cs_team_player_imp::DisableAttackInBattle()
{
	_disable_attack_count++;
	SetMeleeSealMode(true);
	SetSilentSealMode(true);
}


void
cs_team_player_imp::EnableAttackInBattle()
{
	int i = 0;
	for( i = _disable_attack_count; i > 0; i-- )
	{
		SetMeleeSealMode(false);
		SetSilentSealMode(false);
	}
	SetDisAttackCount(i);
}


void 
cs_team_player_imp::DumpBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	unsigned char attacker_building_left;
	unsigned char defender_building_left;
	std::vector<S2C::CMD::battle_info::player_info_in_battle> info;
	if( pManager->GetCommonBattleInfo( _battle_info_seq, attacker_building_left, defender_building_left, info ) )
	{
		for(size_t i = 0; i < info.size(); ++i)
		{
			char buf[512];
			sprintf(buf, "id=%d, faction=%d, kill=%d, death=%d",
				info[i].id, info[i].battle_faction, info[i].kill, info[i].death);	
			this->Say(buf);
		}	
	}
}

bool 
cs_team_player_imp::UseItem(item_list & inv, int inv_index, int where, int item_type)
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return false;
	}

	if( pManager->IsForbiddenItem(item_type) )
	{
		return false;
	}

	item & it = inv[inv_index];
	bool bRst = it.IsBroadcastUseMsg();
	int count = 0;
	bool dolog = it.NeedUseLog();
	int guid1 = it.guid.guid1;
	int guid2 = it.guid.guid2;
	int rst = inv.UseItem(inv_index,this,count);
	if(rst >= 0)
	{
		ASSERT(rst == item_type);
		_runner->use_item(where,inv_index, item_type,count);
		if(bRst) _runner->use_item(item_type);
		if(count > 0 )
		{
			UseItemLog(item_type, guid1,guid2, count);
			if(dolog) GLog::log(GLOG_INFO,"用户%d使用%d个物品%d", _parent->ID.id, count, item_type);
		}

		
		return true;
	}
	return false;
}

bool 
cs_team_player_imp::UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size)
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(NULL == pManager)
	{
		return false;
	}

	if( pManager->IsForbiddenItem(item_type) )
	{
		return false;
	}

	item & it = inv[inv_index];
	if(it.type != item_type) return false;
	bool bBroadcast = it.IsBroadcastUseMsg();
	bool bBCArg = it.IsBroadcastArgUseMsg();
	int count = 0;
	bool dolog = it.NeedUseLog();
	int guid1 = it.guid.guid1;
	int guid2 = it.guid.guid2;
	int rst = inv.UseItemWithArg(inv_index,this,count,arg, arg_size);
	if(rst >= 0)
	{
		_runner->use_item(where,inv_index, item_type,count,arg,arg_size);
		if(bBroadcast) 
		{
			if(bBCArg)
			{
				_runner->use_item(item_type, arg, arg_size);
			}
			else
			{
				_runner->use_item(item_type);
			}
		}
		if(count > 0 )
		{
			UseItemLog(item_type, guid1,guid2, count);
			if(dolog) GLog::log(GLOG_INFO,"用户%d使用%d个物品%d", _parent->ID.id, count, item_type);
		}
		
		return true;
	}
	return false;
}


