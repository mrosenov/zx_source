#ifndef __ONLINEGAME_GS_BG_WORLD_MANAGER_H__ 
#define __ONLINEGAME_GS_BG_WORLD_MANAGER_H__

#include "../global_manager.h"
#include "../world.h"
#include "../usermsg.h"
#include <vector>
#include <hashmap.h>

enum
{
	DEFAULT_MAX_PLAYER_COUNT =		15,	// 每方人数上限
	DEFAULT_NEED_LEVEL_MIN =		1,
	DEFAULT_NEED_LEVEL_MAX =		200,
	// 踢出时间,注意 战场关闭的时间需要比最大踢出时间大一些
	DEFAULT_KICKOUT_TIME_MIN =		60,
	DEFAULT_KICKOUT_TIME_MAX =		180,
	DEFAULT_KICKOUT_CLOSE_TIME =		300,
	CHAT_BATTLE =				25,	// 约定的战场格式化喊话id
	// 保证battle的tag是连续的区间
	BATTLE_WORLD_TAG_BEGIN =		101,	// 战场地图号起始
	
};

// 控制器的问题
enum
{
	// 使用这个基值来计算出实际在战场中使用的id
	CTRL_ID_BATTLE_BASE =			80000,	// 基值
	
	CTRL_CONDISION_ID_BATTLE_MIN =		70204,	// 最小战场控制器id
	CTRL_CONDISION_ID_BATTLE_BEGIN =	70204,	// 战场开启，刷出普通建筑物

	CTRL_CONDISION_ID_BATTLE_ATTACKER_KEY = 70225,	// 刷出攻方key building
	CTRL_CONDISION_ID_BATTLE_DEFENDER_KEY = 70226,	// 刷出守方key building

	CTRL_CONDISION_ID_BATTLE_TEAM_CRSSVR_BEGIN = 70251, //跨服小队PK,第一小场
	CTRL_CONDISION_ID_BATTLE_TEAM_CRSSVR_END   = 70255, //跨服小队PK,最后一个小场
	
	CTRL_CONDISION_ID_BATTLE_MAX =		70700,	// 最大战场控制器id

	CTRL_CONDISION_ID_BATTLE_OFFSET = CTRL_CONDISION_ID_BATTLE_MAX - CTRL_CONDISION_ID_BATTLE_MIN + 1,
}; 

enum BATTLE_RESULT
{
	BATTLE_ATTACKER_WIN =	1,	// 攻方获胜
	BATTLE_DEFENDER_WIN =	2,	// 守方获胜
	BATTLE_DRAW =		3,	// 平手
	BATTLE_MELEE_END = 	4,	// 混战结束
};


/*********************************************************************
	普通战场: BT_COMMON BT_REBORN_COMMON
	跨服PK  : BT_CROSSSERVER
	楚汉战场: BT_KILLFIELD  BT_REBORN_KILLFIELD
	竞技场  : BT_ARENA  BT_REBORN_ARENA
	剧情	: BT_INSTANCE 
***********************************************************************/

enum BATTLE_TYPE
{
	BT_COMMON,		//普通
	BT_CROSSSERVER,		//跨服PK
	BT_INSTANCE,		//剧情
	BT_REBORN_COMMON,	//飞升普通
	BT_KILLFIELD,		//普通杀戮战场(楚汉战场)

//5
	BT_REBORN_KILLFIELD,	//飞升杀戮战场
	BT_ARENA,		//普通竞技场
	BT_REBORN_ARENA,	//飞升竞技场
	BT_MELEE_CROSSSERVER,	//匿名跨服PK
	BT_CHALLENGE_SPACE,   //挑战空间

//10
	BT_TEAM_CRSSVR,      //跨服小队PK
	BT_FLOW_CRSSVR,		 //跨服流水席战场
};

enum BATTLE_FACTION
{       
	BF_NONE                 = 0,    //无阵营
	BF_ATTACKER             = 1,    //攻击方
	BF_DEFENDER             = 2,    //防守方
	BF_VISITOR		= 3,    //观察者 
	BF_MELEE		= 4,	//混战者
};  

struct bg_param
{
	int battle_world_tag;
	int battle_id;
	int end_timestemp;
	int apply_count;
};

struct battle_field_info
{
	int battle_id;
	int battle_world_tag;
	int level_min;
	int level_max;
	int battle_type;
	int reborn_count_need;     //进入战场条件,0代表普通战场，1代表飞升战场
	int fighting_time;         //战场战斗时间，单位为秒
	int max_enter_perday;      //每个玩家每天能参加的次数
	int cooldown_time;         //连续报名的间隔时间,冷却时间,单位为秒
	int max_player_count;      //战场最大人数
};

struct player_battle_info
{
	player_battle_info():battle_faction(0),battle_score(0),kill(0),death(0),rank(1),conKill(0),timestamp(0)
	{
	}
	player_battle_info( int bf, int bs, int k, int d, int r, int co):
		battle_faction(bf), battle_score(bs), kill(k), death(d), rank(r), conKill(co)
	{
	}
	player_battle_info( int bf, int bs, int k, int d, int r, int co, int ti):
		battle_faction(bf), battle_score(bs), kill(k), death(d), rank(r), conKill(co),timestamp(ti) 
	{
	}
	int battle_faction;
	unsigned int battle_score;
	int kill;
	int death;
	int rank;
	int conKill;
	int timestamp;
};

