#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threadpool.h>
#include <conf.h>
#include <io/pollio.h>
#include <io/passiveio.h>
#include <gsp_if.h>
#include <db_if.h>
#include <amemory.h>
#include <glog.h>
#include <algorithm>

#include "facbase_world_manager.h"
#include "../obj_interface.h"
#include "../gmatrix.h"
#include "../config.h"
#include "../playertemplate.h"
#include <factionlib.h>
#include <timer.h>

extern abase::timer g_timer;

void fac_prop_add_event::OnEvent()
{
	mgr->OnPropAddEvent(prop_type, value);
}

void fac_item_add_event::OnEvent()
{
	mgr->OnItemAddEvent(item_id, count);
}

void facbase_auction::OnEnd()
{
	if (winner > 0)
	{
		GMSV::SendFacDynWinAuction(pbase->GetFactionId(), winner_name.begin(), winner_name.size(), itemid, cost);
		pbase->AddAucHistory(facbase_auc_history::WIN_AUC, winner_name, cost, itemid);
	}
	else
		pbase->AddAucHistory(facbase_auc_history::AUC_TIMEOUT, winner_name, cost, itemid);
	pbase->AuctionUpdateBroadcast(S2C::CMD::facbase_auction_update::END, *this);
	GLog::formatlog("formatlog:facbase_auction:end:fid=%d:timestamp=%d:itemid=%d:winner=%d:cost=%d",
			pbase->GetFactionId(), timestamp, itemid, winner, cost);
}

void facbase_auction::OnDel()
{
	pbase->AuctionUpdateBroadcast(S2C::CMD::facbase_auction_update::DEL, *this);
	GLog::formatlog("formatlog:facbase_auction:delete:timestamp=%d:itemid=%d:winner=%d:cost=%d",
			timestamp, itemid, winner, cost);
}

bool facbase_auction::Update(int now, bool & changed)
{
	if (status == ST_BIDDING)
	{
		if (now > end_time)
		{
			status = ST_END;
			OnEnd();
			changed = true;
			if (winner == 0)
			{
				OnDel();//无人竞拍立即删除
				return true;
			}
		}
	}
	else if (status == ST_END)
	{
		if (now > end_time + END_TIME)
		{
			OnDel();
			changed = true;
			return true; //delete item
		}
	}
	return false;
}

facbase_world_manager::facbase_world_manager()
{
	_faction_id = 0;
	_grass = 0;
	_mine = 0;
	_monster_food = 0;
	_monster_core = 0;
	_cash = 0;

	_status = FBS_FREE;
	_status_timeout = 0;
	_base_lock = 0;
	_building_index = 0;
	_building_taskid = 0;

	_heartbeat_counter = 0; 
	_dirty = false;
	_need_close_re = false;

	//_event_lock = 0;
}

void facbase_world_manager::Reset()
{
	_faction_id = 0;
	_grass = 0;
	_mine = 0;
	_monster_food = 0;
	_monster_core = 0;
	_cash = 0;

	fields.clear();
	_msg.clear();
	auctions.clear();
	auction_failers.clear();
	auction_history.clear();

	_status = FBS_FREE;
	_status_timeout = 0;
	_building_index = 0;
	_building_taskid = 0;

	_heartbeat_counter = 0; 
	_dirty = false;
	_need_close_re = false;

	_cash_items_cooldown_info.clear();
	EventMap::iterator it, ite = event_map.end();
	for (it = event_map.begin(); it != ite; ++it)
		it->second->Destroy();
	event_map.clear();

	_mall_indexes.clear();
	_all_list.clear();
	//_event_lock = 0;
}


facbase_world_manager::~facbase_world_manager()
{
//	Release();	
}

namespace
{
	class GetFacBaseTask : public abase::ASmallObject, public GDB::FBaseResult
	{
		int _tag;
		std::string servername;
		facbase_world_manager * _manager;
	public:
		GetFacBaseTask(int tag, facbase_world_manager * manager) : _tag(tag), _manager(manager)
		{
		}
		~GetFacBaseTask()
		{
		}
		/*
		virtual void OnTimeOut()
		{
			if(0 == _manager->GetGetCounter())
			{
				_manager->OnGetCityDetail(bf_world_manager_base::CBFR_GET_DB_TIME_OUT,0,NULL,0);
				GLog::log(GLOG_ERR,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)读取数据超时",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)读取数据超时\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
			}
			else
			{
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)读取数据超时,重试(%d)!!!\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle(),_manager->GetGetCounter());
				_manager->GetCityDetail();
			}
		}
		*/
		virtual void OnGetData(int fid, const GDB::fac_base_info * pinfo)
		{
			_manager->OnGetFacBase(fid, _tag, pinfo);
			delete this;
		}
		virtual void OnPutData()
		{
			ASSERT(false);
		}
		virtual void OnFailed()
		{
			_manager->OnGetFacBaseFail();
			delete this;
		}
	};

	/*
	class PutCityDetailTask : public abase::ASmallObject, public GDB::City_Op_Result
	{
		bf_world_manager_base* _manager;
	public:
		PutCityDetailTask(bf_world_manager_base* manager):_manager(manager)
		{
		}
		~PutCityDetailTask()
		{
		}
		virtual void OnTimeOut()
		{
			if(0 == _manager->GetPutCounter())
			{
				_manager->OnPutCityDetail(bf_world_manager_base::CBFR_GET_DB_TIME_OUT);
				GLog::log(GLOG_ERR,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)存储数据超时",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)存储数据超时\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle());
			}
			else
			{
				__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d,use_for_battle= %d)存储数据超时,重试(%d)!!!\n",
					gmatrix::GetServerIndex(),_manager->GetBattleID(),_manager->GetWorldTag(),_manager->IsUseForBattle(),_manager->GetPutCounter());
				spin_autolock keeper(*_manager->GetBattleLock());
				_manager->PutCityDetail();
			}
		}
		virtual void OnGetCity(int battle_id,int stamp,const void* buf,size_t size)
		{
			ASSERT(false);
		}
		virtual void OnPutCity(int battle_id,int ret_code)
		{
			if(battle_id != _manager->GetBattleID())
			{
				__PRINTF("怎么可能呢\n");
				return;
			}
			_manager->OnPutCityDetail(ret_code);
		}
	};
	*/
}

int facbase_world_manager::TestAllocBase(int tag, int fid)
{
	{
		spin_autolock keeper(_base_lock);
		if (_status != FBS_FREE)
			return -1;
		_faction_id = fid;
		_status = FBS_GETTINGDATA;
	}
	GDB::get_faction_base(fid, new GetFacBaseTask(tag, this));
	return 0;
}

void facbase_world_manager::OnGetFacBaseFail()
{
	int fid = 0;
	{
		spin_autolock keeper(_base_lock);
		if (_status != FBS_GETTINGDATA)
		{
			GLog::log(GLOG_ERR,"帮派 %d 加载失败后发现状态 %d 错误", _faction_id, _status);
			return;
		}
		fid = _faction_id;
		_faction_id = 0;
		_status = FBS_FREE;
	}
	GNET::SendFacBaseStartRe(-6, fid, gmatrix::GetServerIndex(), -1, std::set<int>());
}

