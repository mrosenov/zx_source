#include "farmcompomanager.h"
#include "hometownprotocol.h"
#include "hometownproto_imp.h"
#include "hometownpacket.h"
#include "hometowntemplate.h"
#include "hometowndef.h"
#include "hometownitembody.h"
#include <assert.h>
#include "marshal.h"
#include "gfarmcompoinfo"
#include "dbhometownputcompoinfo.hrp"
#include "gamedbclient.hpp"
#include "mapuser.h"

namespace GNET
{
//从[low, high]中随机选择一个
static int rand_between(int low, int high)
{
	assert(high>=low);
	if (high == low) 
		return low;
	else
		return rand()%(high-low+1)+low;
}

//庄稼状态
DEFINE_SUBSTANCE(HT_CLSID_STATE_DRYFREE, DryFreeState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_DRY, DryState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_PESTFREE, PestFreeState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_PEST, PestState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_WEEDFREE, WeedFreeState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_WEED, WeedState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_WILT, WiltState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_HARVEST, HarvestState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_CROPPROTECT, CropProtectState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_SEED, SeedState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_SPROUT, SproutState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_IMMATURE, ImmatureState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_PREMATURE, PrematureState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_MATURE, MatureState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_ALLSTOLEN, AllStolenState, State)

//土地状态
DEFINE_SUBSTANCE(HT_CLSID_STATE_INCULT, IncultState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_CULT, CultState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_PLOW, PlowState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_PLANT, PlantState, State)

//人的状态
DEFINE_SUBSTANCE(HT_CLSID_STATE_HIDE, HideState, State)

//整个农场状态
DEFINE_SUBSTANCE(HT_CLSID_STATE_FARMPROTECT, FarmProtectState, State)
DEFINE_SUBSTANCE(HT_CLSID_STATE_FARMSPEEDGROW, FarmSpeedGrowState, State);

void FarmCrop::SetDirty(bool dirty)
{ 
	if (dirty) _field->_farm->_fm->SetDirty(true);
}

int FarmCrop::Sow(int id) 
{
	const CROP_ESSENCE *ess = GetEssence(id);
	if (ess == NULL) return HT_ERR_SEEDNOTFOUND;
	_id = id;
	_seed_time = Timer::GetTime();
	_grow_time = 0;
	_grow_period = ess->grow_period;
	_grow_stop_end = _seed_time;
	_state_grow_time = 0;
	_protect_period[0] = ess->protect_period;
	_fruit_left = 0;
	_fruit_total = 0;

	STATE_SET states;
	states.insert(new DryFreeState());
	states.insert(new WeedFreeState());
	states.insert(new PestFreeState());
	states.insert(new SeedState());
	HometownStateObj::Init(states);
	return HT_ERR_SUCCESS;
}

void FarmCrop::CalcFruit(int mature_time)
{
	/*min+ran（0.9，1.1）*生长期/总时间*(max-min)*/
	int total_grow_time = mature_time-_seed_time;
	if (total_grow_time < 1) total_grow_time = 1;
	const CROP_ESSENCE *ess = GetEssence(_id);
	assert(ess != NULL);
	int fruit = ess->fruit_min+(int)((float)rand_between(90,110)/100*_grow_period/total_grow_time*(ess->fruit_max-ess->fruit_min));
	if (fruit<ess->fruit_min) fruit = ess->fruit_min;
	if (fruit>ess->fruit_max) fruit = ess->fruit_max;
	_fruit_left = _fruit_total = fruit;
}

void FarmField::SetDirty(bool dirty)
{
	if (dirty) _farm->_fm->SetDirty(true); 
}

bool FarmField::Init(bool cult)
{
	STATE_SET states;
	if (cult)
		states.insert(new CultState());
	else
		states.insert(new IncultState());
	return HometownStateObj::Init(states);
}

void Farm::SetDirty(bool dirty)
{
	if (dirty) _fm->SetDirty(true); 
}

void FarmPlayer::SetDirty(bool dirty)
{
	if (dirty) _fm->SetDirty(true); 
}

#define CHECK_RES(res) \
do\
{\
	if ((res) != HT_ERR_SUCCESS)\
	{\
		response.set_retcode(res);\
		return;\
	}\
}\
while(0)

void FarmManager::InitFarm(int roleid, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmCompoManager *fcm = FarmCompoManager::GetInstance();
	const FARM_CONFIG &config = fcm->GetConfig();
	_owner._id = roleid;
	_owner._fm = this;
	_owner._money = 100;			//初始给100百草园币
	_owner._pocket.SetCapacity(config.init_pocket_caps);
	_farm._fm = this;

	const LEVEL_CONFIG &lvl_cfg = fcm->GetLevelConfig(_owner._level);
	int i;
	for (i = 0; i < lvl_cfg.normal_field_count; ++i)
	{
		FarmField &field = _farm._fields[i];
		field._farm = &_farm;
		field._id = i;
		field._type = HTF_FIELD_TYPE_NORMAL;
		field._crop._field = &field;
		field.Init(true);
	}
	for (int j = 0; j < lvl_cfg.love_field_count; ++i, ++j)
	{
		FarmField &field = _farm._fields[i];
		field._farm = &_farm;
		field._id = i;
		field._type = HTF_FIELD_TYPE_LOVE;
		field._crop._field = &field;
		field.Init(true);
	}

	for (; i < config.max_field_count; ++i)
	{
		FarmField &field = _farm._fields[i];
		field._farm = &_farm;
		field._id = i;
		field._type = HTF_FIELD_TYPE_NONE;
		field._crop._field = &field;
		field.Init(false);
	}
	SetDirty(true);
}

void FarmManager::View(HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	int res = _owner.CanAction(HTF_ACTION_VIEW, false);
	CHECK_RES(res);
	res = _farm.CanView();
	CHECK_RES(res);
	bool owner_changed = false;
	bool farm_changed = false;
	_owner.PreAction(HTF_ACTION_VIEW, owner_changed);
	_farm.View(farm_changed);
	if (owner_changed || farm_changed) SetDirty(true);
	Make<HOMETOWN::farm_info>::From(response, *this);
}

void FarmManager::ViewField(int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	Make<field_info>::From(response, *field);
}

void FarmManager::SowField(int sponsor, int seed_id, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	if (sponsor == _owner._id)
	{
		if (field->_type == HTF_FIELD_TYPE_LOVE)
		{
			response.set_retcode(HT_ERR_FARMLOVEFIELDBYSELF);
			return;
		}
	}
	else
	{
		if (field->_type == HTF_FIELD_TYPE_NORMAL)
		{
			response.set_retcode(HT_ERR_FARMNRMLFIELDBYOTHER);
			return;
		}
	}
	int sponsor_level;
	int64_t *pocket_money;
	if (sponsor == _owner._id)
	{
		sponsor_level = _owner._level;
		pocket_money = &_owner._money;
	}
	else		//好友来种我的爱心地了
	{
		FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
		if (sponsor_farm == NULL)
		{
			response.set_retcode(HT_ERR_PLAYEROFFLINE);
			return;
		}

		sponsor_level = sponsor_farm->_owner._level;
		const LEVEL_CONFIG &lvl_cfg = FarmCompoManager::GetInstance()->GetLevelConfig(sponsor_level);
		int plant_lovefield_num = FarmCompoManager::GetInstance()->GetPlantLoveFieldNum(sponsor);
		if (plant_lovefield_num >= lvl_cfg.plant_love_field)
		{
			response.set_retcode(HT_ERR_PLANTLOVEFIELDMAX);
			return;
		}

		pocket_money = &sponsor_farm->_owner._money;
	}
	const CROP_ESSENCE *ess = FarmCrop::GetEssence(seed_id);
	if (ess == NULL)
	{
		response.set_retcode(HT_ERR_SEEDNOTFOUND);
		return;
	}
	if (ess->required_level > sponsor_level)
	{
		response.set_retcode(HT_ERR_FARMFIELDREQUIRELEVEL);
		return;
	}
	if (ess->seed_price > *pocket_money)
	{
		response.set_retcode(HT_ERR_MONEYNOTENOUGH);
		return;
	}

	int res = _farm.CanSowField(field);
	CHECK_RES(res);
	bool changed = false;
	res = _farm.SowField(sponsor, seed_id, field, changed);
	CHECK_RES(res);
	*pocket_money -= ess->seed_price;

	//只有在好友爱心地播种才记录动态
	if (sponsor != _owner._id)
	{
		HOMETOWN::DYNAMIC::sow_field log = {sponsor, 20, {}, _owner._id, 20, {}, seed_id};
		HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
		if (sponsor_farm != NULL)
			sponsor_farm->LogActionDynamic(HOMETOWN::FARM_SOW_FIELD, log);
		LogFarmDynamic(HOMETOWN::FARM_SOW_FIELD, log);
	}

	SetDirty(true);
	response << ess->seed_price;
}

void FarmManager::PlowField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	if (sponsor == _owner._id)
	{
		if (field->_type == HTF_FIELD_TYPE_LOVE)
		{
			response.set_retcode(HT_ERR_FARMLOVEFIELDBYSELF);
			return;
		}
	}
	else
	{
		if (field->_type == HTF_FIELD_TYPE_NORMAL)
		{
			response.set_retcode(HT_ERR_FARMNRMLFIELDBYOTHER);
			return;
		}
		else if (field->_type == HTF_FIELD_TYPE_LOVE)
		{
			if (field->_has_crop && field->_friend != sponsor)
			{
				std::set<int> states;
				states.insert(HTF_STATE_WILT);
				states.insert(HTF_STATE_HARVEST);
				states.insert(HTF_STATE_ALLSTOLEN);
				if (!field->_crop.IsInAnyState(states))
				{
					response.set_retcode(HT_ERR_FARMLOVEFIELDUSED);
					return;
				}
			}
		}
	}

