#include <stdio.h>
#include <sys/types.h>
#include "../global_manager.h"
#include "bf_player_imp.h"
#include "bf_world_message_handler.h"

int bf_world_message_handler::HandleMessage(const MSG& msg)
{
	switch( msg.message )
	{
	/*
		case GM_MSG_MODIFIY_INFO_IN_BATTLE:
		{
			ASSERT(msg.content_length == sizeof(msg_modify_info_in_battle_t));
			((bf_world_manager_base*)_manager)->HandleModifyInfo(msg.source.id,*(msg_modify_info_in_battle_t*)msg.content);
		}
		break;

*/
		default:
			return global_world_message_handler::HandleMessage(msg);
	}
	return 0;
}

