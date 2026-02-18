#include "seek_raid_player.h"
#include "seek_raid_world_manager.h"
#include "../playertransform.h"
#include <factionlib.h>

int seek_raid_world_manager::_random_cost_score = 40;

seek_raid_world_manager::seek_raid_world_manager()
{
	_next_round_counter = PREPARE_COUNTER;
	_seek_round = 0;
	_deny_hide_start_counter = 0;
	_deny_hide_end_counter = 0;
	_deny_seeker_start_counter = 0;

	memset(_player_status_arr,0,sizeof(_player_status_arr));
	memset(_round_flag,0,sizeof(_round_flag));
	memset(_round_end_time,0,sizeof(_round_end_time));

	_basic2cost_map.insert(std::make_pair((int)HIDE_SKILL0,10));
	_basic2cost_map.insert(std::make_pair((int)HIDE_SKILL1,10));
	_basic2cost_map.insert(std::make_pair((int)HIDE_SKILL2,10));
	_basic2cost_map.insert(std::make_pair((int)HIDE_SKILL3,10));
	_basic2cost_map.insert(std::make_pair((int)HIDE_SKILL4,10));

	_advance2cost_map.insert(std::make_pair((int)HIDE_SKILL5,50));
	_advance2cost_map.insert(std::make_pair((int)HIDE_SKILL6,50));
	_advance2cost_map.insert(std::make_pair((int)HIDE_SKILL7,50));
	_advance2cost_map.insert(std::make_pair((int)HIDE_SKILL8,100));
	_advance2cost_map.insert(std::make_pair((int)HIDE_SKILL9,50));

	_basic2win_map.insert(std::make_pair((int)HIDE_SKILL0,20));
	_basic2win_map.insert(std::make_pair((int)HIDE_SKILL1,20));
	_basic2win_map.insert(std::make_pair((int)HIDE_SKILL2,20));
	_basic2win_map.insert(std::make_pair((int)HIDE_SKILL3,20));
	_basic2win_map.insert(std::make_pair((int)HIDE_SKILL4,20));

	_advance2win_map.insert(std::make_pair((int)HIDE_SKILL5,80));
	_advance2win_map.insert(std::make_pair((int)HIDE_SKILL6,80));
	_advance2win_map.insert(std::make_pair((int)HIDE_SKILL7,80));
	_advance2win_map.insert(std::make_pair((int)HIDE_SKILL8,50));
	_advance2win_map.insert(std::make_pair((int)HIDE_SKILL9,80));

	_skill2achieve_map.insert(std::make_pair((int)HIDE_SKILL5,107));
	_skill2achieve_map.insert(std::make_pair((int)HIDE_SKILL6,76));
	_skill2achieve_map.insert(std::make_pair((int)HIDE_SKILL7,19));
	_skill2achieve_map.insert(std::make_pair((int)HIDE_SKILL8,36));
	_skill2achieve_map.insert(std::make_pair((int)HIDE_SKILL9,315));
}

seek_raid_world_manager::~seek_raid_world_manager()
{
	_seek_round = 0;
	_players_score_map.clear();
	_seek_player_info_map.clear();

	_debug_seeker_set.clear();
	_debug_hider_set.clear();
}

void seek_raid_world_manager::OnPlayerEnterWorld(gplayer* pPlayer, bool reenter, int faction)
{
	if (!reenter)
	{
		int i = 0;
		for ( ; i < MAX_PLAYER; i++)
		{
			player_status & ps = _player_status_arr[i];
			if (!ps.id)
			{
				ps.id = pPlayer->ID.id;
				ps.status = STATUS_ONLINE;
				break;
			}
		}
		seek_score score;
		score.num = 100;
		score.time = _seek_heartbeat_counter;
		_players_score_map.insert(std::make_pair(pPlayer->ID.id,score));

		//为电脑特别慢的玩家准备
		if (_seek_round == ROUND_ONE_PREPARE || _seek_round == ROUND_ONE_START)
		{
			int player_id = pPlayer->ID.id;
			if (_seek_player_info_map.find(player_id) != _seek_player_info_map.end())
				return;
			__PRINTF("SLOW PLAYER %d,index=%d\n",player_id,i);
			_slow_player_map.insert(std::make_pair(player_id,i));
		}
		else if (_seek_round >= ROUND_ONE_STOP)
		{
			seek_raid_player_imp* pImp = (seek_raid_player_imp*)pPlayer->imp;
			pImp->SendClientRaidRoundInfo(_seek_round,_round_end_time[_seek_round]);
		}
	}
}

