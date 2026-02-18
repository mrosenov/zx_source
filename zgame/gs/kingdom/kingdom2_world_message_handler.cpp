#include "kingdom2_world_message_handler.h"

int kingdom2_world_message_handler::HandleMessage( const MSG& msg )
{
	switch( msg.message )
	{
		case GM_MSG_KINGDOM_SYNC_KEY_NPC:
		{
			msg_kingdom_sync_key_npc key_npc = *(msg_kingdom_sync_key_npc*)msg.content; 
			((kingdom2_world_manager*)_manager)->OnSyncKeyNPCInfo(key_npc.cur_hp, key_npc.max_hp, key_npc.hp_add, key_npc.hp_dec);
		}
		break;
		

		default:
			return global_world_message_handler::HandleMessage(msg);
	}
	return 0;
}

