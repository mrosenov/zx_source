#include "pet_filter.h"
#include "sfilterdef.h"
#include "clstab.h"
#include "petman.h"
#include "player_imp.h"
#include "item/item_talisman.h"

DEFINE_SUBSTANCE(filter_pet_combine1,filter,CLS_FILTER_PET_COMBINE1)
DEFINE_SUBSTANCE(filter_pet_combine2,filter,CLS_FILTER_PET_COMBINE2)

void filter_pet_combine1::OnAttach()
{
	for(int i = 0;i < 6;++i)
	{
		_parent.EnhanceResistance(i,_res_add);
	}
	_parent.IncVisibleState(PET_VISIBLE_STATE_COMBINE1);
	_parent.UpdateBuff(GNET::HSTATE_PET_COMBINE1, _filter_id, _timeout);
	_parent.EnhanceScaleDmgReduce(_dmg_reduce);
	_parent.EnhanceAntiCritRate(_anti_crit_rate);
	_parent.UpdateMagicData();
	_parent.UpdateAttackData();
}

void filter_pet_combine1::OnDetach()
{
	for(int i = 0;i < 6;++i)
	{
		_parent.ImpairResistance(i,_res_add);
	}
	_parent.DecVisibleState(PET_VISIBLE_STATE_COMBINE1);
	_parent.RemoveBuff(GNET::HSTATE_PET_COMBINE1, _filter_id); 
	_parent.ImpairScaleDmgReduce(_dmg_reduce);
	_parent.ImpairAntiCritRate(_anti_crit_rate);
	_parent.UpdateMagicData();
	_parent.UpdateAttackData();
}

void filter_pet_combine2::OnAttach()
{
	//获得imp
	gplayer_imp* pImp = (gplayer_imp*)_parent.GetImpl();
	//获得法宝
	item& it = pImp->GetEquipInventory()[item::EQUIP_INDEX_TALISMAN1];
	size_t len;
	void * buf = it.GetContent(len);
	talisman_essence ess(buf,len);
	if(ess.IsValid())
	{
		talisman_enhanced_essence* epEss = ess.QueryEssence();
		_hp_add = int(float(_effect) / 100 * epEss->hp + 0.5f);
		_mp_add = int(float(_effect) / 100 * epEss->mp + 0.5f);
		_dmg_add = int(float(_effect) / 100 * (epEss->damage_low + epEss->damage_high) / 2 + 0.5f);
	}
	_parent.IncVisibleState(PET_VISIBLE_STATE_COMBINE2);
	_parent.UpdateBuff(GNET::HSTATE_PET_COMBINE2, _filter_id, _timeout);
	_parent.EnhanceMaxHP(_hp_add);
	_parent.EnhanceMaxMP(_mp_add);
	_parent.EnhanceDamage(_dmg_add);
	_parent.EnhanceCrit(_crit_rate);
	_parent.UpdateAttackData();
	_parent.UpdateHPMP();
}

void filter_pet_combine2::OnDetach()
{
	_parent.DecVisibleState(PET_VISIBLE_STATE_COMBINE2);
	_parent.RemoveBuff(GNET::HSTATE_PET_COMBINE2, _filter_id); 
	_parent.ImpairMaxHP(_hp_add);
	_parent.ImpairMaxMP(_mp_add);
	_parent.ImpairDamage(_dmg_add);
	_parent.ImpairCrit(_crit_rate);
	_parent.UpdateAttackData();
	_parent.UpdateHPMP();
}

