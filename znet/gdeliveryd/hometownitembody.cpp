#include "hometownitembody.h"
#include "hometownitemfunc.h"
#include "hometownitem.h"
#include "hometownstateobj.h"
//#include <typeinfo>

namespace GNET
{
	void CropBody::_OnSetTemplate()
	{
		FarmCropItem *item = new FarmCropItem(_ess->id, this);
		item->_grow_period = _ess->grow_period;
		item->_protect_period = _ess->protect_period;
		HometownItemMan::AddItemTemplate(_ess->id, item);
	}

	void FruitBody::_OnSetTemplate()
	{
		HometownSimpleItem *item = new HometownSimpleItem(_ess->id, this);
		HometownItemMan::AddItemTemplate(_ess->id, item);
	}

	void ToolBody::_OnSetTemplate()
	{
		for (size_t i = 0; i < sizeof(_ess->func_ids)/sizeof(_ess->func_ids[0]); ++i)
		{
			if (_ess->func_ids[i])
			{
				const HometownItemFunc *func = HometownItemFuncMan::GetInstance()->GetFunc(_ess->func_ids[i]);
				if (func != NULL) _funcs.push_back(func);
			}
		}

		HometownSimpleItem *item = new HometownSimpleItem(_ess->id, this);
		HometownItemMan::AddItemTemplate(_ess->id, item);
	}

/*
		unsigned int _id;
		int _pos;
		int _count;
		int _max_count;
		Octets _data;
*/
	void CropBody::FillItemData(HometownItemData &idata) const
	{
		idata._id = _ess->id;
		idata._max_count = 1;
	}

	void FruitBody::FillItemData(HometownItemData &idata) const
	{
		idata._id = _ess->id;
		idata._max_count = _ess->pile_limit;
	}

	void ToolBody::FillItemData(HometownItemData &idata) const
	{
		idata._id = _ess->id;
		idata._max_count = _ess->pile_limit;
	}

	int ToolBody::CanUse(const HometownPlayerObj *user, const HometownItem *item) const
	{
		if (user->IsSelfInCoolDown(_ess->cool_type))
			return HT_ERR_COOLDOWN;
		std::vector<const HometownItemFunc *>::const_iterator it, ie = _funcs.end();
		for (it = _funcs.begin(); it != ie; ++it)
		{
			int res = (*it)->CanPerform(user, item);
			if (res != HT_ERR_SUCCESS)
				return res;
		}
		return HT_ERR_SUCCESS;
	}

	int ToolBody::CanUse(const HometownPlayerObj *user, const HometownItem *item, const HometownTargetObj *target) const
	{
		if (target->IsSponsorInCoolDown(user->GetID(), _ess->cool_type))
			return HT_ERR_COOLDOWN;
		std::vector<const HometownItemFunc *>::const_iterator it, ie = _funcs.end();
		for (it = _funcs.begin(); it != ie; ++it)
		{
			int res = (*it)->CanPerform(user, item, target);
			if (res != HT_ERR_SUCCESS)
				return res;
		}
		return HT_ERR_SUCCESS;
	}

	bool ToolBody::OnUse(HometownPlayerObj *user, HometownItem *item) const
	{
		std::vector<const HometownItemFunc *>::const_iterator it, ie = _funcs.end();
		for (it = _funcs.begin(); it != ie; ++it)
		{
			(*it)->Perform(user, item);
		}
		user->SetSelfCoolDown(_ess->cool_type, _ess->cool_time);
		return true;
	}

	bool ToolBody::OnUse(HometownPlayerObj *user, HometownItem *item, HometownTargetObj *target) const
	{
		std::vector<const HometownItemFunc *>::const_iterator it, ie = _funcs.end();
		for (it = _funcs.begin(); it != ie; ++it)
		{
			(*it)->Perform(user, item, target);
		}
		target->SetSponsorCoolDown(user->GetID(), _ess->cool_type, _ess->cool_time);
		return true;
	}

	template <typename ITEMBODY, typename ESSENCE>
	void HometownItemBodyMan::MakeBody(unsigned int id)
	{
		DATA_TYPE dummy;
		const ESSENCE *ess;
		if (_tpl->get_data_ptr(id, ID_SPACE_ESSENCE, dummy, ess) != NULL)
		{
			ITEMBODY *body = new ITEMBODY();
			body->SetTemplate(ess);
			_map[id] = body;
		}
	}

	void HometownItemBodyMan::Init()
	{
		_tpl = HometownTemplate::GetInstance();

		DATA_TYPE type;
		unsigned int id = _tpl->get_first_data_id(ID_SPACE_ESSENCE, type);
		for (; id != 0; id=_tpl->get_next_data_id(ID_SPACE_ESSENCE, type))
		{
			switch (type)
			{
			case DT_CROP_ESSENCE:
				MakeBody<CropBody, CROP_ESSENCE>(id);
				break;
			case DT_FRUIT_ESSENCE:
				MakeBody<FruitBody, FRUIT_ESSENCE>(id);
				break;
			case DT_TOOL_ESSENCE:
				MakeBody<ToolBody, TOOL_ESSENCE>(id);
				break;
			default:
				break;
			}
		}
	}

	const HometownItemBody * HometownItemBodyMan::GetBody(unsigned int id)
	{
		HTIBMap::iterator it = _map.find(id);
		if (it == _map.end()) return NULL;
		return it->second;
	}
};
