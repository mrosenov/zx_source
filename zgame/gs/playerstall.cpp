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
#include "userlogin.h"
#include "playertemplate.h"
#include <common/protocol_imp.h>
#include "playerstall.h"
#include "serviceprovider.h"
#include "task/taskman.h"

bool 
gplayer_imp::PlayerOpenPersonalMarket(int index, int item_id, size_t count, const char name[28], int * entry_list)
{
	if(GetWorldManager()->GetWorldLimit().nomarket) return false;
	if(_parent->IsZombie() || !_pstate.IsNormalState()) return false;
	if(!CanSitDown()) return false;
	if(_cur_session || _session_list.size()) return false;
	if(count > PLAYER_MARKET_SLOT_CAP || count == 0)return false;

	//°ÚÌ¯µÀ¾ß£¬ÓÃÓÚ¸Ä±ä°ÚÌ¯µÄĞÎÏó,¿Í»§¶ËÊ¹ÓÃ
	int market_item_id = -1;
	int market_item_index = -1;
	//ÊÇ·ñÊ¹ÓÃµÀ¾ß°ÚÌ¯
	if( (index >= 0 && index <  (int)_inventory.Size()) && -1 != item_id)
	{
		//¼ì²éÎïÆ·ÀàĞÍÊÇ·ñÊÇ°ÚÌ¯ÓÃÎïÆ·
		int tmp_item_id = _inventory[index].type;
		if(tmp_item_id <= 0 || item_id != tmp_item_id) return false;
		DATA_TYPE dt2;
		const BOOTH_FIGURE_ITEM_ESSENCE &ess= *(const BOOTH_FIGURE_ITEM_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(tmp_item_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_BOOTH_FIGURE_ITEM_ESSENCE || &ess == NULL)
		{
			return false;
		}

		market_item_index = index;
		market_item_id    = item_id;

	}

	packet_wrapper h1(1024);
	using namespace S2C;
	CMD::Make<CMD::self_open_market>::From(h1, market_item_index, market_item_id, count);

	//¶¼Í¨¹ıÁË
	//¼ì²éÎïÆ·ÊÇ·ñºÏºõÕıÈ·
	abase::vector<char> flag_list;
	flag_list.insert(flag_list.begin(),_inventory.Size(),0);
	
	int order_count = 0;
	C2S::CMD::open_personal_market::entry_t * ent = (C2S::CMD::open_personal_market::entry_t *)entry_list;
	for(size_t i = 0; i < count ; i ++)
	{
		if( ent[i].price == 0 || ent[i].price > MONEY_CAPACITY_BASE || 
				(ent[i].index != 0xFFFF && !_inventory.IsItemExist(ent[i].index,ent[i].type,ent[i].count)))
		{
			//Í¨ÖªÒ»ÏÂÎïÆ·²»ÕıÈ·
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return false;
		}
		if(ent[i].index == 0xFFFF)
		{
			item_data * pData=(item_data*)gmatrix::GetDataMan().get_item_for_sell(ent[i].type);
			if(pData == NULL || ent[i].count > pData->pile_limit)
			{
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}
			//¿¼ÂÇ¼ÇÂ¼¹ºÂòµÄÊıÄ¿£¬Í³¼Æ¿ÕÏĞµÄ¿Õ¼ä
			order_count ++;
		}
		else
		{
			if(!_inventory[ent[i].index].CanTrade())
			{
				//½ûÖ¹½»Ò×µÄÎïÆ·²»ÄÜ½»Ò×
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}

			
			if(flag_list[ent[i].index]++)
			{	
				//²»ÔÊĞíÒ»¼şÎïÆ·Âô³öÁ½´Î
				_runner->error_message(S2C::ERR_INVALID_ITEM);
				return false;
			}
		}
	}
	
	//¿¼ÂÇÏŞÖÆ°ÚÌ¯µÄËÙ¶È
	//Õâ¸öÊÇºÜÓĞÓÃµÄ
	int sys_time = g_timer.get_systime();
	if(sys_time == _stall_trade_timer) return false;
	_stall_trade_timer = sys_time;

	//ÎïÆ·ÕıÈ·¿ªÆô°ÚÌ¯£¬½øÈë°ÚÌ¯×´Ì¬£¬½¨Á¢°ÚÌ¯µÄ¶ÔÏó
	ASSERT(!_stall_obj);

	GLog::log(GLOG_INFO,"ÓÃ»§%d¿ªÊ¼°ÚÌ¯",_parent->ID.id);

	_stall_obj = new player_stall(_inventory);
	_stall_obj->SetMarketName(name);
	for(size_t i = 0; i < count ; i ++)
	{
		CMD::Make<CMD::self_open_market>::AddGoods(h1,ent[i].type,ent[i].index,ent[i].count,ent[i].price);
		if(ent[i].index == 0xFFFF)
			_stall_obj->AddOrderGoods(ent[i].index,ent[i].type,ent[i].count,ent[i].price);
		else
		{
			_stall_obj->AddTradeGoods(ent[i].index,ent[i].type,ent[i].count,ent[i].price);
			GLog::log(GLOG_INFO,"ÓÃ»§%dÌí¼Ó°ÚÌ¯»õÎïid=%d,index=%d,count=%d,price=%d",_parent->ID.id, ent[i].type, ent[i].index, ent[i].count, ent[i].price);
		}
	}


	_pstate.StartMarket();
	_stall_trade_id ++;
	if((_stall_trade_id & 0xFF) == 0) _stall_trade_id ++;
	//ĞŞ¸Ä×Ô¼ºµÄ×´Ì¬
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->market_id = _stall_trade_id & 0xFF;
	pPlayer->object_state |= gactive_object::STATE_MARKET; 
	pPlayer->market_item_id = market_item_id;

	//·¢³ö¸ø×Ô¼ºµÄÏûÏ¢
	send_ls_msg(pPlayer,h1);
		
	//·¢³ö¹ã²¥ÏûÏ¢
	_runner->begin_personal_market(market_item_id, _stall_trade_id,_stall_obj->GetName(),_stall_obj->GetNameLen());
	return true;
}

bool 
gplayer_imp::PlayerTestPersonalMarket(int index, int item_id)
{
	if(GetWorldManager()->GetWorldLimit().nomarket) return false;
	if(_parent->IsZombie() || !_pstate.IsNormalState()) return false;
	if(_cur_session || _session_list.size()) return false;
	if(!CanSitDown()) return false;

	//ÊÇ·ñÊ¹ÓÃµÀ¾ß°ÚÌ¯
	if( (index >= 0 && index <  (int)_inventory.Size()) && -1 != item_id)
	{
		//¼ì²éÎïÆ·ÀàĞÍÊÇ·ñÊÇ°ÚÌ¯ÓÃÎïÆ·
		int rt_id = _inventory[index].type;
		if(rt_id <= 0 || item_id != rt_id) return false;
		DATA_TYPE dt2;
		const BOOTH_FIGURE_ITEM_ESSENCE &ess= *(const BOOTH_FIGURE_ITEM_ESSENCE *)gmatrix::GetDataMan().get_data_ptr(rt_id, ID_SPACE_ESSENCE,dt2); 
		if(dt2 != DT_BOOTH_FIGURE_ITEM_ESSENCE || &ess == NULL)
		{
			return false;
		}

		_runner->personal_market_available(index, item_id);
	}
	else
	{
		_runner->personal_market_available(-1, -1);
	}

	return true;
}

bool 
gplayer_imp::CancelPersonalMarket()
{
	//Ê×ÏÈÍ¨ÖªËùÓĞµÄ¿Í»§£¬È¡Ïû·şÎñÁË
	//ÏÖÔÚ²»½øĞĞÁË£¬ÒòÎª²»ĞèÒªÎ¬»¤¿Í»§ĞÅÏ¢ÁË


	GLog::log(GLOG_INFO,"ÓÃ»§%dÍ£Ö¹°ÚÌ¯",_parent->ID.id);

	//Çå³ıÊĞ³¡¶ÔÏó 
	delete _stall_obj;
	_stall_obj = NULL;
	
	//È»ºóĞŞ¸Ä×´Ì¬»ØÀ´
	_pstate.Normalize();
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->object_state &= ~(gactive_object::STATE_MARKET); 
	pPlayer->market_item_id = -1;

	//¹ã²¥
	_runner->cancel_personal_market();

	//°ÚÌ¯½áÊøºóÖØĞÂ¼ì²éÒ»ÏÂÈÎÎñ£¬·ÀÖ¹ÈÎÎñ±»¿¨
	//ÕâÀï²»ÄÜÖ±½Óµ÷ÓÃOnTaskCheckState, ÒòÎªÈç¹ûÊÕµ½¿Í»§¶ËµÄTASK_NOTIFYÃüÁîºó
	//ÔÚOnTaskCheckAwardDirectÀïÃæÈç¹û¿ÉÒÔÖ±½ÓÇå³ı·Ç·¨×´Ì¬µÄ»°, »áÈ¡Ïû°ÚÌ¯×´Ì¬£ Õâ¸öÊ±ºòÈç¹ûÖ±½Óµ÷ÓÃOnTaskCheckState
	//»áµ¼ÖÂÄÚ´æ´íÎó
	MSG msg;
	BuildMessage(msg,GM_MSG_TASK_CHECK_STATE,_parent->ID,_parent->ID,A3DVECTOR(0,0,0),0);
	gmatrix::SendMessage(msg);
	return true;
}

int 
gplayer_imp::DoPlayerMarketTrade(const XID & trader,const XID & buyer,gplayer * pTrader, gplayer *pBuyer, const void *order, size_t length)
{
	//µÚÒ»²½£º¼ì²éÈËÎïÊÇ·ñ¶ÔÓ¦ 
	if(!pTrader->IsActived() || pTrader->ID != trader || pTrader->imp == NULL || pTrader->IsZombie())
	{
		return -1;
	}
	
	if(!pBuyer->IsActived()  || pBuyer->ID != buyer || pBuyer->imp == NULL || pBuyer->IsZombie())
	{
		return 1;
	}

	//µÚ¶ş²½,¼ì²é½»Ò×ÄÚÈİÊÇ·ñºÏ·¨
	player_stall::trade_request & req = *(player_stall::trade_request *)order;
	if(length < sizeof(req) || length != sizeof(req) + req.count*sizeof(player_stall::trade_request::entry_t))
	{
		return -2;
	}
	
	size_t need_money = 0;
	//²ì¿´ÎïÆ·ÊÇ·ñ´æÔÚ Ç®ÊıÊÇ·ñ»áÒç³ö
	if(!((gplayer_imp*)(pTrader->imp))->CheckMarketTradeRequest(req,need_money))
	{
		return -3;
	}

	//²ì¿´°ü¹üÀ¸ÊÇ·ñÓĞ×ã¹»µÄÊ£Óà¿Õ¼ä,½ğÇ®ÊıÄ¿ÊÇ·ñ×ã¹»
	if(!((gplayer_imp*)(pBuyer->imp))->CheckMarketTradeRequire(req,need_money))
	{
		return -4;
	}

	//ËùÓĞÌõ¼şÍ¨¹ı,¿ªÊ¼¼ÓÈëÊı¾İ
	((gplayer_imp*)(pTrader->imp))->DoPlayerMarketTrade(req,(gplayer_imp*)(pBuyer->imp),need_money);

	// ¸üĞÂ»îÔ¾¶È -- °ÚÌ¯²¢Âô³öÎïÆ·
	((gplayer_imp*)(pTrader->imp))->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_STALL_AND_SELL);

	return 0;

}