bool facbase_world_manager::LoadData(const GDB::fac_base_info * info)
{
	ASSERT(_base_lock);
	_faction_id = info->fid;
	_grass = info->grass;
	_mine = info->mine;
	_monster_food = info->monster_food;
	_monster_core = info->monster_core;
	_cash = info->cash;
	//_msg = info->msg;
	_msg = abase::octets(info->msg.data, info->msg.size);
	_db_timestamp = info->timestamp;

	fields.clear();
	_mall_indexes.clear();
	std::vector<GDB::fac_field>::const_iterator it, ite = info->fields.end();
	for (it = info->fields.begin(); it != ite; ++it)
	{
		facbase_building bud;
		bud.tid = it->tid;
		bud.level = it->level;
		bud.status = it->status;
		bud.task_id = it->task_id;
		bud.task_count = it->task_count;
		bud.task_need = it->task_need;
		fields[it->index] = bud;
		if (bud.status == facbase_building::IN_BUILDING)
		{
			_building_index = it->index;
			_building_taskid = it->task_id;
		}
		if (bud.level > 0)
		{
			DATA_TYPE dt;
			const BUILDING_ESSENCE * building_cfg = (const BUILDING_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(it->tid, ID_SPACE_ESSENCE, dt);
			if (dt != DT_BUILDING_ESSENCE || building_cfg == NULL)
			{
				GLog::log(GLOG_ERR, "加载基地，但错误的基地建筑模板 fid %d tid %d index %d", _faction_id, it->tid, it->index);
			}
			else
				OnBuildingUpgrade(building_cfg, it->index, it->level-1);
		}
	}
	//激活基地商城中默认开放的物品
	player_template::MALL_ITEM_INFO::iterator mit, mite = player_template::GetInstance().mall_items.end();
	for (mit = player_template::GetInstance().mall_items.begin(); mit != mite; ++mit)
	{
		if (mit->second.build_id == 0 && mit->second.build_level == 0)
			_mall_indexes.insert(mit->first);
	}
	
	auctions.clear();
	std::vector<GDB::fac_auction>::const_iterator ait, aite = info->auctions.end();
	for (ait = info->auctions.begin(); ait != aite; ++ait)
	{
		facbase_auction auc;
		auc.pbase = this;
		auc.timestamp = ait->timestamp;
		auc.itemid = ait->itemid;
		auc.winner = ait->winner;
		auc.cost = ait->cost;
//		auc.winner_name = ait->winner_name;
		auc.winner_name = abase::octets(ait->winner_name.data, ait->winner_name.size);
		auc.status = ait->status;
		auc.end_time = ait->end_time;
		auctions[ait->timestamp] = auc;
	}

	auction_failers.clear();
	std::vector<GDB::fac_auc_failer>::const_iterator fit, fite = info->auction_failers.end();
	for (fit = info->auction_failers.begin(); fit != fite; ++fit)
		auction_failers[fit->roleid] = fit->return_coupon;

	auction_history.clear();
	std::vector<GDB::fac_auc_history>::const_iterator hit, hite = info->auction_history.end();
	for (hit = info->auction_history.begin(); hit != hite; ++hit)
	{
		facbase_auc_history history;
		history.event_type = hit->event_type;
//		history.rolename = hit->rolename;
		history.rolename = abase::octets(hit->rolename.data, hit->rolename.size);
		history.cost = hit->cost;
		history.itemid = hit->itemid;
		auction_history.push_back(history);
	}
	return true;
}

void facbase_world_manager::OnGetFacBase(int fid, int tag, const GDB::fac_base_info * pinfo)
{
	int fake_tag = tag + gmatrix::RAID_OR_FBASE_TAG_BEGIN;
	spin_autolock keeper(_base_lock);
	if (_status != FBS_GETTINGDATA)
	{
		GLog::log(GLOG_ERR,"帮派 %d 加载后发现状态 %d 错误 tag=%d", fid, _status, fake_tag);
		return;
	}
	gmatrix::facbase_field_info & fbase_info = gmatrix::GetFacBaseInfo();
	int ret = global_world_manager::Init(0, fbase_info.servername.c_str(), fake_tag, fake_tag);
	if (ret != 0)
	{
		GLog::log(GLOG_ERR,"帮派 %d 基地初始化失败 ret=%d", fid, ret);
		return;
	}
	LoadData(pinfo); //因为 LoadData 会激活控制器 所以调用必须发生在 Init() 之后
	/*
	delete _message_handler;
	_message_handler = new raid_world_message_handler( this, &_plane );
	*/
	_status = FBS_READY;
//	gmatrix::IncFacBaseNum();
	GNET::SendFacBaseStartRe(0, fid, gmatrix::GetServerIndex(), fake_tag, _mall_indexes);
}

facbase_building * facbase_world_manager::GetField(int index)
{
	ASSERT(_base_lock);
	FieldMap::iterator it = fields.find(index);
	if (it == fields.end())
		return NULL;
	return &(it->second);
}

int facbase_world_manager::AddFacBuilding(int index, int tid, XID roleid, int name_len, char * playername)
{
	spin_autolock keeper(_base_lock);
	if (_status != FBS_READY)
		return S2C::ERR_FBASE_STATUS;
	if (index <= 0)
		return S2C::ERR_FBASE_LOGIC;
	if (_building_index != 0)
		return S2C::ERR_FBASE_BUILD_REPEAT;
	fbase_field_cfg * field_cfg = gmatrix::GetFBaseCtrl().GetFieldCfg(index);
	if (field_cfg == NULL)
		return S2C::ERR_FBASE_FIELD_INDEX;
	facbase_building * field = GetField(index);
	if (field != NULL)
		return S2C::ERR_FBASE_FIELD_REPEAT;
	if (!field_cfg->AvailBuilding(tid))
		return S2C::ERR_FBASE_BUILDING_UNAVAIL;

	DATA_TYPE dt;
	const BUILDING_ESSENCE * building_cfg = (const BUILDING_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(tid, ID_SPACE_ESSENCE, dt);
	if (dt != DT_BUILDING_ESSENCE || building_cfg == NULL)
	{
		printf("错误的基地建筑模板 tid %d\n", tid);
		return S2C::ERR_FBASE_BUILDING_TID;
	}

	abase::hash_map<int,/*building_tid*/int/*level*/> level_map;
	FieldMap::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second.tid > 0)
		{
			if (it->second.level > level_map[it->second.tid])
				level_map[it->second.tid] = it->second.level;
		}
	}

	for (unsigned int i = 0; i < sizeof(building_cfg->up_building[0].req_building)/sizeof(building_cfg->up_building[0].req_building[0]); i++)
	{
		int pre_tid = building_cfg->up_building[0].req_building[i].building_id;
		if (pre_tid > 0)
		{
			if (level_map[pre_tid] < building_cfg->up_building[0].req_building[i].level)
				return S2C::ERR_FBASE_BUILDING_COND;
		}
	}
	int cost_grass = building_cfg->up_building[0].grass_num;
	int cost_mine = building_cfg->up_building[0].mine_num;
	int cost_food = building_cfg->up_building[0].monster_food_num;
	int cost_core = building_cfg->up_building[0].monster_core_num;
	int cost_cash = building_cfg->up_building[0].money_num;
	if (_grass < cost_grass	|| _mine < cost_mine || _monster_food < cost_food || _monster_core < cost_core || _cash < cost_cash)
		return S2C::ERR_FBASE_BUILDING_COND;
	//检查通过
	_grass -= cost_grass;
	_mine -= cost_mine;
	_monster_food -= cost_food;
	_monster_core -= cost_core;
	_cash -= cost_cash;

	facbase_building build;
	build.tid = tid;
	build.task_count = 0;
	if (building_cfg->up_building[0].task_id <= 0 || building_cfg->up_building[0].finish_num <= 0)
	{
		//立刻完成升级
		//建筑数据修改
		build.level = 1;
		build.status = facbase_building::BUILDING_COMPLETE;
		build.task_id = 0;
		build.task_need = 0;
		//基地内成员广播
		packet_wrapper h1(128);
		using namespace S2C;
		CMD::Make<CMD::fac_building_complete>::From(h1, index, tid, 1); 
		//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
		GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
		/*
		MSG msg;
		msg_fac_building_complete info;
		info.index = index;
		info.tid = tid;
		info.level = 1;
		BuildMessage(msg, GM_MSG_FAC_BUILDING_COMPLETE, roleid, XID(-1,-1),A3DVECTOR(0,0,0), 0, &info, sizeof(info));
		gmatrix::SendMessage(msg);
		*/
		OnBuildingUpgrade(building_cfg, index, 0);
		GMSV::SendFacDynBuildingComplete(_faction_id, build.tid, build.level);
		GMSV::SendFacMallChange(_faction_id, _mall_indexes);
	}
	else
	{
		build.level = 0;
		build.status = facbase_building::IN_BUILDING;
		build.task_id = building_cfg->up_building[0].task_id;
		build.task_need = building_cfg->up_building[0].finish_num;

		_building_index = index;
		_building_taskid = build.task_id;

		//基地内成员广播
		packet_wrapper h1(128);
		using namespace S2C;
		CMD::Make<CMD::fac_building_add>::From(h1, index, tid); 
		//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
		GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
		GMSV::SendFacDynBuildingUpgrade(_faction_id, playername, name_len, tid, 1, cost_grass, cost_mine, cost_food, cost_core);
	}
	fields[index] = build;
	NotifyPropChange(roleid.id);
	_dirty = true;
	GLog::formatlog("formatlog:addfacbuilding:fid=%d:roleid=%d:index=%d:tid=%d", _faction_id, roleid.id, index, tid);
	return 0;
}

