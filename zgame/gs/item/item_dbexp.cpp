#include "item_dbexp.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

int 
item_dbl_exp::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_DBL_EXP))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}

	if(pImp->GetMultiExpTime(_multi_exp) + _dbl_time > MAX_MULTI_EXP_TIME)
	{
		pImp->_runner->error_message(S2C::ERR_MULTI_EXP_TIME_LIMIT);
		return -1;
	}	

	pImp->SetCoolDown(COOLDOWN_INDEX_DBL_EXP, 10*1000);
	pImp->ActiveMultiExpTime(_multi_exp, _dbl_time);
	return 1;
}

