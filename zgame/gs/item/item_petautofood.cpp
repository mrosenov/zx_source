#include "item_petautofood.h"
#include "../clstab.h"
#include "../player_imp.h"

int item_pet_auto_food::OnUse(item::LOCATION l,size_t index,gactive_imp* pImp,item *parent) const
{
	size_t temp = 0;
	return OnUse(l,index,pImp,parent,(const char*)&temp,sizeof(size_t));
}

int item_pet_auto_food::OnUse(item::LOCATION l,size_t index,gactive_imp* pImp,item *parent,const char* arg,size_t arg_size) const
{
	if(pImp->GetWorldManager()->GetWorldLimit().nopetfood)
	{
		return -1;
	}
	if(sizeof(use_arg_t) != arg_size) return -1;
	size_t len;
	const void* buf = parent->GetContent(len);
	int cur_value = 0;
	if(sizeof(int) != len) return -1;
	cur_value = *(int*)buf;
	use_arg_t* pArg = (use_arg_t*)arg;
	int rst = ((gplayer_imp*)pImp)->PlayerAutoFeedPet(pArg->index,parent,
		_hp_gen,_mp_gen,_hunger_gen,pArg->mask,cur_value);
	if(1 == rst)
	{
		//吃掉了
	}
	else if(0 == rst)
	{
		//吃了1部分
		*(int*)buf = cur_value;
		((gplayer_imp*)pImp)->PlayerGetItemInfo(gplayer_imp::IL_INVENTORY,index);
	}
	else if(-1 == rst)
	{
		//失败了
	}
	return rst;
}

void item_pet_auto_food::InitFromShop(gplayer_imp* pImp,item* parent,int value) const
{
	__PRINTF("item_pet_auto_food::InitFromShop\n");
	size_t len;     
	parent->GetContent(len);
	if(sizeof(int) == len) return;
	//多次消耗物品
	int temp_food_value = _food_value;
	if(temp_food_value <= 0)
	{
		__PRINTF("数据错误,_food_value不对\n");
		temp_food_value = 1;
	}
	parent->SetContent(&temp_food_value,sizeof(int));
}

