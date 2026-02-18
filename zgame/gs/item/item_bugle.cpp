#include "item_bugle.h"
#include "../clstab.h"
#include "../world.h" 
#include "../player_imp.h"


void item_bugle::OnActivate(size_t index,gactive_imp* obj, item * parent) const
{               
	((gplayer_imp*)obj)->SetChatEmote(_emote_id);
}                       

void item_bugle::OnDeactivate(size_t index,gactive_imp* obj,  item * parent) const
{       
	((gplayer_imp*)obj)->SetChatEmote(0);
}               

