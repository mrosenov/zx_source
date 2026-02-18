#ifndef __ONLINEGAME_GS_KINGDOM_WORLD_MESSAGE_HANDLER_H__ 
#define __ONLINEGAME_GS_KINGDOM_WORLD_MESSAGE_HANDLER_H__ 

#include "../world.h"
#include "kingdom_world_manager.h"

class kingdom_world_message_handler : public global_world_message_handler
{
public:
	kingdom_world_message_handler(kingdom_world_manager * man, world * plane):
		global_world_message_handler(man, plane){}

	virtual ~kingdom_world_message_handler(){}
	virtual int HandleMessage(const MSG & msg);
};


#endif
