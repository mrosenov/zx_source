#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>

#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>
#include "userlogin.h"
#include "trade.h"
#include <factionlib.h>
#include "consign.h"
#include "task/taskman.h"

#define CONSIGN_ROLE_REQUIRED_LEVEL (90)

bool 
gplayer_imp::CanTrade(const XID & target)
{
	//ÏÖÔÚÒ»ÂÉ½ûÖ¹½»Ò×
//	return false;
	
	//·ÇÕý³£×´Ì¬²»ÄÜ½»Ò×
	if(!_pstate.CanTrade()) return false;
	ASSERT(_trade_obj == NULL);

	//½»Ò×²»ÄÜ¿ªÊ¼µÄÌõ¼þÓÐ
	//µ±Ç°ÓÐÈÎºÎÀàÐÍµÄsession ÕýÔÚÖ´ÐÐÖÐ
	if(_cur_session || _session_list.size()) return false;

	if(OI_TestSafeLock()) return false;

	if(GetWorldManager()->GetWorldLimit().notrade) return false;
/*
	//ÏÖÔÚÕ½¶·×´Ì¬ÔÊÐí½»Ò× 
	//¿´¿´»á²»»á³öÊ²Ã´ÎÊÌâ..
	//Õ½¶·×´Ì¬Ò²²»ÄÜ½»Ò× 
	if(IsCombatState()) return false;
	*/
	
	//ËÀÍö×´Ì¬²»ÄÜ½»Ò× ÔÚÍâÃæÅÐ¶Ï
	//»¹ÓÐÌõ¼þÊÇ ¾àÀë²»ÄÜ³¬¹ý·¶Î§£¬ÔÚÍâÃæÅÐ¶Ï

	//·µ»Øtrue
	return true;

}

void
gplayer_imp::StartTrade(int trade_id, const XID & target)
{
	//Õ½¶·×´Ì¬ÏÖÔÚÔÊÐí½»Ò×
	ASSERT(_pstate.CanTrade() && !_cur_session);
	ASSERT(_trade_obj == NULL);

	//¼ÓÈë½»Ò×¶ÔÏó£¬ ½øÈëµÈ´ý½»Ò××´Ì¬
	_trade_obj = new player_trade(trade_id);
	_pstate.WaitTrade();
	_trade_obj->SetTimeOut(30);	//ÉèÖÃÈýÊ®Ãë´æÅÌ³¬Ê±´íÎó

	class WaitInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		int _trade_id;
		unsigned int _counter;
		unsigned int _counter2; 
		int _cur_order_id;
		world *_plane;
	public:
		WaitInfoWriteBack(int trade_id, gplayer_imp * imp,world * pPlane):_trade_id(trade_id),_plane(pPlane)
		{
			gplayer * pPlayer = imp->GetParent();
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
			_counter = imp->GetTrashBoxCounter();
			_counter2 = imp->_eq_change_counter;
			_cur_order_id = imp->_mallinfo.GetOrderID();
		}

		virtual void OnTimeOut()
		{
			CallWriteBack(false);
			delete this;
		}

		virtual void OnFailed()
		{
			CallWriteBack(false);
			delete this;
		}
		
		virtual void OnPutRole(int retcode)
		{
			ASSERT(retcode == 0);
			CallWriteBack(retcode == 0);
			delete this;
		}

		void CallWriteBack(bool success)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; //can't find
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;	// not match
			}
			ASSERT(pPlayer->imp);
			gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
			if(success)
			{
				if(_counter == pImp->GetTrashBoxCounter() )
				{
					pImp->TryClearTBChangeCounter();
				}

				if(_counter2 == pImp->_eq_change_counter )
				{
					pImp->_eq_change_counter = 0;
				}
				//ÉèÖÃ×î½üÒ»´Î´æÅÌµÄ°Ù±¦¸óÁ÷Ë®ºÅ
				pImp->_mallinfo.SaveDone(_cur_order_id);

				pImp->_db_save_error = 0;
			}
			pImp->WaitingTradeWriteBack(_trade_id,success);
		}


	};

	//½øÐÐ´æÅÌ²Ù×÷
	user_save_data((gplayer*)_parent,new WaitInfoWriteBack(trade_id,this,_plane),0);
	return ;
}

void
gplayer_imp::TradeComplete(int trade_id, int reason,bool need_read)
{
	if(!_pstate.CanCompleteTrade())
	{
		//Î´ÔÚºÏÊÊµÄ×´Ì¬ÖÐ
		return;
	}

	class WaitInfoReadBack: public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		int _trade_id;
		world * _plane;
	public:
		WaitInfoReadBack(int trade_id, gplayer * pPlayer,world *pPlane):_trade_id(trade_id),_plane(pPlane)
		{
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
		}

		virtual void OnTimeOut()
		{
			CallReadBack(NULL);
			delete this;
		}

		virtual void OnFailed()
		{
			CallReadBack(NULL);
			delete this;
		}
		
		virtual void OnGetMoneyInventory(size_t money, const GDB::itemlist & list, int timestamp)
		{
			CallReadBack(&list, money);
			delete this;
		}

		void CallReadBack(const GDB::itemlist * pInv,int money = 0)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; 
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;
			}
			ASSERT(pPlayer->imp);
			((gplayer_imp*)pPlayer->imp)->WaitingTradeReadBack(_trade_id,pInv,money);
		}


	};

	if(_trade_obj->GetTradeID() != trade_id)
	{
		//½»Ò×ID²»·ûºÏÔò¶ÏÏßÖ®
		GLog::log(GLOG_ERR,"ÓÃ»§%d½»Ò×ID%dÍê³ÉºóID²»·ûºÏ(Ó¦Îª%d)",_parent->ID.id, trade_id,_trade_obj->GetTradeID());
		FromTradeToNormal(-1);
		return;
	}

	if(need_read)
	{	
		//½øÈë¶ÁÅÌµÈ´ý×´Ì¬
		_pstate.WaitTradeRead();
		//·¢Æð¶ÁÅÌÇëÇó²¢ÉèÖÃ³¬Ê±
		_trade_obj->SetTimeOut(45);

		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack(trade_id,(gplayer*)_parent,_plane));

	}
	else
	{
		//²»ÐèÒªÖØÐÂ¶ÁÅÌ£¬Ö±½Ó·µ»Ønormal×´Ì¬
		FromTradeToNormal();
	}
	return;
}

