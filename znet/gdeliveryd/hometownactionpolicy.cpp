#include "hometownactionpolicy.h"
#include "hometownstateobj.h"

namespace GNET
{
HometownActionPolicy::STUBMAP HometownActionPolicy::_stubs;
#define STUB(CLASSNAME) static CLASSNAME s_##CLASSNAME
	STUB(FarmViewPolicy);
	STUB(FarmPlowPolicy);
	STUB(FarmWaterPolicy);
	STUB(FarmPestPolicy);
	STUB(FarmWeedPolicy);
	STUB(FarmSowPolicy);
	STUB(FarmStealPolicy);
	STUB(FarmHarvestPolicy);
	STUB(FarmCultPolicy);
#undef STUB

int HometownActionPolicy::CanPerform(const HometownStateObj *target, bool must_accept) const
{
	int res = _CanPerform(target);
	if (res != HT_ERR_SUCCESS) return res;
	std::set<int> cur_states;
	target->GetCurrentStates(cur_states);
	std::set<int> intersect;
	if (must_accept)
	{
		std::set_intersection(_accept_states.begin(), _accept_states.end(), cur_states.begin(), cur_states.end(), inserter(intersect, intersect.begin()));
		if (intersect.empty()) return HT_ERR_TARGETNOACCEPTSTATE;
		intersect.clear();
	}
	std::set_intersection(_reject_states.begin(), _reject_states.end(), cur_states.begin(), cur_states.end(), inserter(intersect, intersect.begin()));
	if (!intersect.empty()) return HT_ERR_TARGETSTATEREJECT;
	return HT_ERR_SUCCESS;
}

};