void seek_raid_world_manager::OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction) 
{
	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;

	SeekPlayerInfoMap::iterator it = _seek_player_info_map.find(pPlayer->ID.id);
	if (it != _seek_player_info_map.end())
	{
		seek_player_info& hsInfo = it->second;
		PlayerSkillMap::iterator it2 = hsInfo.skills.begin();
		for (; it2 != hsInfo.skills.end(); ++it2)
		{
			player_sys_skill& s = it2->second;
			pImp->ClearSysSkill(s.skill_id);
		}
	}

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		player_status & ps  = _player_status_arr[i];
		if (ps.id == pPlayer->ID.id)
		{
			ps.status = STATUS_TMPLEFT;
			ps.leavePos = pPlayer->pos;
			ps.disconnect_time = g_timer.get_systime();
			break;
		}
	}
	UpdatePlayerStatus();
	SendClientPlayerScoreInfo();
}

void seek_raid_world_manager::Reset()
{
	raid_world_manager::Reset();

	_next_round_counter = PREPARE_COUNTER;
	_seek_round = 0;
	_deny_hide_start_counter = 0;
	_deny_hide_end_counter = 0;
	_deny_seeker_start_counter = 0;
	memset(_round_flag,0,sizeof(_round_flag));
	memset(_round_end_time,0,sizeof(_round_end_time));
	memset(_player_status_arr,0,sizeof(_player_status_arr));
	_players_score_map.clear();
	_seek_player_info_map.clear();
	_slow_player_map.clear();
}

struct playerscore
{
	int playerid;
	seek_score score;
};

struct
{
	bool operator() (const playerscore & lhs,const playerscore & rhs)
	{
		return lhs.score.num > rhs.score.num || (lhs.score.num == rhs.score.num && lhs.score.time < rhs.score.time);
	}
} score_cmp;

void seek_raid_world_manager::OnClose()
{
/*	std::vector<playerscore> vec_ps;
	ScoreMap::const_iterator it = _players_score_map.begin();
	for (; it != _players_score_map.end(); ++it)
	{
		playerscore ps;
		ps.playerid = it->first;
		ps.score = it->second;
		vec_ps.push_back(ps);
	}
	std::sort(vec_ps.begin(),vec_ps.end(),score_cmp);

	for (size_t i = 0; i < vec_ps.size(); ++i)
	{
		int itemid = 62926;
		if (i == 0) itemid = 62923;
		else if (i == 1) itemid = 62924;
		else if (i == 2) itemid = 62925;
		MSG msg;
		BuildMessage(msg,GM_MSG_SEEK_RAID_AWARD_ITEM,XID(GM_TYPE_PLAYER,vec_ps[i].playerid),XID(-1,-1),A3DVECTOR(0,0,0),itemid);
		gmatrix::SendMessage(msg);
	}

*/	
	raid_world_manager::OnClose();
}

