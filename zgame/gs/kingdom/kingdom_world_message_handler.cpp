
#include "kingdom_world_message_handler.h"

int kingdom_world_message_handler::HandleMessage( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KINGDOM_CHANGE_KEY_NPC:
		{
			((kingdom_world_manager*)_manager)->OnChangeKeyNPCInfo(msg.param);
		}
		break;

		case GM_MSG_KINGDOM_UPDATE_KEY_NPC:
		{
			msg_kingdom_update_key_npc key_npc = *(msg_kingdom_update_key_npc*)msg.content; 
			((kingdom_world_manager*)_manager)->OnUpdateKeyNPCInfo(key_npc.cur_hp, key_npc.max_hp, key_npc.cur_op_type, key_npc.change_hp);
		}
		break;

		case GM_MSG_KINGDOM_KEY_NPC_INFO:
		{
			msg_kingdom_key_npc_info key_npc = *(msg_kingdom_key_npc_info*)msg.content;
			((kingdom_world_manager*)_manager)->OnQueryKeyNPCInfo(key_npc.cur_hp, key_npc.max_hp);
		}
		break;

		default:
			return global_world_message_handler::HandleMessage(msg);
	}
	return 0;
}

