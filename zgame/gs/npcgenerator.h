#ifndef __NETGAME_GS_NPC_GENERATOR_H__
#define __NETGAME_GS_NPC_GENERATOR_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>

#include "property.h"
#include "playertemplate.h"
#include "template_loader.h"
#include "timesegment.h"

class gplayer_imp;
struct pet_data;
class pet_bedge_essence;
struct gnpc;
class world;
class gnpc_imp;
class gmatter_mine_imp;
class service_npc;
class mob_active_imp;


enum 
{
	ROLE_IN_WAR_MAIN_BUILDING = 1,
	ROLE_IN_WAR_TURRET = 4,
	ROLE_IN_WAR_TOWN_POS = 5,
	ROLE_IN_WAR_REBORN_POS = 6,
	ROLE_IN_WAR_BATTLE_SERVICE_NPC = 7,
	ROLE_IN_WAR_BATTLE_FLAG = 8,
	ROLE_IN_WAR_BATTLE_ENTRY = 9,
	ROLE_IN_WAR_BATTLE_KEY_BUILDING = 10,
	ROLE_IN_WAR_BATTLE_NORMAL_BUILDING = 11,
};

enum
{
	// 动态物品id
	// 门
	DYNOJB_ID_DOOR = 5,
};

class base_spawner
{
public:
	class generate_pos : public substance
	{
		protected:
		A3DVECTOR _pos_min;				//最小的坐标位置
		A3DVECTOR _pos_max;				//最大的坐标位置
		char	  _dir;
		char	  _dir1;
		char	  _rad;

		int 	_collision_id;

		private:
		virtual void Generate(A3DVECTOR &pos,float offset,world_manager* manager)=0;
		
		public:
		generate_pos(const A3DVECTOR &pos_min, const A3DVECTOR &pos_max, char dir,char dir1,char rad)
			:_pos_min(pos_min),_pos_max(pos_max),_dir(dir),_dir1(dir1),_rad(rad),_collision_id(0)
			{}

			inline void GeneratePos(A3DVECTOR &pos,char & dir, float offset,world_manager* manager, char * dir1,char * rad)
			{
				if(_pos_min.squared_distance( _pos_max) < 1e-3)
				{
					dir = _dir;
					if(dir1) *dir1 = _dir1;
					if(rad) *rad = _rad;
				}
				else
				{
					dir = abase::Rand(0,255);
					if(dir1) *dir1 = 0;
					if(rad)  *rad = 0;
				}
				Generate(pos, offset, manager);
			}
			virtual float GenerateY(float x, float y,float z, float offset,world_manager* manager) = 0;
			
			inline void GetDir(char &dir0,char &dir1,char& rad) 
			{
				dir0 = _dir;
				dir1 = _dir1;
				rad = _rad;
			}

			inline void SetCollisionID(int id)
			{
				_collision_id = id;
			}

