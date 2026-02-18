#ifndef __ONLINEGAME_GS_GMATRIX_H__
#define __ONLINEGAME_GS_GMATRIX_H__

#include <lua.hpp>
#include "terrain.h"
#include "template/itemdataman.h"
#include "template/pathman.h"
#include "config.h"
#include "object.h"
#include "player.h"
#include "aiman.h"
#include "objmanager.h"
#include "msgqueue.h"
#include "playermall.h"
#include "staticmap.h"
#include "reborn_skill.h"
#include "raid_controller.h"
#include "facbase_controller.h"
#include "facbase_mall.h"

class world_manager;
class gplayer_imp;
struct gnpc;
struct gmatter;
struct gplayer;

struct world_param
{
public:
	bool forbid_trade;
	bool forbid_faction;
	bool forbid_mail;
	bool forbid_auction;
	bool double_money;
	bool double_drop;
	bool double_sp;
	bool forbid_cash_trade;
	bool anti_wallow;
	bool billing_shop; 	     //是否采用韩国的billing元宝交易系统
	bool pk_forbidden;
	
	int get_multiple_exp()
	{
		return ((multiple_exp >= 2) ? multiple_exp: 0);
	}

	void set_multiple_exp(char multiple)
	{
		multiple_exp = multiple;
	}

	world_param()
	{
		forbid_trade      = false;
		forbid_faction    = false;
		forbid_mail       = false;
		forbid_auction    = false;
		double_money      = false;
		double_drop       = false;
		double_sp         = false;
		forbid_cash_trade = false;
		anti_wallow       = false;
		billing_shop      = false;
		pk_forbidden      = false;
		multiple_exp      = 0;
	}

private:
	char multiple_exp;       //多倍经验
};

struct world_config
{
	float money_drop_rate;			//怪物金钱掉落概率
	int item_money_id;			//金钱物品的ID
	int item_speaker_id;			//千里传音的ID
	int item_speaker_id2;			//千里传音的ID2 
	int item_destroy_id;			//摧毁物品的ID
	int battle_attacker_drop;		//城战攻方掉落表
	int battle_defender_drop;		//城战守方掉落表
	int talisman_reset_item;		//法宝洗点所需物品
	int talisman_combine_fee;		//法宝合成所需费用
	int talisman_enchant_fee;		//法宝灌魔所需费用
	int item_townscroll_id;		//回城卷轴ID
	int id_war_material1;			//城市建设需要物品的id1
	int id_war_material2;			//城市建设需要物品的id2
	int fee_adopt_pet;			//宠物领养费用
	int fee_free_pet;			//宠物放生费用
	int fee_refine_pet;			//宠物修炼费用
	int fee_rename_pet;			//宠物改名费用
	int renew_mount_material;		//骑宠驯化材料
	int element_data_version;		//element data数据版本
	int lock_item_fee;			//锁定物品所需的金钱
	int unlock_item_fee;			//解锁物品所需的金钱
	int unlock_item_id;			//解锁物品所需的道具
	int item_unlocking_id;			//正在解锁的物品ID
	int long_offline_bonus;			//上时间不上线奖励
	int item_broken_id;			//死亡破碎物品
	int item_restore_broken_id;		//恢复破碎的物品的道具
	int item_broken_active;			//破碎物品激活
	int spirit_charge_fee;			//一单位魂力灌注所耗费的金钱
	int item_blood_enchant;			//滴血认主的道具
	int base_spirit_stone_id;		//低级元魂珠的ID
	int adv_spirit_stone_id;		//高级元魂珠的ID
	int gshop_bonus_ratio;			//商城返点比例服务器配置
	int item_change_style;			//改变角色形象需要的物品ID
	int item_arena;				//进入竞技场需要扣除的物品
       	int item_reborn_arena;			//进入飞升竞技场需要扣除的物品	
	int mall_prop;				//商场销售和打折方案,最高8位是打折方案,次高8位是出售方案
	int magic_refine_fee;			//幻灵石是炼化费用
	int item_bid_request;			//唯一价拍卖需要的道具
	int item_bid_reward;			//唯一价拍卖奖励的物品
	int item_zone_cash;			//跨服商城消耗的道具