void seek_raid_world_manager::OnRunning()
{
	// raid_world_manager::OnRunning();

	//处理多次进副本delievery冷却时间
	if( _cur_timestamp < g_timer.get_systime() || _raid_result > 0)
	{
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

	if (_seek_heartbeat_counter >= _next_round_counter) _seek_round++;

	switch (_seek_round)
	{
		case ROUND_ONE_PREPARE:
		{
			if (!_round_flag[_seek_round])
			{
				_seek_player_info_map.clear();
				_round_flag[_seek_round] = ROUND_IN;
				_next_round_counter = _seek_heartbeat_counter + PREPARE_TIME;
				UpdatePlayerStatus();
				SeekRaidPrepare(true);
				int end_timestamp = g_timer.get_systime() + 60;
				_round_end_time[_seek_round] = end_timestamp;
				SendClientRaidRoundInfo(_seek_round,end_timestamp);
				BroadcastHideAndSeekRoleMsg();
				SendClientPlayerScoreInfo();
			}
		}
		break;

		case ROUND_TWO_PREPARE:
		case ROUND_THIRD_PREPARE:
		{
			if (!_round_flag[_seek_round])
			{
				_seek_player_info_map.clear();
				_round_flag[_seek_round] = ROUND_IN;
				_next_round_counter = _seek_heartbeat_counter + PREPARE_TIME;
				UpdatePlayerStatus();
				SeekRaidPrepare(false);
				int end_timestamp = g_timer.get_systime() + 60;
				_round_end_time[_seek_round] = end_timestamp;
				SendClientRaidRoundInfo(_seek_round, end_timestamp);
				BroadcastHideAndSeekRoleMsg();
				SendClientPlayerScoreInfo();
			}
		}
		break;

		case ROUND_ONE_START:
		case ROUND_TWO_START:
		case ROUND_THIRD_START:
		{
			if (!_round_flag[_seek_round])
			{
				_deny_hide_start_counter = _seek_heartbeat_counter + 7200;
				_deny_hide_end_counter = _seek_heartbeat_counter + 9600;
				_deny_seeker_start_counter = _seek_heartbeat_counter + 6000;
				_next_round_counter = _seek_heartbeat_counter + GAME_TIME;
				_round_flag[_seek_round] = ROUND_IN;

				SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
				for ( ; it != _seek_player_info_map.end(); ++it)
				{
					seek_player_info & hsInfo = it->second;
					if (!hsInfo.ishide)
					{
						MSG msg;
						BuildMessage(msg,GM_MSG_SEEK_START,XID(GM_TYPE_PLAYER,it->first),XID(-1,-1),A3DVECTOR(0,0,0));
						gmatrix::SendMessage(msg);
					}
				}
				int end_timestamp = g_timer.get_systime() + 480;
				_round_end_time[_seek_round] = end_timestamp;
				SendClientRaidRoundInfo(_seek_round, end_timestamp);
			}
			else if (_round_flag[_seek_round] == ROUND_IN)
			{
				int win = GetWinnerOnCastSkill();
				if (win)
				{
					_next_round_counter = _seek_heartbeat_counter;
					_round_flag[_seek_round] = win;
				}
			}
			UpdatePlayerStatus();
			SendClientPlayerScoreInfo();
		}
		break;

		case ROUND_ONE_STOP:
		case ROUND_TWO_STOP:
		case ROUND_THIRD_STOP:
		{
			if (!_round_flag[_seek_round])
			{
				_deny_hide_start_counter = 0;
				_deny_hide_end_counter = 0;
				_next_round_counter = _seek_heartbeat_counter + REST_TIME;
				_round_flag[_seek_round] = ROUND_IN;
				SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
				for ( ; it != _seek_player_info_map.end(); ++it)
				{
					seek_player_info & hsInfo = it->second;
					MSG msg;
					int ishide = hsInfo.ishide;
					BuildMessage(msg,GM_MSG_SEEK_STOP,XID(GM_TYPE_PLAYER,it->first),XID(-1,-1),A3DVECTOR(0,0,0),ishide);
					gmatrix::SendMessage(msg);
				}
				if (_seek_round != ROUND_THIRD_STOP)
				{
					int end_timestamp = g_timer.get_systime() + 30;
					_round_end_time[_seek_round] = end_timestamp;
					SendClientRaidRoundInfo(_seek_round, end_timestamp);
				}
				else
				{
					packet_wrapper h1(64);
					using namespace S2C;
					CMD::Make<CMD::hide_and_seek_leave_raid>::From(h1);
					if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1); 
					_raid_result = 1;
				}
				UpdatePlayerStatus();
				CalcScoreOnFinishRound();
				SendClientPlayerScoreInfo();
			}
		}
		break;
	}
}


