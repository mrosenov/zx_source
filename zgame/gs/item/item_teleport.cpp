#include "item_teleport.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"


int 
item_teleport::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *)obj;
	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_ITEM_TELEPORT))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	if(pImp->GetWorldManager()->IsRaidWorld() || pImp->GetWorldManager()->IsFacBase())
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}

	if((_reborn_cnt > 0 && pImp->GetRebornCount() < _reborn_cnt) || (_require_level > 0 && pImp->GetObjectLevel() < _require_level))
	{
		pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	/*
	if(!pImp->GetWorldManager() | pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
		return -1;
	}
	*/

	pImp->SetCoolDown(COOLDOWN_INDEX_ITEM_TELEPORT,5000);
	//开始使用              
	if(pImp->LongJump(_pos,_tag))
	{
		return 1;
	}
	pImp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

