#ifndef __ONLINEGAME_GS_RAID_WORLD_MANAGER_H__ 
#define __ONLINEGAME_GS_RAID_WORLD_MANAGER_H__

#include "../global_manager.h"
#include "../world.h"
#include "../usermsg.h"
#include "../raid_controller.h"
#include <vector>
#include <set>

enum
{
	CHAT_RAID = 27,
};

// 控制器的问题
enum
{
	// 使用这个基值来计算出实际在战场中使用的id
	CTRL_ID_RAID_BASE 							= 80000,	// 基值
	
	CTRL_CONDISION_ID_RAID_MIN 					= 70204,	// 最小战场控制器id
	CTRL_CONDISION_ID_RAID_BEGIN 				= 70204,	// 战场开启，刷出普通建筑物

	CTRL_CONDISION_ID_RAID_ATTACKER_KEY 		= 70225,	// 刷出攻方key building
	CTRL_CONDISION_ID_RAID_DEFENDER_KEY 		= 70226,	// 刷出守方key building

	CTRL_CONDISION_ID_RAID_TEAM_CRSSVR_BEGIN 	= 70251, 	//跨服小队PK,第一小场
	CTRL_CONDISION_ID_RAID_TEAM_CRSSVR_END   	= 70255, 	//跨服小队PK,最后一个小场	
	CTRL_CONDISION_ID_RAID_MAX					= 70700,	// 最大战场控制器id

	CTRL_CONDISION_ID_RAID_OFFSET = CTRL_CONDISION_ID_RAID_MAX - CTRL_CONDISION_ID_RAID_MIN + 1,
}; 

enum RAID_RESULT
{
	RAID_ATTACKER_WIN 	=	1,	// 攻方获胜
	RAID_DEFENDER_WIN 	=	2,	// 守方获胜
	RAID_DRAW 			=	3,	// 平手
};

enum RAID_FACTION
{       
	RF_NONE                 = 0,    //无阵营
	RF_ATTACKER             = 1,    //攻击方
	RF_DEFENDER             = 2,    //防守方
	RF_VISITOR				= 3,    //观察者 
};  

enum RAID_TYPE
{
	RT_COMMON				= 0,	//普通副本
	RT_TOWER,
	RT_COLLISION,
	RT_MAFIA_BATTLE,
	RT_LEVEL,
	RT_COLLISION2,
	RT_TRANSFORM,
	RT_STEP,
	RT_SEEK,
	RT_CAPTURE,
};

struct player_raid_info
{
	player_raid_info(): cls(0),level(0),reborn_cnt(0),timestamp(0)
	{
	}
	player_raid_info( int c, int l, int sl):
		cls(c), level(l), reborn_cnt(sl)
	{
	}
	player_raid_info( int c, int l, int sl, int ti):
		cls(c), level(l), reborn_cnt(sl), timestamp(ti) 
	{
	}

	short cls;
	short level;
	short reborn_cnt;	
	int timestamp;
};

typedef S2C::CMD::raid_info::player_info_in_raid player_info_raid;

// Youshuang add
enum LEVEL_OP
{
	LEVEL_NONE = -1,
	LEVEL_OR,
	LEVEL_AND,
	LEVEL_ALL,
};

struct raid_boss_info
{
	raid_boss_info() : tid( -1 ), is_killed( 0 ), achievement( 0 ), cur_num(0), max_num(0){}
	raid_boss_info( int id, unsigned char killed, unsigned char achieve ) 
		: tid( id ), is_killed( killed ), achievement( achieve ), cur_num(0), max_num(0){}
	int tid;
	int level;
	unsigned char is_killed;
	unsigned char achievement;
	unsigned char cur_num;
	unsigned char max_num;
};

struct raid_level_info
{
	int howto;
	std::vector<raid_boss_info*> boss;
};
// end

class raid_world_manager: public global_world_manager
{
protected:
	int _raid_id;			// 战场id,两个world对应一个id,保证切换使用
	//int _raid_type;			// 战场类型
	int _raid_result;
	int _raid_lock;

	int _max_player_count;		// 最大玩家限制, 每方
	int _need_level_min;		// 进入玩家的等级限制，最小级别
	int _need_level_max;		// 进入玩家的等级限制，最大级别
	
	int _raid_time;         	// 副本时间,0为无限时间
	int _invincible_time;		// 玩家传入副本后无敌等待时间
	int _close_raid_time;		// 关闭副本的时间
	int _player_leave_time;		// 玩家在副本关闭后还能停留的时间

	int _win_ctrl_id;			// 副本胜利条件控制器ID，控制器激活，副本结束
	int _win_task_id;			// 副本胜利奖励任务id
	int _forbidden_items_id[10];// 禁用物品ID列表
	int _forbidden_skill_id[10];// 禁用技能ID列表
	
	int _cur_timestamp;			// 流程使用时间戳
	int _kill_monster_timestamp;// 杀死所有怪物的时间
	int _end_timestamp;			// 副本结束时间戳
	int _start_timestamp;		// 副本开启时间戳

