#ifndef __ONLINEGAME_GS_RAID_PLAYER_H
#define __ONLINEGAME_GS_RAID_PLAYER_H

#include "../player_imp.h"

class raid_player_imp: public gplayer_imp
{
protected:
	static bool __GetHookAttackJudge(gactive_imp * __this, const MSG & msg, attack_msg & amsg);
	static bool __GetHookEnchantJudge(gactive_imp * __this, const MSG & msg,enchant_msg & emsg);
	static void __GetHookAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetHookEnchantFill(gactive_imp * __this, enchant_msg & enchant);

protected:
	int _attack_faction;	//攻防阵营
	int _defense_faction;	//守方阵营
	int _timeout;			//踢出战场的timeout
	bool _is_leaving;		//是否已经在离开状态，等待Delivery通知换线
	int _raid_info_seq;		//战场info序列号
	int _kill_count;		//杀人数
	int _death_count;		//被杀数
	bool _raid_result_sent;	//战场结果发送标志
	bool _cond_kick;		//条件不足踢出标志
	bool _send_raid_leave;	
	bool _success_entered;

	int _leave_raid_time;
	int _kickout_time;

	int _old_world_tag;		//进入战场前的tag	
	A3DVECTOR _old_pos;		//进入战场前的位置

	bool _reenter;			//是否断线重入

public:
	DECLARE_SUBSTANCE( raid_player_imp )
	raid_player_imp()
	{
		_raid_faction = 1;
		_attack_faction = 0;
		_defense_faction = 0;
		_timeout = -1;
		_raid_info_seq = -1;
		_kill_count = 0;
		_death_count = 0;
		_raid_result_sent = false;
		_old_pos = A3DVECTOR(0,0,0);
		_old_world_tag = 0;
		_cond_kick = false;
		_is_leaving = false;
		_leave_raid_time = 0;
		_kickout_time = 0;
		_send_raid_leave = false;
		_success_entered = false;
		_reenter = false;
	}
	virtual void FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int MessageHandler( const MSG& msg );
	virtual void OnHeartbeat( size_t tick );
	virtual int GetFaction();
	virtual int GetEnemyFaction();
	virtual const A3DVECTOR& GetLogoutPos( int &world_tag );

	virtual void OnDeath( const XID & lastattack, bool is_pariah, bool faction_battle, bool is_hostile_duel, int time );
	
	virtual attack_judge GetHookAttackJudge();
	virtual enchant_judge GetHookEnchantJudge();
	virtual attack_fill GetHookAttackFill();
	virtual enchant_fill GetHookEnchantFill();

	virtual void PlayerEnterWorld();
	virtual void PlayerEnter();
	virtual void PlayerLeaveWorld();	
	virtual void PlayerLeave();
	virtual void LeaveRaid(int reason);
	virtual void PlayerLeaveRaid(int reason);

	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();
	virtual void PostLogin(const userlogin_t& user, char trole, char loginflag);

	virtual void GetRaidInfo();
	virtual void SendRaidUseGlobalVars();
	virtual bool ValidPlayer();

protected:	
	virtual void HandleRaidResult();
	virtual void HandleDeath(){}

	virtual void DumpRaidSelfInfo(){}
	virtual void DumpRaidInfo(){}
	virtual void DebugIncRaidScore(int inc){}
	
};

// Youshuang add
class cr_raid_player_imp : public raid_player_imp
{
public:
	DECLARE_SUBSTANCE( cr_raid_player_imp )
	virtual void PlayerEnter();
};
// end
#endif