			inline int GetCollisionID()
			{
				return _collision_id;
			}
	};
	base_spawner():_region(0,0,0,0),_spool_lock(0), _auto_respawn(1),_is_spawned(false),_phase_id(0)
		       {}
protected:
	rect	_region;
	//generate_pos * _pos_generator;
	abase::vector<generate_pos* , abase::fast_alloc<> > _pos_generator;
	int _spool_lock;					//这是可能修改NPC池的操作时加的锁
	abase::static_multimap<XID,int, abase::fast_alloc<> > _xid_list;
	char	_auto_respawn;				// 0,不重生 1,自动重生(死亡重生) 2, 互动过程开启重生(互动物品专用)
	bool	_is_spawned;				//是否有效
	int 	_phase_id;
	virtual ~base_spawner() { abase::clear_ptr_vector(_pos_generator);}
	virtual void Release() = 0;
public:
//	char _dir;					//保存模板里的方向，供NPC使用  和 物品
//	char _dir1;
//	char _rad;
	/*
	void SetDir(unsigned char dir0,unsigned char dir1, unsigned char rad)
	{
		_dir = dir0;
		_dir1 = dir1;
		_rad = rad;
	}*/
	virtual void GeneratePos(A3DVECTOR &pos,char & dir, float offset_terrain,world_manager * manager, int * collision_id , char * dir1= NULL, char * rad = NULL)
	{
		generate_pos * pGen = NULL;
		if(_pos_generator.size() > 1)
		{
			pGen = _pos_generator[abase::Rand(0, _pos_generator.size() -1)];
		}
		else
		{
			pGen = _pos_generator[0];
		}
		pGen->GeneratePos(pos,dir, offset_terrain, manager,dir1,rad);
		if(collision_id) *collision_id = pGen->GetCollisionID();
	}
	/*
	virtual char GenDir()
	{
		if(_pos_min.squared_distance( _pos_max) < 1e-3)
		{
			return _dir;
		}
		else
		{
			return abase::Rand(0,255);
		}
	}*/
	void SetRegion(int region_type, const float vPos[3],const float vExts[3], unsigned char dir0,unsigned char dir1, unsigned char rad);
	void BuildRegionCollision(world_manager * manager, int tid1,int did);
	//const rect & GetRegion() { return _region;}
	void ReleaseSelf()
	{
		Release();
		delete this;
	}
	void SetRespawn(char are) 
	{ 
		_auto_respawn = are;
	}

	void Heartbeat(world * pPlane)
	{
		mutex_spinlock(&_spool_lock);
		if(IsSpawned())
		{
			OnHeartbeat(pPlane);
		}
		mutex_spinunlock(&_spool_lock);
	}

	void SingleSpawn(world * pPlane)
	{
		mutex_spinlock(&_spool_lock);
		CreateObjects(pPlane,true);	
		mutex_spinunlock(&_spool_lock);
	}

	bool BeginSpawn(world * pPlane)
	{
		bool bRst = false;
		mutex_spinlock(&_spool_lock);
		if(!IsSpawned())
		{
			bRst = CreateObjects(pPlane);	
			SetSpawned(bRst);
		}
		mutex_spinunlock(&_spool_lock);
		return bRst;
	}
	
	bool EndSpawn(world * pPlane)
	{
		bool bRst = false;
		mutex_spinlock(&_spool_lock);
		if(IsSpawned())
		{
			ClearObjects(pPlane);	
			SetSpawned(false);
			bRst = true;
		}
		mutex_spinunlock(&_spool_lock);
		return bRst;
	}

	int GetPhaseID() {return _phase_id;}
	void SetPhaseID(int id) { _phase_id = id;}
	bool IsMobActiveReclaim() { return _auto_respawn == 2; }
private:
	virtual void OnHeartbeat(world * pPlane) = 0;
	virtual bool CreateObjects(world *pPlane, bool auto_detach = false) = 0;
	virtual void ClearObjects(world *pPlane) = 0;
	void SetSpawned(bool sp)
	{
		_is_spawned = sp;
	}
	bool IsSpawned() {return _is_spawned;}
	
};

struct crontab_t
{
private:
	int min;
	int hour;
	int month;
	int year;
	int day_of_months;
	int day_of_week;
public:
	struct entry_t
	{
		int min;
		int hour;
		int month;
		int year;
		int day_of_months;
		int day_of_week;
	};
public:
	crontab_t():min(-1), hour(-1), month(-1), year(-1), day_of_months(-1), day_of_week(-1)
	{}

	bool SetParam(int __year, int __month, int __day_of_months, int __hour, int __min, int __day_of_week)
	{
		min		= __min	;
		hour		= __hour;	
		month		= __month;	
		year		= __year;	
		day_of_months	= __day_of_months;	
		day_of_week	= __day_of_week;
		
		return min >= 0 && !(day_of_week >=0 && day_of_months >= 1) && day_of_months != 0;
	}

