/*
 * FILE: kf_player.cpp 
 *
 * DESCRIPTION: killing field player implementation 
 *
 * CREATED BY: lianshuming, 2009/04/13
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#include <stdio.h>
#include "kf_player.h"
#include "../actsession.h"
#include "../clstab.h"
#include "bg_world_manager.h"
#include "arandomgen.h"
#include "../task/taskman.h"
#include "../cooldowncfg.h"

DEFINE_SUBSTANCE(kf_player_imp,  bg_player_imp,  CLS_KF_PLAYER_IMP )

int kf_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_PLAYER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length )
			{
				++_kill_count;
				++_con_kill;
				int score = GetKillScoreByRank(msg.param) + GetExtScoreByConKill(_con_kill);
				UpdateScore(score, GM_MSG_MODIFIY_BATTLE_KILL, _kill_count);
				return 0;
			}
		}
		break;

		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length)
			{
				int monster_id = msg.param;
				int score = GetScoreByKillMonster(monster_id);
				UpdateScore(score); 
				return 0;
			}	
		}
		break;
	}
	return bg_player_imp::MessageHandler(msg);	

}

void kf_player_imp::HandleBattleResult()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// 判断是否需要发送结果,给奖励
	if( !_battle_result_sent && pManager->GetBattleResult() )
	{
		gplayer* pPlayer = GetParent();
		if( ( BATTLE_ATTACKER_WIN == pManager->GetBattleResult() && pPlayer->IsBattleOffense() ) ||
	            ( BATTLE_DEFENDER_WIN == pManager->GetBattleResult() && pPlayer->IsBattleDefence() ) )
		{
			int score = GetWinExtScore();
			UpdateScore(score);
		}

		//为了能够取得准确的数据，晚一点给客户端发
		_update_info_counter = abase::Rand(3, 6); 

		int task_id = GetTaskIDByRank();	
		if(task_id > 0)
		{
			PlayerTaskInterface task_if(this);
			//战场颁发奖励任务
			if(OnTaskCheckDeliver(&task_if,task_id,0))
			{
			}
		}
		_battle_result_sent = true;
	}
}

void kf_player_imp::OnHeartbeat( size_t tick )
{
	bg_player_imp::OnHeartbeat(tick);
	if(_timeout > 0 && _update_info_counter > 0) 
	{
		--_update_info_counter;
		if(_update_info_counter < 0)
		{
			GetKillingFieldInfo();
		}
	}	
}


void kf_player_imp::HandleDeath()
{
	if(IsDead())
	{
		++_revive_counter;
		if(_revive_counter >= REVIVE_FORCE_TIME)
		{
			//强制复活
			_revive_counter = 0;
			ClearSession();
			session_resurrect_in_town *pSession= new session_resurrect_in_town(this);
			pSession->SetExpReduce(0);
			AddStartSession(pSession);
		}

	}
	else
	{
		_revive_counter = 0;
		if(_revive_punish_counter > 0) --_revive_punish_counter;	

		if(_revive_clean_punish > 0)
		{
			--_revive_clean_punish;
			if(_revive_clean_punish <= 0)
			{
				_revive_timeout = REVIVE_MIN_TIMEOUT;	
			}	
		}
	}
}	

void kf_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
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
		_con_kill = 0;
		SendTo<0>( GM_MSG_KILL_PLAYER_IN_BATTLEGROUND, lastattack, _rank);
		UpdateScore(0, GM_MSG_MODIFIY_BATTLE_DEATH, _death_count);
		
		//死亡自然掉落
		HandleDeathDrop();

		//死亡掠夺掉落
		HandleDeathRobDrop();
	}
	
	if(_revive_punish_counter >  0)
	{
		_revive_timeout += 5;
		if(_revive_timeout > REVIVE_MAX_TIMEOUT ) _revive_timeout = REVIVE_MAX_TIMEOUT;		
	}
	_revive_punish_counter = REVIVE_PUNISH_COUNTER; 
	_revive_clean_punish = REVIVE_CLEAN_PUNISH;

	gplayer_imp::OnDeath( lastattack, is_pariah, true, false, _revive_timeout);
	_resurrect_hp_recover = 1.0f;

}


//死亡自然掉落
void kf_player_imp::HandleDeathDrop()
{
	int drop_id = GetDeathDropItemID();
	if(drop_id != -1)
	{
		DATA_TYPE datatype;
		const void * dataptr = gmatrix::GetDataMan().get_data_ptr(drop_id, ID_SPACE_ESSENCE, datatype);
		int name_id = 0;
		if(dataptr != NULL && datatype == DT_SPECIAL_NAME_ITEM_ESSENCE)
		{
			name_id = _parent->ID.id;
		}
		
		int drop_list[32];
		drop_list[0] = 0;
		drop_list[1] = 0;
		drop_list[2] = 0;
		drop_list[3] = 1;
		drop_list[4] = drop_id;
		MSG msg;
		BuildMessage2( msg, GM_MSG_PRODUCE_MONSTER_DROP, XID( GM_TYPE_PLANE, _plane->GetTag() ),
				XID( 0, 0 ) , _parent->pos, name_id, GetBattleFaction(), drop_list, (1 + 4) * sizeof(int) );
		gmatrix::SendPlaneMessage( _plane, msg );
	}	


}

	
//死亡掠夺掉落
void kf_player_imp::HandleDeathRobDrop()
{
	std::vector<player_template::deprive_item> _deprive_item;
	int map_id = _plane->GetWorldIndex();

	if(!player_template::GetDeprivedItemList(map_id, _deprive_item))
	{
		return;
	}	

	for(size_t i=0; i < _deprive_item.size(); ++i)
	{
		if(_deprive_item[i].id == 0 || _deprive_item[i].max_num <=0 ) continue;

		size_t drop_num = abase::Rand(0, _deprive_item[i].max_num);
		for(size_t j=0; j < _inventory.Size() && drop_num> 0; ++j)
		{
			//只有可被回收的物品才能被掠夺掉落
			if(_inventory[j].type != -1 && _inventory[j].type == (int)_deprive_item[i].id && _inventory[j].CanRecycle())
			{
				//这个位置的物品足够掉落，那就掉落吧
				if(_inventory[j].count >= drop_num)
				{
					ThrowInvItem( j, drop_num, false, S2C::DROP_TYPE_DEATH );	
					break;
				}
				//不够掉落的话能掉一个就是一个吧
				else
				{
					ThrowInvItem( j, _inventory[j].count, false, S2C::DROP_TYPE_DEATH );
					drop_num -= _inventory[j].count;
				}
			}
		}
	}
}

void kf_player_imp::SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data , size_t dsize )
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if(pManager == NULL) return;

	pManager->BattleFactionSay(GetFaction(), buf, len, channel, aux_data, dsize, _chat_emote,  _parent->ID.id);	
}

void kf_player_imp::GetBattleInfo()
{
	if(!CheckCoolDown(COOLDOWN_INDEX_GETBATTLEINFO)) return;
	SetCoolDown(COOLDOWN_INDEX_GETBATTLEINFO, GETBATTLEINFO_COOLDOWN_TIME);
	GetKillingFieldInfo();
}


void kf_player_imp::GetKillingFieldInfo()
{	
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	int attacker_score;
	int defender_score;
	int attacker_count;
	int defender_count;

	std::vector<S2C::CMD::killingfield_info::player_info_in_killingfield> info;

	if(pManager->GetKillingFieldInfo(_battle_info_seq, attacker_score, defender_score,attacker_count, defender_count, info))
	{
		if(info.size() > 0)
		{
			_runner->killingfield_info(attacker_score, defender_score, attacker_count, defender_count, info.size(), (const char*)&info[0]);
		}	
		_postion = GetPostion();
		SendClientSelfInfo();
	}
}

int kf_player_imp::GetPostion()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	return pManager->GetPostion(GetParent()->ID.id, GetScore());
}

void kf_player_imp::SyncBattleInfo(int msg_id, int count)
{
	gplayer* pPlayer = GetParent();
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	MSG msg_send;
	player_battle_info info(_battle_faction, _killingfield_score, _kill_count, _death_count, _rank, _con_kill);
	BuildMessage( msg_send, msg_id, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, count, &info, sizeof(info) );
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg_send );

}

void kf_player_imp::SendClientSelfInfo()
{
	_runner->self_killingfield_info(_killingfield_score, _rank, _con_kill, _postion);
}

void kf_player_imp::PlayerEnter()
{
	_killingfield_score = 0;
	bg_player_imp::PlayerEnter();
	gplayer * pPlayer = GetParent();	
	pPlayer->object_state |= gactive_object::STATE_IN_KILLINGFIELD;
	pPlayer->rank = _rank;
}

void kf_player_imp::PlayerLeave()
{
	_killingfield_score = 0;
	gplayer * pPlayer = GetParent();	
	pPlayer->object_state &= ~gactive_object::STATE_IN_KILLINGFIELD;
	pPlayer->rank = 0;
	bg_player_imp::PlayerLeave();
}

void kf_player_imp::ChangeKillingfieldScore(int value)
{
	UpdateScore(value);
}

void kf_player_imp::ResetKillingfieldScore()
{
	UpdateScore(-_killingfield_score);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
void kf_player_imp::DumpBattleSelfInfo()
{
	char buf[512];
	sprintf(buf, "score =%d, faction=%d, rank=%d, con_kill=%d, kill_count=%d, death_count = %d",
		 GetScore(), GetBattleFaction(), GetRank(), GetConKill(), _kill_count, _death_count); 
	this->Say(buf);
}
	

void kf_player_imp::DumpBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	int attacker_score;
	int defender_score;
	int attacker_count;
	int defender_count;

	std::vector<S2C::CMD::killingfield_info::player_info_in_killingfield> info;
	if(pManager->GetKillingFieldInfo(_battle_info_seq, attacker_score, defender_score,attacker_count, defender_count, info))
	{
		char buf[512];
		sprintf(buf, "attacker_score = %d, defender_score = %d, attacker_count = %d, defender_count = %d", 
				attacker_score, defender_score, attacker_count, defender_count);

		this->Say(buf);

		for(size_t i = 0; i < info.size() && i < 5; ++i)
		{
			char buf2[512];
			sprintf(buf2, "[TOP %d]: id=%d, faction=%d, score=%d, rank=%d, conKill=%d", 
					i, info[i].id, info[i].faction, info[i].score, info[i].rank, info[i].conKill);
			this->Say(buf2);
		}
	}
}

void kf_player_imp::DebugIncBattleScore(int inc)
{
	UpdateScore(inc);
}	

