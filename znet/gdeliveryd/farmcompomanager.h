#ifndef _GNET_FARM_COMPO_MANAGER_H_
#define _GNET_FARM_COMPO_MANAGER_H_ 

#include "hometownmanager.h"
#include "hometowndef.h"
#include "hometownstateobj.h"
#include "substance.h"
#include "marshal.h"
#include "hometowndef.h"
#include "hometowntemplate.h"
#include "hometownitem.h"
#include "hometownpacket.h"
#include "hometowndynamic.h"
#include "hometownprotocol.h"
#include <map>
#include <assert.h>
#include <algorithm>

namespace GNET
{

class FarmField;
class FarmCrop : public HometownTargetObj
{
public:
	int _id;
	int _seed_time;			//播种时间
	int _grow_time;			//已经生长时间
	int _grow_period;		//生长期
	int _grow_stop_end;		//上次计算停止生长的时间点
	int _state_grow_time;	//在当前状态下已经生长的时间
	int _protect_period[6];	//剩余防偷保护期
	int _fruit_left;		//剩余果实数
	int _fruit_total;		//总的果实数
	FarmField *_field;

	FarmCrop(FarmField *field):_id(0),_seed_time(0),_grow_time(0),_grow_period(0),_grow_stop_end(0),
							_state_grow_time(0),_fruit_left(0),_fruit_total(0),_field(field)
	{
		memset(_protect_period, 0, sizeof(_protect_period));
	}

	static const CROP_ESSENCE *GetEssence(int id)
	{
		const CROP_ESSENCE *ess;
		DATA_TYPE dummy;
		HometownTemplate::GetInstance()->get_data_ptr(id, ID_SPACE_ESSENCE, dummy, ess);
		return ess;
	}

	void SetDirty(bool dirty); 

	int Sow(int id);		//刚进行播种

	int GetID() { return _id; }

	//如果目前阶段的生长期已满，返回生长期满的时刻, 否则返回-1
	int CalcGrowTime()
	{
		if (Timer::GetTime() <= _grow_stop_end) return -1;
		std::map<int, int> states;
		GetCurrentStates(states);
		int grow_stop_begin = Timer::GetTime();
		std::set<int> grow_stop_states;				//哪些状态会让作物停止生长
		grow_stop_states.insert(HTF_STATE_DRY);
		grow_stop_states.insert(HTF_STATE_PEST);
		grow_stop_states.insert(HTF_STATE_WEED);

		std::set<int>::iterator it, ie = grow_stop_states.end();
		for (it = grow_stop_states.begin(); it != ie; ++it)
		{
			std::map<int, int>::iterator it_find = states.find(*it);
			if (it_find != states.end())
			{
				if (grow_stop_begin > it_find->second)
					grow_stop_begin = it_find->second;
			}
		}

		int grow_transit_time = -1;
		if (_grow_stop_end < grow_stop_begin)
		{
			int grow_time = grow_stop_begin-_grow_stop_end;
			_grow_time += grow_time;

			int state_period = _grow_period/3;		//每个生长阶段的时间
			if (_state_grow_time+grow_time >= state_period)
				grow_transit_time = _grow_stop_end+state_period-_state_grow_time;

			_state_grow_time += grow_time;
		}
		_grow_stop_end = Timer::GetTime();
		return grow_transit_time;
	}

	//计算成熟时候的果实数
	void CalcFruit(int mature_time);

	void Dump()
	{
		HometownTargetObj::Dump();
		LOG_TRACE("grow_time: %d, state_grow_time: %d\n", _grow_time, _state_grow_time);
		LOG_TRACE("protect_period[0]: %d\n", _protect_period[0]);
		LOG_TRACE("fruit_total: %d, fruit_left: %d\n", _fruit_total, _fruit_left);
	}
};

class Farm;
//一块农场耕地
class FarmField : public HometownTargetObj
{
	inline int CanCropAction(int action, bool must_accept=true) const
	{
		if (_has_crop)
			return _crop.CanAction(action, must_accept);
		else
			return HT_ERR_HASNOCROP;
	}

	inline void PreCropAction(int action, void *action_param, bool &changed)
	{
		bool crop_changed = false, field_changed = false;
		if (_has_crop)
			_crop.PreAction(action, action_param, crop_changed);
		PreAction(HTF_ACTION_NONE, field_changed);
		changed = crop_changed||field_changed;
	}