	int res = _farm.CanPlowField(field);
	CHECK_RES(res);
	bool changed = false;
	_farm.PlowField(field, changed);

	//犁好友的爱心地才记录动态
	if (sponsor != _owner._id)
	{
		HOMETOWN::DYNAMIC::plow_field log = {sponsor, 20, {}, _owner._id, 20, {}, emotion};
		HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		LogFarmDynamic(HOMETOWN::FARM_PLOW_FIELD, log);

		FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
		if (sponsor_farm != NULL)
			sponsor_farm->LogActionDynamic(HOMETOWN::FARM_PLOW_FIELD, log);
	}

	SetDirty(true);
}

void FarmManager::PestField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
	if (sponsor_farm == NULL)
	{
		response.set_retcode(HT_ERR_PLAYEROFFLINE);
		return;
	}
	int res = _farm.CanPestField(field);
	CHECK_RES(res);
	bool changed = false;
	_farm.PestField(field, changed);

	//给好友的爱心地除虫才记录动态
	if (sponsor != _owner._id)
	{
		HOMETOWN::DYNAMIC::pest_field log = {sponsor, 20, {}, _owner._id, 20, {}, emotion};
		HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		LogFarmDynamic(HOMETOWN::FARM_PEST_FIELD, log);
		sponsor_farm->LogActionDynamic(HOMETOWN::FARM_PEST_FIELD, log);
	}

	SetDirty(true);
	int charm_change = rand_between(1, 3);
	sponsor_farm->_owner._charm += charm_change;		//操作者魅力值随机加1-3点
	sponsor_farm->SetDirty(true);
	response << charm_change;
}

void FarmManager::WaterField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
	if (sponsor_farm == NULL)
	{
		response.set_retcode(HT_ERR_PLAYEROFFLINE);
		return;
	}
	int res = _farm.CanWaterField(field);
	CHECK_RES(res);
	bool changed = false;
	_farm.WaterField(field, changed);

	//给好友的爱心地浇水才记录动态
	if (sponsor != _owner._id)
	{
		HOMETOWN::DYNAMIC::water_field log = {sponsor, 20, {}, _owner._id, 20, {}, emotion};
		HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		LogFarmDynamic(HOMETOWN::FARM_WATER_FIELD, log);
		sponsor_farm->LogActionDynamic(HOMETOWN::FARM_WATER_FIELD, log);
	}

	SetDirty(true);
	int charm_change = rand_between(1, 3);
	sponsor_farm->_owner._charm += charm_change;		//操作者魅力值随机加1-3点
	sponsor_farm->SetDirty(true);
	response << charm_change;
}

void FarmManager::WeedField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
	if (sponsor_farm == NULL)
	{
		response.set_retcode(HT_ERR_PLAYEROFFLINE);
		return;
	}
	int res = _farm.CanWeedField(field);
	CHECK_RES(res);
	bool changed = false;
	_farm.WeedField(field, changed);

	//给好友的爱心地除草才记录动态
	if (sponsor != _owner._id)
	{
		HOMETOWN::DYNAMIC::weed_field log = {sponsor, 20, {}, _owner._id, 20, {}, emotion};
		HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		LogFarmDynamic(HOMETOWN::FARM_WEED_FIELD, log);
		sponsor_farm->LogActionDynamic(HOMETOWN::FARM_WEED_FIELD, log);
	}

	SetDirty(true);
	int charm_change = rand_between(1, 3);
	sponsor_farm->_owner._charm += charm_change;		//操作者魅力值随机加1-3点
	sponsor_farm->SetDirty(true);
	response << charm_change;
}