bool seek_raid_world_manager::CanCastSkillInWorld(int skill_id, int who)
{
	spin_autolock keeper(_raid_lock);
	SeekPlayerInfoMap::iterator it = _seek_player_info_map.find(who);
	if (it == _seek_player_info_map.end())
	{
		return false;
	}
	seek_player_info& hsInfo = it->second;

	if (hsInfo.ishide)
	{
		if (skill_id == hsInfo.random_skill && !hsInfo.used_random_skill)
		{
			hsInfo.used_random_skill = true;
			_players_score_map[who].num -= 40;
			_players_score_map[who].time = _seek_heartbeat_counter;
			hsInfo.last_used_skill = skill_id;
			return true;
		}
		if (_seek_round % 3 == 0) return false;
		if (skill_id == TAUNTED_SKILL)
		{
			if (_seek_round % 3 != ROUND_ONE_START || _seek_heartbeat_counter < _deny_seeker_start_counter) return false;
		}
		else
		{
			PlayerSkillMap::iterator it2 = hsInfo.skills.find(skill_id);
			if (it2 == hsInfo.skills.end())
			{
				return false;
			}

			//每局比赛结束前2分钟不再允许变换变身
			if (_deny_hide_start_counter <= _seek_heartbeat_counter && _seek_heartbeat_counter <= _deny_hide_end_counter)
			{
				return false;
			}

			if (_basic2cost_map.find(skill_id) != _basic2cost_map.end())
			{
				if (_players_score_map[who].num < _basic2cost_map[skill_id]) return false;
				_players_score_map[who].num -= _basic2cost_map[skill_id];
			}
			else
			{
				if (_players_score_map[who].num < _advance2cost_map[skill_id]) return false;
				_players_score_map[who].num -= _advance2cost_map[skill_id];
			}
			_players_score_map[who].time = _seek_heartbeat_counter;
		}
	}
	else
	{
		PlayerSkillMap::iterator it2 = hsInfo.skills.find(skill_id);
		if (it2 == hsInfo.skills.end())
		{
			return false;
		}
		player_sys_skill & s = it2->second;
		if (s.used_cnt >= s.can_use_cnt) return false;
		s.used_cnt++;
		if (s.used_cnt >= s.can_use_cnt) hsInfo.seeker_exhaust_skill = true;
		char skill_left = 0,skill_buy_num = 0;
		skill_left = s.can_use_cnt - s.used_cnt > 0 ? s.can_use_cnt - s.used_cnt: 0;
		skill_buy_num = s.buy_cnt;

		MSG msg;
		BuildMessage2(msg,GM_MSG_SEEK_RAID_SKILL_LEFT,XID(GM_TYPE_PLAYER,who),XID(-1,-1),A3DVECTOR(0,0,0),skill_left,skill_buy_num);
		gmatrix::SendMessage(msg);
	}
	if (skill_id >= HIDE_SKILL0 && skill_id <= HIDE_SKILL9) hsInfo.last_used_skill = skill_id;
	__PRINTF("Seek Raid Use Skillid = %d\n",skill_id);
	return true;
}

void seek_raid_world_manager::OnCastSkill(gactive_imp* pImp, int skill_id)
{
	if (skill_id >= HIDE_SKILL0 && skill_id <= HIDE_SKILL9)
	{
		for (int i = HIDE_SKILL0; i <= HIDE_SKILL9; i++)
			pImp->SetCoolDown(i+SKILL_COOLINGID_BEGIN,60*1000);
	}
	SendClientPlayerScoreInfo();
}

bool seek_raid_world_manager::PlayerBuySeekerSkill(gplayer_imp* pImp)
{
	spin_autolock keeper(_raid_lock);
	int playerid = pImp->GetParent()->ID.id;

	SeekPlayerInfoMap::iterator it = _seek_player_info_map.find(playerid);
	if (it == _seek_player_info_map.end()) return false;
	seek_player_info &hsInfo = it->second;
	if (hsInfo.ishide) return false;
	player_sys_skill & s = hsInfo.skills[(int)SEEKER_SKILL];
	int score[4] = {5,10,20,40};
	int cost_score = s.buy_cnt < 4 ? *(score+s.buy_cnt) : 80;
	if (cost_score > _players_score_map[playerid].num) return false;
	s.buy_cnt++;
	s.can_use_cnt++;
	_players_score_map[playerid].num -= cost_score;
	_players_score_map[playerid].time = _seek_heartbeat_counter;

	char skill_left = 0,skill_buy_num = 0;
	skill_left = s.can_use_cnt - s.used_cnt > 0 ? s.can_use_cnt - s.used_cnt: 0;
	skill_buy_num = s.buy_cnt;
	hsInfo.seeker_exhaust_skill = false;
	
	MSG msg;
	BuildMessage2(msg,GM_MSG_SEEK_RAID_SKILL_LEFT,XID(GM_TYPE_PLAYER,playerid),XID(-1,-1),A3DVECTOR(0,0,0),skill_left,skill_buy_num);
	gmatrix::SendMessage(msg);
	SendClientPlayerScoreInfo();
	return true;
}

void seek_raid_world_manager::OnSeekRaidKill(int player_id, int attacker_id)
{
	spin_autolock keeper(_raid_lock);
	SeekPlayerInfoMap::iterator it = _seek_player_info_map.find(player_id);
	seek_player_info & hsInfo = it->second;
	if (hsInfo.ishide && hsInfo.attacked_cnt < 2)
	{
		MSG msg;
		hsInfo.attacked_cnt += 1;
		BuildMessage2(msg,GM_MSG_SEEK_RAID_KILL,XID(GM_TYPE_PLAYER,player_id),XID(-1,-1),A3DVECTOR(0,0,0),hsInfo.attacked_cnt,attacker_id);
		gmatrix::SendMessage(msg);
		if (hsInfo.attacked_cnt == 2)
		{
			SendClientHideAndSeekRole(player_id,2);
			int win = GetWinnerOnCastSkill();
			if (win)
			{
				_next_round_counter = _seek_heartbeat_counter;
				_round_flag[_seek_round] = win;
			}
		}
		_players_score_map[attacker_id].num += 5 * hsInfo.attacked_cnt;
		_players_score_map[attacker_id].time = _seek_heartbeat_counter ;
		SendClientPlayerScoreInfo();
	}
}