int facbase_world_manager::UpgradeFacBuilding(int index, int tid, XID roleid, int lev, int name_len, char * playername)
{
	spin_autolock keeper(_base_lock);
	if (_status != FBS_READY)
		return S2C::ERR_FBASE_STATUS;
	if (_building_index != 0)
		return S2C::ERR_FBASE_BUILD_REPEAT;
	facbase_building * field = GetField(index);
	if (field == NULL)
		return S2C::ERR_FBASE_UPGRADE_INDEX;
	if (field->tid != tid)
		return S2C::ERR_FBASE_UPGRADE_TID;
	if (field->level != lev)
		return S2C::ERR_FBASE_UPGRADE_LEV;
	if (field->status == facbase_building::IN_BUILDING)
		return S2C::ERR_FBASE_BUILD_REPEAT;

	DATA_TYPE dt;
	const BUILDING_ESSENCE * building_cfg = (const BUILDING_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(tid, ID_SPACE_ESSENCE, dt);
	if (dt != DT_BUILDING_ESSENCE || building_cfg == NULL)
	{
		printf("错误的基地建筑模板 tid %d\n", tid);
		return S2C::ERR_FBASE_BUILDING_TID;
	}
	int max_lev = 0;
	for (; max_lev < (int)(sizeof(building_cfg->up_building)/sizeof(building_cfg->up_building[0])); max_lev++)
	{
		if (building_cfg->up_building[max_lev].building_model == 0)
			break;
	}
	if (lev >= max_lev)
		return S2C::ERR_FBASE_UPGRADE_FULL;

	abase::hash_map<int,/*building_tid*/int/*level*/> level_map;
	FieldMap::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second.tid > 0)
		{
			if (it->second.level > level_map[it->second.tid])
				level_map[it->second.tid] = it->second.level;
		}
	}

	for (unsigned int i = 0; i < sizeof(building_cfg->up_building[lev].req_building)/sizeof(building_cfg->up_building[lev].req_building[0]); i++)
	{
		int pre_tid = building_cfg->up_building[lev].req_building[i].building_id;
		if (pre_tid > 0)
		{
			if (level_map[pre_tid] < building_cfg->up_building[lev].req_building[i].level)
				return S2C::ERR_FBASE_BUILDING_COND;
		}
	}
	int cost_grass = building_cfg->up_building[lev].grass_num;
	int cost_mine = building_cfg->up_building[lev].mine_num;
	int cost_food = building_cfg->up_building[lev].monster_food_num;
	int cost_core = building_cfg->up_building[lev].monster_core_num;
	int cost_cash = building_cfg->up_building[lev].money_num;
	if (_grass < cost_grass || _mine < cost_mine || _monster_food < cost_food || _monster_core < cost_core || _cash < cost_cash)
		return S2C::ERR_FBASE_BUILDING_COND;
	//检查通过
	_grass -= cost_grass;
	_mine -= cost_mine;
	_monster_food -= cost_food;
	_monster_core -= cost_core;
	_cash -= cost_cash;

	if (building_cfg->up_building[lev].task_id <= 0 || building_cfg->up_building[lev].finish_num <= 0)
	{
		//立刻完成升级
		field->level++;
		field->status = facbase_building::BUILDING_COMPLETE;
		field->task_id = 0;
		field->task_count = 0;
		field->task_need = 0;

		_building_index = 0;
		_building_taskid = 0;

		//基地内成员广播
		packet_wrapper h1(128);
		using namespace S2C;
		CMD::Make<CMD::fac_building_complete>::From(h1, index, tid, field->level); 
		//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
		GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
		/*
		MSG msg;
		msg_fac_building_complete info;
		info.index = index;
		info.tid = tid;
		info.level = field->level;
		BuildMessage(msg, GM_MSG_FAC_BUILDING_COMPLETE, roleid, XID(-1,-1),A3DVECTOR(0,0,0), 0, &info, sizeof(info));
		gmatrix::SendMessage(msg);
		*/
		OnBuildingUpgrade(building_cfg, index, lev);
		GMSV::SendFacDynBuildingComplete(_faction_id, tid, field->level);
		GMSV::SendFacMallChange(_faction_id, _mall_indexes);
	}
	else
	{
		//build.level = 0;
		field->status = facbase_building::IN_BUILDING;
		field->task_id = building_cfg->up_building[lev].task_id;
		field->task_need = building_cfg->up_building[lev].finish_num;
		field->task_count = 0;

		_building_index = index;
		_building_taskid = field->task_id;

		//基地内成员广播
		packet_wrapper h1(128);
		using namespace S2C;
		CMD::Make<CMD::fac_building_upgrade>::From(h1, index, tid, lev); 
		//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
		GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
		GMSV::SendFacDynBuildingUpgrade(_faction_id, playername, name_len, tid, lev+1, cost_grass, cost_mine, cost_food, cost_core);
	}
	NotifyPropChange(roleid.id);
	_dirty = true;
	GLog::formatlog("formatlog:upgradefacbuilding:fid=%d:roleid=%d:index=%d:tid=%d:lev=%d", _faction_id, roleid.id, index, tid, lev);
	return 0;
}
int facbase_world_manager::RemoveFacBuilding(int index, int tid)
{
	spin_autolock keeper(_base_lock);
	if (_status != FBS_READY)
		return S2C::ERR_FBASE_STATUS;
	if (_building_index != 0)
		return S2C::ERR_FBASE_LOGIC;
	facbase_building * field = GetField(index);
	if (field == NULL)
		return S2C::ERR_FBASE_REMOVE_INDEX;
	if (field->tid != tid)
		return S2C::ERR_FBASE_UPGRADE_TID;
	if (field->status == facbase_building::IN_BUILDING)
		return S2C::ERR_FBASE_REMOVE_WHILE_INBUILDING;

	DATA_TYPE dt;
	const BUILDING_ESSENCE * building_cfg = (const BUILDING_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(tid, ID_SPACE_ESSENCE, dt);
	if (dt != DT_BUILDING_ESSENCE || building_cfg == NULL)
	{
		printf("错误的基地建筑模板 tid %d\n", tid);
		return S2C::ERR_FBASE_BUILDING_TID;
	}
	int cfg_lev = (field->status == facbase_building::BUILDING_COMPLETE ? field->level-1 : field->level);
	for (unsigned int i = 0; i < sizeof(building_cfg->up_building[cfg_lev].controller_open)/sizeof(building_cfg->up_building[cfg_lev].controller_open[0]); i++)
	{
		if(building_cfg->up_building[cfg_lev].controller_open[i] > 0)
		{
			__PRINTF("close facbase controller %d\n", building_cfg->up_building[cfg_lev].controller_open[i]);
			ActiveSpawn(this, building_cfg->up_building[cfg_lev].controller_open[i], false);
		}
	}
	//关闭基地商城中的新物品
	player_template::MALL_ITEM_INFO::iterator mit, mite = player_template::GetInstance().mall_items.end();
	for (mit = player_template::GetInstance().mall_items.begin(); mit != mite; ++mit)
	{
		if (mit->second.build_id == tid)
			_mall_indexes.erase(mit->first);
	}

	{
		EventMap::iterator eit, eite = event_map.end();
		for (eit = event_map.begin(); eit != eite;)
		{
			if (eit->second->field_index == index
					&& eit->second->building_lev == field->level)
			{
				eit->second->Destroy();
				event_map.erase(eit++);
			}
			else
				++eit;
		}
	}

	if (_building_index == index)
	{
		_building_index = 0;
		_building_taskid = 0;
	}

	//基地内成员广播
	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::fac_building_remove>::From(h1, index); 
	//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
	GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());

	fields.erase(index);
	_dirty = true;
	GLog::formatlog("formatlog:removefacbuilding:fid=%d:index=%d:tid=%d", _faction_id, index, tid);
	return 0;
}

