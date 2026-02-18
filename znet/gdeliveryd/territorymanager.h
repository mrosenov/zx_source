#ifndef __GNET_TERRITORYMANAGER_H
#define __GNET_TERRITORYMANAGER_H
#include "gterritorystore"
#include "territoryserver"
#include "domaindataman.h"
//测试
//#define TERRI_DEBUG
namespace GNET
{
	class DBTerritoryChallengeArg;
	class GTerritoryEnter;
	class GTerritoryLeave;
	class GTerritoryChallenge;
	class GTerritoryItemGetArg;
	class GTerritoryItemGetRes;
	class PlayerInfo;
	class TerritoryBidAnnounceTask: public Thread::Runnable
	{
		public:
			enum
			{
				ANNOUNCE_BID1 = 1,
				ANNOUNCE_BID2,
				ANNOUNCE_BID3,
				ANNOUNCE_BID4,
			};
			int _type;
			TerritoryBidAnnounceTask(int type)
			{
				_type = type;
			}
			virtual void Run();
	};
	class TerritoryAwardAnnounceTask: public Thread::Runnable
	{
		public:
			int _territoryid;
			int _itemid;
			TerritoryAwardAnnounceTask(int tid, int itemid)
			{
				_territoryid = tid;
				_itemid = itemid;
			}
			virtual void Run();
	};

