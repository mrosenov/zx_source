
#include "item_assistcard.h"
#include "../player_imp.h"

void 
item_assist_card::InitFromShop(gplayer_imp* pImp,item* parent,int value) const
{
	if(parent->expire_date == 0)
	{
		parent->expire_date = g_timer.get_systime() + 3*24*3600;
	}
	
	size_t len;
	parent->GetContent(len);
	if(len == 0)
	{
		int id[2];
		id[0] = pImp->OI_GetMafiaID();
		id[1] = 0;
		parent->SetContent(&id, 2 * sizeof(int));	
	}
}