void seek_raid_world_manager::OnSeekRaidHiderTaunted(int player_id, const A3DVECTOR pos, bool is_start)
{
	spin_autolock keeper(_raid_lock);
	A3DVECTOR sendpos = pos;
	if (is_start)
	{
		sendpos.z = pos.z;
		sendpos.x = pos.x + abase::Rand(-14,14);
		sendpos.y = pos.y + abase::Rand(-14,14);	
	}
	packet_wrapper h1(64);
	using namespace S2C;	
	CMD::Make<CMD::hide_and_seek_hider_taunted>::From(h1,player_id,sendpos,is_start);
	if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1); 
}

void seek_raid_world_manager::OnSeekRaidHiderTauntedGetScore(int player_id)
{
	spin_autolock keeper(_raid_lock);
	_players_score_map[player_id].num += 10;
	_players_score_map[player_id].time += _seek_heartbeat_counter;
	SendClientPlayerScoreInfo();
}

void seek_raid_world_manager::SendSeekSkillInfo(int player_id, int ishide)
{
	if (_debug_seeker_set.find(player_id) != _debug_seeker_set.end())
	{
		ishide = false;
		
	}
	else if (_debug_hider_set.find(player_id) != _debug_hider_set.end())
	{
		ishide = true;
	}

	MSG msg;
	BuildMessage(msg,GM_MSG_SEEK_PREPARE,XID(GM_TYPE_PLAYER,player_id),XID(-1,-1),A3DVECTOR(0,0,0),ishide);
	gmatrix::SendMessage(msg);

	seek_player_info player_info;
	player_info.ishide = ishide;


	if (player_info.ishide) //躲藏者
	{
		for (int i = HIDE_SKILL0; i <= HIDE_SKILL9; ++i)
		{
			player_sys_skill skill;
			skill.skill_id = i;
			skill.used_cnt = 0;
			skill.can_use_cnt = 20;
			skill.buy_cnt = 0;
			skill.skill_level = 1;
			player_info.skills.insert(std::make_pair(skill.skill_id,skill));
		}
	}
	else
	{
		player_sys_skill skill;
		skill.skill_id = SEEKER_SKILL;
		skill.used_cnt = 0;
		skill.can_use_cnt = 20;
		skill.buy_cnt = 0;
		skill.skill_level = 1;
		player_info.skills.insert(std::make_pair(skill.skill_id,skill));
	}
	if (_seek_player_info_map.find(player_id) == _seek_player_info_map.end())
	{
		_seek_player_info_map.insert(std::make_pair(player_id,player_info));
	}
}

void seek_raid_world_manager::UpdatePlayerStatus()
{
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		player_status & ps  = _player_status_arr[i];
		if (!ps.id) break;
		if (ps.status == STATUS_TMPLEFT 
		&& g_timer.get_systime() - ps.disconnect_time > 300)
		{
			ps.status = STATUS_LEFT;
		}
	}
			
}

void seek_raid_world_manager::SendClientRaidRoundInfo(int step, int end_timestamp)
{
	packet_wrapper h1(64);
	using namespace S2C;
	char round = (step-1)/3;
	char round_status = (step-1)%3;
	CMD::Make<CMD::hide_and_seek_round_info>::From(h1,round,round_status, end_timestamp);
	if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
}

void seek_raid_world_manager::SeekRaidPrepare(bool isfirst)
{
	if (isfirst)
	{
		for (int i = 0,j = 0; i < MAX_PLAYER; ++i)
		{
			player_status & pstatus = _player_status_arr[i];
			if (!pstatus.id) break;
			if (pstatus.status)
			{
				bool ishide = j % 5 ? true:false;
				j++;
				SendSeekSkillInfo(pstatus.id,ishide);
			}
		}
	}
	else
	{
		size_t player_num = 0;
		size_t seek_num = 0;
		int rand_id_arr[MAX_PLAYER] = {0};
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			player_status & pstatus = _player_status_arr[i];
			if (!pstatus.id) break;
			if (pstatus.status)
			{
				rand_id_arr[player_num++] = pstatus.id;
			}
		}
		if (player_num)
		{
			std::random_shuffle(rand_id_arr,rand_id_arr+player_num);
			seek_num = player_num%5 ? (player_num/5+1):(player_num/5);
		}

		for (size_t i = 0; i < player_num; ++i)
		{
			bool ishide = i < seek_num ? false : true;
			SendSeekSkillInfo(rand_id_arr[i],ishide);
		}
	}
}

