#ifndef __ONLINEGAME_GS_MAFIA_BATTLE_RAID_PLAYER_H
#define __ONLINEGAME_GS_MAFIA_BATTLE_RAID_PLAYER_H

#include "raid_player.h"

class mafia_battle_raid_player_imp : public raid_player_imp
{
public:
	DECLARE_SUBSTANCE(mafia_battle_raid_player_imp);

	mafia_battle_raid_player_imp();
	virtual ~mafia_battle_raid_player_imp();

	virtual void PlayerEnter();
	virtual bool ValidPlayer();
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void PostLogin(const userlogin_t& user, char trole, char loginflag);

private:
	void DeliverBattleItem();
};

#endif 