	class TChallenge
	{
	public:
		unsigned int _factionid;
		int _itemcount;
		TChallenge(unsigned int id = 0, int count = 0):_factionid(id), _itemcount(count){}
		void Reset()
		{
			_factionid = 0;
			_itemcount = 0;
		}
	};
	class TerritoryManager;
	class TerritoryInfo
	{
	public:
		//DB数据
		int _id;
		unsigned int _owner;
		int _occupy_time;
		int _color;
		std::vector<TChallenge> _candidate_challenges;
		unsigned int _defender;
		TChallenge _success_challenge;
		float _success_award;
		int _start_time;
		int _assis_drawn_num;
		int _rand_award_itemid;
		int _rand_award_itemcount;
		char _rand_award_drawn;
		//从配置读取的数据 
		int type;
		int item_min;
		int item_max;
		//临时数据
		int status;
		int map_tag; //地图编号
		int timeout;
		std::set<int> team_defender;
		std::set<int> team_attacker;
		TerritoryInfo(const GTerritoryInfo & g);
		GTerritoryInfo SaveToDB()
		{
			GTChallengeVector clist;
			std::vector<TChallenge>::const_iterator it, ite = _candidate_challenges.end();
			for (it = _candidate_challenges.begin(); it != ite; ++it)
				clist.push_back(GTChallenge(it->_factionid, it->_itemcount));
			return GTerritoryInfo(_id, _owner, _occupy_time, _color, clist, _defender, GTChallenge(_success_challenge._factionid, _success_challenge._itemcount), _success_award, _start_time, _assis_drawn_num, _rand_award_itemid, _rand_award_itemcount, _rand_award_drawn);
		}
		void EnterRemind();
		void EndAnnounce(int result, int time);
	};
	class TerritoryTag
	{
	public:
		const int gs_id;
		const int map_type;
		const int player_limit;
		int tid; //被分配给哪块领土
		TerritoryTag(int id, int type, int limit):gs_id(id), map_type(type), player_limit(limit), tid(0){}
	};
	class TypeConfig
	{
	public:
		int assis_num; 		//可领取拥兵证个数
		int battle_last_time;	//战斗持续时间
		int score;
		TypeConfig(int assis = 0, int time = 0, int sco = 0):assis_num(assis), battle_last_time(time),score(sco) {}
	};
	class RandAwardItem
	{
	public:
		int itemid;
		int itemcount;
		RandAwardItem(int id = 0, int count = 0):itemid(id), itemcount(count) {}
	};
	class TerritoryManager : public IntervalTimer::Observer
	{
	public:
		#define FLOAT_ZERO 0.001f	//宣战金1-20000千金银票情况下 均不会出现精度问题
		enum STATUS //全部战场状态
		{
			ST_OPEN      = 0x0001,  // 领土战功能是否开启
			ST_DATAREADY = 0x0002,  // 领土数据是否已经初始化
			ST_BID       = 0x0004,  // 领土竞价开始
			ST_ASSISGET  = 0x0008,  // 可领拥兵证状态
			ST_FIGHT     = 0x0010,  // 战斗阶段
			ST_BIDANNOUNCE= 0x0020, // 宣战前公告
			ST_AWARDANNOUNCE= 0x0040, //神器公告
			ST_GSREADY	= 0x0080, //城战gs是否就绪
			ST_REAL 	= ~(ST_OPEN|ST_DATAREADY|ST_GSREADY),//有效状态
		};
		enum TERRITORY_STATUS //单个领土状态
		{
			TS_BIDBUSY       = 0x0001,  // 其他帮派已经出价,正在进行数据库操作
			TS_FIGHTING      = 0x0002,  // 战争进行中
			TS_SENDSTART     = 0x0004,  // 开始城战协议已经发出
			TS_CANCEL 	 = 0x0008,  // 城战取消
		};
		enum
		{
			ATTACKER_WIN = 1,
			DEFENDER_WIN = 2,
		};
		enum TERRITORY_ROLE
		{
			ROLE_ATTACKER = 1,
			ROLE_DEFENDER = 2,
		};
		enum
		{
#ifdef TERRI_DEBUG
			UPDATE_INTERVAL   = 6,
			TERRITORY_BID_ANNOUNCE_INTERVAL   = 10,
			TERRITORY_AWARD_ANNOUNCE_INTERVAL   = 10,
			BEGIN_BID_ANNOUNCE= 0,
			BEGIN_BID_TIME 	= 0,
			END_BID_ANNOUNCE = 540,
			BID_END_NOTICE	  = 570,
			END_BID_TIME	= 600,
			BEGIN_FIGHT_TIME = 900,
			END_FIGHT_TIME 	 = 3000,
			END_AWARD_ANNOUNCE= 3300,
			ENTER_REMIND_AHEAD = 60,
			CONDITION_JOINTIME = 900,
			CHALLENGE_COOLTIME = 10,
			TERRITORY_BID_LIST_MAX = 3,
			CONDITION_CREATE_TIME = 900,
#else
			UPDATE_INTERVAL   = 60,
			TERRITORY_BID_ANNOUNCE_INTERVAL   = 900,
			TERRITORY_AWARD_ANNOUNCE_INTERVAL   = 300,
			BEGIN_BID_ANNOUNCE= 324000,    // 宣战公告开始  周四18:00
			BEGIN_BID_TIME    = 331200,    // 宣战开始,周四20:00
			END_BID_ANNOUNCE  = 337500,    // 宣战公告结束  周四21:45
			BID_END_NOTICE	  = 338100,    // 宣战最后通牒  周四21:55
			END_BID_TIME   	  = 338400,    // 宣战结束 周四22:00
			BEGIN_FIGHT_TIME  = 417600,    // 战斗阶段开始 周五20:00
			END_FIGHT_TIME    = 597600,    // 战斗阶段结束 周日22:00
			END_AWARD_ANNOUNCE= 599400,    // 神器公告结束 周日22:30
			ENTER_REMIND_AHEAD = 300,
			CONDITION_JOINTIME = 604800,	//加入帮派满一周才可参战
			START_ANNOUNCE_AHEAD = 1800,
			CHALLENGE_COOLTIME = 30,
			TERRITORY_BID_LIST_MAX = 50,
			CONDITION_CREATE_TIME = 604800,
#endif
			FIGHTING_TIME_MAX = 3600*3, 	// 每场战斗的超时时间
			BID_ITEMID = 19680,  //千金银票id
			ASSIS_ITEMID = 34129,
			ITEM_TO_MONEY = 10000000,
		};
		enum ITEM_TYPE
		{
			GET_ASSIST = 1,
			GET_BID_FAIL_REWARD,
			GET_FIGHT_SUCCESS_REWARD,
			GET_RAND_AWARD,
		};
		enum SAVELIST_REASON
		{
			SAVELIST_REASON_GETASSIST = 1,
			SAVELIST_REASON_BIDBEGIN,
			SAVELIST_REASON_BIDEND,
			SAVELIST_REASON_FIGHTBEGIN,
			SAVELIST_REASON_STATUS,
			SAVELIST_REASON_SUCCESSREWARD,
		};
		enum SAVE_REASON
		{
			SAVE_REASON_END		= 1,
			SAVE_REASON_BIDFAIL,
			SAVE_REASON_RANDAWARD,
			SAVE_REASON_CANCEL,
			SAVE_REASON_DEBUG,
		};
		enum 
		{
			RAND_AWARD_UNDRAWN = 0,
			RAND_AWARD_DRAWN = 1,
		};
		typedef std::vector<TerritoryInfo>  TVector;
		typedef std::map<int, TerritoryTag> ServerMap;
		typedef std::map<int, TypeConfig> ConfigMap;
		typedef std::vector<RandAwardItem> RandAwardList;
		typedef std::map<int, time_t> CooldownMap;
		typedef std::map<unsigned int, int> ScoreMap; //<factionid, score>
	private:
		time_t BidAnnounceBegin() { return t_base + BEGIN_BID_ANNOUNCE; }
		time_t BidBeginTime() { return t_base + BEGIN_BID_TIME;}
		time_t BidAnnounceEnd() { return t_base + END_BID_ANNOUNCE; }
		time_t BidEndNotice() { return t_base + BID_END_NOTICE; }
		time_t BidEndTime() { return t_base + END_BID_TIME;}
		time_t FightBeginTime() { return t_base + BEGIN_FIGHT_TIME;}
		time_t FightEndTime() { return t_base + END_FIGHT_TIME;}
		time_t AwardAnnounceEnd() { return t_base + END_AWARD_ANNOUNCE; }

