#include "collision_raid_world_manager.h"
#include "raid_world_message_handler.h"
#include "../obj_interface.h"
#include <common/message.h>
#include <factionlib.h>
#include "../faction.h"

collision_raid_world_manager::collision_raid_world_manager()
{
	_max_killer_count = 0;
	_attacker_award_score = 0;
	_defender_award_score = 0;
	_attacker_count_timer = 0;
	_defender_count_timer = 0;
}

collision_raid_world_manager::~collision_raid_world_manager()
{
}


void collision_raid_world_manager::Reset()
{
	_max_killer_count = 0;
	_attacker_pos_list.clear();
	_defender_pos_list.clear();
	_attacker_count_timer = 0;
	_defender_count_timer = 0;
	_player_list.clear();
	
	raid_world_manager::Reset();
}

void collision_raid_world_manager::OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction) 
{
	int roleid = pPlayer->ID.id;
	GetEntryPos(pPlayer->pos, faction, roleid);
        
	packet_wrapper h1(64);
        using namespace S2C;
        CMD::Make<CMD::collision_raid_score>::From(h1, GetAttackerScore(), GetDefenderScore(), GetRaidMaxKillerID(), _player_list.size(), _player_list.begin());
	send_ls_msg(pPlayer, h1);

	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].roleid == roleid)
		{
			_player_list[i].is_online = 1;
			break;
		}
	}
	SendCheatInfo(roleid, faction);
}

void collision_raid_world_manager::OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction) 
{
	int roleid = pPlayer->ID.id;
	ClrEntryPos(roleid, faction);
	
	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].roleid == roleid)
		{
			_player_list[i].is_online = 0;
			break;
		}
	}
}

void collision_raid_world_manager::OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much )
{
	if(killer == deadman) return;

	spin_autolock keeper( _raid_lock );
	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].roleid == killer)
		{
			_player_list[i].kill_count++;
			if(_player_list[i].kill_count > _max_killer_count)
			{
				_max_killer_count = _player_list[i].kill_count;
				_max_killer_id = killer;
			}
		}
		else if(_player_list[i].roleid == deadman)
		{
			_player_list[i].death_count++;
		}
		

	}

	if(deadman_battle_faction == RF_ATTACKER)
	{
		IncDefenderScore(1);
		if(GetDefenderScore() >= 40 && _raid_result == 0)
		{
			_raid_result = RAID_DEFENDER_WIN; 
		}
	}
	else if(deadman_battle_faction == RF_DEFENDER)
	{
		IncAttackerScore(1);
		if(GetAttackerScore() >= 40 && _raid_result == 0)
		{
			_raid_result = RAID_ATTACKER_WIN; 
		}
	}
}


void collision_raid_world_manager::Heartbeat()
{
	if(_status == RS_READY || _status == RS_IDLE) return;

	global_world_manager::Heartbeat();
	++_heartbeat_counter; 
	if( (_heartbeat_counter) % 20 == 0 )
	{
		spin_autolock keeper( _raid_lock );

		switch ( _status )
		{	
			case RS_RUNNING:
			{
				OnRunning();
			}
			break;

			case RS_CLOSING:
			{
				if( _cur_timestamp < g_timer.get_systime() )
				{
					OnClosing();
					// 在这之前需要确保非gm玩家已经被踢出
					__PRINTF( "副本:%d，关闭，恢复到初始状态\n", GetWorldTag());
					Release();
				}
			}
			break;

			default:
			{
				__PRINTF( "ERROR! _status=%d\n", _status );
				ASSERT( false );
			}
		};
	}
}


