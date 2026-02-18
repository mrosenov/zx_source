#include <stdio.h>
#include "ar_player.h"
#include "../actsession.h"
#include "../clstab.h"
#include "bg_world_manager.h"
#include "../skill_filter.h"
#include "../cooldowncfg.h"
#include "../invincible_filter.h"
#include "../task/taskman.h"


DEFINE_SUBSTANCE(ar_player_imp,  bg_player_imp,  CLS_AR_PLAYER_IMP )

void ar_player_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target,attack,dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= _defense_faction;
	attack.target_faction = 0xFFFFFFFF;
}

void ar_player_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gplayer_imp::FillEnchantMsg(target,enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= _defense_faction;
	//可以给所有人加诅咒和祝福
	if(enchant.helpful)
	{
		enchant.target_faction = 0;

	}
	else
	{
		enchant.target_faction = 0xFFFFFFFF;
	}
}
	

int ar_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length )
			{
				++_kill_count;
				++_arena_score;
				_arena_score += msg.param;
				SyncBattleInfo();
				return 0;
			}
		}
		break;

		//禁止组队
		case GM_MSG_TEAM_INVITE:
		case GM_MSG_TEAM_AGREE_INVITE:
		case GM_MSG_TEAM_REJECT_INVITE:
		case GM_MSG_JOIN_TEAM:
		case GM_MSG_LEADER_UPDATE_MEMBER:
		
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



void ar_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsZombie() ) return;
	__PRINTF( "战场处理死亡消息: 玩家 %d 被 %d 杀死了, is_pariah=%d, faction_battle=%d\n",
			_parent->ID.id, lastattack.id, is_pariah, faction_battle );
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	
	// 杀人被杀记录逻辑
	if( lastattack.IsPlayerClass() && pManager->IsBattleRunning() )
	{
		// 如果是被人杀了，并且战场在进行中
		++_death_count;
		SendTo<0>( GM_MSG_KILL_PLAYER_IN_BATTLEGROUND, lastattack, _arena_score);
		MSG msg;
		BuildMessage( msg, GM_MSG_MODIFIY_BATTLE_DEATH, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, _death_count );
		gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg );

	}

	HandleDeathRobDrop();
	HandleDeathDrop();
	
	gplayer_imp::OnDeath( lastattack, is_pariah, true);
	
	_timeout = 1;
	_runner->kickout_instance( _timeout );
}

void ar_player_imp::HandleBattleResult()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// 判断是否需要发送结果,给奖励
	if( !_battle_result_sent && pManager->GetBattleResult() )
	{
		if(pManager->GetBattleResult() != 0 && pManager->GetBattleWinnerID() == _parent->ID.id)
		{
			int task_id = GetRewardTaskID();	
			if(task_id > 0)
			{
				PlayerTaskInterface task_if(this);
				//战场颁发奖励任务
				if(OnTaskCheckDeliver(&task_if,task_id,0))
				{
				}
			}
		}
		_battle_result_sent = true;
	}
}

void ar_player_imp::HandleDeathRobDrop()
{
	//掠夺掉落
	std::set<int> drop_list;
	if(!player_template::GetArenaDropList(_plane->GetWorldIndex(), drop_list))
	{
		return;
	}
	for(size_t i = 0; i < _inventory.Size(); ++i)
	{
		if(drop_list.find(_inventory[i].type) != drop_list.end())
		{
			ThrowInvItem( i, _inventory[i].count, false, S2C::DROP_TYPE_DEATH );	
		}
	}
	for(size_t j = 0; j < _equipment.Size(); ++j)
	{
		if(drop_list.find(_equipment[j].type) != drop_list.end())
		{
			ThrowEquipItem( j, false, S2C::DROP_TYPE_DEATH );	
		}
	}

}

void ar_player_imp::HandleDeathDrop()
{
	//自然掉落
	int drop_id = GetDeathDropItemID();
	if(drop_id != -1)
	{
		int drop_list[32];
		drop_list[0] = 0;
		drop_list[1] = 0;
		drop_list[2] = 0;
		drop_list[3] = 1;
		drop_list[4] = drop_id;
		MSG msg;
		BuildMessage( msg, GM_MSG_PRODUCE_MONSTER_DROP, XID( GM_TYPE_PLANE, _plane->GetTag() ),
				XID( 0, 0 ) , _parent->pos, 0, drop_list, (1 + 4) * sizeof(int) );
		gmatrix::SendPlaneMessage( _plane, msg );
	}	
}


void ar_player_imp::PlayerEnter()
{
	_commander->DenyCmd(controller::CMD_TRANSFORM);
	if(_battle_faction == BF_MELEE)
	{
		bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
		//加无敌filter
		int timeout = g_timer.get_systime() - pManager->GetBattleStartstamp();
		if(timeout > 0 && timeout < INVICIBLE_TIME)
		{
			_filters.AddFilter(new invincible_battleground_filter(this,FILTER_BATTLEGROUND_INVINCIBLE, INVICIBLE_TIME - timeout));

		}
	}
	if(IsTransformState()) PlayerStopTransform();
	bg_player_imp::PlayerEnter();
}

void ar_player_imp::PlayerLeave()
{
	_commander->AllowCmd(controller::CMD_TRANSFORM);
	_arena_score = 0;
	bg_player_imp::PlayerLeave();
}

void ar_player_imp::GetBattleInfo()
{
	if(!CheckCoolDown(COOLDOWN_INDEX_GETBATTLEINFO)) return;
	SetCoolDown(COOLDOWN_INDEX_GETBATTLEINFO, 2000);
	GetArenaInfo();
}

void ar_player_imp::GetArenaInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	int apply_count;
	int remain_count;
	std::vector<S2C::CMD::arena_info::player_info_in_arena> info;
	if(pManager->GetArenaInfo(_battle_info_seq, apply_count, remain_count, info))
	{
		_runner->arena_info(_kill_count, _arena_score, apply_count, remain_count, info.size(), (const char*)&info[0]);
	}
}

void ar_player_imp::SyncBattleInfo()
{
	gplayer* pPlayer = GetParent();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	MSG msg_send;
	int timestamp = g_timer.get_systime();
	player_battle_info info(_battle_faction, _arena_score, _kill_count, _death_count, 0, 0, timestamp);
	BuildMessage( msg_send,GM_MSG_SYNC_BATTLE_INFO , XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, 0, &info, sizeof(info) );
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );
}

//调试函数
void ar_player_imp::DumpBattleSelfInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	int apply_count;
	int remain_count;
	std::vector<S2C::CMD::arena_info::player_info_in_arena> info;
	if(pManager->GetArenaInfo(_battle_info_seq, apply_count, remain_count, info))
	{
		char buf[512];
		sprintf(buf, "kill_count=%d, score =%d, apply_count = %d, remain_count = %d", _kill_count, _arena_score, apply_count, remain_count); 
		this->Say(buf);


	}
}
