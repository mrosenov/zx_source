#ifndef __TRANSFORM_RAID_WORLD_MANAGER_
#define __TRANSFORM_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"

class transform_raid_world_manager : public raid_world_manager
{

public:
	enum 
	{
		MAX_TRANSFORM_NUM = 17,
		MAX_MONSTER_NUM = 5,
	};

	transform_raid_world_manager();
	virtual ~transform_raid_world_manager();

	virtual int GetRaidType() { return RT_TRANSFORM; }
	virtual void HandleTransformRaid(int player_id);
        virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnRunning();
	
	//获得回城点 
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	void SyncTransFormRaidInfo();

	virtual void Reset();

private:
	static int _special_ctrlid;
	static int _transform_taskid[MAX_TRANSFORM_NUM];
	static int _ctrl_id[MAX_TRANSFORM_NUM];

	bool _task_idx_has_deliver[MAX_TRANSFORM_NUM];
	std::map<int, int> _player_transform_map; //playerid --> transform_taskid_index
	int _player_map_lock;
	bool _special_ctrlid_open;

	std::vector<raid_boss_info> _monster_info_vec;

protected:
	static int _monster_id[MAX_MONSTER_NUM];
	static int _monster_num[MAX_MONSTER_NUM];
	int _special_ctrlid_timer;
};

#endif
