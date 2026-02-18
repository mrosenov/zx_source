#ifndef __GNET_UNIQUEAUCTIONMANAGER_H
#define __GNET_UNIQUEAUCTIONMANAGER_H

#include "guniquebidder"
#include "guniquewinitem"
#include "uniquewinner"
#include "guniqueauction"
#include "uniquebidder"
//测试
//#define UNIQUE_DEBUG
namespace GNET
{
	class BidRecord
	{
	public:
		int money;
		int bidtime;
		BidRecord(int _money = 0, int _bid_time = 0) : money(_money), bidtime(_bid_time) {}
	};
	class WinItem //拍卖物品
	{
	public:
		int itemid;
		int bid_min; //起拍价
		WinItem(int _itemid=0, int _bid_min=0) : itemid(_itemid), bid_min(_bid_min) {}
		WinItem(const GUniqueWinItem & rhs) : itemid(rhs.itemid), bid_min(rhs.bid_min) {}
		void Clear()
		{
			itemid = 0;
			bid_min = 0;
		}
	};
	class UniqueAuctionManager : public IntervalTimer::Observer
	{
	public:
		enum STATUS
		{
			ST_OPEN      = 0x0001,
			ST_DATAREADY = 0x0002,
			ST_BID       = 0x0004,
			ST_ANNOUNCE  = 0x0008,
			//ST_WRITEDB   = 0x0008,//正在进行写数据库
		};
		enum RESULT
		{
			RESULT_NO_UNIQUE 	= -1,
		};
		enum
		{
#ifdef UNIQUE_DEBUG
			BEGIN_BID_TIME			= 0,
			END_ANNOUNCE_TIME		= 300,
			LAST_ANNOUNCE_TIME		= 1000,
			END_BID_TIME			= 1200,
			BEGIN_LAG_TIME			= 300,
			ONE_DAY_SECONDS			= 1800,
			//BEGIN_BID_TIME			= 0,
			//END_ANNOUNCE_TIME		= 0,
			//LAST_ANNOUNCE_TIME		= 0,
			//END_BID_TIME			= 75600,
			//BEGIN_LAG_TIME			= 75600,
			//ONE_DAY_SECONDS			= 86400,
/*十分钟周期
			BEGIN_BID_TIME			= 0,
			END_ANNOUNCE_TIME		= 60,
			LAST_ANNOUNCE_TIME		= 510,
			END_BID_TIME			= 540,
			BEGIN_LAG_TIME			= 300,   //服务器启动时刻如果超过BEGIN_BID_TIME半小时就不再开启
*/
#else
			BEGIN_BID_TIME			= 70200,  //19:30开启ST_BID 和 ST_ANNOUNCE状态
			END_ANNOUNCE_TIME		= 72000,  //20:00结束ST_ANNOUNCE 状态
			LAST_ANNOUNCE_TIME		= 75300,  //20:55最后公告
			END_BID_TIME			= 75600,  //21:00结束
			BEGIN_LAG_TIME			= 1860,   //服务器启动时刻如果超过BEGIN_BID_TIME半小时就不再开启
			ONE_DAY_SECONDS			= 86400,
#endif
			UNIQUE_UPDATE_INTERVAL 		= 10,
			//BID_MAX_TIMES_PER_ROLE		= 3,
			//BID_MAX_TIMES			= 30000,
			BID_MAX_TIMES_PER_ROLE 		= 500,
			BID_MAX_TIMES 			= 20000,
			DEFAULT_MAX_PRICE		= 2000000000,
			HISTORY_LIST_SIZE		= 10,
			//SAME_BIDDER_MAX			= 10, //至多显示 个出价相同的玩家
			SAME_BIDDER_MAX 		= 4,
		};
		typedef std::map<int, std::vector<UniqueBidder> > PriceMap; //<price, bidderlist>
		typedef std::map<int, std::map<int, int> > RoleMap; // <roleid, <money, bid_time> >
		typedef std::map<std::pair<int, int>, WinItem> SpecialDayItems; // < <mon,day>, item >
		typedef std::map<int, std::vector<WinItem> > LevelItems; // <level, itemlist>
	private:
		PriceMap price_map;
		RoleMap role_map;
		SpecialDayItems special_items;
		LevelItems level_items;
		int status;
		time_t t_base;
		time_t t_forged;   
		int bid_times;
		WinItem _itemtobid;
		std::vector<UniqueWinner> _winners;
		int _winner;
		int _win_item;
		bool dirty;

		UniqueAuctionManager()
		{ 
			status = 0;
			t_base = 0;
			t_forged = 0;
			bid_times = 0;
			_winner = 0;
			_win_item = 0;
			dirty = false;
		}  
		time_t AnnounceEndTime() { return t_base + END_ANNOUNCE_TIME; }
		time_t BidBeginTime() { return t_base + BEGIN_BID_TIME; }
		time_t LastAnnounceTime() { return t_base + LAST_ANNOUNCE_TIME; }
		time_t BidEndTime() { return t_base + END_BID_TIME;}
	public:
		~UniqueAuctionManager() { }
		static UniqueAuctionManager* GetInstance() { static UniqueAuctionManager instance; return &instance;}
		bool Initialize();
		bool ReadLevelConfig(Conf * conf, std::string level_identification, int level);
		bool LoadConfig();
		bool GenerateItem(bool tomorrow)
		{
			time_t next = GetTime();
			if (tomorrow)
				next += ONE_DAY_SECONDS;
			struct tm dt;
			localtime_r (&next, &dt);
			SpecialDayItems::const_iterator it = special_items.find(std::make_pair(dt.tm_mon, dt.tm_mday));
			if (it != special_items.end())
			{
				_itemtobid.itemid = it->second.itemid;
				_itemtobid.bid_min = it->second.bid_min;
				return true;
			}
			int random = rand();
			int level = random%2;
			std::vector<WinItem> & list = level_items[level];
			if (list.size() == 0)
				return false;
			int index = random%list.size();
			_itemtobid.itemid = list[index].itemid;
			_itemtobid.bid_min = list[index].bid_min;
			return true;
		}
		bool DebugGenerateItem(int mon, int day)
		{
			SpecialDayItems::const_iterator it = special_items.find(std::make_pair(mon, day));
			if (it != special_items.end())
			{
				_itemtobid.itemid = it->second.itemid;
				_itemtobid.bid_min = it->second.bid_min;
				return true;
			}
			_itemtobid.Clear();
			return false;
		}
		bool LoadData(const GUniqueAuction & data);
		bool SendBidHistory(int roleid, unsigned int sid, unsigned int localsid);
		//void SaveWinItem(const GUniqueWinItem & today, const GUniqueWinItem & tomorrow);
		void SetDirty(bool d) { dirty = d; }
		bool IsDirty() { return dirty; }
		void SaveToDB(GUniqueAuction & data);
		int Bid(int roleid, Octets rolename, int lower_bound_money, int upper_bound_money);
		time_t GetTime();
		void SetForgedTime(time_t forge);
		void BeginBid();
		void EndBid();
		time_t UpdateTime();
		void UpdateStatus(time_t now);
		bool Update();
		void OnSaveDB(int retcode);
		bool OnDBConnect(Protocol::Manager *manager, int sid);
		void BidBeginAnnounce(int winitem);
		void BidStartAnnounce(int winitem);
		void BidLastAnnounce();
		void BidEndAnnounce(int roleid, Octets name, int price, int itemid);
		int GetWinItem(int roleid, int & winitem);
		int GetBidTimes() { return bid_times; }
	};
};
#endif