	int	fee_gem_refine;				// 宝石精炼手续费
	int	fee_gem_extract;			// 宝石萃取手续费
	int	fee_gem_tessellation;		// 宝石镶嵌手续费
	int	fee_gem_dismantle;			// 宝石单次拆除手续费
	int	fee_gem_smelt;				// 宝石熔炼手续费
	int fee_gem_slot_customize;		// 宝石插槽定制手续费
	int	fee_gem_slot_identify;		// 宝石插槽鉴定手续费
	int	fee_gem_slot_rebuild;		// 宝石重铸手续费
	int	gem_upgrade_upper_limit;	// 宝石升品上限
	int	id_gem_smelt_article[3];		// 宝石熔炼道具ID
	int	id_gem_refine_article[3];		// 宝石精炼道具ID
	int	id_gem_slot_lock_article[3];	// 宝石插槽重铸时锁定道具ID
	int	id_gem_slot_rebuild_article[3];	//宝石插槽重铸所需道具ID,有三项，可作普通，非绑定，特殊用途等 Added 2011-05-03
	int	consign_role_type;				//角色寄售所需要使用类别 
	int	consign_role_item_id;		//角色角色需要的物品id	
	int	item_treasure_region_upgrade_id;	
	int	item_speaker2_id[3];		//大喇叭的id
	int	item_speaker3_id[3];		//流言的id
	int	item_change_name_id[3];
	int	item_change_faction_name_id[3];
	int	item_change_family_name_id[3];
	int	item_equip_hole_id[3];
	int	item_astrology_upgrade_id[3];	
	int	fee_astrology_add;
	int	fee_astrology_remove;
	int	fee_astrology_identify;
	int	item_fix_prop_lose;  		// 修复天残号(未满150飞升)所需道具id
	int	 id_produce_jinfashen;
	int	fuwen_fragment_id[2];	//符文碎片id
	int 	fuwen_compose_extra_id[2];		//增加符文合成额外次数id
	int 	fuwen_erase_id[2];	//移除符文id

	world_config(float rate, int id1, int id2, int id3, int id4)
		:money_drop_rate(rate),
		item_money_id(id1),item_speaker_id(id2),item_speaker_id2(id3),item_destroy_id(id4),battle_attacker_drop(-1),battle_defender_drop(-1)
	{
		renew_mount_material = 0;
		element_data_version = 0;
		lock_item_fee = 0;
		unlock_item_fee = 0;
		unlock_item_id = 0;
		item_unlocking_id = 0;
		long_offline_bonus = 0;
		item_broken_id = 0;
		item_restore_broken_id = 0;
		item_broken_active = 0;
		spirit_charge_fee = 0;
		item_blood_enchant = 0;
		base_spirit_stone_id = 0;
		adv_spirit_stone_id = 0;
		gshop_bonus_ratio = 0;
		item_change_style = 0;
		item_arena = 0;
		item_reborn_arena = 0;
		mall_prop = 0 | 0x00010000; 	//默认采用第一种销售方案
		magic_refine_fee = 0;
		item_bid_request = 0;
		item_bid_reward = 0;
		item_zone_cash = 41511;		

		fee_gem_refine = 0;				// 宝石精炼手续费
		fee_gem_extract = 0;			// 宝石萃取手续费
		fee_gem_tessellation = 0;		// 宝石镶嵌手续费
		fee_gem_dismantle = 0;			// 宝石单次拆除手续费
		fee_gem_smelt = 0;				// 宝石熔炼手续费
	 	fee_gem_slot_customize = 0;		// 宝石插槽定制手续费
		fee_gem_slot_identify = 0;		// 宝石插槽鉴定手续费
		fee_gem_slot_rebuild = 0;		// 宝石重铸手续费
		gem_upgrade_upper_limit = 0;	// 宝石升品上限
		memset(id_gem_smelt_article, 0, sizeof(id_gem_smelt_article));
		memset(id_gem_refine_article, 0, sizeof(id_gem_refine_article));
		memset(id_gem_slot_lock_article, 0, sizeof(id_gem_slot_lock_article));
		memset(id_gem_slot_rebuild_article, 0, sizeof(id_gem_slot_lock_article));

		consign_role_type				= 0;
		consign_role_item_id				= 0;
		item_treasure_region_upgrade_id = 0;
		memset(item_speaker2_id, 0, sizeof(item_speaker2_id));
		memset(item_speaker3_id, 0, sizeof(item_speaker3_id));
		memset(item_change_name_id, 0, sizeof(item_change_name_id));
		memset(item_change_faction_name_id, 0, sizeof(item_change_faction_name_id));
		memset(item_change_family_name_id, 0, sizeof(item_change_family_name_id));
		memset(item_equip_hole_id, 0, sizeof(item_equip_hole_id));
		memset(item_astrology_upgrade_id, 0, sizeof(item_astrology_upgrade_id));

		fee_astrology_add = 0;
		fee_astrology_remove = 0;
		fee_astrology_identify = 0;

		item_fix_prop_lose = 0;
		id_produce_jinfashen = 0;
		memset(&fuwen_fragment_id, 0, sizeof(fuwen_fragment_id));
		memset(&fuwen_compose_extra_id, 0, sizeof(fuwen_compose_extra_id));
		memset(&fuwen_erase_id, 0, sizeof(fuwen_erase_id));
	}
};

