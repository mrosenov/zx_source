#include "item_poppet.h"
#include "../actobject.h"

/*
void item_poppet_item::OnPutIn(item::LOCATION  l ,size_t index , gactive_imp* obj,item * parent) const
{
	if( l == item::INVENTORY)
	{
		obj->ModifyItemPoppet(1);
	}
}
void item_poppet_item::OnTakeOut(item::LOCATION l,size_t index, gactive_imp*obj,item * parent) const
{
	if( l == item::INVENTORY)
	{
		obj->ModifyItemPoppet(-1);
	}
}

void item_poppet_exp::OnPutIn(item::LOCATION l,size_t index , gactive_imp*obj,item * parent) const
{
	if( l == item::INVENTORY)
	{
		obj->ModifyExpPoppet(1);
	}
}
void item_poppet_exp::OnTakeOut(item::LOCATION l ,size_t index, gactive_imp*obj,item * parent) const
{
	if( l == item::INVENTORY)
	{
		obj->ModifyExpPoppet(-1);
	}
}*/

void item_poppet::OnPutIn(item::LOCATION  l ,size_t index , gactive_imp* obj,item * parent) const
{
	if( l == item::INVENTORY)
	{
		obj->ModifyItemPoppet(1);
	}
}

void item_poppet::OnTakeOut(item::LOCATION l,size_t index, gactive_imp*obj,item * parent) const
{
	if( l == item::INVENTORY)
	{
		obj->ModifyItemPoppet(-1);
	}
}

