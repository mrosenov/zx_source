#ifndef __ONLINEGAME_GS_COLORANT_ITEM_H__
#define __ONLINEGAME_GS_COLORANT_ITEM_H__

#include "../item.h"
#include <crc.h>

struct colorant_essence_data
{
	int type;
	int color;
	int quality;
	int index;
	int shop_price;
};

class colorant_item : public item_body
{
	colorant_essence_data _data;
	colorant_item(){}

public:
	DECLARE_SUBSTANCE( colorant_item );
	colorant_item( const colorant_essence_data& data ) : _data(data){}
	inline const colorant_essence_data& GetEssence() const { return _data; }
	virtual ITEM_TYPE GetItemType() const  { return ITEM_TYPE_COLORANT; }
	static bool CombineFashionColorant( const abase::vector<item*>& items, int& res_color, int& res_quality );
	int GetColor() const { return _data.color; }
	int GetQuality() const { return _data.quality; }
	int GetIndex() const { return _data.index; };
};
#endif

