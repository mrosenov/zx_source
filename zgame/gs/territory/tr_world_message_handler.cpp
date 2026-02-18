
#include "tr_world_message_handler.h"

int tr_world_message_handler::HandleMessage( const MSG& msg )
{
	switch( msg.message )
	{
		default:
			return global_world_message_handler::HandleMessage(msg);
	}
	return 0;
}