extern struct world_config g_config;

//双倍打卡时间控制
struct rest_time_ctrl
{
	struct 
	{
		int min;
		int hour;
		int day; 		    //day in week
	}clear_time;
	int first_rest_time;	//sec
	int rest_period;		//rest period, hour
	int rest_time_unit;		//sec
	int max_rest_time;		//sec
};

class mafia_free_battle
{
	struct node_t 
	{
		int attacker;
		int defender;
		int end_time;
	};
	struct hash_node_t
	{
		int enemy;
		int end_time;
	};

	int	_battle_counter;	//上次新添加帮派野战的倒计时 这个值小于0代表最近没有变化
	int	_battle_lock;		//帮派野战操作lock
	int	_closest_end_time;	//最近的结束时间，用于确定何时进行删除队战的操作
	abase::hashtab<hash_node_t, int,abase::_hash_function,abase::fast_alloc<> > _table;//帮派敌对映射
	abase::vector<node_t> _list;	//保存当前有效节点的数组

	inline void UpdateEndTime(int end_time)
	{
		if(_closest_end_time == 0 || _closest_end_time > end_time)
			_closest_end_time = end_time;
	}
	inline void ClearEndTime()
	{
		_closest_end_time = 0;
	}

	inline bool TestEndTime(int t)
	{
		return _closest_end_time == 0 || _closest_end_time <= t;
	}
public:
	mafia_free_battle():_battle_counter(0),_battle_lock(0),_closest_end_time(0),_table(200)
	{
		_list.reserve(100);
	}

	inline int GetBattleCounter() 
	{
		return _battle_counter;
	}

	inline bool GetMafiaBattle(int mafia_id, int * pEnemy, int * pEndTime)
	{
		spin_autolock keeper(_battle_lock);
		hash_node_t * pNode = _table.nGet(mafia_id);
		if(pNode == NULL) return false;
		*pEndTime = pNode->end_time;
		*pEnemy = pNode->enemy;
		return true;
	}

	bool InsertBattle(int attacker, int defender, int end_time);
	void Heartbeat();
};

class faction_hostiles_manager
{
	typedef abase::hash_map<unsigned int, abase::static_set<unsigned int> > FH_MAP;
	FH_MAP _fh_map;
	int _map_lock;
	int _refresh_seq;

public:
	faction_hostiles_manager(): _map_lock(0), _refresh_seq(0)
	{
	}
	bool RefreshHostileInfo( unsigned int faction_id, int& refresh_sep, 
			abase::static_set<unsigned int>& hostiles, bool bForce = false );
	void SyncFactionHostile( unsigned int faction_id, unsigned int hostile );
	void AddFactionHostile( unsigned int faction_id, unsigned int hostile );
	void DelFactionHostile( unsigned int faction_id, unsigned int hostile );
	void Heartbeat();
};

enum
{
	LUA_ENV_LOTTERY = 0, 		//彩票脚本
	LUA_ENV_TALISMAN = 1,  		//法宝脚本
	LUA_ENV_PET_BEDGE = 2, 		//宠物脚本
	LUA_ENV_PLAYER = 3,	   	//玩家操作脚本
	LUA_ENV_SUMMON = 4,		//召唤脚本
	LUA_ENV_MAGIC = 5, 		//幻灵石脚本
	LUA_ENV_TRANSFORM = 6,		//变身脚本
	LUA_ENV_MISC = 7,		//其他一些杂的脚本
	LUA_ENV_RUNE = 8,		//元魂脚本
	LUA_ENV_ASTROLOGY = 9,		//星座脚本

	LUA_ENV_INDEX_MAX
};

class gmatrix
{
public:
	enum 
	{
		MAX_WORLD_TAG = 1024
	};

	enum
	{
		RAID_OR_FBASE_TAG_BEGIN = 5000,  //副本或基地使用的实例起始号
	};

	enum CONSUMPTION_TYPE
	{
		EQUIP_CONSUMPTION	= 1,
		USEITEM_CONSUMPTION = 2,
		SPECIAL_CONSUMPTION = 3,
	};

	struct cash_gift_award
	{
		int gift_id1;
		int gift_id2;
		int award_id;
		int award_timestamp;
	};

