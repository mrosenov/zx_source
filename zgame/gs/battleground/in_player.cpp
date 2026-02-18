#include "in_player.h"
#include "../clstab.h"
#include "bg_world_manager.h"
#include "../task/taskman.h"
#include "../cooldowncfg.h"

DEFINE_SUBSTANCE(in_player_imp,  bg_player_imp,  CLS_IN_PLAYER_IMP )

int in_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KILL_MONSTER_IN_BATTLEGROUND:
		{
			if( 0 == msg.content_length)
			{
				int score = GetScoreByKillMonster(msg.param);
				if(score > 0)
				{
					bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
					pManager->OnKillMonster(score);
				}
				return 0;
			}	
		}
		break;

		case GM_MSG_BATTLE_INFO_CHANGE:
		{
			if( 0 == msg.content_length)
			{
				if(_battle_info_seq < msg.param )
				{
					GetInstanceBattleInfo();
				}
				return 0;
			}	
		}
		break;
	}
	return bg_player_imp::MessageHandler(msg);	

}


void in_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	gplayer* pPlayer = GetParent();
	if( pPlayer->IsZombie() ) return;

	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	bool kill_much = false;
	pManager->OnPlayerDeath( lastattack.id, pPlayer->ID.id, _battle_faction, kill_much);
	gplayer_imp::OnDeath( lastattack, is_pariah, true );

}

void in_player_imp::HandleBattleResult()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	// 判断是否需要发送结果,给奖励
	if( !_battle_result_sent && pManager->GetBattleResult() )
	{
		gplayer* pPlayer = GetParent();
		bool b_win = false;
		if( ( BATTLE_ATTACKER_WIN == pManager->GetBattleResult() && pPlayer->IsBattleOffense() ) ||
	            ( BATTLE_DEFENDER_WIN == pManager->GetBattleResult() && pPlayer->IsBattleDefence() ) )
		{
			b_win = true;
		}

		int score = pManager->GetBattleTotalScore();
		int task_id = GetAwardTaskID(b_win, score);	
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


void in_player_imp::GetBattleInfo()
{
//	if(!CheckCoolDown(COOLDOWN_INDEX_GETBATTLEINFO)) return;
//	SetCoolDown(COOLDOWN_INDEX_GETBATTLEINFO, GETBATTLEINFO_COOLDOWN_TIME);
	GetInstanceBattleInfo();

}


void in_player_imp::GetInstanceBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	
	int cur_level;
	int level_score;
	int level_time_score;
	int monster_score;
	int death_penalty_score;
	std::vector<char> info;

	if(pManager->GetInstanceBattleInfo(_battle_info_seq, cur_level, level_score, level_time_score, 
				monster_score, death_penalty_score, info))
	{
		if(info.size() > 0)
		{
			_runner->instance_info(cur_level, info.size(), (const char*)&info[0]);
			_runner->self_instance_info(level_score, monster_score, level_time_score, death_penalty_score);
		}	

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void in_player_imp::DumpBattleSelfInfo()
{
}
	
void in_player_imp::DumpBattleInfo()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();

	int cur_level;
	int level_score;
	int level_time_score;
	int monster_score;
	int death_penalty_score;
	std::vector<char> info;

	if(pManager->GetInstanceBattleInfo(_battle_info_seq, cur_level, level_score, level_time_score, 
				monster_score, death_penalty_score, info))
	{
		char buf[512];
		int total_score = level_score + monster_score + level_time_score + death_penalty_score;
		sprintf(buf, "level_score =%d, monster_score=%d, level_time_score=%d, death_score=%d, total_score=%d",
				level_score, monster_score, level_time_score, death_penalty_score, total_score); 
		this->Say(buf);

		if(info.size() > 0)
		{
			for(size_t i = 0; i < info.size(); ++i)
			{
				char buf2[512];
				sprintf(buf2, "level = %d, level_status=%d", i, info[i]);
				this->Say(buf2);
			}
		}	

	}

}

void in_player_imp::DebugIncBattleScore(int inc)
{
}	


