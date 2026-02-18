#ifndef __ONLINEGAME_GS_ASSIST_CARD_ITEM_H__
#define __ONLINEGAME_GS_ASSIST_CARD_ITEM_H__


#include "../item.h"
#include <common/types.h>

//Ó¶±øÆ¾Ö¤
class item_assist_card : public item_body
{

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_ASSIST_CARD;}
	void InitFromShop(gplayer_imp* pImp,item* parent,int value) const;

};


#endif //__ONLINEGAME_GS_ASSIST_CARD_ITEM_H__

