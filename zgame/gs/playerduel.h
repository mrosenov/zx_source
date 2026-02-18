#ifndef __ONLINEGAME_GS_PLAYER_DUEL_H__
#define __ONLINEGAME_GS_PLAYER_DUEL_H__

#include <common/types.h>
#include <string.h>
#include "attack.h"
class gplayer_imp;
class player_duel
{
protected:
	XID  _duel_target;	//和谁决斗
	int  _duel_timeout;	//决斗的超时时间
	int  _duel_mode;	//是否决斗状态 0 无, 1 等待 2 开始

	struct
	{
		XID 	duel_target;	//邀请目标
		int	timeout;	//邀请超时记录
		bool 	is_invite;	//是否邀请了对象
		bool 	is_agree_duel;	//是否同意了决斗
	} _invite;

	enum 
	{
		DUEL_INVITE_TIMEOUT 	= 30,
		DUEL_TIME_LIMIT 	= 600,

		DUEL_RESULT_TIMEOUT	= 0,
		DUEL_RESULT_VICTORY	= 1,
		DUEL_RESULT_DEUCE	= 2,

	};

	void SetDuelPrepareMode();
	void SetDuelStartMode(gplayer_imp * pImp);
	void ClearDuelMode(gplayer_imp * pImp);
	bool IsPrepareMode();
	bool IsDuelStarted();

public:	
	enum {
		DUEL_REPLY_SUCCESS	= 0,
		DUEL_REPLY_TIMEOUT	= 1,
		DUEL_REPLY_REJECT	= 2,
		DUEL_REPLY_OUT_OF_RANGE	= 3,
		DUEL_REPLY_ERR_STATUS	= 4,
	};


	player_duel():_duel_target(-1,-1),_duel_timeout(0),_duel_mode(false)
	{
		memset(&_invite,0,sizeof(_invite));
	}

	template <typename WRAPPER> void Save(WRAPPER & wrapper)
	{
		wrapper.push_back(this, sizeof(*this));
	}

	template <typename WRAPPER> void Load(WRAPPER & wrapper)
	{
		wrapper.pop_back(this, sizeof(*this));
	}

	void Swap(player_duel & rhs)
	{
		player_duel tmp = rhs;
		rhs = * this;
		*this = tmp;
	}
	
	inline bool IsDuelMode()
	{
		return _duel_mode;
	}

	inline const XID & GetDuelTarget()
	{
		return _duel_target;
	}

	void PlayerDuelRequest(gplayer_imp * pImp,const XID & target);
	void PlayerDuelReply(gplayer_imp * pImp,const XID & target,int param);

	void MsgDuelRequest(gplayer_imp * pImp,const XID & who);
	void MsgDuelReply(gplayer_imp * pImp,const XID & who,int param);
	void MsgDuelPrepare(gplayer_imp * pImp,const XID & who);
	void MsgDuelCancel(gplayer_imp * pImp, const XID & who);
	void MsgDuelStart(gplayer_imp * pImp, const XID & who);
	void MsgDuelStop(gplayer_imp * pImp, const XID & who,int param);

	void Heartbeat(gplayer_imp *pImp);
	void OnDeath(gplayer_imp * pImp, bool duel_failed);

/*
	PlayerDuelRequest ---> MSG ---> MsgDuelRequest
	PlayerDuelReply ---> MSG ---> MsgDuelReply
*/
	

};

class mafia_duel
{
	int _state;
	int _enemy;
	int _end_time;
	int _kills;
	int _deaths;
	enum 
	{
		STATE_NORMAL,
		STATE_BATTLE
	};
	void CheckBattleStart(gplayer_imp * pImp,int mafia_id);
	void CheckBattleEnd(gplayer_imp * pImp);
public:
	mafia_duel():_state(STATE_NORMAL),_enemy(0),_end_time(0),_kills(0),_deaths(0)
	{}