	int _heartbeat_counter;		// 心跳计时
	int _raid_info_seq;			// 发送信息序列
	int _update_info_seq;		// 更新信息序列 
	int _apply_count;			// 报名人数

	int _status;				// 战争状态
	int _raid_winner_id;		// 副本胜利者ID
	int _max_killer_id;			// 杀人最多的人

	int _attacker_score;		// 攻方积分
	int _defender_score;		// 守方积分

	int _raidroom_id;			//delivery使用的房间id，delivery做查询用
	int _difficulty;
	int _difficulty_ctrl_id;		//副本难度控制器id
	
	cs_user_map  _defender_list;
	cs_user_map  _attacker_list;
	cs_user_map  _all_list;
	
	std::vector<int> _raid_member_list;	//Delivery处报名该副本成员ID，用于进入检测
	// Youshuang add
	std::vector<raid_level_info> _level_info;
	std::multimap<int, raid_boss_info*> _tid_2_bossinfo;
	// end
	int _seek_heartbeat_counter; //心跳计数

	enum
	{
		RS_READY 		= 0,	// 服务器准备完毕
		RS_PREPARING 	= 1,	// 战斗准备状态(暂时不用)
		RS_RUNNING 		= 2,	// 战斗开始状态
		RS_CLOSING 		= 3,	// 战斗关闭，清理世界管理数据
		RS_IDLE	  		= 4,	// 世界管理器处于空闲状态，可被新的副本实例所使用
	};

	struct building_data
	{
		int normal_building_init;	// 初始化个数
		int key_building_init;		// 初始化个数
		int normal_building_left;	// 剩余普通建筑物个数
	};

	typedef std::map<int,player_raid_info> PlayerInfoMap;
	typedef std::map<int,player_raid_info>::iterator PlayerInfoIt;

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
		std::vector<A3DVECTOR> town_list;      //回城点
		std::vector<A3DVECTOR> entry_list;     //进入点
		CtrlReviveMap revive_map;              //复活点
		int kill_count;

		void Clear()
		{
			player_info_map.clear();
			memset(&buildings, 0, sizeof(building_data));
			first_kill = false;
			town_list.clear();
			entry_list.clear();
			revive_map.clear();
			kill_count = 0;
		}
	};

	competitor_data _attacker_data;
	competitor_data _defender_data;
	bool _all_monster_killed;

	std::vector<int> _global_var_list;
	
public:
	void Heartbeat();
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );
	void PostPlayerLogin( gplayer* pPlayer ) {}
	void RecordRaidBuilding( const A3DVECTOR &pos, int faction, int tid, bool is_key_building = false );

public:
	raid_world_manager();
	virtual ~raid_world_manager();

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

	// 副本地图返回true
	inline bool IsRaidWorld() { return true; }	
	inline bool IsRaidRunning() { return ( RS_RUNNING == _status ); }
	inline int GetRaidID() { return _raid_id; }
	inline int GetLevelMin() { return _need_level_min; }
	inline int GetLevelMax() { return _need_level_max; }
	inline int GetRaidResult() { return _raid_result; }
	inline int GetRaidMaxKillerID() { return _max_killer_id; }
	inline int GetRaidWinnerID(){return _raid_winner_id; }
	inline int GetPlayerLeaveTime() { return _player_leave_time; }
	inline int GetRaidStatus() { return _status; }
	inline int GetRaidStartstamp() { return _start_timestamp; }
	inline int GetRaidEndstamp() { return _end_timestamp; }
	inline int GetWinTaskID() { return _win_task_id; }
	inline int GetRaidRoomID() { return _raidroom_id; }
	inline int GetRaidDifficulty() { return _difficulty;}

	void SetRaidResult(int result); 

public:
 	virtual int GetRaidType() { return RT_COMMON;}
	
	//return 0, 可以创建；-1，world_manager当前被占用, 选择下一个可用的WM；-2，副本data检查错误；失败
	virtual int TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	void ForceCloseRaid();
	void Release();			//释放副本资源，重置副本状态，不要直接调用，关闭副本调用CloseRaid
	virtual void Reset();

	// 统一做吧
	void HandleModifyRaidDeath( int player_id, int death_count, size_t content_length, const void * content);
	void HandleModifyRaidKill( int player_id, int kill_count, size_t content_length, const void * content);
	void HandleSyncRaidInfo( int player_id, int reserver, size_t content_length, const void * content);
	virtual void HandleTransformRaid(int player_id) {}
	virtual void HandleCaptureRaidSyncPos(char faction, const A3DVECTOR& pos) {}

	//副本频道
	// type 0 第一滴血, 1 杀神, 2 阻止
	void BroadcastRaidMsg( unsigned char type, unsigned char raid_faction, int battle_id, int gs_id, int killer, int deadman );
	void RaidFactionSay( int faction ,const void* buf, size_t size, int channel, const void *aux_data, size_t dsize, int char_emote, int self_id);
	void RaidSay( const void* buf, size_t size ){}
	int TranslateCtrlID( int which );
	//int GetPostion(int id, int score);

	bool GetRaidInfo( int& info_seq, std::vector<player_info_raid>& info, bool bForce = false);
	void GetRaidUseGlobalVars(std::vector<int>& var_list);
	
 	size_t GetRaidUseGlobalVarCnt();
	int GetRaidUseGlobalVar(size_t index);