bool
gplayer_imp::StartFactionTrade(int trade_id,int get_mask, int put_mask ,bool no_response)
{
	if(!_pstate.CanTrade() || _cur_session) return false;
	ASSERT(_trade_obj == NULL);

	//¼ÓÈë½»Ò×¶ÔÏó£¬ ½øÈëµÈ´ý½»Ò××´Ì¬
	_trade_obj = new faction_trade(trade_id, put_mask);
	_pstate.WaitFactionTrade();
	_trade_obj->SetTimeOut(45);	//ÉèÖÃÈýÊ®Ãë³¬Ê±´íÎó

	if(!no_response)
	{
		//·¢ËÍ¸öÈËÊý¾Ý
		GNET::syncdata_t data(GetMoney(),0);
//BW		GNET::SendFactionLockResponse(0,trade_id,_parent->ID.id,data);
	}
	((gplayer_dispatcher*)_runner)->mafia_trade_start();
	return true;
}

//ÏÖÔÚÖ»ÓÃÓÚÔª±¦³äÖµ¼´Ê±µ½ÕË£¬ÒòÎªÃ»ÓÐ¶¯°ü¹üËùÒÔ²»ÅÐ¶Ï_cur_session
bool    
gplayer_imp::StartTradeCash(int trade_id, int get_mask, int put_mask)
{       
	if(!_pstate.CanTrade()) return false;
	ASSERT(_trade_obj == NULL);

	//¼ÓÈë½»Ò×¶ÔÏó£¬½øÈëµÈ´ý½»Ò××´Ì¬
	_trade_obj = new faction_trade(trade_id, put_mask);
	_pstate.WaitFactionTrade();
	_trade_obj->SetTimeOut(45); //ÉèÖÃËÄÊ®ÎåÃë³¬Ê±´íÎó

	return true;
}

void 
gplayer_imp::WaitingTradeWriteBack(int trade_id,bool bSuccess)
{
	if(!_pstate.IsWaitTradeState()) return;
	if(_trade_obj->GetTradeID() != trade_id) return;

	gplayer * pPlayer = (gplayer*)_parent;
	if(bSuccess)
	{
		//Çå³ýÒ»ÏÂ²Ö¿â¸Ä±ä¼ÇÂ¼
		TryClearTBChangeCounter();

		//Ð´ÈëÊý¾Ý³É¹¦
		//·¢ËÍÍ¬Òâ½»Ò×µÄÊý¾Ý
		GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),pPlayer->ID.id,pPlayer->cs_sid,true);

		//½øÈë½»Ò××´Ì¬
		_pstate.Trade();
	}
	else
	{
		//Ð´ÈëÊý¾ÝÊ§°Ü
		//·¢ËÍ²»Í¬Òâ½»Ò×µÄÊý¾Ý
		GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),pPlayer->ID.id,pPlayer->cs_sid,false);

		//»Øµ½·Ç½»Ò××´Ì¬
		FromTradeToNormal();
	}
}

void 
gplayer_imp::WaitingTradeReadBack(int trade_id,const GDB::itemlist * pInv,int money)
{
	if(!_pstate.IsWaitTradeReadState()) return;
	if(_trade_obj->GetTradeID() != trade_id) return;
	if(pInv)
	{
		GLog::log(GLOG_INFO,"ÓÃ»§%d½»Ò×³É¹¦£¬½»Ò×ID%d½ðÇ®¸Ä±äÎª%d",_parent->ID.id, trade_id,money - GetMoney());
		//¶ÁÈ¡Êý¾Ý³É¹¦
		//ÖØÐÂÕûÀí×Ô¼ºµÄÎïÆ·À¸
		_inventory.Clear();
		_inventory.InitFromDBData(*pInv);
		if(((size_t)money) > _money_capacity) 
		{
			//
			GLog::log(GLOG_ERR,"ÓÃ»§%d½»Ò×½ðÇ®³¬¹ý×î´óÈÝÁ¿",_parent->ID.id);
			money = _money_capacity;
		}
		_player_money = money;

		//»Øµ½Í¨³£×´Ì¬
		FromTradeToNormal();
	}
	else
	{
		GLog::log(GLOG_INFO,"ÓÃ»§%d½»Ò×ID%dÍê³ÉºóÈ¡Êý¾ÝÊ§°Ü",_parent->ID.id, trade_id);
		//¶ÁÈ¡Êý¾ÝÊ§°Ü Ö±½ÓÏÂÏßÊÇ¸öºÃÖ÷Òâ
		//²¢ÇÒ²»½øÐÐÐ´ÅÌ²Ù×÷
		FromTradeToNormal(-1);
	}
}