void seek_raid_world_manager::SendClientPlayerScoreInfo()
{
	using namespace S2C;
	typedef CMD::hide_and_seek_players_info::seek_player_info seek_player_score_info;
	std::vector<seek_player_score_info> vec_info;

	SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
	for ( ; it != _seek_player_info_map.end(); ++it)
	{
		seek_player_score_info info;
		info.playerid = it->first;
		info.score = _players_score_map[it->first].num;
		seek_player_info & hsInfo = it->second;
		if (_seek_round % 3 == ROUND_ONE_PREPARE)
		{
			info.status = ENUM_PREPARE;
			info.occupation = hsInfo.ishide ? OCCUPATION_HIDE : OCCUPATION_SEEK;
		}
		else if (_seek_round % 3 == ROUND_ONE_START)
		{
			char status;
			for (int i = 0; i < MAX_PLAYER; ++i)
			{
				player_status & ps = _player_status_arr[i];
				if (ps.id == it->first)
				{
					status = ps.status;
					break;
				}
			}
			if (hsInfo.ishide) //躲藏者
			{
				if (hsInfo.attacked_cnt == 2)
					info.status = ENUM_OUT;
				else if (status == STATUS_TMPLEFT)
					info.status = ENUM_TMPLEFT;
				else if (status == STATUS_LEFT)
					info.status = ENUM_LEFT;
				else
					info.status = ENUM_GAME;
			}
			else
			{
				if (hsInfo.seeker_exhaust_skill)
					info.status = ENUM_OUT;
				else if (status == STATUS_TMPLEFT)
					info.status = ENUM_TMPLEFT;
				else if (status == STATUS_LEFT)
					info.status = ENUM_LEFT;
				else
					info.status = ENUM_GAME;
			}
			info.occupation = hsInfo.ishide ? OCCUPATION_HIDE : OCCUPATION_SEEK;
		}
		else
		{
			int win = _round_flag[_seek_round-1];
			if (hsInfo.ishide)
				info.status = win == HIDER_WIN ? ENUM_WIN:ENUM_FAIL;
			else
				info.status = win == SEEKER_WIN ? ENUM_WIN:ENUM_FAIL;
			info.occupation = -1;
		}
		vec_info.push_back(info);
	}
	packet_wrapper h1;
	CMD::Make<CMD::hide_and_seek_players_info>::From(h1,&vec_info[0],vec_info.size());
	if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1);
}

int seek_raid_world_manager::GetWinnerOnCastSkill()
{
	int winner = SEEKER_WIN;
	SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
	for ( ; it != _seek_player_info_map.end(); ++it)
	{
		seek_player_info & hsInfo = it->second;
		if (hsInfo.ishide)
		{
			char status;
			for (int i = 0; i < MAX_PLAYER; ++i)
			{
				player_status & ps = _player_status_arr[i];
				if (ps.id == it->first)
				{
					status =ps.status;
					break;
				}
			}

			if (hsInfo.attacked_cnt < 2 && status != STATUS_LEFT)
			{
				winner = NO_WIN;
				break;
			}
		}
	}

	if (winner == SEEKER_WIN) return winner;

	winner = HIDER_WIN;
	it = _seek_player_info_map.begin();	
	for ( ; it != _seek_player_info_map.end(); ++it)
	{
		seek_player_info & hsInfo = it->second;
		if (!hsInfo.ishide)
		{
			char status;
			for (int i = 0; i < MAX_PLAYER; ++i)
			{
				player_status & ps = _player_status_arr[i];
				if (ps.id == it->first)
				{
					status =ps.status;
					break;
				}
			}
			if (!hsInfo.seeker_exhaust_skill && status != STATUS_LEFT)
			{
				winner = NO_WIN;
				break;
			}
		}
	}
	return winner;
}