void collision_raid_world_manager::OnRunning()
{
	if( _cur_timestamp < g_timer.get_systime() || _raid_result > 0)
	{
		if(_raid_result == 0)
		{
			if(GetAttackerScore() > GetDefenderScore())
			{
				_raid_result = RAID_ATTACKER_WIN; 
			}
			else if(GetAttackerScore() < GetDefenderScore())
			{
				_raid_result = RAID_DEFENDER_WIN;
			}
			else 
			{
				_raid_result = RAID_DRAW;
			}
		}

		CalcAwardScore();

		OnClose();
		_status = RS_CLOSING;
		GNET::SendRaidEnd( gmatrix::Instance()->GetServerIndex(), _raid_id, GetWorldTag(), _raid_result); 
		_cur_timestamp = g_timer.get_systime() + _close_raid_time;
		_kill_monster_timestamp = _cur_timestamp - 90;

		//KillAllMonsters();

		// 战斗结束时更新一次信息
		SendRaidInfo();

		// 计算战斗结果
		__PRINTF( "副本:%d 结束发布结果开始准备清理result=%d, close=%d\n", GetWorldTag(), _raid_result, _close_raid_time );
		GLog::log( GLOG_INFO, "副本结束,gs_id=%d, raid_world_tag=%d, raid_id=%d, end_time=%d,", gmatrix::Instance()->GetServerIndex(), GetWorldTag(), _raid_id, _end_timestamp );
	}
	//检查攻方或者守方是否队员全部不存在
	else if(g_timer.get_systime() > _start_timestamp + 60)
	{
		OnCheckCheat();
	}

}

void collision_raid_world_manager::OnCheckCheat()
{
	bool attacker_exist = false;
	bool defender_exist = false;

	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].is_online && _player_list[i].faction == RF_ATTACKER)
		{
			attacker_exist = true;
		}
		else if(_player_list[i].is_online && _player_list[i].faction == RF_DEFENDER)
		{
			defender_exist = true;
		}

		if(attacker_exist && defender_exist) break;
	}

	if(attacker_exist)
	{
		_attacker_count_timer = 0;
	}
	else 
	{
		_attacker_count_timer++;
	}

	if(defender_exist)
	{
		_defender_count_timer = 0;
	}
	else 
	{
		_defender_count_timer++;
	}

	if(_attacker_count_timer >= 30 && _defender_count_timer >= 30)
	{
		_raid_result = RAID_DRAW;
	}
	else if(_attacker_count_timer >= 30)
	{
		_raid_result = RAID_DEFENDER_WIN;
	}
	else if(_defender_count_timer >= 30)
	{
		_raid_result = RAID_ATTACKER_WIN; 
	}
}

void collision_raid_world_manager::OnClosing()
{
}

void collision_raid_world_manager::CalcAwardScore()
{
	int attacker_num = 0;
	int defender_num = 0;
	int attacker_score_sum = 0;
	int defender_score_sum = 0;
	int attacker_score_avg = 0;
	int defender_score_avg = 0;
	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].faction == RF_ATTACKER)
		{
			attacker_score_sum += _player_list[i].score;
			attacker_num++;
		}	
		else if(_player_list[i].faction == RF_DEFENDER)
		{
			defender_score_sum += _player_list[i].score;
			defender_num++;
		}
	}

	if(attacker_num > 0) attacker_score_avg = attacker_score_sum /attacker_num;
	if(defender_num > 0) defender_score_avg = defender_score_sum /defender_num;

	int score_diff_avg = abs(attacker_score_avg - defender_score_avg);
	int score_diff_level = score_diff_avg / 300;

	//积分计算规则
	//如果双方积分差距在300以内，那么胜利一方加10分，败得一方加10分
	//双方积分差距每300分为一档, 差距每增加300分，加减5分
	//最低获得或者扣除5分
	if(_raid_result == RAID_ATTACKER_WIN)
	{
		if(attacker_score_sum > defender_score_sum && score_diff_avg >= 300)
		{
			_attacker_award_score = 5;
			_defender_award_score = -5;
		}
		else
		{
			_attacker_award_score = 10 + 5 * score_diff_level;
			_defender_award_score = -10 - 5 * score_diff_level;
		}
	}
	else if(_raid_result == RAID_DEFENDER_WIN)
	{
		if(defender_score_sum > attacker_score_sum && score_diff_avg >= 300)
		{
			_attacker_award_score = -5;
			_defender_award_score = 5;
		}
		else
		{
			_attacker_award_score = -10 - 5 * score_diff_level;
			_defender_award_score = 10 + 5 * score_diff_level;
		}
	}
}


