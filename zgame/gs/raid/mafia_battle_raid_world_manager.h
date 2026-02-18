#ifndef __MAFIA_BATTLE_RAID_WORLD_MANAGER_ 
#define __MAFIA_BATTLE_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"

class mafia_battle_raid_world_manager : public raid_world_manager
{

public:
	mafia_battle_raid_world_manager();
	virtual ~mafia_battle_raid_world_manager();

	virtual void PlayerEnter( gplayer* pPlayer, int faction, bool reenter); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer

	virtual int GetRaidType() { return RT_MAFIA_BATTLE; }

	virtual void OnCreate();
	virtual void OnClose();
	virtual void Reset();

	bool IsFirstTimeEnter(gplayer * pPlayer); //是不是第一次进入
private:
	std::set<int> _player_set;
	int _player_set_lock;
};

#endif 