void FarmManager::HarvestField(int sponsor, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	if (sponsor != _owner._id)
	{
		if (field->_type == HTF_FIELD_TYPE_NORMAL)
		{
			response.set_retcode(HT_ERR_FARMNRMLFIELDBYOTHER);
			return;
		}
		if (field->_type==HTF_FIELD_TYPE_LOVE
				&& field->_has_crop
				&& field->_friend != sponsor)
		{
			response.set_retcode(HT_ERR_FARMLOVEFIELDUSED);
			return;
		}
	}

	FarmCrop &crop = field->_crop;
	if (crop._fruit_left <= 0) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOFRUIT);
		return;
	}

	int res = _farm.CanHarvestField(field);
	CHECK_RES(res);

	FarmManager *friend_farm = NULL;
	const CROP_ESSENCE *ess = FarmCrop::GetEssence(crop._id);
	if (ess == NULL)
	{
		response.set_retcode(HT_ERR_SEEDNOTFOUND);
		return;
	}
	const HometownItemBody *body = HometownItemBodyMan::GetInstance()->GetBody(ess->fruit_id);
	if (body == NULL)
	{
		response.set_retcode(HT_ERR_FRUITNOTFOUND);
		return;
	}
	const FRUIT_ESSENCE *fruit_ess;
	DATA_TYPE dummy;
	HometownTemplate::GetInstance()->get_data_ptr(ess->fruit_id, ID_SPACE_ESSENCE, dummy, fruit_ess);
	if (fruit_ess == NULL)
	{
		response.set_retcode(HT_ERR_FRUITNOTFOUND);
		return;
	}
	int charm_add_per_fruit = (int)(fruit_ess->price * 0.1f);
	if (charm_add_per_fruit < 1)
		charm_add_per_fruit = 1;

	HometownItemData idata;
	body->FillItemData(idata);

	//主人收获自己的普通地
	if (field->_type == HTF_FIELD_TYPE_NORMAL)
	{
		idata._count = crop._fruit_left;
		int res = _owner._pocket.AddItem(idata);
		CHECK_RES(res);

		int charm_add = charm_add_per_fruit * crop._fruit_left;
		_owner._charm += charm_add;
		response << charm_add;
		response << ess->fruit_id;
		response << idata._count;
		HOMETOWN::DYNAMIC::harvest_field log = {sponsor, 20, {}, _owner._id, 20, {}, ess->fruit_id, idata._count};
		HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		LogActionDynamic(HOMETOWN::FARM_HARVEST_FIELD, log);
	}
	else	//爱心地
	{
		//爱心地的果实，主人和播种者一人一半
		int owner_share = crop._fruit_left/2;
		int friend_share = crop._fruit_left-owner_share;

		if (sponsor == _owner._id)	//主人来收爱心地了
		{
			if (owner_share > 0)
			{
				idata._count = owner_share;
				int res = _owner._pocket.AddItem(idata);
				CHECK_RES(res);
				int charm_add = charm_add_per_fruit * owner_share;
				_owner._charm += charm_add;
				response << charm_add;
				response << ess->fruit_id;
				response << owner_share;

				HOMETOWN::DYNAMIC::harvest_field log = {sponsor, 20, {}, _owner._id, 20, {}, ess->fruit_id, owner_share};
				HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
				HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
				LogActionDynamic(HOMETOWN::FARM_HARVEST_FIELD, log);
			}
			if (friend_share > 0)
			{
				friend_farm = FarmCompoManager::GetInstance()->GetFarm(field->_friend);
				if (friend_farm != NULL)
				{
					idata._count = friend_share;
					friend_farm->_owner._pocket.AddItemAsMuch(idata);
					friend_farm->_owner._charm += charm_add_per_fruit * (friend_share-idata._count);
					HOMETOWN::DYNAMIC::harvest_lovefield log = {sponsor, 20, {}, _owner._id, 20, {}, ess->fruit_id, friend_share-idata._count, idata._count};
					HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
					HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
					friend_farm->LogFarmDynamic(HOMETOWN::FARM_HARVEST_FIELD, log);
				}
			}
		}
		else						//耕种的人来收爱心地了
		{
			if (friend_share > 0)
			{
				friend_farm = FarmCompoManager::GetInstance()->GetFarm(field->_friend);
				if (friend_farm != NULL)
				{
					idata._count = friend_share;
					int res = friend_farm->_owner._pocket.AddItem(idata);
					CHECK_RES(res);
					int charm_add = charm_add_per_fruit * friend_share;
					friend_farm->_owner._charm += charm_add;
					response << charm_add;
					response << ess->fruit_id;
					response << friend_share;

					HOMETOWN::DYNAMIC::harvest_field log = {sponsor, 20, {}, _owner._id, 20, {}, ess->fruit_id, friend_share};
					HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
					HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
					friend_farm->LogActionDynamic(HOMETOWN::FARM_HARVEST_FIELD, log);
				}
			}
			if (owner_share > 0)
			{
				idata._count = owner_share;
				_owner._pocket.AddItemAsMuch(idata);
				_owner._charm += charm_add_per_fruit * (owner_share-idata._count);
				HOMETOWN::DYNAMIC::harvest_lovefield log = {sponsor, 20, {}, _owner._id, 20, {}, ess->fruit_id, owner_share-idata._count, idata._count};
				HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
				HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
				LogFarmDynamic(HOMETOWN::FARM_HARVEST_FIELD, log);
			}
		}
	}
	bool changed = false;
	_farm.HarvestField(field, changed);
	crop._fruit_left = 0;

	SetDirty(true);
	if (friend_farm!=NULL) friend_farm->SetDirty(true);
}