int collision_raid_world_manager::TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	if(_status != RS_IDLE)
	{
		return -1;
	}

	if((size % sizeof(player_apply_info)) != 0 )
	{
		return -2;
	}


	return 0;
}

int collision_raid_world_manager::CreateRaid(int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty)
{
	spin_autolock keeper( _raid_lock );
	ASSERT(size % sizeof(player_apply_info) == 0);

	_status = RS_READY;

	_raid_id = rwt.rwinfo.raid_id;
	int ret = global_world_manager::Init(0, rwt.servername.c_str(), tag, index);
	if( 0 != ret )
	{
		return ret;
	}
	delete _message_handler;
	_message_handler = new raid_world_message_handler( this, &_plane );

	if(!OnInit()) 
	{
		return -10015;
	}
	__PRINTF( "副本创建请求, raid_template_id=%d, raid_id=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id);
	
	DATA_TYPE dt;
	const TRANSCRIPTION_CONFIG& raid_config = *(const TRANSCRIPTION_CONFIG*) gmatrix::GetDataMan().get_data_ptr(rwt.rwinfo.raid_template_id, ID_SPACE_CONFIG, dt);
	if(dt != DT_TRANSCRIPTION_CONFIG || &raid_config == NULL)
	{
		__PRINTF("无效的副本模板: %d\n", rwt.rwinfo.raid_template_id);
		GLog::log( GLOG_INFO, "副本开启失败,原因：副本模板不存在 raid_template_id=%d, raid_id=%d, _status=%d\n", rwt.rwinfo.raid_template_id, rwt.rwinfo.raid_id, _status );
		return -10011;
	}	
	
	
	_close_raid_time = 300;
	_player_leave_time = 60;
	_raid_time = raid_config.total_exist_time + 30;	//30s准备时间

	_start_timestamp = g_timer.get_systime();
	_end_timestamp = _start_timestamp + _raid_time;
	_cur_timestamp = _end_timestamp;
	_raid_id = rwt.rwinfo.raid_id;
	_apply_count = roleid_list.size();
	
	OnCreate();
	_status = RS_RUNNING;
	
	player_apply_info * pInfo = (player_apply_info * ) buf;
	size_t count = size / sizeof(player_apply_info);
	collision_player_info info; 
	for(size_t i = 0; i < count; ++i, ++pInfo)
	{
		info.roleid = pInfo->roleid;
		info.faction = pInfo->faction;
		info.kill_count = 0;
		info.death_count = 0;
		info.score = pInfo->score;
		info.is_online = 0;
		_player_list.push_back(info);
	}

	__PRINTF( "副本:%d:%d开启,持续时间:%d\n", _raid_id, GetWorldTag(), _raid_time); 
	GLog::log( GLOG_INFO, "副本开启成功 raid_template_id=%d, raid_id=%d, end_time=%d\n", rwt.rwinfo.raid_template_id,_raid_id, _end_timestamp);
	return 0;
	
}



bool collision_raid_world_manager::GetCollisionPlayerInfo(int roleid, int & battle_faction) 
{
	spin_autolock keeper( _raid_lock );

	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(roleid == _player_list[i].roleid)
		{
			battle_faction = _player_list[i].faction;
			return true;
		}
	}
	return false;
}

void collision_raid_world_manager::RecordEntryPos(const A3DVECTOR& pos,int faction)
{
	entry_pos epos;
	memset(&epos, 0, sizeof(entry_pos));
	epos.pos = pos;

	if(faction & FACTION_OFFENSE_FRIEND)
	{ 
		_attacker_pos_list.push_back(epos);
	}
	else if(faction & FACTION_DEFENCE_FRIEND)
	{
		_defender_pos_list.push_back(epos);
	}
}

