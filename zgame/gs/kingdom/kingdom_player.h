#ifndef __ONLINEGAME_GS_KINGDOM_PLAYER_H
#define __ONLINEGAME_GS_KINGDOM_PLAYER_H

#include "../player_imp.h"

//国王战里面地图1玩家的实现
class kingdom_player_imp : public gplayer_imp
{
public:
	DECLARE_SUBSTANCE( kingdom_player_imp )

	kingdom_player_imp()
	{
		_attacker_faction = 0;
		_defender_faction = 0;
		_timeout = 0;
		_kick_timeout = 0;
		_last_revive_timeout = 0;
		_cur_phase = 0;
		_kingdom_mafia_id = 0;
		_is_leaving = 0;
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
	virtual void PlayerLeaveKingdom();
	
	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	virtual void OnMineStart(int mine_id);
	virtual void OnMineEnd(int item_id);
	
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();
	virtual bool IsResurrectPosActive();
        virtual void OnUpdateMafiaInfo();

private:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);

	void LeaveKingdom();
	void HandleBattleHalfResult(int battle_result, int cur_time);
	void HandleBattleEnd(int battle_result);
	void UpdateKeyNPCInfo();
private:
	int _attacker_faction;		//攻防阵营
	int _defender_faction;		//守方阵营
	int _timeout;			//踢出战场的timeout
	int _kick_timeout;
	int _last_revive_timeout;
	int _cur_phase;			//当前处的阶段
	int _kingdom_mafia_id;		//以玩家进入战场时候的帮派id为准，防止玩家突然退帮
	bool _is_leaving;		//是否正在退出
};


#endif