int facbase_world_manager::GetClientTag()
{
	return gmatrix::GetFacBaseInfo().world_tag; //客户端用的 tag
}

void facbase_world_manager::OnActiveSpawn(int id, bool active)
{
	global_world_manager::OnActiveSpawn(id, active, true, GetWorldTag());
}

world_manager::INIT_RES facbase_world_manager::OnInitMoveMap()
{
	__PRINTF("基地自己初始化 MoveMap\n");
	_movemap = gmatrix::GetFBaseCtrl().GetMoveMap();
	return world_manager::INIT_SUCCESS;
}
world_manager::INIT_RES facbase_world_manager::OnInitCityRegion()
{
	__PRINTF("基地自己初始化 CityRegion\n");
	_region = gmatrix::GetFBaseCtrl().GetCityRegion();
	return world_manager::INIT_SUCCESS;
}
world_manager::INIT_RES facbase_world_manager::OnInitBuffArea()
{
	__PRINTF("基地自己初始化 BuffArea\n");
	_buffarea = gmatrix::GetFBaseCtrl().GetBuffArea();
	return world_manager::INIT_SUCCESS;
}
world_manager::INIT_RES facbase_world_manager::OnInitPathMan()
{
	__PRINTF("基地自己初始化 PathMan\n");
	_pathman = gmatrix::GetFBaseCtrl().GetPathMan();
	return world_manager::INIT_SUCCESS;
}
bool facbase_world_manager::OnTraceManAttach()
{
	__PRINTF("基地加载共享凸包数据\n");
	_trace_man.Attach(&(gmatrix::GetFBaseCtrl().GetTraceMan()));
	return true;
}

void facbase_world_manager::SaveData(GDB::FBaseResult * callback)
{
	__PRINTF("基地存盘 fid=%d\n", _faction_id);
	//应该已经完成锁定
	ASSERT(_base_lock);

	GDB::fac_base_info info;
	info.fid = _faction_id;
	info.grass = _grass;
	info.mine = _mine;
	info.monster_food = _monster_food;
	info.monster_core = _monster_core;
	info.cash = _cash;
	info.timestamp = IncreaseDBTimeStamp();
	//info.msg = _msg;
	info.msg.data = _msg.begin();
	info.msg.size = _msg.size();

	FieldMap::const_iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		GDB::fac_field fd;
		fd.index = it->first;
		fd.tid = it->second.tid;
		fd.level = it->second.level;
		fd.status = it->second.status;
		fd.task_id = it->second.task_id;
		fd.task_count = it->second.task_count;
		fd.task_need = it->second.task_need;
		info.fields.push_back(fd);
	}
	AuctionMap::iterator ait, aite = auctions.end();
	for (ait = auctions.begin(); ait != aite; ++ait)
	{
		GDB::fac_auction auc;
		auc.timestamp = ait->second.timestamp;
		auc.itemid = ait->second.itemid;
		auc.winner = ait->second.winner;
		auc.cost = ait->second.cost;
//		auc.winner_name = ait->second.winner_name;
		auc.winner_name.data = ait->second.winner_name.begin();
		auc.winner_name.size = ait->second.winner_name.size();
		auc.status = ait->second.status;
		auc.end_time = ait->second.end_time;
		info.auctions.push_back(auc);
	}
	AuctionFailers::iterator fit, fite = auction_failers.end();
	for (fit = auction_failers.begin(); fit != fite; ++fit)
	{
		GDB::fac_auc_failer failer;
		failer.roleid = fit->first;
		failer.return_coupon = fit->second;
		info.auction_failers.push_back(failer);
	}
	AuctionHistory::iterator hit, hite = auction_history.end();
	for (hit = auction_history.begin(); hit != hite; ++hit)
	{
		GDB::fac_auc_history history;
		history.event_type = hit->event_type;
//		history.rolename = hit->rolename;
		history.rolename.data = hit->rolename.begin();
		history.rolename.size = hit->rolename.size();
		history.cost = hit->cost;
		history.itemid = hit->itemid;
		info.auction_history.push_back(history);
	}
/*---------------------------发出保存请求------------------------------------*/
	GDB::put_faction_base(_faction_id, info, callback);
	GLog::log(GLOG_INFO,"save facbase: 发送保存基地消息 fid=%d", _faction_id);
}

void facbase_world_manager::AutoSave()
{
	class PutFacBaseTask : public abase::ASmallObject, public GDB::FBaseResult
	{	
		int fid;
		/*
		world * _plane;
		int _userid;
		unsigned int _counter2; 
		*/
	public:
		PutFacBaseTask(int id)
		{
			fid = id;
		//	_plane = imp->_plane;
		//	_userid = imp->_parent->ID.id;
		}

		virtual void OnFailed() 
		{
			GLog::log(GLOG_ERR,"帮派基地 %d 存盘失败", fid);
			/*
			MSG msg;
			BuildMessage(msg,GM_MSG_DBSAVE_ERROR,XID(GM_TYPE_PLAYER,_userid),XID(GM_TYPE_PLAYER,_userid),A3DVECTOR(0,0,0));
			gmatrix::SendMessage(msg);
			*/
			delete this;
		}

		virtual void OnPutData()
		{
			GLog::log(GLOG_INFO,"帮派基地 %d 自动存盘完成", fid);
			delete this;
		}

		virtual void OnGetData(int fid, const GDB::fac_base_info * pinfo)
		{
			ASSERT(false);
		}
	};
	SaveData(new PutFacBaseTask(_faction_id));
}

void facbase_world_manager::GetBaseInfo(S2C::CMD::player_fac_base_info & base_info, std::vector<S2C::INFO::player_fac_field> & field_info, abase::octets & msg)
{
	spin_autolock keeper( _base_lock );

	base_info.prop.grass = _grass;
	base_info.prop.mine = _mine;
	base_info.prop.monster_food = _monster_food;
	base_info.prop.monster_core = _monster_core;
	base_info.prop.cash = _cash;

	base_info.prop.task_id = _building_taskid;
	if (_building_index)
	{
		base_info.prop.task_count = fields[_building_index].task_count;
		base_info.prop.task_need = fields[_building_index].task_need;
	}
	else
	{
		base_info.prop.task_count = 0;
		base_info.prop.task_need = 0;
	}

	FieldMap::const_iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		S2C::INFO::player_fac_field fd;
		fd.index = it->first;
		fd.tid = it->second.tid;
		fd.level = it->second.level;
		fd.status = it->second.status;
		field_info.push_back(fd);
	}

	msg = _msg;
}

int facbase_world_manager::GetBuildingLevel(int iBuildId)
{
	spin_autolock keeper( _base_lock );
	FieldMap::const_iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second.tid == iBuildId)
			return it->second.level;
	}
	return 0;
}

int facbase_world_manager::GetInBuildingLevel()
{
	spin_autolock keeper( _base_lock );
	if (_building_index == 0)
		return 0;
	facbase_building * field = GetField(_building_index);
	if (field == NULL)
		return 0;
	return field->level + 1;
}

void facbase_world_manager::AddEvent(int time, fac_building_event * eve)
{
//	ASSERT(_event_lock);
	ASSERT(_base_lock);
	event_map.insert(std::make_pair(time, eve));
}

void facbase_world_manager::Release()
{
	Reset();
	global_world_manager::Release();
	gmatrix::CleanWorldActiveCtrlList(GetWorldTag());
}