	bool SetParam(const entry_t & param)
	{
		min		= param.min;
		hour		= param.hour;	
		month		= param.month;	
		year		= param.year;	
		day_of_months	= param.day_of_months;	
		day_of_week	= param.day_of_week;
		
		return min >= 0 && !(day_of_week >=0 && day_of_months >= 1) && day_of_months != 0;
	}

	int CheckTime(const tm &tt);
	int CheckTime2(time_t t1,int DUR);
};

/*struct time_segment
{
public:
	struct entry_t
	{
		int min;
		int hour;
		int month;
		int year;
		int day_of_months;
		int day_of_week;

		bool is_inited;
		entry_t():min(-1), hour(-1), month(-1), year(-1), day_of_months(-1), day_of_week(-1), is_inited(false)
		{}

		void SetParam(int __year, int __month, int __day_of_months, int __hour, int __min, int __day_of_week)
		{
			min	    	= __min	;
			hour		= __hour;	
			month		= __month;	
			year		= __year;	
			day_of_months	= __day_of_months;	
			day_of_week	= __day_of_week;

			is_inited   = true;
		}

		void SetParam(const entry_t & param)
		{
			min		= param.min;
			hour		= param.hour;	
			month		= param.month;	
			year		= param.year;	
			day_of_months	= param.day_of_months;	
			day_of_week	= param.day_of_week;

			is_inited = true;
		}

	
		void Clear()
		{
			min		    = -1;
			hour		= -1;	
			month		= -1;	
			year		= -1;	
			day_of_months	= -1;	
			day_of_week	= -1;

			is_inited   = false;
		}
	};

public:
	entry_t _start_time;
	entry_t _end_time;
	time_segment()
	{
		_start_time.Clear();
		_end_time.Clear();
	}

	~time_segment()
	{
		_start_time.Clear();
		_end_time.Clear();
	}

	bool IsInTimeSegment(const tm &tt);
	int  CheckTimeToStart(const tm &tt);
	int  CheckTimeToStop(const  tm &tt);
	bool IsValid(void);

private:
	bool CheckTimeSegment(const tm &curtime, const entry_t &start_time, const entry_t &end_time);
	int  TimeCalculation(const tm &tt, const entry_t &target_time);
};*/

class spawner_ctrl 
{
	int _spawn_id;			//生成区的分类ID
	bool _auto_spawn;		//是否自动生成
	bool _spawn_flag;		//是否已经生成
	bool _active_flag;		//是否已经激活		激活后不一定立刻生成
	abase::vector<base_spawner *, abase::fast_alloc<> > _list;

	int _spawn_after_active;	//激活到生成的缓冲时间
	int _active_life;		//激活后的寿命 0为无穷

	bool _has_active_date;
	bool _has_stop_date;
	bool _detach_after_spawn;	//生成怪物后即脱离联系
	int _active_date_duration;
	crontab_t _active_date;
	crontab_t _stop_date;

	//用于时间段计算的数据
	time_segment _timeseg;
	int _timeseg_counter_down;

	int  _date_counter_down;
	int  _cur_active_life;	
	int  _time_before_spawn;
	int  _lock;
public:
	
	spawner_ctrl():_spawn_id(0),_auto_spawn(true),_spawn_flag(false),_active_flag(false)
	{
		_spawn_after_active = 0;
		_active_life = 0;
		_has_active_date = 0;
		_has_stop_date = 0;
		_detach_after_spawn = false;

		_date_counter_down = 0;
		_timeseg_counter_down = 0;
		_cur_active_life = 0;
		_time_before_spawn = 0;
		_active_date_duration = 0;
		_lock = 0;
	}
	inline void SetDetachAfterSpawn(bool val)
	{
		_detach_after_spawn = val;
	}

	inline bool InitTimeSegment(std::vector<time_segment::entry_t> & atimeseg, bool is_intersection) 
	{ 
		return _timeseg.Init(atimeseg, is_intersection);
	} 

