#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <openssl/md5.h>
#include <common/protocol.h>
#include "../world.h"
#include "../clstab.h"
#include "cb_player.h"
#include "../usermsg.h"
#include "../actsession.h"
#include "../userlogin.h"
#include "../playertemplate.h"
#include "../serviceprovider.h"
#include <common/protocol_imp.h>
#include "../task/taskman.h"
#include "../playerstall.h"
#include "../pvplimit_filter.h"
#include <glog.h>
#include "../pathfinding/pathfinding.h"
#include "../player_mode.h"
#include "../cooldowncfg.h"
#include "../template/globaldataman.h"
#include "../petnpc.h"
#include "../item_manager.h"
#include "../netmsg.h"
#include "../mount_filter.h"
#include "bg_world_manager.h"
#include "../faction.h"
#include <vector>

DEFINE_SUBSTANCE( cb_player_imp, gplayer_imp, CLS_CB_PLAYER_IMP )

int cb_player_imp::MessageHandler( const MSG& msg )
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
				if( KILL_MUCH_NUMBER == _kill_count )
				{
					_kill_much = true;
					// 发送信息
					pManager->BroadcastBattleMsg( 1, 
						GetFaction(), 
						pManager->GetBattleID(), 
						gmatrix::Instance()->GetServerIndex(), pPlayer->ID.id, msg.source.id );
					__PRINTF( "在 %d 战场(%d线), %d 已经成为杀神,拜托谁去阻止下他的杀戮吧!\n",
						pManager->GetBattleID(), gmatrix::Instance()->GetServerIndex(), pPlayer->ID.id );
				}
			}
		}
		break;
	}

	return bg_player_imp::MessageHandler( msg );
}


void cb_player_imp::HandleBattleResult()
{
	bg_world_manager* pManager = (bg_world_manager*)_plane->GetWorldManager();
	if( !_battle_result_sent && pManager->GetBattleResult())
	{
		// 奖励
		gplayer* pPlayer = GetParent();
		size_t inc = 0;
		int item_id = 0;
		int item_num = 0;

		if( BATTLE_DRAW == pManager->GetBattleResult() )
		{
			inc = pManager->GetBattleType() == BT_COMMON ? BATTLE_REWARD_DRAW : BATTLE_REBORN_REWARD_DRAW;
			if(pManager->GetBattleType() == BT_REBORN_COMMON)
			{
				item_id = DT_REBORN_AWARD_ID;
				item_num = DT_REBORN_DRAW_AWARD_CNT;
			}	
		}
		else if( ( BATTLE_ATTACKER_WIN == pManager->GetBattleResult() && pPlayer->IsBattleOffense() ) ||
				( BATTLE_DEFENDER_WIN == pManager->GetBattleResult() && pPlayer->IsBattleDefence() ) )
		{
			// 非跨服，更新活跃度，杀怪战场胜利
			if (!IsZoneServer()) {
				EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_CB_ZHANCHANG_WIN);
			}

			inc = pManager->GetBattleType() == BT_COMMON ? BATTLE_REWARD_WIN : BATTLE_REBORN_REWARD_WIN;
			if(pManager->GetBattleType() == BT_REBORN_COMMON)
			{
				item_id = DT_REBORN_AWARD_ID;
				item_num = DT_REBORN_WIN_AWARD_CNT;
			}	
		}
		else
		{
			inc = pManager->GetBattleType() == BT_COMMON ? BATTLE_REWARD_LOST : BATTLE_REBORN_REWARD_LOST;
		}
	
		if( CheckIncBattleScore( inc ) )
		{
			GainBattleScore( inc );
			// 通知玩家新的battle_score
			_runner->gain_battle_score( inc );
		}

		// 如果是第一的话加额外分数
		if( pManager->GetBattleMaxKillerID() ==  _parent->ID.id )
		{
			inc = pManager->GetBattleType() == BT_COMMON ? BATTLE_REWARD_MAX_KILL : BATTLE_REBORN_REWARD_MAX_KILL;
			GainBattleScore( inc);
			_runner->gain_battle_score( inc);
			if(pManager->GetBattleType() == BT_REBORN_COMMON)
			{
				item_id = DT_REBORN_AWARD_ID;
				item_num += DT_REBORN_FIRST_AWARD_CNT;
			}	
		}
		
		//派发物品奖励
		if(item_id != 0 && item_num != 0)
		{
			DeliverItem(item_id, item_num, false, 0, ITEM_INIT_TYPE_TASK);   
		}

		_runner->battle_result( pManager->GetBattleResult(), 0, 0 );
		_battle_result_sent = true;

		// 同步battle_score给delivery
		SyncPlayerMafiaInfo();
	}

}

void cb_player_imp::HandleDeath()
{
	if(_revive_punish_counter > 0 && !IsDead())
	{
		--_revive_punish_counter;	
		if(_revive_punish_counter <=0)
		{
			_revive_timeout = REVIVE_MIN_TIMEOUT;	
		}
	}
}	

