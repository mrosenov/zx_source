#ifndef __ONLINEGAME_GS_CS_TEAM_PLAYER_IMP_H__
#define __ONLINEGAME_GS_CS_TEAM_PLAYER_IMP_H__

#include "bg_player.h"

class cs_team_player_imp : public bg_player_imp
{
	enum
	{
		REVIVE_FORCE_COUNTER = 10,	 //强制复活计数器
		INVICIBLE_TIME = 15,
	};

public:
	DECLARE_SUBSTANCE( cs_team_player_imp )

public:
	virtual int  MessageHandler( const MSG& msg );
    virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void HandleDeath();
	virtual void OnHeartbeat( size_t tick );
	virtual void SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data, size_t dsize);

	virtual void GetBattleInfo();
	virtual void HandleBattleResult();
	virtual bool IsInBattleCombatZone();
	virtual bool UseItem(item_list & inv, int inv_index, int where, int item_type);
	virtual bool UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size);

    void GetCrossServerTeamBattleInfo();

	//调试函数
	virtual void DumpBattleInfo();
	virtual void DebugIncBattleScore(int inc){}

	cs_team_player_imp();
	~cs_team_player_imp();

private:
	bool RoundStart();
	bool RoundEnd();
	void DisableAttackInBattle();
	void EnableAttackInBattle();
	void SetDisAttackCount(int count) { _disable_attack_count = count; }

	bool _is_in_combat_zone;//标识player是否在战斗区域，不在则在休息区
	int _revive_counter;	//强制复活计时器	
	int _cs_team_info_seq;	
	int _disable_attack_count;

	std::vector<S2C::CMD::crossserver_team_battle_info::history_score> _history_info; 
};


#endif