	inline void PreCropAction(int action, bool &changed) 
	{ 
		return PreCropAction(action, NULL, changed); 
	}
	
	inline int CanFieldAction(int action, bool must_accept=true) const
	{
		return CanAction(action, must_accept);
	}

	inline void PreFieldAction(int action, void *action_param, bool &changed)
	{
		bool crop_changed = false, field_changed = false;
		PreAction(action, action_param, field_changed);
		if (_has_crop)
			_crop.PreAction(HTF_ACTION_NONE, crop_changed);
		changed = crop_changed||field_changed;
	}

	inline void PreFieldAction(int action, bool &changed) 
	{ 
		return PreFieldAction(action ,NULL, changed); 
	}

	inline int CanFieldCropAction(int action, bool field_must_accept=true, bool crop_must_accept=true) const
	{
		if (_has_crop)
		{
			int res = _crop.CanAction(action, crop_must_accept);
			if (res != HT_ERR_SUCCESS) return res;
		}
		int res = CanAction(action, field_must_accept);
		if (res != HT_ERR_SUCCESS) return res;
		return HT_ERR_SUCCESS;
	}

	inline void PreFieldCropAction(int action, void *action_param, bool &changed)
	{
		bool crop_changed = false, field_changed = false;
		PreAction(action, action_param, field_changed);
		if (_has_crop)
			_crop.PreAction(action, action_param, crop_changed);
		changed = crop_changed||field_changed;
	}

	inline void PreFieldCropAction(int action, bool &changed) 
	{ 
		return PreFieldCropAction(action, NULL, changed); 
	}
	
public:
	int _id;				//field_id
	char _type;				//0:普通地，1:爱心地
	int _friend;			//如果是爱心地，种植这块地的人
	Octets _friend_name;
	bool _has_crop;
	FarmCrop _crop;
	Farm *_farm;

	FarmField(int id=0, char type=0, Farm *farm=NULL):_id(id), _type(type), _friend(0), _has_crop(false), _crop(this),_farm(farm){}
	int GetID() { return _id; }
	bool Init(bool cult);

	void SetDirty(bool dirty);
	FarmCrop *GetCrop() { return &_crop; }

	inline int CanSow() const
	{
		return CanFieldAction(HTF_ACTION_SOW);
	}
	inline int Sow(int sponsor, int id, bool &changed)
	{
		int res = _crop.Sow(id);
		if (res != HT_ERR_SUCCESS) return res;
		PreAction(HTF_ACTION_SOW, &id, changed);
		_crop.ClearCoolDown();
		ClearCoolDown();							//重新播种后清除冷却
		_has_crop = true;
		if (_type == HTF_FIELD_TYPE_LOVE)
		{
			_friend = sponsor;
			HometownManager::GetInstance()->GetPlayerName(_friend, _friend_name);
		}
		changed = true;
		return HT_ERR_SUCCESS;
	}

	inline int CanView() const
	{
		return CanFieldCropAction(HTF_ACTION_VIEW, false, false);
	}
	inline void View(bool &changed) 			
	{ 
		PreFieldCropAction(HTF_ACTION_VIEW, changed);
	}
	inline int CanPlow() const
	{
		return CanFieldAction(HTF_ACTION_PLOW);
	}
	inline void Plow(bool &changed) 
	{ 
		PreFieldAction(HTF_ACTION_PLOW, changed); 
	}
	inline int CanPest() const
	{
		return CanCropAction(HTF_ACTION_PEST);
	}
	inline void Pest(bool &changed) 
	{ 
		PreCropAction(HTF_ACTION_PEST, changed); 
	}
	inline int CanWater() const
	{
		return CanCropAction(HTF_ACTION_WATER);
	}
	inline void Water(bool &changed) 
	{ 
		PreCropAction(HTF_ACTION_WATER, changed); 
	}
	inline int CanWeed() const
	{
		return CanCropAction(HTF_ACTION_WEED);
	}
	inline void Weed(bool &changed) 
	{ 
		PreCropAction(HTF_ACTION_WEED, changed); 
	}
	inline int CanHarvest() const
	{ 
		return CanCropAction(HTF_ACTION_HARVEST); 
	}
	inline void Harvest(bool &changed) 
	{ 
		return PreCropAction(HTF_ACTION_HARVEST, changed); 
	}
	inline int CanSteal() const
	{ 
		return CanFieldCropAction(HTF_ACTION_STEAL, false, true); 
	}
	inline void Steal(bool &changed) 
	{ 
		PreCropAction(HTF_ACTION_STEAL, changed); 
	}
	//cult只会在玩家升级时由系统调用
	inline int CanCult() const
	{ 
		return CanFieldAction(HTF_ACTION_CULT); 
	}
	inline void Cult(bool &changed) 
	{ 
		PreFieldAction(HTF_ACTION_CULT, changed); 
	}

