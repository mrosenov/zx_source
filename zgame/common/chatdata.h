#ifndef __ONLINEGAME_COMMON_CHAT_DATA_H__
#define __ONLINEGAME_COMMON_CHAT_DATA_H__

#include "types.h"

#pragma pack(1)

namespace CHAT_S2C
{
	enum 
	{
		CHAT_EQUIP_ITEM,
		CHAT_ACHIEVEMENT,
	};

	struct chat_equip_item
	{
		short cmd_id;
		int type;
		int expire_date;
		unsigned short content_length;
		char content[];
	};

	struct chat_get_achievement
	{
		short cmd_id;
		unsigned short achieve_id;
		int finish_time;
		size_t namelen;
		const void* name;
	};
}

namespace CHAT_C2S
{
	enum		//CHAT_CMD
	{
		CHAT_EQUIP_ITEM, 
		CHAT_ACHIEVEMENT,
	};

	struct chat_equip_item
	{
		short cmd_id;
		char where;
		short index;
	};

	struct chat_get_achievement
	{
		short cmd_id;
		unsigned short achieve_id;
	};
}

#pragma pack()
#endif

