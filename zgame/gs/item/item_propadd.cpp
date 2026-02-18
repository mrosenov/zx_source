#include "item_propadd.h"
#include "../playertemplate.h"
#include "../player_imp.h"

int item_propadd :: OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{
	gplayer_imp * pImp = (gplayer_imp*)imp;
	int r_level = pImp->GetRebornCount() * 150 + pImp->GetObjectLevel();
	if(r_level < _require_level + _require_reborn_count * 150) 
	{
		imp->_runner->error_message(S2C::ERR_CANNOT_USE_ITEM);
		return -1;
	}
		
	for(int i = 0; i < PROP_ADD_NUM; i ++)
	{
		int limit = player_template::GetPropAddLimit(i);
		if(limit <= 0)
		{
			continue;
		}
		int curr = pImp->GetPropAdd(i);
		int inc = curr + _prop_add[i] > limit ? limit - curr : _prop_add[i];
		if(inc > 0)
		{
			pImp->IncPropAdd(i, inc);
			GLog::log(GLOG_INFO,"用户%d吃属性丹增强了属性(%d)%d点", pImp->_parent->ID.id, i, inc);
		}
	}
	player_template::IncPropAdd(pImp->GetPlayerClass(), pImp);
	property_policy::UpdatePlayer(pImp->GetPlayerClass(),pImp);
	pImp->SetRefreshState();
	return 1;
}
