#ifndef __CAPTURE_RAID_WORLD_MANAGER_
#define __CAPTURE_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"


class capture_raid_world_manager : public raid_world_manager
{
protected:
	enum
	{
		ATTACKER_MATTER_ID = 63234, //隐仙阁矿
		DEFENDER_MATTER_ID = 63231, //逸龙轩矿
		ATTACKER_MATTER_ITEM_ID = 63233, //隐仙阁军旗
		DEFENDER_MATTER_ITEM_ID = 63232, //逸龙轩军旗
		CAPTURE_FLAG_TRANSFORM_SKILL0 = 5350,
		CAPTURE_FLAG_TRANSFORM_SKILL1 = 5351,

		CAPTURE_KILL_MONSTER_SCORE = 10,
		CAPTURE_SUBMIT_FLAG_SCORE = 20,
		CAPTURE_BROAD_CAST_SCORE = 40,
		CAPTURE_WIN_SCORE = 60,

		CAPTURE_FLAG_BORN_TIME = 60,
		CAPTURE_FLAG_REBORN_TIME = 10,

		CAPTURE_FLAG_MAX = 3,
	};

	struct player_apply_info
	{
		int roleid;
		int faction; //1-攻方 2-守方
	};

	struct capture_player_info
	{
		int roleid;
		short kill_count;
		short death_count;
		A3DVECTOR leavePos;
		char faction;
		char flag_count;
		short score;
	};

	int _max_attacker_killer_id; //杀神
	int _max_defender_killer_id; //杀神

	short _max_attacker_kill_count;
	short _max_defender_kill_count;

	char _attacker_flag_count; //攻方夺旗数
	char _defender_flag_count; //守方夺旗数

	int _attacker_flag_capture_id; //攻方夺旗id
	int _defender_flag_capture_id; //守方夺旗id

	int _attacker_score_time;
	int _defender_score_time;

	char _reset_flag_timer;

	char _forbid_move_timer;

	char _tip2_timer;

	typedef std::map<int,capture_player_info> CapturePlayerInfoMap;
	CapturePlayerInfoMap _capture_player_info_map;
	abase::vector<player_apply_info> _player_list;
	std::set<int> _forbid_set;

public:
	capture_raid_world_manager();
	virtual ~capture_raid_world_manager();

	virtual int GetRaidType() { return RT_CAPTURE; }
	virtual void OnPlayerEnterWorld(gplayer* pPlayer, bool reenter, int faction);
	virtual void OnPlayerLeave(gplayer* pPlayer, bool cond_kick, int faction);

	virtual bool CanCastSkillInWorld(int skill_id, int who);
	virtual void Reset();
	virtual void OnRunning();
	virtual void OnClosing();

	//return 0, 可以创建；-1，world_manager当前被占用, 选择下一个可用的WM；-2，副本data检查错误；失败
	virtual int TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);

	virtual void OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void AddAttackerScore(int score);
	virtual void AddDefenderScore(int score);

	virtual bool GetCapturePlayerInfo(int roleid, int & battle_faction);
        virtual int OnMobDeath( world * pPlane, int faction, int tid,const A3DVECTOR& pos, int attacker_id);
	virtual void OnClose();
	void SetAttackerFlagID(gplayer_imp * pImp);
	void SetDefenderFlagID(gplayer_imp * pImp);
	void SubmitCaptureRaidFlag(gplayer_imp * pImp, int faction);
	virtual void HandleCaptureRaidSyncPos(char faction, const A3DVECTOR& pos);

	int GetRaidMaxAttackerKillerID()
	{
		return _max_attacker_killer_id;
	}

	int GetRaidMaxDefenderKillerID()
	{
		return _max_defender_killer_id;
	}

	virtual void UpdatePlayerScore();

protected:
	void IncAttackerScore(int score);
	void IncDefenderScore(int score);
	virtual void UpdateScore();
	void BroadCastScoreUp(char faction, char score);
	void BroadCastSubmitFlag(char faction, gplayer_imp * pImp);
	void BroadCastResetFlag(char faction);
	void BroadCastGatherFlag(char faction, gplayer_imp * pImp);
	int RemoveCaptureRaidFlag(gplayer_imp * pImp, int player_id, int faction);
	void GenMatter(int faction);
	void KillAllMatter();
	void GetSameFactionId(int player_id ,vector<int>& vec_factionid);
	virtual void OnCreate();

};

#endif