	struct facbase_field_info
	{
		int world_tag;
		int max_instance; //基地的最大数量
//		int cur_num; //目前已经激活的数量
		int region_tid; //BUILDING_REGION_CONFIG 的模板 id
		int region_transfer_tid; //FACTION_TRANSFER_CONFIG 的模板 id
		std::string servername;
		facbase_field_info()
		{
			world_tag = 0;
			max_instance = 0;
//			cur_num = 0;
			region_tid = 0;
			region_transfer_tid = 0;
		}
	};

protected:
	static gmatrix _instance;
	static int _server_index;
	static itemdataman _dataman;
	static size_t _player_max_count;
	static size_t _npc_max_count;
	static size_t _matter_max_count;
	static ai_trigger::manager _trigger_man;
	static world_param _world_param;
	static rest_time_ctrl _rest_ctrl;	            //双倍打卡时间的参数控制
	static abase::vector<int> _instance_tag_list;	//包含了所有副本的tag 不包括公共副本
	static reborn_bonus_man _reborn_bonus;
	struct Insertor
	{
		static int push_back(abase::vector<gnpc*> & list, gnpc * pNPC) 
		{ 
			if(pNPC->idle_timer > 0)
			{
				list.push_back(pNPC);
				return 0;
			}
			else
			{
				if((--(pNPC->idle_timer_count)) <= 0)
				{
					pNPC->idle_timer_count = pNPC->npc_idle_hearbeat?pNPC->npc_idle_hearbeat:30;
					list.push_back(pNPC);
				}
				return 1;
			}
		}
		template <typename T>
		static int push_back(abase::vector<T*> &list, T *obj)
		{
			list.push_back(obj);
			return 0;
		}
	};
	typedef abase::hashtab<int,int,abase::_hash_function,abase::fast_alloc<> >  query_map;

	static obj_manager<gmatter,TICK_PER_SEC * MATTER_HEARTBEAT_SEC,Insertor> w_matter_man;
	static obj_manager<gnpc   ,TICK_PER_SEC      ,Insertor> w_npc_man;
	static obj_manager<gplayer,TICK_PER_SEC      ,Insertor> w_player_man;
	static query_map w_player_map;
	static int w_pmap_lock;	

	static world_manager* _worlds[MAX_WORLD_TAG];
	static int w_max_tag;
	static char * _server_name;
	static std::string _restart_shell;
	static std::string _toplist_script;
	typedef abase::hash_map<int , abase::vector<short, abase::fast_alloc<> >, abase::_hash_function, abase::fast_alloc<> > TOPLIST;
	static TOPLIST _toptitle;
	static int  _toptitle_lock;
	static std::set<int> _spawner_ctrl_disable_list;
	static std::set<int> _pocket_item_list;
	static int _is_battle_field_server;
	static int _is_zone_server;
	static int _lua_data_version;
	static int _active_ctrl_lock;
	static std::set<int> _active_ctrl_list;
	static int _gmtoff;

	//每个副本激活的控制器列表
	static int _world_active_ctrl_lock;
	typedef abase::hash_map<int, abase::static_set<int> > WORLD_ACTIVE_CTRL_LIST;
	static WORLD_ACTIVE_CTRL_LIST _world_active_ctrl_list;
	
	//Raid, 最大副本或基地实例TAG, 从w_max_tag + 1 到 w_max_raid_or_fbase_tag为该GS中副本 或基地
	static int w_max_raid_or_fbase_tag;
	static raid_controller w_raid_controller;
	static int _is_raid_server;
	static int _raid_lock;
	//最大基地实例 TAG, 从 w_max_tag + 1 到 w_max_facbase_tag 为该gs种的基地实例，gs
	//static int w_max_facbase_tag;
	struct territory_info
	{
		territory_info(){}
		territory_info(int type, unsigned int id) : territory_type(type), owner(id){} 
		int territory_type;
		unsigned int owner;
	};

	
	static int _territory_lock;
	static std::map<int, territory_info> _territory_owner_list;
	static std::map<int, int> _consign_item_list; //寄售物品列表<item_id, item_type>
	static std::map<int, int> _order_item_list; //物品整理的顺序id列表

	//消费值对照表
	typedef std::map<int, int> ConsumptionValueConfigMap;
	static ConsumptionValueConfigMap _equip_consumption_map; 
	static ConsumptionValueConfigMap _useitem_consumption_map;
	static ConsumptionValueConfigMap _special_consumption_map;

	static bool ReadConsumpConfigfile(const char * filename, const std::string & keyword, ConsumptionValueConfigMap & options);
	static std::string SimpleTrim(const std::string & str);

	static int lottery_give_item_prob;
	static int lottery_ty_exp_probs[5];

