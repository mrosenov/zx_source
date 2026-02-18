#ifndef __ONLINEGAME_GS_KINGDOM2_WORLD_MESSAGE_HANDLER_H__ 
#define __ONLINEGAME_GS_KINGDOM2_WORLD_MESSAGE_HANDLER_H__ 

#include "../world.h"
#include "kingdom2_world_manager.h"

class kingdom2_world_message_handler : public global_world_message_handler
{
public:
	kingdom2_world_message_handler(kingdom2_world_manager * man, world * plane):
		global_world_message_handler(man, plane){}

	virtual ~kingdom2_world_message_handler(){}
	virtual int HandleMessage(const MSG & msg);
};


#endif
