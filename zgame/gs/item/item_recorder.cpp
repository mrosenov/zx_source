#include "../clstab.h"
#include "../actobject.h"
#include "../item_list.h"
#include "../cooldowncfg.h"
#include "item_recorder.h"
#include "../player_imp.h"

int     
item_recorder::OnUse(item::LOCATION ,size_t index, gactive_imp* imp,item * parent) const
{       
	if(!imp->CheckCoolDown(COOLDOWN_INDEX_RECORDER))
	{
		imp->_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return -1;
	}
	imp->SetCoolDown(COOLDOWN_INDEX_RECORDER,RECORDER_COOLDOWN_TIME);
	gplayer_imp * pImp = (gplayer_imp * )imp;
	pImp->StartRecorder();
	return 0;
}       