	struct activity_info
	{
		int map_id;
		float pos_x;
		float pos_y;
		float pos_z;
	};

	static std::map<int, activity_info> _activity_info_list;

	static std::map<int, facbase_mall*> _facbase_mall_list;  // Youshuang add
	static int _facbase_mall_lock;	// Youshuang add
	
	//激活的事件列表
	static std::set<int> _active_event_list;
	static int _event_lock;

	MsgQueueList  _msg_queue; 
	MsgQueueList2 _msg_queue2; 
	mafia_free_battle _battle_ctrl;
	faction_hostiles_manager _fh_man;
	int _heartbeat_lock;
	
	abase::vector<lua_State *> _global_lua_state;
	abase::vector<int>	   _global_lua_lock;
	void AddGlobalLua(size_t index,  lua_State * state)
	{
		if(index >= _global_lua_state.size())
		{
			for(size_t i = _global_lua_state.size(); i <= index; i ++)
			{
				_global_lua_state.push_back(NULL);
				_global_lua_lock.push_back(0);
			}
		}
		ASSERT(_global_lua_state[index] == NULL);
		_global_lua_state[index] = state;
	}
	
	struct cash_gift_info
	{
		int cur_gift_id1;		//1个商城积分对应的物品id
		int cur_gift_id2;		//10000个商城积分对应的物品id

		std::vector<cash_gift_award> gift_award_list; //保留每一期积分的兑换信息
		cash_gift_info()
		{
			cur_gift_id1 = 0;
			cur_gift_id2 = 0;
		}
	};
	
	static cash_gift_info _cash_gift_info;

	struct kingdom_info
	{
		int kingdom_lock;
		int mafia_id;	
		int point;
		int kingdom_task_type;
		std::map<int, int> kingdom_title;	// id <--> title 
		bool is_dirty;
		bool is_bath_open;
		int bath_count_lock;
		std::map<int, int> bath_count_map;	// id <--> bath_count

		kingdom_info()
		{
			kingdom_lock = 0;
			point = 0;
			mafia_id = 0;
			is_dirty = 0;
			is_bath_open = false;
			bath_count_lock = 0;
			kingdom_task_type = 0;
		}
	};
	static kingdom_info _kingdom_info;

	struct fuwen_compose
	{
		int fuwen_id;
		float prob;
	};
	static abase::vector<fuwen_compose> _fuwen_compose_list;


	struct gt_buff_id
	{
		int skill_id;
		int buff_id;

		gt_buff_id()
		{
			skill_id = 0;
			buff_id = 0;
		}
	};
	static gt_buff_id _gt_buff_id;
	

	static int _is_flow_battle_server;

	static bool _disable_cash_gift;			//关闭商城购买积分反赠功能和log打折功能(海外版本)
	static facbase_field_info _facbase_info;
	static facbase_controller w_fbase_controller;

protected:
	void InsertWorldManager(int tag, world_manager * manager)
	{
		_worlds[tag] = manager;
		if(tag > w_max_tag) w_max_tag = tag;
	}

	void InsertRaidOrFBaseManager(world_manager * manager)
	{
		_worlds[++ w_max_raid_or_fbase_tag] = manager;
	}
	/*
	void InsertFacBaseManager(world_manager * manager)
	{
		_worlds[++ w_max_facbase_tag] = manager;
	}
	*/
	static gobject * locate_object_from_msg(const MSG & msg);
	static bool InitDisableSpawnerCtrl(const char * str);
	static bool InitPocketItemList(const char * filename);
 
	bool InitScriptSystem(const char * filename, const int * list, size_t count);
	bool ClearScriptDoFile(const int * list, size_t count);
	void RegisterObjectScript(int env);
	void RegisterMiscScript();

	lua_State * AttachScript(size_t index);
	void DetachScript(size_t index);
	static bool LoadLuaVersion(const char * file);
	static bool InitWallowParam();

public:
	static void SendPlaneMessage(world * plane, const MSG & msg);
	static void SendPlaneMessage(world * plane, const MSG & msg, int latancy);
	static void SendWorldMessage(int tag , const MSG & msg);

	static void SendMessage(const MSG & msg);
	static void SendMessage(const MSG & msg, int latancy);
	static void SendMessage(const XID * first, const XID * last, const MSG & msg);
	static void SendPlayerMessage(int * player_list, size_t count, const MSG & msg);
	static void SendMultiMessage(abase::vector<gobject*,abase::fast_alloc<> > &list, const MSG & msg);
	static void SendMultiMessage(abase::vector<gobject*,abase::fast_alloc<> > &list, const MSG & msg,size_t limit);
	static void SendAllMessage(const MSG & msg);

