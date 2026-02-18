#ifndef __ONLINEGAME_GS_ITEM_FIREWORKS_H__
#define __ONLINEGAME_GS_ITEM_FIREWORKS_H__

#include "../item.h"
#include "../config.h"
#include "../staticmap.h"

class item_fireworks : public item_body
{
public:
	item_fireworks()
	{}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_FIREWORKS;}

	virtual bool IsItemCanUse(item::LOCATION l,gactive_imp* pImp) const { return true;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const;
	virtual bool IsItemBroadcastUse() const {return true;}
};

class item_fireworks_text : public item_body
{
	size_t _max_char;
	abase::static_set<unsigned short> _char_set;

public:
	item_fireworks_text(const unsigned short * pChar, size_t len, int max_char):_max_char(max_char)
	{
		ASSERT(_max_char > 0 && _max_char <256);
		for(size_t i = 0; i < len; i ++)
		{
			if(pChar[i] == 0) break;
			_char_set.insert(pChar[i]);
		}
	}

public:
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_FIREWORKS;}
	virtual bool IsItemCanUseWithArg(item::LOCATION l,size_t buf_size,gactive_imp* pImp) const { return buf_size >= 2 && buf_size/2 <= _max_char;}
	virtual int OnGetUseDuration() const { return -1;}
	virtual int OnUse(item::LOCATION ,size_t index, gactive_imp*,item * parent,const char * arg, size_t arg_size) const;
	virtual bool IsItemBroadcastUse() const {return true;}
	virtual bool IsItemBroadcastArgUse() const {return true;}

};
#endif



