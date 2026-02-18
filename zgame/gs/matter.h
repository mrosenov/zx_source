#ifndef __ONLINEGAME_GS_MATTER_H__
#define __ONLINEGAME_GS_MATTER_H__

#include "object.h"
#include "actobject.h"
#include "gmatrix.h"


class mine_template;
class gmatter_imp : public gobject_imp
{
	
public:
DECLARE_SUBSTANCE(gmatter_imp);
public:
	bool _collision_actived;	//碰撞是否生效
	gmatter_imp()
	{
		_collision_actived = false;
	}

	void ActiveCollision(bool active);

	gmatter * GetParent() { return (gmatter*)_parent;}
};

class gmatter_dispatcher: public dispatcher
{
public:
DECLARE_SUBSTANCE(gmatter_dispatcher);
public:
	virtual void begin_transfer() {ASSERT(false);}//物品本身不会受到数据所以本函数为空}
	virtual void end_transfer(){ASSERT(false);}
	virtual void enter_slice(slice * ,const A3DVECTOR &){ASSERT(false);}
	virtual void leave_slice(slice * ,const A3DVECTOR &){ASSERT(false);}
	virtual void get_base_info(){ASSERT(false);}
	virtual void leave_world(){ASSERT(false);}
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode){ ASSERT(false);}

	virtual void enter_world();
	virtual void disappear(char at_once = 0);
	virtual void matter_pickup(int id);

	virtual void notify_combine_mine_state(int state);  // Youshuang add
};

class gmatter_controller : public controller
{
public:
	DECLARE_SUBSTANCE(gmatter_controller)
public:
	virtual int CommandHandler(int cmd_type,const void * buf, size_t size){ ASSERT(false); return -1;}
	virtual int MoveBetweenSlice(gobject *obj,slice * src, slice * dest) { ASSERT(false); return -1;}
	virtual void Release(bool free_parent);//这个函数会忽视free_parent的值，一律按照true来处理
};

class gmatter_item_base_imp: public gmatter_imp
{
public:
	int _owner;			//0代表every one
	int _team_owner;		//0代表no team
	int _team_seq;			//队伍的seq，只有当_team_owner非0的时候才有效
	int _life;			//寿命 单位秒
	int _owner_time;		//所有权维持时间 单位秒
	int _drop_user;			//谁扔出来的
	DECLARE_SUBSTANCE(gmatter_item_base_imp);
public:
	gmatter_item_base_imp(int life = MATTER_ITEM_LIFE,int belong_time = MATTER_ITEM_PROTECT)
			:_owner(0),_team_owner(0),_life(life),_owner_time(belong_time),_drop_user(0)
	{}

	virtual ~gmatter_item_base_imp();

	virtual void Init(world * pPlane,gobject*parent);
	virtual int MessageHandler(const MSG & msg);
	virtual void OnRelease() {}
public:
	void SetLife(int new_life)
	{
		_life = new_life;
	}

	int GetLife() 
	{
		return _life;
	}

	
	void SetOwner(const XID & who, int team , int team_seq)
	{
		_owner = who.id;
		_team_owner = team;
		_team_seq = team_seq;
	}

	void SetDrop(int user)
	{
		_drop_user = user;
	}

	bool SpecUser()
	{
		return _owner && !_team_owner;
	}
	
	bool CheckPickup(const XID & who, int team_id, int team_seq)
	{
		return (_team_owner && team_id == _team_owner && team_seq == _team_seq) 
			|| (!_team_owner && !_owner)  
			|| (_owner && who.id == _owner);
	}

	template<int>
	void Pickup(const XID & who, int random_pickup,  int team_id, int team_seq, const A3DVECTOR &pos, const XID & bb, bool is_check)
	{
		if(is_check && !CheckPickup(who,team_id,team_seq))
		{
			MSG msg;
			BuildMessage(msg,GM_MSG_ERROR_MESSAGE,who,_parent->ID,_parent->pos,S2C::ERR_ITEM_CANT_PICKUP);
			gmatrix::SendMessage(msg);
			return;
		}
		
		if(pos.squared_distance(_parent->pos) > (PICKUP_DISTANCE+0.1f)*(PICKUP_DISTANCE+0.1f))
		{
			//是否有错误消息？
			return ;
		}

		if(_owner || _team_owner <= 0 || !random_pickup)
		{
			//属于某玩家或者不属于队伍的(包含无保护的)， 以及自由捡物品的 物品将会发给该玩家
			OnPickup(who,_team_owner,false);
		}
		else
		{
			//属于队伍的， 那么按照队伍进行分配
			OnPickup(who,team_id,true);
		}

		//自己消失
		_runner->matter_pickup(who.id);
		_commander->Release();
		return ;
	}

	virtual void OnPickup(const XID & id,int team_id,bool is_team) = 0;
	
};

/*
 *		钱是一种特殊的matter,并不是物品
 */
class gmatter_money_imp : public gmatter_item_base_imp
{
public:
	size_t _money;
	DECLARE_SUBSTANCE(gmatter_money_imp);
public:
	gmatter_money_imp():_money(0){}
	gmatter_money_imp(size_t money):_money(money){}
	virtual void Init(world * pPlane,gobject*parent)
	{
		gmatter_item_base_imp::Init(pPlane,parent);
		gmatter * pMatter = (gmatter *) parent;
		pMatter->matter_type = g_config.item_money_id;		//代表金钱的物品编号:)
	}
	virtual int MessageHandler(const MSG & msg);
	virtual void OnPickup(const XID & who, int team_id,bool is_team);
};