void collision_raid_world_manager::GetEntryPos(A3DVECTOR& pos,int raid_faction, int roleid)
{
	pos = A3DVECTOR(0,0,0);
	if(RF_ATTACKER == raid_faction)
	{       
		for(size_t i = 0; i < _attacker_pos_list.size(); ++i)
		{
			if(_attacker_pos_list[i].is_used) continue;

			_attacker_pos_list[i].roleid = roleid;
			_attacker_pos_list[i].is_used = true;
			pos = _attacker_pos_list[i].pos;
			return;
		}
	}
	else if(RF_DEFENDER == raid_faction)
	{       
		for(size_t i = 0; i < _defender_pos_list.size(); ++i)
		{
			if(_defender_pos_list[i].is_used) continue;

			_defender_pos_list[i].roleid = roleid;
			_defender_pos_list[i].is_used = true;
			pos = _defender_pos_list[i].pos;
			return;
		}
	}
	else
	{
	}
}


void collision_raid_world_manager::ClrEntryPos(int roleid, int raid_faction)
{
	if(RF_ATTACKER == raid_faction)
	{       
		for(size_t i = 0; i < _attacker_pos_list.size(); ++i)
		{
			if(_attacker_pos_list[i].roleid == roleid) 
			{
				_attacker_pos_list[i].roleid = roleid;
				_attacker_pos_list[i].is_used = false;
			}

		}
	}
	else if(RF_DEFENDER == raid_faction)
	{       
		for(size_t i = 0; i < _defender_pos_list.size(); ++i)
		{
			if(_defender_pos_list[i].roleid == roleid) 
			{
				_defender_pos_list[i].roleid = roleid;
				_defender_pos_list[i].is_used = false;
			}

		}
	}
	else
	{
	}
}

#define PI 3.14159265
bool collision_raid_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	world_tag = GetWorldTag();
	
	A3DVECTOR center_pos(2.647, 220.8, -247.801); 
	float angel = abase::Rand(float(0.0), (float)(2*PI));
	float radius = abase::Rand(float(3.0), float(35.0));
	
	A3DVECTOR new_pos;
	pos.x = center_pos.x + radius * cos(angel); 
	pos.y = center_pos.y; 
	pos.z = center_pos.z + radius * sin(angel); 
	return true;
}


void collision_raid_world_manager::AddAttackerScore(int score)
{
	spin_autolock keeper( _raid_lock );

	IncAttackerScore(score);
	if(GetAttackerScore() >= 40 && _raid_result == 0)
	{
		_raid_result = RAID_ATTACKER_WIN; 
	}
}

void collision_raid_world_manager::AddDefenderScore(int score)
{
	spin_autolock keeper( _raid_lock );

	IncDefenderScore(score);
	if(GetDefenderScore() >= 40 && _raid_result == 0)
	{
		_raid_result = RAID_DEFENDER_WIN; 
	}
}

void collision_raid_world_manager::IncAttackerScore(int score)
{
	_attacker_score += score;
	UpdateScore();
}

void collision_raid_world_manager::IncDefenderScore(int score)
{
	_defender_score += score;
	UpdateScore();
}

void collision_raid_world_manager::UpdateScore()
{
        packet_wrapper h1(64);
        using namespace S2C;
        CMD::Make<CMD::collision_raid_score>::From(h1, GetAttackerScore(), GetDefenderScore(), GetRaidMaxKillerID(), _player_list.size(), _player_list.begin());
        if(_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);	
}

void collision_raid_world_manager::GetAllPlayerID(std::vector<int> & id_list)
{
	spin_autolock keeper( _raid_lock );

	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		id_list.push_back(_player_list[i].roleid);
	}
}