	void ClearBattle();
	void CheckAndSetBattle(int mafia_id);
	void OnDeath(int mafia_id, const XID & self, const XID & killer);

	inline bool InBattle() { return _state;}
	inline bool IsEnemy(int id) { return _enemy == id;}
	inline int GetEnemy() const { return _enemy; }
	inline void Heartbeat(gplayer_imp *pImp, int mafia_id)
	{
		if(_state == STATE_NORMAL)
		{
			CheckBattleStart(pImp, mafia_id);
		}
		else
		{
			CheckBattleEnd(pImp);
		}
	}
	inline void KillEnemy()
	{
		_kills ++;
	}

	inline void SetKills(int value) 
	{
		_kills = value;
	}

	inline void SetDeaths(int value)
	{
		_deaths = value;
	}
	inline int GetKills() const
	{
		return _kills;
	}
	inline int GetDeaths() const
	{
		return _deaths;
	}
public:
	template <typename ATTACK_MSG>
	bool AdjustAttack(int mafia_id , ATTACK_MSG & msg)
	{
		if(msg.attacker_mode & attack_msg::PVP_SANCTUARY) return true;
		if(mafia_id == msg.ainfo.mafia_id) return false;
		if(_enemy == msg.ainfo.mafia_id) 
		{
			//如果是帮战 则修正之
			msg.attacker_mode |=  attack_msg::PVP_MAFIA_DUEL;
		}
		return true;
	}
	
	template <typename ENCHANT_MSG>
	int AdjustEnchant(int mafia_id , ENCHANT_MSG & msg)
	{
		//敌对帮
		if(_enemy == msg.ainfo.mafia_id) return 0;

		if(msg.attacker_mode & attack_msg::PVP_SANCTUARY) return -1;
		//本帮
		if(mafia_id == msg.ainfo.mafia_id) return 1;
		return -1;
	}
	
};

class faction_hostile_duel
{
	abase::static_set<unsigned int> _hostiles;
	int _refresh_seq;

public:
	faction_hostile_duel(): _refresh_seq(0)
	{
		_hostiles.reserve( 5 );
	}

	void Refresh( gplayer_imp* pImp, unsigned int faction_id, bool bForce = false );
	
	inline void Heartbeat( gplayer_imp *pImp, unsigned int faction_id )
	{
		Refresh( pImp, faction_id );
	}
	
	inline bool IsHostile( unsigned int hostile_faction_id )
	{
		abase::static_set<unsigned int>::iterator it = _hostiles.find( hostile_faction_id );
		if( _hostiles.end() == it ) return false;
		return true;
	}
	
	void Swap( faction_hostile_duel& rhs )
	{
		int temp;
		temp = rhs._refresh_seq;
		rhs._refresh_seq = _refresh_seq;
		_refresh_seq = temp;
		_hostiles.swap( rhs._hostiles );
	}

	inline bool HaveHostiles() const { return ( 0 != _hostiles.size() ); }

public:
	template <typename ATTACK_MSG>
	bool AdjustAttack( int faction_id , ATTACK_MSG& msg )
	{
		if( msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return true;
		if( faction_id == msg.ainfo.mafia_id ) return false;
		if( IsHostile( msg.ainfo.mafia_id ) )
		{
			// 如果是敌对帮派加个标志
			msg.attacker_mode |=  attack_msg::PVP_HOSTILE_DUEL;
		}
		return true;
	}

	template <typename ENCHANT_MSG>
	int AdjustEnchant( int mafia_id , ENCHANT_MSG& msg )
	{
		// 是敌对帮派
		if( IsHostile( msg.ainfo.mafia_id ) ) return 0;
		if( msg.attacker_mode & attack_msg::PVP_SANCTUARY ) return -1;
		//本帮
		if( mafia_id == msg.ainfo.mafia_id ) return 1;
		return -1;
		// 0 return false; 1 return true; -1 go on;
	}
};

#endif