void seek_raid_world_manager::AddScoreOnRoundFinish(int win)
{
	SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
	if (win == HIDER_WIN)
	{
		for ( ; it != _seek_player_info_map.end(); ++it)
		{
			seek_player_info & hsInfo = it->second;
			if (hsInfo.ishide)
			{
				_players_score_map[it->first].num += WIN_SCORE;
				if (_basic2win_map.find(hsInfo.last_used_skill) != _basic2win_map.end())
				{
					_players_score_map[it->first].num += _basic2win_map[hsInfo.last_used_skill];
					GLog::log( GLOG_INFO, "seek_raid, role_id = %d, skill_id = %d, add_score = %d\n",it->first, hsInfo.last_used_skill, _basic2win_map[hsInfo.last_used_skill]);
				}
				else
				{
					_players_score_map[it->first].num += _advance2win_map[hsInfo.last_used_skill];
					GLog::log( GLOG_INFO, "seek_raid, role_id = %d, skill_id = %d, add_score = %d\n",it->first, hsInfo.last_used_skill, _advance2win_map[hsInfo.last_used_skill]);
				}
				_players_score_map[it->first].time = _seek_heartbeat_counter;
			}
		}
	}
	else
	{
		for ( ; it != _seek_player_info_map.end(); ++it)
		{
			seek_player_info & hsInfo = it->second;
			if (!hsInfo.ishide)
			{
				_players_score_map[it->first].num += WIN_SCORE;
			}
			else
			{
				_players_score_map[it->first].num += HIDER_LOSE_SCORE;
			}
			_players_score_map[it->first].time = _seek_heartbeat_counter;
		}
	}
}

void seek_raid_world_manager::CalcScoreOnFinishRound()
{
	int win = _round_flag[_seek_round-1]; //是否已经计算过结果
	if (win < HIDER_WIN)
	{
		win = SEEKER_WIN;
		SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
		for ( ; it != _seek_player_info_map.end(); ++it)
		{
			seek_player_info & hsInfo = it->second;
			if (hsInfo.ishide)
			{
				char status;
				for (int i = 0; i < MAX_PLAYER; ++i)
				{
					player_status & ps = _player_status_arr[i];
					if (ps.id == it->first)
					{
						status =ps.status;
						break;
					}
				}
				if (hsInfo.attacked_cnt < 2 && status == STATUS_ONLINE)
				{
					win = HIDER_WIN;
					break;
				}
			}
		}
		_round_flag[_seek_round-1] = win;
	}
	AddScoreOnRoundFinish(win);
}

void seek_raid_world_manager::SendClientHideAndSeekRole(int player_id, char role_type)
{
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<CMD::hide_and_seek_role>::From(h1,player_id,role_type);
	if (_all_list.size()) multi_send_ls_msg(_all_list,h1.data(),h1.size(),-1); 
}

void seek_raid_world_manager::BroadcastHideAndSeekRoleMsg()
{
	SeekPlayerInfoMap::iterator it = _seek_player_info_map.begin();
	for ( ; it != _seek_player_info_map.end(); ++it)
	{
		seek_player_info &hsInfo = it->second;
		char roletype = hsInfo.ishide ? 0: 1;
		SendClientHideAndSeekRole(it->first,roletype);
		if (!hsInfo.ishide)
		{
			MSG msg;
			BuildMessage2(msg,GM_MSG_SEEK_RAID_SKILL_LEFT,XID(GM_TYPE_PLAYER,it->first),XID(-1,-1),A3DVECTOR(0,0,0),SEEKER_SKILL_NUM,0);
			gmatrix::SendMessage(msg);
		}
	}
}

void seek_raid_world_manager::DebugSetHider(int player_id, int hider)
{
	if (hider)
	{
		if (_debug_hider_set.find(player_id) == _debug_hider_set.end())
		{
			_debug_hider_set.insert(player_id);
		}

		if (_debug_seeker_set.find(player_id) != _debug_seeker_set.end())
		{   
			_debug_seeker_set.erase(player_id);
		}   
	}
	else
	{
		if (_debug_seeker_set.find(player_id) == _debug_seeker_set.end())
		{
			_debug_seeker_set.insert(player_id);
		}

		if (_debug_hider_set.find(player_id) != _debug_hider_set.end())
		{
			_debug_hider_set.erase(player_id);
		}
	}
}

