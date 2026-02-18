#ifndef __ONLINEGAME_GS_TRANSFORM_RAID_PLAYER_H
#define __ONLINEGAME_GS_TRANSFORM_RAID_PLAYER_H

#include "raid_player.h"

class transform_raid_player_imp : public raid_player_imp
{
public:
	DECLARE_SUBSTANCE(transform_raid_player_imp);
	transform_raid_player_imp();

	virtual ~transform_raid_player_imp();
	virtual int MessageHandler(const MSG& msg);
	virtual void GetRaidTransformTask();
	virtual void PlayerEnter();
};

#endif