void 
gplayer_imp::PutItemAfterTrade(item_list & backpack)
{
	XID self = _parent->ID;
	for(size_t i = 0; i < backpack.Size();i ++)
	{
		if(backpack[i].type == -1) break;
		item it;
		backpack.Remove(i,it);
		ASSERT(it.type != -1);
		size_t ocount = it.count;
		int expire_date = it.expire_date;
		int type = it.type;
		int rst = _inventory.Push(it);
		if(rst >= 0)
		{
			//»ñµÃÁË¶«Î÷
			int state = item::Proctype2State(it.proc_type);
			((gplayer_dispatcher*)_runner)->receive_item_after_trade(type,expire_date,ocount-it.count,_inventory[rst].count,rst,state);
		}

		if(it.type != -1)
		{	
			//ÎÞ·¨·ÅÏÂÁË£¬½øÐÐ¶ªÆúµÄ²Ù×÷
			item_data data;
			item_list::ItemToData(it,data);
			DropItemFromData(_plane,_parent->pos,data,self,0,0); 
			it.Release();
		}
	}
}

void 
gplayer_imp::DropAllAfterTrade(item_list & backpack,size_t money)
{
	XID self = _parent->ID;
	const A3DVECTOR & pos = _parent->pos;
	for(size_t i = 0; i < backpack.Size();i ++)
	{
		if(backpack[i].type == -1) break;
		item it;
		backpack.Remove(i,it);
		ASSERT(it.type != -1);
		item_data data;
		item_list::ItemToData(it,data);
		DropItemFromData(_plane,pos,data,self,0,0); 
		it.Release();
	}
	if(money) DropMoneyItem(_plane,pos,money,self,0,0);
}

