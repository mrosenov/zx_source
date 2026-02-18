#include "mafia_battle_raid_world_manager.h"
#include "raid_player.h"
#include <factionlib.h>


mafia_battle_raid_world_manager::mafia_battle_raid_world_manager()
{
	_player_set_lock = 0;
}

mafia_battle_raid_world_manager::~mafia_battle_raid_world_manager()
{
}

void mafia_battle_raid_world_manager::OnCreate()
{
	GNET::SendGFactionPkRaidFightNotify(GetRaidID(), GetRaidRoomID(), _end_timestamp, (char)true);	
}

void mafia_battle_raid_world_manager::OnClose()
{
	GNET::SendGFactionPkRaidFightNotify(GetRaidID(), GetRaidRoomID(), _end_timestamp, (char)false);	
}

void mafia_battle_raid_world_manager::PlayerEnter( gplayer* pPlayer, int faction, bool reenter)
{
	if(RF_VISITOR == faction) return;

	raid_world_manager::PlayerEnter(pPlayer, faction, reenter);
	spin_autolock keeper(_player_set_lock);

	int roleid= pPlayer->ID.id;	
	if(_player_set.find(roleid) == _player_set.end())
	{
		_player_set.insert(roleid);
	}
}


bool mafia_battle_raid_world_manager::IsFirstTimeEnter(gplayer * pPlayer)
{
	spin_autolock keeper(_player_set_lock);
	int roleid = pPlayer->ID.id;
	return _player_set.find(roleid) == _player_set.end();
}

void mafia_battle_raid_world_manager::Reset()
{
	raid_world_manager::Reset();
	_player_set_lock = 0;
	_player_set.clear();
}

