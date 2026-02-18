#ifndef __GAMEDB_IF_H__
#define __GAMEDB_IF_H__

#define DBMASK_PUT_STOREHOUSE     0x00000001
#define DBMASK_PUT_INVENTORY      0x00000002
#define DBMASK_PUT_TASK           0x00000004
#define DBMASK_PUT_CASH           0x00000008
#define DBMASK_PUT_ACHIEVEMENT	  0x00000010
#define DBMASK_PUT_ALL            0x0000001F
#define DBMASK_PUT_SYNC           (DBMASK_PUT_INVENTORY|DBMASK_PUT_STOREHOUSE|DBMASK_PUT_CASH)
#define DBMASK_PUT_SYNC_TIMEOUT   (DBMASK_PUT_ALL^DBMASK_PUT_INVENTORY)

#include <vector>
#include <string>

namespace GNET
{
	class GRoleDetail;
	class GRoleInventory;
	template <class T> class RpcDataVector;
	class GFactionBase;
};

namespace GDB
{
struct base_info
{
	unsigned int 	id;
	unsigned int    userid;
	int 		cls;
	bool 		gender;
	bool		trashbox_active;
	bool		cash_active;
	short		cur_title;
	short 		level;
	int64_t		exp;
	int		pp;
	int 		hp;
	int		mp;
	float 		posx;
	float 		posy;
	float 		posz;
	int		worldtag;
	int		money;
	unsigned int 	factionid;
	int 		factionrole;
	int 		reputation;
	int		time_used;      
	size_t		trash_money;
	int		create_time;
	short		storesize;
	short		storesize2;
	short		bagsize;
	short 		pocketsize;
	short 		mountwingsize;
	int		timestamp;
	int		pkvalue;
	int		produceskill;
	int		produceexp;
	int		cash;
	int		cash_add2;
	int		cash_total;
	int		cash_used;
	int		cash_delta;
	int		cash_serial;
	unsigned int	contribution;
	unsigned int	spouse;
	int		combatkills;
	int		jointime;
	int		familyid;
	int		sectid;
	short		initiallevel;
	unsigned int	devotion;
	unsigned int	talismanscore;
	unsigned int	battlescore;
	int		loginip;
	short		cultivation;
	unsigned int	updatetime;
	int 		bonus_withdraw;
	int 		bonus_reward;
	int 		bonus_used;
	int		referrer;
	unsigned int 	circleid;
	unsigned char 	circletitlemask;
	int 		src_zoneid;
	unsigned short 	deity_level;
	int64_t 	deity_exp;
	int 		dp;
	unsigned char 	flag_mask; //布尔型标记扩充使用此字段
	int 		runescore;
	int64_t		comsumption;
	int64_t		fac_coupon_add; //帮派金券累计值 		
	int		fac_coupon; //帮派金券
	short		used_title;
};

struct ivec
{
	const void * data;
	unsigned int size;
};

struct itemdata
{
	unsigned int id;
	int index;
	int count;
	short client_size;
	short max_count;
	int guid1;
	int guid2;
	int proctype;
	int expire_date;
	const void * data;
	unsigned int size;
};

struct  itemlist
{
	itemdata * list;
	size_t count;
};

struct pocket_item
{
	unsigned int id;
	short index;
	short count;
};

struct pocket_itemlist
{
	pocket_item * list;
	size_t count;
};

struct shoplog
{
	int order_id;
	int item_id;
	int expire;
	int item_count;
	int order_count;
	int cash_need;
	int time;
	int guid1;
	int guid2;
};

struct  loglist
{
	shoplog * list;
	size_t count;
};

struct webmallgoods
{
	int id;
	int count;
	int flagmask;
	int timelimit;
};
struct webmallfunc //礼包结构
{
	int id;
	std::string name;//礼包名称
	int count;
	int price;

	std::vector<webmallgoods> goods;
};
struct weborder //订单结构
{
	int64_t orderid;
	int userid;
	int roleid;
	int paytype;
	int status;
	int timestamp;
	
	std::vector<webmallfunc> funcs;
};

struct vecdata
{
	ivec user_name;		//这个数据不会存盘
	ivec custom_status;
	ivec filter_data;
	ivec charactermode;
	ivec instancekeylist;
	itemlist inventory;
	itemlist equipment;
	itemlist petbadge;
	itemlist petequip;
	itemlist task_inventory;
	itemlist trash_box;
	itemlist trash_box2;
	itemlist fashion;
	itemlist mountwing;
	itemlist gifts;
	itemlist fuwen;
	pocket_itemlist pocket;
	ivec skill_data;
	ivec task_data;
	ivec finished_task_data;
	ivec finished_time_task_data;
	ivec var_data;
	ivec trashbox_passwd;
	ivec waypoint_list;
	ivec coolingtime;
	ivec dbltime_data;		
	ivec recipes;		
	ivec title_list;
	ivec region_reputation;
	ivec petdata;
	ivec reborndata;
	ivec fashion_hotkey;
	loglist logs;
	ivec achieve_map;
	ivec achieve_active;
	ivec achieve_spec_info;
	ivec achieve_award_map;
	ivec composkills;
	ivec raid_data;
	ivec vipaward;
	ivec onlineaward;
	ivec five_year;
	ivec treasure_info;
	ivec littlepet;
	ivec tower_raid;
	ivec ui_transfer;
	ivec collision_info;
	ivec astrology_info;
	ivec liveness_info;
	ivec sale_promotion_info;
	ivec propadd; 
	ivec multi_exp;
	ivec fuwen_info;
	ivec phase; //相位
	ivec award_info_6v6;
	ivec hide_and_seek_info;
	ivec newyear_award_info;