	static void DispatchMessage(const MSG &msg);			//此函数只应由msgqueue调用
	static void DispatchMessage(gobject * obj, const MSG & msg);	//此函数只应由msgqueue调用
	static void DispatchWorldMessage(int tag, const MSG & msg);	//此函数只应由msgqueue调用

	class ScriptKeeper 
	{
		gmatrix & _instance;
		size_t _index;
		int _stack_index;
		lua_State * _L;
		public:
		ScriptKeeper(gmatrix & instance,size_t index):_instance(instance),_index(index) 
		{ 
			_L = instance.AttachScript(index); 
			_stack_index = lua_gettop(_L);
		}
		~ScriptKeeper() 
		{
			lua_settop(_L,_stack_index);
			_instance.DetachScript(_index); 
		}
		lua_State * GetState() { return _L;}
	};
public:
	static gmatrix *Instance() { return &_instance; }
	gmatrix();
	virtual ~gmatrix() {}
	int Init(const char * gmconf_file,char ** world_name, char ** ins_name, char ** zone_name );

	virtual void Heartbeat();	//一秒20次的心跳
	void ShutDown();
	void WriteAllUserBack();
	
	inline mafia_free_battle & GetBattleCtrl() { return _battle_ctrl;}
	inline faction_hostiles_manager& GetFHManager() { return _fh_man; }

public:
	static void TriggerSpawn(int ctrl_id, bool active);
	static void TriggerSpawn(int ctrl_id, bool active, int world_tag);
	static void BroadcastAllNPCMessage(MSG & msg);
	static void CheckCtrlID(int ctrl_id, bool active);
	//副本用单独世界激活的控制器列表
	static void CheckCtrlID(int world_tag, int ctrl_id, bool active);
	//副本关闭时清除相应世界激活的控制器列表
	static void CleanWorldActiveCtrlList(int world_tag);
	static itemdataman & GetDataMan() { return _dataman;}
	static world_manager*  FindWorld(int tag);
	static size_t GetMaxPlayerCount() { return _player_max_count;}
	static size_t GetMaxNPCCount() { return _npc_max_count;}
	static size_t GetMaxMatterCount() { return _matter_max_count;}
	static ai_trigger::manager & GetTriggerMan() { return _trigger_man;}
	static world_param & GetWorldParam() { return _world_param;}
	static int GetServerIndex() { return _server_index;}
	static bool InitQuestSystem(const char * path,const char *path2, const char * path3, const char * path4);
	static bool InitNPCTemplate();
	static bool InitTagList();
	static bool InitNetClient(const char * gmconf);
	static abase::vector<int>& GetInstanceTags() { return _instance_tag_list; }
	static const char * GetTopListScript() { return _toplist_script.c_str();}
	static int GetLuaVersion() { return _lua_data_version;}
	static reborn_bonus_man & GetRebornBonusMan() { return _reborn_bonus;}
	static bool IsControllerDisable(int id)
	{
		return _spawner_ctrl_disable_list.find(id) != _spawner_ctrl_disable_list.end();
	}
	static bool IsPocketItem(int id)
	{
		return _pocket_item_list.find(id) != _pocket_item_list.end();
	}
	static bool IsActiveCtrl(int id)
	{
		spin_autolock keeper(_active_ctrl_lock);
		return _active_ctrl_list.find(id) != _active_ctrl_list.end();
	}
	static bool IsActiveCtrl(int world_tag, int id)
	{
		spin_autolock keeper(_world_active_ctrl_lock);
		return _world_active_ctrl_list[world_tag].find(id) != _world_active_ctrl_list[world_tag].end();
	}
	static void SyncTerritoryOwner(int territoryid, int territory_type, unsigned int owner)
	{
		spin_autolock alock(_territory_lock);
		territory_info info(territory_type, owner);
		_territory_owner_list[territoryid] = info;
	}
	static int GetTerritoryCount(unsigned int mafia_id, int territory_type)
	{
		spin_autolock alock(_territory_lock);
		int cnt = 0;

		std::map<int, territory_info>::iterator iter;
		for(iter = _territory_owner_list.begin(); iter != _territory_owner_list.end(); ++iter)
		{
			if(iter->second.territory_type == territory_type && iter->second.owner == mafia_id) ++cnt;
		}
		return cnt;
	}

	static int GetTerritoryScore(unsigned int mafia_id)
	{
		if(mafia_id == 0) return 0;
		spin_autolock alock(_territory_lock);
		
		int score = 0;
		std::map<int, territory_info>::iterator iter;
		for(iter = _territory_owner_list.begin(); iter != _territory_owner_list.end(); ++iter)
		{
			if(iter->second.owner == mafia_id)
			{
				if(iter->second.territory_type == 3) score +=10;
				else if(iter->second.territory_type == 2) score +=25;
				else if(iter->second.territory_type == 1) score +=50;
			}
		}
		return score;
	}

