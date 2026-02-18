#ifndef __ONLINEGAME_GS_BG_WORLD_MESSAGE_HANDLER_H__ 
#define __ONLINEGAME_GS_BG_WORLD_MESSAGE_HANDLER_H__ 


#include "../world.h"
#include "raid_world_manager.h"

class raid_world_message_handler: public global_world_message_handler
{
public:
	raid_world_message_handler( raid_world_manager* man, world* plane ):
		global_world_message_handler( man, plane ) { }
	virtual ~raid_world_message_handler() { }
	virtual int HandleMessage(const MSG& msg);
};

#endif