	void Dump()
	{
		LOG_TRACE("--------Field %d States------\n", _id);
		HometownStateObj::Dump();
		if (_has_crop)
		{
			LOG_TRACE("-----Crop %d States-----\n", _id);
			_crop.Dump();
			LOG_TRACE("\n");
		}
	}
};

class FarmManager;
class FarmPlayer : public HometownPlayerObj
{
public:
	int _id;
	int64_t _charm;
	int64_t _money;
	int _level;
	HometownItemMan _pocket;
	FarmManager *_fm;

	FarmPlayer(FarmManager *fm):_id(0),_charm(0),_money(0), _level(1), _fm(fm){}

	int GetID() const { return _id; }
	void SetDirty(bool dirty); 
};

//状态到期后变为下一状态，比如不干旱-->干旱
template <typename STATE_OBJ, typename CUR_STATE, typename NEXT_STATE>
class ExpirePolicy : public ParamPolicy<STATE_OBJ, CUR_STATE, NEXT_STATE>
{
protected:
	bool operator () (STATE_OBJ *obj, CUR_STATE *s, int action, const void *action_param, NEXT_STATE *&next)
	{
		if (Timer::GetTime() >= s->_expire_time)
		{
			next = new NEXT_STATE(s->_expire_time);
			return true;
		}
		return false;
	}
};

//超时后消失，不会变迁到下一状态
template <typename STATE_OBJ, typename CUR_STATE>
class ExpireGonePolicy : public ParamPolicy<STATE_OBJ, CUR_STATE, CUR_STATE>
{
protected:
	bool operator () (STATE_OBJ *obj, CUR_STATE *s, int action, const void *action_param, CUR_STATE *&next)
	{
		if (Timer::GetTime() >= s->_expire_time)
		{
			next = NULL;
			return true;
		}
		return false;
	}
};

/*
   状态到期后按一定概率变迁，比如不长虫-->长虫,不长草-->长草
*/
template <typename STATE_OBJ, typename CUR_STATE, typename NEXT_STATE>
class ExpireRandomPolicy : public ParamPolicy<STATE_OBJ, CUR_STATE, NEXT_STATE>
{
protected:
	bool operator () (STATE_OBJ *obj, CUR_STATE *s, int action, const void *action_param, NEXT_STATE *&next)
	{
		const static int HOUR12 = 12*3600;
		int now = Timer::GetTime();
		if (now <= s->_save_policy_time) return false;
		int diff = now-s->_expire_time;
		bool result = false;

		if (diff >= HOUR12)				//大于12小时一定长草或长虫
			result = true;
		else if ((rand()%HOUR12)<diff)
			result = true;
		
		if (result)
		{
			int transit_time = rand()%(now-s->_save_policy_time)+s->_save_policy_time+1;
			next = new NEXT_STATE(transit_time);
		}

		s->_save_policy_time = now;			//now之前的随机时间都已算过，下次不再重复计算
		return result;
	}
};

template <typename STATE_OBJ, typename CUR_STATE, int ACTION, typename NEXT_STATE>
class ActionPolicy : public ParamPolicy<STATE_OBJ, CUR_STATE, NEXT_STATE>
{
protected:
	bool operator () (STATE_OBJ *obj, CUR_STATE *s, int action, const void *action_param, NEXT_STATE *&next)
	{
		if (action == ACTION)
		{
			next = new NEXT_STATE(Timer::GetTime());
			return true;
		}
		return false;
	}
};

template <typename CUR_STATE, typename NEXT_STATE>
class GrowPolicy : public ParamPolicy<FarmCrop, CUR_STATE, NEXT_STATE>
{
protected:
	bool operator () (FarmCrop *crop, CUR_STATE *s, int action, const void *action_param, NEXT_STATE *&next)
	{
		int state_period = crop->_grow_period/3;
		if (crop->_state_grow_time >= state_period)
		{
			next = new NEXT_STATE(Timer::GetTime()<s->_start_time+state_period ? Timer::GetTime() : s->_start_time+state_period);
			crop->_state_grow_time -= state_period;
			return true;
		}
		int grow_transit_time = crop->CalcGrowTime();
		if (grow_transit_time != -1)
		{
			crop->_state_grow_time -= state_period;
			next = new NEXT_STATE(grow_transit_time);
			return true;
		}
		else
		{
			s->_start_time = s->_expire_time = Timer::GetTime()-crop->_state_grow_time;
			return false;
		}
	}
};

class DryState;
class DryFreeState;
class PestState;
class PestFreeState;
class WeedState;
class WeedFreeState;
class WiltState;
class SeedState;
class SproutState;
class ImmatureState;
class PrematureState;
class MatureState;
class HarvestState;
class CropProtectState;

typedef ExpirePolicy<FarmCrop, DryFreeState, DryState> DryPolicy;
typedef ActionPolicy<FarmCrop, DryState, HTF_ACTION_WATER, DryFreeState> DryFreePolicy;
typedef ExpirePolicy<FarmCrop, DryState, WiltState> DryWiltPolicy;
typedef ExpireRandomPolicy<FarmCrop, PestFreeState, PestState> PestPolicy;
typedef ActionPolicy<FarmCrop, PestState, HTF_ACTION_PEST, PestFreeState> PestFreePolicy;
typedef ExpirePolicy<FarmCrop, PestState, WiltState> PestWiltPolicy;
typedef ExpireRandomPolicy<FarmCrop, WeedFreeState, WeedState> WeedPolicy;
typedef ActionPolicy<FarmCrop, WeedState, HTF_ACTION_WEED, WeedFreeState> WeedFreePolicy;
typedef ExpirePolicy<FarmCrop, WeedState, WiltState> WeedWiltPolicy;
typedef ExpirePolicy<FarmCrop, SeedState, SproutState> SproutPolicy;
typedef GrowPolicy<SproutState, ImmatureState> ImmaturePolicy;
typedef GrowPolicy<ImmatureState, PrematureState> PrematurePolicy;
typedef GrowPolicy<PrematureState, MatureState> MaturePolicy;
typedef ExpirePolicy<FarmCrop, MatureState, WiltState> MatureWiltPolicy;
typedef ActionPolicy<FarmCrop, MatureState, HTF_ACTION_HARVEST, HarvestState> HarvestPolicy;
typedef ExpireGonePolicy<FarmCrop, CropProtectState> CropProtectGonePolicy;

template <typename POLICY>
class PolicySingleton
{
public:
	static POLICY *GetPolicy()
	{
		static POLICY inst;
		return &inst;
	}
};

//---------------作物的各种状态-------------------
class DryFreeState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(DryFreeState)
	DryFreeState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_DRY)
	{
		AddPolicy((StatePolicy *)PolicySingleton<DryPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_DRYFREE; }
};