public:
	virtual int GetClientTag() { return _raid_id; } //副本中 raid_id 就是客户端所用地图号
	virtual bool SaveCtrlSpecial() { return true; } //是否为不同实例单独保存控制器状态 副本和帮派基地返回 true
	virtual void PlayerEnter( gplayer* pPlayer, int faction, bool reenter); //Faction: 1 attacker, 2 defneder 3 visitor 4 meleer
	virtual void PlayerLeave( gplayer* pPlayer, int faction,bool cond_kick);
	virtual void PlayerEnterWorld( gplayer* pPlayer, int faction, bool reenter);

	//注册回城点
	virtual void RecordTownPos(const A3DVECTOR& pos,int faction);
	//注册复活点
	virtual void RecordRebornPos(const A3DVECTOR& pos,int faction,int cond_id);
	//注册进入点
	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	//获得回城点 
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	//获得复活点
	virtual bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag) { return false; }
	//获得随机进入点
	virtual void GetRandomEntryPos(A3DVECTOR& pos,int raid_faction);

	virtual int GetRaidTotalScore()
	{
		return _attacker_score + _defender_score;
	}

	virtual int GetAttackerScore() { return _attacker_score;}
	virtual int GetDefenderScore() { return _defender_score;}
	
	// Youshuang
	virtual std::vector<raid_boss_info> GetKilledBossList() const;
	raid_boss_info* GetBossByID( int tid, unsigned char killed = 1 ) const;
	virtual void OnMobReborn( int tid );
	int NotifyBossBeenKilled();
	// end
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void OnKillMonster(gplayer* pPlayer, int monster_tid, int faction){}
	virtual void OnUsePotion(gplayer* pPlayer, char type, int value) {}
	virtual void OnGatherMatter(gplayer* pPlayer, int matter_tid, int cnt) {}

	virtual int  GetRoundStartstamp() { return 0; }
	virtual bool IsRoundRunning() { return false; }
	virtual void UpdateRoundScore(int raid_faction, int change) {}
	virtual bool IsLimitSkillInWorld(int skill_id);
	virtual bool IsForbiddenItem(int item_type);

	virtual bool IsIdle() const { return _status == RS_IDLE; } 
	virtual bool CanEnterWorld() const { return _status == RS_RUNNING; }
	virtual void OnActiveSpawn(int id,bool active);
/*
	virtual INIT_RES OnInitMoveMap(path_finding::PathfindMap* & mm);
	virtual INIT_RES OnInitCityRegion(city_region* & cr);
	virtual INIT_RES OnInitBuffArea(buff_area* & ba); 
	virtual INIT_RES OnInitPathMan(path_manager* & pm); 
	*/
	virtual INIT_RES OnInitMoveMap();
	virtual bool OnReleaseMoveMap() { return true; }
	virtual INIT_RES OnInitCityRegion();
	virtual bool OnReleaseCityRegion() { return true; }
	virtual INIT_RES OnInitBuffArea();
	virtual bool OnReleaseBuffArea() { return true; }
	virtual INIT_RES OnInitPathMan();
	virtual bool OnReleasePathMan() { return true; }

	virtual bool OnTraceManAttach();
	virtual void OnDisconnect(); //与 gdeliveryd 断开连接时的处理
protected:
	virtual bool OnInit(){return true;}		
	virtual void OnCreate();
	virtual void OnRunning();
	virtual void OnClosing();
	virtual void OnReady();
	virtual void OnClose();  //状态不是Running则立即调用
	
	virtual int OnDestroyKeyBuilding( int faction);
	virtual int OnDestroyNormalBuilding( int faction){return 0;}
	virtual void OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction) {}
	virtual void OnPlayerEnterWorld(gplayer* pPlayer, bool reenter, int faction) {}
	virtual void OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction) {}
	virtual void OnRaidInfoChange(){}
	virtual void KillAllMonsters();
	
protected:
	void SendRaidInfo();
	void DumpWorldMsg();
	void UpdatePlayerInfo(PlayerInfoIt &it, size_t content_length, const void * content);	
};

////////////////////////////////////////////////////////////////////////////////////////////////////
//普通副本逻辑
////////////////////////////////////////////////////////////////////////////////////////////////////
class cr_world_manager : public raid_world_manager
{       
protected:
	virtual bool OnInit();
	virtual void OnCreate();
	virtual int OnDestroyNormalBuilding(int faction);
	virtual void OnClosing();

	// Youshuang add
public:
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	// end
};
#endif