//服务器碰撞检测算法, 目前不使用
/*
void collision_raid_world_manager::CheckCollision()
{
	std::map<int, collision_player_info>::iterator iter1;
	std::map<int, collision_player_info>::iterator iter2;
	std::set<int> collision_ids;

	for(iter1 = _collision_player.begin(); iter1 != _collision_player.end(); ++iter1)
	{
		int roleid1 = iter1->first;
		if(collision_ids.find(roleid1) != collision_ids.end()) continue;
		if(iter1->second.col_timer > 0) 
		{
			iter1->second.col_timer--;
			continue;
		}

		iter2 = iter1;
		++iter2;
		for(; iter2 != _collision_player.end(); ++iter2)
		{
			int roleid2 = iter2->first;
			if(collision_ids.find(roleid2) != collision_ids.end()) continue;
			if(iter2->second.col_timer > 0) continue;

			if(IsCollision2(roleid1, iter1->second.cur_speed, roleid2, iter2->second.cur_speed))
			{
				collision_ids.insert(roleid1);
				collision_ids.insert(roleid2);

				iter1->second.col_timer = 30;
				iter2->second.col_timer = 30;
			}
		}
	}
}

#define PI 3.141592
bool collision_raid_world_manager::IsCollision(int roleid1, short role1_speed, int roleid2, short role2_speed)
{
	world::object_info info1;
	world * plane = GetWorldByIndex(0);

	if(!plane->QueryObject(XID(GM_TYPE_PLAYER, roleid1), info1)) return false;
	unsigned char role1_dir = info1.dir;
	A3DVECTOR & role1_pos = info1.pos;

	world::object_info info2;
	if(!plane->QueryObject(XID(GM_TYPE_PLAYER, roleid2), info2)) return false;
	unsigned char role2_dir = info2.dir;
	A3DVECTOR & role2_pos = info2.pos;

	float distance = role1_pos.horizontal_distance(role2_pos);
	if(distance < 3 * 3)
	{
		float col_ang = 0.0f;
		if(role2_pos.x - role1_pos.x == 0)
		{
			col_ang = (1.0/2.0)*PI; 
		}
		else
		{
			col_ang = atan((role2_pos.z - role1_pos.z) / (role2_pos.x - role1_pos.x));
		}
		col_ang = (col_ang/PI) * 180;

		float role1_ang = (role1_dir)/255.0 * 360.0;
		float role2_ang = (role2_dir)/255.0 * 360.0;
				
		float role1_col_ang = role1_ang - col_ang;
		float role2_col_ang = role2_ang - col_ang;


		float role1_vspeed = (role2_speed) * cos(role2_col_ang/180*PI);
		float role2_vspeed = (role1_speed) * cos(role1_col_ang/180*PI);

		float role1_hspeed = (role1_speed) * sin(role1_col_ang/180*PI);
		float role2_hspeed = (role2_speed) * sin(role2_col_ang/180*PI);


		float role1_rspeed = sqrt(role1_vspeed * role1_vspeed + role1_hspeed * role1_hspeed);
		float role2_rspeed = sqrt(role2_vspeed * role2_vspeed + role2_hspeed * role2_hspeed);

		float role1_rang = acos(role1_vspeed/role1_rspeed);
		role1_rang = role1_rang/PI*180 + col_ang;
		if(role1_vspeed < 0) role1_rang += 180;
		else if(role1_hspeed < 0 && role1_vspeed > 0) role1_rang += 360;

		float role2_rang = acos(role2_vspeed/role2_rspeed);
		role2_rang = role2_rang/PI*180 + col_ang;
		if(role2_vspeed < 0) role2_rang += 180;
		else if(role2_hspeed < 0 && role2_vspeed > 0) role2_rang += 360;

		if(role1_rang >= 360.0) role1_rang -= 360;
		if(role2_rang >= 360.0) role2_rang -= 360;

		if(role1_rang < 0) role1_rang += 360;
		if(role2_rang < 0) role2_rang += 360;

		__PRINTF("发生碰撞了, roleid1=%d, roleid2=%d, role1_speed=%f, role2_speed=%f, role1_ang=%f, role2_ang=%f\n", 
				roleid1, roleid2, role1_rspeed, role2_rspeed, role1_rang, role2_rang);
		return true;
	}

	return false;
}

bool collision_raid_world_manager::IsCollision2(int roleid1, short role1_speed, int roleid2, short role2_speed)
{
	world * plane = GetWorldByIndex(0);

	world::object_info info1;
	if(!plane->QueryObject(XID(GM_TYPE_PLAYER, roleid1), info1)) return false;
	A3DVECTOR & role1_pos = info1.pos;
	A3DVECTOR & role1_oldpos = info1.old_pos;
	A3DVECTOR  role1_vpos(role1_pos.x-role1_oldpos.x, role1_pos.y-role1_oldpos.y, role1_pos.z - role1_oldpos.z);
	role1_vpos.normalize();
	A3DVECTOR role1_vspeed(role1_vpos.x*role1_speed, role1_vpos.y*role1_speed, role1_vpos.z*role1_speed); 

	world::object_info info2;
	if(!plane->QueryObject(XID(GM_TYPE_PLAYER, roleid2), info2)) return false;
	A3DVECTOR & role2_pos = info2.pos;
	A3DVECTOR & role2_oldpos = info2.old_pos;

	A3DVECTOR  role2_vpos(role2_pos.x-role2_oldpos.x, role2_pos.y-role2_oldpos.y, role2_pos.z - role2_oldpos.z);
	role2_vpos.normalize();
	A3DVECTOR role2_vspeed(role2_vpos.x*role2_speed, role2_vpos.y*role2_speed, role2_vpos.z*role2_speed); 
	
	float distance = role1_pos.squared_distance(role2_pos);
	if(distance < 3 * 3)
	{
		A3DVECTOR  col_pos = role2_pos; 
		col_pos -= role1_pos;
		col_pos.normalize();

		float role1_dot = (role1_vspeed.dot_product(col_pos));
		A3DVECTOR role1_qspeed(col_pos.x * role1_dot, col_pos.y * role1_dot, col_pos.z * role1_dot);
		A3DVECTOR role1_fspeed = role1_vspeed; 
		role1_fspeed -= role1_qspeed;
		
		col_pos = role1_pos; 
		col_pos -= role2_pos;
		col_pos.normalize();
		float role2_dot = role2_vspeed.dot_product(col_pos);
		A3DVECTOR role2_qspeed(col_pos.x * role2_dot, col_pos.y * role2_dot, col_pos.z * role2_dot);
		A3DVECTOR role2_fspeed = role2_vspeed;
		role2_fspeed -= role2_qspeed;

		A3DVECTOR role1_rspeed = role2_qspeed;
		role1_rspeed += role1_fspeed;

		A3DVECTOR role2_rspeed = role1_qspeed;
		role2_rspeed += role2_fspeed;

		msg_player_collision mpc;
		mpc.role_id1 = roleid1;
		mpc.role_id2 = roleid2;
		mpc.role1_speed = role1_rspeed;
		mpc.role2_speed = role2_rspeed;

		MSG msg;
		BuildMessage(msg, GM_MSG_PLAYER_COLLISION, XID(GM_TYPE_PLAYER, roleid1), XID(-1,-1), A3DVECTOR(0,0,0), 0, &mpc, sizeof(mpc)); 
		gmatrix::SendMessage(msg);

		__PRINTF("发生碰撞了, roleid1=%d, roleid2=%d, role1_speed=(%f, %f, %f), role2_speed=(%f, %f, %f)\n", roleid1, roleid2, role1_rspeed.x, role1_rspeed.y, role1_rspeed.z,
				role2_rspeed.x, role2_rspeed.y, role2_rspeed.z); 
		return true;
	}
	return false;

}

void collision_raid_world_manager::SetRoleSpeed(int roleid, short speed)
{
	std::map<int, collision_player_info>::iterator iter;
	for(iter = _collision_player.begin(); iter != _collision_player.end(); ++iter)
	{
		if(iter->first == roleid)
		{
			iter->second.cur_speed = speed;
			return;
		}
	}
}

*/