void FarmManager::StealField(int sponsor, int emotion, int field_id, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmField *field = _farm.GetField(field_id);
	if (field == NULL) 
	{
		response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
		return;
	}
	if (field->_type == HTF_FIELD_TYPE_LOVE)
	{
		response.set_retcode(HT_ERR_FARMFIELDSTEALLOVE);
		return;
	}
	if (!field->_has_crop)
	{
		response.set_retcode(HT_ERR_HASNOCROP);
		return;
	}
	if (field->IsSponsorInCoolDown(sponsor, HTF_SPONSOR_COOLDOWN_STEAL))
	{
		response.set_retcode(HT_ERR_COOLDOWN);
		return;
	}
	int res = _farm.CanStealField(field);
	CHECK_RES(res);
	FarmCrop &crop = field->_crop;
	if (crop._fruit_left <= 0)
	{
		response.set_retcode(HT_ERR_FARMFIELDNOFRUIT);
		return;
	}
	//要在调用Farm::StealField之前把果实数去掉
	FarmManager *sponsor_farm = FarmCompoManager::GetInstance()->GetFarm(sponsor);
	if (sponsor_farm == NULL)
	{
		response.set_retcode(HT_ERR_PLAYEROFFLINE);
		return;
	}
	FarmPlayer &thief = sponsor_farm->_owner;
	const CROP_ESSENCE *ess = FarmCrop::GetEssence(crop._id);
	if (ess == NULL)
	{
		response.set_retcode(HT_ERR_SEEDNOTFOUND);
		return;
	}
	const HometownItemBody *body = HometownItemBodyMan::GetInstance()->GetBody(ess->fruit_id);
	if (body == NULL)
	{
		response.set_retcode(HT_ERR_FRUITNOTFOUND);
		return;
	}
	//剩余百分比<=1-最大可偷百分比时不允许再偷了
	if (crop._fruit_left <= (100-ess->steal_limit)/100.0f * crop._fruit_total)
	{
		response.set_retcode(HT_ERR_HAVE_STOLEN_TOOMUCH);
		return;
	}
	const FRUIT_ESSENCE *fruit_ess;
	DATA_TYPE dummy;
	HometownTemplate::GetInstance()->get_data_ptr(ess->fruit_id, ID_SPACE_ESSENCE, dummy, fruit_ess);
	if (fruit_ess == NULL)
	{
		response.set_retcode(HT_ERR_FRUITNOTFOUND);
		return;
	}
	//检查一下小偷本身的状态，比如隐身是否过期
	bool changed = false;
	thief.PreAction(HTF_ACTION_NONE, changed);

	const FARM_CONFIG &farm_config = FarmCompoManager::GetInstance()->GetConfig();
	float steal_ratio = (ess->steal_limit)/100.0f*rand_between(thief._level, farm_config.max_level)/(float)farm_config.max_level;
	int steal_num = (int)(steal_ratio*crop._fruit_left);
	if (steal_num > crop._fruit_left) steal_num = crop._fruit_left;

	HometownItemData idata;
	body->FillItemData(idata);
	idata._count = steal_num;
	res = thief._pocket.AddItem(idata);
	CHECK_RES(res);

	
	crop._fruit_left -= steal_num;

	_farm.StealField(field, changed);
	field->SetSponsorCoolDown(sponsor, HTF_SPONSOR_COOLDOWN_STEAL, HTF_TIME_COOLDOWN_STEAL);
	//计算偷窃者和被偷人魅力值变化
	//被偷者魅力值始终加
	_owner._charm += fruit_ess->price;
	//偷窃者按照概率加或减
	int thief_charm_change = 0;
	if (rand_between(1, 100) <= 70+thief._level/2)
	{
		if (rand_between(1, 100) <= 10)
			thief_charm_change = 2*fruit_ess->price;
		else
			thief_charm_change = fruit_ess->price;
	}
	else
	{
		if (rand_between(1, 100) <= 10)
			thief_charm_change = -2*fruit_ess->price*4/5;
		else
			thief_charm_change = -fruit_ess->price*4/5;
	}
	thief._charm += thief_charm_change;
	if (thief._charm < 0) thief._charm = 0;

	HOMETOWN::DYNAMIC::steal_field log = {sponsor, 20, {}, _owner._id, 20, {}, emotion, ess->fruit_id, steal_num};
	HometownManager::GetInstance()->GetPlayerName(sponsor, log.sponsor_name, log.sponsor_name_size);
	HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
	sponsor_farm->LogActionDynamic(HOMETOWN::FARM_STEAL_FIELD, log);
	//小偷如果没有隐身，要在主人农场里记录动态
	if (!thief.IsInState(HTF_STATE_HIDE))
		LogFarmDynamic(HOMETOWN::FARM_STEAL_FIELD, log);

	SetDirty(true);
	sponsor_farm->SetDirty(true);
	response << thief_charm_change;
	response << ess->fruit_id;
	response << steal_num;
}

void FarmManager::ListPocket(HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	Make<HOMETOWN::pocket_info>::From(response, _owner._pocket);
}

void FarmManager::ExchgMoney(char type, int amount, HOMETOWN::WRAPPER &response)
{
	HometownPlayerRef ht_player;
	if (!HometownManager::GetInstance()->FindPlayerOnline(_owner._id, ht_player))
	{
		response.set_retcode(HT_ERR_PLAYEROFFLINE);
		return;
	}
	const FARM_CONFIG &config = FarmCompoManager::GetInstance()->GetConfig();
	if (amount <= 0)
	{
		response.set_retcode(HT_ERR_MONEYEXCHGAMOUNT);
		return;
	}

	int64_t exchg_amount_out = 0, exchg_amount_in = 0;
	if (type==0)		//通用币换成组件币，兑换数目必须小于等于100,而且要冷却1天
	{
		if (_owner.IsSelfInCoolDown(HTF_SELF_COOLDOWN_EXCHGMONEY))
		{
			response.set_retcode(HT_ERR_COOLDOWN);
			return;
		}
		if (amount > 100) amount = 100;
		if (amount < config.money_universal)
		{
			response.set_retcode(HT_ERR_MONEYEXCHGAMOUNT);
			return;
		}
		exchg_amount_out = amount/config.money_universal*config.money_universal;
		if (ht_player->GetMoney()<exchg_amount_out)
		{
			response.set_retcode(HT_ERR_MONEYNOTENOUGH);
			return;
		}
		ht_player->AddMoney(-exchg_amount_out);
		exchg_amount_in = exchg_amount_out/config.money_universal*config.money_local; 
		_owner._money += exchg_amount_in;
		_owner.SetSelfCoolDown(HTF_SELF_COOLDOWN_EXCHGMONEY, HTF_TIME_COOLDOWN_EXCHGMONEY);
	}
	else //pkt->type==1
	{
		if (amount < config.money_local)
		{
			response.set_retcode(HT_ERR_MONEYEXCHGAMOUNT);
			return;
		}
		exchg_amount_out = amount/config.money_local*config.money_local;
		if (_owner._money<exchg_amount_out)
		{
			response.set_retcode(HT_ERR_MONEYNOTENOUGH);
			return;
		}
		_owner._money -= exchg_amount_out;
		exchg_amount_in = exchg_amount_out/config.money_local*config.money_universal;
		ht_player->AddMoney(exchg_amount_in);
	}
	response << ht_player->GetMoney();
	response << _owner._money;

	HOMETOWN::DYNAMIC::exchg_money log = {type, exchg_amount_in, exchg_amount_out};
	LogActionDynamic(HOMETOWN::EXCHG_MONEY, log);
	SetDirty(true);
}

void FarmManager::LevelUp(HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	FarmCompoManager *fcm = FarmCompoManager::GetInstance();
	const FARM_CONFIG &config = fcm->GetConfig();
	if (_owner._level >= config.max_level)
	{
		response.set_retcode(HT_ERR_MAXLEVEL);
		return;
	}
	int64_t charm_need = fcm->GetUpgradeCharm(_owner._level);
	if (_owner._charm < charm_need)
	{
		response.set_retcode(HT_ERR_CHARMNOTENOUGH);
		return;
	}
	_owner._charm -= charm_need;
	const LEVEL_CONFIG &oldcfg = fcm->GetLevelConfig(_owner._level);
	++_owner._level;
	const LEVEL_CONFIG &newcfg = fcm->GetLevelConfig(_owner._level);
	if (oldcfg.level != newcfg.level)
	{
		int add_nrml_field = newcfg.normal_field_count-oldcfg.normal_field_count;
		int add_love_field = newcfg.love_field_count-oldcfg.love_field_count;
		if (add_nrml_field>0 || add_love_field>0)
		{
			Farm::FieldMap::iterator it, ie = _farm._fields.end();
			for (it = _farm._fields.begin(); it!=ie && add_nrml_field>0; ++it)
			{
				if (it->second._type == HTF_FIELD_TYPE_NONE)
				{
					it->second._type = HTF_FIELD_TYPE_NORMAL;
					bool changed = false;
					it->second.Cult(changed);
					assert(changed);
					--add_nrml_field;
				}
			}
			for (; it!=ie && add_love_field>0; ++it)
			{
				if (it->second._type == HTF_FIELD_TYPE_NONE)
				{
					it->second._type = HTF_FIELD_TYPE_LOVE;
					bool changed = false;
					it->second.Cult(changed);
					assert(changed);
					--add_love_field;
				}
			}
		}
	}

	HOMETOWN::DYNAMIC::level_up log = {_owner._level};
	LogActionDynamic(HOMETOWN::LEVEL_UP, log);
	SetDirty(true);
	Make<HOMETOWN::farm_info>::From(response, *this);
}

