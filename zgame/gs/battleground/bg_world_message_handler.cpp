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
#include "bg_world_message_handler.h"

int bg_world_message_handler::HandleMessage( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_MODIFIY_BATTLE_DEATH:
		{
			((bg_world_manager*)_manager)->HandleModifyBattleDeath( msg.source.id, msg.param, msg.content_length, msg.content);
		}
		break;

		case GM_MSG_MODIFIY_BATTLE_KILL:
		{
			((bg_world_manager*)_manager)->HandleModifyBattleKill( msg.source.id, msg.param, msg.content_length, msg.content);
		}
		break;

		case GM_MSG_SYNC_CSFLOW_PLAYER_INFO:
		{
			((bg_world_manager*)_manager)->SyncCSFlowPlayerInfo( msg.source.id, msg.content_length, msg.content);
		}
		break;
		
		case GM_MSG_SYNC_BATTLE_INFO: 
		{
			((bg_world_manager*)_manager)->HandleSyncBattleInfo( msg.source.id, msg.param, msg.content_length, msg.content);
		}
		break;




		default:
			return global_world_message_handler::HandleMessage(msg);
	}
	return 0;
}

