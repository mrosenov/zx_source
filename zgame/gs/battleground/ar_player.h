/*
 * FILE: ar_player.h 
 *
 * DESCRIPTION: arena player implementation 
 *
 * CREATED BY: lianshuming, 2009/04/17
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_AR_PLAYER_H__
#define __ONLINEGAME_GS_AR_PLAYER_H__

#include "bg_player.h"

class ar_player_imp : public bg_player_imp
{

private:
	
	enum
	{
		INVICIBLE_TIME = 180,
		NORMAL_REWARD_TASK_ID = 20058,
		REBORN_REWARD_TASK_ID = 16980,
		DEATH_DROP_ID = 30001,
	};

public:
	DECLARE_SUBSTANCE( ar_player_imp )

	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);

	virtual int MessageHandler( const MSG& msg );
	
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	
	virtual void GetBattleInfo();


protected:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void HandleBattleResult();
	//µ÷ÊÔº¯Êý
	virtual void DumpBattleSelfInfo();
	virtual void DumpBattleInfo(){}
	virtual void DebugIncBattleScore(int inc){}
	
private:
	void HandleDeathRobDrop();
	void HandleDeathDrop();
	void SyncBattleInfo();
	void GetArenaInfo();

	int GetDeathDropItemID(){return DEATH_DROP_ID;}
	int GetRewardTaskID(){return GetRebornCount() == 0 ? NORMAL_REWARD_TASK_ID : REBORN_REWARD_TASK_ID;}
};


#endif

