#include "travel_filter.h"
#include "player_imp.h"

void 
travel_filter::OnAttach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	gplayer * pPlayer = pImp->GetParent();
	pPlayer->SetExtraState(gplayer::STATE_TRAVEL);
	pPlayer->travel_vehicle_id = _travel_vehicle_id;
	pImp->_runner->player_start_travel_around(_travel_vehicle_id, _travel_speed, _travel_path_id);
	pImp->SetNoCheckMove(true);

	
}

void 
travel_filter::OnDetach()
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	gplayer * pPlayer = pImp->GetParent();
	pPlayer->ClrExtraState(gplayer::STATE_TRAVEL);
	pPlayer->travel_vehicle_id = 0;
	pImp->_runner->player_stop_travel_around();
	pImp->SetNoCheckMove(false);
}


void 
travel_filter::Heartbeat(int)
{
	gplayer_imp * pImp = (gplayer_imp*)_parent.GetImpl();
	//×÷±×¼ì²é
	if(pImp->GetWorldTag() != _travel_map_id)
	{
		_is_deleted = true;
		return;
	}
}


void
charge_filter::OnAttach()
{
	_parent.SetRootSeal(true);
}

void
charge_filter::OnDetach()
{
	_parent.SetRootSeal(false);
}

