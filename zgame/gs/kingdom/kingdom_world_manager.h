#ifndef __ONLINEGAME_GS_KINGDOM_WORLD_MANAGER_H
#define __ONLINEGAME_GS_KINGDOM_WORLD_MANAGER_H 

#include "kingdom_common.h"
#include "../global_manager.h"
#include "../usermsg.h"
#include <vector>
#include <set>


class kingdom_world_manager : public global_world_manager
{
public:
	friend class kingdom_player_imp;

	kingdom_world_manager()
	{
		_kickout_time_min = 0;
		_kickout_time_max = 0;
		_kickout_close_time = 0;

		_cur_timestamp = 0;
		_start_timestamp = 0;
		_half_timestamp = 0;
		_end_timestamp = 0;

		_heartbeat_counter = 0;
		_battle_status = 0;
		_battle_result = 0;
		_battle_result2 = 0;
		_battle_lock = 0;
		memset(&_key_npc_info, 0, sizeof(_key_npc_info));

	}

	void Reset()
	{
		_cur_timestamp = 0;
		_start_timestamp = 0;
		_half_timestamp = 0;
		_end_timestamp = 0;

		_heartbeat_counter = 0;
		_battle_result = 0;
		_battle_result2 = 0;

		_mafia_info_list.clear();

		for(size_t i = 0; i < _reborn_info_list.size(); ++i)
		{
			_reborn_info_list[i].npc_id = 0;
			_reborn_info_list[i].mafia_id = 0;
			_reborn_info_list[i].is_active = false;
			_reborn_info_list[i].is_used = false;
		}
		memset(&_key_npc_info, 0, sizeof(_key_npc_info));
	}
	
	int Init( const char* gmconf_file, const char* servername, int tag, int index);

	bool IsDefenderFaction(int mafia_id) 
	{
		for(size_t i = 0; i < _mafia_info_list.size(); ++i)
		{
			if(mafia_id == _mafia_info_list[i].mafia_id && !_mafia_info_list[i].is_attacker) return true;
			
		}
		return false;
	} 
	bool IsAttackerFaction(int mafia_id) 
	{
		for(size_t i = 0; i < _mafia_info_list.size(); ++i)
		{
			if(mafia_id == _mafia_info_list[i].mafia_id && _mafia_info_list[i].is_attacker) return true;
			
		}
		return false;
	}
	
	int GetDefenderMafiaID()
	{
		for(size_t i = 0; i < _mafia_info_list.size(); ++i)
		{
			if(!_mafia_info_list[i].is_attacker)
			{
				return _mafia_info_list[i].mafia_id;
			}
		}
		return 0;
	}

	void GetAttackerMafiaID(std::vector<int> & attacker_mafia_list)
	{
		for(size_t i = 0; i < _mafia_info_list.size(); ++i)
		{
			if(_mafia_info_list[i].is_attacker) 
			{
				attacker_mafia_list.push_back(_mafia_info_list[i].mafia_id);

			}
		}
	}

	int GetBattleStatus() const {return _battle_status;}
	int GetBattleStartstamp() const {return _start_timestamp;}
	int GetBattleHalfstamp() const {return _half_timestamp;}
	int GetBattleEndstamp() const {return _end_timestamp;}
	bool IsBattleRunning() { return _battle_status ==  BS_RUNNING || _battle_status == BS_RUNNING2;}
	inline bool IsBattleWorld() { return true; }
	inline bool IsKingdomWorld() { return true; }

	int GetKickoutTimeMin() const {return _kickout_time_min;}
	int GetKickoutTimeMax() const {return _kickout_time_max;}

	virtual void Heartbeat();

	virtual void PlayerEnter( gplayer* pPlayer, int mafia_id); 
	virtual void PlayerLeave( gplayer* pPlayer, int mafia_id);
	
	virtual void OnDeliveryConnected( std::vector<kingdom_field_info>& info );
	virtual bool CreateKingdom(char fieldtype, int tag_id, int defender, std::vector<int> attacker_list); 
	virtual void StopKingdom(char fieldtype, int tag_id);
	virtual int OnPlayerLogin( const GDB::base_info * pInfo, const GDB::vecdata * data, bool is_gm );
	virtual void OnActiveSpawn(int id,bool active);
	
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
	
	//注册复活点信息
	virtual void RecordBattleNPC(gnpc_imp * pImp);