void FarmManager::BuyItem(int item_id, int count, HOMETOWN::WRAPPER &response)
{
	using HOMETOWN::S2C::Make;
	assert(count>0);
	const TOOL_ESSENCE *ess;
	DATA_TYPE dummy;
	HometownTemplate::GetInstance()->get_data_ptr(item_id, ID_SPACE_ESSENCE, dummy, ess);
	if (ess == NULL)
	{
		response.set_retcode(HT_ERR_ITEMNOTFOUND);
		return;
	}
	if (_owner._level < ess->required_level)
	{
		response.set_retcode(HT_ERR_FARMFIELDREQUIRELEVEL);
		return;
	}
	int64_t charm_cost = ess->charm_price*count;
	int64_t money_cost = ess->money_price*count;
	if (_owner._charm < charm_cost)
	{
		response.set_retcode(HT_ERR_CHARMNOTENOUGH);
		return;
	}
	if (_owner._money < money_cost)
	{
		response.set_retcode(HT_ERR_MONEYNOTENOUGH);
		return;
	}
	const HometownItemBody *body = HometownItemBodyMan::GetInstance()->GetBody(item_id);
	if (body == NULL)
	{
		response.set_retcode(HT_ERR_ITEMNOTFOUND);
		return;
	}
	HometownItemData idata;
	body->FillItemData(idata);
	idata._count = count;
	int res = _owner._pocket.AddItem(idata);
	CHECK_RES(res);
	_owner._charm -= charm_cost;
	_owner._money -= money_cost;

	HOMETOWN::DYNAMIC::buy_item log = {item_id, count, charm_cost, money_cost};
	LogActionDynamic(HOMETOWN::BUY_ITEM, log);

	SetDirty(true);
	response << charm_cost;
	response << money_cost;
}

void FarmManager::UseItem(int pos, HOMETOWN::WRAPPER &response, const HOMETOWN::C2S::CMD::use_item *pkt)
{
	int item_id;
	int res = _owner._pocket.GetItemID(pos, item_id);
	CHECK_RES(res);
	if (item_id != pkt->item_id)
	{
		response.set_retcode(HT_ERR_POSITEMMISMATCH);
		return;
	}
	char consumed = 0;
	response.set_retcode(_owner._pocket.UseItem(&_owner, pos, consumed));
	
	if (response.get_retcode() == HT_ERR_SUCCESS)
	{
		response << consumed;

		HOMETOWN::DYNAMIC::use_item log = {_owner._id, 20, {}, _owner._id, 20, {}, item_id, pkt->target_type};
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.target_name, log.target_name_size);
		LogActionDynamic(HOMETOWN::USE_ITEM, log);
	}
}

void FarmManager::UseItem(int pos, HometownTargetObj *target, HOMETOWN::WRAPPER &response, const HOMETOWN::C2S::CMD::use_item *pkt)
{
	int item_id;
	int res = _owner._pocket.GetItemID(pos, item_id);
	CHECK_RES(res);
	if (item_id != pkt->item_id)
	{
		response.set_retcode(HT_ERR_POSITEMMISMATCH);
		return;
	}
	char consumed = 0;
	response.set_retcode(_owner._pocket.UseItem(&_owner, pos, target, consumed));

	if (response.get_retcode() == HT_ERR_SUCCESS)
	{
		response << consumed;

		HOMETOWN::DYNAMIC::use_item log = {_owner._id, 20, {}, pkt->roleid, 20, {}, item_id, pkt->target_type};
		HometownManager::GetInstance()->GetPlayerName(_owner._id, log.sponsor_name, log.sponsor_name_size);
		HometownManager::GetInstance()->GetPlayerName(pkt->roleid, log.target_name, log.target_name_size);
		LogActionDynamic(HOMETOWN::USE_ITEM, log);
		
		//对别人使用了道具才记录在别人的菜园动态中
		if (_owner._id != pkt->roleid)
		{
			FarmManager *target_farm = FarmCompoManager::GetInstance()->GetFarm(pkt->roleid);
			if (target_farm != NULL)
				target_farm->LogFarmDynamic(HOMETOWN::USE_ITEM, log);
		}
	}
}

void FarmManager::SellItem(int pos, int count, HOMETOWN::WRAPPER &response, const HOMETOWN::C2S::CMD::sell_item *pkt)
{
	assert(count > 0);
	int item_id;
	int res = _owner._pocket.GetItemID(pos, item_id);
	CHECK_RES(res);
	if (item_id != pkt->item_id)
	{
		response.set_retcode(HT_ERR_POSITEMMISMATCH);
		return;
	}

	const FRUIT_ESSENCE *ess;
	DATA_TYPE dummy;
	HometownTemplate::GetInstance()->get_data_ptr(item_id, ID_SPACE_ESSENCE, dummy, ess);
	if (ess == NULL)
	{
		response.set_retcode(HT_ERR_CANNOTSELL);
		return;
	}
	res = _owner._pocket.SubItem(pos, count);
	CHECK_RES(res);
	int64_t earning = ess->price*count;
	_owner._money += earning;
	response << earning;

	HOMETOWN::DYNAMIC::sell_item log = {item_id, count, earning};
	LogActionDynamic(HOMETOWN::SELL_ITEM, log);
	SetDirty(true);
}

