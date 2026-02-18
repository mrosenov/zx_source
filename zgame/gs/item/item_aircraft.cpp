#include "item_aircraft.h"
#include "../clstab.h"
#include "../actobject.h"
#include "../item_list.h"
#include "../item_manager.h"
#include "../player_imp.h"
#include "item_equip.h"

void
item_aircraft::OnPutIn(item::LOCATION l,size_t index, gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
			//      Activate(index,obj);  现在不作了，在外面统一扫描
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

void
item_aircraft::OnTakeOut(item::LOCATION l,size_t index,gactive_imp* obj,item * parent) const
{
	switch(l)
	{
		case item::BODY:
			Deactivate(index,obj,parent);
			break;
		case item::INVENTORY:
		case item::TASK_INVENTORY:
		case item::BACKPACK:
		default:
			break;
	};
}

bool
item_aircraft::VerifyRequirement(item_list & list,gactive_imp* obj,const item * parent) const
{
	if(list.GetLocation() == item::BODY)
	{
		return obj->GetObjectLevel() >= _level_required
			&& object_base_info::CheckCls(obj->GetObjectClass(), _class_required, _class_required1);
	}
	else
	{
		return false;
	}
}

void 
item_aircraft::OnActivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	pImp->ActiveAircraft(_speed, 0 ,0 );
}

void 
item_aircraft::OnDeactivate(size_t index,gactive_imp* obj, item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp *) obj;
	pImp->DisableAircraft();

	//去除可能的飞行filter $$$$$$$$$
}


int 
item_aircraft_stamina_potion::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	return -1;
/*
	gplayer_imp * pImp = (gplayer_imp*)imp;
	if(!pImp->IsAircraftAvailable())
	{
		//不报错误了 
		imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
	
	size_t len;
	void * buf = parent->GetContent(len);
	if(len == sizeof(int))
	{
		int & stamina = *(int*)buf;
		if(stamina <= 0) return 1;
		if(stamina > _max_stamina) stamina = _max_stamina;

		int need = pImp->ChargeAircraftStamina(stamina);
		if(need < _min_cost) need = _min_cost;
		if(need > stamina) need = stamina;
		stamina -= need;
		if(stamina <= 0) 
			return 1;
		else
		{
			pImp->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
			return 0;
		}
	}

	imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
	return -1;
	*/
}


