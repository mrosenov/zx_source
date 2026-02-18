#include "item_off_agent.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

int 
item_offline_agent::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	pImp->ActiveOfflineAgentTime(_agent_time);
	return 1;
}