	inline bool IsAutoSpawn() { return _auto_spawn;}
	inline bool IsActived() { return _active_flag;}
	inline void AddSpawner(base_spawner * sp) {_list.push_back(sp);}
	inline int GetConditionID()
	{
		return _spawn_id;
	}

	void SetCondition(int spawn_id, bool auto_spawn, int spawn_delay = 0, int active_life = 0)
	{
		_spawn_id = spawn_id;
		_auto_spawn = auto_spawn;
		_spawn_after_active =  spawn_delay;
		_active_life = active_life;
	}

	bool SetActiveDate(const crontab_t::entry_t & ent, int duration)
	{
		_has_active_date = false;
		if(!_active_date.SetParam(ent)) return false;
		_active_date_duration = duration;
		_has_active_date = true;
		return true;
	}

	bool SetStopDate(const crontab_t::entry_t & ent)
	{
		_has_stop_date = false;
		if(!_stop_date.SetParam(ent)) return false;
		_has_stop_date = true;
		return true;
	}

	bool SetAutoSpawn(bool auto_spawn)
	{
		_auto_spawn = auto_spawn;
		return true;
	}

	void OnHeartbeat(world * pPlane);
protected:
	inline void SetSpawnFlag(bool sf) { _spawn_flag = sf;}
	inline bool IsSpawned() { return _spawn_flag;}
	void Spawn(world * pPlane);
	void Active(world * pPlane);
	void SingleActive(world * pPlane);
	void Stop(world * pPlane);

public:
	bool BeginSpawn(world * pPlane);

	bool EndSpawn(world * pPlane)
	{
		spin_autolock keeper(_lock);
		if(!IsActived()) return false;
		Stop(pPlane);
		return true; 
	}
};

class gnpc_imp;
class npc_spawner : public base_spawner
{
	public:
		struct entry_t
		{
			int npc_tid;
			int msg_mask_or;
			int msg_mask_and;
			int alarm_mask;			//未用 
			int enemy_faction;		//未用 
			bool has_faction;
			int faction;			//未用 
			bool ask_for_help;
			int monster_faction_ask_help;
			bool accept_ask_for_help; 
			int monster_faction_accept_for_help;
			int reborn_time;
			int path_id;			//巡逻路线，如果是0则未用
			int path_type;			//巡论类型
			int corpse_delay;		//尸体残留时间，单位为秒 
			bool speed_flag;		//速度标记
			size_t mobs_count;
			float offset_terrain;		//和地形高度的偏移量
		};
	protected:
		int _reborn_time;				//总的重生时间，给group_spawner使用
		abase::vector<entry_t, abase::fast_alloc<> > _entry_list;//本生成器中的所有entry列表
		abase::vector<gnpc *, abase::fast_alloc<> > _npc_pool;   //npc的生成池，所有释放的npc 会被缓冲到这里
		int _mobs_total_gen_num;				//能够生成多少对象
		int _mobs_cur_gen_num;					//当前生成了多少对象
		int _mob_life;						//创建怪物的寿命

	public:
		npc_spawner():_reborn_time(0),_mobs_total_gen_num(0),_mobs_cur_gen_num(0),_mob_life(0){}
		virtual ~npc_spawner() {}
		virtual int Init(const void * buf, size_t len) { return 0;}
		virtual void OnHeartbeat(world * pPlane) = 0;
		virtual bool Reclaim(world * pPlane, gnpc * pNPC, gnpc_imp * imp,bool is_reset) = 0;
		virtual bool CreateMobs(world *pPlane, bool auto_detach) = 0;
		virtual void ForwardFirstAggro(world * pPlane,const XID & id, int rage) {}

		virtual bool CreateObjects(world *pPlane, bool auto_detach)
		{
			_mobs_cur_gen_num = 0;
			return CreateMobs(pPlane,auto_detach);
		}
		virtual void ClearObjects(world * pPlane);

