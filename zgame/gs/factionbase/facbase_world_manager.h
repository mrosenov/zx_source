#ifndef __ONLINEGAME_GS_FACBASE_WORLD_MANAGER_H__ 
#define __ONLINEGAME_GS_FACBASE_WORLD_MANAGER_H__

#include "../global_manager.h"
#include "../world.h"
#include "../usermsg.h"
#include <vector>
#include <octets.h>

struct facbase_building
{
	enum
	{
		IN_BUILDING = 0,
		BUILDING_COMPLETE,
	};
	int tid;
	int level;
	int status;
	int task_id;
	int task_count;
	int task_need;
	facbase_building ()
	{
		tid = 0;
		level = 0;
		status = IN_BUILDING;
		task_id = 0;
		task_count = 0;
		task_need = 0;
	}
};

class facbase_world_manager;
struct facbase_auction
{
	enum
	{
		ST_NILL = 0, //初始状态
		ST_BIDDING,  //开放竞拍状态
		ST_END,	     //竞拍结束
	};
	enum
	{
		END_TIME = 259200, //拍卖停止之后的领取时间
		MAX_COUNT = 100, //最多 100条拍卖
	};
	facbase_world_manager * pbase;
	int timestamp;
	int itemid;
	int winner;
	int cost;
	abase::octets winner_name;
	int status;
	int end_time;
	facbase_auction()
	{
		pbase = NULL;
		timestamp = 0;
		itemid = 0;
		winner = 0;
		cost = 0;
		status = ST_NILL;
		end_time = 0;
	}
	bool Update(int now, bool & changed); //返回是否需要删除
	void OnEnd();
	void OnDel();
};

struct facbase_auc_history
{
	enum
	{
		MAX_SIZE = 30,
	};
	enum
	{
		PUT_AUC = 1, //上架
		WIN_AUC,     //获胜
		AUC_TIMEOUT, //流拍
	};
	int event_type;
	abase::octets rolename;
	int cost;
	int itemid;
	facbase_auc_history()
	{
		event_type = 0;
		cost = 0;
		itemid = 0;
	}
};
class facbase_world_manager;
class fac_building_event
{
public:
	facbase_world_manager * mgr;
	int field_index; //
	int building_lev;
	int time_period;

	fac_building_event(facbase_world_manager * _mgr, int index, int lev, int time) : mgr(_mgr), field_index(index), building_lev(lev), time_period(time) {}
	virtual ~fac_building_event() {}
	virtual void OnEvent() = 0;
	virtual void Destroy() { delete this; }
};

class fac_prop_add_event : public fac_building_event
{
public:
	int prop_type;
	int value;
	fac_prop_add_event(facbase_world_manager * mgr, int in, int lev, int period, int type, int v)
	: fac_building_event(mgr, in, lev, period), prop_type(type), value(v) {}
	virtual void OnEvent();
};

class fac_item_add_event : public fac_building_event
{
public:
	int item_id;
	int count;
	fac_item_add_event(facbase_world_manager * mgr, int in, int lev, int period, int item, int cn)
	: fac_building_event(mgr, in, lev, period), item_id(item), count(cn) {}
	virtual void OnEvent();
};

class facbase_world_manager: public global_world_manager
{
protected:
//存db属性
	int _faction_id;		// 所属的帮派 id
	int _grass; //灵草
	int _mine; //矿石
	int _monster_food; //神兽口粮
	int _monster_core; //妖核
	int _cash; //帮派元宝
	int _db_timestamp; //存盘记 数
	typedef abase::static_map<int/*index*/, facbase_building, facbase_controller::FIELD_CAPACITY> FieldMap;
	FieldMap fields;
	abase::octets _msg;//基地留言板
	typedef abase::hash_map<int/*timestamp*/, facbase_auction> AuctionMap;
	AuctionMap auctions;
	typedef abase::hash_map<int/*roleid*/, int/*return_coupon*/> AuctionFailers;
	AuctionFailers auction_failers;
	typedef abase::vector<facbase_auc_history> AuctionHistory;
	AuctionHistory auction_history;
//内存属性
	int _status;
	int _status_timeout;
	int _base_lock;		
	int _building_index;		//正在建设中的地块 index; 从 1 开始编号
	int _building_taskid;

