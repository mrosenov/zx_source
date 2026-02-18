#include "../clstab.h"
#include "../actobject.h"
#include "../item_list.h"
#include "../cooldowncfg.h"
#include "item_fireworks.h"

int     
item_fireworks::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{       
	if(!imp->CheckCoolDown(COOLDOWN_INDEX_FIREWORKS))
	{
		imp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	imp->SetCoolDown(COOLDOWN_INDEX_FIREWORKS,FIREWORKS_COOLDOWN_TIME);
	return 1;
}       

int     
item_fireworks_text::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent,const char * arg, size_t arg_size) const
{       
	if(!imp->CheckCoolDown(COOLDOWN_INDEX_FIREWORKS))
	{
		imp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	if(arg_size <2 || (arg_size & 0x01) || arg_size /2 > _max_char) 
	{
		imp->_runner->error_message(S2C::ERR_USE_ITEM_FAILED);
		return -1;
	}

	const unsigned short * pChar = (const unsigned short *)arg;
	size_t count = arg_size / 2;
	for(size_t i = 0; i < count; i ++)
	{
		if(!_char_set.exist(pChar[i])) 
		{
			imp->_runner->error_message(S2C::ERR_USE_ITEM_FAILED);
			return -1;
		}
	}
	
	imp->SetCoolDown(COOLDOWN_INDEX_FIREWORKS,FIREWORKS_COOLDOWN_TIME);
	return 1;
} 