void facbase_world_manager::Heartbeat()
{
	if (_status == FBS_FREE || _status == FBS_GETTINGDATA)
		return;

	if (_status != FBS_CLOSING) //CLOSING 状态停止心跳 这样就不会再刷出新怪?
		global_world_manager::Heartbeat();

	++_heartbeat_counter; 

	if (_heartbeat_counter % 20 != 0)
		return;

	spin_autolock keeper( _base_lock );

	if (_status == FBS_READY)
	{
		int now = g_timer.get_systime();

		std::set<fac_building_event *> event_set;
		EventMap::iterator it, ite = event_map.end();
		for (it = event_map.begin(); it != ite; )
		{
			if (it->first >= now)
				break;
			it->second->OnEvent();
			event_set.insert(it->second);
			event_map.erase(it++);
		}
		std::set<fac_building_event *>::const_iterator eit, eite = event_set.end();
		for (eit = event_set.begin(); eit != eite; ++eit)
			AddEvent(now + (*eit)->time_period, *eit);

		if (_heartbeat_counter % (20 * 30) == 0) //30 sec
		{
			AuctionMap::iterator ait;
			for (ait = auctions.begin(); ait != auctions.end(); )
			{
				bool changed = false;
				if (ait->second.Update(now, changed))
				{
					AuctionMap::iterator tmp = ait;
					++ait;
					auctions.erase(tmp);
				}
				else
					++ait;
				if (changed)
					_dirty = true;
			}
		}

		if (_heartbeat_counter % (20 * 30) == 0) //30 sec
		{
			if (_dirty && CanSave())
			{
				AutoSave();
				_dirty = false;
			}
		}
	}
	else if (_status == FBS_CLOSING)	
	{
		if (_dirty && CanSave())
		{
			AutoSave();
			_dirty = false;
		}
		if (g_timer.get_systime() > _status_timeout )
		{
			GLog::log(GLOG_ERR, "基地关闭倒计时时间到 tag=%d need_close_re %d", GetWorldTag(), _need_close_re);
			if (_need_close_re)
				GNET::SendFacBaseStopRe(0, _faction_id, gmatrix::GetServerIndex(), GetWorldTag());
			Release();
		}
	}
}

void facbase_world_manager::OnFacBasePropChange(int noti_roleid, int fid, int type, int delta)
{
	spin_autolock keeper( _base_lock );
	if (fid != _faction_id)
	{
		GLog::log(GLOG_ERR,"基地属性同步修改 fid %d:%d 不匹配 type %d delta %d",
				fid, _faction_id, type, delta);
		return;
	}
	ChangeProp(noti_roleid, type, delta);
}