void seek_raid_world_manager::OnPlayerReenterSeekRaid(gplayer* pPlayer, bool reenter, int faction)
{
	spin_autolock keeper(_raid_lock);
	seek_raid_player_imp* pImp = (seek_raid_player_imp*)pPlayer->imp;

	pImp->SendClientRaidRoundInfo(_seek_round,_round_end_time[_seek_round]);
	SeekPlayerInfoMap::iterator it = _seek_player_info_map.find(pPlayer->ID.id);
	if (it != _seek_player_info_map.end() && _seek_round && _seek_round%3)
	{
		seek_player_info &hsInfo = it->second;
		char roletype = OCCUPATION_HIDE;
		bool setdiestate = false;
		if (!hsInfo.ishide)
		{
			roletype = OCCUPATION_SEEK;
			if (hsInfo.seeker_exhaust_skill) setdiestate = true;
		}
		else
		{
			if (hsInfo.attacked_cnt == 2)
			{
				roletype = OCCUPATION_HIDE_DIE;	
				setdiestate = true;
			}
		}
		if (setdiestate) pPlayer->SetExtraState2(gplayer::STATE_SEEKRAID_DIE);
		if (reenter) __PRINTF("roleid=%d,roletype=%d\n",pPlayer->ID.id, roletype);
		SendClientHideAndSeekRole(pPlayer->ID.id,roletype);
		if (hsInfo.ishide)
		{
			pImp->SendClientHiderBloodNum(0,2-hsInfo.attacked_cnt);
		}
		else
		{
			player_sys_skill & s = hsInfo.skills[SEEKER_SKILL];
			char skill_left = 0,skill_buy_num = 0;
			skill_left = s.can_use_cnt - 20 > 0 ? s.can_use_cnt - 20 : 0;
			skill_buy_num = s.buy_cnt;
			pImp->SendClientSeekerSkillInfo(skill_left,skill_buy_num);
			if (_seek_round % 3) pImp->OI_StartTransform(923,1,1,600,player_transform::TRANSFORM_TASK);
		}
		if (_seek_round%3 == ROUND_ONE_PREPARE)
		{
			pImp->_commander->DenyCmd(controller::CMD_MOVE);
		}
		PlayerSkillMap::iterator it2 = hsInfo.skills.begin();
		for (; it2 != hsInfo.skills.end(); ++it2)
		{
			player_sys_skill& s = it2->second;
			pImp->InsertSysSkill(s.skill_id,s.skill_level);
		}
	}

	for (int i = 0; i < MAX_PLAYER; i++)
	{
		player_status & ps = _player_status_arr[i];
		if (ps.id == pPlayer->ID.id)
		{
			ps.status = STATUS_ONLINE;
			pPlayer->pos = ps.leavePos;
			break;
		}
	}
	UpdatePlayerStatus();
	SendClientPlayerScoreInfo();
}

void seek_raid_world_manager::SlowPlayerEnterSeekRaid(gplayer* pPlayer)
{
	spin_autolock keeper(_raid_lock);
	if (_seek_round < ROUND_ONE_PREPARE || _seek_round > ROUND_ONE_STOP) return;
	seek_raid_player_imp* pImp = (seek_raid_player_imp*)pPlayer->imp;
	
	int player_id = pPlayer->ID.id;
	if (_seek_player_info_map.find(player_id) != _seek_player_info_map.end())
	{
		return;
	}
	if (_slow_player_map.find(player_id) == _slow_player_map.end())
	{
		return;
	}
	int index = _slow_player_map[player_id];
	__PRINTF("SLOWPLAYEREN playerid=%d,index=%d\n",player_id,index);
	bool ishide = index % 5 ? true:false;
	SendSeekSkillInfo(player_id,ishide);
	pImp->SendClientRaidRoundInfo(_seek_round,_round_end_time[_seek_round]);
	char roletype = ishide ? 0: 1;
	SendClientHideAndSeekRole(player_id,roletype);
	if (!ishide)
	{
		MSG msg;
		BuildMessage2(msg,GM_MSG_SEEK_RAID_SKILL_LEFT,XID(GM_TYPE_PLAYER,player_id),XID(-1,-1),A3DVECTOR(0,0,0),SEEKER_SKILL_NUM,0);
		gmatrix::SendMessage(msg);
	}
	UpdatePlayerStatus();
	SendClientPlayerScoreInfo();
}

void seek_raid_world_manager::DebugSetSeekRaidScore(int player_id, int score)
{
	if (_players_score_map.find(player_id) != _players_score_map.end())
	{
		_players_score_map[player_id].num += score;
		_players_score_map[player_id].time = _seek_heartbeat_counter;
		UpdatePlayerStatus();
		SendClientPlayerScoreInfo();
	}
}

void seek_raid_world_manager::OnClosing()
{
}