typedef S2C::CMD::killingfield_info::player_info_in_killingfield player_info_killingfield;
typedef S2C::CMD::battle_info::player_info_in_battle player_info_battle;
typedef S2C::CMD::arena_info::player_info_in_arena player_info_arena;
typedef S2C::CMD::crossserver_team_battle_info::history_score history_round_score;
typedef S2C::CMD::flow_battle_info::player_info_in_flow_battle player_info_flow_battle;
typedef S2C::CMD::cs_battle_off_line_info::player_off_line_in_cs_battle player_off_line_in_cs_battle;

class bg_world_manager: public global_world_manager
{
protected:
	int _max_player_count;		// 最大玩家限制, 每方
	int _need_level_min;		// 进入玩家的等级限制，最小级别
	int _need_level_max;		// 进入玩家的等级限制，最大级别
	
	int _reborn_count_need;     //进入战场条件,0代表普通战场，1代表飞升战场
	int _fighting_time;         //战场战斗时间，单位为秒
	int _max_enter_perday;      //每个玩家每天能参加的次数
	int _cooldown_time;         //连续报名的间隔时间,冷却时间,单位为秒
		
	int _kickout_time_min;
	int _kickout_time_max;
	int _kickout_close_time;

	int _cur_timestamp;		// 流程使用时间戳
	int _end_timestamp;		// 战争结束时间戳
	int _start_timestamp;		// 战场开启时间戳

	int _heartbeat_counter;		// 心跳计时
	int _battle_info_seq;		// 发送信息序列
	int _update_info_seq;		// 更新信息序列 
	int _apply_count;		// 报名人数
	int _battle_id;			// 战场id,两个world对应一个id,保证切换使用
	int _battle_type;		// 战场类型
	int _battle_result;
	int _battle_lock;
	int _status;			// 战争状态
	int _battle_winner_id;
	int _max_killer_id;		// 杀人最多的人

	int _attacker_score;		// 攻方积分
	int _defender_score;		// 守方积分
	
	cs_user_map  _defender_list;
	cs_user_map  _attacker_list;
	cs_user_map  _all_list;

	int _bg_template_id;	//战场模板ID
	
	enum
	{
		BS_READY = 0,		// 服务器准备完毕等待delivery命令
		BS_PREPARING = 1,	// 战斗准备状态(暂时不用)
		BS_RUNNING = 2,		// 战斗开始状态
		BS_CLOSING = 3,		// 战斗关闭，清理怪物等操作进行中
	};

	struct building_data
	{
		int normal_building_init;	// 初始化个数
		int key_building_init;		// 初始化个数
		int normal_building_left;	// 剩余普通建筑物个数
	};

	typedef std::map<int,player_battle_info> PlayerInfoMap;
	typedef std::map<int,player_battle_info>::iterator PlayerInfoIt;

	// 单方数据
	struct competitor_data
	{
		PlayerInfoMap player_info_map;
		// 建筑物数据
		building_data buildings;
		// 首次击杀记录
		bool first_kill;
		struct revive_pos_t
		{
			A3DVECTOR pos;
			bool active;
		};
		typedef std::map<int,std::vector<revive_pos_t> > CtrlReviveMap;
		typedef std::map<int,std::vector<revive_pos_t> >::iterator CtrlReviveMapIt;
		std::vector<A3DVECTOR> town_list;             //回城点
		std::vector<A3DVECTOR> entry_list;            //进入点
		CtrlReviveMap revive_map;                       //复活点
		int kill_count;
	};
	
	struct scorePredicate
	{
		scorePredicate(size_t score) : _score(score)
		{}

		bool operator()(const std::pair<int, player_battle_info> & pr)
		{
			return (pr.second).battle_score > _score;
		}	

		size_t _score;
	};

	competitor_data _attacker_data;
	competitor_data _defender_data;
	competitor_data _melee_data;  //竞技场使用这个

	int _off_line_heartbeat_counter; //离线扫描心跳计时
	int _min_player_deduct_score; //跨服战场少于多少人就扣分
	int _first_deduct_off_line_time; // 跨服战场第一次扫描时间
	int _delta_time; //跨服战场扫描时间间隔
	int _deduct_score; //离线扣分数


public:
	int Init( const char* gmconf_file, const char* servername, int tag, int index);
	void Heartbeat();
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );
	void PostPlayerLogin( gplayer* pPlayer );
	void RecordBattleBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building = false );