void facbase_world_manager::OnFacBaseBuildingProgress(int fid, int task_id, int value)
{
	spin_autolock keeper( _base_lock );
	if (fid != _faction_id)
	{
		GLog::log(GLOG_ERR,"基地建筑建设进度增加 fid %d:%d 不匹配 task_id %d value %d",
				fid, _faction_id, task_id, value);
		return;
	}
	if (task_id == 0 || task_id != _building_taskid)
		return;
	facbase_building * field = GetField(_building_index);
	if (field == NULL)
		return;
	field->task_count += value;
	if (field->task_count < field->task_need)
	{
		NotifyPropChange(0);
		return;
	}
	DATA_TYPE dt;
	const BUILDING_ESSENCE * building_cfg = (const BUILDING_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(field->tid, ID_SPACE_ESSENCE, dt);
	if (dt != DT_BUILDING_ESSENCE || building_cfg == NULL)
	{
		printf("错误的基地建筑模板 tid %d OnFacBaseBuildingProgress \n", field->tid);
		return;
	}

	//完成升级
	int lev = field->level;
	int index = _building_index;
	field->level++;
	field->status = facbase_building::BUILDING_COMPLETE;
	field->task_id = 0;
	field->task_count = 0;
	field->task_need = 0;

	_building_index = 0;
	_building_taskid = 0;

	//基地内成员广播
	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::fac_building_complete>::From(h1, index, field->tid, field->level); 
	//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
	GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
	/*
	MSG msg;
	msg_fac_building_complete info;
	info.index = index;
	info.tid = tid;
	info.level = field->level;
	BuildMessage(msg, GM_MSG_FAC_BUILDING_COMPLETE, roleid, XID(-1,-1),A3DVECTOR(0,0,0), 0, &info, sizeof(info));
	gmatrix::SendMessage(msg);
	*/
	NotifyPropChange(0);
	OnBuildingUpgrade(building_cfg, index, lev);
	GMSV::SendFacDynBuildingComplete(fid, field->tid, field->level);
	GMSV::SendFacMallChange(fid, _mall_indexes);
	_dirty = true;
	GLog::formatlog("formatlog:facbuildingupgradesuccess:fid=%d:index=%d", _faction_id, index);
}

void facbase_world_manager::NotifyPropChange(int noti_roleid)
{
	ASSERT(_base_lock);
	//基地内成员广播
	packet_wrapper h1(128);
	using namespace S2C;
	int task_count = 0;
	int task_need = 0;
	if (_building_index)
	{
		task_count = fields[_building_index].task_count;
		task_need = fields[_building_index].task_need;
	}
	INFO::fac_base_prop prop = {_grass, _mine, _monster_food, _monster_core, _cash, _building_taskid, task_count, task_need};
	CMD::Make<CMD::fac_base_prop_change>::From(h1, prop, noti_roleid); 
	//if (_all_list.size()) multi_send_ls_msg(_all_list, h1.data(), h1.size(), -1);
	GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
}

void facbase_world_manager::ChangeProp(int noti_roleid, int type, int delta)
{
	ASSERT(_base_lock);
	if (type == FBASE_PROP_GRASS)
		_grass += delta;
	else if (type == FBASE_PROP_MINE)
		_mine += delta;
	else if (type == FBASE_PROP_MONSTERFOOD)
		_monster_food += delta;
	else if (type == FBASE_PROP_MONSTERCORE)
		_monster_core += delta;
	else if (type == FBASE_PROP_CASH)
		_cash += delta;
	else
		GLog::log(GLOG_ERR,"基地属性同步修改 fid %d , invalid type %d delta %d",
				_faction_id, type, delta);
	NotifyPropChange(noti_roleid);
	_dirty = true;
}

void facbase_world_manager::OnPropAddEvent(int type, int value)
{
	ASSERT(_base_lock);
	ChangeProp(0, type, value);
}

void facbase_world_manager::OnItemAddEvent(int item_id, int count)
{
	ASSERT(_base_lock);
	for (int i = 0; i < count; i++)
	{
		int time = 0;
		if (PutAuction(item_id, time) != 0)
			break;
	}
	_dirty = true;
}

void facbase_world_manager::PlayerEnterBase(gplayer * pPlayer)
{
	__PRINTF("基地 %d 有玩家 %d 进入\n", _faction_id, pPlayer->ID.id);
	spin_autolock keeper( _base_lock );
	AddMapNode(_all_list, pPlayer);
}

void facbase_world_manager::PlayerLeaveBase(gplayer * pPlayer)
{
	__PRINTF("基地 %d 有玩家 %d 退出\n", _faction_id, pPlayer->ID.id);
	spin_autolock keeper( _base_lock );
	DelMapNode(_all_list, pPlayer);
}

void facbase_world_manager::OnBuildingUpgrade(const BUILDING_ESSENCE * building_cfg, int index, int old_lev)
{
	ASSERT(_base_lock);
	//关闭该关闭的控制器
	for (unsigned int i = 0; i < sizeof(building_cfg->up_building[old_lev].controller_close)/sizeof(building_cfg->up_building[old_lev].controller_close[0]); i++)
	{
		if(building_cfg->up_building[old_lev].controller_close[i] > 0)
		{
			__PRINTF("close facbase controller %d\n", building_cfg->up_building[old_lev].controller_close[i]);
			ActiveSpawn(this, building_cfg->up_building[old_lev].controller_close[i], false);
		}
	}
	//开启该开启的控制器
	for (unsigned int i = 0; i < sizeof(building_cfg->up_building[old_lev].controller_open)/sizeof(building_cfg->up_building[old_lev].controller_open[0]); i++)
	{
		if(building_cfg->up_building[old_lev].controller_open[i] > 0)
		{
			__PRINTF("open facbase controller %d\n", building_cfg->up_building[old_lev].controller_open[i]);
			ActiveSpawn(this, building_cfg->up_building[old_lev].controller_open[i], true);
		}
	}
	//清除旧建筑等级导致的事件
	EventMap::iterator eit, eite = event_map.end();
	for (eit = event_map.begin(); eit != eite;)
	{
		if (eit->second->field_index == index
				&& eit->second->building_lev == old_lev)
		{
			eit->second->Destroy();
			event_map.erase(eit++);
		}
		else
			++eit;
	}
	//生成新等级导致的事件
	int now = g_timer.get_systime();
	for (unsigned int i = 0; i < sizeof(building_cfg->up_building[old_lev].item)/sizeof(building_cfg->up_building[old_lev].item[0]); i++)
	{
		const BUILDING_ESSENCE::update_building::add_item * item_cfg = &(building_cfg->up_building[old_lev].item[i]);
		if (item_cfg->item_id > 0)
		{
			fac_building_event * eve = new fac_item_add_event(this, index, old_lev+1, item_cfg->time, item_cfg->item_id, item_cfg->item_num);
			AddEvent(now + item_cfg->time, eve);
		}
	}
	if (building_cfg->up_building[old_lev].added_grass_num > 0 && building_cfg->up_building[old_lev].added_grass_time > 0)
	{
		fac_building_event * eve = new fac_prop_add_event(this, index, old_lev+1, building_cfg->up_building[old_lev].added_grass_time, FBASE_PROP_GRASS, building_cfg->up_building[old_lev].added_grass_num);
		AddEvent(now + building_cfg->up_building[old_lev].added_grass_time, eve);
	}
	if (building_cfg->up_building[old_lev].add_mine_num > 0 && building_cfg->up_building[old_lev].add_mine_time > 0)
	{
		fac_building_event * eve = new fac_prop_add_event(this, index, old_lev+1, building_cfg->up_building[old_lev].add_mine_time, FBASE_PROP_MINE, building_cfg->up_building[old_lev].add_mine_num);
		AddEvent(now + building_cfg->up_building[old_lev].add_mine_time, eve);
	}
	if (building_cfg->up_building[old_lev].add_monster_core_num > 0 && building_cfg->up_building[old_lev].add_mosnter_core_tiem > 0)
	{
		fac_building_event * eve = new fac_prop_add_event(this, index, old_lev+1, building_cfg->up_building[old_lev].add_mosnter_core_tiem, FBASE_PROP_MONSTERCORE, building_cfg->up_building[old_lev].add_monster_core_num);
		AddEvent(now + building_cfg->up_building[old_lev].add_mosnter_core_tiem, eve);
	}
	if (building_cfg->up_building[old_lev].add_mosnter_food_num > 0 && building_cfg->up_building[old_lev].add_monster_food_time > 0)
	{
		fac_building_event * eve = new fac_prop_add_event(this, index, old_lev, building_cfg->up_building[old_lev].add_monster_food_time, FBASE_PROP_MONSTERFOOD, building_cfg->up_building[old_lev].add_mosnter_food_num);
		AddEvent(now + building_cfg->up_building[old_lev].add_monster_food_time, eve);
	}
	if (building_cfg->up_building[old_lev].add_money_num > 0 && building_cfg->up_building[old_lev].add_money_time > 0)
	{
		fac_building_event * eve = new fac_prop_add_event(this, index, old_lev+1, building_cfg->up_building[old_lev].add_money_time, FBASE_PROP_CASH, building_cfg->up_building[old_lev].add_money_num);
		AddEvent(now + building_cfg->up_building[old_lev].add_money_time, eve);
	}
	//激活基地商城中的新物品
	player_template::MALL_ITEM_INFO::iterator mit, mite = player_template::GetInstance().mall_items.end();
	for (mit = player_template::GetInstance().mall_items.begin(); mit != mite; ++mit)
	{
		if (mit->second.build_id == (int)building_cfg->id &&
				old_lev+1 >= mit->second.build_level)
			_mall_indexes.insert(mit->first);
	}
}

std::map<int, int> facbase_world_manager::GetCashItemsCooldownInfo()
{
	spin_autolock keeper( _base_lock );
	int cur_time = g_timer.get_systime();
	for( std::map<int,int>::iterator iter = _cash_items_cooldown_info.begin(); iter != _cash_items_cooldown_info.end(); iter++ )
	{
		if( iter->second <= cur_time )
		{
			_cash_items_cooldown_info.erase( iter );
		}
	}
	return _cash_items_cooldown_info;
}

void facbase_world_manager::NotifyCashItemCooldown()
{
	ASSERT( _base_lock );
	packet_wrapper tmp;
       	using namespace S2C;
       	CMD::Make<CMD::get_facbase_cash_items_info>::From( tmp, _cash_items_cooldown_info );
       	multi_send_ls_msg( _all_list, tmp );
}

int facbase_world_manager::TestCashItemCoolDown( const facbase_cash_item_info* iteminfo, int item_idx, int& result_time )
{
	ASSERT( _base_lock );
	int cur_time = g_timer.get_systime();
	int res_time = cur_time + iteminfo->cooldown_time;
	std::map<int,int>::iterator iter = _cash_items_cooldown_info.find( item_idx );
	if( ( iter != _cash_items_cooldown_info.end() ) && ( iter->second > cur_time ) )
	{
		NotifyCashItemCooldown();
		return S2C::ERR_FBASE_CASH_ITEM_IN_COOLDOWN;
	}
	result_time = res_time;
	return 0;
}

int facbase_world_manager::BuyCashItem(int roleid, int item_idx, int name_len, char * player_name)
{
	const facbase_cash_item_info* iteminfo = player_template::GetInstance().GetCashItemInfoByIndex( item_idx );
	if( !iteminfo ){ return S2C::ERR_FATAL_ERR; }

	spin_autolock keeper( _base_lock );
	if (_status != FBS_READY){ return S2C::ERR_FBASE_STATUS; }
	if( _cash < iteminfo->needed_faction_cash ){ return S2C::ERR_FBASE_NOT_ENOUGH_CASH; }
	int result_time = -1;
	int ret = TestCashItemCoolDown( iteminfo, item_idx, result_time );
	if( ret != 0 ){ return ret; }
	
	switch( iteminfo->award_type )
	{
		case AWARD_AUCTION_ITEM:
		{
			int time = 0;
			ret = PutAuction(iteminfo->auition_item_id, time);
			if (ret != 0)
				return ret;
			GMSV::SendFacDynBuyAuction(_faction_id, player_name, name_len, iteminfo->auition_item_id, time);
			AddAucHistory(facbase_auc_history::PUT_AUC, abase::octets(player_name, name_len), 0, iteminfo->auition_item_id);
			break;
		}
		case MULTIPLE_EXPERIENCE:
		{
			GMSV::SendFactionMultiExp(_faction_id, iteminfo->exp_multi,iteminfo->multi_exp_time + g_timer.get_systime());
			break;
		}
		case ENABLE_CONTROLLER:
		{
			__PRINTF("open facbase controller %d\n", iteminfo->controller_id);
			ActiveSpawn( this, iteminfo->controller_id, true );
			break;
		}
		default:
			break;
	}
	
	if( iteminfo->cooldown_time > 0 ){ _cash_items_cooldown_info[item_idx] = result_time; }
	ChangeProp(roleid, FBASE_PROP_CASH, -iteminfo->needed_faction_cash );
	GLog::formatlog("formatlog:buyfaccashitem:fid=%d:roleid=%d:item_idx=%d:cost_cash=%d:type=%d:auction_item=%d:controller_id=%d:multi_exp=%d:last_time=%d",
			_faction_id, roleid, item_idx, iteminfo->needed_faction_cash, iteminfo->award_type, iteminfo->auition_item_id, iteminfo->controller_id,
			iteminfo->exp_multi, iteminfo->multi_exp_time);
	_dirty = true;
	return 0;
}

void facbase_world_manager::UpdateFacBaseMsg(char * msg, size_t len)
{
	spin_autolock keeper( _base_lock );

	_msg = abase::octets(msg, len);

	packet_wrapper h1(256);
	using namespace S2C;
	CMD::Make<CMD::facbase_msg_update>::From(h1, msg, len);
	GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());

	_dirty = true;
}

