#include "item_petequip.h"
#include "../clstab.h"
#include "../petdataman.h"

void item_pet_equip::InitFromShop(gplayer_imp* pImp,item* parent,int value) const
{
	__PRINTF("item_pet_equip::InitFromShop\n");
	size_t len;     
	parent->GetContent(len);
	if(sizeof(itemdataman::_pet_armor_essence) == len)
	{
		//init¹ýÁË
		return;
	} 
	itemdataman::_pet_armor_essence temp;
	const pet_equip_data_temp* pTemplate = pet_equip_dataman::Get(parent->type);
	if(!pTemplate) return;
	int i = 0;
	temp.maxhp = (int)(abase::Rand(pTemplate->int_props[0].minimum,pTemplate->int_props[0].maximum) + 1e-6f);
	temp.maxmp = (int)(abase::Rand(pTemplate->int_props[11].minimum,pTemplate->int_props[11].maximum) + 1e-6f);
	temp.defence = (int)(abase::Rand(pTemplate->int_props[1].minimum,pTemplate->int_props[1].maximum) + 1e-6f);
	temp.attack = (int)(abase::Rand(pTemplate->int_props[2].minimum,pTemplate->int_props[2].maximum) + 1e-6f);
	for( i = 0; i < 6; ++i )
	{
		temp.resistance[i] = (int)(abase::Rand(pTemplate->int_props[3+i].minimum,pTemplate->int_props[3+i].maximum) + 1e-6f);
	}
	temp.hit = (int)(abase::Rand(pTemplate->int_props[9].minimum,pTemplate->int_props[9].maximum ) + 1e-6f);
	temp.jouk = (int)(abase::Rand(pTemplate->int_props[10].minimum,pTemplate->int_props[10].maximum ) + 1e-6f);
	temp.crit_rate = abase::Rand(pTemplate->float_props[0].minimum,pTemplate->float_props[0].maximum + 1e-6f);
	temp.crit_damage = abase::Rand(pTemplate->float_props[1].minimum,pTemplate->float_props[1].maximum + 1e-6f);
	temp.reserved = 0;
	parent->SetContent(&temp,sizeof(itemdataman::_pet_armor_essence));
}

