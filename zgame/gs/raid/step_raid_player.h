#ifndef __ONLINEGAME_GS_STEP_RAID_PLAYER_H
#define __ONLINEGAME_GS_STEP_RAID_PLAYER_H

#include "raid_player.h"

class step_raid_player_imp : public raid_player_imp
{
public:
        DECLARE_SUBSTANCE(step_raid_player_imp);
        step_raid_player_imp();
        virtual ~step_raid_player_imp();
        virtual void PlayerEnter();
        virtual int MessageHandler(const MSG& msg);
};

#endif