class DryState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(DryState)
	DryState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_WILT)
	{
		AddPolicy((StatePolicy *)PolicySingleton<DryWiltPolicy>::GetPolicy());
		AddPolicy((StatePolicy *)PolicySingleton<DryFreePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_DRY; }
protected:
	void OnLeave(State *prev, int action, const void *action_param, FarmCrop *crop)
	{
		crop->CalcGrowTime();
	}
};

class PestFreeState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(PestFreeState)
	PestFreeState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_PEST)
	{
		AddPolicy((StatePolicy *)PolicySingleton<PestPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_PESTFREE; }
};

class PestState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(PestState)
	PestState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_WILT)
	{
		AddPolicy((StatePolicy *)PolicySingleton<PestWiltPolicy>::GetPolicy());
		AddPolicy((StatePolicy *)PolicySingleton<PestFreePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_PEST; }
protected:
	void OnLeave(State *prev, int action, const void *action_param, FarmCrop *crop)
	{
		crop->CalcGrowTime();
	}
};

class WeedFreeState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(WeedFreeState)
	WeedFreeState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_WEED)
	{
		AddPolicy((StatePolicy *)PolicySingleton<WeedPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_WEEDFREE; }
};
class WeedState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(WeedState)
	WeedState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_WILT)
	{
		AddPolicy((StatePolicy *)PolicySingleton<WeedWiltPolicy>::GetPolicy());
		AddPolicy((StatePolicy *)PolicySingleton<WeedFreePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_WEED; }
protected:
	void OnLeave(State *prev, int action, const void *action_param, FarmCrop *crop)
	{
		crop->CalcGrowTime();
	}
};

