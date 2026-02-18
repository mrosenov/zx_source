#include "level_raid_player.h"
#include "level_raid_world_manager.h"
#include "../clstab.h"
#include <glog.h>
#include <stdio.h>

DEFINE_SUBSTANCE(level_raid_player_imp, raid_player_imp, CLS_LEVEL_RAID_PLAYER_IMP)

level_raid_player_imp::level_raid_player_imp()
{
	_final_award_deliveried = false;
	memset(_level_award_deliveried, 0, sizeof(_level_award_deliveried));
}

void level_raid_player_imp::OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time )
{
	raid_player_imp::OnDeath(lastattack, is_pariah, faction_battle, is_hostile_duel, time);
	bool kill_much = false;
	gplayer* pPlayer = GetParent();
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	pManager->OnPlayerDeath(pPlayer, lastattack.id, pPlayer->ID.id, _battle_faction, kill_much);
}

void level_raid_player_imp::OnUsePotion(char type, int value)
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	pManager->OnUsePotion(GetParent(), type, value);
}

void level_raid_player_imp::OnGatherMatter(int matter_tid, int cnt)
{
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();
	pManager->OnGatherMatter(GetParent(), matter_tid, cnt);
}

void level_raid_player_imp::GetRaidLevelReward(int level)
{
	if(level < -1 || level > 9)
	{
		return;
	}
	level_raid_world_manager* man = (level_raid_world_manager*)_plane->GetWorldManager();
	bool final = false;
	bool level_result[10];
	memset(level_result, 0, sizeof(level_result));
	man->GetRaidLevelResult(final, level_result);
	if((level == -1 && (!final || _final_award_deliveried)))
	{
		return;
	}	
	else if(!level_result[level] || _level_award_deliveried[level])
	{
		return;
	}

	int configTid = man->GetRaidLevelConfigTid();
	
	DATA_TYPE dt;
	const CHALLENGE_2012_CONFIG& config = *(const CHALLENGE_2012_CONFIG*) gmatrix::GetDataMan().get_data_ptr(configTid, ID_SPACE_CONFIG, dt);
	if(dt != DT_CHALLENGE_2012_CONFIG || &config == NULL)
	{
		__PRINTF("无效的副本条件检测模板: %d\n", configTid);
		GLog::log( GLOG_INFO, "副本条件检测模板不存在level_config_tid=%d\n", configTid);
		return;
	}	
	int item_id = (level == -1 ? config.main_config.award_item_id : config.lv_config[level].award_item_id);
	int item_cnt = 1;
	if(GetInventory().GetEmptySlotCount() < (size_t)item_cnt)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	
	const item_data * pItem = (const item_data*)gmatrix::GetDataMan().get_item_for_sell(item_id);
	if(pItem)
	{
		int rst =_inventory.Push(*pItem, item_cnt,0);
		if(rst >= 0 && item_cnt == 0)
		{
			_inventory[rst].InitFromShop(this,ITEM_INIT_TYPE_TASK);
			int state = item::Proctype2State(pItem->proc_type);
			_runner->obtain_item(item_id,0,1,_inventory[rst].count, 0,rst,state);
			if(level == -1)
			{
				_final_award_deliveried = true;
			}
			else
			{
				_level_award_deliveried[level] = true;
			}
		}
	}
}

void level_raid_player_imp::GetRaidLevelResult()
{
	level_raid_world_manager* man = (level_raid_world_manager*)_plane->GetWorldManager();
	bool final = false;
	bool level_result[10];
	memset(level_result, 0, sizeof(level_result));
	man->GetRaidLevelResult(final, level_result);
	_runner->raid_level_result(final, level_result, _final_award_deliveried, _level_award_deliveried);
}


int level_raid_player_imp::MessageHandler( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_LEVEL_RAID_START:
		{
			char level = (char)msg.param;
			_runner->raid_level_start(level);
			return 0;
		}
		break;
		case GM_MSG_LEVEL_RAID_END:
		{
			char level = (char)msg.param;
			bool result = msg.param2;
			_runner->raid_level_end(level, result);
			return 0;
		}
		break;
		case GM_MSG_LEVEL_RAID_INFO_CHANGE:
		{
			if(msg.content_length != sizeof(msg_level_raid_info))
			{
				return 0;
			}
			msg_level_raid_info& info = *(msg_level_raid_info*)msg.content;
			_runner->raid_level_info(info.level, info.matter_cnt, info.start_time);
			return 0;
		}
		break;
	}
	return raid_player_imp::MessageHandler(msg);

}

// Youshuang add
void level_raid_player_imp::PlayerEnter()
{
        raid_player_imp::PlayerEnter();
        level_raid_world_manager* pManager = (level_raid_world_manager*)_plane->GetWorldManager();
        _runner->get_raid_boss_info( pManager->GetKilledBossList() );
}
// end