	static bool IsTerritoryOwner(unsigned int mafia_id, int territory_id)
	{
		spin_autolock alock(_territory_lock);
		std::map<int, territory_info>::iterator iter;
		if((iter = _territory_owner_list.find(territory_id)) != _territory_owner_list.end())
		{
			if(iter->second.owner == mafia_id && mafia_id != 0) return true;
		}
		return false;
	}

	static bool GetActivityInfo(int id, int & tag, A3DVECTOR & pos)
	{
		std::map<int, activity_info>::iterator iter;
		if( (iter = _activity_info_list.find(id)) != _activity_info_list.end())
		{
			tag = iter->second.map_id;
			pos.x = iter->second.pos_x;
			pos.y = iter->second.pos_y;
			pos.z = iter->second.pos_z;
			return true;
		}
		return false;
	}	
		
	static void RestartProcess();

	static bool AntiWallow()
	{
		return _world_param.anti_wallow;
	}
	
	static gnpc 	*AllocNPC()
	{
		return w_npc_man.Alloc();
	}
	static void 	FreeNPC(gnpc* pNPC)
	{
		w_npc_man.Free(pNPC);
	}

	static gmatter *AllocMatter()
	{
		return w_matter_man.Alloc();
	}
	static void 	FreeMatter(gmatter *pMatter)
	{
		return w_matter_man.Free(pMatter);
	}

	static gplayer *AllocPlayer()
	{
		return w_player_man.Alloc();
	}
	static void 	FreePlayer(gplayer * pPlayer)
	{
		return w_player_man.Free(pPlayer);
	}

	static int GetPlayerAlloced()
	{
		return w_player_man.GetAllocedCount();
	}
	
	static int GetNPCAlloced()
	{
		return w_npc_man.GetAllocedCount();
	}
	
	static void InsertPlayerToMan(gplayer *pPlayer) 
	{ 
		w_player_man.Insert(pPlayer);
	}
	static void RemovePlayerToMan(gplayer *pPlayer) 
	{ 	
		w_player_man.Remove(pPlayer);
	}

	static void InsertNPCToMan(gnpc * pNPC)
	{
		w_npc_man.Insert(pNPC);
	}

	static void RemoveNPCFromMan(gnpc * pNPC)
	{
		w_npc_man.Remove(pNPC);
	}

	static void InsertMatterToMan(gmatter * pMatter)
	{
		w_matter_man.Insert(pMatter);
	}

	static void RemoveMatterFromMan(gmatter * pMatter)
	{
		w_matter_man.Remove(pMatter);
	}

	static inline bool MapPlayer(int uid,int index) 
	{ 
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.put(uid,index);
	}
	
	static inline int UnmapPlayer(int uid) 
	{
		spin_autolock alock(w_pmap_lock);
		return w_player_map.erase(uid);
	}
	
	static inline int FindPlayer(int uid) 
	{
		spin_autolock alock(w_pmap_lock);
		query_map::pair_type p = w_player_map.get(uid);
		if(!p.second) return -1;
		return *p.first;
	}

	static gplayer* FindPlayer(int uid, int & world_index)
	{
		int index = FindPlayer(uid);
		if(index < 0) return NULL;
		world_index = 0;
		return w_player_man.GetByIndex(index);
	}

	static bool IsPlayerExist(int uid)
	{
		spin_autolock alock(w_pmap_lock);
		query_map::pair_type p = w_player_map.get(uid);
		if(!p.second) return false;
		return true;
	}

