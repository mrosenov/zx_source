#include "item_recipe.h"
#include "../player_imp.h"

int 
item_recipe::OnUse(item::LOCATION ,size_t index, gactive_imp* obj,item * parent) const
{
	ASSERT(obj->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()));
	gplayer_imp * pImp = (gplayer_imp * ) obj;
	if(pImp->GetProduceLevel() >= _require_produce_level && pImp->AddRecipe(_recipe_id))
	{
		pImp->_runner->player_learn_recipe(_recipe_id);
		return 1;
	}
	
	//error message ¼ÓÈë
	pImp->_runner->error_message(S2C::ERR_LEARN_RECIPE_FAILED);
	
	return 0;
}