void 
gplayer_imp::FromTradeToNormal(int type)
{
	if(type < 0)
	{
		//½«ÎïÆ·ÈÓÔÚµØÉÏ
		DropAllAfterTrade(_trade_obj->GetBackpack(),_trade_obj->GetMoney());

		delete _trade_obj;
		_trade_obj = NULL;
		_pstate.Normalize();

		//È»ºó½øÐÐÏÂÏß²Ù×÷£¬²»½øÐÐ´æÅÌ
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}
	//¸æËßÍæ¼Òµ±Ç°µÄ°ü¹üÀ¸ÄÚÈÝ
	PlayerGetInventoryDetail(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//Ê×ÏÈ½«»ýÀÛµÄÎïÆ·ºÍÇ®½»¸øÍæ¼Ò
	size_t inc_money = _trade_obj->GetMoney();
	if(inc_money)
	{
		size_t tmp = GetMoney();
		GainMoneyWithDrop(inc_money);
		((gplayer_dispatcher*)_runner)->receive_money_after_trade(GetMoney() - tmp);
	}
	PutItemAfterTrade(_trade_obj->GetBackpack());

	//È»ºóÉ¾³ý¶ÔÏó£¬»Øµ½½»Ò××´Ì¬
	delete _trade_obj;
	_trade_obj = NULL;

	//È»ºó»Øµ½Í¨ÓÃ×´Ì¬ ²¢ÇÒÍ¨ÖªplayerÏÖÔÚ´¦ÓÚ·Ç½»Ò××´Ì¬
	//ËÀÍö×´Ì¬ÒÑ¾­¶ÀÁ¢³öÀ´ÁË
	_pstate.Normalize();
	_write_timer = 512;

	if(_parent->b_disconnect)
	{
		//Íæ¼ÒÒÑ¾­¶ÏÏß£¬Ö´ÐÐ¶ÏÏßÂß¼­  ²¢ÇÒ´æÅÌ
		//½øÈë¶ÏÏßÂß¼­
		_pstate.Disconnect();
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
	else
	{
		//½»Ò×½áÊøºóÖØÐÂ¼ì²éÒ»ÏÂÈÎÎñ£¬·ÀÖ¹ÈÎÎñ±»¿¨
		//ÕâÀï²»ÄÜÖ±½Óµ÷ÓÃOnTaskCheckState, ÒòÎªÈç¹ûÊÕµ½¿Í»§¶ËµÄTASK_NOTIFYÃüÁîºó
		//ÔÚOnTaskCheckAwardDirectÀïÃæÈç¹û¿ÉÒÔÖ±½ÓÇå³ý·Ç·¨×´Ì¬µÄ»°, »áÈ¡Ïû°ÚÌ¯×´Ì¬£ Õâ¸öÊ±ºòÈç¹ûÖ±½Óµ÷ÓÃOnTaskCheckState
		//»áµ¼ÖÂÄÚ´æ´íÎó
		MSG msg;
		BuildMessage(msg,GM_MSG_TASK_CHECK_STATE,_parent->ID,_parent->ID,A3DVECTOR(0,0,0),0);
		gmatrix::SendMessage(msg);
	}
}

void 
gplayer_imp::FromFactionTradeToNormal(int type)
{
	if(type)
	{
		//½«ÎïÆ·ÈÓÔÚµØÉÏ
		DropAllAfterTrade(_trade_obj->GetBackpack(),_trade_obj->GetMoney());

		delete _trade_obj;
		_trade_obj = NULL;
		_pstate.Normalize();

		//È»ºó½øÐÐÏÂÏß²Ù×÷£¬²»½øÐÐ´æÅÌ
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}

	ASSERT(_pstate.IsWaitFactionTradeState() || _pstate.IsWaitFactionTradeReadState());
	//¸æËßÍæ¼Òµ±Ç°µÄ°ü¹üÀ¸ÄÚÈÝ
	//ÅÐ¶ÏÊÇ·ñ¸æËßÍæ¼ÒË¢ÐÂ×°±¸ ÉÐÎ´×ö
	//PlayerGetInventory(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//Ê×ÏÈ½«»ýÀÛµÄÎïÆ·ºÍÇ®½»¸øÍæ¼Ò
	size_t inc_money = _trade_obj->GetMoney();
	if(inc_money)
	{
		size_t tmp = GetMoney();
		GainMoneyWithDrop(inc_money);
		((gplayer_dispatcher*)_runner)->receive_money_after_trade(GetMoney() - tmp);
	}
	PutItemAfterTrade(_trade_obj->GetBackpack());

	//°ïÅÉ½»Ò×ÏÖÔÚÏÈ²»ÖØÐÂ·¢ËÍÎïÆ·Êý¾Ý
	//PlayerGetInventory(0);

	//È»ºóÉ¾³ý¶ÔÏó
	delete _trade_obj;
	_trade_obj = NULL;

	//È»ºó»Øµ½Í¨ÓÃ×´Ì¬ ²¢ÇÒÍ¨ÖªplayerÏÖÔÚ´¦ÓÚ·Ç½»Ò××´Ì¬
	//ËÀÍö×´Ì¬ÒÑ¾­¶ÀÁ¢³öÀ´ÁË
	_pstate.Normalize();

	((gplayer_dispatcher*)_runner)->mafia_trade_end();
	if(_parent->b_disconnect)
	{
		//Íæ¼ÒÒÑ¾­¶ÏÏß£¬Ö´ÐÐ¶ÏÏßÂß¼­  ²¢ÇÒ´æÅÌ
		Logout(GMSV::PLAYER_LOGOUT_FULL);
	}
}

void 
gplayer_imp::FactionTradeTimeout()
{
	ASSERT(_pstate.IsWaitFactionTradeState());
	class WaitInfoReadBack: public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		world * _plane;
	public:
		WaitInfoReadBack(gplayer * pPlayer,world *pPlane):_plane(pPlane)
		{
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
		}

		virtual void OnTimeOut()
		{
			//CallReadBack(NULL);
			//ÕâÀï²»ÄÜÔÚµ÷ÓÃrpcÁË£¬»áËÀËø
			//ËùÒÔÊ¹ÓÃtrade_objµÄ³¬Ê±
			delete this;
		}

		virtual void OnFailed()
		{
			CallReadBack(NULL);
			delete this;
		}
		
		virtual void OnGetMoneyInventory(size_t money, const GDB::itemlist & list,int timestamp)
		{
			CallReadBack(&list, money, timestamp);
			delete this;
		}

		void CallReadBack(const GDB::itemlist * pInv,int money = 0, int timestamp = 0)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; 
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;	
			}
			ASSERT(pPlayer->imp);
			gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
			pImp->WaitingFactionTradeReadBack(pInv, money, timestamp);
		}

	};

	//½øÈë¶ÁÅÌµÈ´ý×´Ì¬
	_pstate.WaitFactionTradeRead();
	//·¢Æð¶ÁÅÌÇëÇó²¢ÉèÖÃ³¬Ê±
	_trade_obj->SetTimeOut(45);

	faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
	if(pTrade)
	{
		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack((gplayer*)_parent,_plane));
	}
	else
	{
		ASSERT(false);
		//Ó¦¸ÃÈÃÍæ¼Ò¶ÏÏß£¬²¢ÇÒ²»´æÅÌ.....
		FromFactionTradeToNormal(-1);
	}
	return;
}

void 
gplayer_imp::WaitingFactionTradeReadBack(const GDB::itemlist * pInv,int money, int timestamp)
{
	if(!_pstate.IsWaitFactionTradeReadState()) return ;
	if(pInv)
	{
		if(((int)(timestamp -_db_timestamp)) >= 0)
		{
			//ÐÞÕýÇ®Êý
			if(((size_t)money) > _money_capacity) 
			{
				//
				GLog::log(GLOG_ERR,"ÓÃ»§%dÍâ²¿½»Ò×³¬Ê±ºó£¬½ðÇ®³¬¹ý×î´óÈÝÁ¿",_parent->ID.id);
				money = _money_capacity;
			}
			_player_money = money;
			GLog::log(GLOG_INFO,"ÓÃ»§%dÍâ²¿½»Ò×³¬Ê±ºó£¬½ðÇ®¸Ä±ä ª%d" ,_parent->ID.id,money - GetMoney());

			//¶ÁÈ¡Êý¾Ý³É¹¦
			//ÖØÐÂÕûÀí×Ô¼ºµÄÎïÆ·À¸
			_inventory.Clear();
			_inventory.InitFromDBData(*pInv);
			PlayerGetInventoryDetail(0);
			_db_timestamp = timestamp;
		}
		else
		{
			GLog::log(GLOG_ERR,"ÓÃ»§%dÍâ²¿½»Ò×½áÊø£¬µ«Ê±´Á½ÏÐ¡%d(%d)",_parent->ID.id,timestamp,_db_timestamp);
			//¶Á³öÀ´µÄÊ±¼ä´Á¹ýÐ¡£¬²»Ê¹ÓÃ ¶øÊÇÓÃ×Ô¼ºµÄÊý¾Ý
		}

		//»Øµ½Í¨³£×´Ì¬
		FromFactionTradeToNormal();
	}
	else
	{
		//¶ÁÈ¡Êý¾ÝÊ§°Ü 
		GLog::log(GLOG_ERR,"ÓÃ»§%dÍâ²¿½»Ò×Íê³ÉºóÈ¡Êý¾ÝÊ§°Ü",_parent->ID.id);
		
		//±£´æÒ»´Î·Ç°ü¹üÊý¾Ý
		faction_trade * pTrade = dynamic_cast<faction_trade*>(_trade_obj);
		if(pTrade)
		{
			user_save_data((gplayer*)_parent,NULL,2,pTrade->_put_mask);
		}
		else
		{
			ASSERT(false);
		}
		
		//²¢ÇÒ²»½øÐÐÐ´ÅÌ²Ù×÷
		FromFactionTradeToNormal(-1);
	}
}

