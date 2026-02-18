#ifndef __ONLINEGAME_GS_CH_PLAYER_IMP_H__
#define __ONLINEGAME_GS_CH_PLAYER_IMP_H__

#include "bg_player.h"

//ÃÙ’Ωø’º‰
class ch_player_imp : public bg_player_imp 
{
public:
	DECLARE_SUBSTANCE( ch_player_imp )
	ch_player_imp()	{}

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
};

#endif
