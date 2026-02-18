#ifndef __ONLINEGAME_GS_TR_WORLD_MANAGER_H_
#define __ONLINEGAME_GS_TR_WORLD_MANAGER_H_

#include "../usermsg.h"
#include "../global_manager.h"

struct territory_field_info
{
	int battle_type;
	int battle_world_tag;
	int player_limit;
};

class tr_world_manager : public global_world_manager
{
private:
	//静态信息
	int _max_player_limit;		// 双方最大玩家数量限制
	int _min_player_level;		// 玩家最低等级
	int _max_player_level;		// 玩家最高等级

	int _kickout_time_min;		// 踢出最小时间
	int _kickout_time_max;		// 踢出最大时间
	int _kickout_close_time;	// 战场关闭时间

	int _battle_type;		// 战场类型

	//动态信息
	int _cur_timestamp;		// 流程使用时间戳
	int _start_timestamp;		// 战场开启时间戳
	int _end_timestamp;		// 战争结束时间戳

	int _heartbeat_counter;		// 心跳计时
	int _battle_info_seq;		// 发送信息序列
	int _update_info_seq;		// 更新信息序列 

	int _battle_id;			// 战场对应的区域id
	int _battle_status;		// 战场状态
	int _battle_result;		// 战场结果
	int _battle_lock;		// 战场锁

	cs_user_map  _defender_list;	// 守方列表
	cs_user_map  _attacker_list;	// 攻方列表
	cs_user_map  _all_list;

	struct player_battle_info
	{
		player_battle_info() : battle_faction(0), kill(0), death(0){}
		int battle_faction;
		int kill;
		int death;
	};
	typedef std::map<int,player_battle_info> PlayerInfoMap;
	typedef std::map<int,player_battle_info>::iterator PlayerInfoIt;
		
	struct revive_pos_t
	{
		A3DVECTOR pos;
		bool active;
	};
	typedef std::map<int,std::vector<revive_pos_t> > CtrlReviveMap;
	typedef std::map<int,std::vector<revive_pos_t> >::iterator CtrlReviveMapIt;

	struct battle_data
	{
		int mafia_id;
		PlayerInfoMap player_info_map;		      //玩家信息
		std::vector<A3DVECTOR> town_list;             //回城点
		std::vector<A3DVECTOR> entry_list;            //进入点
		CtrlReviveMap revive_map;                     //复活点
	};
	battle_data _attacker_data;
	battle_data _defender_data;

	enum BATTLE_TYPE
	{
		BT_NONE 	= 0,
		BT_HIGH 	= 1,		//高级战场
		BT_MIDDLE 	= 2,		//中级战场
		BT_LOW 		= 3,		//低级战场
	};

	enum BATTLE_RESULT
	{
		BR_NONE		=	0,	// 无结果
		BR_ATTACKER_WIN =	1,	// 攻方获胜
		BR_DEFENDER_WIN =	2,	// 守方获胜
	};


	enum BATTLE_STATUS
	{
		BS_NONE 	= 0,		// 无
		BS_READY	= 1,		// 战斗准备完毕
		BS_RUNNING 	= 2,		// 战斗运行
		BS_CLOSING 	= 3,		// 战斗关闭
	};

	enum BATTLE_FACTION
	{       
		BF_NONE                 = 0,    //无阵营
		BF_ATTACKER             = 1,    //攻击方
		BF_DEFENDER             = 2,    //防守方
	};  
	
	enum
	{
		DEFAULT_MAX_PLAYER_COUNT = 100,

		DEFAULT_KICKOUT_TIME_MIN = 3,
		DEFAULT_KICKOUT_TIME_MAX = 15,
		DEFAULT_KICKOUT_CLOSE_TIME = 30,

		TERRITORY_WORLD_TAG_BEGIN = 501,
		CTRL_ID_BATTLE_BASE =			90000,	// 基值
		CTRL_CONDISION_ID_BATTLE_MIN =		80000,	// 最小战场控制器id
		CTRL_CONDISION_ID_BATTLE_PVE_BEGIN =	80052,	// PVE战场开启
		CTRL_CONDISION_ID_BATTLE_PVP_BEGIN =	80053,	// PVP战场开启
		CTRL_CONDISION_ID_BATTLE_MAX =		81000,	// 最大战场控制器id
		CTRL_CONDISION_ID_BATTLE_OFFSET = CTRL_CONDISION_ID_BATTLE_MAX - CTRL_CONDISION_ID_BATTLE_MIN + 1,
	};

