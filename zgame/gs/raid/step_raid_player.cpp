#include "step_raid_player.h"
#include "../clstab.h"
#include "../task/taskman.h"
#include "step_raid_world_manager.h"

DEFINE_SUBSTANCE( step_raid_player_imp, raid_player_imp, CLS_STEP_RAID_PLAYER_IMP)

step_raid_player_imp::step_raid_player_imp()
{
}

step_raid_player_imp::~step_raid_player_imp()
{
}

void step_raid_player_imp::PlayerEnter()
{
        raid_player_imp::PlayerEnter();
        step_raid_world_manager * pManager = (step_raid_world_manager *)GetWorldManager();
        pManager->SyncStepRaidInfo();
}

int step_raid_player_imp::MessageHandler(const MSG& msg )
{
        switch ( msg.message)
        {   
                case GM_MSG_GET_STEP_RAID_TASK:
                {   
                        int task_id = msg.param;
                        PlayerTaskInterface task_if(this);
                        OnTaskCheckDeliver(&task_if,task_id,0);
			GLog::log(GLOG_INFO,"用户userid=%d,roleid=%d,十神宝窟获得任务奖励%d",_db_magic_number,_parent->ID.id,task_id);
			return 0;
                }   
                break;
        }   
        return raid_player_imp::MessageHandler(msg);
}