class WiltState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(WiltState)
	WiltState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
	}
	int GetStateID() { return HTF_STATE_WILT; }
protected:
	void OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop);
};

class HarvestState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(HarvestState)
	HarvestState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
	}
	int GetStateID() { return HTF_STATE_HARVEST; }
protected:
	void OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop);
};

class CropProtectState : public ParamState<FarmCrop>
{
	char _buffid;
	int _last_heartbeat_time;
public:
	DECLARE_SUBSTANCE(CropProtectState)
	CropProtectState(char buffid=0, int start_time=Timer::GetTime(), int duration=0):ParamState<FarmCrop>(start_time, duration),_buffid(buffid),_last_heartbeat_time(start_time)
	{
		AddPolicy((StatePolicy *)PolicySingleton<CropProtectGonePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_CROPPROTECTBEGIN+_buffid; }
	OctetsStream& marshal(OctetsStream &os) const
	{
		ParamState<FarmCrop>::marshal(os);
		os << _buffid;
		os << _last_heartbeat_time;
		return os;
	}
	const OctetsStream& unmarshal(const OctetsStream &os)
	{
		ParamState<FarmCrop>::unmarshal(os);
		os >> _buffid;
		os >> _last_heartbeat_time;
		return os;
	}
protected:
	void OnLeave(State *prev, int action, const void *action_param, FarmCrop *crop);
	void OnHeartbeat(FarmCrop *crop);
};

//种子状态
class SeedState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(SeedState)
	SeedState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<SproutPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_SEED; }
};

//幼苗状态
class SproutState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(SproutState)
	SproutState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<ImmaturePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_SPROUT; }
};

//未成熟状态
class ImmatureState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(ImmatureState)
	ImmatureState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<PrematurePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_IMMATURE; }
};

//即将成熟状态
class PrematureState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(PrematureState)
	PrematureState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<MaturePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_PREMATURE; }
};

//被偷光状态
class AllStolenState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(AllStolenState)
	AllStolenState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, 0)
	{
	}
	int GetStateID() { return HTF_STATE_ALLSTOLEN; }
protected:
	void OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop);
};

class AllStolenPolicy : public ActionPolicy<FarmCrop, MatureState, HTF_ACTION_STEAL, AllStolenState>
{
protected:
	bool operator () (FarmCrop *crop, MatureState *s, int action, const void *action_param, AllStolenState *&next)
	{
		if (action == HTF_ACTION_STEAL)
		{
			if (crop->_fruit_left <= 0)
			{
				next = new AllStolenState(Timer::GetTime());
				return true;
			}
		}
		return false;
	}
};

//成熟状态
class MatureState : public ParamState<FarmCrop>
{
public:
	DECLARE_SUBSTANCE(MatureState)
	MatureState(int start_time=Timer::GetTime()):ParamState<FarmCrop>(start_time, HTF_TIME_MATUREWILT)
	{
		AddPolicy((StatePolicy *)PolicySingleton<MatureWiltPolicy>::GetPolicy());
		AddPolicy((StatePolicy *)PolicySingleton<HarvestPolicy>::GetPolicy());
		AddPolicy((StatePolicy *)PolicySingleton<AllStolenPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_MATURE; }
protected:
	void OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop);
};

//-----------------------土地的各种状态-----------------------
class IncultState;
class CultState;
class PlowState;
class PlantState;
typedef ActionPolicy<FarmField, IncultState, HTF_ACTION_CULT, CultState> CultPolicy;
typedef ActionPolicy<FarmField, CultState, HTF_ACTION_PLOW, PlowState> CultPlowPolicy;
typedef ActionPolicy<FarmField, PlowState, HTF_ACTION_SOW, PlantState> PlantPolicy;
typedef ActionPolicy<FarmField, PlantState, HTF_ACTION_PLOW, PlowState> PlantPlowPolicy;

//未开垦状态
class IncultState : public ParamState<FarmField>
{
public:
	DECLARE_SUBSTANCE(IncultState)
	IncultState(int start_time=Timer::GetTime()):ParamState<FarmField>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<CultPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_INCULT; }
};

