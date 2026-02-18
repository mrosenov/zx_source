#ifndef __ONLINEGAME_GS_BG_WORLD_MESSAGE_HANDLER_H__ 
#define __ONLINEGAME_GS_BG_WORLD_MESSAGE_HANDLER_H__ 

#include "../world.h"
#include "tr_world_manager.h"

class tr_world_message_handler : public global_world_message_handler
{
public:
	tr_world_message_handler(tr_world_manager * man, world * plane):
		global_world_message_handler(man, plane){}

	virtual ~tr_world_message_handler(){}
	virtual int HandleMessage(const MSG & msg);
};


#endif