int 
gplayer_imp::DoPlayerMarketPurchase(const XID & trader,const XID & buyer,gplayer * pTrader, gplayer *pBuyer, const void *order, size_t length)
{
	//µÚÒ»²½£º¼ì²éÈËÎïÊÇ·ñ¶ÔÓ¦ 
	if(!pTrader->IsActived() || pTrader->ID != trader || pTrader->imp == NULL || pTrader->IsZombie())
	{
		return -1;
	}
	
	if(!pBuyer->IsActived()  || pBuyer->ID != buyer || pBuyer->imp == NULL || pBuyer->IsZombie())
	{
		return 1;
	}

	//µÚ¶ş²½,¼ì²é½»Ò×ÄÚÈİÊÇ·ñºÏ·¨
	player_stall::trade_request & req = *(player_stall::trade_request *)order;
	if(length < sizeof(req) || length != sizeof(req) + req.count*sizeof(player_stall::trade_request::entry_t))
	{
		return -2;
	}
	
	size_t total_price = 0;
	//½»ÑéÊÕ¹ºÇëÇóÊÇ·ñÕıÈ·£¬¼ÆËãÊÕ¹ºµÄ¼Û¸ñÊÇ·ñ¹ı´ó ¼ÆËãÊÇ·ñÓĞ×ã¹»µÄ¿Õ¼ä±£´æÎïÆ·
	if(!((gplayer_imp*)(pTrader->imp))->CheckMarketPurchaseRequest(req,total_price))
	{
		return -3;
	}

	//²ì¿´¹ºÂòÕßÉíÉÏÊÇ·ñÓĞ¶ÔÓ¦µÄÎïÆ·´æÔÚ
	if(!((gplayer_imp*)(pBuyer->imp))->CheckMarketPurchaseRequire(req,total_price))
	{
		return -4;
	}

	//ËùÓĞÌõ¼şÍ¨¹ı,¿ªÊ¼¼ÓÈëÊı¾İ
	((gplayer_imp*)(pTrader->imp))->DoPlayerMarketPurchase(req,(gplayer_imp*)(pBuyer->imp),total_price);

	return 0;

}