public:
	bg_world_manager()
	{
		_battle_id = 0;
		_max_player_count = 0;
		_need_level_min = 0;
		_need_level_max = 0;
		_battle_type = -1;

		_reborn_count_need = 0;
		_fighting_time     = 1800;   //默认为半小时
		_max_enter_perday  = 0;
		_cooldown_time     = 1800;   //默认为半小时

		_kickout_time_min = 0;
		_kickout_time_max = 0;
		_kickout_close_time = 0;

		_battle_lock = 0;
		_apply_count = 0;

		memset( &_attacker_data.buildings, 0, sizeof( building_data ) );
		memset( &_defender_data.buildings, 0, sizeof( building_data ) );

		_attacker_data.first_kill = false;
		_defender_data.first_kill = false;

		_attacker_score = 0;
		_defender_score = 0;

		_bg_template_id = 0;

		_min_player_deduct_score = -1;
		_first_deduct_off_line_time = -1;
		_delta_time = -1;
		_off_line_heartbeat_counter = 0;
	}
	virtual ~bg_world_manager()
	{
	}
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
	bool CreateBattleGround( const bg_param& );

	virtual void Reset()
	{
		_start_timestamp = 0;
		_end_timestamp = 0;
		_cur_timestamp = 0;
		_heartbeat_counter = 0;
		_status = BS_READY;
		_battle_result = 0;
		_max_killer_id = 0;
		_battle_winner_id = 0;
		_battle_info_seq = 0;
		_update_info_seq = 0;
		_apply_count = 0;
		_attacker_data.player_info_map.clear();
		_defender_data.player_info_map.clear();
		_attacker_data.buildings.normal_building_left = _attacker_data.buildings.normal_building_init;
		_defender_data.buildings.normal_building_left = _defender_data.buildings.normal_building_init;
		_attacker_data.first_kill = true;
		_defender_data.first_kill = true;
		_attacker_data.kill_count = 0;
		_defender_data.kill_count = 0;
		_attacker_score = 0;
		_defender_score = 0;
		_defender_list.clear();
		_attacker_list.clear();
		_all_list.clear();
	}

	inline int get_cs_user_map_size(const cs_user_map& map )
	{

		int counter = 0;
		for (cs_user_map::const_iterator it = map.begin(); it != map.end(); ++it)
		{
			const cs_user_list & list = it->second;
			counter += list.size();
		}
		return counter;
	}

	// 战场map返回true
	inline bool IsBattleWorld() { return true; }
	//飞升玩家才能参加的战场
	inline bool IsRebornBattle(){return _battle_type == BT_REBORN_COMMON || _battle_type == BT_REBORN_KILLFIELD || _battle_type == BT_REBORN_ARENA;}
	//未飞升玩家才能参加的战场
	inline bool IsNormalBattle(){return _battle_type == BT_COMMON || _battle_type == BT_KILLFIELD || _battle_type == BT_ARENA;}
	//普通战场(包裹飞升和未飞升战场)
	inline bool IsCommonBattle(){return _battle_type == BT_COMMON || _battle_type == BT_REBORN_COMMON;}
	inline bool IsKillingField() {return _battle_type == BT_KILLFIELD || _battle_type == BT_REBORN_KILLFIELD;  }
	//竞技场(包括飞升和未飞升)	
	inline bool IsArena(){return _battle_type == BT_ARENA || _battle_type == BT_REBORN_ARENA;}
	inline bool IsInstanceBattle(){return _battle_type == BT_INSTANCE; }
	
	inline bool IsBattleRunning() { return ( BS_RUNNING == _status ); }

	inline int GetBattleID() { return _battle_id; }
	inline int GetBattleType() { return _battle_type; }
	inline int GetLevelMin() { return _need_level_min; }
	inline int GetLevelMax() { return _need_level_max; }
	inline int GetBattleResult() { return _battle_result; }
	inline int GetBattleMaxKillerID() { return _max_killer_id; }
	inline int GetBattleWinnerID(){return _battle_winner_id; }
	inline int GetKickoutTimeMin() { return _kickout_time_min; }
	inline int GetKickoutTimeMax() { return _kickout_time_max; }
	inline int GetKickoutCloseTime() { return _kickout_close_time; }
	inline int GetBattleStatus() { return _status; }
	inline int GetBattleStartstamp() { return _start_timestamp; }
	inline int GetBattleEndstamp() { return _end_timestamp; }
	inline int GetBattleTemplateId() { return _bg_template_id; }


	// type 0 第一滴血, 1 杀神, 2 阻止
	void BroadcastBattleMsg( unsigned char type, unsigned char battle_faction, 
			int battle_id, int gs_id, int killer, int deadman );
public:
	// 统一做吧
	virtual void HandleModifyBattleDeath( int player_id, int death_count, size_t content_length, const void * content);
	virtual void HandleModifyBattleKill( int player_id, int kill_count, size_t content_length, const void * content);
	virtual void SyncCSFlowPlayerInfo( int player_id, size_t content_length, const void* content) {}
	void HandleSyncBattleInfo( int player_id, int reserver, size_t content_length, const void * content);

	//战场频道
	void BattleFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int char_emote, int self_id);
	void BattleSay( const void* buf, size_t size ){}
	int TranslateCtrlID( int which );
	int GetPostion(int id, int score);

