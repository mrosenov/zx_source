#include "transform_raid_player.h"
#include "../clstab.h"
#include "../task/taskman.h"
#include "transform_raid_world_manager.h"


DEFINE_SUBSTANCE( transform_raid_player_imp, raid_player_imp, CLS_TRANSFORM_RAID_PLAYER_IMP)

transform_raid_player_imp::transform_raid_player_imp()
{
}

transform_raid_player_imp::~transform_raid_player_imp()
{
}

int transform_raid_player_imp::MessageHandler(const MSG& msg)
{
	switch( msg.message)
	{
		case GM_MSG_GET_RAID_TRANSFORM_TASK:
		{
			int task_id = msg.param;
			PlayerTaskInterface task_if(this);
			OnTaskCheckDeliver(&task_if,task_id,0);
			return 0;
		}
		break;
	}
	return raid_player_imp::MessageHandler(msg);
}


void transform_raid_player_imp::GetRaidTransformTask()
{
	gplayer* pPlayer = GetParent();
	raid_world_manager* pManager = (raid_world_manager*)_plane->GetWorldManager();

	MSG msg;
	BuildMessage( msg, GM_MSG_GET_RAID_TRANSFORM_TASK, XID(GM_TYPE_MANAGER,pManager->GetWorldTag()), 
			pPlayer->ID, pPlayer->pos, _death_count );
	gmatrix::SendWorldMessage( pManager->GetWorldTag(), msg );
}

void transform_raid_player_imp::PlayerEnter()
{
	raid_player_imp::PlayerEnter();
	transform_raid_world_manager * pManager = (transform_raid_world_manager *)GetWorldManager();
	pManager->SyncTransFormRaidInfo();
}