	friend class tr_player_imp;

public:
	tr_world_manager()
	{
		_max_player_limit = 0;
		_min_player_level = 0;
		_max_player_level = 0;
		_kickout_time_min = 0;
		_kickout_time_max = 0;
		_kickout_close_time = 0;
		_battle_type = 0;

		_cur_timestamp = 0;
		_start_timestamp = 0;
		_end_timestamp = 0;
		_heartbeat_counter = 0;
		_battle_info_seq = 0;
		_update_info_seq = 0;
		_battle_id = 0;
		_battle_status = 0;
		_battle_result = 0;
		_battle_lock = 0;
		
		_attacker_data.mafia_id = 0;
		_defender_data.mafia_id = 0;
	}

	~tr_world_manager()
	{
	}

	int Init( const char* gmconf_file, const char* servername, int tag, int index);

	void Reset()
	{
		_cur_timestamp = 0;
		_start_timestamp = 0;
		_end_timestamp = 0;
		_heartbeat_counter = 0;
		_battle_info_seq = 0;
		_update_info_seq = 0;
		_battle_id = 0;
		_battle_result = 0;
		_battle_lock = 0;

		_defender_list.clear();
		_attacker_list.clear();
		_all_list.clear();

		_attacker_data.mafia_id = 0;
		_defender_data.mafia_id = 0;

		_attacker_data.player_info_map.clear();
		_defender_data.player_info_map.clear();
		
	}

	inline bool IsBattleWorld() { return true; }
	int GetLevelMin() const { return _min_player_level;}
	int GetLevelMax() const { return _max_player_level;}
	int GetKickoutTimeMin() const {return _kickout_time_min;}
	int GetKickoutTimeMax() const {return _kickout_time_max;}
	int GetBattleType() const {return _battle_type;}
	int GetBattleID() const {return _battle_id;}
	int GetBattleStatus() const {return _battle_status;}
	int GetBattleResult() const {return _battle_result;}
	int GetAttackerMafiaID() const {return _attacker_data.mafia_id;}
	int GetDefenderMafiaID() const {return _defender_data.mafia_id;}
	int GetBattleStartstamp() const {return _start_timestamp;}
	int GetBattleEndstamp() const {return _end_timestamp;}
	int GetBattleCtrlID() const { return _defender_data.mafia_id == 0 ? CTRL_CONDISION_ID_BATTLE_PVE_BEGIN : CTRL_CONDISION_ID_BATTLE_PVP_BEGIN;}
	void SendBattleInfo() {++_battle_info_seq;}
	bool IsBattleRunning() { return _battle_status ==  BS_RUNNING;}
	void ChangeBattleResult(int result)
	{
		spin_autolock keeper( _battle_lock );
		_battle_result = result;
		SendBattleInfo();
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

	virtual bool CreateBattleTerritory(int battle_id, int tag_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp);
	
	virtual void Heartbeat();

	virtual void PlayerEnter( gplayer* pPlayer, int faction); 
	virtual void PlayerLeave( gplayer* pPlayer, int faction);
	
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );
	virtual void OnDeliveryConnected( std::vector<territory_field_info>& info );
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnActiveSpawn(int id,bool active);
	
	virtual void BattleFactionSay( int faction ,const void* buf, size_t size, int channel = 0, const void *aux_data = 0, size_t dsize =0, int char_emote =0, int self_id =0);
	virtual void BattleSay( const void* buf, size_t size ){ }
	virtual int TranslateCtrlID( int which );
	virtual void ActiveCtrlIDLog(int which, bool active);

	void DumpWorldMsg();

private:
	void OnInit();	
	void OnReady();
	void OnCreate();
	void OnRunning();
	void OnClosing();

	virtual int OnDestroyKeyBuilding( int faction);
	virtual void OnPlayerLeave( gplayer* pPlayer);

};




#endif //__ONLINEGAME_GS_TR_WORLD_MANAGER_H_