	unsigned int _heartbeat_counter; 
	bool _dirty;
	bool _need_close_re; //关闭时是否需要给deliveryd返回 re

	enum
	{
		AWARD_AUCTION_ITEM	= 1,
		MULTIPLE_EXPERIENCE	= 2,
		ENABLE_CONTROLLER	= 3,
	};
	std::map<int, int> _cash_items_cooldown_info;  // item index to cooldown time
	
	typedef std::multimap<int/*time_t*/, fac_building_event *> EventMap;
	EventMap event_map;
	typedef std::set<int/*item_index*/> MallOpenIndexes; //帮派商城已经开启的商品 index 列表
	MallOpenIndexes _mall_indexes;
	cs_user_map  _all_list;

	enum
	{
		FBS_FREE		= 0,	// 世界管理器处于空闲状态，可被新的副本实例所使用
		FBS_GETTINGDATA		= 1,	// 正在从 db 读取基地数据
		FBS_READY 		= 2,	// 服务器准备完毕
		FBS_CLOSING		= 3,	// 世界正在关闭
	};

	enum
	{
		CLEAN_TIME = 35, //清除时间 包括踢玩家 NPC 存DB 等
	};
	inline void AddMapNode( cs_user_map& map, gplayer* pPlayer )
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val( pPlayer->ID.id, pPlayer->cs_sid );
		if( cs_index >= 0 && val.first >= 0 )
		{
			map[cs_index].push_back( val );
		}
	}
	inline void DelMapNode( cs_user_map& map, gplayer* pPlayer )
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val( pPlayer->ID.id, pPlayer->cs_sid );
		if( cs_index >= 0 && val.first >= 0 )
		{
			cs_user_list& list = map[cs_index];
			int id =  pPlayer->ID.id;
			size_t i = 0;
			for( i = 0; i < list.size(); ++i )
			{
				if( id == list[i].first )
				{
					list.erase( list.begin() + i );
					i --;
				}
			}
		}
	}
