#include "item_treasure_digcount.h"
#include "../player_imp.h"

int item_treasure_digcount::OnUse(item::LOCATION l ,size_t index, gactive_imp* imp,item * parent) const
{
	if( l != item::INVENTORY) return -1;
	if(_dig_count <= 0) return -1;
	
	gplayer_imp *pImp = (gplayer_imp *)imp;
	if(!pImp->IsTreasureActive()) return -1;

	pImp->PlayerIncTreasureDigCount(_dig_count);

	return 1;
}
