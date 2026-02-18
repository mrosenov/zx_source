#ifndef __SEEK_RAID_WORLD_MANAGER_
#define __SEEK_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"

struct player_sys_skill
{
	int skill_id;
	int skill_level;
	int used_cnt;
	char can_use_cnt;
	char buy_cnt;
};

typedef std::map<int, player_sys_skill> PlayerSkillMap;

struct seek_player_info
{
	PlayerSkillMap skills;
	int random_skill;
	int last_used_skill;
	char attacked_cnt;
	bool ishide;
	bool used_random_skill;
	bool seeker_exhaust_skill;
	
	seek_player_info()
	{
		random_skill = 0;
		last_used_skill = 0;
		attacked_cnt = 0;
		used_random_skill = false;
		seeker_exhaust_skill = false;
	}

	void Reset()
	{
		skills.clear();
		random_skill = 0;
		last_used_skill = 0;
		attacked_cnt = 0;
		used_random_skill = false;
		seeker_exhaust_skill = false;
	}
};

typedef std::map<int,seek_player_info> SeekPlayerInfoMap;
typedef std::map<int,int> IntMap;

struct seek_score
{
	int num;
	int time;
};

typedef std::map<int, seek_score> ScoreMap;

class seek_raid_world_manager : public raid_world_manager
{
public:
	enum
	{
		MAX_PLAYER = 20,

		ROUND_ONE_PREPARE = 1,
		ROUND_ONE_START = 2,
		ROUND_ONE_STOP = 3,
		ROUND_TWO_PREPARE = 4,
		ROUND_TWO_START = 5,
		ROUND_TWO_STOP = 6,
		ROUND_THIRD_PREPARE = 7,
		ROUND_THIRD_START = 8,
		ROUND_THIRD_STOP = 9,

		STATUS_LEFT = 0,
		STATUS_ONLINE = 1,
		STATUS_TMPLEFT = 2,

		SEEKER_SKILL = 5328,
		TAUNTED_SKILL = 5330,

		NO_WIN = 0,
		ROUND_IN = 1,	
		HIDER_WIN = 2,
		SEEKER_WIN = 3,

		//准备中,游戏中,出局,暂离,离开,胜利,失败
		ENUM_PREPARE = 0,
		ENUM_GAME = 1,
		ENUM_OUT = 2,
		ENUM_TMPLEFT = 3,
		ENUM_LEFT = 4,
		ENUM_WIN = 5,
		ENUM_FAIL = 6,

		SEEKER_SKILL_NUM = 20,

		PREPARE_COUNTER = 1200,

		HIDE_SKILL0 = 5331,
		HIDE_SKILL1 = 5332,
		HIDE_SKILL2 = 5333,
		HIDE_SKILL3 = 5334,
		HIDE_SKILL4 = 5335,
		HIDE_SKILL5 = 5336,
		HIDE_SKILL6 = 5337,
		HIDE_SKILL7 = 5338,
		HIDE_SKILL8 = 5339,
		HIDE_SKILL9 = 5340,

		PREPARE_TIME = 1200,
		GAME_TIME = 9600,
		REST_TIME = 600,

		WIN_SCORE = 20,
		HIDER_LOSE_SCORE = 10,

		OCCUPATION_HIDE = 0,
		OCCUPATION_SEEK = 1,
		OCCUPATION_HIDE_DIE = 2,
	};
	seek_raid_world_manager();
	virtual ~seek_raid_world_manager();

	virtual int GetRaidType() { return RT_SEEK; }
	virtual void OnPlayerEnterWorld(gplayer* pPlayer, bool reenter, int faction);
	virtual void OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction);
	virtual void Reset();
	virtual void OnClose();
	virtual bool CanCastSkillInWorld(int skill_id, int who);
	virtual void OnCastSkill(gactive_imp* pImp, int skill_id);
	virtual bool PlayerBuySeekerSkill(gplayer_imp* pImp);
	virtual void OnSeekRaidKill(int player_id, int attacker_id);
	virtual void OnSeekRaidHiderTaunted(int player_id, const A3DVECTOR pos, bool is_start);
	virtual void OnSeekRaidHiderTauntedGetScore(int player_id);
	virtual void DebugSetHider(int player_id, int hider);
	virtual void DebugSetSeekRaidScore(int player_id, int score);
	virtual void OnPlayerReenterSeekRaid(gplayer* pPlayer, bool reenter, int faction);
	virtual void SlowPlayerEnterSeekRaid(gplayer* pPlayer); 


	ScoreMap & GetScoreMap()
	{
		return _players_score_map;
	}

	IntMap & GetSkill2AchieveMap()
	{
		return _skill2achieve_map;
	}

	IntMap & GetBasic2CostMap()
	{
		return _basic2cost_map;
	}

	SeekPlayerInfoMap & GetSeekPlayerInfoMap()
	{
		return _seek_player_info_map;
	}

protected:
	void SendSeekSkillInfo(int id, int ishide);
	void UpdatePlayerStatus();
	void SendClientRaidRoundInfo(int step, int end_timestamp);
	void SeekRaidPrepare(bool isfirst);
	void SendClientPlayerScoreInfo();
	int GetWinnerOnCastSkill();
	void AddScoreOnRoundFinish(int win);
	void CalcScoreOnFinishRound();
	void SendClientHideAndSeekRole(int player_id, char role_type);
	void BroadcastHideAndSeekRoleMsg();
	virtual void OnRunning();
	virtual void OnClosing();

	

	struct player_status
	{
		int id;
		int disconnect_time;
		A3DVECTOR leavePos;
		char status;
	};

	player_status _player_status_arr[MAX_PLAYER];

	int _next_round_counter;
	int _seek_round;
	int _deny_hide_start_counter;
	int _deny_hide_end_counter;
	int _deny_seeker_start_counter;

	IntMap _basic2cost_map;
	IntMap _advance2cost_map;
	IntMap _basic2win_map;
	IntMap _advance2win_map;
	IntMap _skill2achieve_map;
	ScoreMap _players_score_map;

	SeekPlayerInfoMap _seek_player_info_map;

	char _round_flag[10];

	int _round_end_time[10];

	static int _random_cost_score;

	std::set<int> _debug_seeker_set;
	std::set<int> _debug_hider_set;

	IntMap _slow_player_map;
};

#endif