int 
gplayer_imp::MarketHandler(const MSG & msg)
{
	switch(msg.message)
	{       

		case GM_MSG_QUERY_MARKET_NAME:
			_runner->send_market_name(msg.source,*(int*)(msg.content), msg.param,_stall_obj->GetName(),_stall_obj->GetNameLen());
			break;

		case GM_MSG_SERVICE_HELLO:
		{
			if(msg.pos.squared_distance(_parent->pos) < 36.f)	//ÁùÃ×ÏŞÖÆ
			{
				SendTo<0>(GM_MSG_SERVICE_GREETING,msg.source,0);
			}
		}
		return 0;

		case GM_MSG_SERVICE_REQUEST:
		if(msg.pos.squared_distance(_parent->pos) > 36.f)	//6Ã×ÏŞÖÆ
		{
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		//¶Ô·şÎñµÄÇëÇóµ½À´(ÒªÇó·şÎñ)
		if(msg.param == service_ns::SERVICE_ID_PLAYER_MARKET
				|| msg.param == service_ns::SERVICE_ID_PLAYER_MARKET2)
		{
			//·şÎñIDÕıÈ·,È»ºó´¦Àíµ½À´µÄÍæ¼ÒÇëÇó
			int index1 = 0;
			gplayer *pParent = GetParent();
			XID self = pParent->ID;
			gplayer *pPlayer1 = gmatrix::FindPlayer(msg.source.id,index1);
			if(!pPlayer1  || pPlayer1 == pParent || pPlayer1->tag != GetWorldTag())
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
				return 0;
			}

			//Ê×ÏÈÊÔÍ¼Ëø¶¨Ò»ÏÂÍæ¼Ò 
			if(pPlayer1 < pParent)
			{
				if(mutex_spinset(&pPlayer1->spinlock) != 0)
				{
					//ÊÔÍ¼ËøÊ§°Ü
					//ÖØĞÂ½øĞĞ¼ÓËø²Ù×÷
					mutex_spinunlock(&pParent->spinlock);
					mutex_spinlock(&pPlayer1->spinlock);
					mutex_spinlock(&pParent->spinlock);
				}
			}
			else
			{
				//¿ÉÒÔÖ±½ÓËø¶¨
				mutex_spinlock(&pPlayer1->spinlock);
			}

			//Ö±½Óµ÷ÓÃÖ´ĞĞ½»Ò×µÄº¯Êı
			if(msg.param == service_ns::SERVICE_ID_PLAYER_MARKET)
			{
				//Íæ¼Ò¹ºÂòÉÌµêÎïÆ·
				if(DoPlayerMarketTrade(self,msg.source, pParent,pPlayer1,msg.content,msg.content_length)<0)
				{
					//·¢ËÍ´íÎóÊı¾İ ²»Ê¹ÓÃ±¾ÉíµÄSendToÊÇÒòÎªÓĞ¸ÅÂÊ×ÔÉíÒÑ¾­±»ÊÍ·Å
					MSG msg2;
					BuildMessage(msg2,GM_MSG_ERROR_MESSAGE,msg.source,self,A3DVECTOR(0,0,0),S2C::ERR_SERVICE_UNAVILABLE);
					gmatrix::SendMessage(msg2);
				}
			}
			else
			{
				//Íæ¼ÒÏòÉÌµêÂô³öÎïÆ·
				if(DoPlayerMarketPurchase(self,msg.source, pParent,pPlayer1,msg.content,msg.content_length)<0)
				{
					//·¢ËÍ´íÎóÊı¾İ ²»Ê¹ÓÃ±¾ÉíµÄSendToÊÇÒòÎªÓĞ¸ÅÂÊ×ÔÉíÒÑ¾­±»ÊÍ·Å
					MSG msg2;
					BuildMessage(msg2,GM_MSG_ERROR_MESSAGE,msg.source,self,A3DVECTOR(0,0,0),S2C::ERR_SERVICE_UNAVILABLE);
					gmatrix::SendMessage(msg2);
				}
			}

			//½âËø¹Ë¿Í 
			mutex_spinunlock(&pPlayer1->spinlock);
		}
		else
		{
			//±¨¸æ´íÎó
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		return 0;

		case GM_MSG_SERVICE_QUIERY_CONTENT:
		if(msg.pos.squared_distance(_parent->pos) > 36.f)	//6Ã×ÏŞÖÆ
		{
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			return 0;
		}
		if(msg.content_length == sizeof(int) * 2)
		{
			int cs_index = *(int*)msg.content;
			int sid = *((int*)msg.content + 1);
			//·¢ËÍµ±Ç°µÄ×°±¸Êı¾İ¸øÍæ¼Ò
			ASSERT(_stall_obj);
			if(!_stall_obj) return 0;
			packet_wrapper  h1(2048);
			using namespace S2C;
			size_t count = _stall_obj->_goods_list.size();
			CMD::Make<CMD::player_market_info>::From(h1,_parent,_stall_trade_id,count);
			for(size_t i =0; i < count ;i ++)
			{
				const player_stall::entry_t & ent = _stall_obj->_goods_list[i];
				if(ent.count)
				{
					if(ent.index == 0xFFFF)
					{
						//ÂòÈëÎïÆ·
						CMD::Make<INFO::market_goods>::BuyItem(h1,ent.type,ent.count,ent.price);
						continue;
					}
					
					size_t index = ent.index;
					item_data_client data;
					if(_inventory.GetItemDataForClient(index,data) > 0 && ent.type == data.type 
							&& data.expire_date == ent.expire_date
							&& _inventory[index].GetCRC() == ent.crc
							)
					{
						CMD::Make<INFO::market_goods>::SellItem(h1,ent.type,ent.count,ent.price,data, item::Proctype2State(data.proc_type));
						continue;
					}
				}
				CMD::Make<INFO::market_goods>::From(h1);	//·ÅÖÃÒ»¸ö¿ÕÎïÆ·
			}
			send_ls_msg(cs_index, msg.source.id, sid,h1.data(),h1.size());

		}
		else
		{
			ASSERT(false);
		}
		return 0;
	}
	return 0;
}

bool 
gplayer_imp::CheckMarketTradeRequest(player_stall::trade_request & req, size_t &need_money)
{
	//¼ì²éÊÇ·ñÄÜ¹»½øĞĞ½»Ò×
	if(!_pstate.IsMarketState()) return false;
	if(!_stall_obj) { ASSERT(false); return false;}
	if(_stall_trade_id != req.trade_id) return false;
	size_t m = 0;
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_stall_obj->_goods_list.size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		size_t index = req.list[i].index;
		if(index >= _stall_obj->_goods_list.size()) return false;
		const player_stall::entry_t & ent = _stall_obj->_goods_list[index];
		size_t item_count = req.list[i].count;
		if(item_count == 0) return false;
		if(ent.index == 0xFFFF) return false; //ÕâÊÇ¹ºÂòÌõÄ¿
		if(ent.type != req.list[i].type) return false;
 		if(ent.count < item_count || ent.count < list[index] + item_count)
		{
			//ÂòÈëµÄÊıÄ¿¹ı¶à
			return false;
		}
		list[index] += item_count;

		if(!_inventory.IsItemExist(ent.index,ent.type,ent.count)) return false;
		//ÅĞ¶ÏÊÇ·ñ²»ÔÊĞí½»Ò×
		if(!_inventory[ent.index].CanTrade()) return false;
		if(ent.crc != _inventory[ent.index].GetCRC()) return false;
		if(ent.expire_date != _inventory[ent.index].expire_date) return false;

		//½øĞĞ¼Û¸ñ¼ÆËã
		size_t p = ent.price * item_count;
		if(p/item_count != ent.price) return false;
		size_t tmp =  m + p;
		if(tmp < m) return false;
		m = tmp;
	}
	
	size_t m_total =  m + GetMoney();
	if(m_total < GetMoney() || m_total > _money_capacity)
	{
		//Ó¦µ±È¡Ïû½»Ò×
		CancelPersonalMarket();
		return false;
	}
	//ËùÓĞÅĞ¶ÏÌõ¼şÍ¨¹ıÁË
	need_money = m;
	return true;
}