public:
	virtual void PlayerEnter( gplayer* pPlayer, int faction); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer
	virtual void PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick, bool disconnected = false);

	//注册回城点
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction);
	//注册复活点
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id);
	//注册进入点
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	//获得回城点 
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	//获得复活点
	virtual bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag);
	//获得随机进入点
	virtual void GetRandomEntryPos(A3DVECTOR& pos,int battle_faction);

	virtual bool GetCommonBattleInfo(int& info_seq, unsigned char& attacker_building_left, unsigned char& defender_building_left, 
			std::vector<player_info_battle>& info, bool bForce = false ){return false;}
	virtual bool GetKillingFieldInfo(int& info_seq, int & attacker_score, int & defender_score, int & attacker_count, int & defender_count, 
			std::vector<player_info_killingfield> & info, bool bForce = false){return false;}
	virtual bool GetCrossServerBattleInfo(int& info_seq, S2C::CMD::crossserver_battle_info& info, bool bForce = false){return false;}
	virtual bool GetCrossServerOfflineInfo(std::vector<player_off_line_in_cs_battle>& vec_info) { return false;}
	virtual bool GetCSFlowBattleInfo(int player_id, int & score, int & c_kill_cnt, int& m_kill_num){return false;}
	virtual bool GetCSFlowBattleInfo(int& info_seq, std::vector<player_info_flow_battle>& info, bool bForce = false ){return false;}

	virtual bool GetArenaInfo(int& info_seq, int & apply_count, int & remain_count, std::vector<player_info_arena> & info, bool bForce = false){return false;}
	virtual bool GetInstanceBattleInfo(int& info_seq, int& cur_level, int& level_score, int& level_time_score,
			int& monster_score, int& death_penalty_score, std::vector<char> & level_info, bool bForce = false){return false;}
	virtual int GetBattleTotalScore()
	{
		return _attacker_score + _defender_score;
	}

	virtual int GetAttackerScore() { return _attacker_score;}
	virtual int GetDefenderScore() { return _defender_score;}
	
	virtual void OnDeliveryConnected( std::vector<battle_field_info>& info );
	virtual void OnDeliveryConnected( std::vector<instance_field_info>& info ){}
	virtual int OnMobDeath( world * pPlane, int faction,int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnPlayerDeath( int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void OnKillMonster(int score){}

	virtual int  GetRoundStartstamp() { return 0; }
	virtual bool IsRoundRunning() { return false; }
	virtual void UpdateRoundScore(int battle_faction, int change) {}
	virtual bool GetCrossServerTeamBattleInfo(int& info_seq, S2C::CMD::crossserver_team_battle_info & info, std::vector<history_round_score>& history_info, bool bForce = false) { return false; }
	virtual bool IsLimitSkillInWorld(int skill_id) { return false; }
	virtual bool IsForbiddenItem(int item_type) { return false; }


	virtual void ForceClose();
	
protected:
	virtual bool OnInit(){return true;}	
	virtual void OnCreate(){}
	virtual void OnRunning(){}			
	virtual void OnClose(){}
	virtual void OnReady(){}
	
	virtual int OnDestroyKeyBuilding( int faction);
	virtual int OnDestroyNormalBuilding( int faction){return 0;}
	virtual void OnPlayerLeave( gplayer* pPlayer, int faction, bool cond_kick);
	virtual void OnBattleInfoChange(){}
	
protected:
	void SendBattleInfo();
	void DumpWorldMsg();
	void KillAllMonsters();
	void UpdatePlayerInfo(PlayerInfoIt &it, size_t content_length, const void * content);
	
};


////////////////////////////////////////////////////////////////////////////////////////////////////
//普通战场逻辑
class cb_world_manager : public bg_world_manager
{
public:
	virtual bool GetCommonBattleInfo(int& info_seq, unsigned char& attacker_building_left, unsigned char& defender_building_left, 
			std::vector<player_info_battle>& info, bool bForce = false );

protected:
	virtual bool OnInit();
	virtual void OnCreate()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
	}
	
	virtual int OnDestroyNormalBuilding(int faction);
	virtual void OnClose()
	{
		FindMaxKillerID();
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, false );
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_ATTACKER_KEY, false );
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_DEFENDER_KEY, false );

		if(0 == _battle_result) _battle_result = BATTLE_DRAW; 

	}

private:
	void FindMaxKillerID();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//楚汉战场逻辑
class kf_world_manager : public bg_world_manager
{
public:
	virtual bool GetKillingFieldInfo(int& info_seq, int & attacker_score, int & defender_score, int & attacker_count, int & defender_count, 
			std::vector<player_info_killingfield> & info, bool bForce = false);

protected:
	virtual bool OnInit()
	{
		if(_attacker_data.buildings.normal_building_init > 0 || _defender_data.buildings.normal_building_init > 0 ||
				_attacker_data.buildings.key_building_init != _defender_data.buildings.key_building_init )
		{
			return false;
		}
		return true;
	}
	virtual void OnCreate()
	{
		ActiveSpawn( this, player_template::GetMapControllerID(GetWorldIndex()), true );
	}
	
	virtual void OnRunning()
	{
		// 30秒更新一次信息
		if(_heartbeat_counter %  (30 * 20) == 0 && _update_info_seq < _battle_info_seq)
		{
			OnBattleInfoChange();
			_update_info_seq = _battle_info_seq;
			
		}
	}

