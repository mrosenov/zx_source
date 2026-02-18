#include <common/message.h>
#include <algorithm>
#include "playerstate.h"
#include "player_imp.h"

namespace 
{


class player_state_imp
{
protected:
	static int __m;
	void ErrorMessage(gplayer_imp * imp, const MSG & msg) const ;
public:
	static player_state_imp * __state_imp[player_state::STATE_COUNT];
	static inline int DeathHandleMessage(gplayer_imp * p_imp, const MSG & msg)
	{
		return __state_imp[player_state::STATE_DEATH]->HandleMessage(p_imp, msg);
	}
public:
	virtual const char * GetName() const { return "unknown player_state_imp";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const = 0;
};

//这里包含了一些不易阅读的内容 包含的位置和顺序是严格要求的
#define __USE_BY_PLAYER_STATE_CPP__
#include "playerstateimp.h"

#define handle_msg_d(imp1,imp2,msg) player_msg_dispatcher::_ACCEPT_D->DispatchMsg(imp1,imp2,msg);
#define handle_msg_a(imp1,imp2,msg) player_msg_dispatcher::_ACCEPT_A->DispatchMsg(imp1,imp2,msg);

void player_state_imp::ErrorMessage(gplayer_imp * imp, const MSG & msg) const
{
	__PRINTF("错误的消息传递策略. role:%d '%s' msg:%s(%d)\n",
			imp->_parent->ID.id,GetName(), GetMsgStr(msg.message),msg.message);
	GLog::log(LOG_ERR,"错误的消息传递策略. role:%d '%s' msg:%s(%d)\n",
			imp->_parent->ID.id,GetName(), GetMsgStr(msg.message),msg.message);
}

class player_state_normal : public  player_state_imp
{
public:
	virtual const char * GetName() const { return "class player_state_normal";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		return 0;
	}
};

class player_state_base_trade : public player_state_imp
{
public:
	virtual const char * GetName() const { return "class player_state_base_trade";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_PICKUP_MONEY:
			if(imp->IsTrade())
			{
				imp->PickupMoneyInTrade(msg.param);
			}
			return 0;
		case GM_MSG_RECEIVE_MONEY:
		case GM_MSG_MONSTER_MONEY:
			if(imp->IsTrade())
			{
				imp->PickupMoneyInTrade(msg.param + imp->CalcMoneyAddon(msg.param));
			}
			return 0;

		case GM_MSG_PICKUP_ITEM:
			if(imp->IsTrade())
			{
				imp->PickupItemInTrade(msg.pos,msg.content,msg.content_length,msg.param);
			}
			return 0;

		default:
			ErrorMessage(imp,msg);
			break;
		}
		return 0;
	}
};

class player_state_wait_trade : public player_state_base_trade
{
public:
	virtual const char * GetName() const { return "class player_state_wait_trade";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_HEARTBEAT:
			if(imp->IsTrade() && !imp->TradeObjectHeartbeat())
			{
				imp->ReplyTradeRequest(false);
				imp->FromTradeToNormal();		
			}
			if(imp->IsConsign() && !imp->ConsignObjectHeartbeat())
			{
				imp->FromConsignToNormal();
			}
			return imp->MessageHandler(msg);

		case GM_MSG_ENCHANT:
			if(((enchant_msg*)msg.content)->helpful)
			{
				return handle_msg_d(imp,this,msg);
			}
		case GM_MSG_HURT:
		case GM_MSG_DUEL_HURT:
		case GM_MSG_ATTACK:
			if(imp->IsTrade())
			{
				//有害法术或者伤害制止了交易操作
				imp->ReplyTradeRequest(false);
				imp->FromTradeToNormal();
			}
			if(imp->IsConsign())
			{
				imp->FromConsignToNormal();
			}
			return handle_msg_d(imp,this,msg);

		default:
			return player_state_base_trade::HandleMessage(imp, msg);
		}
		return 0;
	}
};

class player_state_trade : public player_state_base_trade
{
public:
	virtual const char * GetName() const { return "class player_state_trade";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_ENCHANT:
			if(((enchant_msg*)msg.content)->helpful)
			{
				return handle_msg_d(imp,this,msg);
			}
		case GM_MSG_HURT:
		case GM_MSG_DUEL_HURT:
		case GM_MSG_ATTACK:
			if(imp->IsTrade())
			{
				imp->DiscardTrade();
			}
			return handle_msg_d(imp,this,msg);

		default:
			return player_state_base_trade::HandleMessage(imp, msg);
		}
		return 0;
	}
};

