/*
 * FILE: cb_player.h 
 *
 * DESCRIPTION: common battleground player implementation 
 *
 * CREATED BY: lianshuming, 2009/07/02
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_CB_PLAYER_IMP_H__
#define __ONLINEGAME_GS_CB_PLAYER_IMP_H__

#include "bg_player.h"

class cb_player_imp : public bg_player_imp 
{

private:
	bool _kill_much;		//杀15人的标志, 在杀死第15人时被置位，之后被杀被复位
	int _revive_timeout; 		//复活时间倒计时, 给客户端用的
	int _revive_punish_counter;   	//复活惩罚时限

	enum
	{
		DT_CUPRUM_MEDAL = 10536,
		DT_SILVER_MEDAL = 10537,

		DT_ATTACKER_DROP1 = 24238,
		DT_ATTACKER_DROP2 = 24239,
		DT_DEFENDER_DROP1 = 24240,
		DT_DEFENDER_DROP2 = 24701,

		DT_REBORN_AWARD_ID = 23858,
		DT_REBORN_WIN_AWARD_CNT = 5,
		DT_REBORN_DRAW_AWARD_CNT = 2,
		DT_REBORN_FIRST_AWARD_CNT = 1,

		//非飞升战场的奖励
		BATTLE_REWARD_WIN =			80,	// 胜利方得分
		BATTLE_REWARD_LOST =			10,	// 失败方得分
		BATTLE_REWARD_DRAW =			30,	// 平手得分
		BATTLE_REWARD_MAX_KILL =		50,	// 杀人数第一，且仅有一个人

		//飞升战场的奖励	
		BATTLE_REBORN_REWARD_WIN =		120,	// 胜利方得分
		BATTLE_REBORN_REWARD_LOST =		15,	// 失败方得分
		BATTLE_REBORN_REWARD_DRAW =		45,	// 平手得分
		BATTLE_REBORN_REWARD_MAX_KILL =		75,	// 杀人数第一，且仅有一个人

		KILL_MUCH_NUMBER = 15,

		REVIVE_PUNISH_COUNTER = 90,	 //复活后惩罚的计时器
		REVIVE_MIN_TIMEOUT = 30,  	 //复活倒计时的最小值
		REVIVE_MAX_TIMEOUT = 90,  	 //复活倒计时的最大值
	};

public:
	DECLARE_SUBSTANCE( cb_player_imp )
	cb_player_imp()
	{
		_kill_much = false;
	       	_revive_timeout = REVIVE_MIN_TIMEOUT;
		_revive_punish_counter = 0;	
	}

	virtual int MessageHandler( const MSG& msg );
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void GetBattleInfo();

protected:
	virtual void HandleDeath();
	virtual void HandleBattleResult();
	
private:
	static int GetKillLevel( int kill_count )
	{
		const static int kill_level_list[] = {3,7,15};
		int i = 0;
		for( i = 0; i < 3 ; ++i )
		{
			if( kill_count <= kill_level_list[i] ) return i;
		}
		return 3;
	}

	static int GetDeathCuprumMedalDropCount( int kill_count )
	{
		static float drop_prop[][5] =
		{
			{0.5f,0.51f},			// 50%不掉 50%掉1个
			{0.4f,0.3f,0.31f},		// 40%不掉 30%掉1个 30%掉2个
			{0.25f,0.25f,0.25f,0.251f},	// 25%不掉 25%掉1个 25%掉2个 25%掉3个
			{0,0.25f,0.25f,0.25f,0.251f},	// 0%不掉 25%掉1个 25%掉2个 25%掉3个 25%掉4个
		};
		int kill_level = GetKillLevel( kill_count );
		ASSERT( kill_level >= 0 && kill_level <= 3 );
		return abase::RandSelect( drop_prop[kill_level], 5 );
	};

	void GetCommonBattleInfo();

};

#endif