void 
gplayer_imp::FactionTradeComplete(int trade_id,const GNET::syncdata_t & data)
{
	if(!_pstate.IsWaitFactionTradeState())
	{
		//Î´ÔÚºÏÊÊµÄ×´Ì¬ÖÐ
		return;
	}
	GLog::log(GLOG_INFO,"ÓÃ»§%d°ïÅÉ²Ù×÷Íê³É£¬½ðÇ®¸Ä±äÎª%d",_parent->ID.id, data.money - GetMoney());

	//¸üÐÂÊý¾Ý
	ASSERT(_trade_obj != NULL);
	ASSERT(data.money >=0 && data.money <= _money_capacity);
	
	if(trade_id != _trade_obj->GetTradeID()) return ;

	//·¢ËÍ³É¹¦Êý¾Ý
	//Í¨Öª¿Í»§¶ËÍê³É´Ë²Ù×÷£¬¿ÉÒÔ½øÐÐ²Ù×÷
	_player_money = data.money;
	
	SetRefreshState();
	//GSMV_SEND FACTIONTRADECOMPELETE
	//×îºÃ½øÐÐ´æÅÌ²Ù×÷
	
	FromFactionTradeToNormal();
	return;
}

void 
gplayer_imp::SyncTradeComplete(int trade_id,size_t money, const GDB::itemlist & item_change,bool writetrashbox, bool money_change, bool cash_change)
{
	if(!_pstate.IsWaitFactionTradeState())
	{
		//Î´ÔÚºÏÊÊµÄ×´Ì¬ÖÐ
		return;
	}
	//¸üÐÂÊý¾Ý
	ASSERT(_trade_obj != NULL);
	if(money_change)
	{
		ASSERT(money >=0 && money <= _money_capacity);
	}
	else
	{
		money = GetMoney();
	}
	
	GLog::log(GLOG_INFO,"ÓÃ»§%dÍâ²¿²Ù×÷Íê³É£¬½ðÇ®¸Ä±äÎª%d",_parent->ID.id, money - GetMoney());

	if(trade_id != _trade_obj->GetTradeID()) return ;

	//·¢ËÍ³É¹¦Êý¾Ý
	//Í¨Öª¿Í»§¶ËÍê³É´Ë²Ù×÷£¬¿ÉÒÔ½øÐÐ²Ù×÷
	_player_money = money;

	//½øÐÐ°ü¹üÀ¸µÄÐÞ¸Ä
	for(size_t i = 0; i < item_change.count; i ++)
	{
		GDB::itemdata * pData = item_change.list + i;
		if(pData->count)
		{
			item & it = _inventory[pData->index];
			//¸üÐÂ²Ù×÷
			if(pData->id != (unsigned int)it.type) 
			{
				_inventory.Remove(pData->index);
				item it2;
				if(MakeItemEntry(it2,*pData))
				{
					bool bRst = _inventory.Put(pData->index,it2);
					ASSERT(bRst);
				}
				else
				{
					GLog::log(LOG_ERR, "SyncTradeCompleteÊ±´´½¨ÎïÆ·Ê§°Ü%d",it2.type);
					it2.Clear();
				}
			}
			else
			{
				if((unsigned int)pData->count > it.count)
				{
					_inventory.IncAmount(pData->index, pData->count - it.count);
				}
				else
				{
					_inventory.DecAmount(pData->index, it.count - pData->count);
				}
			}
		}
		else
		{
			//É¾³ý²Ù×÷
			_inventory.Remove(pData->index);
		}
	}
	
	if(writetrashbox)
	{
		TryClearTBChangeCounter();
	}
	if(item_change.count)
	{
		PlayerGetInventoryDetail(0);
	}

	if(cash_change)
	{
		_mallinfo.SaveDone(_mallinfo.GetOrderID());
	}
	FromFactionTradeToNormal();
	return;
}