bool 
gplayer_imp::CheckMarketPurchaseRequest(player_stall::trade_request & req, size_t &total_price)
{
	//¼ì²éÊÇ·ñÄÜ¹»½øĞĞ½»Ò×
	if(!_pstate.IsMarketState()) return false;
	if(!_stall_obj) { ASSERT(false); return false;}
	if(_stall_trade_id != req.trade_id) return false;
	size_t m = 0;
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_stall_obj->_goods_list.size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		size_t index = req.list[i].index;
		if(index >= _stall_obj->_goods_list.size()) return false;
		size_t item_count = req.list[i].count;
		const player_stall::entry_t & ent = _stall_obj->_goods_list[index];
		if(item_count == 0) return false;     //ÎïÆ·ÊıÄ¿´íÎó
		if(ent.index != 0xFFFF) return false; //Õâ²»ÊÇ¹ºÂòÌõÄ¿
		if(ent.type != req.list[i].type ) return false; //ÊıÄ¿»òÕßÀàĞÍ²»Æ¥Åä
		if(ent.count < item_count || ent.count < item_count + list[index])
		{
			//³¬¹ıÁËÎïÆ·ÊıÄ¿ÉÏÏŞ
			return false;
		}
		list[index] += item_count;

		//½øĞĞ¼Û¸ñ¼ÆËã
		size_t p = ent.price * item_count;
		if(p/item_count != ent.price) return false;		//µ¥¼Û³¬³öÏµÍ³ÉÏÏŞ
		size_t tmp =  m + p;
		if(tmp < m) return false;				//×Ü¼Û¸ñ³¬½ç
		m = tmp;
	}
	if(GetMoney() < m) 
	{
		//Ã»Ç®ÁË£¬Ó¦¸ÃÈ¡Ïû½»Ò×
		return false;
	}

	//ÅĞ¶ÏÊÇ·ñÓĞ×ã¹»µÄ¿Õ¼ä±£´æÎïÆ·
	if(_inventory.GetEmptySlotCount() < req.count) return false;

	//ËùÓĞÅĞ¶ÏÌõ¼şÍ¨¹ıÁË
	total_price = m;
	return true;
}