public:
	int GetBaseId() { return _faction_id; }
	int TestAllocBase(int tag, int fid);
	void OnGetFacBaseFail();
	void OnGetFacBase(int fid, int tag, const GDB::fac_base_info * pinfo);
	int AddFacBuilding(int index, int tid, XID roleid, int name_len, char * playername);
	int UpgradeFacBuilding(int index, int tid, XID roleid, int lev, int name_len, char * playername);
	int RemoveFacBuilding(int index, int tid);
	facbase_building * GetField(int index);
	bool LoadData(const GDB::fac_base_info * info);
	bool CanSave() { return _status == FBS_READY || _status == FBS_CLOSING; }
	void SaveData(GDB::FBaseResult * callback);
	void AutoSave();
	void GetBaseInfo(S2C::CMD::player_fac_base_info & base_info, std::vector<S2C::INFO::player_fac_field> & field_info, abase::octets & msg);
	int GetBuildingLevel(int iBuildId);
	void ChangeProp(int noti_foleidd, int type, int delta);
	int GetGrass()
	{ 
		spin_autolock keeper( _base_lock );
		return _grass;
	}
	void ChangeGrass(int roleid, int num)
	{
		spin_autolock keeper( _base_lock );
		ChangeProp(roleid, FBASE_PROP_GRASS, num);
	}
	int GetMine()
	{
		spin_autolock keeper( _base_lock );
		return _mine;
	}
	void ChangeMine(int roleid, int num)
	{
		spin_autolock keeper( _base_lock );
		ChangeProp(roleid, FBASE_PROP_MINE, num);
	}
	int GetMonsterFood()
	{
		spin_autolock keeper( _base_lock );
		return _monster_food;
	}
	void ChangeMonsterFood(int roleid, int num)
	{
		spin_autolock keeper( _base_lock );
		ChangeProp(roleid, FBASE_PROP_MONSTERFOOD, num);
	}
	int GetMonsterCore()
	{
		spin_autolock keeper( _base_lock );
		return _monster_core;
	}
	void ChangeMonsterCore(int roleid, int num)
	{
		spin_autolock keeper( _base_lock );
		ChangeProp(roleid, FBASE_PROP_MONSTERCORE, num);
	}
	int GetCash()
	{
		spin_autolock keeper( _base_lock );
		return _cash;
	}
	void ChangeCash(int roleid, int num)
	{
		spin_autolock keeper( _base_lock );
		ChangeProp(roleid, FBASE_PROP_CASH, num);
	}
	void AddEvent(int time, fac_building_event * eve);
	void OnPropAddEvent(int type, int value);
	void OnItemAddEvent(int item_id, int count);
	void PlayerEnterBase(gplayer * pPlayer);
	void PlayerLeaveBase(gplayer * pPlayer);
	//建筑升级之后引起的属性变化 事件触发
	void OnBuildingUpgrade(const BUILDING_ESSENCE * building_cfg, int index, int old_lev);
	void NotifyPropChange(int noti_roleid); //通知基地内的成员基地属性变化
	std::map<int, int> GetCashItemsCooldownInfo();
	int BuyCashItem(int roleid, int item_idx, int name_len, char * player_name);
	void NotifyCashItemCooldown();
	int TestCashItemCoolDown( const facbase_cash_item_info* iteminfo, int item_idx, int& result_time );
	bool IsClosing() { return _status == FBS_CLOSING; }
	void KillAllMonsters();
	void Reset();
	void Release();
	facbase_auction * GetAuction(int timestamp);
	void ReturnCoupon(int roleid, int coupon);
	int WithdrawAuction(int roleid, int item_index, int & itemid); //领取竞拍成功奖品
	int WithdrawCoupon(int roleidd, int & coupon); //领取竞拍失败返还的帮派金券
	int PutAuction(int itemid, int & time);
	void UpdateFacBaseMsg(char * msg, size_t len);
	void FacBaseBid(int roleid, int timestamp, int itemid, int cost, int name_len, char * player_name);
	bool GetAuction(std::vector<S2C::INFO::fac_base_auc_item> & list, std::vector<S2C::INFO::fac_base_auc_history> & history_list);
	void AuctionUpdateBroadcast(int type, const facbase_auction & auc);
	int GetFactionId() { return _faction_id; }
	void AddAucHistory(int event, const abase::octets & name, int cost, int itemid);
	int IncreaseDBTimeStamp()
	{
		_db_timestamp++;
		return _db_timestamp;
	}

	facbase_world_manager();
	virtual ~facbase_world_manager();

	virtual bool IsIdle() const { return _status != FBS_READY; } 
	virtual bool CanEnterWorld() const { return _status == FBS_READY; }
	virtual bool IsFacBase() { return true; }
	virtual int GetClientTag();
	virtual bool SaveCtrlSpecial() { return true; } //是否为不同实例单独保存控制器状态 副本和帮派基地返回 true
	virtual bool IsFree() { return _status == FBS_FREE; }
	virtual void OnActiveSpawn(int id,bool active);

	virtual INIT_RES OnInitMoveMap();
	virtual bool OnReleaseMoveMap() { return true; }
	virtual INIT_RES OnInitCityRegion();
	virtual bool OnReleaseCityRegion() { return true; }
	virtual INIT_RES OnInitBuffArea();
	virtual bool OnReleaseBuffArea() { return true; }
	virtual INIT_RES OnInitPathMan();
	virtual bool OnReleasePathMan() { return true; }

	virtual bool OnTraceManAttach();

	virtual void Heartbeat();
	virtual void OnFacBasePropChange(int noti_roleid, int fid, int type, int delta);
	virtual void OnFacBaseBuildingProgress(int fid, int task_id, int value);
	virtual void OnDisconnect();
	virtual void HandleFBaseDeliverCMD(int roleid, const void * buf, size_t size);
	virtual void HandleFBaseClientCMD(int roleid, const void * buf, size_t size, int linkid, int localsid);
	virtual bool GetTownPosition(gplayer_imp * pImp, const A3DVECTOR& opos, A3DVECTOR & pos,int & world_tag);
	virtual int GetInBuildingLevel(); // 1 级升 2 级 返回 2
	virtual void FacBaseTaskPutAuction(int roleid, int name_len, char * playername, int itemid);
	virtual void OnFacBaseStop(int fid);
};

#endif

