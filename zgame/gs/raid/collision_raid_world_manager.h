#ifndef __COLLISION_RAID_WORLD_MANAGER_
#define __COLLISION_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"


class collision_raid_world_manager : public raid_world_manager
{
public:
	collision_raid_world_manager();
	virtual ~collision_raid_world_manager();
	
	virtual int GetRaidType() { return RT_COLLISION; }

	virtual void OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction);
	virtual void OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction);
	virtual void OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void Heartbeat();
	virtual void Reset();
	virtual void OnRunning();
	virtual void OnClosing();
	
	//return 0, 可以创建；-1，world_manager当前被占用, 选择下一个可用的WM；-2，副本data检查错误；失败
	virtual int TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);

	virtual void AddAttackerScore(int score);
	virtual void AddDefenderScore(int score);
	
//	virtual void SetRoleSpeed(int roleid, short speed);
	virtual bool GetCollisionPlayerInfo(int roleid, int & battle_faction);
	virtual void UpdateScore();
	virtual void GetAllPlayerID(std::vector<int> & id_list);

	virtual void RecordEntryPos(const A3DVECTOR& pos,int faction);
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);

	virtual int GetAttackerAwardScore() const { return _attacker_award_score;}
	virtual int GetDefenderAwardScore() const { return _defender_award_score;}

	virtual void OnCheckCheat();	//防止作弊检查
	virtual void SendCheatInfo(int roleid,  int faction){}

protected:
	void GetEntryPos(A3DVECTOR& pos,int raid_faction, int roleid);
	void ClrEntryPos(int roleid, int raid_faction);

	void IncAttackerScore(int score);
	void IncDefenderScore(int score);

	void CalcAwardScore();
	/*
	void CheckCollision(); 
	bool  IsCollision(int roleid1, short roleid1_speed, int roleid2, short roleid2_speed);
	bool  IsCollision2(int roleid1, short roleid1_speed, int roleid2, short roleid2_speed);
	*/

protected:
	struct entry_pos
	{
		int roleid;
		bool is_used;
		A3DVECTOR pos;
	};
	std::vector<entry_pos> _attacker_pos_list;
	std::vector<entry_pos> _defender_pos_list;

	struct player_apply_info
	{
		int roleid;
		int faction; //1-攻方 2-守方
		int score;
	};

	struct collision_player_info 
	{
		int roleid;
		int faction;
		int kill_count;
		int death_count;
		int score;
		int is_online;
	};

	int _max_killer_count;
	abase::vector<collision_player_info> _player_list;

	int _attacker_award_score;	//结束后攻方获得积分(有可能为负值)
	int _defender_award_score;	//结束后守方获得积分(有可能为负值)

	int _attacker_count_timer;	//攻方人数计数器 (如果人数为0则计算器加1, 当大于30的时候为输)
	int _defender_count_timer;	//守方人数计数器 (如果人数为0则计算器加1, 当大于30的时候为输)

};


class collision2_raid_world_manager : public collision_raid_world_manager 
{
public:
	collision2_raid_world_manager();
	virtual ~collision2_raid_world_manager();
	
	virtual int GetRaidType() { return RT_COLLISION2; }
	void CalcAwardScore(){}
	virtual bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);
	virtual void OnCheckCheat();	//防止作弊检查
	virtual void SendCheatInfo(int roleid,  int faction);
	virtual void Reset();

	void NotifyCheatInfo(int faction, bool is_cheat, int cheat_counter); 

private:
	int _attacker_cheat_counter;
	bool _attacker_cheat_on;
	int _defender_cheat_counter;
	bool _defender_cheat_on;

};

#endif
