#include "uniqueauctionmanager.h"
#include "gdeliveryserver.hpp"
#include "gamedbclient.hpp"
#include "uniquebidhistory_re.hpp"
#include "localmacro.h"
#include "dbuniqueauctionsave.hrp"
#include "dbuniqueauctionload.hrp"
#include "maplinkserver.h"
#include "mapuser.h"
#include "chatbroadcast.hpp"

namespace GNET
{
void UniqueAuctionManager::BidBeginAnnounce(int winitem)
{
	LOG_TRACE("Unique Auction Start Announce");
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_UNIQUEAUCTION_BEGIN;
	Marshal::OctetsStream msg;
	msg<<winitem;
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

void UniqueAuctionManager::BidStartAnnounce(int winitem)
{
	LOG_TRACE("Unique Auction Start2 Announce");
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_UNIQUEAUCTION_START;
	Marshal::OctetsStream msg;
	msg<<winitem;
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

void UniqueAuctionManager::BidLastAnnounce()
{
	LOG_TRACE("Unique Auction Last Announce");
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_UNIQUEAUCTION_LAST;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

void UniqueAuctionManager::BidEndAnnounce(int roleid, Octets name, int price, int itemid)
{
	LOG_TRACE("Unique Auction End Announce");
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_UNIQUEAUCTION_END;
	Marshal::OctetsStream msg;
	msg<<roleid<<name<<price<<itemid;
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

bool UniqueAuctionManager::SendBidHistory(int roleid, unsigned int sid, unsigned int localsid)
{
	if (!(status & ST_DATAREADY))
		return false;
	UniqueBidHistory_Re re;
	re.retcode = ERR_SUCCESS;
	re.status = (status & (ST_BID|ST_ANNOUNCE));
	re.localsid = localsid;
	if (status & ST_BID)
	{
		re.item_tobid = _itemtobid.itemid;
		re.money_min = _itemtobid.bid_min;
	}
	re.winner_list = _winners;
	re.winner = _winner;
	re.win_item = _win_item;
	RoleMap::iterator it = role_map.find(roleid);
	if (it != role_map.end())
	{
		std::map<int, int>::const_iterator bit, bite = it->second.end();
		for (bit = it->second.begin(); bit != bite; ++bit)
		{
			UniquePriceInfo priceinfo(bit->first);
			if (!(status & ST_BID) && _winner!=0)
			{
				PriceMap::const_iterator pit = price_map.find(bit->first);
				if (pit != price_map.end())
				{
					std::vector<UniqueBidder>::const_iterator bit2, bite2 = pit->second.end();
					for (bit2 = pit->second.begin(); bit2 != bite2 && priceinfo.same_bidders.size() < SAME_BIDDER_MAX; ++bit2)
					{
						if (bit2->roleid != roleid)
							priceinfo.same_bidders.push_back(*bit2);
					}
				}
			}
			re.self_bid_list.push_back(priceinfo);
		}
	}
	GDeliveryServer::GetInstance()->Send(sid, re);
	return true;
}

bool UniqueAuctionManager::ReadLevelConfig(Conf * conf, std::string level_identification, int level)
{
	std::string level_str = conf->find(level_identification.c_str(), "itemlist");
	if (level_str.length() <= 0 || level_str.length() > 1023)
	{
		Log::log(LOG_ERR, "Invalid:%s size:%d", level_identification.c_str(), level_str.length());
		return false;
	}
	char level_buffer[1024];
	memset(level_buffer, 0, sizeof(level_buffer));
	strncpy(level_buffer, level_str.c_str(), std::min(sizeof(level_buffer)-1, level_str.length()));
	char * cur = level_buffer;
	char * token = strchr(cur, '(' );
	while (NULL != token)
	{
		cur = token + 1;
		token = strchr( cur, ',');
		if (NULL == token) 	break;
		*token = 0;
		int itemid = atol(cur);
		if (itemid <= 0)
		{
			Log::log(LOG_ERR, "UniqueAuction %s, invalid itemid %d", level_identification.c_str(), itemid);
			return false;
		}
		cur = token+1;
		token = strchr(cur, ')');
		if (NULL == token) break;
		*token = 0;
		int lowprice = atol(cur);
		if (lowprice <= 0)
		{
			Log::log(LOG_ERR, "UniqueAuction %s, invalid lowprice %d", level_identification.c_str(), lowprice);
			return false;
		}
		level_items[level].push_back(WinItem(itemid, lowprice));
		cur = token+1;
		token = strchr(cur, '(');
	}
	return true;
}
bool UniqueAuctionManager::LoadConfig()
{
/*
uniqueauction.conf
######################################################
[SpecialDayItems]
#(Month-Day,itemid,lowprice)
itemlist = (1-1,2008,1500)(6-1,2009,1800)(12-23,2010,3400)
[FirstClassItems]
#(itemid,lowprice)
itemlist = (3333,2200)(4444,3200)(5555,4800)
[SecondClassItems]
#(itemid,lowprice)
itemlist = (6666,1200)(7777,8200)(8888,9800)
######################################################
*/
	Conf *conf = Conf::GetInstance("uniqueauction.conf");
	int ret = 0;
	std::string special_str = conf->find("SpecialDayItems", "itemlist");
	if( special_str.length() > 1023 )
	{               
		Log::log(LOG_ERR, "UniqueAuction load conf, itemlist is too long");
		Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
		return false;
	}
	char special_buffer[1024];
	memset(special_buffer, 0, sizeof(special_buffer));
	strncpy( special_buffer, special_str.c_str(), std::min(sizeof(special_buffer)-1,special_str.length()) );
	char * cur = special_buffer;
	char * token = strchr( cur, '(' );
	while( NULL != token )
	{       
		cur = token+1;
		token = strchr( cur, ',' );
		if( NULL == token )     break;
		*token = 0;
		int mon, day;
		ret = sscanf(cur, "%d-%d", &mon, &day);
		if (ret != 2)
		{
			Log::log(LOG_ERR, "UniqueAuction load conf, invalid special date");
			Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
			return false;
		}
		cur = token+1;
		token = strchr( cur, ',' );
		if( NULL == token )     break;
		*token = 0;
		int itemid = atol(cur);
		if (itemid <= 0)
		{
			Log::log(LOG_ERR, "UniqueAuction load conf, invalid itemid %d", itemid);
			Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
			return false;
		}
		cur = token+1;
		token = strchr( cur, ')' );
		if( NULL == token )     break;
		*token = 0;
		int lowprice = atol(cur);
		if (lowprice <= 0)
		{
			Log::log(LOG_ERR, "Territory load conf, invalid lowprice %d", lowprice);

			Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
			return false;
		}
//		randawardlist.push_back(RandAwardItem(itemid, itemcount));
		if (special_items.insert(std::make_pair(std::make_pair(mon-1, day), WinItem(itemid, lowprice))).second == false)
		{
			Log::log(LOG_ERR, "Territory load conf, duplicate special date Mon:%d,Day:%d", mon, day);

			Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
			return false;
		}
		cur = token+1;
		token = strchr( cur, '(' );
	}       
	SpecialDayItems::const_iterator it, ite;
	for (it=special_items.begin(),ite=special_items.end(); it!=ite; ++it)
		LOG_TRACE("Mon:%d Day:%d itemid:%d lowprice:%d", it->first.first, it->first.second, it->second.itemid, it->second.bid_min);
	if (!ReadLevelConfig(conf, "FirstClassItems", 0))
	{
		Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
		return false;
	}
	if (!ReadLevelConfig(conf, "SecondClassItems", 1))
	{
		Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
		return false;
	}
	LevelItems::const_iterator lit, lite = level_items.end();
	for (lit=level_items.begin(); lit != lite; ++lit)
	{
		std::vector<WinItem>::const_iterator wit, wite = lit->second.end();
		for (wit=lit->second.begin(); wit!=wite; ++wit)
			LOG_TRACE("Level:%d itemid:%d lowprice:%d", lit->first, wit->itemid, wit->bid_min);
	}
	//加载原始配置文件
	Conf::GetInstance(GDeliveryServer::GetInstance()->conf_file.c_str());
	return true;
}

bool UniqueAuctionManager::Initialize()
{
	if (!LoadConfig())
	{
		LOG_TRACE( "UniqueAuctionManager::Init() load conf failed" );
		return false;
	}
	UpdateTime();
	IntervalTimer::Attach( this,UNIQUE_UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());
	status |= ST_OPEN;
	return true;
}
/*
void UniqueAuctionManager::SaveWinItem(const GUniqueWinItem & today, const GUniqueWinItem & tomorrow)
{
	GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_DBUNIQUESETWINITEM,
					DBUniqueSetWinItem(today, tomorrow)));
}
*/
void UniqueAuctionManager::SaveToDB(GUniqueAuction & data)
{
	data.itemtobid = GUniqueWinItem(_itemtobid.itemid, _itemtobid.bid_min);
	std::vector<UniqueWinner>::const_iterator it, ite = _winners.end();
	for (it = _winners.begin(); it != ite; ++it)
		data.winners.push_back(GUniqueWinner(it->roleid, it->rolename, it->money, it->itemid, it->time));
	data.winner = _winner;
	data.win_item = _win_item;
	PriceMap::const_iterator pit, pite = price_map.end();
	for (pit = price_map.begin(); pit != pite; ++pit)
	{
		GUniqueBidderVector list;
		std::vector<UniqueBidder>::const_iterator bit, bite = pit->second.end();
		for (bit = pit->second.begin(); bit != bite; ++bit)
			list.push_back(GUniqueBidder(bit->roleid, bit->rolename, bit->bidtime));
		data.pricelist.push_back(GUniquePriceInfo(pit->first, list));
	}
}
					
bool UniqueAuctionManager::LoadData(const GUniqueAuction & data)
{
	if((status&ST_DATAREADY))
		Log::log(LOG_ERR,"UniqueAuction data already loaded. status=%d", status);
	else
	{
		_itemtobid = WinItem(data.itemtobid);
		bid_times = 0;
		price_map.clear();
		role_map.clear();
		GUniquePriceInfoVector::const_iterator it, ite = data.pricelist.end();
		for (it = data.pricelist.begin(); it != ite; ++it)
		{
			std::vector<UniqueBidder> bidders;
			GUniqueBidderVector::const_iterator bit, bite = it->bidderlist.end();
			for (bit = it->bidderlist.begin(); bit != bite; ++bit)
			{
				bid_times++;
				bidders.push_back(UniqueBidder(bit->roleid, bit->rolename, bit->bidtime));
				std::map<int, int> &recordlist = role_map[bit->roleid];
				std::map<int, int>::iterator rit, rite = recordlist.end();
				for (rit = recordlist.begin(); rit != rite; ++rit)
				{
					if(bit->bidtime < rit->second)
					      	break;
				}
				recordlist.insert(rit, std::make_pair(it->money, bit->bidtime));
			}
			price_map.insert(std::make_pair(it->money, bidders));
		}
		_winners.clear();
		GUniqueWinnerVector::const_iterator wit, wite = data.winners.end();
		for (wit = data.winners.begin(); wit != wite; ++wit)
			_winners.push_back(UniqueWinner(wit->roleid, wit->rolename, wit->money, wit->itemid, wit->time));
		_winner = data.winner;
		_win_item = data.win_item;
		LOG_TRACE("load UniqueAuction data, bid_times %d", bid_times);
		if (_itemtobid.itemid == 0)
		{
			bool next_day = false;
			time_t now = GetTime();
			if (now >= BidBeginTime()+BEGIN_LAG_TIME)
				next_day = true;
			if (GenerateItem(next_day))
			{
				Log::formatlog("UniqueAuction","generate winitem:%d zoneid %d",  _itemtobid.itemid, GDeliveryServer::GetInstance()->zoneid);
				SetDirty(true);
			}
			else
				Log::log(LOG_ERR, "UniqueAuction generate winitem failed");
		}
/*
		if (changed)
			SaveWinItem(GUniqueWinItem(_today_item.itemid, _today_item.bid_min),
				GUniqueWinItem(_tomorrow_item.itemid, _tomorrow_item.bid_min))
*/
		status |= ST_DATAREADY;
	}
	return true;
}

int UniqueAuctionManager::Bid(int roleid, Octets rolename, int lower_bound_money, int upper_bound_money)
{
	// lower_bound_money == upper_bound_money表示单次出价，否则为范围出价
	if (!(status&ST_BID) || (status&ST_ANNOUNCE))
		return ERR_UNIQUE_BID_TIME;
	
	if (_itemtobid.itemid == 0)
		return -1;

	if(lower_bound_money > upper_bound_money)
	      	return ERR_UNIQUE_BID_MONEY;
	if(lower_bound_money < _itemtobid.bid_min || upper_bound_money > DEFAULT_MAX_PRICE)
	      	return ERR_UNIQUE_BID_MONEY;

	int bidtimes = upper_bound_money - lower_bound_money + 1;
	std::map<int, int> &recordlist = role_map[roleid];
	if(recordlist.size() + bidtimes > BID_MAX_TIMES_PER_ROLE)
	      	return ERR_UNIQUE_BID_TIMES_PERSON;	
	if(bid_times + bidtimes > BID_MAX_TIMES)
	      	return ERR_UNIQUE_BID_TIMES_TOTAL;
	
	std::map<int, int>::const_iterator it = recordlist.lower_bound(lower_bound_money), ie = recordlist.upper_bound(upper_bound_money);
	for(; it != ie; ++it)
	{
		if((it->first >= lower_bound_money) && (it->first <= upper_bound_money))
		      	return ERR_UNIQUE_BID_REPEAT;
	}

	time_t now = GetTime();
	for(int i = lower_bound_money; i <= upper_bound_money; i++)
	{
		recordlist.insert(std::make_pair(i, now));
		price_map[i].push_back(UniqueBidder(roleid, rolename, now));
	}
	bid_times += bidtimes;
	SetDirty(true);
	return ERR_SUCCESS;
}

int UniqueAuctionManager::GetWinItem(int roleid, int & winitem)
{
	if (roleid != _winner)
		return ERR_UNIQUE_GETITEM_NOTWINNER;	
	if (_win_item == 0)
		return ERR_UNIQUE_GETITEM_ITEMID;
	winitem = _win_item;
	Log::formatlog("UniqueAuction","winner %d get win item %d zoneid %d", roleid, winitem, GDeliveryServer::GetInstance()->zoneid);
//	_winner = 0; //不修改_winner
	_win_item = 0;
	SetDirty(true);
	return ERR_SUCCESS;
}

time_t UniqueAuctionManager::GetTime()
{
	if (t_forged)
		return t_base + t_forged;
	else
		return Timer::GetTime();
}

void UniqueAuctionManager::SetForgedTime(time_t forge)
{
	t_forged = forge;
}

void UniqueAuctionManager::BeginBid()
{
	LOG_TRACE("UniqueAuction begin bid status %x bid times %d", status, bid_times);
	if (_winner != 0)
	{
		_winner = 0;
		_win_item = 0;
		price_map.clear();
		role_map.clear();
		bid_times = 0;
		SetDirty(true);
	}
	//_winner 为 0 时不清除出价记录 表明前一天的出价未正常结束 数据保留至今天
}

void UniqueAuctionManager::EndBid()
{
	PriceMap::const_iterator it ,ite = price_map.end(), wit = price_map.end();
	unsigned int min_repeat = BID_MAX_TIMES;
	int min_price = DEFAULT_MAX_PRICE;
	for (it = price_map.begin(); it != ite; ++it)
	{
		if (it->second.size() <= 0)
			continue;
		if (it->second.size() < min_repeat)
		{
			min_repeat = it->second.size();
			min_price = it->first;
			wit = it;
		}
		else if (it->second.size() == min_repeat)
		{
			if (it->first < min_price)
			{
				min_price = it->first;
				wit = it;
			}
		}
	}
	time_t now = GetTime();
	if (wit == price_map.end())
	{
		_winner = 0;
		_win_item = 0;
		_winners.push_back(UniqueWinner(0, Octets(), 0, _itemtobid.itemid, now));
		if (_winners.size() > HISTORY_LIST_SIZE)
			_winners.erase(_winners.begin());
		BidEndAnnounce(0, Octets(), 0, 0);
		Log::formatlog("UniqueAuction","nobody bid zoneid %d", GDeliveryServer::GetInstance()->zoneid);
	}
	else if (min_repeat > 1)
	{
		_winner = RESULT_NO_UNIQUE;
		_win_item = 0;
		_winners.push_back(UniqueWinner(0, Octets(), 0, _itemtobid.itemid, now));
		if (_winners.size() > HISTORY_LIST_SIZE)
			_winners.erase(_winners.begin());
		BidEndAnnounce(RESULT_NO_UNIQUE, Octets(), 0, 0);
		Log::formatlog("UniqueAuction","no unique winner, total bid times %d zoneid %d", bid_times, GDeliveryServer::GetInstance()->zoneid);
	}
	else
	{
		_winner = wit->second.front().roleid;
		_win_item = _itemtobid.itemid;
		_winners.push_back(UniqueWinner(_winner, wit->second.front().rolename, wit->first, _itemtobid.itemid, now));
		if (_winners.size() > HISTORY_LIST_SIZE)
			_winners.erase(_winners.begin());
		BidEndAnnounce(_winner, wit->second.front().rolename, wit->first, _win_item);
		Log::formatlog("UniqueAuction","role %d win item %d with price %d, price_repeat %d total bid times %d zoneid %d", _winner, _itemtobid.itemid, wit->first, wit->second.size(), bid_times, GDeliveryServer::GetInstance()->zoneid);
	}
	LOG_TRACE("UniqueAuction end bid status %x bid times %d winner %d", status, bid_times, _winner);
	if (GenerateItem(true))
		Log::formatlog("UniqueAuction","generate winitem:%d zoneid %d", _itemtobid.itemid, GDeliveryServer::GetInstance()->zoneid);
	else
		Log::log(LOG_ERR, "UniqueAuction generate winitem failed");
	SetDirty(true);
}

time_t UniqueAuctionManager::UpdateTime()
{
	time_t now = GetTime();
	LOG_TRACE("UniqueAuction status %x Timer update: (%d) %s", status, t_forged, ctime(&now));
#ifdef UNIQUE_DEBUG
	if (now-t_base>1800 || now<t_base)
	{
		struct tm dt;
		localtime_r (&now, &dt);
		dt.tm_sec = 0;
		if (dt.tm_min < 30)
			dt.tm_min = 0;
		else
			dt.tm_min = 30;
		t_base = mktime(&dt);
	}
/*十分钟周期
	if (now-t_base>600 || now < t_base)
	{
		struct tm dt;
		localtime_r (&now, &dt);
		dt.tm_sec = 0;
		dt.tm_min = (dt.tm_min/10)*10;
		t_base = mktime(&dt);
	}
*/
#else
	if (now-t_base>86400 || now<t_base)
	{
		struct tm dt;
		localtime_r (&now, &dt);
		dt.tm_sec = 0;
		dt.tm_min = 0;
		dt.tm_hour = 0;
		t_base = mktime(&dt);
	}
#endif
	return now;
}

void UniqueAuctionManager::UpdateStatus(time_t now)
{
	if (now >= BidBeginTime() && now < AnnounceEndTime())
	{
		if (!(status & ST_ANNOUNCE))
		{
			BidBeginAnnounce(_itemtobid.itemid);
			status |= ST_ANNOUNCE;
		}
	}
	else
	{
		if (status & ST_ANNOUNCE)
		{
			BidStartAnnounce(_itemtobid.itemid);
			status &= (~ST_ANNOUNCE);
		}
	}
	if (now >= BidBeginTime() && now < BidEndTime())
	{
		if (now < BidBeginTime()+BEGIN_LAG_TIME && !(status & ST_BID))
		{
			//BidStartAnnounce(_itemtobid.itemid);
			BeginBid();
			status |= ST_BID;
		}
	}
	else
	{
		if (status & ST_BID)
		{
			EndBid();
			status &= (~ST_BID);
		}
	}
}

bool UniqueAuctionManager::Update()
{
	if (!(status & ST_DATAREADY))
		return true;
	time_t now = UpdateTime();
	UpdateStatus(now);
	if ((status&ST_BID) && now > LastAnnounceTime() && now < LastAnnounceTime() + UNIQUE_UPDATE_INTERVAL)
		BidLastAnnounce();
	if (IsDirty())
	{
		DBUniqueAuctionSaveArg arg;
		SaveToDB(arg.data);
		GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_DBUNIQUEAUCTIONSAVE, arg));
		SetDirty(false);
		LOG_TRACE("UniqueAuction SaveDB");
	}
	return true;
}

void UniqueAuctionManager::OnSaveDB(int retcode)
{
	if (retcode != ERR_SUCCESS)
	{
		Log::log(LOG_ERR, "Save UniqueAuction data errorno %d", retcode);
		SetDirty(true);
	}
}

bool UniqueAuctionManager::OnDBConnect(Protocol::Manager *manager, int sid)
{
	if((status&ST_OPEN) && !(status&ST_DATAREADY))
	{
		DBUniqueAuctionLoadArg arg;
		manager->Send(sid, Rpc::Call(RPC_DBUNIQUEAUCTIONLOAD, arg));
	}
	return true;
}

};

