#include "hometownitemfunc.h"
#include "farmcompomanager.h"
#include "hometowndef.h"
#include <assert.h>
//#include <typeinfo>

namespace GNET
{
HometownToolFunc::TypeFuncMap HometownToolFunc::_stubs;
#define STUB(TOOLFUNC) static TOOLFUNC s_##TOOLFUNC
STUB(FarmProtectToolFunc);
STUB(FieldProtectToolFunc);
STUB(FieldSpeedGrowToolFunc);
STUB(FarmSpeedGrowToolFunc);
STUB(FarmHideToolFunc);
#undef STUB

int FarmProtectToolFunc::CanPerform(const HometownPlayerObj *user, const HometownItem *item, const HometownTargetObj *target) const
{
	const Farm *farm = dynamic_cast<const Farm *>(target);
	if (farm == NULL) return HT_ERR_INVALIDTARGETTYPE;
	return HT_ERR_SUCCESS;
}

void FarmProtectToolFunc::Perform(HometownPlayerObj *user, HometownItem *item, HometownTargetObj *target) const
{
	Farm *farm = dynamic_cast<Farm *>(target);
	if (farm == NULL) return;
	assert(_func->num_params==2);//该TOOL_FUNC的param1是时间,param2是buffid

	if (farm->IsInState(HTF_STATE_FARMPROTECTBEGIN+_func->param2))
	{
		farm->AddStateExpireTime(HTF_STATE_FARMPROTECTBEGIN+_func->param2, _func->param1);
	}
	else
	{       
		FarmProtectState *s = new FarmProtectState(_func->param2, Timer::GetTime(), _func->param1);
		farm->RawAddState(s);
	}  
	farm->_protect_period[_func->param2] += _func->param1;
	farm->SetDirty(true);
}

int FieldProtectToolFunc::CanPerform(const HometownPlayerObj *user, const HometownItem *item, const HometownTargetObj *target) const
{
	const FarmField *field = dynamic_cast<const FarmField *>(target);
	if (field == NULL) return HT_ERR_INVALIDTARGETTYPE;
	if (!field->_has_crop) return HT_ERR_HASNOCROP;
	const FarmCrop &crop = field->_crop;
	std::set<int> grow_states;
	grow_states.insert(HTF_STATE_SEED);
	grow_states.insert(HTF_STATE_SPROUT);
	grow_states.insert(HTF_STATE_IMMATURE);
	grow_states.insert(HTF_STATE_PREMATURE);
	grow_states.insert(HTF_STATE_MATURE);
	if (!crop.IsInAnyState(grow_states))
		return HT_ERR_TARGETNOACCEPTSTATE;
	return HT_ERR_SUCCESS;
}

void FieldProtectToolFunc::Perform(HometownPlayerObj *user, HometownItem *item, HometownTargetObj *target) const
{
	FarmField *field = dynamic_cast<FarmField *>(target);
	if (field == NULL) return;
	if (!field->_has_crop) return;
	assert(_func->num_params==2);//该TOOL_FUNC的param1是时间, param2是buffid
	FarmCrop &crop = field->_crop;
	if (crop.IsInState(HTF_STATE_CROPPROTECTBEGIN+_func->param2))
	{
		crop.AddStateExpireTime(HTF_STATE_CROPPROTECTBEGIN+_func->param2, _func->param1);
	}
	else
	{       
		CropProtectState *s = new CropProtectState(_func->param2, Timer::GetTime(), _func->param1);
		crop.RawAddState(s);
	}  
	crop._protect_period[_func->param2] += _func->param1;
	crop.SetDirty(true);
}

int FieldSpeedGrowToolFunc::CanPerform(const HometownPlayerObj *user, const HometownItem *item, const HometownTargetObj *target) const
{
	const FarmField *field = dynamic_cast<const FarmField *>(target);
	if (field == NULL) return HT_ERR_INVALIDTARGETTYPE;
	if (!field->_has_crop) return HT_ERR_HASNOCROP;
	std::set<int> grow_states;
	grow_states.insert(HTF_STATE_SEED);
	grow_states.insert(HTF_STATE_SPROUT);
	grow_states.insert(HTF_STATE_IMMATURE);
	grow_states.insert(HTF_STATE_PREMATURE);
	if (!field->_crop.IsInAnyState(grow_states))
		return HT_ERR_TARGETNOACCEPTSTATE;
	return HT_ERR_SUCCESS;
}

void FieldSpeedGrowToolFunc::Perform(HometownPlayerObj *user, HometownItem *item, HometownTargetObj *target) const
{
	FarmField *field = dynamic_cast<FarmField *>(target);
	if (field == NULL) return;
	if (!field->_has_crop) return;
	assert(_func->num_params==1);//该TOOL_FUNC的param1是时间
	FarmCrop &crop = field->_crop;
	bool changed = false;
	crop.PreAction(HTF_ACTION_NONE, changed);
	if (crop._grow_time+_func->param1 > crop._grow_period)
	{
		crop._state_grow_time += crop._grow_period-crop._grow_time;
		crop._grow_time = crop._grow_period;
	}
	else
	{
		crop._grow_time += _func->param1;
		crop._state_grow_time += _func->param1;
	}
	crop.PreAction(HTF_ACTION_NONE, changed);
	crop.SetDirty(true);
}

int FarmSpeedGrowToolFunc::CanPerform(const HometownPlayerObj *user, const HometownItem *item, const HometownTargetObj *target) const
{
	const Farm *farm = dynamic_cast<const Farm *>(target);
	if (farm == NULL) return HT_ERR_INVALIDTARGETTYPE;
	return HT_ERR_SUCCESS;
}

void FarmSpeedGrowToolFunc::Perform(HometownPlayerObj *user, HometownItem *item, HometownTargetObj *target) const
{
	Farm *farm = dynamic_cast<Farm *>(target);
	if (farm == NULL) return;
	assert(_func->num_params==2);//该TOOL_FUNC的param1是时间, param2是buffid
	std::set<int> grow_states;
	grow_states.insert(HTF_STATE_SEED);
	grow_states.insert(HTF_STATE_SPROUT);
	grow_states.insert(HTF_STATE_IMMATURE);
	grow_states.insert(HTF_STATE_PREMATURE);

	Farm::FieldMap::iterator it = farm->_fields.begin(), ie = farm->_fields.end();
	for (; it != ie; ++it)
	{
		if (it->second._has_crop)
		{
			FarmCrop &crop = it->second._crop;
			if (crop.IsInAnyState(grow_states))
			{
				bool changed = false;
				crop.PreAction(HTF_ACTION_NONE, changed);
				if (crop._grow_time+_func->param1 > crop._grow_period)
				{
					crop._state_grow_time += crop._grow_period-crop._grow_time;
					crop._grow_time = crop._grow_period;
				}
				else
				{
					crop._grow_time += _func->param1;
					crop._state_grow_time += _func->param1;
				}
				crop.PreAction(HTF_ACTION_NONE, changed);
			}
		}
	}
	if (farm->IsInState(HTF_STATE_FARMSPEEDGROWBEGIN+_func->param2))
	{
		farm->AddStateExpireTime(HTF_STATE_FARMSPEEDGROWBEGIN+_func->param2, HTF_TIME_SHOWFARMSPEEDGROW);
	}
	else
	{
		farm->RawAddState(new FarmSpeedGrowState(_func->param2));
	}
	farm->SetDirty(true);
}

int FarmHideToolFunc::CanPerform(const HometownPlayerObj *user, const HometownItem *item) const
{
	const FarmPlayer *player = dynamic_cast<const FarmPlayer *>(user);
	if (player == NULL) return HT_ERR_INVALIDTARGETTYPE;
	if (player->IsInState(HTF_STATE_HIDE))
		return HT_ERR_TARGETSTATEREJECT;
	return HT_ERR_SUCCESS;
}

void FarmHideToolFunc::Perform(HometownPlayerObj *user, HometownItem *item) const
{
	FarmPlayer *player = dynamic_cast<FarmPlayer *>(user);
	if (player == NULL) return;
	assert(_func->num_params==1);//该TOOL_FUNC的param1是时间
	HideState *s = new HideState(Timer::GetTime(), _func->param1);
	user->RawAddState(s);
	user->SetDirty(true);
}

template <typename ITEMFUNC, typename FUNC_TPL>
void HometownItemFuncMan::MakeFunc(unsigned int id)
{
	DATA_TYPE dummy;
	const FUNC_TPL *func;
	if (_tpl->get_data_ptr(id, ID_SPACE_FUNC, dummy, func) != NULL)
	{       
		ITEMFUNC *body = new ITEMFUNC();
		body->SetTemplate(func);
		_map[id] = body; 
	}
}

void HometownItemFuncMan::MakeToolFunc(unsigned int id)
{
	DATA_TYPE dummy;
	const TOOL_FUNC *func;
	if (_tpl->get_data_ptr(id, ID_SPACE_FUNC, dummy, func) != NULL)
	{       
		HometownToolFunc *body = HometownToolFunc::MakeToolFunc(func->type);
		if (body != NULL)
		{
			body->SetTemplate(func);
			_map[id] = body; 
		}
	}
}

void HometownItemFuncMan::Init()
{
	_tpl = HometownTemplate::GetInstance();

	DATA_TYPE type;
	unsigned int id = _tpl->get_first_data_id(ID_SPACE_FUNC, type);
	for (; id != 0; id=_tpl->get_next_data_id(ID_SPACE_FUNC, type))
	{       
		switch (type)
		{       
			case DT_TOOL_FUNC:
				MakeToolFunc(id);
				break;  
			default:
				break;  
		}       
	}      
}
const HometownItemFunc * HometownItemFuncMan::GetFunc(unsigned int id) 
{
	HTIFMap::iterator it = _map.find(id);
	if (it == _map.end()) return NULL;
	return it->second;
}
};