		ServerMap servers;
		ConfigMap type2config;
		RandAwardList randawardlist;
		TVector tlist;
		int status;
		time_t t_base;      //周一凌晨0:00
		time_t t_forged;   
		bool add_task;
		CooldownMap cooldown;
		ScoreMap scoremap;
		int cooldown_cursor;
		std::set<time_t> fight_announce_set;

		TerritoryManager()
		{ 
			status = 0;
			t_base = 0;
			t_forged = 0;
			add_task = false;
			cooldown_cursor = 0;
		}  
	public:
		~TerritoryManager() { }
		static TerritoryManager* GetInstance() { static TerritoryManager instance; return &instance;}
		bool AllocMapTag(int type, int territoryid, int & tag_id, int & gs)
		{
			if (territoryid == 0) return false;
			ServerMap::iterator it, ite = servers.end();
			for (it = servers.begin(); it != ite; ++it)
			{
				if (it->second.map_type == type && it->second.tid == 0)
				{
					tag_id = it->first;
					gs = it->second.gs_id;
					it->second.tid = territoryid;
					return true;
				}
			}
			return false;
		}
		void FreeMapTag(int tag_id)
		{
			ServerMap::iterator it = servers.find(tag_id);
			if (it != servers.end())
				it->second.tid = 0;
		}
		TerritoryTag * FindMapTag(int tag_id)
		{
			ServerMap::iterator it = servers.find(tag_id);
			if (it == servers.end())
				return NULL;
			else
				return &(it->second);
		}
		bool IsTerritoryServer(int gs_id, int map_tag)
		{
			if (gs_id == 0 || gs_id == -1)
				return false;
			ServerMap::const_iterator it = servers.find(map_tag);
			if (it == servers.end())
				return false;
			else if (it->second.gs_id != gs_id)
				return false;
			else
				return true;
		}
		void SetCooldown(int fid)
		{
			cooldown[fid] = GetTime();
		}
		bool IsInCooldown(int fid)
		{
			CooldownMap::iterator it = cooldown.find(fid);
			if (it == cooldown.end())
				return false;
			else
				return it->second+CHALLENGE_COOLTIME >= GetTime();
		}
		int GetScore(int fid)
		{
			if (fid == 0) return 0;
			ScoreMap::const_iterator it = scoremap.find(fid);
			if (it == scoremap.end())
				return 0;
			else
				return it->second;
		}
		void SetOwner(TerritoryInfo & info, unsigned int new_owner, bool load);
		bool SendTerritoryMap(int roleid, unsigned int sid, unsigned int localsid);
		bool LoadConfig();
		bool Initialize();
		int Challenge(const GTerritoryChallenge& proto);
		bool RegisterServer(int server, const TerritoryServerVector & fields);
		int  GetMapType(int id);
		bool LoadTerritoryList(const GTerritoryStore & v);
		void OnChallenge(int ret, DBTerritoryChallengeArg *arg, int & itemtotal);
		void LogTerritoryList();
		void OnTerritoryEnd(int id, int result);
		void OnTerritoryStart(int territoryid, int retcode);
		void OnLogout(int roleid, int gsid, int map);
		bool OnDBConnect(Protocol::Manager *manager, int sid);
		void OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map, char & role);
		int Enter(const GTerritoryEnter & proto, int & dst_gs, int & map_tag, PlayerInfo * pinfo);
		int Leave(const GTerritoryLeave & proto);
		void OnLoginWorld(int roleid);
		void OnLoginTerritory(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map);
		bool SyncTerritoryGS();
		bool SyncTerritoryGS(unsigned int sid);
		bool Update();
		bool UpdateStatus(time_t now);
		void UpdateTerritory(time_t now);
		char SelectColor(unsigned int factionid);
		bool SyncMapNotice();
		bool SyncMapNotice(short);
		void BeginChallenge();
		bool SendBonus();
		bool OnSendBonus(short ret, unsigned int fid, unsigned int money);
		void RecordFactionDynamic();
		void EndChallenge();
		void BeginFight();
		void EndFight();
		bool ArrangeTerritory();
//		void TestTerritory(int id, int challenger);
		void DebugSetOwner(int id, int factionid);
		time_t UpdateTime();
		time_t GetTime();
		void SetForgedTime(time_t forge);
		bool IsAdjacent(int territoryid, unsigned int fid);
		void ItemGet(GTerritoryItemGetArg * arg, GTerritoryItemGetRes * res);
		void SaveTerritoryList(int reason);
		void SaveTerritory(int reason, const GTerritoryInfo & info);
		void StartAnnounce();
		bool IsBidAnnounceState() { return (status & ST_BIDANNOUNCE); } 
		bool IsAwardAnnounceState() { return (status & ST_AWARDANNOUNCE); } 
		void OnDelFaction(unsigned int factionid);
		void OnDisconnect(int gsid);
	};
};
#endif