void facbase_world_manager::KillAllMonsters()
{	
	//参考副本实现
	//需要残留怪物,物品等检测 全杀,策划保证战斗停止后所有控制器不会再触发	
	//策划是否能保证？ 还是需要手动关闭控制器？？？
	__PRINTF("基地(gs_id: %d, world_tag: %d) 杀死所有npc\n",		gmatrix::GetServerIndex(), GetWorldTag());	
	MSG msg;	
	BuildMessage(msg, GM_MSG_BATTLE_NPC_DISAPPEAR, XID(GM_TYPE_NPC,-1),XID(-1,-1),A3DVECTOR(0,0,0));	
	std::vector<exclude_target> empty;
	_plane.BroadcastSphereMessage(msg,A3DVECTOR(0,0,0),10000.f,0xFFFFFFFF,empty);
}


void facbase_world_manager::OnDisconnect()
{
	GLog::log(GLOG_ERR, "gdeliveryd 断线 释放基地 tag %d status %d", GetWorldTag(), _status);
	spin_autolock keeper( _base_lock );
	switch (_status)
	{
		case FBS_GETTINGDATA:
		{
			Reset();
			break;
		}
		case FBS_FREE:
		{
			break;
		}
		case FBS_READY:
		{
			_status = FBS_CLOSING;
			_status_timeout = g_timer.get_systime() + CLEAN_TIME;
			KillAllMonsters();
			break;
		}
		case FBS_CLOSING:
		{
			break;
		}
		default:
			break;
	}
}

void facbase_world_manager::OnFacBaseStop(int fid)
{
	GLog::log(GLOG_INFO, "gdeliveryd 关闭基地 tag %d status %d", GetWorldTag(), _status);
	spin_autolock keeper( _base_lock );
	if (fid != _faction_id)
	{
		GLog::log(GLOG_ERR, "close facbase fid %d:%d mismatch, tag=%d", fid, _faction_id, GetWorldTag());
		return;
	}
	switch (_status)
	{
		case FBS_GETTINGDATA:
		{
			GNET::SendFacBaseStopRe(0, fid, gmatrix::GetServerIndex(), GetWorldTag());
			Reset();
			break;
		}
		case FBS_FREE:
		{
			GLog::log(GLOG_ERR, "close facbase %d status %d invalid", fid, _status);
			break;
		}
		case FBS_READY:
		{
			_status = FBS_CLOSING;
			_status_timeout = g_timer.get_systime() + CLEAN_TIME;
			KillAllMonsters();
			_need_close_re = true;
			break;
		}
		case FBS_CLOSING:
		{
			GLog::log(GLOG_ERR, "close facbase %d status %d invalid", fid, _status);
			break;
		}
		default:
			break;
	}
}

facbase_auction * facbase_world_manager::GetAuction(int timestamp)
{
	ASSERT(_base_lock);
	AuctionMap::iterator it = auctions.find(timestamp);
	if (it == auctions.end())
		return NULL;
	return &(it->second);
}

void facbase_world_manager::ReturnCoupon(int roleid, int coupon)
{
	ASSERT(_base_lock);
	int & total = auction_failers[roleid];
	total += coupon;;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::fac_coupon_return>::From(h1, total);
	GMSV::SendFacBaseData(0, roleid, 0, h1.data(), h1.size());

	GLog::formatlog("formatlog:return_coupon:roleid=%d:delta=%d:total=%d",
			roleid, coupon, total);
}

void facbase_world_manager::FacBaseBid(int roleid, int timestamp, int itemid, int cost, int name_len, char * player_name)
{
	spin_autolock keeper( _base_lock );
	if (_status != FBS_READY)
		return;
	if (cost <= 0)
		return;
	facbase_auction * auc = GetAuction(timestamp);
	if (auc == NULL)
	{
		GLog::log(GLOG_INFO, "FacBaseBid but timestamp %d not exist, fid %d roleid %d cost %d",
				timestamp, _faction_id, roleid, cost);
		ReturnCoupon(roleid, cost);
		return;
	}
	if (auc->itemid != itemid)
	{
		GLog::log(GLOG_INFO, "FacBaseBid item_id mismatch %d:%d, timestamp %d fid %d roleid %d cost %d",
				auc->itemid, itemid, timestamp, _faction_id, roleid, cost);
		ReturnCoupon(roleid, cost);
		return;
	}
	if (auc->status != facbase_auction::ST_BIDDING)
	{
		GLog::log(GLOG_INFO, "FacBaseBid status %d invalid, timestamp %d fid %d roleid %d cost %d",
				auc->status, timestamp, _faction_id, roleid, cost);
		ReturnCoupon(roleid, cost);
		return;
	}
	if (cost <= auc->cost)
	{
		GLog::log(GLOG_INFO, "FacBaseBid cost %d <= cur_cost %d, timestamp %d fid %d roleid %d cost %d",
				cost, auc->cost, timestamp, _faction_id, roleid, cost);
		ReturnCoupon(roleid, cost);
		return;
	}
	if (auc->winner > 0)
		ReturnCoupon(auc->winner, auc->cost);
	auc->winner = roleid;
	auc->cost = cost;
//	auc->winner_name = std::string(player_name, name_len);
	auc->winner_name = abase::octets(player_name, name_len);
	AuctionUpdateBroadcast(S2C::CMD::facbase_auction_update::REFRESH, *auc);

	GLog::formatlog("formatlog:bid_auction:fid=%d:timestamp=%d:itemid=%d:winner=%d:cost=%d",
			_faction_id, timestamp, itemid, roleid, cost);

	_dirty = true;
}

int facbase_world_manager::WithdrawAuction(int roleid, int item_index, int & itemid)
{
	spin_autolock keeper( _base_lock );
	if (_status != FBS_READY)
		return S2C::ERR_FBASE_STATUS;
	facbase_auction * auc = GetAuction(item_index);
	if (auc == NULL)
		return S2C::ERR_FBASE_AUC_NOT_EXIST;
	if (auc->itemid <= 0)
		return S2C::ERR_FBASE_LOGIC;
	if (auc->status != facbase_auction::ST_END)
		return S2C::ERR_FBASE_LOGIC;
	if (auc->winner != roleid)
		return S2C::ERR_FBASE_AUC_NOT_WINNER;
	itemid = auc->itemid;
	auc->OnDel();
	GLog::formatlog("formatlog:withdraw_auction:fid=%d:timestamp=%d:itemid=%d:winner=%d",
			_faction_id, item_index, itemid, roleid);
	auctions.erase(item_index);

	_dirty = true;
	return 0;
}

int facbase_world_manager::WithdrawCoupon(int roleid, int & coupon)
{
	spin_autolock keeper( _base_lock );
	if (_status != FBS_READY)
		return S2C::ERR_FBASE_STATUS;
	AuctionFailers::iterator it = auction_failers.find(roleid);
	if (it == auction_failers.end() || it->second <= 0)
		return S2C::ERR_FBASE_LOGIC;
	coupon = it->second;
	auction_failers.erase(roleid);

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::fac_coupon_return>::From(h1, 0);
	GMSV::SendFacBaseData(0, roleid, 0, h1.data(), h1.size());

	GLog::formatlog("formatlog:withdraw_coupon:fid=%d:roleid=%d:coupon=%d",
			_faction_id, roleid, coupon);

	_dirty = true;
	return 0;
}

