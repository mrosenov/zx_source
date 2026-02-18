#ifndef _GNET_HOMETOWN_ACTION_POLICY_H_
#define _GNET_HOMETOWN_ACTION_POLICY_H_

#include <set>
#include <map>
#include <algorithm>
#include "hometowndef.h"
#include <assert.h>

namespace GNET
{
	class HometownStateObj;
	class HometownActionPolicy
	{
		typedef std::map<int, const HometownActionPolicy *> STUBMAP;
		static STUBMAP _stubs;

	protected:
		std::set<int> _accept_states;				//只要目标处在该状态之一就能执行该操作
		std::set<int> _reject_states;				//只要目标处在该状态之一就不能执行改操作

		void AddRejectState(int state) { _reject_states.insert(state); }
		void AddAcceptState(int state) { _accept_states.insert(state); }

		HometownActionPolicy(int id)
		{
			assert(_stubs.find(id) == _stubs.end());
			_stubs[id] = this;
		}

		virtual int _CanPerform(const HometownStateObj *target) const { return HT_ERR_SUCCESS; }

	public:
		virtual ~HometownActionPolicy() {}
		static const HometownActionPolicy *GetActionPolicy(int id)
		{
			STUBMAP::iterator it = _stubs.find(id);
			if (it != _stubs.end()) 
				return it->second;
			else
				return NULL;
		}

		int CanPerform(const HometownStateObj *target, bool must_accept=true) const;
	};

	class FarmViewPolicy : public HometownActionPolicy
	{
	public:
		FarmViewPolicy() : HometownActionPolicy(HTF_ACTION_VIEW)
		{
		}
	};
	
	class FarmPlowPolicy : public HometownActionPolicy
	{
	public:
		FarmPlowPolicy() : HometownActionPolicy(HTF_ACTION_PLOW)
		{
			AddAcceptState(HTF_STATE_CULT);
			AddAcceptState(HTF_STATE_PLANT);
		}
	};

	class FarmSowPolicy : public HometownActionPolicy
	{
	public:
		FarmSowPolicy() : HometownActionPolicy(HTF_ACTION_SOW)
		{
			AddAcceptState(HTF_STATE_PLOW);
		}
	};
	class FarmWaterPolicy : public HometownActionPolicy
	{
	public:
		FarmWaterPolicy() : HometownActionPolicy(HTF_ACTION_WATER)
		{
			AddAcceptState(HTF_STATE_DRY);
		}
	};
	class FarmPestPolicy : public HometownActionPolicy
	{
	public:
		FarmPestPolicy() : HometownActionPolicy(HTF_ACTION_PEST)
		{
			AddAcceptState(HTF_STATE_PEST);
		}
	};
	class FarmWeedPolicy : public HometownActionPolicy
	{
	public:
		FarmWeedPolicy() : HometownActionPolicy(HTF_ACTION_WEED)
		{
			AddAcceptState(HTF_STATE_WEED);
		}
	};
	class FarmHarvestPolicy : public HometownActionPolicy
	{
	public:
		FarmHarvestPolicy() : HometownActionPolicy(HTF_ACTION_HARVEST)
		{
			AddAcceptState(HTF_STATE_MATURE);
		}
	};
	class FarmStealPolicy : public HometownActionPolicy
	{
	public:
		FarmStealPolicy() : HometownActionPolicy(HTF_ACTION_STEAL)
		{
			for (int i = HTF_STATE_CROPPROTECTBEGIN; i <= HTF_STATE_CROPPROTECTEND; ++i)
				AddRejectState(i);
			for (int i = HTF_STATE_FARMPROTECTBEGIN; i <= HTF_STATE_FARMPROTECTEND; ++i)
				AddRejectState(i);
			AddAcceptState(HTF_STATE_MATURE);
		}
	};
	class FarmCultPolicy : public HometownActionPolicy
	{
	public:
		FarmCultPolicy() : HometownActionPolicy(HTF_ACTION_CULT)
		{
			AddAcceptState(HTF_STATE_INCULT);
		}
	};
};

#endif
