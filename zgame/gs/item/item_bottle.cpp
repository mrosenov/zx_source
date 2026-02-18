#include "item_bottle.h"
#include "../player_imp.h"
#include "../config.h"
#include "../worldmanager.h"

void AdjustValue(int& need, int& capacity)
{
	if(need < 200) need = 200;
	if(need > capacity) need = capacity;
	capacity -= need;
}

bool item_bottle::IsItemCanUse(item::LOCATION l,gactive_imp* imp) const
{
	gplayer_imp *pImp = (gplayer_imp *)imp;
	if(!pImp->CheckCoolDown(GetCoolDownIndex())) return false;
	if(!pImp->CanUsePotion()) return false;
	return true;
}

int item_bottle::GetCoolDownIndex() const
{
	switch(_bottle_type)
	{
		case BOTTLE_TYPE_HP: return COOLDOWN_INDEX_BOTTLE_HP;
		case BOTTLE_TYPE_MP: return COOLDOWN_INDEX_BOTTLE_MP;
		case BOTTLE_TYPE_DP: return COOLDOWN_INDEX_BOTTLE_DEITY;
	};
	return -1;
}

void item_bottle::SetCoolDown(gactive_imp *imp) const
{
	gplayer_imp *pImp = (gplayer_imp *)imp;
	pImp->SetCoolDown(GetCoolDownIndex(), _cooldown_time);	
}

int item_bottle::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
		imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	if(imp->GetObjectLevel() < _require_level)
	{
		imp->_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -1;
	}
	gplayer_imp * pImp = (gplayer_imp *)imp;
	if(_require_reborn_count > 0 && _require_reborn_count > pImp->GetRebornCount() ) 
	{
		imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return false;
	}
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	};

	int & capacity = *(int*)buf;
	if(capacity <= 0) 
	{
		return -1;
	}
	SetCoolDown(imp);
	int need = 0;
	if(_bottle_type == BOTTLE_TYPE_HP)
	{
		need = imp->GetMaxHP() - imp->GetHP();
		AdjustValue(need, capacity);
		imp->Heal(need, true, false);
	}
	else if(_bottle_type == BOTTLE_TYPE_MP)
	{
		need = imp->GetMaxMP() - imp->GetMP();
		AdjustValue(need, capacity);
		imp->InjectMana(need, true);
	}
	else if(_bottle_type == BOTTLE_TYPE_DP)
	{
		need = imp->GetMaxDP() - imp->GetDP();
		AdjustValue(need, capacity);
		imp->InjectDeity(need, true);
	}
	else
	{
		return -1;
	}
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
	return 0;
}

int item_bottle::FillBottle(gactive_imp* obj, item* parent, size_t bottle_index, int potion_type, int potion_tid, int potion_value, size_t potion_canuse_zone, size_t potion_cannotuse_zone, size_t potion_cantrade_zone, int& potion_left) 
{ 
	if(potion_value <= 0)
	{
		return -1;
	}
	if((parent->proc_type & item::ITEM_PROC_TYPE_ZONETRADE) != potion_cantrade_zone)
	{
		return -1;
	}
	if((parent->proc_type & item::ITEM_PROC_TYPE_ZONEUSE) != potion_canuse_zone)
	{
		return -1;
	}
	if((parent->proc_type & item::ITEM_PROC_TYPE_ZONENOUSE) != potion_cannotuse_zone)
	{
		return -1;
	}
	
	bool flag = false;
	for(size_t i = 0; i < sizeof(_potion_tids) / sizeof(int); i ++)
	{
		if(_potion_tids[i] == potion_tid)
		{
			flag = true;
			break;
		}
	}
	if(!flag)
	{
		return -1;
	}
	
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return -1;
	};

	int & capacity = *(int*)buf;
	if(capacity >= MAX_BOTTLE_CAPACITY)
	{
		return -1;
	}
	if(capacity + potion_value <= capacity)
	{
		return -1;
	}
	potion_left = 0;
	if(capacity + potion_value > MAX_BOTTLE_CAPACITY)
	{
		potion_left = capacity + potion_value - MAX_BOTTLE_CAPACITY;
		capacity = MAX_BOTTLE_CAPACITY;
	}
	else
	{
		capacity += potion_value;
	}
	gplayer_imp * pImp = (gplayer_imp *)obj;
	//满足绑定条件，进行绑定
	parent->Bind();	
	pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY, bottle_index);
	pImp->CheckUseItemConsumption(potion_tid, 1);
	return 0; 
}