		static gnpc * CreateMobBase(npc_spawner * __this,world * pPlane,const entry_t & et,
				int spawn_index, int phase_id, const A3DVECTOR & pos,const int cid[3],unsigned char dir, 
				int ai_policy_cid,int aggro_policy, gnpc * orign_npc,int life = 0, 
				const void *script_data = 0,size_t script_size = 0);

		static gnpc * CreatePetBase(gplayer_imp *pMaster,const A3DVECTOR& pos,const pet_data * pData,
				const pet_bedge_essence* pEss,const int cid[3],unsigned char dir, 
				int ai_policy_cid,int aggro_policy,int tid);

		static gnpc * CreateSummonBase(gplayer_imp *pCaster, const A3DVECTOR& pos, const int cid[3], 
				unsigned char dir, int ai_policy_cid, int aggro_policy, int tid, int life, bool is_clone);

		static gnpc * CreateMobActiveBase(npc_spawner * __this,world * pPlane,const entry_t & et,
				int spawn_index, const A3DVECTOR & pos,const int cid[3],unsigned char dir, 
				int ai_policy_cid,int aggro_policy, gnpc * orign_npc,int life = 0, 
				const void *script_data = 0,size_t script_size = 0);

		void RegenAddon(gnpc_imp * pImp, int npc_id);		

		void SetGenLimit(int life, int total_num)
		{
			_mob_life = life;
			_mobs_total_gen_num = total_num;
		}


	public:
		bool AddEntry(const entry_t & ent)
		{
			_entry_list.push_back(ent);
			_npc_pool.push_back(NULL);
			return true;
		}

		void SetRebornTime(int rtime) 
		{
			_reborn_time = rtime;
		}
};

class mine_spawner : public base_spawner
{
public:
	struct entry_t
	{
		int mid;
		int mine_count;
		int reborn_time;
	};
protected:
	abase::vector<entry_t,abase::fast_alloc<> > _entry_list;//本生成器中的所有entry列表
	abase::vector<gmatter *,abase::fast_alloc<> > _mine_pool;//npc的生成池，所有释放的npc 会被缓冲到这里
	float _offset_terrain;
	int _mine_total_gen_num;				//能够生成多少对象
	int _mine_cur_gen_num;					//当前生成了多少对象

public:
	mine_spawner():_offset_terrain(0),_mine_total_gen_num(0),_mine_cur_gen_num(0) {}
	virtual ~mine_spawner() {}
	virtual void OnHeartbeat(world * pPlane);
	virtual bool Reclaim(world * pPlane,gmatter * pMatter, gmatter_mine_imp * imp);
	virtual bool CreateMines(world * pPlane);
	static gmatter * CreateMine(mine_spawner* ,const A3DVECTOR & pos, world * pPlane,int index,const entry_t & ent,char dir,char dir1, char rad, int lifetime = 0);
	static void GenerateMineParam(gmatter_mine_imp * imp, mine_template * pTemplate);
	virtual void Reborn(world * pPlane,gmatter * header, gmatter * tail,int mid,int index);
	void SetOffsetTerrain(float offset) { _offset_terrain = offset; }
	void Release();
	void SetGenLimit(int total_num)
	{
		_mine_total_gen_num = total_num;
	}


	virtual bool CreateObjects(world *pPlane, bool auto_detach)
	{
		if(auto_detach) return false;
		_mine_cur_gen_num = 0;
		return CreateMines(pPlane);
	}

	virtual void ClearObjects(world * pPlane);

public:
	bool AddEntry(const entry_t & ent)
	{
		_entry_list.push_back(ent);
		_mine_pool.push_back(NULL);
		return true;
	}

};

