/*
 * FILE: in_player.h 
 *
 * DESCRIPTION: instance battle player implementation 
 *
 * CREATED BY: lianshuming, 2009/07/09
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_IN_PLAYER_H__
#define __ONLINEGAME_GS_IN_PLAYER_H__

#include "bg_player.h"

class in_player_imp : public bg_player_imp
{
public:
	DECLARE_SUBSTANCE( in_player_imp )
	virtual int MessageHandler( const MSG& msg );
	
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void HandleBattleResult();
	
	virtual void GetBattleInfo();


protected:
	//µ÷ÊÔº¯Êý
	virtual void DumpBattleSelfInfo();
	virtual void DumpBattleInfo();
	virtual void DebugIncBattleScore(int inc);
	
private:
	void GetInstanceBattleInfo();

	inline int GetScoreByKillMonster(int id)
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetInstanceKillMonsterScore(map_id, id);
	}

	inline int GetAwardTaskID(bool b_win, int total_score)
	{
		int map_id = _plane->GetWorldIndex();
		if(total_score <= 0) return 0;
		return player_template::GetInstanceAwardTaskID(map_id, total_score, b_win);

	}
};

#endif
