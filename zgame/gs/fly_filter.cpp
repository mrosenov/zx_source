#include "fly_filter.h"
#include "clstab.h"
#include "arandomgen.h"
#include "player_imp.h"
#include "world.h"
#include "item.h"


void 
aircraft_filter::OnAttach()
{

	//2:增加移动速度
	_parent.EnhanceOverrideSpeed(_speed);
	_parent.UpdateSpeedData();
	_parent.SendClientCurSpeed();

	//生效附加属性
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->ActiveFlyState(0);
}

void 
aircraft_filter::OnDetach()
{
	_parent.ImpairOverrideSpeed(_speed);
	_parent.UpdateSpeedData();
	_parent.SendClientCurSpeed();

	//生效附加属性
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	pImp->DeactiveFlyState(1);
}
	
void 
aircraft_filter::Heartbeat(int)
{
	if(_cost > 0 && _parent.SpendTalismanStamina(_cost) <= 0)
	{
		_is_deleted = true;
	}
	else
	{
		_exp_counter ++;
		if(_exp_counter >= 30)
		{
			if(_exp_add > 0) _parent.TalismanGainExp(_exp_add, true);
			_exp_counter = 0;
		}
	}
}

