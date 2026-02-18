/*
 * FILE: cs_flow_player.h 
 *
 * DESCRIPTION: cross server flow battleground player implementation 
 *
 * CREATED BY: Houjun, 2013/03/27
 *
 * HISTORY:
 *
 * Copyright (c) 2013 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_CS_FLOW_PLAYER_IMP_H__
#define __ONLINEGAME_GS_CS_FLOW_PLAYER_IMP_H__

#include "bg_player.h"


class cs_flow_player_imp : public bg_player_imp 
{
private:
	int _revive_counter;	//强制复活计时器	
	int _heartbeat_counter;	//心跳计时器 
	int _cs_info_seq;	

	int _score;	
	int _c_kill_cnt;			//连杀值
	int _max_c_kill_cnt;
	int _m_kill_cnt;			//多杀值
	int _m_kill_cnt_timeatamp;	//多杀时间戳
	int _max_m_kill_cnt;
	int _monster_kill_cnt;		//杀死怪物数量
	bool _reenter;
	bool _disconnected;

	enum
	{
		REVIVE_FORCE_COUNTER = 30,	 //强制复活计数器
		INVICIBLE_TIME = 60,
	};

public:
	DECLARE_SUBSTANCE( cs_flow_player_imp )
	cs_flow_player_imp()
	{
		_revive_counter = 0;
		_heartbeat_counter = 0;
		_cs_info_seq = -1;

		_score = 0;
		_c_kill_cnt = 0;
		_m_kill_cnt = 0;
	 	_m_kill_cnt_timeatamp = 0;	
		_monster_kill_cnt = 0;
		_disconnected = false;
		_reenter = false;
		_max_c_kill_cnt = 0;
		_max_m_kill_cnt = 0;
	}
	virtual int MessageHandler( const MSG& msg );
	virtual void OnHeartbeat( size_t tick );

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void GetBattleInfo();
	virtual bool UseItem(item_list & inv, int inv_index, int where, int item_type);
	virtual bool UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, size_t arg_size);

	virtual void PostLogin(const userlogin_t & user, char trole, char loginflag);

	virtual void PlayerFlowBattleLeave();

	virtual void SetLogoutDisconnect();
	virtual bool IsLogoutDisconnected() { return _disconnected; }
	virtual void OnTimeOutKickout();
	//用于在客户端创建角色后发送角色战场相关信息
	virtual void PostPlayerEnterWorld();
	virtual void DispatchFlowScore(int score);

protected:
	virtual void PlayerEnter();
	virtual void HandleDeath();	
	virtual void HandleBattleResult();
	void SendPlayerCSFlowInfo(bool mKillChange = false);
	void SyncBattleInfo();
	void IncScore(int sc);
	void SyncFlowPlayerInfo();

};

#endif