void FarmManager::ToGFarmCompoInfo(GFarmCompoInfo &info)
{
	//Save Dynamic
	Marshal::OctetsStream os_farm_dym;
	_farm_dynamic.Save(os_farm_dym);
	info.farm_dynamic.swap(os_farm_dym);

	Marshal::OctetsStream os_action_dym;
	_action_dynamic.Save(os_action_dym);
	info.action_dynamic.swap(os_action_dym);

	//Save player
	GFarmPlayer &gowner = info.owner;
	FarmPlayer &owner = _owner;
	gowner.charm = owner._charm;
	gowner.money = owner._money;
	gowner.level = owner._level;

	Marshal::OctetsStream os_owner_states;
	owner.SaveStates(os_owner_states);
	gowner.states.swap(os_owner_states);

	Marshal::OctetsStream os_items;
	os_items << owner._pocket;
	gowner.items.swap(os_items);

	Marshal::OctetsStream os_owner_cooldown;
	owner.CleanupCoolDown();
	owner.SaveCoolDown(os_owner_cooldown);
	gowner.cooldown.swap(os_owner_cooldown);

	//Save farm
	GFarm &gfarm = info.farm;
	Farm &farm = _farm;
	gfarm.protect_period.insert(gfarm.protect_period.end(), &farm._protect_period[0], sizeof(farm._protect_period));

	Marshal::OctetsStream os_farm_states;
	farm.SaveStates(os_farm_states);
	gfarm.states.swap(os_farm_states);

	Marshal::OctetsStream os_farm_cooldown;
	farm.CleanupCoolDown();
	farm.SaveCoolDown(os_farm_cooldown);
	gfarm.cooldown.swap(os_farm_cooldown);

	Farm::FieldMap &fields = farm._fields;
	Farm::FieldMap::iterator it = fields.begin(), ie = fields.end();
	for (; it != ie; ++it)
	{
		gfarm.fields.push_back(GFarmField());
		GFarmField &gfield = gfarm.fields.back();
		FarmField &field = it->second;
		gfield.id = field._id;
		gfield.type = field._type;
		gfield.friend_id = field._friend;
		gfield.friend_name = field._friend_name;
		gfield.has_crop = field._has_crop;

		Marshal::OctetsStream os_field_states;
		field.SaveStates(os_field_states);
		gfield.states.swap(os_field_states);

		Marshal::OctetsStream os_field_cooldown;
		field.CleanupCoolDown();
		field.SaveCoolDown(os_field_cooldown);
		gfield.cooldown.swap(os_field_cooldown);

		GFarmCrop &gcrop = gfield.crop;
		FarmCrop &crop = field._crop;
		gcrop.id = crop._id;
		gcrop.seed_time = crop._seed_time;
		gcrop.grow_time = crop._grow_time;
		gcrop.grow_period = crop._grow_period;
		gcrop.grow_stop_end = crop._grow_stop_end;
		gcrop.state_grow_time = crop._state_grow_time;
		gcrop.protect_period.insert(gcrop.protect_period.end(), &crop._protect_period[0], sizeof(crop._protect_period));
		gcrop.fruit_left = crop._fruit_left;
		gcrop.fruit_total = crop._fruit_total;

		Marshal::OctetsStream os_crop_states;
		crop.SaveStates(os_crop_states);
		gcrop.states.swap(os_crop_states);

		Marshal::OctetsStream os_crop_cooldown;
		crop.CleanupCoolDown();
		crop.SaveCoolDown(os_crop_cooldown);
		gcrop.cooldown.swap(os_crop_cooldown);
	}
}

void FarmManager::FromGFarmCompoInfo(int roleid, const GFarmCompoInfo &info)
{
	//Load Dynamic
	_farm_dynamic.Load(Marshal::OctetsStream(info.farm_dynamic));
	_action_dynamic.Load(Marshal::OctetsStream(info.action_dynamic));

	//Load player
	const GFarmPlayer &gowner = info.owner;
	FarmPlayer &owner = _owner;
	_owner._fm = this;
	owner._id = roleid;
	owner._charm = gowner.charm;
	owner._money = gowner.money;
	owner._level = gowner.level;
	owner.LoadStates(Marshal::OctetsStream(gowner.states));
	Marshal::OctetsStream(gowner.items) >> owner._pocket;
	owner.LoadCoolDown(Marshal::OctetsStream(gowner.cooldown));
	owner.CleanupCoolDown();

	//Load farm
	Farm &farm = _farm;
	farm._fm = this;
	Farm::FieldMap &fields = _farm._fields;
	const GFarm &gfarm = info.farm;
	if (gfarm.protect_period.size() == sizeof(farm._protect_period))
		memcpy(farm._protect_period, gfarm.protect_period.begin(), sizeof(farm._protect_period));
	else
		memset(farm._protect_period, 0, sizeof(farm._protect_period));
	farm.LoadStates(Marshal::OctetsStream(gfarm.states));
	farm.LoadCoolDown(Marshal::OctetsStream(gfarm.cooldown));
	farm.CleanupCoolDown();

	std::vector<GFarmField>::const_iterator it, ie = gfarm.fields.end();
	for (it = gfarm.fields.begin(); it != ie; ++it)
	{
		FarmField &field = fields[it->id];
		field._farm = &farm;
		field._id = it->id;
		field._type = it->type;
		field._friend = it->friend_id;
		field._friend_name = it->friend_name;
		field._has_crop = it->has_crop;
		field.LoadStates(Marshal::OctetsStream(it->states));
		field.LoadCoolDown(Marshal::OctetsStream(it->cooldown));
		field.CleanupCoolDown();

		FarmCrop &crop = field._crop;
		const GFarmCrop &gcrop = it->crop;
		crop._field = &field;
		crop._id = gcrop.id;
		crop._seed_time = gcrop.seed_time;
		crop._grow_time = gcrop.grow_time;
		crop._grow_period = gcrop.grow_period;
		crop._grow_stop_end = gcrop.grow_stop_end;
		crop._state_grow_time = gcrop.state_grow_time;
		if (gcrop.protect_period.size() == sizeof(crop._protect_period))
			memcpy(crop._protect_period, gcrop.protect_period.begin(), sizeof(crop._protect_period));
		else
			memset(crop._protect_period, 0, sizeof(crop._protect_period));
		crop._fruit_left = gcrop.fruit_left;
		crop._fruit_total = gcrop.fruit_total;
		crop.LoadStates(Marshal::OctetsStream(gcrop.states));
		crop.LoadCoolDown(Marshal::OctetsStream(gcrop.cooldown));
		crop.CleanupCoolDown();
	}
}

int FarmCompoManager::GetPlantLoveFieldNum(int roleid) const
{
	int num = 0;
	HometownPlayerRef ht_player;
	if (HometownManager::GetInstance()->FindPlayerOnline(roleid, ht_player))
	{
		std::set<int>::iterator it, ie = ht_player->_friend_list.end();
		for (it=ht_player->_friend_list.begin(); it!=ie; ++it)
		{
			const FarmManager *fm = GetFarm(*it);
			if (fm != NULL)
				num += fm->_farm.GetPlantLoveFieldNum(roleid);
		}
	}

	return num;
}

void FarmCompoManager::OnSyncToDB(int retcode, int roleid)
{
	if (retcode != ERR_SUCCESS)
	{
		FarmManager *fm = GetFarm(roleid);
		if (fm != NULL) fm->SetDirty(true);
		LOG_TRACE("FarmCompoManager: fail to sync to DB for roleid=%d\n", roleid);
	}
}

void FarmCompoManager::SyncToDB(int roleid)
{
	FarmManager *fm = GetFarm(roleid);
	if (fm == NULL) return;

	try
	{
		GFarmCompoInfo info;
		fm->ToGFarmCompoInfo(info);

		DBHometownPutCompoInfoArg arg;
		arg.roleid = roleid;
		arg.compo_id = GetCompoID();
		arg.info.swap(Marshal::OctetsStream()<<info);
		DBHometownPutCompoInfo *rpc = (DBHometownPutCompoInfo *)Rpc::Call(RPC_DBHOMETOWNPUTCOMPOINFO, arg);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		fm->SetDirty(false);
		LOG_TRACE("FarmCompoManager: sync into to DB for roleid=%d\n", roleid);
	}
	catch (...)
	{
		Log::log(LOG_ERR, "FarmCompoManager: fail to sync info to DB for roleid=%d\n", roleid);
	}
}