	virtual void OnClose()
	{
		ActiveSpawn( this, player_template::GetMapControllerID(GetWorldIndex()), false );
		OnBattleInfoChange();
		if(0 != _battle_result) return;
		if(_attacker_score > _defender_score)
		{
			_battle_result = BATTLE_ATTACKER_WIN;
		}
		else if(_attacker_score < _defender_score)
		{
			_battle_result = BATTLE_DEFENDER_WIN;
		}
		else
		{
			_battle_result = BATTLE_DRAW;
		}
	}
	
	virtual void OnReady()
	{
		_top_list.clear();
	}
	
	virtual void OnBattleInfoChange()
	{
		PlayerInfoIt iter; 
		_attacker_score = 0;
		_defender_score = 0;
		for(iter = _attacker_data.player_info_map.begin(); iter != _attacker_data.player_info_map.end(); ++iter)
		{
			_attacker_score += (iter->second).battle_score;
		}	

		for(iter = _defender_data.player_info_map.begin(); iter != _defender_data.player_info_map.end(); ++iter)
		{
			_defender_score += (iter->second).battle_score;
		}	
		UpdateTopList();

	}

private:
	enum
	{
		TOP_LIST_MAX_NUM = 			20,	// 战场里最多排行榜的人数
	};
	
	class CompareScore
	{
	public:
		bool operator()(const player_info_killingfield & p1, const player_info_killingfield & p2)
		{
			return p1.score > p2.score;
		}
		
	};
	std::vector<player_info_killingfield> _top_list;
	void UpdateTopList();
		
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//竞技场逻辑
class ar_world_manager : public bg_world_manager
{
public:
	//注册回城点
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction);
	//注册复活点
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id);
	//注册进入点
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	virtual bool GetArenaInfo(int& info_seq, int & apply_count, int & remain_count, std::vector<player_info_arena> & info, bool bForce = false);

protected:
	virtual bool OnInit()
	{
		if(_attacker_data.buildings.normal_building_init > 0 || _defender_data.buildings.normal_building_init > 0 ||
				_attacker_data.buildings.key_building_init > 0 ||  _defender_data.buildings.key_building_init  > 0)
		{
			return false;
		}
		return true;
	}
	virtual void OnCreate()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
	}
	virtual void OnRunning()
	{
		// 1秒更新一次信息
		if(_heartbeat_counter %  (30) == 0 && _update_info_seq < _battle_info_seq)
		{
			OnBattleInfoChange();
			_update_info_seq = _battle_info_seq;
			
		}

		if(g_timer.get_systime() > GetBattleStartstamp() + ARENA_RESULT_OPEN)
		{
			if(_melee_data.player_info_map.size() <= 1)
			{
				_battle_result = BATTLE_MELEE_END;		
			}	
		}
	}
	virtual void OnClose()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, false );
		UpdateTopList();
		FindArenaWinnerID();
		if(0 == _battle_result) _battle_result = BATTLE_MELEE_END; 
	}
	
	virtual void OnBattleInfoChange()
	{
		UpdateTopList();
	}
private:
	enum
	{
		ARENA_RESULT_OPEN = 			180,	// 竞技场开始统计结果
		TOP_LIST_MAX_NUM = 			10,	// 战场里最多排行榜的人数
	};

	void FindArenaWinnerID()
	{
		PlayerInfoMap::iterator it;
		unsigned int max_score = 0;
		int max_timestamp = 0;
		for(it = _melee_data.player_info_map.begin(); it != _melee_data.player_info_map.end(); ++it)
		{
			if(it->second.battle_score > max_score || (it->second.battle_score == max_score && it->second.timestamp < max_timestamp))
			{
				max_score = it->second.battle_score;
				max_timestamp = it->second.timestamp;
				_battle_winner_id = it->first;
			}	
		}
	}
	
	class CompareScore
	{
	public:
		bool operator()(const player_info_arena & p1, const player_info_arena & p2)
		{
			return p1.score > p2.score;
		}
		
	};
	std::vector<player_info_arena> _top_list;
	void UpdateTopList();

};

////////////////////////////////////////////////////////////////////////////////////////////////////
//跨服战场逻辑
class cs_world_manager : public bg_world_manager
{
public:
	virtual void PlayerEnter( gplayer* pPlayer, int faction); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer
	virtual void PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick, bool disconnected = false);
	virtual bool GetCrossServerBattleInfo(int& info_seq, S2C::CMD::crossserver_battle_info& info, bool bForce = false);
	virtual bool GetCrossServerOfflineInfo(std::vector<player_off_line_in_cs_battle>& vec_info); 
	virtual bool GetCommonBattleInfo(int& info_seq, unsigned char& attacker_building_left, unsigned char& defender_building_left, 
			std::vector<player_info_battle>& info, bool bForce = false );
	
	virtual bool IsLimitSkillInWorld(int skill_id);

	//是否是禁用物品
	bool IsForbiddenItem(int item_type);