collision2_raid_world_manager::collision2_raid_world_manager()
{
	_attacker_cheat_counter = 0;
	_defender_cheat_counter = 0;
	_attacker_cheat_on = false;
	_defender_cheat_on = false;
}

collision2_raid_world_manager::~collision2_raid_world_manager()
{
}

void collision2_raid_world_manager::Reset()
{
	_attacker_cheat_counter = 0;
	_defender_cheat_counter = 0;
	_attacker_cheat_on = false;
	_defender_cheat_on = false;
	collision_raid_world_manager::Reset();

}


#define PI 3.14159265
bool collision2_raid_world_manager::GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag)
{
	world_tag = GetWorldTag();
	
	A3DVECTOR center_pos(237.351, 308, 269.107);
	float angel = abase::Rand(float(0.0), (float)(2*PI));
	float radius = abase::Rand(float(3.0), float(70.0));
	
	A3DVECTOR new_pos;
	pos.x = center_pos.x + radius * cos(angel); 
	pos.y = center_pos.y; 
	pos.z = center_pos.z + radius * sin(angel); 
	return true;
}

void collision2_raid_world_manager::OnCheckCheat()
{
	int attacker_count = 0;
	int defender_count = 0;

	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].is_online && _player_list[i].faction == RF_ATTACKER)
		{
			attacker_count++;
		}
		else if(_player_list[i].is_online && _player_list[i].faction == RF_DEFENDER)
		{
			defender_count++;
		}
	}

	if(!_attacker_cheat_on && attacker_count <= 4)
	{
		_attacker_cheat_on = true;
		NotifyCheatInfo(RF_ATTACKER, _attacker_cheat_on, _attacker_cheat_counter);
	}
	else if(_attacker_cheat_on && attacker_count > 4)
	{
		_attacker_cheat_on = false;
		NotifyCheatInfo(RF_ATTACKER, _attacker_cheat_on, _attacker_cheat_counter);
	}

	if(!_defender_cheat_on && defender_count <= 4)
	{
		_defender_cheat_on = true;
		NotifyCheatInfo(RF_DEFENDER, _defender_cheat_on, _defender_cheat_counter);

	}
	else if(_defender_cheat_on && defender_count > 4)
	{
		_defender_cheat_on = false;
		NotifyCheatInfo(RF_DEFENDER, _defender_cheat_on, _defender_cheat_counter); 
	}

	if(_attacker_cheat_on)
	{
		_attacker_cheat_counter++;
	}
	if(_defender_cheat_on)
	{
		_defender_cheat_counter++;
	}

	if(_attacker_cheat_counter >= 180 && _defender_cheat_counter >= 180)
	{
		_raid_result = RAID_DRAW;
	}
	else if(_attacker_cheat_counter >= 180)
	{
		_raid_result = RAID_DEFENDER_WIN;
	}
	else if(_defender_cheat_counter >= 180)
	{
		_raid_result = RAID_ATTACKER_WIN; 
	}


}