class player_state_wait_trade_complete : public player_state_base_trade
{
public:
	virtual const char * GetName() const { return "class player_state_wait_trade_complete";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_HEARTBEAT:
			if(imp->IsTrade() && !imp->TradeObjectHeartbeat())
			{       
				//让玩家断线 且不进行存盘
				GLog::log(LOG_ERR,"drop player for trade timeout roleid:%d",imp->_parent->ID.id);
				imp->FromTradeToNormal(-1);
				return 0; //这时对象可能已经被释放了
			}
			return imp->MessageHandler(msg);

		default:
			return player_state_base_trade::HandleMessage(imp, msg);
		}
		return 0;
	}
};

class player_state_wait_trade_read : public player_state_base_trade
{
public:
	virtual const char * GetName() const { return "class player_state_wait_trade_read";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_HEARTBEAT:
			if(imp->IsTrade() && !imp->TradeObjectHeartbeat())
			{       
				//让玩家断线 且不进行存盘
				GLog::log(LOG_ERR,"drop player for traderead timeout roleid:%d",imp->_parent->ID.id);
				imp->FromTradeToNormal(-1);
				return 0; //这时对象可能已经被释放了
			}
			if(imp->IsConsign() && !imp->ConsignObjectHeartbeat())
			{
				imp->FromConsignToNormal(-1);
				return 0;
			}
			return imp->MessageHandler(msg);

		default:
			return player_state_base_trade::HandleMessage(imp, msg);
		}
		return 0;
	}
};

class player_state_wait_faction_trade : public player_state_base_trade
{
public:
	virtual const char * GetName() const { return "class player_state_wait_faction_trade";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_HEARTBEAT:
			if(imp->IsTrade() && !imp->TradeObjectHeartbeat())
			{       
				//调用出错处理
				imp->FactionTradeTimeout();
				return 0; //这时对象可能已经被释放了
			}
			return imp->MessageHandler(msg);

		default:
			return player_state_base_trade::HandleMessage(imp, msg);
		}
		return 0;
	}
};

class player_state_wait_faction_trade_read : public player_state_base_trade
{
public:
	virtual const char * GetName() const { return "class player_state_wait_faction_trade_read";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_HEARTBEAT:
			if(imp->IsTrade() && !imp->TradeObjectHeartbeat())
			{       
				GLog::log(LOG_ERR,"drop player for faction traderead timeout roleid:%d",imp->_parent->ID.id);
				imp->FactionTradeTerminateSave();
				//断线且不存盘，前面的保存函数保存了部分内容
				imp->FromFactionTradeToNormal(-1);
				return 0;
			}
			return imp->MessageHandler(msg);

		default:
			return player_state_base_trade::HandleMessage(imp, msg);
		}
		return 0;
	}
};

class player_state_disconnect : public player_state_imp
{
public:
	virtual const char * GetName() const { return "class player_state_disconnect";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_HEARTBEAT:
			if(!imp->DisconnectHeartbeat()) return 0;
			return imp->MessageHandler(msg);

		default:
			ErrorMessage(imp, msg);
			break;
		}
		return 0;
	}
};

class player_state_market : public player_state_imp
{
public:
	virtual const char * GetName() const { return "class player_state_market";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_QUERY_MARKET_NAME:
		case GM_MSG_SERVICE_HELLO:
		case GM_MSG_SERVICE_REQUEST:
		case GM_MSG_SERVICE_QUIERY_CONTENT:
			return imp->MarketHandler(msg);

		default:
			ErrorMessage(imp,msg);
			break;
		}
		return 0;
	}
};

class player_state_bind : public player_state_imp
{
public:
	virtual const char * GetName() const { return "class player_state_bind";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		return 0;
	}
};

class player_state_dead : public player_state_imp
{
public:
	virtual const char * GetName() const { return "class player_state_dead";}
	virtual int HandleMessage(gplayer_imp * imp, const MSG & msg) const
	{
		switch(msg.message)
		{
		case GM_MSG_GATHER_REPLY:
			imp->SendTo<0>(GM_MSG_GATHER_CANCEL,msg.source,0);
			return 0;

		default:
			ErrorMessage(imp,msg);
			break;
		}
		return 0;
	}
};

player_state_imp * player_state_imp::__state_imp[]=
{
	new player_state_normal,
	new player_state_wait_trade,
	new player_state_trade,
	new player_state_wait_trade_complete,
	new player_state_wait_trade_read,
	new player_state_wait_faction_trade,
	new player_state_wait_faction_trade_read,
	new player_state_disconnect,
	new player_state_market,
	new player_state_bind,
	new player_state_dead,
};

}

int 
player_state::DispatchMessage(gplayer_imp * imp, const MSG & msg)
{
	return GetMsgDispatcher(msg, _state)->DispatchMsg(imp, player_state_imp::__state_imp[_state], msg);
}

const char * Msg2String(int msg)
{
	return GetMsgStr(msg);
}

