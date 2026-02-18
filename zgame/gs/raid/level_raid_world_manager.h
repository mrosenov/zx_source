#ifndef __LEVEL_RAID_WORLD_MANAGER_
#define __LEVEL_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"
#include <amemobj.h>

#include "../config.h"
#include "../template/exptypes.h"
#include "../general_indexer.h"
#include <string>

enum LEVEL_CONDITION_TYPE
{
	LEVEL_CONDITION_TYPE_NPCKILL = 0,
	LEVEL_CONDITION_TYPE_GATHER,
	LEVEL_CONDITION_TYPE_DEATH,
	LEVEL_CONDITION_TYPE_TIME,
	LEVEL_CONDITION_TYPE_USEHPPOTION,
	LEVEL_CONDITION_TYPE_USEMPPOTION,
	LEVEL_CONDITION_TYPE_GLOBALVALUE,
	LEVEL_CONDITION_TYPE_CTRLACTIVE,
	LEVEL_CONDITION_NUM,
};

class level_condition : public abase::ASmallObject
{
protected:
	int _ctrl_id;
	int _type;
	
protected:
	level_condition(int ctrl_id, int type) : _ctrl_id(ctrl_id), _type(type) {} 

public:
	virtual ~level_condition() {}
	virtual void Start() {}
	virtual void SetValue(int v){}
	virtual void IncValue(int v){}
	virtual void IncValue(int id, int v){}
	virtual void DecValue(int v){}
	virtual void DecValue(int id, int v){}
	virtual int GetValue() const { return 0; }
	virtual bool Meet() const = 0;
};

class level_condition_npc_kill : public level_condition
{
private:
	int _npc_tid;
	int _kill_max;
	int _kill_min;
	int _kill_cnt;

public:
	level_condition_npc_kill(int ctrl_id, int npc_tid, int max, int min)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_NPCKILL), _npc_tid(npc_tid), _kill_max(max), _kill_min(min), _kill_cnt(0)
	{
	}
	virtual ~level_condition_npc_kill(){}

	void IncValue(int id, int offset)
	{
		if(id == _npc_tid)
		{
			_kill_cnt += offset;
		}
	}


	bool Meet() const
	{
		return _kill_cnt <= _kill_max && _kill_cnt >= _kill_min;
	}
};

class level_condition_gather : public level_condition
{
private:
	int _matter_tid;
	int _gather_max;
	int _gather_min;
	int _gather_cnt;

public:

	level_condition_gather(int ctrl_id, int matter_tid, int max, int min)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_GATHER), _matter_tid(matter_tid), _gather_max(max), _gather_min(min), _gather_cnt(0)
	{
	}
	virtual ~level_condition_gather(){}

	void IncValue(int mTid, int offset)
	{
		if(mTid == _matter_tid)
		{
			_gather_cnt += offset;
		}
	}
	
	virtual int GetValue() const 
	{ 
		return _gather_cnt; 
	}

	bool Meet() const
	{
		return _gather_cnt <= _gather_max && _gather_cnt >= _gather_min;
	}
};

class level_condition_death : public level_condition
{
private:
	int _max_death_cnt;
	int _death_cnt;

public:
	level_condition_death(int ctrl_id, int max_death_cnt)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_DEATH), _max_death_cnt(max_death_cnt), _death_cnt(0)
	{
	}

	virtual ~level_condition_death() {}

	void IncValue(int v)
	{
		_death_cnt += v;
	}

	void DecValue(int v) {}

	bool Meet() const
	{
		return _death_cnt < _max_death_cnt;
	}
};

class level_condition_time : public level_condition
{
private:
	int _time;
	int _start_time;

public:
	level_condition_time(int ctrl_id, int time) 
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_TIME), _time(time), _start_time(0)
	{
	}

	virtual ~level_condition_time(){}

	void Start()
	{
		_start_time = g_timer.get_systime();
	}

	virtual int GetValue() const 
	{
		return _start_time; 
	}

	bool Meet() const
	{
		return g_timer.get_systime() - _start_time < _time;	
	}

};