void collision2_raid_world_manager::NotifyCheatInfo(int faction, bool is_cheat, int cheat_counter) 
{
	for(size_t i = 0; i < _player_list.size(); ++i)
	{
		if(_player_list[i].is_online && _player_list[i].faction == faction)
		{
			msg_cs6v6_cheat_info mcc;
			mcc.is_cheat = is_cheat;
			mcc.cheat_counter = cheat_counter;
				
			MSG msg;
			BuildMessage(msg, GM_MSG_CS6V6_CHEAT_INFO, XID(GM_TYPE_PLAYER, _player_list[i].roleid), XID(-1,-1), A3DVECTOR(0,0,0), 0, &mcc, sizeof(mcc)); 
			gmatrix::SendMessage(msg);
		}
	}
}

void collision2_raid_world_manager::SendCheatInfo(int roleid, int faction)
{
	if(g_timer.get_systime() > _start_timestamp + 60)
	{
		OnCheckCheat();
	}
	bool is_cheat = false;
	int cheat_counter = 0;
	if(faction == RF_ATTACKER)
	{
		is_cheat = _attacker_cheat_on;
		cheat_counter = _attacker_cheat_counter;
	}	
	else if(faction == RF_DEFENDER)
	{
		is_cheat = _defender_cheat_on;
		cheat_counter = _defender_cheat_counter;
	}
	if(!is_cheat) return;

	msg_cs6v6_cheat_info mcc;
	mcc.is_cheat = is_cheat;
	mcc.cheat_counter = cheat_counter;

	MSG msg;
	BuildMessage(msg, GM_MSG_CS6V6_CHEAT_INFO, XID(GM_TYPE_PLAYER, roleid), XID(-1,-1), A3DVECTOR(0,0,0), 0, &mcc, sizeof(mcc)); 
	gmatrix::SendMessage(msg);
}

