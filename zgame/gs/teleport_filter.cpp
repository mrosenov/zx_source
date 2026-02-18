#include "teleport_filter.h"
#include "clstab.h"
#include "statedef.h"
#include "player_imp.h"

DEFINE_SUBSTANCE(teleport1_filter,filter,CLS_FILTER_TELEPORT1)
DEFINE_SUBSTANCE(teleport2_filter,filter,CLS_FILTER_TELEPORT2)


void
teleport1_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	_parent.UpdateBuff(GNET::HSTATE_TELEPORT1, _filter_id, 0); 
	pImp->GetParent()->SetExtraState(gplayer::STATE_SUMMON_TELEPORT1);
}

void
teleport1_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	_parent.RemoveBuff(GNET::HSTATE_TELEPORT1, _filter_id); 
	pImp->GetParent()->ClrExtraState(gplayer::STATE_SUMMON_TELEPORT1);
}



void
teleport2_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	_parent.UpdateBuff(GNET::HSTATE_TELEPORT2, _filter_id, 0); 
	pImp->GetParent()->SetExtraState(gplayer::STATE_SUMMON_TELEPORT2);
}

void
teleport2_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	_parent.RemoveBuff(GNET::HSTATE_TELEPORT2, _filter_id); 
	pImp->GetParent()->ClrExtraState(gplayer::STATE_SUMMON_TELEPORT2);
}