class level_condition_use_hp_potion : public level_condition
{
private:
	int _max_use;
	int _use;

public:
	level_condition_use_hp_potion(int ctrl_id, int max_use)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_USEHPPOTION), _max_use(max_use), _use(0)
	{
	}

	virtual ~level_condition_use_hp_potion(){}

	void IncValue(int v)
	{
		_use += v;
	}

	bool Meet() const
	{
		return _use <= _max_use;
	}
};

class level_condition_use_mp_potion : public level_condition
{
private:
	int _max_use;
	int _use;

public:
	level_condition_use_mp_potion(int ctrl_id, int max_use)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_USEMPPOTION), _max_use(max_use), _use(0)
	{
	}

	virtual ~level_condition_use_mp_potion(){}

	void IncValue(int v)
	{
		_use += v;
	}

	bool Meet() const
	{
		return _use <= _max_use;
	}
};

class level_condition_global_value : public level_condition
{
private:
	int _global_var;
	int _max;
	int _min;
	int _world_tag;
	int _world_index;

public:
	level_condition_global_value(int ctrl_id, int global_var, int max, int min, int world_tag, int world_index)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_GLOBALVALUE), _global_var(global_var), _max(max), _min(min), _world_tag(world_tag), _world_index(_world_index)
	{
	}

	virtual ~level_condition_global_value(){}

	bool Meet() const
	{
		GeneralIndexer indexer;
		indexer.SetParam(_world_tag, _world_index, 0);
		int value = 0;
		indexer.Get(_global_var, value);
		return value <= _max && value >= _min;
	}
};

class level_condition_ctrl_active : public level_condition
{
private:
	int _id;
	bool _active;

public:
	level_condition_ctrl_active(int ctrl_id, int target_ctrl_id)
		: level_condition(ctrl_id, LEVEL_CONDITION_TYPE_CTRLACTIVE), _id(target_ctrl_id), _active(false)
	{
	}

	virtual ~level_condition_ctrl_active(){}

	void SetValue(int v)
	{
		_active = (_id == v);
	}

	bool Meet() const
	{
		return _active;
	}
};

class level_raid_world_manager : public raid_world_manager
{       
protected:
	int _level_config_tid;

	int  _final_ctrl;
	level_condition* _final_conds[LEVEL_CONDITION_NUM];	
	bool _final_result;
	int  _final_win_ctrl_id;
	
	int _curr_level_ctrl;
	int _curr_level_index;
	level_condition* _conds[LEVEL_CONDITION_NUM];
	bool _level_result[10];
	int _curr_level_win_ctrl_id;
	unsigned char _start_hidden_level;  // Youshuang add
	
public:
	level_raid_world_manager();
	virtual ~level_raid_world_manager() {}
	virtual int GetRaidType() { return RT_LEVEL; }

	virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual void Reset();
	virtual void OnActiveSpawn(int id,bool active);
	virtual void OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void OnKillMonster(gplayer* pPlayer, int monster_tid, int faction);	
	virtual void OnUsePotion(gplayer* gPlayer, char type, int value);
	virtual void OnGatherMatter(gplayer* pPlayer, int matter_tid, int cnt);

	void GetRaidLevelResult(bool& final, bool level_result[10]);
	int  GetRaidLevelConfigTid();
	void SendRaidLevelInfo(bool final); 
	void SendRaidLevelInfo(int playerId, bool final);
	virtual void OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction);
	// Youshuang add
	virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual std::vector<raid_boss_info> GetKilledBossList() const;
	// end

protected:
	void AddLevelConditions(int ctrl_id);
	void AddLevelConditions(const CHALLENGE_2012_CONFIG::CONFIG_TMPL& config, bool final);
	void ClearFinalConditions();
	void ClearLevelConditions();

	void SendRaidLevelStatusChange(int level, bool start, bool result);
	void SendRaidLevelStatusChange(int playerid, int level, bool start, bool result);
	virtual bool OnInit();
	virtual void OnCreate();
	virtual int OnDestroyNormalBuilding(int faction);
	virtual void OnClosing();

	std::string GetAllPlayerIdsStr();
};

#endif /*__LEVEL_RAID_WORLD_MANAGER_*/