void cb_player_imp::OnDeath( const XID& lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time)
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
		SendTo<0>( GM_MSG_KILL_PLAYER_IN_BATTLEGROUND, lastattack, _death_count );
		MSG msg;
		BuildMessage( msg, GM_MSG_MODIFIY_BATTLE_DEATH, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
				pPlayer->ID, pPlayer->pos, _death_count );
		gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg );

		// 击杀判定
		pManager->OnPlayerDeath( lastattack.id, pPlayer->ID.id, _battle_faction, _kill_much );

		// 处理掉落逻辑
		// 银质勋章掉落逻辑
		bool bDropSilverMedal = false;
		if( ( _battle_score < 100 && _death_count <= 1 ) ||
				( _battle_score >= 100 && _battle_score <= 500 && _death_count <= 2 ) ||
				( _battle_score >= 501 && _battle_score <= 5000 && _death_count <= 3 ) ||
				( _battle_score >= 5001 && _battle_score <= 10000 && _death_count <= 4 ) ||
				( _battle_score >= 10001 && _death_count <= 5 ) )
		{
			bDropSilverMedal = true;
		}
		if( bDropSilverMedal )
		{
			int drop_list[32];
			drop_list[0] = 0;
			drop_list[1] = 0;
			drop_list[2] = 0;
			drop_list[3] = 1;
			drop_list[4] = DT_SILVER_MEDAL;
			MSG msg;
			BuildMessage( msg, GM_MSG_PRODUCE_MONSTER_DROP, XID( GM_TYPE_PLANE, _plane->GetTag() ),
					XID( 0, 0 ) , _parent->pos, 0,drop_list, (1 + 4) * sizeof(int) );
			gmatrix::SendPlaneMessage( _plane, msg );
		}

		// 铜质勋章掉落逻辑
		// 从系统掉落的逻辑
		{
			int drop_list[32];
			drop_list[0] = 0;
			drop_list[1] = 0; 
			drop_list[2] = 0;
			drop_list[3] = 1;
			drop_list[4] = DT_CUPRUM_MEDAL;
			MSG msg;
			BuildMessage( msg, GM_MSG_PRODUCE_MONSTER_DROP, XID( GM_TYPE_PLANE, _plane->GetTag() ),
					XID( 0, 0 ) , _parent->pos, 0,drop_list, (1 + 4) * sizeof(int) );
			gmatrix::SendPlaneMessage( _plane, msg );
		}
		// 从包裹里掉落的逻辑
		int drop_cuprum_count = GetDeathCuprumMedalDropCount( _kill_count );
		if( drop_cuprum_count )
		{
			// 需要从包裹里掉落
			abase::vector<int,abase::fast_alloc<> > inv_list;
			inv_list.reserve( _inventory.Size() );
			size_t i = 0;
			for( i = 0; i < _inventory.Size(); ++i )
			{
				if( DT_CUPRUM_MEDAL != _inventory[i].type ) continue;
				inv_list.push_back(i);
			}
			while( inv_list.size() && drop_cuprum_count )
			{
				int drop_index = inv_list[0];
				int drop_count = drop_cuprum_count;
				if( (int)_inventory[drop_index].count < drop_cuprum_count )
					drop_count = _inventory[drop_index].count;
				ThrowInvItem( drop_index, drop_count, false, S2C::DROP_TYPE_DEATH );
				inv_list.erase_noorder( inv_list.begin() );
				drop_cuprum_count -= drop_count;
				ASSERT( drop_cuprum_count >= 0 );
			}
		}

		//飞升战场额外掉落
		if(pManager->GetBattleType() == BT_REBORN_COMMON)
		{
			int drop_id = 0;
			if(_battle_faction & 0x01)
			{
				drop_id = (_kill_count - _death_count >= 5) ? DT_ATTACKER_DROP2 : DT_ATTACKER_DROP1;
			}	
			else if(_battle_faction & 0x02)
			{			
				drop_id = (_kill_count - _death_count >= 5) ? DT_DEFENDER_DROP2 : DT_DEFENDER_DROP1;
			}	

			if(drop_id != 0)
			{
				int drop_list[32];
				drop_list[0] = 0;
				drop_list[1] = 0; 
				drop_list[2] = 0;
				drop_list[3] = 1;
				drop_list[4] = drop_id;
				MSG msg;
				BuildMessage( msg, GM_MSG_PRODUCE_MONSTER_DROP, XID( GM_TYPE_PLANE, _plane->GetTag() ),
						XID( 0, 0 ) , _parent->pos, 0,drop_list, (1 + 4) * sizeof(int) );
				gmatrix::SendPlaneMessage( _plane, msg );
			}
		}
	}
	if(_revive_punish_counter >  0)
	{
		_revive_timeout += 10;
		if(_revive_timeout > REVIVE_MAX_TIMEOUT ) _revive_timeout = REVIVE_MAX_TIMEOUT;		
	}
	_revive_punish_counter = REVIVE_PUNISH_COUNTER; 
	gplayer_imp::OnDeath( lastattack, is_pariah, true, false, _revive_timeout);

	if(BT_REBORN_COMMON == pManager->GetBattleType())
	{
		_resurrect_hp_recover = 1.0f;
	}
}

void cb_player_imp::GetBattleInfo()
{
	GetCommonBattleInfo();
}

void cb_player_imp::GetCommonBattleInfo()
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
