#include "item_couplejump.h"
#include "../player_imp.h"
#include "../cooldowncfg.h"

int 
item_couple_jump::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	if(!pImp->IsMarried())
	{
		return -1;
	}

	if(!pImp->CheckCoolDown(COOLDOWN_INDEX_COUPLE_JUMP))
	{
		pImp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}

	if(!pImp->GetWorldManager() || pImp->GetWorldManager()->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_OUT);
		return -1;
	}

	int sp = pImp->GetSpouse();
	world::object_info info;
	if(!pImp->_plane->QueryObject(XID(GM_TYPE_PLAYER,sp),info,true))
	{
		//²»ÔÚÏß
		pImp->_runner->error_message(S2C::ERR_CAN_NOT_FIND_SPOUSE);
		return -1;
	}

	world_manager * pManager = gmatrix::FindWorld(info.tag);
	if(!pManager || pManager->IsIdle() || pManager->GetWorldLimit().nocouplejump)
	{
		pImp->_runner->error_message(S2C::ERR_CAN_TRANSMIT_IN);
		return -1;
	}

	pImp->SetCoolDown(COOLDOWN_INDEX_COUPLE_JUMP,COUPLE_JUMP_COOLDOWN_TIME);
	pImp->LongJump(info.pos, info.tag);
	return 1;
}