	//获得回城点 
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	//获得复活点
	virtual bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& world_tag);
	//获得随机进入点
	virtual void GetRandomEntryPos(A3DVECTOR& pos,int battle_faction);

	void GetAttackerMafiaFaction(int mafia_id, int & faction, int & enemy_faction);

	bool IsRebornActive(gplayer_imp * pImp);

	void OnChangeKeyNPCInfo(int type);
	void OnUpdateKeyNPCInfo(int cur_hp, int max_hp, int op_type, int change_hp);
	void OnQueryKeyNPCInfo(int cur_hp, int max_hp);

	void OnGetKeyNPCInfo(int & cur_hp, int & max_hp, int & hp_add, int & hp_dec);

	virtual int OnMobDeath(world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnPlayerDeath( int killer, int deadman, int deadman_battle_faction, bool& kill_much );

	virtual void OnMineStart(int roleid, int mafia_id, int mine_id);
	virtual void OnMineEnd(int roleid, int mafia_id, int item_id);

	void KingTryCallGuard(gplayer_imp * pImp);

private:
	enum BATTLE_RESULT
	{
		BR_NONE		=	0,	// 无结果
		BR_DEFENDER_WIN =	1,	// 守方获胜
		BR_ATTACKER_WIN =	2,	// 攻方获胜
	};


	enum BATTLE_STATUS
	{
		BS_NONE 	= 0,		// 无
		BS_READY	= 1,		// 战斗准备完毕
		BS_RUNNING 	= 2,		// 战斗运行 (第一阶段, 4个进攻方对1个防守方)
		BS_RUNNING2	= 3,		// 战斗运行 (第二阶段, 进攻方之间互相争夺)
		BS_CLOSING 	= 4,		// 战斗关闭
	};

	enum BATTLE_FACTION
	{       
		BF_NONE                 = 0,    //无阵营
		BF_ATTACKER             = 1,    //攻击方
		BF_DEFENDER             = 2,    //防守方
	};  
	
	enum
	{
		DEFAULT_KICKOUT_TIME_MIN = 3,
		DEFAULT_KICKOUT_TIME_MAX = 15,
		DEFAULT_KICKOUT_CLOSE_TIME = 30,

		KEY_MINE_ID = 55849,
		KEY_MINE_ITEM_ID = 55856,
		KINGDOM2_TAG_ID = 151,
		OPEN_CTRL_ID1 = 12834,		//第一场是对怪物需要开控制器id
		OPEN_CTRL_ID2 = 12835,		//其他场次开启的控制器id


	};

	struct kingdom_player_info
	{
		int roleid;
		int cs_sid;
		int cs_index;
		int kill_count;
		int death_count;
		int mafia_id;
		bool is_online;
	};

	//复活点信息
	struct kingdom_mafia_info
	{
		int mafia_id;
		int mafia_score;
		int kill_count;
		int death_count;
		bool is_attacker;
		std::vector<kingdom_player_info> player_list;
	};

	struct reborn_info
	{
		int seq;			//第几个复活点 (通过这个产生阵营)
		A3DVECTOR reborn_pos;
		int tid;
		int npc_id;
		int mafia_id;
		bool is_attacker;		//是否是攻方复活点
		bool is_used;			//这个复活点的信息是否被使用
		bool is_active;			//这个复活点是否激活(被摧毁后就关闭)
	};

	struct key_npc_info
	{
		int npc_id;
		int cur_hp;
		int max_hp;
		int hp_add;
		int hp_dec;
		bool is_alive;
	};

private:
	void OnInit();	
	void OnReady();
	void OnCreate();
	void OnRunning();
	void OnRunning2();
	void OnClosing();

	void Close();
	bool GetMafiaRebornInfo(int mafia_id, bool is_attacker);
	void GetActiveRebornAttackerMafia(std::vector<int> & mafia_id_list);
	void SyncMafiaInfo();
	void SyncKeyNPCInfo();
	void InitRebornPos();
	void ResetRebornFaction();
	void GetFactionBySeq(int seq, int & faction, int & enemy_faction);

private:
	int _cur_timestamp;		//流程使用时间戳
	int _start_timestamp;		//战场开启时间戳
	int _half_timestamp;		//半场结束时间戳
	int _end_timestamp;		//战争结束时间戳
	
	int _kickout_time_min;		//踢出最小时间
	int _kickout_time_max;		//踢出最大时间
	int _kickout_close_time;	//战场关闭时间
	
	int _battle_status;
	int _battle_lock;
	int _battle_result;		//第一场比赛结果(攻方或者守方胜利)
	int _battle_result2;		//第二场比赛结果(胜利的帮派id)
	int _heartbeat_counter;		//心跳计时

	cs_user_map  _all_list;

	std::vector<reborn_info> _reborn_info_list;
	std::vector<kingdom_mafia_info> _mafia_info_list;

	key_npc_info _key_npc_info;
};	

#endif
