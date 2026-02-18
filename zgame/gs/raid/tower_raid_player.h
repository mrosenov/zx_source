#ifndef _TOWER_RAID_PLAYER_H_
#define _TOWER_RAID_PLAYER_H_

#include "raid_player.h"

class tower_raid_player_imp : public raid_player_imp
{
public:
	DECLARE_SUBSTANCE(tower_raid_player_imp);
	tower_raid_player_imp();

	virtual bool ValidPlayer();
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void PlayerStartRandomTowerMonster(char client_idx);
	virtual void OnResurrect();
};

#endif /*_TOWER_RAID_PLAYER_H_*/