class dyn_object_spawner : public mine_spawner  
{       
public:         
	virtual bool CreateMines(world * pPlane);
protected:      
	gmatter * CreateDynObject(mine_spawner* ,const A3DVECTOR & pos,size_t index, world * pPlane,const entry_t & ent, char dir, char dir1, char rad);
};

class CNPCGenMan;
class npc_generator : public ONET::Thread::Runnable
{

private:
//	abase::vector<npc_spawner*,abase::fast_alloc<> > _spawner_list;
//	abase::vector<mine_spawner*,abase::fast_alloc<> > _mine_spawner_list;

	abase::vector<base_spawner *,abase::fast_alloc<> > _sp_list;
	abase::vector<spawner_ctrl *,abase::fast_alloc<> > _ctrl_list;

	abase::hash_map<int, spawner_ctrl *> _ctrl_map;		//condition id-->ctrl 的对照表，里面并未包含所有ctrl
	abase::hash_map<int, spawner_ctrl *> _ctrl_idx_map; 	//ctrl id-->ctrl的对照表 里面包含了所有的ctrl

	npc_generator & operator=(const npc_generator & );
public:
	npc_generator():_task_offset(0),_tcount(0),_tlock(0),_plane(0)
	{
		//_spawner_list.reserve(MAX_SPAWN_COUNT);
		//_mine_spawner_list.reserve(MAX_SPAWN_COUNT);
		_sp_list.reserve(MAX_SPAWN_COUNT);
	}

	bool LoadGenData(world_manager* manager,CNPCGenMan & npcgen, rect & region);
	bool InsertSpawner(int ctrl_idx, base_spawner * sp)
	{
		spawner_ctrl * ctrl = _ctrl_idx_map[ctrl_idx];
		if(ctrl == NULL) 
		{
			__PRINTF("布置区域无法找到合适的控制对象 ctrl_idx:%d\n", ctrl_idx);
			return false;
		}

		//检查一下是否本ctrl_condtion被禁止
		if(gmatrix::IsControllerDisable(ctrl->GetConditionID()))
		{
			//printf("某怪物生成区由于取消的控制器idx:%d(cond:%d)而不再生成\n",ctrl_idx,ctrl->GetConditionID());
			sp->ReleaseSelf();
			return true;
		}
		CheckCtrlID( ctrl->GetConditionID() );
		_sp_list.push_back(sp);
		ctrl->AddSpawner(sp);
		return true;
	}
	void CheckCtrlID( int id );

	bool InsertSpawnControl(int ctrl_idx, int condition_id, bool auto_spawn,int spawn_delay, int active_life,
				int active_date_duration,
				const crontab_t::entry_t * active_date = NULL, 
				const crontab_t::entry_t * stop_date = NULL,
				bool auto_detach = false)
	{
		spawner_ctrl * ctrl = _ctrl_idx_map[ctrl_idx];
		if(ctrl) return false;
		if(condition_id > 0)
		{
			ctrl = _ctrl_map[condition_id];
			if(ctrl) return false;
		}
		ctrl = new  spawner_ctrl();
		ctrl->SetCondition(condition_id, auto_spawn, spawn_delay, active_life);
		ctrl->SetDetachAfterSpawn(auto_detach);
		bool bRst = true;
		if(active_date)
		{
			bRst = bRst  && ctrl->SetActiveDate(*active_date,active_date_duration);
		}

		if(stop_date)
		{
			bRst = bRst  && ctrl->SetStopDate(*stop_date);
		}

		_ctrl_idx_map[ctrl_idx] = ctrl;
		_ctrl_list.push_back(ctrl);

		if(condition_id > 0) _ctrl_map[condition_id] = ctrl;
		return bRst;
	}

