#include "item_pkreset.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

int 
item_pk_reset::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_PK_RESET))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	pImp->SetCoolDown(COOLDOWN_INDEX_PK_RESET,5*60*1000);
	pImp->ResetPKValue(abs(pImp->GetPKValue()), 0);
	return 1;
}

