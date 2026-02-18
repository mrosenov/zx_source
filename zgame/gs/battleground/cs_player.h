/*
 * FILE: cs_player.h 
 *
 * DESCRIPTION: cross server battleground player implementation 
 *
 * CREATED BY: lianshuming, 2009/07/01
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_CS_PLAYER_IMP_H__
#define __ONLINEGAME_GS_CS_PLAYER_IMP_H__

#include "bg_player.h"


class cs_player_imp : public bg_player_imp 
{
private:
	int _revive_counter;	//强制复活计时器	
	int _heartbeat_counter;	//心跳计时器 
	int _cs_info_seq;	

	enum
	{
		REVIVE_FORCE_COUNTER = 30,	 //强制复活计数器
		INVICIBLE_TIME = 60,
	};

public:
	DECLARE_SUBSTANCE( cs_player_imp )
	cs_player_imp()
	{
		_revive_counter = 0;
		_heartbeat_counter = 0;
		_cs_info_seq = -1;
	}
	virtual int MessageHandler( const MSG& msg );
	virtual void OnHeartbeat( size_t tick );
	virtual const A3DVECTOR& GetLogoutPos( int &world_tag );

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void GetBattleInfo();
	virtual void HandleBattleResult();
	virtual bool UseItem(item_list & inv, int inv_index, int where, int item_type);
	virtual bool UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size);

protected:
	virtual void PlayerEnter();
	virtual void HandleDeath();

	//调试函数
	virtual void DumpBattleSelfInfo();
	virtual void DumpBattleInfo();
	virtual void DebugIncBattleScore(int inc){}
	
	void GetCrossServerBattleInfo();
	void SyncBattleInfo();

};

//匿名跨服PK player的实现
class cs_melee_player_imp : public cs_player_imp
{
public:
	DECLARE_SUBSTANCE( cs_melee_player_imp )
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int MessageHandler( const MSG& msg );
};
			  


#endif
