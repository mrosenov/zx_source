#include "kingdom_filter.h"
#include "statedef.h"
#include "clstab.h"

DEFINE_SUBSTANCE(kingdom_exp_filter,filter,CLS_FILTER_KINGDOM_EXP)

void 
kingdom_exp_filter::OnAttach()
{
	_parent.UpdateBuff(GNET::HSTATE_KINDOMEXP, _filter_id, 0); 
}

void 
kingdom_exp_filter::OnDetach()
{
	_parent.RemoveBuff(GNET::HSTATE_KINDOMEXP, _filter_id);
}

