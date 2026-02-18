#ifndef __ONLINEGAME_GS_PLAYER_STATE_H__
#define __ONLINEGAME_GS_PLAYER_STATE_H__

#include <ASSERT.h>
#include <string.h>

class gplayer_imp;
class player_state
{
protected:
	int _state;
public:
	enum
	{
		STATE_NORMAL,          
		STATE_WAIT_TRADE,         
		STATE_TRADE,                 
		STATE_WAIT_TRADE_COMPLETE,   
		STATE_WAIT_TRADE_READ,       
		STATE_WAIT_FACTION_TRADE, 
		STATE_WAIT_FACTION_TRADE_READ,
		STATE_DISCONNECT,            
		STATE_MARKET,          
		STATE_BIND,            
		STATE_DEATH,		//玩家永远不会是这个状态， 只是当最终消息分发时作为一个消息处理而发送
		STATE_COUNT,		//真正状态的数目

		STATE_SEAL_ROOT 	= 26,		//这不是一个状态，只是一个占位
		STATE_SEAL_SILENT 	= 27,		//这不是一个状态，只是一个占位
		STATE_SEAL_MELEE 	= 28,		//这不是一个状态，只是一个占位
		STATE_SEAL_DIET 	= 29,		//这不是一个状态，只是一个占位
		STATE_IDLE 		= 30,		//这不是一个状态，只是一个占位
		STATE_DEAD 		= 31,		//这不是一个状态，只是一个占位
	};

	enum 
	{
		MSG_OP_DENY = 0,
		MSG_OP_ACCEPT,
		MSG_OP_HANDLE,
	};

public:
	player_state()
	{
		_state = STATE_NORMAL;
	}

	int DispatchMessage(gplayer_imp * imp, const MSG & msg);

	inline int GetStateValue() { return _state;}

public:
	inline void Disconnect() { _state = STATE_DISCONNECT;}
	inline void WaitTradeComplete() { _state = STATE_WAIT_TRADE_COMPLETE;}
	inline void Bind() { _state = STATE_BIND;}
	inline void Normalize() { _state = STATE_NORMAL;}
	inline void StartMarket() { _state = STATE_MARKET;}
	inline void WaitTradeRead() {_state = STATE_WAIT_TRADE_READ;}
	inline void WaitFactionTrade() {_state = STATE_WAIT_FACTION_TRADE;}
	inline void WaitFactionTradeRead() {_state = STATE_WAIT_FACTION_TRADE_READ;}
	inline void Trade() {_state = STATE_TRADE;}
	inline void WaitTrade() { _state = STATE_WAIT_TRADE;}
	
public:

	inline bool IsNormalState() { return STATE_NORMAL == _state;}
	inline bool IsMarketState() { return STATE_MARKET == _state;}
	inline bool IsBindState() { return STATE_BIND == _state;}
	inline bool IsWatingTradeState() { return STATE_WAIT_TRADE == _state;}
	inline bool IsTradeState() { return STATE_TRADE == _state;}
	inline bool IsWaitFactionTradeState() { return STATE_WAIT_FACTION_TRADE == _state;}
	inline bool IsWaitFactionTradeReadState() { return STATE_WAIT_FACTION_TRADE_READ == _state;}
	inline bool IsWaitTradeState() { return STATE_WAIT_TRADE == _state;}
	inline bool IsWaitTradeReadState() { return STATE_WAIT_TRADE_READ == _state;}


	inline bool IsOnline() { return _state != STATE_DISCONNECT;}

	inline bool CanDeathDrop() { return _state == STATE_NORMAL || _state == STATE_DISCONNECT || _state == STATE_BIND;}
	inline bool CanResurrect() { return _state == STATE_NORMAL;}
	inline bool CanSitDown() { return _state == STATE_NORMAL;}
	inline bool CanLongJump() { return _state == STATE_NORMAL || _state == STATE_BIND || _state == STATE_MARKET;}
	inline bool CanSwitch() { return STATE_NORMAL == _state || _state == STATE_BIND;}
	inline bool CanSave() {return _state == STATE_NORMAL || _state == STATE_MARKET || _state == STATE_BIND; }
	inline bool CanLogout() {return _state == STATE_NORMAL || _state == STATE_DISCONNECT || _state == STATE_BIND; }
	inline bool CanSitDwon() { return _state == STATE_NORMAL;}
	inline bool CanShopping() { return _state == STATE_NORMAL || _state == STATE_BIND;}
	inline bool CanTrade() { return _state == STATE_NORMAL;}
	inline bool CanCompleteTrade() { return _state == STATE_WAIT_TRADE || _state == STATE_TRADE || _state == STATE_WAIT_TRADE_COMPLETE;}
	inline bool CanProduceItem(){return  _state == STATE_NORMAL || _state == STATE_BIND || _state == STATE_MARKET;}	
};
#endif

//可能需要 virtual化的操作在player_imp中 
// lost_connection
// LeaveAbnormalState
//