int gplayer_imp::CanConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time)
{
	if(!_pstate.CanTrade()) return -1;
	ASSERT(_consign_obj == NULL);
	if(_cur_session || _session_list.size()) return -2;
	if(OI_TestSafeLock()) 
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return -3;
	}

	//ÊÛ¼Û·¶Î§¼ì²â£¬²»ÄÜµÍÓÚ10£¬²»ÄÜ¸ßÓÚ10w, µ¥Î»RMBÔª
	if(price < 10 || price > 100000) return -12;
	if(sold_time < 1 || sold_time > 7) return -13;
	
	if(type == 1)
	{
		int cost = money + margin;
		if(money < 1000000 || money > 2000000000 || margin < 0  || margin > 2000000000 || cost < 0 || cost > 2000000000 || (int)_player_money < cost) 
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return -4;
		}
	}
	else if(type == 2)
	{
		if(item_id <= 0 || item_cnt <= 0 || item_idx < 0 || item_type <= 0 || (size_t)item_idx >= _inventory.Size()) 
		{
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return -5;
		}
		item& item = _inventory[item_idx];
		if(item.type == -1 || item.type != item_id) 
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return -7;
		}
		if(item_cnt > (int)item.count || item_cnt > (int)item.pile_limit) 
		{
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return -8;
		}
		if(!item.CanTrade() || item.IsBind() || item.expire_date > 0) 
		{
			
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return -9;
		}
		if(margin < 0 || margin > 2000000000 || (int)_player_money < margin) 
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return -10;
		}
	}
	else
	{
		return -11;
	}

	return 0;
}

void gplayer_imp::StartConsign(char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, char* sellto_name, char sellto_name_len)
{
	ASSERT(_pstate.CanTrade() && !_cur_session);
	ASSERT(_consign_obj == NULL);

	//¼ÓÈë¼ÄÊÛ¶ÔÏó£¬½øÈë½»Ò×µÈ´ý×´Ì¬
	_consign_obj = new player_consign;
	_pstate.WaitTrade();
	_consign_obj->SetTimeOut(30); //ÉèÖÃ30Ãë´æÅÌ³¬Ê±´íÎó

	class WaitInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		unsigned int _counter;
		unsigned int _counter2; 
		int _cur_order_id;
		world *_plane;

		char _type;
		int _margin;
		int _item_id;
		int _item_idx;
		short _item_cnt;
		int _item_type;
		int _money;
		int _price;
		int _sold_time;
		char _sellto_name_len;
		char _sellto_name[28];	
		
	public:
		WaitInfoWriteBack(gplayer_imp * imp,world * pPlane, char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, const char* sellto_name, char sellto_name_len)
			:_plane(pPlane) 																																										  
		{
			gplayer * pPlayer = imp->GetParent();
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
			_counter = imp->GetTrashBoxCounter();
			_counter2 = imp->_eq_change_counter;
			_cur_order_id = imp->_mallinfo.GetOrderID();

			_type = type;
			_margin = margin;
			_item_id = item_id;
			_item_idx = item_idx;
			_item_cnt = item_cnt;
			_item_type = item_type;
			_money = money;
			_price = price;
			_sold_time = sold_time;

			_sellto_name_len = sellto_name_len > 28 ? 28 : sellto_name_len;
			memcpy(_sellto_name, sellto_name, _sellto_name_len);
		}
		
		virtual void OnTimeOut()
		{
			CallWriteBack(false);
			delete this;
		}

		virtual void OnFailed()
		{
			CallWriteBack(false);
			delete this;
		}
		
		virtual void OnPutRole(int retcode)
		{
			ASSERT(retcode == 0);
			CallWriteBack(retcode == 0);
			delete this;
		}

		void CallWriteBack(bool success)
		{
			//sleep(35);
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; //can't find
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;	// not match
			}
			ASSERT(pPlayer->imp);
			gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
			if(success)
			{
				if(_counter == pImp->GetTrashBoxCounter() )
				{
					pImp->TryClearTBChangeCounter();
				}

				if(_counter2 == pImp->_eq_change_counter )
				{
					pImp->_eq_change_counter = 0;
				}
				//ÉèÖÃ×î½üÒ»´Î´æÅÌµÄ°Ù±¦¸óÁ÷Ë®ºÅ
				pImp->_mallinfo.SaveDone(_cur_order_id);

				pImp->_db_save_error = 0;
			}
			pImp->WaitingConsignWriteBack(success, _type, _margin, _item_id, _item_cnt, _item_idx, _item_type, _money, _price, _sold_time, _sellto_name, _sellto_name_len);
		}
	};

	//½øÐÐ´æÅÌ²Ù×÷
	WaitInfoWriteBack* wb = new WaitInfoWriteBack(this,_plane, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time, sellto_name, sellto_name_len);
	user_save_data((gplayer*)_parent, wb, 0);
	//user_save_data((gplayer*)_parent,new WaitInfoWriteBack(this,_plane, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time, sellto_name, sellto_name_len),0);
	return ;
}

void gplayer_imp::CompleteConsign(int reason, char need_read)
{
	if(!_pstate.CanCompleteTrade()) return;

	class WaitInfoReadBack: public GDB::Result, public abase::ASmallObject
	{
		int _userid;
		int _cs_index;
		int _cs_sid;
		world * _plane;
	public:
		WaitInfoReadBack(gplayer * pPlayer,world *pPlane):_plane(pPlane)
		{
			_userid = pPlayer->ID.id;
			_cs_index = pPlayer->cs_index;
			_cs_sid = pPlayer->cs_sid;
		}

		virtual void OnTimeOut()
		{
			CallReadBack(NULL);
			delete this;
		}

		virtual void OnFailed()
		{
			CallReadBack(NULL);
			delete this;
		}
		
		virtual void OnGetMoneyInventory(size_t money, const GDB::itemlist & list, int timestamp)
		{
			CallReadBack(&list, money);
			delete this;
		}

		void CallReadBack(const GDB::itemlist * pInv,int money = 0)
		{
			int index = gmatrix::FindPlayer(_userid);
			if(index < 0)
			{
				return; 
			}
			gplayer * pPlayer = _plane->GetPlayerByIndex(index);
			spin_autolock keeper(pPlayer->spinlock);

			if(pPlayer->ID.id != _userid || !pPlayer->IsActived()
			  || pPlayer->cs_index != _cs_index || pPlayer->cs_sid != _cs_sid)
			{
				return;
			}
			ASSERT(pPlayer->imp);
			((gplayer_imp*)pPlayer->imp)->WaitingConsignReadBack(pInv,money);
		}
	};

	if(need_read)
	{	
		//½øÈë¶ÁÅÌµÈ´ý×´Ì¬
		_pstate.WaitTradeRead();
		//·¢Æð¶ÁÅÌÇëÇó²¢ÉèÖÃ³¬Ê±
		_consign_obj->SetTimeOut(45);
		GDB::get_money_inventory(_parent->ID.id,new WaitInfoReadBack((gplayer*)_parent,_plane));
	}
	else
	{
		//²»ÐèÒªÖØÐÂ¶ÁÅÌ£¬Ö±½Ó·µ»Ønormal×´Ì¬
		FromConsignToNormal();
	}
	return;
}

