
#ifndef __ONLINEGAME_GS_SPECIAL_NAME_ITEM_H__
#define __ONLINEGAME_GS_SPECIAL_NAME_ITEM_H__ 

#include "../item.h"
#include "../config.h"
#include <common/types.h>

class item_special_name : public item_body
{

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_SPECIAL_NAME;}
};

#endif
