#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <threadpool.h>
#include <conf.h>
#include <io/pollio.h>
#include <io/passiveio.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <glog.h>

#include "../template/itemdataman.h"
#include "../template/npcgendata.h"
#include "../global_manager.h"
#include "raid_world_message_handler.h"

int raid_world_message_handler::HandleMessage( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_MODIFIY_BATTLE_DEATH:
		{
			((raid_world_manager*)_manager)->HandleModifyRaidDeath( msg.source.id, msg.param, msg.content_length, msg.content);
		}
		break;

		case GM_MSG_MODIFIY_BATTLE_KILL:
		{
			((raid_world_manager*)_manager)->HandleModifyRaidKill( msg.source.id, msg.param, msg.content_length, msg.content);
		}
		break;
		
		case GM_MSG_SYNC_BATTLE_INFO: 
		{
			((raid_world_manager*)_manager)->HandleSyncRaidInfo( msg.source.id, msg.param, msg.content_length, msg.content);
		}
		break;
		
		case GM_MSG_GET_RAID_TRANSFORM_TASK:
		{
			((raid_world_manager*)_manager)->HandleTransformRaid(msg.source.id);
		}
		break;

		case GM_MSG_CAPTURE_SYNC_FLAG_POS:
		{
			((raid_world_manager*)_manager)->HandleCaptureRaidSyncPos(msg.param, msg.pos);
		}
		break;

		default:
			return global_world_message_handler::HandleMessage(msg);
	}
	return 0;
}

