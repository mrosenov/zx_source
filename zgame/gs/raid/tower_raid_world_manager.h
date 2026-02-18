#ifndef __TOWER_RAID_WORLD_MANAGER_
#define __TOWER_RAID_WORLD_MANAGER_

#include "raid_world_manager.h"
#include "../config.h"

#define MAX_PLAYER_DEATH_CNT 3

struct tower_raid_data
{
	int init_tower_level;
};

class tower_raid_world_manager : public raid_world_manager
{       
protected:
	int _tower_opened_level;//有效的最大层数
	int _init_tower_level;	//玩家选择的初始关
	int _curr_tower_level;	//当前层数
	std::vector<int> _addition_skill_list; 		//手牌技能
	int _level_tids[MAX_TOWER_RAID_LEVEL];	//每一层配置表模板ID

	int _level_start_time;

	struct player_sys_skill
	{
		int skill_id;
		int skill_level;
		bool used;
	};

	typedef std::map<int, player_sys_skill> PlayerSkillMap;
	struct tower_player_info
	{
		int used_skill_cnt;
		PlayerSkillMap skills;
		A3DVECTOR leavePos;
		int deathCnt;
		bool reenter;
	};

	typedef std::map<int,tower_player_info> TowerPlayerInfoMap;
	TowerPlayerInfoMap _tower_player_info_map;

	enum
	{
		MONSTER_STATE_EMPTY = 0,	
		MONSTER_STATE_RAND,
		MONSTER_STATE_GENED,	
		MONSTER_STATE_KILLED,
	};

	int _monster_result_send_cnt;	//给客户端发送结果的计数
	int _monster_cnt;		//每一层生成的怪物数量
	struct level_monster
	{
		int monster_tid;
		char monster_state;
		char client_idx;
	}_monsters[4];
	
	char _rand_send_client[4];	//表示发送给客户端的那个一个是空，0表示空

	int _level_victory_controller;
	int _actived_controller;
	A3DVECTOR _curr_reborn_pos;

	int _reset_monster_wait_time;
	
public:
	tower_raid_world_manager();
	virtual ~tower_raid_world_manager();

	virtual int GetRaidType() { return RT_TOWER; }
	virtual int TestCreateRaid( const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual int CreateRaid( int raidroom_id, const raid_world_template& rwt, int tag, int index, const std::vector<int>& roleid_list, const void* buf, size_t size, char difficulty);
	virtual void Reset();
	virtual void OnPlayerEnter(gplayer* pPlayer, bool reenter, int faction);
	virtual void OnPlayerLeave( gplayer* pPlayer, bool cond_kick, int faction);
	virtual void OnPlayerDeath(gplayer* pPlayer, int killer, int deadman, int deadman_battle_faction, bool& kill_much );
	virtual void OnRunning();
	virtual void OnKillMonster(gplayer* pPlayer, int monster_tid, int faction);
	
	virtual int GetInitTowerLevel() const { return _init_tower_level; }
	virtual bool CanCastSkillInWorld(int skill_id, int who);
	virtual void OnCastSkill(gactive_imp* pImp, int skill_id);
	void GenLevelMonsters(gplayer* gplayer, int level, char client_idx);
	int GetCurrTowerLevel() const { return _curr_tower_level; }
	virtual void GetAllDataWorldManager(gplayer_imp * pImp);
	bool GetRebornPos(gplayer_imp* pImp,A3DVECTOR& pos,int& tag);
	bool GetTownPosition(gplayer_imp* pImp,const A3DVECTOR& opos,A3DVECTOR &pos,int& world_tag);

private:
	void ResetLevelMonsters(gplayer* pPlayer, int level);
	bool GenMonster(int monster_cnt, int id, const TOWER_TRANSCRIPTION_PROPERTY_CONFIG& config);
	void SaveTowerGiveSkillClient(gplayer* pPlayer, archive& ar);
};

#endif /*__TOWER_RAID_WORLD_MANAGER_*/