void gplayer_imp::WaitingConsignWriteBack(bool bSuccess, char type, int margin, int item_id, int item_cnt, short item_idx, int item_type, int money, int price, int sold_time, char* sellto_name, char sellto_name_len)
{
	if(!_pstate.IsWaitTradeState()) return;
	
	gplayer* pPlayer = (gplayer*)_parent;
	if(bSuccess)
	{
		//Çå³ýÒ»ÏÂ²Ö¿â¸Ä±ä¼ÇÂ¼
		TryClearTBChangeCounter();

		//Ð´ÈëÊý¾Ý³É¹¦
		//·¢ËÍÍ¬Òâ½»Ò×µÄÊý¾Ý
		GMSV::SendConsignRequest(pPlayer->ID.id, type, item_id, item_idx, item_cnt, item_type, money, sold_time, price, margin, sellto_name, sellto_name_len);
		GLog::log(GLOG_INFO,"Íæ¼Ò¼ÄÊÛ¿ªÊ¼ ÓÃ»§%d,  ÀàÐÍ%d, ±£Ö¤½ð%d, ÎïÆ·id:%d, ÊýÁ¿%d, °ü¹üÖÐÎ»ÖÃ%d, ÎïÆ·¼ÄÊÛÀàÐÍ%d, ¼ÄÊÛ½ðÇ®%d, ¼ÄÊÛ¼Û¸ñ%d(RMBÔª), ¼ÄÊÛÊ±¼ä%d(Ìì)", pPlayer->ID.id, type, margin, item_id, item_cnt, item_idx, item_type, money, price, sold_time);

		//½øÈë½»Ò××´Ì¬
		_pstate.Trade();
	}
	else
	{
		//»Øµ½·Ç½»Ò××´Ì¬
		FromConsignToNormal();
	}
}

void gplayer_imp::WaitingConsignReadBack(const GDB::itemlist * pInv,int money)
{
	if(!_pstate.IsWaitTradeReadState()) return;
	if(pInv)
	{
		GLog::log(GLOG_INFO,"ÓÃ»§%d¼ÄÊÛ³É¹¦£¬¼ÄÊÛ½ðÇ®¸Ä±äÎª%d",_parent->ID.id, money - GetMoney());
		//¶ÁÈ¡Êý¾Ý³É¹¦
		//ÖØÐÂÕûÀí×Ô¼ºµÄÎïÆ·À¸
		_inventory.Clear();
		_inventory.InitFromDBData(*pInv);
		if(((size_t)money) > _money_capacity)
		{
			GLog::log(GLOG_ERR,"ÓÃ»§%d¼ÄÊÛ¼ÆÊý½ðÇ®³¬¹ý×î´óÈÝÁ¿",_parent->ID.id);
			money = _money_capacity;
		}
		_player_money = money;

		//»Øµ½Í¨³£×´Ì¬
		FromConsignToNormal();
	}
	else
	{
		GLog::log(GLOG_INFO,"ÓÃ»§%d¼ÄÊÛÍê³ÉºóÈ¡Êý¾ÝÊ§°Ü",_parent->ID.id);
		//¶ÁÈ¡Êý¾ÝÊ§°Ü Ö±½ÓÏÂÏßÊÇ¸öºÃÖ÷Òâ
		//²¢ÇÒ²»½øÐÐÐ´ÅÌ²Ù×÷
		FromConsignToNormal(-1);
	}
}

