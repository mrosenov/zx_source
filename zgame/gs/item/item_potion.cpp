#include "item_potion.h"
#include "../potion_filter.h"
#include "../player_imp.h"

bool 
item_base_potion::IsItemCanUse(item::LOCATION l,gactive_imp* imp) const 
{
	gplayer_imp *pImp = (gplayer_imp *)imp;
	if(!pImp->CheckCoolDown(GetCoolDownIndex())) return false;
	if(!pImp->CanUsePotion()) return false;
	if(_battleground_only && !pImp->GetWorldManager()->GetWorldLimit().can_use_battleground_potion) return false;

	bool map_limit = false;
	int cur_map_id = pImp->GetWorldManager()->GetClientTag();
	/*
	if(pImp->GetWorldManager()->IsRaidWorld())
	{
		cur_map_id = pImp->GetWorldManager()->GetRaidID();
	}
	else 
	{
		cur_map_id = pImp->GetWorldTag();
	}
	*/	
	for(size_t i = 0; i < 5; ++i)
	{
		if(_map_limit[i] > 0 && _map_limit[i] != cur_map_id)
		{
			map_limit = true;
		}
		else if(_map_limit[i] > 0 &&  _map_limit[i] == cur_map_id)
		{
			map_limit = false;
			break;
		}
	}
	if(map_limit) return false;

	return true;
}

void
item_base_potion::SetCoolDown(gactive_imp *imp) const
{
	gplayer_imp *pImp = (gplayer_imp *)imp;
	pImp->SetCoolDown(GetCoolDownIndex(), _cooldown_time);	
}

int 
item_step_healing_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return -1;
	}
	SetCoolDown(imp);
	imp->_filters.AddFilter(new healing_potion_filter(imp,_life));
	return 1;
}

int 
item_step_mana_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	SetCoolDown(imp);
	imp->_filters.AddFilter(new mana_potion_filter(imp,_mana));
	return 1;
}

int 
item_step_rejuvenation_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	SetCoolDown(imp);
	imp->_filters.AddFilter(new rejuvenation_potion_filter(imp,_life,_mana));
	return 1;
}

int 
item_healing_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	
	SetCoolDown(imp);
	size_t len;
	void * buf = parent->GetContent(len);
	if(len == sizeof(int))
	{
		gplayer_imp * pImp = (gplayer_imp*)imp;
		int & heal = *(int*)buf;
		if(heal <= 0) return 1;
		int need = imp->GetMaxHP() - imp->GetHP();
		if(need < 200) need = 200;
		if(need > heal) need = heal;
		heal -= need;
		imp->Heal(need, true, false);
		imp->OnUsePotion(0, need);
		if(heal <= 0) 
			return 1;
		else
		{
			pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
			return 0;
		}
	}

	imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

void item_healing_potion::SetPotionCapacity(item* parent, int v)
{
	if(v <= 0)
	{
		return; //值小于等于0的时候直接删除
	}
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return;
	}
	*(int*)buf = v;
}

int item_healing_potion::GetPotionCapacity(item* parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return 0;
	}
	return *(int*)buf;
}

int 
item_mana_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	
	SetCoolDown(imp);
	size_t len;
	void * buf = parent->GetContent(len);
	if(len == sizeof(int))
	{
		gplayer_imp * pImp = (gplayer_imp*)imp;
		int & mana = *(int*)buf;
		if(mana <= 0) return 1;
		int need = imp->GetMaxMP() - imp->GetMP();
		if(need < 200) need = 200;
		if(need > mana) need = mana;
		mana -= need;
		imp->InjectMana(need, true);
		imp->OnUsePotion(1, need);
		if(mana <= 0) 
			return 1;
		else
		{
			pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
			return 0;
		}
	}

	imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

void item_mana_potion::SetPotionCapacity(item* parent, int v)
{
	if(v <= 0)
	{
		return; //值小于等于0的时候直接删除
	}
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return;
	}
	*(int*)buf = v;
}

int item_mana_potion::GetPotionCapacity(item* parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return 0;
	}
	return *(int*)buf;
}

int 
item_rejuvenation_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	
	SetCoolDown(imp);
	size_t len;
	void * buf = parent->GetContent(len);
	if(len == sizeof(rejuvenation_potion_essence))
	{
		gplayer_imp * pImp = (gplayer_imp*)imp;
		rejuvenation_potion_essence & ess = *(rejuvenation_potion_essence*)buf;
		if(ess.hp <= 0 && ess.mana <= 0) return 1;
		int need_heal = imp->GetMaxHP() - imp->GetHP();
		if(need_heal < 200) need_heal = 200;
		if(need_heal > ess.hp) need_heal = ess.hp;;
		
		int need_mana = imp->GetMaxMP() - imp->GetMP();
		if(need_mana < 200) need_mana = 200;
		if(need_mana > ess.mana) need_mana = ess.mana;

		ess.mana -= need_mana;
		ess.hp -= need_heal ;
		imp->Heal(need_heal, true, false);
		imp->InjectMana(need_mana, true);
		imp->OnUsePotion(0, need_heal);
		imp->OnUsePotion(1, need_mana);
		if(ess.hp <= 0 && ess.mana <= 0) 
			return 1;
		else
		{
			pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
			return 0;
		}
	}

	imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

int 
item_deity_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	
	SetCoolDown(imp);
	size_t len;
	void * buf = parent->GetContent(len);
	if(len == sizeof(int))
	{
		gplayer_imp * pImp = (gplayer_imp*)imp;
		int & deity = *(int*)buf;
		if(deity <= 0) return 1;
		int need = imp->GetMaxDP() - imp->GetDP();
		if(need < 200) need = 200;
		if(need > deity) need = deity;
		deity -= need;
		imp->InjectDeity(need, true);
		if(deity <= 0) 
			return 1;
		else
		{
			pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
			return 0;
		}
	}

	imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
}

void item_deity_potion::SetPotionCapacity(item* parent, int v)
{
	if(v <= 0)
	{
		return; //值小于等于0的时候直接删除
	}
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return;
	}
	*(int*)buf = v;
}

int item_deity_potion::GetPotionCapacity(item* parent) const
{
	size_t len;
	void * buf = parent->GetContent(len);
	if(len != sizeof(int))
	{
		return 0;
	}
	return *(int*)buf;
}

int 
item_step_deity_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	if(imp->GetWorldManager()->GetWorldLimit().nodrug)
	{
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
		return false;
	}
	SetCoolDown(imp);
	imp->_filters.AddFilter(new deity_potion_filter(imp,_dp));
	return 1;
}