bool 
gplayer_imp::CheckMarketPurchaseRequire(player_stall::trade_request & req, size_t total_price)
{
	//¼ì²éÊÇ·ñÄÜ¹»½øĞĞ½»Ò×
	if(!_pstate.CanTrade()) return false;
	if(OI_TestSafeLock()) return false;

	size_t m_total =  total_price + GetMoney();
	if(m_total < GetMoney() || m_total > _money_capacity)
	{
		//½ğÇ®¹ı¶à£¬ÎŞ·¨·ÅÏÂ
		return false;
	}
	
	//ÅĞ¶ÏÎïÆ·ÊÇ·ñ´æÔÚ
	abase::vector<int,abase::fast_alloc<> > list;
	list.insert(list.begin(),_inventory.Size(),0);
	for(size_t i = 0; i < req.count;i ++)
	{
		int type = req.list[i].type;
		size_t count = req.list[i].count;
		size_t inv_index = req.list[i].inv_index;
		if(!_inventory.IsItemExist(inv_index,type,count)) return false;
		//ÅĞ¶ÏÎïÆ·ÊÇ·ñ²»ÔÊĞí½»Ò×
		if(!_inventory[inv_index].CanTrade()) return false;
		if(list[inv_index]) return false;
		list[inv_index] = 1;	//²»ÔÊĞíÖØ¸´ÊÛ³öÎïÆ·
	}

	//Í¨¹ıÁËËùÓĞÅĞ¶¨£¬¿ÉÒÔ¹ºÂòÁË
	return true;
}

