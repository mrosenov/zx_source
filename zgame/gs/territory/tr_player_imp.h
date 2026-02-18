#ifndef __ONLINEGAME_GS_TR_PLAYER_IMP_H
#define __ONLINEGAME_GS_TR_PLAYER_IMP_H

#include "../player_imp.h"

class tr_player_imp : public gplayer_imp
{
private:
	int _attack_faction;		//攻防阵营
	int _defense_faction;		//守方阵营
	int _timeout;			//踢出战场的timeout
	int _battle_info_seq;		//战场info序列号
	int _old_world_tag;		//进入战场前的tag	
	int _kill_count;		//杀人数
	int _death_count;		//被杀数
	int _turret_counter;		
	int _kick_timeout;
	int _battle_id;			//领土id
	int _task_timeout;		//发放任务的timeout
	bool _battle_result_sent;	//战场结果发送标志
	bool _is_assist;		//是否是佣兵
	bool _is_leaving;		//是否正在退出
	A3DVECTOR _old_pos;		//进入战场前的位置

public:
	DECLARE_SUBSTANCE( tr_player_imp )

	tr_player_imp()
	{
		_attack_faction = 0;
		_defense_faction = 0;
		_timeout = -1;
		_battle_info_seq = -1;
		_old_world_tag = 0;
		_kill_count = 0;
		_death_count = 0;
		_turret_counter = 0;
		_kick_timeout = 0;
		_battle_id = 0;
		_task_timeout = 0;
		_battle_result_sent = false;
		_is_assist = false;
		_is_leaving = false;
		_old_pos = A3DVECTOR(0,0,0);
	}
	
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int MessageHandler( const MSG& msg );
	virtual int GetFaction();
	virtual int GetEnemyFaction();

	virtual void OnHeartbeat( size_t tick );

	virtual void PlayerEnterWorld();
	virtual void PlayerLeaveWorld();
	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();
	virtual void PostLogin(const userlogin_t& user, char trole, char loginflag);
	virtual void GetLogoutPos(int& world_tag,A3DVECTOR& pos);
	virtual void PlayerLeaveTerritory();

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void GetBattleInfo(){}
	
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();

	virtual void PlayerChangeTerritoryResult(int result);
	virtual int CheckUseTurretScroll();

private:
	virtual bool ValidPlayer();
	virtual void HandleBattleResult(){}
	virtual void HandleDeath(){}

	void LeaveTerritory();


	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);
};


#endif
