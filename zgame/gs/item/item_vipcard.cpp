#include "item_vipcard.h"
#include "../player_imp.h"

extern abase::timer      g_timer;
#define VIP_TITLE 10849

bool 
item_vipcard::IsItemCanUse(item::LOCATION l, gactive_imp* pImp) const
{
	if(l != item::INVENTORY) return false;
	return true;
}


int 
item_vipcard::OnUse(item::LOCATION l, size_t index, gactive_imp *imp, item * parent) const
{
	if(l != item::INVENTORY) return 0;

	//在同一时刻，只能有一张VIP卡在使用
	gplayer_imp *pImp = (gplayer_imp *)imp;
	if(pImp->GetParent()->IsVIPState()) return 0;
	if(parent != NULL && parent->expire_date < 0) return 0;
	
	parent->Bind();
	if(parent != NULL && parent->expire_date == 0)
	{
		parent->expire_date = g_timer.get_systime() + _duration;	
		imp->UpdateExpireItem(parent->expire_date);
	}

	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);

	if(pImp->PlayerEnableVIPState(_tid))
	{
		if(pImp->InsertPlayerTitle(VIP_TITLE))
		{
		 	pImp->_runner->player_add_title(VIP_TITLE);
		}
		// 更新活跃度[炼丹炉-VIP卡状态]
		pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_VIP_CARD_OPEN);
	}

	return 0;
}

void
item_vipcard::OnTakeOut(item::LOCATION l,size_t index, gactive_imp* imp,item * parent) const
{
	if(l != item::INVENTORY) return;

	if(parent->expire_date > 0)
	{
		gplayer_imp *pImp = (gplayer_imp *)imp;
		if(l == item::INVENTORY)
		{
			pImp->PlayerDisableVIPState();		
			pImp->RemovePlayerTitle(VIP_TITLE);
			pImp->_runner->player_del_title(VIP_TITLE);
		}
	}
}

void
item_vipcard::OnPutIn(item::LOCATION  l,size_t index , gactive_imp* imp,item * parent) const
{
	if(l != item::INVENTORY) return;

	gplayer_imp *pImp = (gplayer_imp *)imp;
	if(pImp->GetParent()->IsVIPState()) return;

	if(parent->expire_date > 0)
	{
		if(pImp->PlayerEnableVIPState(_tid))
		{
			if(pImp->InsertPlayerTitle(VIP_TITLE))
			{
				pImp->_runner->player_add_title(VIP_TITLE);
			}
		}
	}
}