//开垦状态
class CultState : public ParamState<FarmField>
{
public:
	DECLARE_SUBSTANCE(CultState)
	CultState(int start_time=Timer::GetTime()):ParamState<FarmField>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<CultPlowPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_CULT; }
};

//已经犁地状态
class PlowState : public ParamState<FarmField>
{
public:
	DECLARE_SUBSTANCE(PlowState)
	PlowState(int start_time=Timer::GetTime()):ParamState<FarmField>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<PlantPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_PLOW; }
protected:
	void OnEnter(State *prev, int action, const void *action_param, FarmField *field);
};

//已经耕种状态
class PlantState : public ParamState<FarmField>
{
public:
	DECLARE_SUBSTANCE(PlantState)
	PlantState(int start_time=Timer::GetTime()):ParamState<FarmField>(start_time, 0)
	{
		AddPolicy((StatePolicy *)PolicySingleton<PlantPlowPolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_PLANT; }
};

//-----------------------人的各种状态-----------------------------
class HideState;	//隐身状态
typedef ExpireGonePolicy<FarmPlayer, HideState> HideGonePolicy;
class HideState : public ParamState<FarmPlayer>
{
public:
	DECLARE_SUBSTANCE(HideState)
	HideState(int start_time=Timer::GetTime(), int duration=0):ParamState<FarmPlayer>(start_time, duration)
	{
		AddPolicy((StatePolicy *)PolicySingleton<HideGonePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_HIDE; }
};

//----整个农场的状态
class FarmProtectState;
class FarmSpeedGrowState;
typedef ExpireGonePolicy<Farm, FarmProtectState> FarmProtectGonePolicy;
typedef ExpireGonePolicy<Farm, FarmSpeedGrowState> FarmSpeedGrowGonePolicy;

class FarmProtectState : public ParamState<Farm>
{
	char _buffid;
	int _last_heartbeat_time;
public:
	DECLARE_SUBSTANCE(FarmProtectState)
	FarmProtectState(char buffid=0, int start_time=Timer::GetTime(), int duration=0):ParamState<Farm>(start_time, duration),_buffid(buffid),_last_heartbeat_time(start_time)
	{
		AddPolicy((StatePolicy *)PolicySingleton<FarmProtectGonePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_FARMPROTECTBEGIN+_buffid; }
	OctetsStream& marshal(OctetsStream &os) const
	{
		ParamState<Farm>::marshal(os);
		os << _buffid;
		os << _last_heartbeat_time;
		return os;
	}
	const OctetsStream& unmarshal(const OctetsStream &os)
	{
		ParamState<Farm>::unmarshal(os);
		os >> _buffid;
		os >> _last_heartbeat_time;
		return os;
	}
protected:
	void OnLeave(State *prev, int action, const void *action_param, Farm *crop);
	void OnHeartbeat(Farm *crop);
};

class FarmSpeedGrowState : public ParamState<Farm>
{
	char _buffid;
public:
	DECLARE_SUBSTANCE(FarmSpeedGrowState)
	FarmSpeedGrowState(char buffid=0, int start_time=Timer::GetTime(), int duration=HTF_TIME_SHOWFARMSPEEDGROW):ParamState<Farm>(start_time, duration),_buffid(buffid)
	{
		AddPolicy((StatePolicy *)PolicySingleton<FarmSpeedGrowGonePolicy>::GetPolicy());
	}
	int GetStateID() { return HTF_STATE_FARMSPEEDGROWBEGIN+_buffid; }
	OctetsStream& marshal(OctetsStream &os) const
	{
		ParamState<Farm>::marshal(os);
		os << _buffid;
		return os;
	}
	const OctetsStream& unmarshal(const OctetsStream &os)
	{
		ParamState<Farm>::unmarshal(os);
		os >> _buffid;
		return os;
	}
};


class Farm : public HometownTargetObj
{
public:
	FarmManager *_fm;
	int _protect_period[6];	//剩余防偷保护期

	Farm(FarmManager *fm):_fm(fm)
	{
		memset(_protect_period, 0, sizeof(_protect_period));
	}

	typedef std::map<int, FarmField> FieldMap;		//field id->field
	FieldMap _fields;

	void SetDirty(bool dirty);