protected:
	virtual bool OnInit()
	{
		if(_attacker_data.buildings.normal_building_init > 0 || _defender_data.buildings.normal_building_init > 0 ||
				_attacker_data.buildings.key_building_init > 0 ||  _defender_data.buildings.key_building_init  > 0)
		{
			return false;
		}

		InitiForbiddenItemList();

		memset(_off_line_score_list,0,sizeof(_off_line_score_list));


		return true;
	}
	virtual void OnCreate()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
	}

	virtual void Reset()
	{
		bg_world_manager::Reset();
		_vec_off_line_list.clear();
		memset(_off_line_score_list,0,sizeof(_off_line_score_list));
	}

	virtual void OnRunning()
	{
		//人数限制扫描
		int scan_off_line_counter = _first_deduct_off_line_time * 20;
		if (_off_line_heartbeat_counter >= scan_off_line_counter && (_off_line_heartbeat_counter - scan_off_line_counter) % (20 * _delta_time) == 0)
		{
			int attacker_counter = 0;
			int defender_counter = 0;

			attacker_counter =  get_cs_user_map_size(_attacker_list);
			defender_counter =  get_cs_user_map_size(_defender_list);
			player_off_line_in_cs_battle tmp;
			if (attacker_counter < _min_player_deduct_score)
			{
				tmp.battle_faction = BF_ATTACKER;
				tmp._timestamp = _off_line_heartbeat_counter / 20;
				tmp.off_line_count = attacker_counter;
				tmp.off_line_score = (_min_player_deduct_score - attacker_counter) * _deduct_score;
				_vec_off_line_list.push_back(tmp);
				_off_line_score_list[BF_ATTACKER] += tmp.off_line_score;
			}
			if (defender_counter < _min_player_deduct_score)
			{
				tmp.battle_faction = BF_DEFENDER;
				tmp._timestamp = _off_line_heartbeat_counter / 20;
				tmp.off_line_count = defender_counter;
				tmp.off_line_score = (_min_player_deduct_score - defender_counter) * _deduct_score;
				_vec_off_line_list.push_back(tmp);
				_off_line_score_list[BF_DEFENDER] += tmp.off_line_score;
			}
		}

	}

	virtual void OnClose()
	{
		_attacker_score = _attacker_data.kill_count;
		_defender_score = _defender_data.kill_count;

		_attacker_score += _off_line_score_list[BF_DEFENDER];
		_defender_score += _off_line_score_list[BF_ATTACKER];
		if(0 != _battle_result) return;
		if(_attacker_score > _defender_score)
		{
			_battle_result = BATTLE_ATTACKER_WIN;
		}
		else if(_attacker_score < _defender_score)
		{
			_battle_result = BATTLE_DEFENDER_WIN;
		}
		else
		{
			_battle_result = BATTLE_DRAW;
		}
	}	

	
private:
	int attacker_score;
	int defender_score;
	enum
	{
		CROSSSERVER_RESULT_OPEN =	180,	// 跨服战场开始统计结果
	};

	bool InitiForbiddenItemList();  //只在本地图中禁止的物品
	abase::static_set<int, abase::fast_alloc<> > _forbidden_item_set;
	std::vector<player_off_line_in_cs_battle> _vec_off_line_list;
	int  _off_line_score_list[BF_DEFENDER+1];
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//剧情战场逻辑
class ib_world_manager : public bg_world_manager
{
private:
	int _win_ctrl_id;				//胜利控制器ID
	int _cur_level;					//当前的关卡
	int _level_score;				//关卡积分
	int _level_time_score;				//关卡时间积分
	int _monster_score;				//杀怪积分
	int _death_penalty_score;			//死亡扣分
	
	std::vector<instance_level_info> _level_info;	//关卡详细信息

	enum LEVEL_STATUS
	{
		LEVEL_INACTIVE = 0,
		LEVEL_RUNNING,
		LEVEL_FAILURE,
		LEVEL_COMPLETE,
	};	

public:
	ib_world_manager()
	{
		_win_ctrl_id = 0;
		_cur_level = 0;
		_level_score = 0;
		_level_time_score = 0;
		_monster_score = 0;
		_death_penalty_score = 0;
	}

	virtual void OnDeliveryConnected( std::vector<battle_field_info>& info ){}
	virtual void OnDeliveryConnected( std::vector<instance_field_info>& info );
	virtual int OnMobDeath(world * pPlane, int faction,int tid,const A3DVECTOR& pos, int attacker_id){return 0;}
	virtual void OnPlayerDeath( int killer, int deadman, int deadman_battle_faction, bool& kill_much )
	{
		spin_autolock keeper( _battle_lock );
		if(_cur_level <= 0 || (size_t)_cur_level > _level_info.size()) return;
		if(_level_info[_cur_level-1].status != LEVEL_RUNNING) return;
		_death_penalty_score -= _level_info[_cur_level-1].death_penlty_score;
		OnBattleInfoChange();
	}
	virtual void OnKillMonster(int score)
	{
		spin_autolock keeper( _battle_lock );
		if(score > 0)
		{
			_monster_score += score;
			OnBattleInfoChange();
		}
	}	

	virtual void OnActiveSpawn(int id,bool active);
	
