#include "rage_filter.h"
#include "player_imp.h"

void rage_filter::Heartbeat(int tick)
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	if(((++_heartbeat_counter % 2)  == 0) && !pImp->IsCombatState())
	{
		if(pImp->GetRage() == 0) return;
		int value = int(pImp->GetMaxRage() * 0.02);
		if(value == 0)
		{
			if(pImp->GetRage() > 0) value = 1;
		}	
		if(value > 0) pImp->DecRage(value);
	}
}