int facbase_world_manager::PutAuction(int itemid, int & time)
{
	ASSERT(_base_lock);
	if (_status != FBS_READY)
		return S2C::ERR_FBASE_STATUS;
	if (auctions.size() >= facbase_auction::MAX_COUNT)
		return S2C::ERR_FBASE_AUC_SIZE;
	const facbase_auction_item_info* auction_item_info = player_template::GetInstance().GetAuctionItemInfoByID(itemid);
	if (!auction_item_info || auction_item_info->aucitontime <= 0)
		return S2C::ERR_FATAL_ERR;
	int new_idx = g_timer.get_systime();
	while (true)
	{
		if (auctions.find(new_idx) == auctions.end())
			break;
		new_idx++;
	}
	facbase_auction new_auc;
	new_auc.pbase = this;
	new_auc.timestamp = new_idx;
	new_auc.itemid = itemid;
	new_auc.cost = auction_item_info->needed_faction_coupon;
	new_auc.status = facbase_auction::ST_BIDDING;
	time = auction_item_info->aucitontime;
	new_auc.end_time = new_auc.timestamp + time;
	auctions[new_idx] = new_auc;
	AuctionUpdateBroadcast(S2C::CMD::facbase_auction_update::ADD, new_auc);
	GLog::formatlog("formatlog:facbase:PutAuction:fid=%d:itemid=%d:idx=%d:time=%d", _faction_id, itemid, new_auc.timestamp, time);
	return 0;
}

void facbase_world_manager::HandleFBaseDeliverCMD(int roleid, const void * buf, size_t size)
{
	//基地gs中很可能没有此 roleid 所以不能直接给 roleid 发消息
	if (_status != FBS_READY)
		return;
	int cmd = ((const C2S::cmd_header*)buf) -> cmd;
	switch (cmd)
	{
		case C2S::POST_FAC_BASE_MSG:
		{
			C2S::CMD::post_fac_base_msg & psm = *(C2S::CMD::post_fac_base_msg *)buf;
			if(size > 4096 || size != sizeof(psm) + sizeof(char)*(size_t)psm.msg_len)
				return;
			UpdateFacBaseMsg(psm.msg, psm.msg_len);
		}
		break;
		case C2S::BID_ON_FACBASE:
		{
			C2S::CMD::bid_on_facbase & bid = *(C2S::CMD::bid_on_facbase*)buf;
			if(size != sizeof(C2S::CMD::bid_on_facbase)+sizeof(char)*(size_t)bid.name_len) 
				return;
			FacBaseBid(roleid, bid.item_index, bid.item_id, bid.coupon, bid.name_len, bid.player_name);
		}
		break;
	}
	return;
}

bool facbase_world_manager::GetAuction(std::vector<S2C::INFO::fac_base_auc_item> & list, std::vector<S2C::INFO::fac_base_auc_history> & history_list)
{
	spin_autolock keeper( _base_lock );
	AuctionMap::iterator ait, aite = auctions.end();
	for (ait = auctions.begin(); ait != aite; ++ait)
	{
		int name_len = std::min(ait->second.winner_name.size(), sizeof(S2C::INFO::fac_base_auc_item().playername));
		S2C::INFO::fac_base_auc_item item = {ait->second.timestamp, ait->second.itemid, ait->second.winner, ait->second.cost, name_len, {}, ait->second.status, ait->second.end_time};
		memcpy(item.playername, (char*)ait->second.winner_name.begin(), name_len);
		list.push_back(item);
	}
	AuctionHistory::iterator hit, hite = auction_history.end();
	for (hit = auction_history.begin(); hit != hite; ++hit)
	{
		int name_len = std::min(hit->rolename.size(), sizeof(S2C::INFO::fac_base_auc_history().playername));
		S2C::INFO::fac_base_auc_history entry = {hit->event_type, name_len, {}, hit->cost, hit->itemid};
		memcpy(entry.playername, (char*)hit->rolename.begin(), name_len);
		history_list.push_back(entry);
	}
	return true;
}

void facbase_world_manager::HandleFBaseClientCMD(int roleid, const void * buf, size_t size, int linkid, int localsid)
{
	//基地gs中很可能没有此 roleid 所以不能直接给 roleid 发消息
	if (_status != FBS_READY)
		return;
	int cmd = ((const C2S::cmd_header*)buf) -> cmd;
	switch (cmd)
	{
		case C2S::GET_FACBASE_AUCTION:
		{
			C2S::CMD::get_facbase_auction & get_auc = *(C2S::CMD::get_facbase_auction *)buf;
			if (size != sizeof(get_auc))
				return;
			using namespace S2C;
			std::vector<INFO::fac_base_auc_item> list;
			std::vector<INFO::fac_base_auc_history> history_list;
			if (!GetAuction(list, history_list))
				return;
			int return_coupon = 0;
			AuctionFailers::iterator it = auction_failers.find(roleid);
			if (it != auction_failers.end())
				return_coupon = it->second;
			packet_wrapper h1(8192);
			CMD::Make<CMD::player_facbase_auction>::From(h1, return_coupon, list, history_list);
			GMSV::SendFacBaseData(linkid, roleid, localsid, h1.data(), h1.size());
		}
		break;
		case C2S::GET_FAC_BASE_INFO:
		{
			C2S::CMD::get_fac_base_info & get_info = *(C2S::CMD::get_fac_base_info *)buf;
			if (size != sizeof(get_info))
				return;
			using namespace S2C;
			CMD::player_fac_base_info base_info;
			std::vector<INFO::player_fac_field> field_info;
			abase::octets msg;
			GetBaseInfo(base_info, field_info, msg);
			packet_wrapper h1(1024);
			CMD::Make<CMD::player_fac_base_info>::From(h1, base_info, field_info, msg.begin(), msg.size());
			GMSV::SendFacBaseData(linkid, roleid, localsid, h1.data(), h1.size());
		}
		break;
	}
	return;
}

void facbase_world_manager::AuctionUpdateBroadcast(int type, const facbase_auction & auc)
{
	ASSERT(_base_lock);
	int name_len = std::min(auc.winner_name.size(), sizeof(S2C::INFO::fac_base_auc_item().playername));
	S2C::INFO::fac_base_auc_item item = {auc.timestamp, auc.itemid, auc.winner, auc.cost, name_len, {}, auc.status, auc.end_time};
	memcpy(item.playername, (char*)auc.winner_name.begin(), name_len);

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::facbase_auction_update>::From(h1, type, item);
	GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
}

bool facbase_world_manager::GetTownPosition(gplayer_imp * pImp, const A3DVECTOR& opos, A3DVECTOR & pos,int & world_tag)
{
	pos = A3DVECTOR(312, 189, 33);
	world_tag = GetWorldTag();
	return true;
}

void facbase_world_manager::AddAucHistory(int event, const abase::octets & name, int cost, int itemid)
{
	ASSERT(_base_lock);
	facbase_auc_history history;
	history.event_type = event;
	history.rolename = name;
	history.cost = cost;
	history.itemid = itemid;
	auction_history.push_back(history);
	if (auction_history.size() > facbase_auc_history::MAX_SIZE)
		auction_history.erase(auction_history.begin());

	int name_len = std::min(history.rolename.size(), sizeof(S2C::INFO::fac_base_auc_history().playername));
	S2C::INFO::fac_base_auc_history entry = {history.event_type, name_len, {}, history.cost, history.itemid};
	memcpy(entry.playername, (char*)history.rolename.begin(), name_len);

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::facbase_auction_add_history>::From(h1, entry);
	GMSV::BroadcastFacBaseData(_faction_id, h1.data(), h1.size());
}

void facbase_world_manager::FacBaseTaskPutAuction(int roleid, int name_len, char * playername, int itemid)
{
	spin_autolock keeper( _base_lock );
	int time = 0;
	int ret = PutAuction(itemid, time);
	if (ret != 0)
		return;
	GMSV::SendFacDynBuyAuction(_faction_id, playername, name_len, itemid, time);
	AddAucHistory(facbase_auc_history::PUT_AUC, abase::octets(playername, name_len), 0, itemid);
}