	FarmField *GetField(int id)
	{
		FieldMap::iterator it = _fields.find(id);
		if (it != _fields.end())
			return &it->second;
		else
			return NULL;
	}

	bool HasPlantLoveField(int roleid) const
	{
		FieldMap::const_iterator it, ie = _fields.end();
		for (it = _fields.begin(); it != ie; ++it)
		{
			const FarmField &field = it->second;
			if (field._type == HTF_FIELD_TYPE_LOVE
					&& field._friend == roleid)
				return true;
		}
		return false;
	}

	int GetPlantLoveFieldNum(int roleid) const
	{
		int num = 0;
		FieldMap::const_iterator it, ie = _fields.end();
		for (it = _fields.begin(); it != ie; ++it)
		{
			const FarmField &field = it->second;
			if (field._type == HTF_FIELD_TYPE_LOVE
					&& field._friend == roleid)
				++num;
		}
		return num;
	}

	bool HasMatureField() const
	{
		FieldMap::const_iterator it, ie = _fields.end();
		for (it = _fields.begin(); it != ie; ++it)
		{
			const FarmField &field = it->second;
			if (field._type == HTF_FIELD_TYPE_NORMAL
					&& field._has_crop 
					&& field._crop.IsInState(HTF_STATE_MATURE))
				return true;
		}
		return false;
	}

	inline int CanView() const			//只检查farm是否允许view，不检查每块地
	{
		return CanAction(HTF_ACTION_VIEW, false);
	}

	void View(bool &changed)
	{
		PreAction(HTF_ACTION_VIEW, changed);
		FieldMap::iterator it = _fields.begin(), ie = _fields.end();
		for (; it != ie; ++it)
		{
			bool field_changed = false;
			it->second.View(field_changed);
			if (field_changed) changed = true;
		}
	}
	inline int CanSowField(const FarmField *field)
	{
		return field->CanSow();
	}
	inline int SowField(int sponsor, int id, FarmField *field, bool &changed)
	{
		return field->Sow(sponsor, id, changed);
	}
	inline int CanPlowField(const FarmField *field)
	{
		return field->CanPlow();
	}
	inline void PlowField(FarmField *field, bool &changed)
	{
		field->Plow(changed);
	}
	inline int CanPestField(const FarmField *field)
	{
		return field->CanPest();
	}
	inline void PestField(FarmField *field, bool &changed)
	{
		field->Pest(changed);
	}
	inline int CanWaterField(const FarmField *field)
	{
		return field->CanWater();
	}
	inline void WaterField(FarmField *field, bool &changed)
	{
		field->Water(changed);
	}
	inline int CanWeedField(const FarmField *field)
	{
		return field->CanWeed();
	}
	inline void WeedField(FarmField *field, bool &changed)
	{
		field->Weed(changed);
	}
	inline int CanHarvestField(const FarmField *field)
	{
		return field->CanHarvest();
	}
	inline void HarvestField(FarmField *field, bool &changed)
	{
		field->Harvest(changed);
	}
	inline int CanStealField(const FarmField *field)
	{
		int res = CanAction(HTF_ACTION_STEAL, false);
		if (res != HT_ERR_SUCCESS) return res;
		return field->CanSteal();
	}
	inline void StealField(FarmField *field, bool &changed)
	{
		field->Steal(changed);
	}

	void Dump()
	{
		LOG_TRACE("-----------------FARM STATES--------------\n");
		HometownStateObj::Dump();
		FieldMap::iterator it = _fields.begin(), ie = _fields.end();
		for (; it != ie; ++it)
		{
			it->second.Dump();
		}
	}
};

class GFarmCompoInfo;
class FarmManager
{
public:
	FarmPlayer _owner;
	Farm _farm;
	bool _dirty;
	FarmDynamic _farm_dynamic;
	FarmDynamic _action_dynamic;

	FarmManager():_owner(this),_farm(this),_dirty(false) {}

	bool IsDirty() const { return _dirty; }
	void SetDirty(bool dirty=true) { _dirty = dirty; }