bool 
gplayer_imp::CheckMarketTradeRequire(player_stall::trade_request & req, size_t need_money)
{
	//¼ì²éÊÇ·ñÄÜ¹»½øĞĞ½»Ò×
	if(!_pstate.CanTrade()) return false;
	if(OI_TestSafeLock()) return false;
	if(GetMoney() < need_money) return false;	//×Ê½ğ²»×ã
	
	//ÅĞ¶ÏÊÇ·ñÓĞ×ã¹»µÄ¿ÕÎ»
	if(_inventory.GetEmptySlotCount() < req.count) return false;
	return true;
}

//ÕâÊÇÍæ¼ÒÉÌµêÂô³öÎïÆ·
void 
gplayer_imp::DoPlayerMarketTrade(player_stall::trade_request & req, gplayer_imp * pImp, size_t need_money)
{
	item_list & inv = pImp->_inventory;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,pImp->GetParent()->ID.id,need_money,req.count);

	int gold1 = GetMoney();
	int gold2 = pImp->GetMoney();
	//½»Ò×¿ªÊ¼
	//Ê×ÏÈ½øĞĞÎïÆ·µÄ½»»»
	for(size_t i = 0; i < req.count;i ++)
	{
		size_t item_index = req.list[i].index;
		ASSERT(item_index < _stall_obj->_goods_list.size());
		player_stall::entry_t & ent = _stall_obj->_goods_list[item_index];
		size_t item_count = req.list[i].count;
		ASSERT(item_count);
		ASSERT(ent.type == req.list[i].type && ent.count >= item_count);
		ASSERT(_inventory.IsItemExist(ent.index,ent.type,ent.count));
		ASSERT(ent.crc == _inventory[ent.index].GetCRC()); 

		item it;
		if(_inventory[ent.index].count == item_count)
		{
			_inventory.Exchange(ent.index,it);
		}
		else
		{
			//Òª½«ÎïÆ··Ö¿ª
			it = _inventory[ent.index];
			it.content.BuildFrom(_inventory[ent.index].content);
			it.count = item_count;
			_inventory.DecAmount(ent.index,item_count);
		}
		//ĞŞÕıÂô³öµÄÊı¾İ
		ent.count -= item_count;
		_runner->trade_away_item(pImp->GetParent()->ID.id ,ent.index, ent.type, item_count);
		
		int expire_date = it.expire_date;
		int rst = inv.Push(it);
		if(rst < 0) 
		{
			ASSERT(false);
			//²»¿ÉÄÜµÄ, µ«ÊÇÖ»ºÃÌø¹ı,Ëãµ¹Ã¹ÁË contine 
			it.Release();
			continue;
		}
		GLog::log(GLOG_INFO,"ÓÃ»§%dÂô¸øÓÃ»§%d %d¸ö%d£¬index=%d£¬µ¥¼Û%d",_parent->ID.id, pImp->_parent->ID.id, item_count, ent.type, ent.index, ent.price);

		gold1 += ent.price*item_count;
		gold2 -=  ent.price*item_count;
		GLog::action("baitan, huid=%d:hrid=%d:cuid=%d:crid=%d:mode=2:moneytype=1:price=%d:h_lv=%d:c_lv=%d:h_gold=%d:c_gold=%d:hitemid=%d,%d",
							_db_magic_number, _parent->ID.id, pImp->_db_magic_number, pImp->_parent->ID.id,ent.price * item_count,
							GetObjectLevel(),pImp->GetObjectLevel(),gold1,gold2, ent.type, item_count);

		//×éÖ¯Òª·¢¸ø¿Í»§¶ËµÄÊı¾İ
		int state = item::Proctype2State(it.proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,ent.type,expire_date,item_count,rst,state);

	}

	//ĞŞ¸Ä½»Ò×Ë«·½µÄ½ğÇ®ÊıÁ¿
	//ÕâÀïÓ¦¸ÃÒÑ¾­±£Ö¤ÁËÇ®Êı±ØÈ»ÕıÈ·
	ASSERT(need_money + _player_money <= _money_capacity && need_money + _player_money >= _player_money);
	pImp->SpendMoney(need_money);
	GainMoney(need_money);
	_runner->get_player_money(GetMoney(),_money_capacity);
	send_ls_msg(pImp->GetParent(),h1);

	//·¢ËÍ³É¹¦µÄÏûÏ¢
	_runner->market_trade_success(pImp->GetParent()->ID.id);
	pImp->_runner->market_trade_success(GetParent()->ID.id);

	//¼õÉÙË«·½½»Ò×µÄ´æÅÌÊ±¼ä
	pImp->ReduceSaveTimer(100);
	ReduceSaveTimer(100);
}