void gplayer_imp::FromConsignToNormal(int type)
{
	if(type < 0)
	{
		delete _consign_obj;
		_consign_obj = NULL;
		_pstate.Normalize();

		//È»ºó½øÐÐÏÂÏß²Ù×÷£¬²»½øÐÐ´æÅÌ
		_offline_type = PLAYER_OFF_LPG_DISCONNECT;
		Logout(-1);
		return;
	}

	//¸æËßÍæ¼Òµ±Ç°µÄ°ü¹üÀ¸ÄÚÈÝ
	PlayerGetInventoryDetail(0);
	_runner->get_player_money(GetMoney(),_money_capacity);

	//È»ºóÉ¾³ý¶ÔÏó£¬»Øµ½½»Ò××´Ì¬
	delete _consign_obj;
	_consign_obj = NULL;

	//È»ºó»Øµ½Í¨ÓÃ×´Ì¬ ²¢ÇÒÍ¨ÖªplayerÏÖÔÚ´¦ÓÚ·Ç½»Ò××´Ì¬
	//ËÀÍö×´Ì¬ÒÑ¾­¶ÀÁ¢³öÀ´ÁË
	_pstate.Normalize();
	_write_timer = 512;

	if(_parent->b_disconnect)
	{
		//Íæ¼ÒÒÑ¾­¶ÏÏß£¬Ö´ÐÐ¶ÏÏßÂß¼­  ²¢ÇÒ´æÅÌ
		//½øÈë¶ÏÏßÂß¼­
		_pstate.Disconnect();
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
}

//½ÇÉ«¼ÄÊÛ
int gplayer_imp::CanConsignRole(char type, int item_idx, int sold_time, int price, int margin)
{
	if(!_pstate.CanTrade()) return -1;
	if(_cur_session || _session_list.size()) return -2;
	if(4 != type) return -3; //±¾º¯ÊýÖ»ÄÜÓÃÓÚ¼ÄÊÛ½ÇÉ«

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_CONSIGN_IN_COMBAT);
		return -1;
	}
	
	//ÊÛ¼Û·¶Î§¼ì²â£¬²»ÄÜµÍÓÚ60£¬²»ÄÜ¸ßÓÚ100w, µ¥Î»RMBÔª
	if(price < 60 || price > 1000000) return -4;
	if(sold_time < 1 || sold_time > 7) return -5;
	if(margin < 0 || margin > 2000000000) return -6;

	//¼ì²éµÈ¼¶
	if(GetRebornCount() <= 0 || GetObjectLevel() < CONSIGN_ROLE_REQUIRED_LEVEL)
	{
		_runner->error_message(S2C::ERR_LEVEL_NOT_MATCH);
		return -14;
	}

	//¼ì²éÍæ¼ÒÊÇ·ñ´¦ÓÚºìÃû×´Ì¬
	if(GetPKValue() > 0)
	{
		_runner->error_message(S2C::ERR_CONSIGN_ROLE_REDNAME);
		return -15;
	}

	//¼ì²éÍæ¼ÒÊÇ·ñÓÐÕÙ»½ÊÞ£¬»³¹âµÄ·ÖÉí»á¼ÓÑªÁ¿
	if(GetSummonMan().HasSummonNPC())
	{
		_runner->error_message(S2C::ERR_HAS_SUMMON_CAN_NOT_CONSIGNROLE);
		return -19;
	}

	size_t inv_size = _inventory.Size();
	if(item_idx < 0 || (size_t)item_idx >= inv_size) return -16;

	if(_inventory[item_idx].type != g_config.consign_role_item_id) return -17;
	
	PlayerTaskInterface task_if(this);
	if(!task_if.CheckSpecialTasksForConsign()) return -18;

	return 0;
}

void gplayer_imp::StartConsignRole(char type, int item_id, int item_idx, int item_cnt, int item_type, int sold_time, int price, int margin, char* sellto_name, char sellto_name_len)
{
	ASSERT(_pstate.CanTrade() && !_cur_session);

	gplayer* pPlayer = (gplayer*)_parent;
	GMSV::SendConsignStartRole(pPlayer->ID.id, type, item_id, item_idx, item_cnt, item_type, sold_time, price, margin, sellto_name, sellto_name_len);
	SetConsignRoleFlag(true);

	GLog::log(GLOG_INFO,"Íæ¼Ò¼ÄÊÛ½ÇÉ«¿ªÊ¼ ÓÃ»§%d, ÀàÐÍ%d, ±£Ö¤½ð%d, ¿Û³ýÎïÆ·id:%d, ÊýÁ¿%d, °ü¹üÖÐÎ»ÖÃ%d, ¼ÄÊÛ¼Û¸ñ%d(RMBÔª), ¼ÄÊÛÊ±¼ä%d(Ìì)", pPlayer->ID.id, type, margin, item_id, item_cnt, item_idx, price, sold_time);
}

bool gplayer_imp::CheckToSendConsignRoleInfo(int result)
{
	if(!IsConsignRole())
	{
		return false;
	}

	if(CONSIGN_ROLE_SUCCESS == result)
	{
		//Çå³ýËùÓÐbuff
		_filters.ClearSpecFilter(0xFFFFFFFF);
		OI_UpdateAllProp();

		GMSV::role_basic_attribute basic_attr;
		GMSV::role_deity_info deity_info;
		raw_wrapper title_rp;
		raw_wrapper skill_rp;
		std::vector<GMSV::role_pet_prop_added> pet_prop_added_list;

		GetGsRoleInfoForConsign(basic_attr, deity_info, title_rp, skill_rp, pet_prop_added_list);
		GMSV::role_title_info title_info = {title_rp.size(), title_rp.data()};
		GMSV::role_skill_info skill_info = {skill_rp.size(), skill_rp.data()};
		GMSV::SendPlayerConsignOfflineRe(GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid, result, basic_attr, deity_info, title_info, skill_info, pet_prop_added_list);
	}
	else
	{
		GMSV::SendPlayerConsignFailedOfflineRe(GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid, result);
	}

	//È¥µôÉíÉÏµÄ±êÖ¾
	SetConsignRoleFlag(false);
	//Í¨Öª¿Í»§¶ËÍæ¼ÒÏÂÏß
	_runner->error_message(S2C::ERR_CONSIGN_ROLE_LOGOUT);
	GLog::log(GLOG_INFO, "Íæ¼Ò½ÇÉ«¼ÄÊÛoffline_re·¢ËÍ£¬result=%d", result);

	return true;
}