	virtual bool GetInstanceBattleInfo(int& info_seq, int& cur_level, int& level_score, int& level_time_score,
			int& monster_score, int& death_penalty_score, std::vector<char> & level_info, bool bForce = false);
	virtual int GetBattleTotalScore();

protected:
	virtual bool OnInit()
	{
		_win_ctrl_id = TranslateCtrlID(player_template::GetInstanceWinCtrlID(GetWorldIndex()));

		if(_win_ctrl_id == 0 ) return false;
		memset(&_level_info, 0, sizeof(_level_info));
		if(player_template::GetInstanceLevelInfo(GetWorldIndex(), _level_info) == -1) return false;

		for(size_t i = 0; i < _level_info.size(); ++i)
		{
			_level_info[i].open_ctrl_id = TranslateCtrlID(_level_info[i].open_ctrl_id);
			_level_info[i].finish_ctrl_id = TranslateCtrlID(_level_info[i].finish_ctrl_id);
		}

		UpdateBattleStatus(BS_READY);	
		return true;
	}
	virtual void OnCreate()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
		UpdateBattleStatus(BS_RUNNING);	
	}
	virtual void OnRunning()
	{
		int cur_time = g_timer.get_systime();
		for(size_t i = 0; i < _level_info.size(); ++i)
		{
			if(_level_info[i].status == LEVEL_RUNNING && _level_info[i].start_timestamp != 0)
			{
				if(_level_info[i].time_limit > 0 && cur_time - _level_info[i].start_timestamp >= _level_info[i].time_limit)
				{
					_cur_level = 0;
					_level_info[i].status = LEVEL_FAILURE;
					SendBattleInfo();
					OnBattleInfoChange();
				}
			}
		}
	}			
	virtual void OnClose()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, false );
		//玩家是攻方
		if(0 == _battle_result) _battle_result = BATTLE_DEFENDER_WIN;
		UpdateBattleStatus(BS_CLOSING);	
	}
	virtual void OnReady()
	{
		_cur_level = 0;
		_level_score = 0;
		_level_time_score = 0;
		_monster_score = 0;
		_death_penalty_score = 0;
		UpdateBattleStatus(BS_READY);	
		for(size_t i = 0; i < _level_info.size(); ++i)
		{
			_level_info[i].status = LEVEL_INACTIVE;
			_level_info[i].start_timestamp = 0;
			_level_info[i].close_timestamp = 0;
		}
	}
	virtual void OnPlayerLeave(gplayer* pPlayer, int faction, bool cond_kick);
	virtual void OnBattleInfoChange();

protected:
	void UpdateBattleStatus(int status);

};

//挑战空间
class ch_world_manager : public bg_world_manager
{
public:
	virtual bool OnInit();
	virtual void OnCreate()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
	}
	
	virtual void OnClose()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, false );
	}
};


//跨服小队PK
class cs_team_world_manager : public bg_world_manager
{
	enum{UNKNOW, WAITING, PREPARING, RUNNING, CLOSING};

	enum
	{
		ROUND_IS_WAITING = 0,
		ROUND_IS_RUNNING = 1,

		RESURRECT_SKILL_1 = 268,     //天音的复活技能
		RESURRECT_SKILL_2 = 2469,    //天华的复活技能
	};

	struct battle_match_info
	{
		int state_end_time;    //传给客户端用于显示，该值的休息状态包含WAITING、PREPARING、CLOSING
		int cur_round;         //表示当前是第几局
		int result_attacker;   //大场比分
		int result_defender;   //大场比分
		int round_start_time;  //每局的开始时间
		int best_killer_id;    //每小局累计击杀数最高的player
	    bool is_round_end;     

		inline void clear_best_killer() { best_killer_id = -1; }
		inline void set_best_killer(int id) { best_killer_id = id; }

		inline void set_end_time(int endtime) { state_end_time = endtime; }
		inline void set_cur_round(int roundno) { cur_round = roundno; }
		inline void inc_cur_round() { cur_round++; }

		//大场比分操作
		inline void inc_att_res() { result_attacker++; }	
		inline void inc_def_res() { result_defender++; }
		inline int  get_att_res() { return result_attacker; }
		inline int  get_def_res() { return result_defender; }

		inline bool is_att_greater() 
		{
			if(result_attacker > result_defender)
			{
				return true;
			}
			return false;
		}
		inline bool is_def_greater()
		{
			if(result_attacker < result_defender)
			{
				return true;
			}
			return false;
		}

		inline bool is_att_def_equal()
		{
			if(result_attacker == result_defender)
			{
				return true;
			}
			return false;
		}

		inline void clear_game_result() 
		{
			result_attacker = 0;
			result_defender = 0;
		}

		inline void set_is_round_end(bool isroundend) { is_round_end = isroundend; }
		inline void set_round_start_time(int time) { round_start_time = time; }
		inline int  get_round_start_time() { return round_start_time; }
	};

