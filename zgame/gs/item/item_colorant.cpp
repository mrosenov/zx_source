#include <set>
#include "../clstab.h"
#include "item_colorant.h"

DEFINE_SUBSTANCE( colorant_item, item_body, CLS_ITEM_COLORANT )

bool colorant_item::CombineFashionColorant( const abase::vector<item*>& items, int& res_color, int& res_quality )
{
	int cnt = items.size();
	int quality = -1;
	std::set<int> colors;
	for( int i = 0; i < cnt; ++i )
	{
		if( !items[i] || !items[i]->body )
		{
			return false;
		}
		colorant_item* body = dynamic_cast<colorant_item*>( items[i]->body );
		if( !body )
		{
			return false;
		}
		if( quality == -1 )
		{
			quality = body->_data.quality;
		}
		if( quality != body->_data.quality )
		{
			return false;
		}
		colors.insert( body->_data.color );
	}
	if( player_template::GetInstance().HasQuality( quality ) != QUALITY_OK )
	{
		return false;
	}
	res_quality = player_template::GetInstance().GetBetterQuality( quality );
	if( (res_quality != quality) && cnt < 3 )
	{
		return false;
	}
	res_color = player_template::GetInstance().GetResultColor( res_quality, quality, colors, cnt );
	return player_template::GetInstance().HasColor( res_color );
}