	bool InsertSpawnCtrlTimeSeg(int ctrl_idx, std::vector<time_segment::entry_t> & atimeseg, bool is_intersection)
	{
		spawner_ctrl * ctrl = _ctrl_idx_map[ctrl_idx];
		if(!ctrl) return false;

		//有激活时间段时，初始激活无效
		ctrl->SetAutoSpawn(false);
		
		time_segment temp_timeseg;
		for(size_t i = 0; i < atimeseg.size(); i += 2)
		{
			if( !temp_timeseg.CheckValidity(atimeseg[i], atimeseg[i+1]) )
			{
				atimeseg.clear();
				__PRINTF("控制器时间段填写错误 id=%d\n", ctrl->GetConditionID());
				return false;
			}
		}

		if( !temp_timeseg.CheckTimeSegAvailable(atimeseg, is_intersection) )
		{
			__PRINTF("控制器时间段数据有误 ctrl_id = %d, is_intersection=%d\n", ctrl->GetConditionID(), is_intersection);
			return false;
		}
		else
		{
			ctrl->InitTimeSegment(atimeseg, is_intersection);
		}
	
		return true;
	}

	bool InitIncubator(world * pPlane);		//初始化所有的spawner，去掉不属于自己的部分

	bool BeginSpawn();
	bool TriggerSpawn(int condition);
	void ClearSpawn(int condition);
	void Release();
public:
	
	void StartHeartbeat()
	{
		_task_offset = 0;
		_task_offset2 = 0;
		_tcount = 0;
		_tcount2 = 0;
		_tlock = 0;
	}
protected:
	size_t _task_offset;
	size_t _task_offset2;
	int _tcount;
	int _tcount2;
	int _tlock;
	world * _plane;

	void OnTimer(int index,int rtimes)
	{
		ONET::Thread::Pool::AddTask(this);
	}
public:

/*
	virtual void Run()
	{
		spin_autolock keeper(_tlock);
		_tcount += _spawner_list.size();

		while(_tcount > TICK_PER_SEC)
		{
			_spawner_list[_task_offset]->OnHeartbeat(_plane);
			_task_offset ++;
			if(_task_offset >= _spawner_list.size()) _task_offset = 0;
			_tcount -= TICK_PER_SEC;
		}

		_tcount2 += _mine_spawner_list.size();
		while(_tcount2 > TICK_PER_SEC)
		{
			_mine_spawner_list[_task_offset2]->OnHeartbeat(_plane);
			_task_offset2 ++;
			if(_task_offset2 >= _mine_spawner_list.size()) _task_offset2 = 0;
			_tcount2 -= TICK_PER_SEC;
		}
	}*/

	virtual void Run()
	{
		spin_autolock keeper(_tlock);
		_tcount += _sp_list.size();

		while(_tcount > TICK_PER_SEC)
		{
			_sp_list[_task_offset]->Heartbeat(_plane);
			_task_offset ++;
			if(_task_offset >= _sp_list.size()) _task_offset = 0;
			_tcount -= TICK_PER_SEC;
		}

		_tcount2 += _ctrl_list.size();
		while(_tcount2 > TICK_PER_SEC)
		{
			_ctrl_list[_task_offset2]->OnHeartbeat(_plane);
			_task_offset2 ++;
			if(_task_offset2 >= _ctrl_list.size()) _task_offset2 = 0;
			_tcount2 -= TICK_PER_SEC;
		}
	}

};

class mobs_spawner : public npc_spawner
{
public:
	int _reborn_count;
	int _rrcount;
protected:
	//path_graph ..
	gnpc * CreateMob(world * pPlane,int spawn_index,const entry_t & et, bool auto_detach);
	void Reborn(world * pPlane,gnpc * header, gnpc * tail,float height,int tid);

	virtual void ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR & pos,char dir, int index);
public:
	mobs_spawner():_reborn_count(0),_rrcount(50){}
	void OnHeartbeat(world * pPlane);

	//创建所有的mobs 
	virtual bool CreateMobs(world * pPlane, bool auto_detach);
	virtual bool Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp, bool is_reset);
	virtual void Release();

};