	std::vector<weborder> weborders; //用于 getrole
	std::vector<int64_t> processed_weborders; //用于 putrole
};

struct fac_field
{
	int index;
	int tid;
	int level;
	int status;
	int task_id;
	int task_count;
	int task_need;
	fac_field()
	{
		index = 0;
		tid = 0;
		level = 0;
		status = 0;
		task_id = 0;
		task_count = 0;
		task_need = 0;
	}
};

struct fac_auction
{
	int timestamp;
	int itemid;
	int winner;
	int cost;
	ivec winner_name;
	int status;
	int end_time;
	fac_auction()
	{
		timestamp = 0;
		itemid = 0;
		winner = 0;
		cost = 0;
		status = 0;
		end_time = 0;
	}
};

struct fac_auc_failer
{
	int roleid;
	int return_coupon;
	fac_auc_failer()
	{
		roleid = 0;
		return_coupon = 0;
	}
};

struct fac_auc_history
{
	int event_type;
	ivec rolename;
	int cost;
	int itemid;
	fac_auc_history()
	{
		event_type = 0;
		cost = 0;
		itemid = 0;
	}
};

struct fac_base_info
{
	int fid;
	int grass;
	int mine;
	int monster_food;
	int monster_core;
	int cash;
	int timestamp;
	ivec msg;//基地留言
	std::vector<fac_field> fields;
	std::vector<fac_auction> auctions;
	std::vector<fac_auc_failer> auction_failers;
	std::vector<fac_auc_history> auction_history;
	fac_base_info()
	{
		fid = 0;
		grass = 0;
		mine = 0;
		monster_food = 0;
		monster_core = 0;
		cash = 0;
		timestamp = 0;
	}
};

class Result
{
public:
	virtual void OnTimeOut() = 0;
	virtual void OnFailed() = 0;
	virtual void OnGetRole(int id,const base_info * pInfo, const vecdata * data,const GNET::GRoleDetail * pRole){}
	virtual void OnPutRole(int retcode){}
	virtual void OnPutMoneyInventory(int retcode) {}
	virtual void OnGetMoneyInventory(size_t money, const itemlist & list, int timestamp) {}
	virtual void OnGetCashTotal(int cash_total, int cash_add2, int userid){}
	virtual ~Result(){}
};

class FBaseResult
{
public:
	//virtual void OnTimeOut() = 0;
	virtual void OnFailed() = 0;
	virtual void OnGetData(int fid, const fac_base_info * pinfo) = 0;
	virtual void OnPutData() = 0;
	virtual ~FBaseResult(){}
};

class CountDropResult
{
public:
	virtual void OnCountDropResult(int retcode) = 0;
	virtual ~CountDropResult() {}		
};

bool init_gamedb();
//$$$超时60秒
bool put_role(int line_id, int id, const base_info* pInfo, const vecdata* data,
			Result *callback=NULL,int priority=0,int mask=DBMASK_PUT_ALL);
bool get_role(int line_id, int id, Result * callback);

bool set_couple(int id1, int id2, int op);

bool put_money_inventory(int id, size_t money, itemlist & list,Result * callback);
bool get_money_inventory(int id, Result * callback);

bool get_faction_base(int faction_id, FBaseResult * callback);
bool put_faction_base(int faction_id, const fac_base_info & info, FBaseResult * callback);

bool get_cash_total(int roleid, Result *callback);

size_t convert_item(const GNET::RpcDataVector<GNET::GRoleInventory> & ivec,  itemdata * list , size_t size);
 
bool put_counter_drop(int template_id, int start_time, int duration, int max_count, CountDropResult* result = NULL);//全服掉落限量

//城战相关
class City_Op_Result
{
public:
	virtual void OnGetCity(int battle_id,int stamp,int ret_code,const void* buf,size_t size){}
	virtual void OnPutCity(int battle_id,int ret_code){}
	virtual void OnTimeOut() = 0;
	virtual ~City_Op_Result(){}
};

bool put_city_detail(int battle_id,int stamp,const void* buf,size_t size,City_Op_Result* callback);
bool get_city_detail(int battie_id,City_Op_Result* callback);

};


#endif


