#ifndef __ONLINE_GAME_GS_ITEM_DESTROY_ITEM_H__
#define __ONLINE_GAME_GS_ITEM_DESTROY_ITEM_H__

class item_destroying_item : public item_body
{
public: 
	item_destroying_item()
	{
	}       


public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_DUMMY;}
	virtual void GetItemDataForClient(const void **data, size_t & size, const void * buf, size_t len)
	{
		if(len == 8 * sizeof(int))
		{
			*data = buf;
			size = len;
		}
		else
		{
			*data = buf;
			size =  8 * sizeof(int);
		}
	}
};

#endif

