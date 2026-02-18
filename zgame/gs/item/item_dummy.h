#ifndef __ONLINE_GAME_GS_ITEM_DUMMY_H__
#define __ONLINE_GAME_GS_ITEM_DUMMY_H__

class item_dummy : public item_body
{
public: 
	item_dummy()
	{
	}       


public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_DUMMY;}
};

#endif

