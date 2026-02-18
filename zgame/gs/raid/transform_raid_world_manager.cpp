#include "transform_raid_world_manager.h"

int transform_raid_world_manager::_transform_taskid[MAX_TRANSFORM_NUM] = {29575,29576,29577,29578,29579,29580,29581,29582,29583,
									29584,29585,29586,29587,29588,29589,29590,29591};
int transform_raid_world_manager::_ctrl_id[MAX_TRANSFORM_NUM] = {82896,82897,82898,82899,82900,82901,82902,82903,82904,
								82905,82906,82907,82908,82909,82910,82911,82912};
int transform_raid_world_manager::_special_ctrlid = 82895;

int transform_raid_world_manager::_monster_id[MAX_MONSTER_NUM] = {60806,60804,60799,60805,61157};

int transform_raid_world_manager::_monster_num[MAX_MONSTER_NUM] = {6,1,6,1,1};

transform_raid_world_manager::transform_raid_world_manager()
{
	for (int i = 0; i < MAX_TRANSFORM_NUM; i++)
	{
		_task_idx_has_deliver[i] = false;
	}
	_player_map_lock = 0;
	_special_ctrlid_open = false;
	_special_ctrlid_timer = 0;

	if (!_monster_info_vec.size())
	{
		for (int i = 0; i < MAX_MONSTER_NUM; i++)
		{
			_monster_info_vec.push_back(raid_boss_info(0,0,1));
			_monster_info_vec[i].max_num = _monster_num[i];
		}
		_monster_info_vec[0].achievement = 2;
	}
}

transform_raid_world_manager::~transform_raid_world_manager()
{
	_player_transform_map.clear();
}

void transform_raid_world_manager::Reset()
{
	raid_world_manager::Reset();
	_player_map_lock = 0;
	_player_transform_map.clear();
	_special_ctrlid_open = false;
	_special_ctrlid_timer = 0;

	for (int i = 0; i < MAX_TRANSFORM_NUM; ++i)
	{
		_task_idx_has_deliver[i] = false;
	}

	for (int i = 0; i < MAX_MONSTER_NUM; ++i)
	{
		_monster_info_vec[i].is_killed = 0;
		_monster_info_vec[i].cur_num = 0;
		_monster_info_vec[i].achievement = 1;
	}
	_monster_info_vec[0].achievement = 2;
}

void transform_raid_world_manager::HandleTransformRaid(int player_id)
{
	int sendtaskid = 0;
	if(RS_RUNNING != _status ) return;

	{
		spin_autolock keeper(_player_map_lock);
		if (_player_transform_map.find(player_id) != _player_transform_map.end())
		{
			sendtaskid = _player_transform_map[player_id];
		}
		else
		{
			int task_free[MAX_TRANSFORM_NUM] = {0};
			int task_index = 0;
			if (_player_transform_map.size() == 0)
			{
				task_index = abase::Rand(0,2);
				sendtaskid = _transform_taskid[task_index];
			}
			else
			{
				int i = 0;
				int task_free_num = 0;
				for (i = 0; i < MAX_TRANSFORM_NUM;i++)
				{
					if (!_task_idx_has_deliver[i])
					{
						task_free[task_free_num++] = i;
					}
				}
				task_index = task_free[abase::Rand(0,task_free_num-1)];
				sendtaskid = _transform_taskid[task_index];
			}
			_task_idx_has_deliver[task_index] = true;
			_player_transform_map.insert(std::make_pair(player_id,sendtaskid));
		}
		if (!_special_ctrlid_open)
		{
			if (_player_transform_map.size() == 1)
			{
				_special_ctrlid_timer = 5;
			}
		}
	}

	MSG msg;
	BuildMessage(msg, GM_MSG_GET_RAID_TRANSFORM_TASK, XID(GM_TYPE_PLAYER, player_id),
			XID(-1,-1),A3DVECTOR(0,0,0), sendtaskid);

	gmatrix::SendMessage(msg);
}

void transform_raid_world_manager::OnRunning()
{
	raid_world_manager::OnRunning();

	if (_special_ctrlid_timer > 0)
	{
		_special_ctrlid_timer--;
		if (_special_ctrlid_timer <= 0)
		{
			_special_ctrlid_open = true;
			ActiveSpawn(this,_special_ctrlid,true);
			for (int i = 0; i < MAX_TRANSFORM_NUM; ++i)
			{
				if (!_task_idx_has_deliver[i])
					ActiveSpawn(this,_ctrl_id[i],true);
			}
		}
	}
}


inline bool is_in_zone(A3DVECTOR min, A3DVECTOR max, A3DVECTOR pos)
{
	return (min.x <= pos.x && min.y <= pos.y && min.z <= pos.z) && (max.x >= pos.x && max.y >= pos.y && max.z >= pos.z);
}

bool transform_raid_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	A3DVECTOR posAMin(-326,50,-326),posAMax(-125,400,-80);
	A3DVECTOR posBMin(54,50,0),posBMax(251,400,234);

	if (is_in_zone(posAMin,posAMax,opos))
	{

		A3DVECTOR townpos(-255,165,-296);
		pos = townpos;
	}
	else if (is_in_zone(posBMin,posBMax,opos))
	{
		A3DVECTOR townpos(122,163,39);
		pos = townpos;
	}
	world_tag = GetWorldTag();
	return true;
}

int transform_raid_world_manager::OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id)
{
	for (int i = 0; i < MAX_MONSTER_NUM; ++i)
	{
		if (tid == _monster_id[i])
		{
			if (_monster_info_vec[i].cur_num < _monster_info_vec[i].max_num)
			{
				_monster_info_vec[i].cur_num++;
				_monster_info_vec[i].achievement = 2;
				if (_monster_info_vec[i].cur_num == _monster_info_vec[i].max_num)
				{
					_monster_info_vec[i].is_killed = 1;
					if (i + 1 < MAX_MONSTER_NUM) _monster_info_vec[i+1].achievement = 2;
				}
				SyncTransFormRaidInfo();
			}

		}
	}
	return raid_world_manager::OnMobDeath( pPlane, faction, tid, pos, attacker_id );
}

void transform_raid_world_manager::SyncTransFormRaidInfo()
{
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<CMD::raid_boss_been_killed>::From(h1,_monster_info_vec);
	if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
}