//Íæ¼ÒÉÌµê¹ºÂò
void 
gplayer_imp::DoPlayerMarketPurchase(player_stall::trade_request & req, gplayer_imp * pImp, size_t total_price)
{
	item_list & inv = pImp->_inventory;

	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,pImp->GetParent()->ID.id,total_price,req.count,false);

	int gold1 = GetMoney();
	int gold2 = pImp->GetMoney();
	//½»Ò×¿ªÊ¼
	//Ê×ÏÈ½øĞĞÎïÆ·µÄ½»»»
	for(size_t i = 0; i < req.count;i ++)
	{

		size_t item_index = req.list[i].inv_index;
		size_t item_count = req.list[i].count;
		size_t stall_index = req.list[i].index;
		ASSERT(item_count);
		ASSERT(inv.IsItemExist(item_index,req.list[i].type,item_count));
		player_stall::entry_t & ent = _stall_obj->_goods_list[stall_index];
		ASSERT(ent.type == req.list[i].type && ent.count >= item_count);

		item it;
		if(inv[item_index].count == item_count)
		{
			inv.Remove(item_index,it);
		}
		else
		{
			//Òª½«ÎïÆ··Ö¿ª
			it = inv[item_index];
			it.content.BuildFrom(inv[item_index].content);
			it.count = item_count;
			inv.DecAmount(item_index,item_count);

		}

		//ĞŞÕıÂô³öµÄÊı¾İ
		ent.count -= item_count;
		pImp->_runner->player_drop_item(IL_INVENTORY,item_index,ent.type,item_count,S2C::DROP_TYPE_TRADEAWAY);
		
		int expire_date = it.expire_date;
		int rst = _inventory.Push(it);
		if(rst < 0) 
		{
			ASSERT(false);
			//²»¿ÉÄÜµÄ, µ«ÊÇÖ»ºÃÌø¹ı,Ëãµ¹Ã¹ÁË contine 
			it.Release();
			continue;
		}
		GLog::log(GLOG_INFO,"ÓÃ»§%dÊÕ¹ºÓÃ»§%d %d¸ö%d£¬µ¥¼Û%d",_parent->ID.id, pImp->_parent->ID.id, item_count, ent.type, ent.price);

		gold1 -= ent.price*item_count;
		gold2 +=  ent.price*item_count;
		GLog::action("baitan, huid=%d:hrid=%d:cuid=%d:crid=%d:mode=1:moneytype=1:price=%d:h_lv=%d:c_lv=%d:h_gold=%d:c_gold=%d:hitemid=%d,%d",
							_db_magic_number, _parent->ID.id, pImp->_db_magic_number, pImp->_parent->ID.id,ent.price * item_count,
							GetObjectLevel(),pImp->GetObjectLevel(),gold1,gold2, ent.type, item_count);


		//×éÖ¯Òª·¢¸ø¿Í»§¶ËµÄÊı¾İ
		int state = item::Proctype2State(it.proc_type);
		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,ent.type,expire_date,item_count,rst,stall_index & 0xFF,state);
	}

	//ĞŞ¸Ä½»Ò×Ë«·½µÄ½ğÇ®ÊıÁ¿
	ASSERT(total_price + pImp->_player_money <= pImp->_money_capacity && total_price + pImp->_player_money >= pImp->_player_money);
	SpendMoney(total_price);
	pImp->GainMoney(total_price);
	send_ls_msg(GetParent(),h1);
	pImp->_runner->get_player_money(pImp->GetMoney(),pImp->_money_capacity);

	//·¢ËÍ³É¹¦µÄÏûÏ¢
	_runner->market_trade_success(pImp->GetParent()->ID.id);
	pImp->_runner->market_trade_success(GetParent()->ID.id);

	//¼õÉÙË«·½½»Ò×µÄ´æÅÌÊ±¼ä
	pImp->ReduceSaveTimer(100);
	ReduceSaveTimer(100);
}

