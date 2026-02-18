/*
 * FILE: kf_player.h 
 *
 * DESCRIPTION: killing field player implementation 
 *
 * CREATED BY: lianshuming, 2009/04/13
 *
 * HISTORY:
 *
 * Copyright (c) 2009 Archosaur Studio, All Rights Reserved.
 */

#ifndef __ONLINEGAME_GS_KF_PLAYER_H__
#define __ONLINEGAME_GS_KF_PLAYER_H__

#include "bg_player.h"

class kf_player_imp : public bg_player_imp
{
private:
	char _rank;			//军衔
	int _con_kill;			//连续击杀次数	
	int _postion;			//排名
	int _revive_counter;		//强制复活计时器
	int _revive_timeout; 		//复活时间倒计时, 给客户端用的
	int _revive_punish_counter;   	//复活惩罚时限
	int _revive_clean_punish;	//清除复活惩罚
	int _update_info_counter;	//延时发送信息计算器
		
	enum
	{
		REVIVE_MIN_TIMEOUT = 10,  	 //复活倒计时的最小值
		REVIVE_MAX_TIMEOUT = 50,  	 //复活倒计时的最大值
		REVIVE_FORCE_TIME  = 90,  	 //强制复活时间
		REVIVE_PUNISH_COUNTER = 60,	 //复活后惩罚的计时器
		REVIVE_CLEAN_PUNISH = 180,	 //清除复活惩罚的计时器
	};
public:
		
	DECLARE_SUBSTANCE( kf_player_imp )
	kf_player_imp()
	{
		_rank = 1; 	
		_con_kill = 0;	
		_postion = 0;
		_revive_counter = 0;
	       	_revive_timeout = REVIVE_MIN_TIMEOUT;
		_revive_punish_counter = 0;	
		_revive_clean_punish = 0;
		_update_info_counter = 0;
	}
	
	virtual int MessageHandler( const MSG& msg );
	
	virtual void OnHeartbeat( size_t tick );
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	
	virtual void GetBattleInfo();
	virtual void SendNormalChat(char channel, const void * buf, size_t len, const void * aux_data, size_t dsize);
	virtual void ChangeKillingfieldScore (int value);
	virtual void ResetKillingfieldScore();

protected:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void HandleBattleResult();
	virtual void HandleDeath();
	
	virtual void DumpBattleSelfInfo();
	virtual void DumpBattleInfo();
	virtual void DebugIncBattleScore(int inc);

private:
	inline char GetRank() const { return _rank;}
	inline size_t GetScore() const { return _killingfield_score; }
	size_t GetBattleScore() { return _killingfield_score; }
	inline int GetConKill() const { return _con_kill; }
	inline int GetDeathCount() const { return _death_count;}

	inline void UpdateScore(int offset, int sync_msg = GM_MSG_SYNC_BATTLE_INFO , int sync_param =0)
	{
		_killingfield_score += offset;  
		UpdateRank();
		
		//同步信息到客户端和战场
		SendClientSelfInfo();
		SyncBattleInfo(sync_msg, sync_param);
	}
	
	inline void UpdateRank()
	{
		int map_id = _plane->GetWorldIndex();
		int rank =  player_template::GetRankByScore(map_id, _killingfield_score); 
		if(rank != -1 && rank != _rank)
		{	
			gplayer * pPlayer = GetParent();
			_rank = rank;
			pPlayer->rank = _rank;
			_runner->rank_change(_rank);
		}
	}

	inline int GetKillScoreByRank(int rank)
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetKillScoreByRank(map_id, rank);
	}
	
	inline int GetExtScoreByConKill(int _con_kill)
	{
		if(_con_kill < 3) return 0;
		int map_id = _plane->GetWorldIndex();
		return player_template::GetConKillScore(map_id, _con_kill);
	}
	
	inline int GetWinExtScore()
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetBattleWinScore(map_id);
	}

	inline int GetScoreByKillMonster(int id)
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetKillMonsterScore(map_id, id);
	}
	
	inline int GetDeathDropItemID()
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetDeathDropItemID(map_id, _rank);
	}

	inline int GetTaskIDByRank()
	{
		int map_id = _plane->GetWorldIndex();
		return player_template::GetTaskIDByRank(map_id, _rank);
	}
	
	void HandleDeathDrop();
	void HandleDeathRobDrop();
	
	//获取当前的排名
	int GetPostion();

	//同步自己的战场信息给manager
	void SyncBattleInfo(int msg_id, int param);
	
	void SendClientSelfInfo();
	void GetKillingFieldInfo();

};

#endif