	void InitFarm(int roleid, HOMETOWN::WRAPPER &response);
	void View(HOMETOWN::WRAPPER &response);
	void ViewField(int field_id, HOMETOWN::WRAPPER &response);
	void SowField(int sponsor, int seed_id, int field_id, HOMETOWN::WRAPPER &response);
	void PlowField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response);
	void PestField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response);
	void WaterField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response);
	void WeedField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response);
	void HarvestField(int sponsor, int field_id, HOMETOWN::WRAPPER &response);
	void StealField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response);

	void ListPocket(HOMETOWN::WRAPPER &response);
	void LevelUp(HOMETOWN::WRAPPER &response);	
	void BuyItem(int item_id, int count, HOMETOWN::WRAPPER &response);
	//对自己用
	void UseItem(int pos, HOMETOWN::WRAPPER &response, const HOMETOWN::C2S::CMD::use_item *pkt);	
	//对其他目标用
	void UseItem(int pos, HometownTargetObj *target, HOMETOWN::WRAPPER &response, const HOMETOWN::C2S::CMD::use_item *pkt);
	void SellItem(int pos, int count, HOMETOWN::WRAPPER &response, const HOMETOWN::C2S::CMD::sell_item *pkt);
	void ExchgMoney(char type, int amount, HOMETOWN::WRAPPER &response);

	template <typename T>
	void LogFarmDynamic(int action, const T &dynamic, int time=(int)Timer::GetTime())
	{
		_farm_dynamic.MergeLog(time, action, dynamic);
	}

	template <typename T>
	void LogActionDynamic(int action, const T &dynamic, int time=(int)Timer::GetTime())
	{
		_action_dynamic.MergeLog(time, action, dynamic);
	}

	void ToGFarmCompoInfo(GFarmCompoInfo &ginfo);
	void FromGFarmCompoInfo(int roleid, const GFarmCompoInfo &ginfo);

	void Dump()
	{
		_owner.Dump();
		_farm.Dump();
	}
};

//农场管理
class FarmCompoManager : public BaseCompoManager
{
	typedef std::map<int, FarmManager> FarmMap;		//roleid->farmmanager
	FarmMap _farm_map;
	const FARM_CONFIG *_config;

	FarmCompoManager() {}

public:
	static FarmCompoManager *GetInstance()
	{
		static FarmCompoManager inst;
		return &inst;
	}

	FarmManager *GetFarm(int owner)
	{
		FarmMap::iterator it = _farm_map.find(owner);
		return it!=_farm_map.end() ? &it->second : NULL;
	}
	const FarmManager *GetFarm(int owner) const
	{
		FarmMap::const_iterator it = _farm_map.find(owner);
		return it!=_farm_map.end() ? &it->second : NULL;
	}
	const FARM_CONFIG &GetConfig() { return *_config; }
	int64_t GetUpgradeCharm(int level)//得到从level到level+1需要的魅力值
	{
		assert(level>=1 && level<(int)_config->upgrade_config.size());
		return _config->upgrade_config[level];
	}
	const LEVEL_CONFIG &GetLevelConfig(int level)
	{
		size_t index;
		for (index = 0; index < _config->level_config.size()-1; ++index)
		{
			const LEVEL_CONFIG &curcfg = _config->level_config[index];
			const LEVEL_CONFIG &nextcfg = _config->level_config[index+1];
			if (level>=curcfg.level && level<nextcfg.level)
				break;
		}
		const LEVEL_CONFIG &lvl_cfg = _config->level_config[index];
		assert(level >= lvl_cfg.level);
		return lvl_cfg;
	}
	bool IsCrop(int id)
	{
		return std::binary_search(_config->crop_list.begin(), _config->crop_list.end(), id);
	}
	bool IsTool(int id)
	{
		return std::binary_search(_config->tool_list.begin(), _config->tool_list.end(), id);
	}
	int GetPlantLoveFieldNum(int roleid) const;		//得到某一个玩家在好友家种植的爱心地的总数目
	int GetCompoID() const { return _config->id; }
	bool Init();
	void HandleCmd(int roleid, int cmd_type, void *data, int size, HOMETOWN::WRAPPER &response);
	void OnLoad(int roleid, const Octets &data);
	void ErasePlayer(int roleid) { _farm_map.erase(roleid); }
	bool IsDirty(int roleid)
	{
		FarmManager *fm = GetFarm(roleid);
		return fm==NULL?false:fm->IsDirty();
	}
	void SyncToDB(int roleid);
	void OnSyncToDB(int retcode, int roleid);
	void Update() {}
	void Dump(int roleid)
	{
		FarmManager *fm = GetFarm(roleid);
		if (fm != NULL) fm->Dump();
	}
};

};
#endif
