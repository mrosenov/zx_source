#ifndef _TOWER_RAID_PLAYER_H_
#define _TOWER_RAID_PLAYER_H_

#include "raid_player.h"

class level_raid_player_imp : public raid_player_imp
{
private:
	bool _final_award_deliveried;
	bool _level_award_deliveried[10];

public:
	DECLARE_SUBSTANCE(level_raid_player_imp);
	level_raid_player_imp();

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time);
	virtual void OnUsePotion(char type, int value);
	virtual void OnGatherMatter(int matter_tid, int cnt);
	virtual void GetRaidLevelReward(int level);
	virtual void GetRaidLevelResult();
	virtual int MessageHandler( const MSG& msg );
	virtual void PlayerEnter();  // Youshuang add
};

#endif /*_TOWER_RAID_PLAYER_H_*/