	static void UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size, char flag);

	static std::map<int, int> & GetOrderListMap() { return _order_item_list; }
	static gmatter * GetMatterPool() { return w_matter_man.GetPool();}
	static gplayer*  GetPlayerPool() { return w_player_man.GetPool();}
	static gnpc* 	 GetNPCPool() { return w_npc_man.GetPool();}
	static gmatter * GetMatterByIndex(size_t index) { return w_matter_man.GetByIndex(index);}
	static gplayer*  GetPlayerByIndex(size_t index) {return w_player_man.GetByIndex(index);}
	static gnpc* 	 GetNPCByIndex(size_t index) { return w_npc_man.GetByIndex(index);}
	static size_t GetPlayerIndex(gplayer *pPlayer)  { return w_player_man.GetIndex(pPlayer);}
	static size_t GetMatterIndex(gmatter *pMatter)  { return w_matter_man.GetIndex(pMatter);}
	static size_t GetNPCIndex(gnpc *pNPC)  { return w_npc_man.GetIndex(pNPC);}
	static bool CheckPlayerDropCondition()
	{
		size_t cap = w_matter_man.GetCapacity();
		size_t count = w_matter_man.GetAllocedCount();
		return (cap - count) >= (cap >> 2);
	}
	static void InitTopTitle(std::map<int , std::vector<int> > & list);
	static int GetTopTitle(int userid, gplayer_imp * pImp); 
	static int IsBattleFieldServer() { return _is_battle_field_server; }
	static int IsZoneServer() {return _is_zone_server;}
	static int GetRankLevel(int index);
	static int GetServerGMTOff(){ return _gmtoff;}

	static bool InitConsignItemList(const char* filepath);

	/**
	  *@Brief:add by yesonggao
	  *
	  *快速整理包裹
	  *物品整理的顺序id
	  *
	  */
	static bool InitOrderItemList(const char* filepath);

	/**
	 *@Brief: 	Add By Houjun 2010-08-11, 
	 *@Return: 	物品存在, 返回>0的物品ID; 物品不存在, 返回-1;
	 */
	static int  GetConsignItemType(int item_id);

	static bool InitConsumptionValueList(const char * filename);
	static int  GetConsumptionValue(CONSUMPTION_TYPE type, int item_type);

	static int script_AddActivityInfo(lua_State *L);

	static int CreateRaidWorldManager(int mapid, int raidroom_id, const std::vector<int>& roleid_list, int& world_tag, const void* buf, size_t size, char difficulty);
	static int ReleaseRaidWorldManager(int tag);
	static int WorldManagerOnDisconnect();
	
	static int CloseAllFlowBattleManager();
	
	static void GetRaidWorldInfos(std::vector<raid_world_info>& infos);
	static raid_world_template* GetRaidWorldTemplate(int raid_id);
	static void IncRaidWorldInstance(int raid_id);
	static void DecRaidWorldInstance(int raid_id);

	static bool IsRaidServer() { return _is_raid_server; }

	static void StartEvent(int event_id);
	static void StopEvent(int event_id);
	static void GetEventList(std::set<int> & event_list);

	static bool InitLotteryGiveItemProb(const char* filepath);
	static int GetLotteryGiveItemProb() { return lottery_give_item_prob; }
	static int GetLotteryTyExpProb(int idx)
	{
		return lottery_ty_exp_probs[idx];
	}

	static bool InitCashGiftInfo(const char * filepath);
	static bool CheckGiftItem(std::set<int> & item_list, int item_id);
	static int GetCurCashGiftID1(){ return _cash_gift_info.cur_gift_id1;}
	static int GetCurCashGiftID2() {return _cash_gift_info.cur_gift_id2;}
	static std::vector<cash_gift_award> & GetCashGiftAwardList() { return _cash_gift_info.gift_award_list;}

	static int GetKingdomMafiaID() { return _kingdom_info.mafia_id;}
	static int GetKingdomPoint() { return _kingdom_info.point;}
	static int GetKingdomTaskType(){return _kingdom_info.kingdom_task_type;}
	static void ConsumeKingdomPoint(int point);
	static void ObtainKingdomPoint(int point);
	static void SyncKingdomPoint(int point);
	static void SyncKingdomInfo(int mafia_id, int kingdom_point, std::map<int, int> & kingdom_title, int task_type);
	static int GetKingdomTitle(int roleid);
	static bool IsBathOpen();
	static void OpenBath();
	static void CloseBath();
	static void BathClosed();
	static void SyncBathCount(int id, int count);
	static void ClearBathCount();
	static int GetBathCount(int id);

	static bool InitFuwenComposeInfo(const char * filepath);
	static int GetFuwenComposeID();

	static bool InitGTBuffInfo(const char * filepath);
	static int GetGTSkillID() { return _gt_buff_id.skill_id;} 
	static int GetGTBuffID() { return _gt_buff_id.buff_id;} 

	static bool IsDisableCashGift(){return _disable_cash_gift;}
	static facbase_field_info & GetFacBaseInfo() { return _facbase_info; }
	static int CreateFacBaseManager(int fid);
	static facbase_controller & GetFBaseCtrl() { return w_fbase_controller; }
	// Youshuang add
	static facbase_mall* GetFacbaseMall( int fid );
	static void AddFacbaseMall( int fid, facbase_mall* new_mall );
	static void ModFacbaseMall( int fid, const std::set<int>& actived_items );
	static int* GetFacbaseLock() { return &_facbase_mall_lock; }
	// end
};

#endif