	//各个状态的持续时间,单位为秒
	static const int STATE_WAITING_TIME   = 60;
	static const int STATE_PREPARING_TIME = 20;
	static const int STATE_RUNNING_TIME   = 180;
	static const int STATE_CLOSING_TIME   = 10;
	static const int ROUND_COUNT = 5;


public:
	virtual void PlayerEnter( gplayer* pPlayer, int faction); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer
	virtual void PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick, bool disconnected = false);

	virtual bool OnInit();
	virtual void OnRunning();	
	virtual void OnCreate();
	virtual void OnClose();
    virtual bool GetRebornPos(gplayer_imp* pImp, A3DVECTOR& pos, int& tag);
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);

	virtual int GetRoundStartstamp() { return _match_info.get_round_start_time(); }
	virtual bool IsRoundRunning() { return (_cur_round_status == RUNNING); } 
	virtual void UpdateRoundScore(int battle_faction, int change);
	virtual bool GetCommonBattleInfo( int& info_seq, unsigned char& attacker_building_left,
			unsigned char& defender_building_left, std::vector<player_info_battle>& info, bool bForce );

	virtual bool GetCrossServerTeamBattleInfo(int& info_seq, S2C::CMD::crossserver_team_battle_info & info, std::vector<history_round_score>& history_info, bool bForce);


	virtual bool IsLimitSkillInWorld(int skill_id);
	virtual int GetAttackerScore();
	virtual int GetDefenderScore();
	virtual void OnReady();

	//是否是禁用物品
	bool IsForbiddenItem(int item_type);

	cs_team_world_manager();
	~cs_team_world_manager();


private:
	void ClearScore();
	int  CalculateRoundResult(); //计算每小局的比赛结果，返回best_killer的id

	void SendRoundStartMessage();
	void SendRoundEndMessage();

	bool CheckBattleResult();
	bool InitiForbiddenItemList();  //只在本地图中禁止的物品


private:
	inline void set_cur_status(int status, int endtime)
	{
		_cur_round_status = status;
		_cur_state_end_time = endtime;
	}
	inline int get_cur_endtime() { return _cur_state_end_time; };


private:
	int _cur_round_status;   //标记当前小场比赛的状态
	int _cur_state_end_time; //当前状态的结束时间,该值与传给客户端的当前状态结束时间不同,见_state_end_time

	int _cs_team_ctrl[ROUND_COUNT + 1]; //五场比赛分别对应五个不同的控制器，再加上一个默认的70204
	int _cs_team_lock;

	abase::static_set<int, abase::fast_alloc<> > _forbidden_item_set;

	battle_match_info _match_info;  //记录战场内的比赛信息
	std::vector<history_round_score> _history_round_score; //记录历史局的比分
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//跨服流水席战场逻辑
class cs_flow_world_manager : public bg_world_manager
{
public:
	virtual ~cs_flow_world_manager() {}

	virtual void PlayerEnter( gplayer* pPlayer, int faction); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer
	virtual void PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick, bool disconnected = false);
	virtual void OnPlayerLeave( gplayer* pPlayer, int faction, bool cond_kick);
	virtual bool IsLimitSkillInWorld(int skill_id);
	virtual bool GetCSFlowBattleInfo(int& info_seq, std::vector<player_info_flow_battle>& info, bool bForce = false );
	virtual bool IsFlowBattle() { return true; }
	virtual void FlowBattlePlayerLeave(int role_id, char faction);

	//是否是禁用物品
	bool IsForbiddenItem(int item_type);

	int GetPlayerCKillCnt(int player_id);

	int GetKillBaseScore() { return _kill_base_score; }
	int GetTargetScore() { return _target_score; }
	int GetKillMonsterScore(int tid) 
	{
		monster_score_map::iterator it = _monster_score_map.find(tid);
		return it != _monster_score_map.end() ? it->second : 0;
	}

	void SyncCSFlowPlayerInfo( int player_id, size_t content_length, const void* content);
	bool GetPlayerCSFlowInfo(int player_id, int& score, int& c_kill_cnt, int &death_cnt, int& kill_cnt, int& monster_kill_cnt, int& max_ckill_cnt, int& max_mkill_cnt, bool & battle_result_sent);

protected:
	virtual bool OnInit();

	virtual void OnCreate()
	{
		ActiveSpawn( this, CTRL_CONDISION_ID_BATTLE_BEGIN, true );
	}

	virtual void Reset()
	{
		bg_world_manager::Reset();
		_atk_fp_map.clear();
		_def_fp_map.clear();
	}

	
protected:
	struct flow_player_info 
	{
		short level;
		int   cls;
		int   zoneId;
		short score;	
		short c_kill_cnt;	//连杀值
		short monster_kill_cnt;
		short max_ckill_cnt;
		short max_mkill_cnt;
		bool battle_result_sent;
		
		flow_player_info()
		{
			level = 0;
			cls = -1;
			score = 0;
			c_kill_cnt = 0;
			zoneId = -1;
			monster_kill_cnt = 0;
			max_ckill_cnt = 0;
			max_mkill_cnt = 0;
			battle_result_sent = false;
		}
	};
	typedef abase::hash_map<int, flow_player_info, abase::_hash_function, abase::fast_alloc<> > flow_player_map;
	flow_player_map _atk_fp_map;
	flow_player_map _def_fp_map;
	
	bool InitiForbiddenItemList();  //只在本地图中禁止的物品
	abase::static_set<int, abase::fast_alloc<> > _forbidden_item_set;

	typedef abase::hash_map<int, int> monster_score_map;
	monster_score_map _monster_score_map;
	int _kill_base_score;
	int _target_score;

	char _is_delay;
	int  _delay_time;
};

#endif

