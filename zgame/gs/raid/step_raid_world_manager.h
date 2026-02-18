#ifndef __STEP_RAID_WORLD_MANAGER_
#define __STEP_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"

class step_raid_world_manager : public raid_world_manager
{
public:
        enum { STEP_NUM = 20 };
        step_raid_world_manager();
        virtual ~step_raid_world_manager();

        virtual int GetRaidType() { return RT_STEP; } 
        virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
        virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, 
                        const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
        virtual void OnCreate();
        virtual void OnClose();
        void SyncStepRaidInfo();

	virtual void DebugSetRaidScore(int score)
	{ 
		if (score > 400) score = 400;
		_score = score;
		SyncStepRaidInfo();
	}

        virtual void Reset();

private:
        int _step_level;
        int _step_index;
        int _monster_kill_num;
        int _score;
        int _award_level;

protected:
        static int _ctrl_id[2][20];
        static int _monster_id[2][20];
        static int _monster_num[2][20];
        static int _monster_score[2][20];
        static int _award_task[2][4];
	static int _award_id[2][4];
};

#endif
