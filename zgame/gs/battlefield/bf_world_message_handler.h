#ifndef __ONLINEGAME_GS_BF_WORLD_MESSAGE_HANDLER_H__ 
#define __ONLINEGAME_GS_BF_WORLD_MESSAGE_HANDLER_H__ 

#include "bf_world_manager.h"

class bf_world_message_handler : public global_world_message_handler
{
public:
	bf_world_message_handler(bf_world_manager_base* man,world* plane):
		global_world_message_handler(man,plane) { }

	virtual ~bf_world_message_handler() { }
	
	virtual int HandleMessage(const MSG& msg);
};

#endif