//npc服务，是否应该将若干npc放在一个spawner里面
class server_spawner : public mobs_spawner
{
public:
protected:
	//path_graph ..
	gnpc* CreateNPC(world * pPlane, int spawn_index,const entry_t & et, bool auto_detach);
	gnpc* CreateNPC(world * pPlane, int spawn_index,const entry_t & et, const A3DVECTOR & pos, gnpc * pNPC, bool auto_detach,char dir);

	//void Reborn(world * pPlane,gnpc * header, gnpc * tail,float height,int tid);
	virtual void ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR & pos, char dir,int index);
public:
	server_spawner(){}
	void OnHeartbeat(world * pPlane);

	//创建所有的mobs 
	virtual bool CreateMobs(world * pPlane, bool auto_detach);
	virtual bool Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp, bool is_reset);

	static void AddNPCServices(service_npc * pImp, npc_template * pTemplate);
	
};

//普通群怪
class group_spawner : public mobs_spawner
{
protected:
	int _next_time;
	int _lock;				//这是群怪的锁，当操作重生时间时加这个锁，理论上这个锁可以归并到_spool_lock
	int _leader_id;
	A3DVECTOR _leader_pos;
	bool _gen_pos_mode;
	gnpc * CreateMasterMob(world * pPlane,int spawner_index, const entry_t &et, bool auto_detach = false);
	gnpc * CreateMinorMob(world * pPlane,int spawner_index, int leader_id, const A3DVECTOR & leaderpos, const entry_t &et, bool auto_detach = false);
public:
	group_spawner():_next_time(0),_lock(0),_leader_id(0),_gen_pos_mode(false)
	{
		_rrcount = 1;
	}

	virtual void ClearObjects(world *pPlane)
	{
		mobs_spawner::ClearObjects(pPlane);
		_next_time = 0;
		_leader_id = 0;
	}
	void OnHeartbeat(world * pPlane);
	virtual bool CreateMobs(world *pPlane, bool auto_detach);
	virtual bool Reclaim(world * pPlane, gnpc * pNPC, gnpc_imp * imp, bool is_reset);
	virtual void GeneratePos(A3DVECTOR &pos,char & dir, float offset_terrain,world_manager* manager,int * collision_id, char*dir1=NULL,char*rad=NULL);
};

//特殊群怪
class boss_spawner : public group_spawner
{
protected:
	abase::vector<XID,abase::fast_alloc<> > _mobs_list;
	int _mobs_list_lock;
public:
	boss_spawner()
	{
		_mobs_list_lock = 0;
	}
	virtual void ClearObjects(world *pPlane)
	{
		group_spawner::ClearObjects(pPlane);
		spin_autolock keeper(_mobs_list_lock);
		_mobs_list.clear();
	}
	virtual bool CreateMobs(world *pPlane, bool auto_detach);
	gnpc * CreateMasterMob(world * pPlane,int spawner_index, const entry_t &et, bool auto_detach = false);
	gnpc * CreateMinorMob(world * pPlane,int spawner_index, int leader_id, const A3DVECTOR & leaderpos, const entry_t &et, bool auto_detach = false);
	virtual void ForwardFirstAggro(world* pPlane,const XID & id, int rage);
};

//互动物品怪
class mobactive_spawner : public mobs_spawner
{
protected:

	gnpc * CreateMobActive(world * pPlane, int spawn_index,const entry_t & et, bool auto_detach);
	gnpc * CreateMobActive(world * pPlane, int spawn_index,const entry_t & et, const A3DVECTOR & pos, gnpc * pNPC, bool auto_detach, char dir);

	virtual void ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR & pos,char dir, int index);
public:
	mobactive_spawner() {}
	void OnHeartbeat(world * pPlane);

	//创建所有的mobs 
	virtual bool CreateMobs(world * pPlane, bool auto_detach);
	virtual bool Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp, bool is_reset);
	static void GenerateMobActiveParam(mob_active_imp * imp, npc_template * pTemplate);
};

#endif

