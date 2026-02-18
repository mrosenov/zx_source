#include <conf.h>
#include "step_raid_world_manager.h"

int step_raid_world_manager::_ctrl_id[2][20] = 
{
        {82925,82926,82927,82928,82929,82930,82931,82932,82933,82934,
         82935,82936,82937,82938,82939,82940,82941,82942,82943,82944},
    
        {82945,82946,82947,82948,82949,82950,82951,82952,82953,82954,
         82955,82956,82957,82958,82959,82960,82961,82962,82963,82964,},
};

int step_raid_world_manager::_monster_id[2][20] = 
{
        {60368,60380,60371,60381,60372,60382,60373,60383,60374,60384,
         60375,60385,60376,60386,60377,60387,60378,60388,60379,60389,},

        {60390,60400,60391,60401,60392,60402,60393,60403,60394,60404,
         60395,60405,60396,60406,60397,60407,60398,60408,60399,60409},
};

int step_raid_world_manager::_monster_num[2][20] =
{
        {1,30,1,30,1,30,1,30,1,30,
         1,30,1,30,1,30,1,30,1,30},

        {1,30,1,30,1,30,1,30,1,30,
         1,30,1,30,1,30,1,30,1,30},
};

int step_raid_world_manager::_monster_score[2][20] = 
{
        {10,1,10,1,10,1,10,1,10,1,
         10,1,10,1,10,1,10,1,10,1},

        {10,1,10,1,10,1,10,1,10,1,
         10,1,10,1,10,1,10,1,10,1},
};

int step_raid_world_manager::_award_task[2][4] = 
{
        {29727,29729,29731,29732},
        {29733,29734,29735,29736},
};

int step_raid_world_manager::_award_id[2][4] = 
{
        {60423,60424,60425,60426},
        {60427,60428,60429,60430},
};

step_raid_world_manager::step_raid_world_manager()
{
        _step_index = 0;
        _monster_kill_num = 0;
        _score = 0;
        _award_level = 0;
}

step_raid_world_manager::~step_raid_world_manager()
{
}

void step_raid_world_manager::Reset()
{
	raid_world_manager::Reset();
        _step_index = 0;
        _monster_kill_num = 0;
        _score = 0;
        _award_level = 0;
}

int step_raid_world_manager::CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index,
        const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
        ONET::Conf *conf = ONET::Conf::GetInstance();
        ONET::Conf::section_type section = "World_";
        section += rwt.servername;
        _step_level = atoi(conf->find(section,"step_level" ).c_str());
        return raid_world_manager::CreateRaid(raidroom_id, rwt, tag, index, roleid_list, buf, size, difficulty);
}

void step_raid_world_manager::OnCreate()
{
	raid_world_manager::OnCreate();
        _step_index = 0;
        ActiveSpawn(this,_ctrl_id[_step_level][0],true);
}

int step_raid_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
        if (_step_index < STEP_NUM)
        {
                if (_monster_id[_step_level][_step_index] == tid)
                {
                        _score += _monster_score[_step_level][_step_index];
                        _monster_kill_num++;
                        if (_monster_kill_num == _monster_num[_step_level][_step_index])
                        {
                                _step_index++;
                                _monster_kill_num = 0;
				if (_step_index < STEP_NUM)
				{
					ActiveSpawn(this,_ctrl_id[_step_level][_step_index],true);
				}
				else if (_step_index == STEP_NUM)
				{
					_raid_result  = 1;
				}
                                SyncStepRaidInfo();
                        }

                }
        }
        return raid_world_manager::OnMobDeath( pPlane, faction, tid, pos, attacker_id );
}

void step_raid_world_manager::SyncStepRaidInfo()
{
        char monster_index = _step_index / 2;
        char tower_index = _step_index - monster_index;

        if (_score >= 100 && _score < 200)
        {
                 _award_level = 1;
        }
        else if (_score >= 200 && _score < 300)
        {
                _award_level = 2;
        }
        else if (_score >= 300 && _score < 400)
        {
                _award_level = 3;
        }
        else if (_score >= 400)
        {
                _award_level = 4;
        }

        packet_wrapper h1(64);
        using namespace S2C;
	int award_icon = -1;
	int award_id = -1;
	if (_award_level > 0)
	{
		award_icon = _award_level -1;
		award_id = _award_id[_step_level][_award_level-1];
	}
	CMD::Make<CMD::step_raid_score_info>::From(h1,tower_index,monster_index,award_icon,award_id,_score);
	if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
}

void step_raid_world_manager::OnClose()
{
        if (_award_level)
        {
                cs_user_map::iterator it = _all_list.begin();
                for (;it != _all_list.end(); ++it)
                {
                        cs_user_list & list = it->second;
                        cs_user_list::const_iterator listit = list.begin();
                        for (;listit != list.end();++listit)
                        {
                                MSG msg;
                                BuildMessage(msg,GM_MSG_GET_STEP_RAID_TASK,XID(GM_TYPE_PLAYER,listit->first),XID(-1,-1),A3DVECTOR(0,0,0),_award_task[_step_level][_award_level-1]);
                                gmatrix::SendMessage(msg);
                        }
                }
        }

        raid_world_manager::OnClose();
}