void FarmCompoManager::OnLoad(int roleid, const Octets &data)
{
	if (_farm_map.find(roleid) != _farm_map.end())
	{
		Log::log(LOG_ERR, "FarmCompoManager:: load an already loaded role, id=%d\n", roleid);
		return;
	}
	
	try
	{
		GFarmCompoInfo info;
		Marshal::OctetsStream(data) >> info;
		FarmManager &fm = _farm_map[roleid];
		fm.FromGFarmCompoInfo(roleid, info);
		LOG_TRACE("FarmCompoManager::load data for roleid=%d\n", roleid);
	}
	catch (...)
	{
		Log::log(LOG_ERR, "Load invalid farm compo info for roleid=%d\n", roleid);
	}
}

bool FarmCompoManager::Init()
{
	_config = HometownTemplate::GetInstance()->get_farm_config();
	if (_config==NULL) return false;
	return BaseCompoManager::Init();
}

#define CHECK_SIZE(expr) \
do\
{\
	if (!(expr)) \
	{\
		response.set_retcode(HT_ERR_PACKETSIZE);\
		return;\
	}\
}\
while(0)

void FarmCompoManager::HandleCmd(int roleid, int cmd_type, void *data, int size, HOMETOWN::WRAPPER &response) 
{
	using namespace HOMETOWN::C2S::CMD;
	switch (cmd_type)
	{
	case HOMETOWN::COMPO_ADD:
		{
			CHECK_SIZE(size==0);
			if (GetFarm(roleid) != NULL)
			{
				response.set_retcode(HT_ERR_ALREADYADDCOMPO);
				return;
			}
			_farm_map[roleid].InitFarm(roleid, response);
		}
		break;
	case HOMETOWN::EXCHG_MONEY:
		{
			CHECK_SIZE(size==sizeof(struct exchg_money));
			response.push_back(data, size);
			HometownPlayerRef ht_player;
			struct exchg_money *pkt = (struct exchg_money *)data;
			FarmManager *pfm = GetFarm(roleid);
			if (pfm == NULL)
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->ExchgMoney(pkt->type, pkt->money, response);
		}
		break;
	case HOMETOWN::LEVEL_UP:
		{
			CHECK_SIZE(size==0);
			FarmManager *pfm = GetFarm(roleid);
			if (pfm == NULL)
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->LevelUp(response);
		}
		break;
	case HOMETOWN::BUY_ITEM:
		{
			CHECK_SIZE(size==sizeof(struct buy_item));
			response.push_back(data, size);
			FarmManager *pfm = GetFarm(roleid);
			if (pfm == NULL)
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			struct buy_item *pkt = (struct buy_item *)data;
			if (!IsTool(pkt->item_id))
			{
				response.set_retcode(HT_ERR_NOTFORSALE);
				return;
			}
			if (pkt->count <= 0)
			{
				response.set_retcode(HT_ERR_INVALIDNUM);
				return;
			}
			pfm->BuyItem(pkt->item_id, pkt->count, response);
		}
		break;
	case HOMETOWN::USE_ITEM:
		{
			CHECK_SIZE(size==sizeof(struct use_item));
			response.push_back(data, size);
			FarmManager *pfm = GetFarm(roleid);
			if (pfm == NULL)
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			struct use_item *pkt = (struct use_item *)data;
			switch (pkt->target_type)
			{
			case HOMETOWN::FARM_TARGET_SELF:
				pfm->UseItem(pkt->pos, response, pkt);
				break;
			case HOMETOWN::FARM_TARGET_PLAYER:
				{
					if (pkt->roleid == roleid)
					{
						response.set_retcode(HT_ERR_CANNOTSELFTARGET);
						return;
					}
					if (!HometownManager::GetInstance()->IsFriend(roleid, pkt->roleid))
					{
						response.set_retcode(HT_ERR_NOTFRIEND);
						return;
					}
					FarmManager *other = GetFarm(pkt->roleid);
					if (other == NULL)
					{
						response.set_retcode(HT_ERR_NOTADDCOMPO);
						return;
					}
					pfm->UseItem(pkt->pos, &other->_owner, response, pkt);
				}
				break;
			case HOMETOWN::FARM_TARGET_FARM:
				{
					if (pkt->roleid!=roleid &&
							!HometownManager::GetInstance()->IsFriend(roleid, pkt->roleid))
					{
						response.set_retcode(HT_ERR_NOTFRIEND);
						return;
					}
					FarmManager *other = GetFarm(pkt->roleid);
					if (other == NULL)
					{
						response.set_retcode(HT_ERR_NOTADDCOMPO);
						return;
					}
					pfm->UseItem(pkt->pos, &other->_farm, response, pkt);
				}
				break;
			case HOMETOWN::FARM_TARGET_FIELD:
				{
					if (pkt->roleid!=roleid &&
							!HometownManager::GetInstance()->IsFriend(roleid, pkt->roleid))
					{
						response.set_retcode(HT_ERR_NOTFRIEND);
						return;
					}
					FarmManager *other = GetFarm(pkt->roleid);
					if (other == NULL)
					{
						response.set_retcode(HT_ERR_NOTADDCOMPO);
						return;
					}
					FarmField *field = other->_farm.GetField(pkt->field_id);
					if (field == NULL)
					{
						response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
						return;
					}
					pfm->UseItem(pkt->pos, field, response, pkt);
				}
				break;
			case HOMETOWN::FARM_TARGET_CROP:
				{
					if (pkt->roleid!=roleid &&
							!HometownManager::GetInstance()->IsFriend(roleid, pkt->roleid))
					{
						response.set_retcode(HT_ERR_NOTFRIEND);
						return;
					}
					FarmManager *other = GetFarm(pkt->roleid);
					if (other == NULL)
					{
						response.set_retcode(HT_ERR_NOTADDCOMPO);
						return;
					}
					FarmField *field = other->_farm.GetField(pkt->field_id);
					if (field == NULL)
					{
						response.set_retcode(HT_ERR_FARMFIELDNOTFOUND);
						return;
					}
					if (!field->_has_crop)
					{
						response.set_retcode(HT_ERR_HASNOCROP);
						return;
					}
					pfm->UseItem(pkt->pos, field->GetCrop(), response, pkt);
				}
				break;
			default:
				response.set_retcode(HT_ERR_INVALIDTARGETTYPE);
				return;
			}
		}
		break;
	case HOMETOWN::SELL_ITEM:
		{
			CHECK_SIZE(size==sizeof(struct sell_item));
			response.push_back(data, size);
			struct sell_item *pkt = (struct sell_item *)data;
			if (pkt->count <= 0)
			{
				response.set_retcode(HT_ERR_INVALIDNUM);
				return;
			}
			FarmManager *pfm = GetFarm(roleid);
			if (pfm == NULL)
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->SellItem(pkt->pos, pkt->count, response, pkt);
		}
		break;
	case HOMETOWN::PACKAGE_LIST:
		{
			CHECK_SIZE(size == 0);
			FarmManager *pfm = GetFarm(roleid);
			if (pfm == NULL)
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->ListPocket(response);
		}
		break;
	case HOMETOWN::FARM_VIEW:
		{
			CHECK_SIZE(size==sizeof(struct farm_view));	
			response.push_back(data, size);
			struct farm_view *pkt = (struct farm_view *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->View(response);
		}
		break;
	case HOMETOWN::FARM_VIEW_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_view_field));	
			response.push_back(data, size);
			struct farm_view_field *pkt = (struct farm_view_field *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->ViewField(pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_PLOW_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_plow_field));
			response.push_back(data, size);
			struct farm_plow_field *pkt = (struct farm_plow_field *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->PlowField(roleid, pkt->emotion, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_SOW_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_sow_field));
			response.push_back(data, size);
			struct farm_sow_field *pkt = (struct farm_sow_field *)data;
			if (!IsCrop(pkt->seed_id))
			{
				response.set_retcode(HT_ERR_SEEDNOTFOUND);
				return;
			}
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->SowField(roleid, pkt->seed_id, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_WATER_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_water_field));
			response.push_back(data, size);
			struct farm_water_field *pkt = (struct farm_water_field *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->WaterField(roleid, pkt->emotion, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_PEST_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_pest_field));
			response.push_back(data, size);
			struct farm_pest_field *pkt = (struct farm_pest_field *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->PestField(roleid, pkt->emotion, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_WEED_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_weed_field));
			response.push_back(data, size);
			struct farm_weed_field *pkt = (struct farm_weed_field *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->WeedField(roleid, pkt->emotion, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_HARVEST_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_harvest_field));
			response.push_back(data, size);
			struct farm_harvest_field *pkt = (struct farm_harvest_field *)data;
			if (pkt->roleid!=roleid &&
				!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->HarvestField(roleid, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_STEAL_FIELD:
		{
			CHECK_SIZE(size==sizeof(struct farm_steal_field));
			response.push_back(data, size);
			struct farm_steal_field *pkt = (struct farm_steal_field *)data;
			if (roleid == pkt->roleid)
			{
				response.set_retcode(HT_ERR_FARMFIELDSTEALSELF);
				return;
			}
			else if (!HometownManager::GetInstance()->IsFriend(pkt->roleid, roleid))
			{
				response.set_retcode(HT_ERR_NOTFRIEND);
				return;
			}
			FarmManager *pfm = GetFarm(pkt->roleid);
			if (pfm == NULL) 
			{
				response.set_retcode(HT_ERR_NOTADDCOMPO);
				return;
			}
			pfm->StealField(roleid, pkt->emotion, pkt->field_id, response);
		}
		break;
	case HOMETOWN::FARM_LIST_LOVE_FIELD:
	case HOMETOWN::FARM_LIST_MATURE_FIELD:
		{
			CHECK_SIZE(size == 0);
			HometownManager *htm = HometownManager::GetInstance();
			HometownPlayerRef ht_player;
			if (!htm->FindPlayerOnline(roleid, ht_player))
			{
				response.set_retcode(HT_ERR_PLAYEROFFLINE);
				return;
			}
			std::set<int> friend_list;
			std::set<int>::iterator it, ie = ht_player->_friend_list.end();
			for (it=ht_player->_friend_list.begin(); it!=ie; ++it)
			{
				if (htm->IsFriend(roleid, *it)) 
				{
					FarmManager *fm = GetFarm(*it);
					if (fm != NULL)
					{
						if (cmd_type == FARM_LIST_LOVE_FIELD)
						{
							if (fm->_farm.HasPlantLoveField(roleid))
								friend_list.insert(*it);
						}
						else
						{
							if (fm->_farm.HasMatureField())
								friend_list.insert(*it);
						}
					}
				}
			}

			response << friend_list;
		}
		break;
	default:
		response.set_retcode(HT_ERR_INVALIDCMDTYPE);
		break;
	}
}

