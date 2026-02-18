#ifndef __ONLINEGAME_GS_KINGDOM3_PLAYER_H
#define __ONLINEGAME_GS_KINGDOM3_PLAYER_H

#include "../player_imp.h"

//澡堂里面的玩家

class kingdom_bath_player_imp : public gplayer_imp
{
public:
	enum
	{
		MAX_BATH_INVITE_COUNT = 5,
	};
	
	DECLARE_SUBSTANCE( kingdom_bath_player_imp )

	kingdom_bath_player_imp()
	{
		_timeout = 0;
		_kick_timeout = 0;
		_bath_count = 0;
		_is_leaving = 0;
		_old_world_tag = 0;
		_old_pos = A3DVECTOR(0,0,0);
		_battle_faction = 0;
		_attack_faction = 0;
		_defense_faction = 0;
	}

	virtual void PlayerEnterWorld();
	virtual void PlayerLeaveWorld();
	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();

	virtual int MessageHandler( const MSG& msg );

	virtual void PlayerLeaveKingdom();

	virtual int GetFaction();
	virtual int GetEnemyFaction();
	virtual void OnHeartbeat( size_t tick );

	virtual const A3DVECTOR& GetLogoutPos( int &world_tag );

	virtual void PlayerBathInvite(const XID &target);
	virtual void PlayerBathInviteReply(const XID & target, int param);
	virtual void OnBathStart(bool is_leader);
	virtual void OnBathEnd(bool is_leader);
private:
	void LeaveBathWorld();

private:
	int _timeout;			//踢出战场的timeout
	int _kick_timeout;
	int _bath_count;
	bool _is_leaving;		//是否正在退出
	int _old_world_tag;		//进入战场前的tag	
	A3DVECTOR _old_pos;		//进入战场前的位置
	int _attack_faction;		//攻防阵营
	int _defense_faction;		//守方阵营
};

#endif //__ONLINEGAME_GS_KINGDOM3_PLAYER_H