class mine_spawner;
class gmatter_mine_imp : public gmatter_imp
{
public:
	DECLARE_SUBSTANCE(gmatter_mine_imp);
	int _produce_id;	//这个值可以为0
	int _produce_amount;
	int _produce_tool;
	int _produce_task_id;	//如果非0 表示是任务相关出产矿物  这个id是产生的id
	int _task_id;		//如果非0 表示是任务相关矿物      这个id是输入的任务id
	int _lock_id;
	mine_spawner  * _spawner;
	bool _eliminate_tool;      //是否消耗采集工具
	bool _gather_no_disappear; //采集完成后是否消失
	bool _can_be_interrupted;  //采集可以被攻击中断
	bool _broadcast_aggro;
	bool _lock;
	char _lock_time_out;
	unsigned short _gather_time;
	int _level;
	int _exp;
	int _self_faction;	
	int _ask_help_faction;
	float _aggro_range;
	int _aggro_count;
	int _life;

	struct 
	{
		int mob_id;
		int num;
		float radius;
		int remain_time;
	}produce_monster[4];

	struct
	{
		bool actived_all;
		bool deactived_all;
		unsigned char active_count;
		unsigned char deactive_count;
		int active_cond[4];
		int deactive_cond[4];
	}ctrl_info;

	

public:
	gmatter_mine_imp():_produce_id(0),_produce_amount(0),_produce_tool(0),_produce_task_id(0),_task_id(0),
			   _lock_id(0),_spawner(0),_gather_no_disappear(false),_can_be_interrupted(true),
			   _lock(false),_lock_time_out(0), _level(0),_exp(0), _life(0)
			   {
				   _eliminate_tool = false;
			   	_broadcast_aggro = false;
				_self_faction = 0;
				_ask_help_faction = 0;
				_aggro_count = 0;
				memset(produce_monster,0,sizeof(produce_monster));
				memset(&ctrl_info,0,sizeof(ctrl_info));
			   }
			   
	void SetParam(int id, int amount,unsigned short gather_time,int tool,int level, int exp)
	{
		_produce_id = id;
		_produce_amount = amount;
		_produce_tool = tool;
		_gather_time = gather_time;
		_level = level;
		_exp = exp;
	}

	void SetCtrlInfo(bool active_all, unsigned char active_count, int active_list[4],bool deactive_all, unsigned char deactive_count, int deactive_list[4])
	{
		ctrl_info.actived_all = active_all;
		ctrl_info.deactived_all = deactive_all;
		ctrl_info.active_count = active_count;
		ctrl_info.deactive_count = deactive_count;
		memcpy(ctrl_info.active_cond, active_list, 4 * sizeof(int));
		memcpy(ctrl_info.deactive_cond, deactive_list, 4 * sizeof(int));
	}


	void SetTaskParam(int task_in, int task_out, bool no_interrupted, bool gather_no_disappear, bool eliminate_tool, int self_faction , int ask_help_faction ,float aggro_range ,int aggro_count)
	{
		_task_id = task_in;
		_produce_task_id = task_out;
		_can_be_interrupted = !no_interrupted;
		_gather_no_disappear = gather_no_disappear;
		_eliminate_tool = eliminate_tool;
		_broadcast_aggro = (ask_help_faction != 0);
		_self_faction = self_faction;
		_ask_help_faction = ask_help_faction;
		_aggro_range = aggro_range;
		_aggro_count = aggro_count;
	}
	void SetMonsterParam(void * buf, size_t count);

	void SetLife(int lifetime)
	{
		_life = lifetime;
	}
	
	virtual int MessageHandler(const MSG & msg);

	template <int> void SendErrMessage(const XID & who, int message)
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_ERROR_MESSAGE,who,_parent->ID,_parent->pos,message);
		gmatrix::SendMessage(msg);
	}

	void BeMined();
	virtual void OnMined() {}
	virtual void PrepareMine(const MSG & msg) {}
	virtual void Reborn();
	// Youshuang add
	virtual void InitTemplate( mine_template* pTemplate );
	virtual void MineEnterWorld();
	// end

	bool IsMineOwner(int role_id, int team_id, int master_id);
};

class gmatter_dyn_imp : public gmatter_imp
{
public:
	DECLARE_SUBSTANCE(gmatter_dyn_imp);
	mine_spawner  * _spawner;
public:
	gmatter_dyn_imp(): _spawner(0)
	{}

	virtual void Init(world * pPlane,gobject*parent)
	{
		gmatter_imp::Init(pPlane,parent);
		gmatter * pMatter = (gmatter *) parent;
		pMatter->matter_state = gmatter::STATE_MASK_DYN_OBJECT;
	}
			   
			   
	virtual int MessageHandler(const MSG & msg);

	template <int> void SendErrMessage(const XID & who, int message)
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_ERROR_MESSAGE,who,_parent->ID,_parent->pos,message);
		gmatrix::SendMessage(msg);
	}

	virtual void Reborn();
};

// Youshuang add
class  gmatter_combine_mine_imp : public gmatter_mine_imp
{
public:
	DECLARE_SUBSTANCE(gmatter_combine_mine_imp);
	gmatter_combine_mine_imp();
	virtual void Init( world * pPlane,gobject*parent );
	virtual void OnMined();
	virtual void InitTemplate( mine_template* pTemplate );
	virtual void MineEnterWorld();
	void GenCombineMineData();
	int GetState(){ return state; }
	virtual void Reborn();
	void NotifyMineState();

private:
	int state;
	int gather_count;
	int result_tid;
	std::vector<int> tids;
};
// end

void DropMoneyItem(world * pPlane, const A3DVECTOR & pos, size_t amount,const XID &owner,int owner_team,int seq,int drop_id = 0);
#endif