void WiltState::OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop)
{
	ParamState<FarmCrop>::OnEnter(prev, action, action_param, crop);
	crop->MarkClearOtherStates(this);
}

void HarvestState::OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop)
{
	ParamState<FarmCrop>::OnEnter(prev, action, action_param, crop);
	crop->MarkClearOtherStates(this);
}

void AllStolenState::OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop)
{
	ParamState<FarmCrop>::OnEnter(prev, action, action_param, crop);
	crop->MarkClearOtherStates(this);
}

void MatureState::OnEnter(State *prev, int action, const void *action_param, FarmCrop *crop)
{
	ParamState<FarmCrop>::OnEnter(prev, action, action_param, crop);
	crop->CalcFruit(_start_time);		//计算产量
	//种子自身的保护期生效
	const CROP_ESSENCE *ess = FarmCrop::GetEssence(crop->_id);
	if (ess!=NULL && ess->protect_period>0)
	{
		if (crop->IsInState(HTF_STATE_CROPPROTECTBEGIN+0))
		{
			crop->AddStateExpireTime(HTF_STATE_CROPPROTECTBEGIN+0, ess->protect_period);
		}
		else
		{
			CropProtectState *s = new CropProtectState(0, _start_time, ess->protect_period);
			crop->MarkAddState(s);
		}
	}
}

void CropProtectState::OnLeave(State *prev, int action, const void *action_param, FarmCrop *crop)
{
	if (_expire_time > _last_heartbeat_time)
	{
		crop->_protect_period[(size_t)_buffid] -= _expire_time-_last_heartbeat_time;
		if (crop->_protect_period[(size_t)_buffid] < 0)
			crop->_protect_period[(size_t)_buffid] = 0;
	}
}

void CropProtectState::OnHeartbeat(FarmCrop *crop)
{
	if (Timer::GetTime() > _last_heartbeat_time)
	{
		crop->_protect_period[(size_t)_buffid] -= Timer::GetTime()-_last_heartbeat_time;
		if (crop->_protect_period[(size_t)_buffid] < 0)
			crop->_protect_period[(size_t)_buffid] = 0;
		_last_heartbeat_time = Timer::GetTime();
	}
}

void FarmProtectState::OnLeave(State *prev, int action, const void *action_param, Farm *farm)
{
	if (_expire_time > _last_heartbeat_time)
	{
		farm->_protect_period[(size_t)_buffid] -= _expire_time-_last_heartbeat_time;
		if (farm->_protect_period[(size_t)_buffid] < 0)
			farm->_protect_period[(size_t)_buffid] = 0;
	}
}

void FarmProtectState::OnHeartbeat(Farm *farm)
{
	if (Timer::GetTime() > _last_heartbeat_time)
	{
		farm->_protect_period[(size_t)_buffid] -= Timer::GetTime()-_last_heartbeat_time;
		if (farm->_protect_period[(size_t)_buffid] < 0)
			farm->_protect_period[(size_t)_buffid] = 0;
		_last_heartbeat_time = Timer::GetTime();
	}
}

void PlowState::OnEnter(State *prev, int action, const void *action_param, FarmField *field)
{
	ParamState<FarmField>::OnEnter(prev, action, action_param, field);
	if (field->_type == HTF_FIELD_TYPE_LOVE) 
	{
		field->_friend = 0;
		field->_friend_name.clear();
	}
	field->_crop.RawClearStates();
	field->_has_crop = false;
}

}
