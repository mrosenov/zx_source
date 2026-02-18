#include "conf.h"
#include "log.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

#include "gtopitem"
#include "gtoptable"
#include "gtoptableold"
#include "roleid"
#include "userid"
#include "user"
#include "groleinventory"
#include "grolepocket"
#include "grolestorehouse"
#include "grolestatus"
#include "groleforbid"
#include "grolebase"
#include "familyid"
#include "gfolk"
#include "gfamilyskill"
#include "gfamily"
#include "factionid"
#include "gmember"
#include "gfactioninfo"
#include "gsnsroleinfo"
#include "gsnsplayerinfo"
#include "grolebase2"
#include "gfactionbase"
#include "gcrssvrteamsdata"

#include "storage.h"

#include "thread.h"
#include "timertask.h"
#include "protocol.h"
#include "rpcdefs.h"
#include "integer"
#include "callid.hxx"
#include "xmlversion.h"
#include "marshal.h"
#include "localmacro.h"

using namespace GNET;

struct NewToptable;

class DBPolicy : public Thread::Pool::Policy
{   
public:
    virtual void OnQuit( )
    {   
		printf("toplist quit\n");
    }   
};

static class DBPolicy s_policy;
static int g_now;

namespace GNET
{

class CrssvrTeamsManager
{
public:
	static CrssvrTeamsManager* Instance() { static CrssvrTeamsManager instance; return &instance;}
	typedef std::pair<int, int> PairPeriod;
	typedef std::vector<PairPeriod> OpenPairPeriodsVec;

protected:
	OpenPairPeriodsVec seasonperiod;// 赛季开始分段

public:
	CrssvrTeamsManager()
	{
		// 每年1.4.7.10月的1号开始比赛 3，6，9，12月25日结束比赛
		seasonperiod.push_back(std::make_pair(0,2));// 1-3月
		seasonperiod.push_back(std::make_pair(3,5));// 4-6月
		seasonperiod.push_back(std::make_pair(6,8));// 7-9月
		seasonperiod.push_back(std::make_pair(9,11));// 10-12月
	}
	
	~CrssvrTeamsManager() 
	{ 
	}

	time_t GetSeasonOpenTime()
	{
		time_t start_time = 0;
		time_t now = Timer::GetTime();
		struct tm dt;
		localtime_r(&now, &dt);
		int t_mon = dt.tm_mon;
		int t_day = dt.tm_mday;

		OpenPairPeriodsVec::const_iterator it, ite = seasonperiod.end();
		for (it = seasonperiod.begin(); it != ite; ++it)
		{

			LOG_TRACE("GetSeasonOpenTime, now=%d, t_mon=%d, t_day=%d, it->first=%d, it->second=%d", now, t_mon, t_day, it->first, it->second);
			if((t_mon >= it->first && t_mon < it->second ) || (t_mon == it->second && t_day >= CRSSVRTEAMS_SEASON_OPEN_DAY && t_day <= CRSSVRTEAMS_SEASON_REOPEN_DAY))
			{
				// 赛季开始的时间
				dt.tm_sec = 0;
				dt.tm_min = 0;
				dt.tm_hour = 0;
				dt.tm_mday = CRSSVRTEAMS_SEASON_OPEN_DAY;
				dt.tm_mon = it->first;
				start_time = mktime(&dt);
				LOG_TRACE("GetSeasonOpenTime, now=%d, it->first=%d, it->second=%d, start_time=%d", now, it->first, it->second, start_time);
				return start_time;
			}
		}
		
		return start_time;

	}
};

static bool g_bWeekly = false;

class DBPutTopTable : public Rpc
{
#define RPC_BASECLASS Rpc
	#include "dbputtoptable"
#undef RPC_BASECLASS
	NewToptable *newtoptable;
	int tid;
	void Server(Rpc::Data *argument, Rpc::Data *result, Manager *manager, Manager::Session::ID sid)
	{
	}

	void Client(Rpc::Data *argument, Rpc::Data *result, Manager *manager, Manager::Session::ID sid);
	void OnTimeout();

};
static GNET::Protocol::Type _state_GameDBClient[] = 
{
	RPC_DBPUTTOPTABLE,
};

Protocol::Manager::Session::State state_GameDBClient(_state_GameDBClient, sizeof(_state_GameDBClient)/sizeof(GNET::Protocol::Type), 86400);

class GameDBClient : public Protocol::Manager
{
	static GameDBClient instance;
	size_t		accumulate_limit;
	Session::ID	sid;
	bool		conn_state;
	Thread::Mutex	locker_state;
	enum { BACKOFF_INIT = 2, BACKOFF_DEADLINE = 256 };
	size_t		backoff;
	void Reconnect();
	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid);
	void OnAbortSession(Session::ID sid);
	void OnCheckAddress(SockAddr &) const;
public:
	NewToptable *newtoptable;
	static GameDBClient *GetInstance() { return &instance; }
	std::string Identification() const { return "GameDBClient"; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }
	GameDBClient() : accumulate_limit(0), conn_state(false), locker_state("GameDBClient::locker_state"),backoff(BACKOFF_INIT) { }

	bool SendProtocol(const Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(const Protocol *protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(Protocol &protocol) { return conn_state && Send(sid, protocol); }
	bool SendProtocol(Protocol *protocol) { return conn_state && Send(sid, protocol); }

};

static DBPutTopTable __stub_DBPutTopTable (RPC_DBPUTTOPTABLE, new GTopTable, new Integer);

GameDBClient GameDBClient::instance;

void GameDBClient::Reconnect()
{
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	//backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

const Protocol::Manager::Session::State* GameDBClient::GetInitState() const
{
	return &state_GameDBClient;
}

void GameDBClient::OnDelSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log(LOG_ERR,"toplist:: disconnect from GameDB\n");
}

void GameDBClient::OnAbortSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	conn_state = false;
	Reconnect();
	Log::log(LOG_ERR,"toplist:: connect GameDB failed\n");
}

void GameDBClient::OnCheckAddress(SockAddr &sa) const
{
}

};

static char conf_filename[256];

static void TravelTable(const char *table_name, StorageEnv::IQuery &q)
{
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage(table_name);
		printf("TravelTable %s, pstorage = 0x%08x\n", table_name, (unsigned int)pstorage);
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.browse( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log(LOG_ERR, "err occur when walking %s\n", table_name);
		return;
	}
}

namespace ListInfo
{
	const int PERSONAL_LIST_SIZE = 500;
	const int FACTION_LIST_SIZE = 50;
	const int FACTION_ACT_LIST_SIZE = 200;
	const int FAMILY_LIST_SIZE = 100;
	const int CHARM_LIST_SIZE = 50;
	const int AREA_NUM = 20; //不再扩充
	const int AREA_NUM2 = 12; //在此处扩充
	const int AREA_TOTAL = AREA_NUM + AREA_NUM2;
	const int TASK_NUM = 9;
	const int DEITY_NUM = 4;
	//const int WEEKLYTOP_BEGIN = 1000;

	enum {
		TOP_PERSONAL_LEVEL = 1,
		TOP_PERSONAL_MONEY = 2,
		TOP_PERSONAL_REPUTATION = 3,
		TOP_PERSONAL_CREDIT_START = 10,
		//此处只使用20个声望
		TOP_FACTION_LEVEL = 31,
		TOP_FACTION_MONEY = 32,
		TOP_FACTION_POPULATION = 33,
		TOP_FACTION_PROSPERITY = 34,
		TOP_FACTION_NIMBUS	= 35,
		TOP_FACTION_ACTIVITY	= 36,
		TOP_FACTION_CREDIT_START = 40,
		//此处只使用20个声望
		TOP_FAMILY_TASK_START = 60,

		TOP_PERSONAL_CHARM= 80,
		TOP_FRIEND_CHARM = 81,
		TOP_SECT_CHARM = 82,
		TOP_FACTION_CHARM = 83,
		
		TOP_PERSONAL_DEITY_LEVEL_START = 90,
		TOP_TOWERRAID_LEVEL = 95,

		TOP_CRSSVRTEAMS_SCORE = 96,

		TOP_PERSONAL_CREDIT_START2 = 200,
		TOP_FACTION_CREDIT_START2 = 300,
		TOP_TABLEID_MAX = TOP_FACTION_CREDIT_START2+100, // 预留100个声望id
	};

	const int MIN_LIST_MONEY = 10000;
	
	static inline bool IsPersonal(int tid) { return tid < 30 || (tid >= TOP_PERSONAL_CREDIT_START2 && tid < (TOP_PERSONAL_CREDIT_START2 + AREA_NUM2)); }
	static inline bool IsFaction(int tid)  { return (tid > 30 && tid < 60) || (tid >= TOP_FACTION_CREDIT_START2 && tid < (TOP_FACTION_CREDIT_START2 + AREA_NUM2)); }
	static inline bool IsFamily(int tid)  { return tid >= 60 && tid < 80; }
	static inline bool IsCrssvrTeams(int tid)  { return tid == 96; }

	static inline int MaxSize(int tid)  { return IsPersonal(tid) ? PERSONAL_LIST_SIZE : FACTION_LIST_SIZE; }
};

struct TRoleData
{
	int level;
	int64_t exp;
	unsigned int  money;
	int reputation;
	int jointime;
	int credits[ListInfo::AREA_TOTAL];
	int charm_personal;
	int charm_friend;
	int charm_sect;
	short cultivation;
	unsigned short deity_level;
	int64_t deity_exp;
	int total_tower_time;//通过塔各等级的总时间
	int tower_level;//爬塔最高等级

	unsigned short oldrank_level;
	unsigned short oldrank_money;
	unsigned short oldrank_reputation;
	unsigned short oldrank_credits[ListInfo::AREA_TOTAL];
	unsigned short oldrank_charm_personal;
	unsigned short oldrank_charm_friend;
	unsigned short oldrank_charm_sect;
	unsigned short oldrank_deity_level[ListInfo::DEITY_NUM];
	unsigned short oldrank_tower;//爬塔
	TRoleData() : level(0), exp(0), money(0), reputation(0), jointime(0),charm_personal(0), charm_friend(0), charm_sect(0),cultivation(0),deity_level(0),deity_exp(0),total_tower_time(0),tower_level(0),
	oldrank_level(0xffff),oldrank_money(0xffff),oldrank_reputation(0xffff),
	oldrank_charm_personal(0xffff), oldrank_charm_friend(0xffff), oldrank_charm_sect(0xffff),oldrank_tower(0xffff)
	{ 
		memset(credits, 0, sizeof(int) * ListInfo::AREA_TOTAL);
		memset(oldrank_credits, 0xff, sizeof(unsigned short) * ListInfo::AREA_TOTAL);
		memset(oldrank_deity_level, 0xff, sizeof(unsigned short) * ListInfo::DEITY_NUM);
	}
};

struct TFactionData
{
	std::vector<int> members;
	char level;
	unsigned int total_level;
	unsigned int prosperity;
	unsigned int nimbus;
	unsigned int master;
	int charm;
	int64_t money;
	int64_t credits[ListInfo::AREA_TOTAL];
	int activity;

	unsigned short oldrank_level;
	unsigned short oldrank_money;
	unsigned short oldrank_population;
	unsigned short oldrank_prosperity;
	unsigned short oldrank_nimbus;
	unsigned short oldrank_credits[ListInfo::AREA_TOTAL];
	unsigned short oldrank_charm;
	unsigned short oldrank_act;

	TFactionData() : level(0), total_level(0), prosperity(0), nimbus(0),charm(0), money(0),activity(0) 
		, oldrank_level(0xffff), oldrank_money(0xffff), oldrank_population(0xffff)
		, oldrank_prosperity(0xffff), oldrank_nimbus(0xffff), oldrank_charm(0xffff), oldrank_act(0xffff)
	{
		memset(credits, 0, sizeof(int64_t) * ListInfo::AREA_TOTAL);
		memset(oldrank_credits, 0xff, sizeof(unsigned short) * ListInfo::AREA_TOTAL);
	}
};

struct TFamilyData
{
	unsigned int master;
	std::vector<int> members;
	unsigned int records[ListInfo::TASK_NUM];
	unsigned short oldrank_records[ListInfo::TASK_NUM];
	TFamilyData() : master(0) {
		memset(records, 0, sizeof(unsigned int) * ListInfo::TASK_NUM);
		memset(oldrank_records, 0xff, sizeof(unsigned short) * ListInfo::TASK_NUM);
	}
};

struct TCrssvrTeamsData
{
	int crssvrteams_score;// 战队积分
	
	unsigned short oldrank_crssvrteams_score;
	TCrssvrTeamsData() : crssvrteams_score(0), oldrank_crssvrteams_score(0xffff) 
	{
	}
};



typedef std::map<unsigned int, TRoleData> RoleMap;
typedef std::map<unsigned int, TFactionData> FactionMap;
typedef std::map<int, GTopTable> ToptableMap;
typedef std::map<unsigned int, int> JointimeMap;
typedef std::map<unsigned int, TFamilyData> FamilyMap;
typedef std::map<unsigned int, TCrssvrTeamsData> CrssvrTeamsMap;


struct TDataBase
{

	class UserQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os;
				value_os = value;

				UserID      userid;
				User        user;
				try
				{
					key_os >> userid;
					value_os >> user;
					if( tdatabase->filter.find(userid.id) != tdatabase->filter.end() )
						tdatabase->real_filter.insert(user.logicuid);
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "UserQuery, error unmarshal.");
					return true;
				}
				return true;
			}
	};

	class StatusQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os;
				value_os = value;

				RoleId      roleid;
				GRoleStatus status;
				try
				{
					key_os >> roleid;
					if( tdatabase->Forbid(roleid.id) ) return true;
					value_os >> status;
					TRoleData &roledata = tdatabase->role_map[roleid.id];
					roledata.level = status.level + status.reborndata.size()/sizeof(int)*200;
					roledata.exp   = status.exp;
					roledata.reputation = status.reputation;
					roledata.cultivation =status.cultivation;
					if( status.credit.size() )
						memcpy(roledata.credits, status.credit.begin(), std::min(status.credit.size(), sizeof(int)*ListInfo::AREA_TOTAL));
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "StatusQuery, error unmarshal role %d", roleid.id);
					return true;
				}
				return true;
			}
	}; 

	class FamilyQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os;
				value_os = value;

				unsigned int      fid;
				GFamily family;
				try
				{
					key_os >> fid;
					value_os >> family;
					TFamilyData familydata;
					for(std::vector<GFolk>::const_iterator it = family.member.begin(), ie = family.member.end(); it != ie; ++it)
					{
						if( tdatabase->Forbid(it->rid) )
							continue;
						familydata.members.push_back(it->rid);
					}
					if( ! family.task_record.empty() )
					{
						std::copy(family.task_record.begin(), family.task_record.end(), familydata.records);
					}
					familydata.master = family.master;
					tdatabase->family_map.insert(FamilyMap::value_type(fid, familydata));
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "FamilyQuery, error unmarshal.");
					return true;
				}
				return true;
			}
	}; 

	class PocketQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os;
				value_os = value;

				RoleId      roleid;
				GRolePocket pocket;
				try
				{
					key_os >> roleid;
					value_os >> pocket;
					RoleMap::iterator it = tdatabase->role_map.find(roleid.id);
					if( it == tdatabase->role_map.end() )
						return true;
					it->second.money   = pocket.money;
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "PocketQuery, error unmarshal, roleid=%d", roleid.id);
					return true;
				}
				return true;
			}
	}; 

	class StorehouseQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os;
				value_os = value;

				RoleId      roleid;
				GRoleStorehouse storehouse;
				try
				{
					key_os >> roleid;
					value_os >> storehouse;
					RoleMap::iterator it = tdatabase->role_map.find(roleid.id);
					if( it == tdatabase->role_map.end() )
						return true;
					it->second.money   += storehouse.money;
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "StorehouseQuery, error unmarshal.");
					return true;
				}
				return true;
			}
	};

	class FactionQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os;
				value_os = value;

				FactionId      factionid;
				GFactionInfo factioninfo;
				try
				{
					key_os >> factionid;
					value_os >> factioninfo;
					if( tdatabase->Forbid(factioninfo.master) )  return true;

					TFactionData factiondata;
					factiondata.level = factioninfo.level;
					factiondata.master = factioninfo.master;
					factiondata.prosperity = factioninfo.prosperity;
					factiondata.nimbus = factioninfo.nimbus;
					factiondata.charm = factioninfo.charm;
					for(std::vector<FamilyId>::const_iterator it = factioninfo.member.begin(),
							ie = factioninfo.member.end(); it != ie; ++it)
					{
						FamilyMap::const_iterator fit = tdatabase->family_map.find(it->fid);
						if( fit == tdatabase->family_map.end()) continue;
						factiondata.members.insert(factiondata.members.end(),
								fit->second.members.begin(), fit->second.members.end() );
					}
					tdatabase->faction_map.insert(FactionMap::value_type(factionid.fid, factiondata));
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "FactionQuery, error unmarshal.");
					return true;
				}
				return true;
			}
	}; 

	class FactionBaseQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os(key);
				Marshal::OctetsStream value_os(value);

				FactionId      factionid;
				GFactionBase factionbase;
				try
				{
					key_os >> factionid;
					value_os >> factionbase;
					if (factionbase.activity <= 0 || (tdatabase->last_fac_act_toptime > 0 && factionbase.act_uptime <= tdatabase->last_fac_act_toptime))
						return true;
					FactionMap::iterator it = tdatabase->faction_map.find(factionid.fid);
					if (it == tdatabase->faction_map.end() )
					{
						Log::log(LOG_ERR, "factionbase query, fid %d not found", factionid.fid);
						return true;
					}
					it->second.activity = factionbase.activity;
				}
				catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "FactionBaseQuery, error unmarshal.");
					return true;
				}
				return true;
			}
	}; 

	class ToptableQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				//LOG_TRACE( "ToptableQuery Mashal top");
				Marshal::OctetsStream key_os;
				key_os = key;
				Marshal::OctetsStream value_os, value_old_os;
				value_os = value;
				value_old_os = value;

				int tid;
				GTopTable toptable;
				GTopTableOld toptableold;
				
				key_os >> tid;

				try
				{
					GTopTable tab;
					value_os >> tab;
					toptable = tab;
				} catch ( Marshal::Exception & ) {
					value_old_os >> toptableold;
					
					toptable.id = toptableold.id;
					toptable.maxsize = toptableold.maxsize;
					toptable.time = toptableold.time;
					toptable.items = toptableold.items;
					toptable.weekly = toptableold.weekly;
					toptable.items_detail.clear();
					toptable.reserved = 0;
					toptable.reserved1 = 0;

					LOG_TRACE( "ToptableQuery, error unmarshal, toptable.id=%d.", tid);
				}

				tdatabase->old_toptable_map[tid] = toptable;
				return true;
			}
	}; 

	class SNSPlayerInfoQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os(key);
				Marshal::OctetsStream value_os(value);

				int roleid;
				GSNSPlayerInfo info;
				try
				{
					key_os >> roleid;
					value_os >> GSNSRoleInfo() >> info;
					RoleMap::iterator it = tdatabase->role_map.find(roleid);
					if( it == tdatabase->role_map.end() )
						return true;
					it->second.charm_personal   = info.charm_personal;
					it->second.charm_friend   = info.charm_friend;
					it->second.charm_sect   = info.charm_sect;
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "SNSPlayerInfoQuery, error unmarshal, roleid=%d.", roleid);
				}
				return true;
			}
	}; 

	class Base2Query : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			const static int TOWER_DATA_HEADER = 24;
			const static int TOWER_MAX_LEVEL = 100;
			const static int TOWER_DATA_TAILER = 8*TOWER_MAX_LEVEL;
			struct tower_raid_data
			{
				int total_tower_time;
				int tower_level;
				tower_raid_data(){total_tower_time = 0; tower_level = 0;}
			};
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os(key);
				Marshal::OctetsStream value_os(value);

				int roleid;
				GRoleBase2 base;
				try
				{
					key_os >> roleid;
					value_os >> base;
					RoleMap::iterator it = tdatabase->role_map.find(roleid);
					if( it == tdatabase->role_map.end() )
						return true;
					it->second.deity_level  = base.deity_level;
					it->second.deity_exp 	= base.deity_exp;
					
					tower_raid_data raiddata = GetTowerData(base.tower_raid,roleid);
					it->second.tower_level  = raiddata.tower_level;
					it->second.total_tower_time = raiddata.total_tower_time;
				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "Base2Query, error unmarshal, roleid=%d.", roleid);
				}
				return true;
			}
			tower_raid_data GetTowerData(const Octets & towerdata,int roleid)
			{
				tower_raid_data rdata;
				//test code
				//rdata.tower_level = 2;	
				//rdata.total_tower_time = 1012;
				//return rdata;	
				//end
				if(towerdata.size()==0)
					return rdata;
				const int * pi = static_cast<const int *>(towerdata.begin());
				int version = *pi++;
				rdata.tower_level = *pi++;
				int start_level_today = *pi++;
				int end_level_today = *pi++;
				int tower_daily_reward_reset_time = *pi++;
				int monster_size = *pi++;
				if(towerdata.size() != TOWER_DATA_HEADER + sizeof(int)*monster_size + TOWER_DATA_TAILER)
					Log::log( LOG_ERR, "Base2Query, error decode towerdata, roleid=%d monstersize=%d datasize=%d", roleid, monster_size ,towerdata.size());
				if(rdata.tower_level > TOWER_MAX_LEVEL)
					Log::log( LOG_ERR, "Base2Query, error decode towerdata, roleid=%d tower_level=%d", roleid,rdata.tower_level);
				pi += monster_size;
			
				for(int i=0;i<TOWER_MAX_LEVEL && i<= rdata.tower_level;i++)
				{
					rdata.total_tower_time += *(pi+i*2);
				}		
				return rdata;
			}
	};
	class CrssvrTeamsQuery : public StorageEnv::IQuery
	{   
		public:
			TDataBase *tdatabase;
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				Marshal::OctetsStream key_os(key);
				Marshal::OctetsStream value_os(value);

				int teamid;
				GCrssvrTeamsData crssvrteams;
				try
				{
					key_os >> teamid;
					if(teamid <= 0) return true;
					value_os >> crssvrteams;

					int seasonlastbattletime = crssvrteams.info.seasonlastbattletime;
					int seasonstartime = CrssvrTeamsManager::Instance()->GetSeasonOpenTime();
					LOG_TRACE("teamid=%d, seasonlastbattletime=%d, seasonstartime=%d", teamid, seasonlastbattletime, seasonstartime);

					if(seasonlastbattletime < seasonstartime)
					{
						LOG_TRACE("teamid=%d, seasonlastbattletime=%d less than seasonstartime=%d", teamid, seasonlastbattletime, seasonstartime);
						return true;
					}

					// 最后一次战斗时间如果小于本赛季开始时间，则不排榜，因为不是本赛季的
					TCrssvrTeamsData &crssvrteamsdata = tdatabase->crssvrteams_map[teamid];
					crssvrteamsdata.crssvrteams_score  = crssvrteams.info.score;


				} catch ( Marshal::Exception & ) {
					Log::log( LOG_ERR, "CrssvrTeamsQuery, error unmarshal, teamid=%d.", teamid);
				}
				return true;
			}
	};

	RoleMap role_map;
	FactionMap faction_map;
	ToptableMap old_toptable_map;
	FamilyMap family_map;
	CrssvrTeamsMap crssvrteams_map;
	int last_fac_act_toptime; //上次帮派活跃度周榜更新时间

	TDataBase::TDataBase() : last_fac_act_toptime(0) {} 

	std::set<unsigned int> filter;
	std::set<unsigned int> real_filter;

	void AddFilter(const char *filtername = "filter")
	{
		if( access(filtername, R_OK) == -1 ) return;
		std::ifstream ifs(filtername);
		string line;
		filter.insert(0);
		while (std::getline(ifs, line))
		{
			int uid = atoi(line.c_str());
			if( uid != 0 )
			{
				fprintf(stderr, "filter %d\n", uid);
	//			filter.insert(uid&0xffffFFF0);
				filter.insert(uid);
			}
		}
	}
	bool Forbid(unsigned roleid) const
	{
		return roleid < 1024 || real_filter.find(roleid&0xffffFFF0) != real_filter.end();
	}

	void LoadLastFacActTopTime()
	{
		try
		{
			StorageEnv::Storage * ptop = StorageEnv::GetStorage("top");
			StorageEnv::AtomTransaction txn;
			try
			{
				Marshal::OctetsStream key, value, value_old;
				key << int(ListInfo::TOP_FACTION_ACTIVITY + WEEKLYTOP_BEGIN);
				if (ptop->find(key, value, txn))
				{
					GTopTable top;
					GTopTableOld toptableold_weekly;
					
					value_old = value;
					try
					{
						GTopTable tab;
						value >> tab;
						top = tab;
					}
					catch(Marshal::Exception &)
					{
						value_old >> toptableold_weekly;

						top.id = toptableold_weekly.id;
						top.maxsize = toptableold_weekly.maxsize;
						top.time = toptableold_weekly.time;
						top.items = toptableold_weekly.items;
						top.weekly = toptableold_weekly.weekly;
						top.items_detail.clear();
						top.reserved = 0;
						top.reserved1 = 0;

						LOG_TRACE( "LoadLastFacActTopTime post weekly table, unpack GTopTableOld tableid=%d\n", top.id );
					}

					last_fac_act_toptime = top.time;
					Log::formatlog("toplist", "load last_fac_act_toptime %d", last_fac_act_toptime);
					LOG_TRACE("toplist load last_fac_act_toptime %d", last_fac_act_toptime);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log(LOG_ERR, "err occur when LoadLastFacActTopTime what=%s\n", e.what());
			return;
		}
	}
	void Load()
	{
		LoadLastFacActTopTime();

		UserQuery qu;
		qu.tdatabase = this;
		TravelTable("user", qu);
		StatusQuery q;
		q.tdatabase = this;
		TravelTable("status", q);
		FamilyQuery qfa;
		qfa.tdatabase = this;
		TravelTable("family", qfa);

		PocketQuery qp;
		qp.tdatabase = this;
		TravelTable("inventory", qp);

		StorehouseQuery qs;
		qs.tdatabase = this;
		TravelTable("storehouse", qs);

		FactionQuery qf;
		qf.tdatabase = this;
		TravelTable("faction", qf);

		FactionBaseQuery qfb;
		qfb.tdatabase = this;
		TravelTable("factionbase", qfb);

		ToptableQuery qt;
		qt.tdatabase = this;
		TravelTable("top", qt);

		SNSPlayerInfoQuery qsns;
		qsns.tdatabase = this;
		TravelTable("snsplayerinfo", qsns);

		Base2Query qb2;
		qb2.tdatabase = this;
		TravelTable("base2",qb2);

		CrssvrTeamsQuery qct;
		qct.tdatabase = this;
		TravelTable("crssvrteams",qct);

	}
	void Clear()
	{
		role_map.clear();
		faction_map.clear();
		family_map.clear();
		crssvrteams_map.clear();
	}
	void CalFactionData()
	{
		for(FactionMap::iterator it = faction_map.begin(), ie = faction_map.end(); it != ie; ++it)
		{
			TFactionData &fdata = (*it).second;
			for(std::vector<int>::const_iterator it2 = fdata.members.begin(),
					ie2 = fdata.members.end(); it2 != ie2; ++it2)
			{
				const RoleMap::const_iterator it3 = role_map.find((*it2));
				if( it3 == role_map.end() )
					continue;
				const TRoleData &roledata = (*it3).second;
				fdata.money += (int64_t)(roledata.money);
				fdata.total_level += (unsigned int)(roledata.level);
				for(int j=0; j<ListInfo::AREA_TOTAL; ++j)
					fdata.credits[j] += (int64_t)(roledata.credits[j]);
			}
		}
	}
	void FillOld()
	{
		for(ToptableMap::const_iterator it = old_toptable_map.begin(), ie = old_toptable_map.end();it!=ie;++it)
		{
			const GTopTable &fdata = (*it).second;
			int tid = (*it).first;
			unsigned short n = fdata.items.size();
			for(unsigned short j = 0; j < n; ++j)
				FillOldRank(tid, fdata.items[j].id, j+1 );
		}
	}

	void FillOldRank(int tid, unsigned int id, unsigned short rank)
	{
		if( tid == ListInfo::TOP_PERSONAL_LEVEL )
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_level = rank;
			return;
		}
		else if( tid == ListInfo::TOP_PERSONAL_MONEY )
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_money = rank;
			return;
		}
		else if( tid == ListInfo::TOP_PERSONAL_REPUTATION )
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_reputation = rank;
			return;
		}
		else if( tid == ListInfo::TOP_CRSSVRTEAMS_SCORE )
		{
			CrssvrTeamsMap::iterator it = crssvrteams_map.find(id);
			if( it != crssvrteams_map.end() )
				it->second.oldrank_crssvrteams_score = rank;
			return;
		}
		else if( tid >= ListInfo::TOP_PERSONAL_CREDIT_START 
				&& tid < ListInfo::TOP_PERSONAL_CREDIT_START + ListInfo::AREA_NUM )
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_credits[tid-ListInfo::TOP_PERSONAL_CREDIT_START] = rank;
			return;
		}
		else if( tid >= ListInfo::TOP_PERSONAL_CREDIT_START2 
				&& tid < ListInfo::TOP_PERSONAL_CREDIT_START2 + ListInfo::AREA_NUM2 )
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_credits[tid-ListInfo::TOP_PERSONAL_CREDIT_START2+ListInfo::AREA_NUM] = rank;
			return;
		}
		else if( tid == ListInfo::TOP_FACTION_LEVEL )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_level = rank;
			return;
		}
		else if( tid == ListInfo::TOP_FACTION_MONEY )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_money = rank;
			return;
		}
		else if( tid == ListInfo::TOP_FACTION_POPULATION )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_population = rank;
			return;
		}
		else if( tid == ListInfo::TOP_FACTION_PROSPERITY )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_prosperity = rank;
			return;
		}
		else if( tid == ListInfo::TOP_FACTION_NIMBUS )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_nimbus = rank;
			return;
		}
		else if( tid == ListInfo::TOP_FACTION_ACTIVITY)
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_act = rank;
			return;
		}
		else if( tid >= ListInfo::TOP_FACTION_CREDIT_START 
				&& tid < ListInfo::TOP_FACTION_CREDIT_START + ListInfo::AREA_NUM )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_credits[tid-ListInfo::TOP_FACTION_CREDIT_START] = rank;
			return;
		}
		else if( tid >= ListInfo::TOP_FACTION_CREDIT_START2 
				&& tid < ListInfo::TOP_FACTION_CREDIT_START2 + ListInfo::AREA_NUM2 )
		{
			FactionMap::iterator it = faction_map.find(id);
			if( it != faction_map.end() )
				it->second.oldrank_credits[tid-ListInfo::TOP_FACTION_CREDIT_START2+ListInfo::AREA_NUM] = rank;
			return;
		}
		else if( tid >= ListInfo::TOP_FAMILY_TASK_START 
				&& tid < ListInfo::TOP_FAMILY_TASK_START + ListInfo::TASK_NUM )
		{
			FamilyMap::iterator it = family_map.find(id);
			if( it != family_map.end() )
				it->second.oldrank_records[tid-ListInfo::TOP_FAMILY_TASK_START] = rank;
			return;
		}
		else if( tid >= ListInfo::TOP_PERSONAL_DEITY_LEVEL_START
			&& tid < ListInfo::TOP_PERSONAL_DEITY_LEVEL_START + ListInfo::DEITY_NUM)
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_deity_level[tid - ListInfo::TOP_PERSONAL_DEITY_LEVEL_START] = rank;
			return;
		}
		else if( tid == ListInfo::TOP_TOWERRAID_LEVEL )
		{
			RoleMap::iterator it = role_map.find(id);
			if( it != role_map.end() )
				it->second.oldrank_tower = rank;
			return;
		}
	}
	// for test
	void Dump()
	{
		printf("rolemap size %d\n", role_map.size());
		for(RoleMap::const_iterator it = role_map.begin(), ie = role_map.end(); it != ie; ++it)
		{
			const TRoleData &roledata = (*it).second;
			printf("role id %d, level %d, exp %lld, money %u towerlevel %d towertime %d, rep %d\tcredits:",
					(*it).first, roledata.level, roledata.exp, roledata.money, roledata.reputation, roledata.tower_level, roledata.total_tower_time);
			for(int j = 0; j < ListInfo::AREA_NUM; ++j)
				printf("[%d] ", roledata.credits[j]);
			printf("\n");
		}
		printf("factionmap size %d\n", faction_map.size());
		for(FactionMap::const_iterator it = faction_map.begin(), ie = faction_map.end(); it != ie; ++it)
		{
			const TFactionData &fdata = (*it).second;
			printf("faction id %d, level %d, pop %d, money %lld, prosperity %u, nimbus %u,\tcredits:",
					(*it).first, fdata.level, fdata.members.size(), fdata.money, fdata.prosperity, fdata.nimbus);
			for(int j = 0; j < ListInfo::AREA_NUM; ++j)
				printf("[%lld] ", fdata.credits[j]);
			printf("\n");
		}
		printf("crssvrteams size %d\n", crssvrteams_map.size());
		for(CrssvrTeamsMap::const_iterator it = crssvrteams_map.begin(), ie = crssvrteams_map.end(); it != ie; ++it)
		{
			const TCrssvrTeamsData &fdata = (*it).second;
			printf("crssvrteams id %d, score %d\t",
					(*it).first, fdata.crssvrteams_score);
			printf("\n");
		}
	
		printf("oldtoptablemap size %d\n", old_toptable_map.size());
		for(ToptableMap::const_iterator it = old_toptable_map.begin(), ie = old_toptable_map.end();it!=ie;++it)
		{
			const GTopTable &fdata = (*it).second;
			printf("toptable id %d\n", (*it).first);
			for(size_t j = 0; j < fdata.items.size(); ++j)
			{
				printf("\t\tid %d, name [%.*s], oldrank %d, value %lld, cls %d\n",
						fdata.items[j].id,
						fdata.items[j].name.size(),
						(char *)fdata.items[j].name.begin(),
						fdata.items[j].oldrank,
						fdata.items[j].value,
						fdata.items[j].occupation );
			}
			printf("\n");
		}
	}
};


struct NewToptable
{
	TDataBase *tdatabase;
	ToptableMap toptable_map;
	Thread::Mutex locker;
	//
	struct PersonalLevelKey
	{
		int level;
		int64_t exp;
		unsigned short oldrank;
		unsigned int id;
		PersonalLevelKey() { }
		PersonalLevelKey(unsigned int _id, int _level, int64_t _exp, unsigned short _oldrank)
		{
			level = _level;
			exp = _exp;
			oldrank = _oldrank;
			id = _id;
		}
		bool operator<(const PersonalLevelKey &rhs) const
		{
			return level < rhs.level ||
				level == rhs.level && exp < rhs.exp ||
				level == rhs.level && exp == rhs.exp && oldrank > rhs.oldrank;
		}
		bool operator==(const PersonalLevelKey &rhs) const
		{
			return level == rhs.level && exp == rhs.exp && oldrank == rhs.oldrank;	
		}
		inline int64_t GetValue() const { return level; }
	};
	struct TowerLevelKey:public PersonalLevelKey
	{
		bool operator<(const TowerLevelKey &rhs) const
		{
			return level < rhs.level ||
				level == rhs.level && exp > rhs.exp ||
				level == rhs.level && exp == rhs.exp && oldrank > rhs.oldrank;
		}
		TowerLevelKey(unsigned int _id, int _tower_level, int _total_tower_time, unsigned short _oldrank)
			:PersonalLevelKey(_id,_tower_level,_total_tower_time,_oldrank){}
		inline int64_t GetValue() const { 
		//	printf("towertable level %d time%lld\n", level, exp );
			return ((int64_t)level<<32) + exp; 
		}
	};
	typedef PersonalLevelKey DeityLevelKey;
	struct FactionLevelKey
	{
		char level;
		unsigned int total_level;
		unsigned short oldrank;
		unsigned int id;
		FactionLevelKey() { }
		FactionLevelKey(unsigned int _id, char _level, unsigned int _total_level, unsigned short _oldrank)
		{
			level = _level;
			total_level = _total_level;
			oldrank = _oldrank;
			id = _id;
		}
		bool operator<(const FactionLevelKey &rhs) const
		{
			return level < rhs.level ||
				level == rhs.level && total_level < rhs.total_level ||
				level == rhs.level && total_level == rhs.total_level && oldrank > rhs.oldrank;
		}
		bool operator==(const FactionLevelKey &rhs) const
		{
			return level == rhs.level && total_level == rhs.total_level && oldrank == rhs.oldrank;	
		}
		inline int64_t GetValue() const { return level; }
	};
	struct PersonalMoneyKey
	{
		unsigned int money;
		unsigned short oldrank;
		unsigned int id;
		PersonalMoneyKey() { }
		PersonalMoneyKey(unsigned int _id, int _money, unsigned short _oldrank)
		{
			money = _money;
			oldrank = _oldrank;
			id = _id;
		}
		bool operator<(const PersonalMoneyKey &rhs) const
		{
			return money < rhs.money ||
				money == rhs.money && oldrank > rhs.oldrank;
		}
		bool operator==(const PersonalMoneyKey &rhs) const
		{
			return money == rhs.money && oldrank == rhs.oldrank;	
		}
		inline int64_t GetValue() const { return money; }
	};
	struct FactionMoneyKey
	{
		int64_t money;
		unsigned short oldrank;
		unsigned int id;
		FactionMoneyKey() { }
		FactionMoneyKey(unsigned int _id, int64_t _money, unsigned short _oldrank)
		{
			money = _money;
			oldrank = _oldrank;
			id = _id;
		}
		bool operator<(const FactionMoneyKey &rhs) const
		{
			return money < rhs.money ||
				money == rhs.money && oldrank > rhs.oldrank;
		}
		bool operator==(const FactionMoneyKey &rhs) const
		{
			return money == rhs.money && oldrank == rhs.oldrank;	
		}
		inline int64_t GetValue() const { return money; }
	};

	struct CharmKey
	{
		int id;
		unsigned short oldrank;
		int charm;

		CharmKey(unsigned int _id, int _charm, int _oldrank):id(_id), oldrank(_oldrank), charm(_charm) { }

		bool operator < (const CharmKey &rhs) const
		{
			return charm<rhs.charm || (charm==rhs.charm && oldrank>rhs.oldrank);
		}

		inline int64_t GetValue() const { return charm; }
	};
	struct CrssvrTeamsScoreKey
	{
		unsigned int score;
		unsigned short oldrank;
		unsigned int id;
		CrssvrTeamsScoreKey() { }
		CrssvrTeamsScoreKey(unsigned int _id, int _score, unsigned short _oldrank)
		{
			score = _score;
			oldrank = _oldrank;
			id = _id;
		}
		bool operator<(const CrssvrTeamsScoreKey &rhs) const
		{
			return score < rhs.score ||
				score == rhs.score && oldrank > rhs.oldrank;
		}
		bool operator==(const CrssvrTeamsScoreKey &rhs) const
		{
			return score == rhs.score && oldrank == rhs.oldrank;	
		}
		inline int64_t GetValue() const { return score; }
	};

	template<class Key, int maxsize>
	struct SortQueue
	{
		std::multiset<Key> queue;
		void Push(const Key &key)
		{
			queue.insert(key);
			if( queue.size() > maxsize )
				queue.erase(queue.begin());
		}
		void Build(ToptableMap &map, int tid)
		{
			GTopTable newtable;
			newtable.id = tid;
			newtable.weekly = g_bWeekly ? 1 : 0;
			for(typename std::multiset<Key>::const_reverse_iterator it = queue.rbegin(), ie = queue.rend();
				it != ie; ++it)
			{
				GTopItem item;
				item.id = (*it).id;
				item.value = (*it).GetValue();
				item.oldrank = (*it).oldrank;
				newtable.items.push_back(item);

				GTopDetail detail;
				detail.id = (*it).id;
				newtable.items_detail.push_back(detail);
				if(tid == 96)
				printf("towertable value%lld\n", item.value );
			
			}
			map[tid] = newtable;
			printf("Build toptable %d count %d\n", tid, newtable.items_detail.size());	
		}
	};

	std::map<int, bool> write2server_map;
	//
	NewToptable(TDataBase *_tdatabase) : tdatabase(_tdatabase), locker("NewToptable::locker") { }
	bool cultivation_judge(short cultivation,int index)
	{
		if(index==0)
			return true;
		else if(index<=ListInfo::DEITY_NUM && (cultivation&(0x1<<(index-1))))
			return true;
		return
			false;
	}
		
	void Update()
	{
		SortQueue<PersonalLevelKey, ListInfo::PERSONAL_LIST_SIZE> personal_level_queue;
		SortQueue<PersonalMoneyKey, ListInfo::PERSONAL_LIST_SIZE> personal_money_queue;
		SortQueue<PersonalMoneyKey, ListInfo::PERSONAL_LIST_SIZE> personal_reputation_queue;
		SortQueue<PersonalMoneyKey, ListInfo::PERSONAL_LIST_SIZE> personal_credits_queue[ListInfo::AREA_TOTAL];

		SortQueue<FactionLevelKey, ListInfo::FACTION_LIST_SIZE> faction_level_queue;
		SortQueue<FactionMoneyKey, ListInfo::FACTION_LIST_SIZE> faction_money_queue;
		SortQueue<FactionMoneyKey, ListInfo::FACTION_LIST_SIZE> faction_population_queue;
		SortQueue<FactionMoneyKey, ListInfo::FACTION_LIST_SIZE> faction_prosperity_queue;
		SortQueue<FactionMoneyKey, ListInfo::FACTION_LIST_SIZE> faction_nimbus_queue;
		SortQueue<FactionMoneyKey, ListInfo::FACTION_ACT_LIST_SIZE> faction_act_queue;
		SortQueue<FactionMoneyKey, ListInfo::FACTION_LIST_SIZE> faction_credits_queue[ListInfo::AREA_TOTAL];

		SortQueue<FactionMoneyKey, ListInfo::FAMILY_LIST_SIZE> family_records_queue[ListInfo::TASK_NUM];

		SortQueue<CharmKey, ListInfo::CHARM_LIST_SIZE> personal_charm_queue;
		SortQueue<CharmKey, ListInfo::CHARM_LIST_SIZE> friend_charm_queue;
		SortQueue<CharmKey, ListInfo::CHARM_LIST_SIZE> sect_charm_queue;
		SortQueue<CharmKey, ListInfo::CHARM_LIST_SIZE> faction_charm_queue;

		SortQueue<DeityLevelKey, ListInfo::PERSONAL_LIST_SIZE> personal_deity_level_queue[ListInfo::DEITY_NUM];
		SortQueue<TowerLevelKey, ListInfo::PERSONAL_LIST_SIZE> personal_tower_level_queue;
		
		SortQueue<CrssvrTeamsScoreKey, ListInfo::PERSONAL_LIST_SIZE> crssvrteams_score_queue;
		
		const RoleMap &rolemap = tdatabase->role_map;
		for(RoleMap::const_iterator it = rolemap.begin(), ie = rolemap.end(); it != ie; ++it)
		{
			unsigned int roleid = (*it).first;
			const TRoleData &roledata = (*it).second;

			personal_level_queue.Push(
				PersonalLevelKey(roleid, roledata.level, roledata.exp, roledata.oldrank_level));
			personal_money_queue.Push(
				PersonalMoneyKey(roleid, roledata.money/ListInfo::MIN_LIST_MONEY, roledata.oldrank_money));
			personal_reputation_queue.Push(
				PersonalMoneyKey(roleid, roledata.reputation, roledata.oldrank_reputation));

			for(int i=0; i< ListInfo::AREA_TOTAL; ++i)
			{
				personal_credits_queue[i].Push(
					PersonalMoneyKey(roleid, roledata.credits[i] , roledata.oldrank_credits[i]));
			}

			for(int i=0; i< ListInfo::DEITY_NUM; ++i)
			{
				if(roledata.deity_level && cultivation_judge(roledata.cultivation,i))
				{
					personal_deity_level_queue[i].Push(
						DeityLevelKey(roleid, roledata.deity_level, roledata.deity_exp,\
						       	roledata.oldrank_deity_level[i]));
				}
			}

			personal_charm_queue.Push(
				CharmKey(roleid, roledata.charm_personal, roledata.oldrank_charm_personal));
			friend_charm_queue.Push(
				CharmKey(roleid, roledata.charm_friend, roledata.oldrank_charm_friend));
			sect_charm_queue.Push(
				CharmKey(roleid, roledata.charm_sect, roledata.oldrank_charm_sect));

			if(roledata.total_tower_time>0)
			{
				personal_tower_level_queue.Push(
				TowerLevelKey(roleid, roledata.tower_level, roledata.total_tower_time,\
					roledata.oldrank_tower));
			}

		}
		
		const FactionMap &factionmap = tdatabase->faction_map;
		for(FactionMap::const_iterator it = factionmap.begin(), ie = factionmap.end(); it != ie; ++it)
		{
			unsigned int fid = (*it).first;
			const TFactionData &fdata = (*it).second;

			faction_level_queue.Push(
				FactionLevelKey(fid, fdata.level, fdata.total_level, fdata.oldrank_level));
			faction_money_queue.Push(
				FactionMoneyKey(fid, fdata.money/ListInfo::MIN_LIST_MONEY, fdata.oldrank_money));
			faction_population_queue.Push(
				FactionMoneyKey(fid, fdata.members.size(), fdata.oldrank_population));
			faction_prosperity_queue.Push(
				FactionMoneyKey(fid, fdata.prosperity, fdata.oldrank_prosperity));
			faction_nimbus_queue.Push(
				FactionMoneyKey(fid, fdata.nimbus, fdata.oldrank_nimbus));

			if (fdata.activity > 0) //只有活跃度大于 0 才参与排榜
			{
				faction_act_queue.Push(
						FactionMoneyKey(fid, fdata.activity, fdata.oldrank_act));
			}

			for(int i=0; i< ListInfo::AREA_TOTAL; ++i)
			{
				faction_credits_queue[i].Push(
					FactionMoneyKey(fid, fdata.credits[i] , fdata.oldrank_credits[i]));
			}

			faction_charm_queue.Push(
				CharmKey(fid, fdata.charm, fdata.oldrank_charm));
		}

		const FamilyMap &familymap = tdatabase->family_map;
		for(FamilyMap::const_iterator it = familymap.begin(), ie = familymap.end(); it != ie; ++it)
		{
			unsigned int fid = (*it).first;
			const TFamilyData &fdata = (*it).second;

			for(int i=0; i< ListInfo::TASK_NUM; ++i)
			{
				family_records_queue[i].Push(
					FactionMoneyKey(fid, fdata.records[i] , fdata.oldrank_records[i]));
			}
		}

		const CrssvrTeamsMap &crssvrteamsmap = tdatabase->crssvrteams_map;
		printf("crssvrteams_map size %d\n", crssvrteamsmap.size());
		for(CrssvrTeamsMap::const_iterator it = crssvrteamsmap.begin(), ie = crssvrteamsmap.end(); it != ie; ++it)
		{
			unsigned int fid = (*it).first;
			const TCrssvrTeamsData &fdata = (*it).second;

			crssvrteams_score_queue.Push(
					CrssvrTeamsScoreKey(fid, fdata.crssvrteams_score, fdata.oldrank_crssvrteams_score));
		}

		printf("crssvrteams_score_queue size %d\n", crssvrteams_score_queue.queue.size());
		crssvrteams_score_queue.Build(toptable_map, ListInfo::TOP_CRSSVRTEAMS_SCORE);
		personal_level_queue.Build(toptable_map, ListInfo::TOP_PERSONAL_LEVEL);
		personal_money_queue.Build(toptable_map, ListInfo::TOP_PERSONAL_MONEY);
		personal_reputation_queue.Build(toptable_map, ListInfo::TOP_PERSONAL_REPUTATION);
		for(int i=0; i< ListInfo::AREA_NUM; ++i)
			personal_credits_queue[i].Build(toptable_map, ListInfo::TOP_PERSONAL_CREDIT_START + i);
		for(int i=0; i< ListInfo::AREA_NUM2; ++i)
			personal_credits_queue[ListInfo::AREA_NUM+i].Build(toptable_map, ListInfo::TOP_PERSONAL_CREDIT_START2 + i);
		for(int i=0; i< ListInfo::DEITY_NUM; ++i)
			personal_deity_level_queue[i].Build(toptable_map, ListInfo::TOP_PERSONAL_DEITY_LEVEL_START + i);
		printf("personal_tower_level_queue size %d\n", personal_tower_level_queue.queue.size());
		personal_tower_level_queue.Build(toptable_map, ListInfo::TOP_TOWERRAID_LEVEL);
		personal_charm_queue.Build(toptable_map, ListInfo::TOP_PERSONAL_CHARM);
		friend_charm_queue.Build(toptable_map, ListInfo::TOP_FRIEND_CHARM);
		sect_charm_queue.Build(toptable_map, ListInfo::TOP_SECT_CHARM);
		faction_charm_queue.Build(toptable_map, ListInfo::TOP_FACTION_CHARM);

		faction_level_queue.Build(toptable_map, ListInfo::TOP_FACTION_LEVEL);
		faction_money_queue.Build(toptable_map, ListInfo::TOP_FACTION_MONEY);
		faction_population_queue.Build(toptable_map, ListInfo::TOP_FACTION_POPULATION);
		faction_prosperity_queue.Build(toptable_map, ListInfo::TOP_FACTION_PROSPERITY);
		faction_nimbus_queue.Build(toptable_map, ListInfo::TOP_FACTION_NIMBUS);
		faction_act_queue.Build(toptable_map, ListInfo::TOP_FACTION_ACTIVITY);
		for(int i=0; i< ListInfo::AREA_NUM; ++i)
			faction_credits_queue[i].Build(toptable_map, ListInfo::TOP_FACTION_CREDIT_START + i);
		for(int i=0; i< ListInfo::AREA_NUM2; ++i)
			faction_credits_queue[ListInfo::AREA_NUM+i].Build(toptable_map, ListInfo::TOP_FACTION_CREDIT_START2 + i);

		for(int i=0; i< ListInfo::TASK_NUM; ++i)
			family_records_queue[i].Build(toptable_map, ListInfo::TOP_FAMILY_TASK_START + i);
		printf("toptable_map size %d\n", toptable_map.size());
	}
	void FillDetail()
	{
		try
		{
			StorageEnv::Storage * pcrssvrteams = StorageEnv::GetStorage("crssvrteams");

			StorageEnv::AtomTransaction txn;
			try
			{
				for(ToptableMap::iterator it = toptable_map.begin(), ie = toptable_map.end(); 
					it != ie; ++it)
				{
					GTopTable &toptable = (*it).second;
					int tid = (*it).first;
					printf("FillDetail tid %d, items_detail.size=%d\n", tid, toptable.items_detail.size());

					for(std::vector<GTopDetail>::iterator it2 = toptable.items_detail.begin();
						it2 != toptable.items_detail.end();)
					{
						GTopDetail &topdetail = (*it2);
						try 
						{
							if( ListInfo::IsCrssvrTeams(tid) )
							{
								Marshal::OctetsStream key;
								key << topdetail.id;
								GCrssvrTeamsData finfo;

								Marshal::OctetsStream(pcrssvrteams->find( key, txn )) >> finfo;

								//std::vector<GCrssvrTeamsRole> final_roles;

								std::vector<GCrssvrTeamsRole>& roles = finfo.roles;
								/*std::vector<GCrssvrTeamsRole>::iterator it3, it3_end = roles.end();
								for(it3 = roles.begin(); it3 != it3_end; ++it3)
								{
									final_roles.push_back((*it3).roleid);
								}*/
								Marshal::OctetsStream top_os;
								top_os << roles << finfo.info.zoneid;
								topdetail.content = top_os;

								topdetail.value1 = finfo.info.battle_wins;
								topdetail.value2 = finfo.info.battle_losses;
								topdetail.value3 = finfo.info.battle_draws;
								printf("FillDetail tid %d, value1=%d, value2=%d,value3=%d\n", tid, topdetail.value1, topdetail.value2, topdetail.value3);
								//topdetail.content = Octets((char*)&final_roles, sizeof(int)*final_roles.size());
							}

							++it2;
	
						}
						catch( ... )
						{
							printf("error here, topdetail.id=%d\n", topdetail.id);
							it2 = toptable.items_detail.erase(it2);
						}

					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log(LOG_ERR, "read base or factioninfo error");
		}

	}
	void FillName()
	{
		typedef std::map<unsigned int, unsigned char> RoleClsCache;
		static RoleClsCache role_cls_cache;
		typedef std::map<unsigned int, Octets> RoleNameCache;
		static RoleNameCache role_name_cache;
		typedef std::map<unsigned int, Octets> FactionNameCache;
		static FactionNameCache faction_name_cache;
		typedef std::map<unsigned int, Octets> FamilyNameCache;
		static FamilyNameCache family_name_cache;
		typedef std::map<unsigned int, Octets> CrssvrTeamsNameCache;
		static CrssvrTeamsNameCache crssvrteams_name_cache;

		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			StorageEnv::Storage * pcrssvrteams = StorageEnv::GetStorage("crssvrteams");

			StorageEnv::AtomTransaction txn;
			try
			{
				for(ToptableMap::iterator it = toptable_map.begin(), ie = toptable_map.end(); 
					it != ie; ++it)
				{
					GTopTable &toptable = (*it).second;
					int tid = (*it).first;
					for(std::vector<GTopItem>::iterator it2 = toptable.items.begin();
						it2 != toptable.items.end();)
					{
						GTopItem &topitem = (*it2);
						try {
							if( ListInfo::IsPersonal(tid) || tid==ListInfo::TOP_PERSONAL_CHARM \
								|| tid==ListInfo::TOP_FRIEND_CHARM || tid==ListInfo::TOP_SECT_CHARM \
								||(tid>=ListInfo::TOP_PERSONAL_DEITY_LEVEL_START && tid<ListInfo::TOP_PERSONAL_DEITY_LEVEL_START + ListInfo::DEITY_NUM || tid==ListInfo::TOP_TOWERRAID_LEVEL))
							{
								RoleClsCache::const_iterator ita = role_cls_cache.find(topitem.id);
								if( ita != role_cls_cache.end() )
								{
									topitem.occupation = (*ita).second;
									topitem.name = role_name_cache[topitem.id];
								}
								else
								{
									Marshal::OctetsStream key;
									key << topitem.id;
									GRoleBase	base;
									Marshal::OctetsStream(pbase->find( key, txn )) >> base;
									topitem.name = base.name;
									role_name_cache[topitem.id] = topitem.name;

									Marshal::OctetsStream key2;
									key2 << RoleId(topitem.id);
									GRoleStatus	status;
									Marshal::OctetsStream(pstatus->find( key2, txn )) >> status;
									topitem.occupation = status.occupation;
									role_cls_cache[topitem.id] = topitem.occupation;
								}
							}
							else if( ListInfo::IsFaction(tid) || tid==ListInfo::TOP_FACTION_CHARM)
							{
								FactionNameCache::const_iterator ita = faction_name_cache.find(topitem.id);
								if( ita != faction_name_cache.end() )
								{
									topitem.name = faction_name_cache[topitem.id];
								}
								else
								{
									Marshal::OctetsStream key;
									key << FactionId(topitem.id);
									GFactionInfo finfo;
									Marshal::OctetsStream(pfaction->find( key, txn )) >> finfo;
									topitem.name = finfo.name;
									faction_name_cache[topitem.id] = topitem.name;
								}
							}
							else if( ListInfo::IsFamily(tid) )
							{
								FamilyNameCache::const_iterator ita = family_name_cache.find(topitem.id);
								if( ita != family_name_cache.end() )
								{
									topitem.name = family_name_cache[topitem.id];
								}
								else
								{
									Marshal::OctetsStream key;
									key << topitem.id;
									GFamily finfo;
									Marshal::OctetsStream(pfamily->find( key, txn )) >> finfo;
									topitem.name = finfo.name;
									family_name_cache[topitem.id] = topitem.name;
								}
							}
							else if( ListInfo::IsCrssvrTeams(tid) )
							{
								CrssvrTeamsNameCache::const_iterator ita = crssvrteams_name_cache.find(topitem.id);
								if( ita != crssvrteams_name_cache.end() )
								{
									topitem.name = crssvrteams_name_cache[topitem.id];
								}
								else
								{
									Marshal::OctetsStream key;
									key << topitem.id;
									GCrssvrTeamsData finfo;

									Marshal::OctetsStream(pcrssvrteams->find( key, txn )) >> finfo;
									topitem.name = finfo.info.name;
									crssvrteams_name_cache[topitem.id] = topitem.name;


								}
							}

							++it2;
						}catch( ... )
						{
							printf("error here, roleid=%d\n", topitem.id);
							it2 = toptable.items.erase(it2);
						}
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log(LOG_ERR, "read base or factioninfo error");
		}
	}
	void FillMaster()
	{
		typedef std::map<unsigned int, unsigned int> MasterCache;
		static MasterCache faction_master_cache;
		static MasterCache family_master_cache;
		for(ToptableMap::iterator it = toptable_map.begin(), ie = toptable_map.end(); it != ie; ++it)
		{
			int tid = (*it).first;
			if( ListInfo::IsFaction(tid) || tid==ListInfo::TOP_FACTION_CHARM )
			{
				GTopTable &toptable = (*it).second;
				for(std::vector<GTopItem>::iterator it2 = toptable.items.begin(),
					ie2 = toptable.items.end(); it2 != ie2; ++it2)
				{
					GTopItem &topitem = (*it2);
					std::map<unsigned int, unsigned int>::const_iterator itt 
						= faction_master_cache.find(topitem.id);
					if( itt == faction_master_cache.end() )
					{
						topitem.occupation = tdatabase->faction_map[topitem.id].master;
						faction_master_cache[topitem.id] = topitem.occupation;
					}
					else
						topitem.occupation = (*itt).second;
				}
			}
			else if( ListInfo::IsFamily(tid) )
			{
				GTopTable &toptable = (*it).second;
				for(std::vector<GTopItem>::iterator it2 = toptable.items.begin(),
					ie2 = toptable.items.end(); it2 != ie2; ++it2)
				{
					GTopItem &topitem = (*it2);
					std::map<unsigned int, unsigned int>::const_iterator itt 
						= family_master_cache.find(topitem.id);
					if( itt == family_master_cache.end() )
					{
						topitem.occupation = tdatabase->family_map[topitem.id].master;
						family_master_cache[topitem.id] = topitem.occupation;
					}
					else
						topitem.occupation = (*itt).second;
				}
			}
		}
	}

	void WriteToStdout()
	{		
		printf("toptablemap size %d\n", toptable_map.size());
		for(ToptableMap::const_iterator it = toptable_map.begin(), ie = toptable_map.end();it!=ie;++it)
		{
			const GTopTable &fdata = (*it).second;
			printf("toptable id %d\n", (*it).first);
			for(size_t j = 0; j < fdata.items.size(); ++j)
			{
				printf("\t\tid %d, name [%.*s], oldrank %d, value %lld, cls %d\n",
					fdata.items[j].id,
					fdata.items[j].name.size(),
					(char *)fdata.items[j].name.begin(),
					fdata.items[j].oldrank,
					fdata.items[j].value,
					fdata.items[j].occupation );
			}
			for(size_t j = 0; j < fdata.items_detail.size(); ++j)
			{
				printf("\t\tdetail.id %d, content [%.*s]\n",
					fdata.items_detail[j].id,
					fdata.items_detail[j].content.size(),
					(char *)fdata.items_detail[j].content.begin());
			}

			printf("\n");
		}
	}
	void WriteToDB()
	{
		try
		{
			StorageEnv::Storage * ptoptable = StorageEnv::GetStorage("toptable");
			StorageEnv::AtomTransaction txn;
			try
			{
				for(ToptableMap::const_iterator it = toptable_map.begin(), ie = toptable_map.end(); 
					it != ie; ++it)
				{
					const GTopTable &toptable = (*it).second;
					ptoptable->insert( 
						Marshal::OctetsStream() << toptable.id, Marshal::OctetsStream() << toptable, txn );
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
   	        Log::log(LOG_ERR, "Toptable write, error");
		}
	}
	void ServerRet(int tid, int retcode)
	{
		Thread::Mutex::Scoped l(locker);
		Log::log(LOG_INFO, "toplist send toptable %d ret %d\n", tid, retcode);
		if( retcode == 0 )
		{
			write2server_map.erase(tid);
		}	
		else
		{
			std::map<int,bool>::iterator it = write2server_map.find(tid);
			if( it != write2server_map.end() )
				it->second = true;
		}
	}
	void PrepareWriteAll()
	{
		for(ToptableMap::const_iterator it = toptable_map.begin(), ie = toptable_map.end(); 
			it != ie; ++it)
		{
			int tid = (*it).first;
			write2server_map[tid] = true;
		}
	}
	void PrepareWrite(int tid)
	{
		for(ToptableMap::const_iterator it = toptable_map.begin(), ie = toptable_map.end(); 
			it != ie; ++it)
		{
			if(tid == (*it).first)
			{
				LOG_TRACE("PrepareWrite, tid=%d", tid);
				write2server_map[tid] = true;
				break;
			}
		}
	}

	void WriteToDBServer()
	{
		printf("NewToptable writetodbserver\n");
		static int sleep_time = 0;
		{
			Thread::Mutex::Scoped l(locker);
			if( write2server_map.size() == 0 || sleep_time >= 300 )
			{
				Log::log(LOG_INFO, "toplist quit map.size %d sleep_time %d\n", write2server_map.size(), sleep_time);
				s_policy.SetState(Thread::Pool::stateQuitAtOnce);
				PollIO::WakeUp();
			}
			int send_count = 2;
			for(std::map<int,bool>::iterator it = write2server_map.begin(), ie = write2server_map.end();
					it != ie; ++it)
			{
				if( ! (*it).second ) continue;
				int tid = (*it).first;
				ToptableMap::iterator it2 = toptable_map.find(tid);
				if( it2 == toptable_map.end() ) continue;
				DBPutTopTable* rpc=(DBPutTopTable*) Rpc::Call(RPC_DBPUTTOPTABLE, (*it2).second);
				rpc->newtoptable = this;
				rpc->tid = tid;
				bool sendok = GameDBClient::GetInstance()->SendProtocol(rpc);
				(*it).second = !sendok;
				Log::log(LOG_INFO, "send toptable %d, %s\n", tid, sendok?"success":"failed");
				if( 0 == send_count-- ) break;
			}
			sleep_time ++;
		}
	}
};

class SendTask : public Thread::Runnable
{
public:
	NewToptable *newtoptable;
	void Run()
	{
		printf("sendtask run\n");
		newtoptable->WriteToDBServer();
		Thread::HouseKeeper::AddTimerTask(this,3);
	}
};

namespace GNET
{
	void DBPutTopTable::Client(Rpc::Data *argument, Rpc::Data *result, Manager *manager, Manager::Session::ID sid)
	{
		Integer *res = (Integer *)result;
		printf("DBPutTopTable tid %d ret %d\n", tid, res->value);
		newtoptable->ServerRet(tid, res->value);
	}

	void DBPutTopTable::OnTimeout()
	{
		printf("DBPutTopTable tid %d timeout\n", tid);
		newtoptable->ServerRet(tid, -1);
	}

	void GameDBClient::OnAddSession(Session::ID sid)
	{
		{
		Thread::Mutex::Scoped l(locker_state);
		if (conn_state)
		{
			Close(sid);
			return;
		}
		conn_state = true;
		this->sid = sid;
		backoff = BACKOFF_INIT;
		}
		printf("gamedbclient onaddsession\n");
		SendTask *sendtask = new SendTask();
		sendtask->newtoptable = newtoptable;

		Timer::Update(); // PollIO::Task::Run() 正常执行才会使 Timer正常工作 这里手动Update一次避免 rpc 误报超时
		sendtask->Run(); //OnAddSession()之后必须立即调用Send()操作一次 否则有可能出现连接只发不收的情况 20120614 liuyue
		//Thread::HouseKeeper::AddTimerTask(sendtask,3/* delay */);
	}
};

void CheckFlushOneTable(int onetableid, bool& bQueryOneTable)
{
	if(onetableid >= 1 && onetableid <= ListInfo::TOP_TABLEID_MAX)
	{
		bQueryOneTable = true;
	}
	else
	{
		bQueryOneTable = false;
	}
}

int main(int argc, char *argv[])
{
	struct rlimit rl;
	rl.rlim_cur = 0x10000000;
	rl.rlim_max = 0x10000000;
	setrlimit(RLIMIT_CORE, &rl);
	if (argc < 3 || access(argv[1], R_OK) == -1 )
	{
#ifdef USE_WDB
		std::cerr << "Toplist version " << XMLVERSION << std::endl;
		std::cerr << "Usage: " << argv[0] << " conf-file dbhomewdb [weekly]" << std::endl;
#else
		std::cerr << "Usage: " << argv[0] << " conf-file dbhome [weekly]" << std::endl;
#endif
		exit(-1);
	}
	bool bQuery = ( strcmp(argv[2], "query" ) == 0 ); 
	g_bWeekly = ( argc> 3 && strcmp(argv[3], "weekly" ) == 0 );

	bool bQueryOneTable = false;
	int onetableid = 0;

	// 周榜只刷指定榜命令 toplist toplist.conf dbhomewdb weekly onetable 1
	if(g_bWeekly && argc == 6 && strcmp(argv[4], "onetable" ) == 0)
	{
		onetableid = atoi(argv[5]);
		CheckFlushOneTable(onetableid, bQueryOneTable);
	}

	// 日榜只刷指定榜命令 toplist toplist.conf dbhomewdb onetable 1
	if((!g_bWeekly) && argc == 5 && strcmp(argv[3], "onetable" ) == 0)
	{
		onetableid = atoi(argv[4]);
		CheckFlushOneTable(onetableid, bQueryOneTable);
	}
	
	printf("g_bWeekly=%d, argc=%d, bQueryOneTable=%d, onetableid=%d\n", g_bWeekly, argc, bQueryOneTable, onetableid);
	Conf *conf = Conf::GetInstance(argv[1]);
	if( !bQuery )
	{
		if( -1 == access(argv[2], R_OK) )
		{
			std::cerr << "Bad dbhome dir" << std::endl;
			exit(-1);
		}
#ifdef USE_WDB
		conf->put("storagewdb", "homedir", argv[2]);
#else
		conf->put("storage", "homedir", argv[2]);
#endif
	}
	strcpy(conf_filename,argv[1]);
	Log::setprogname(argv[0]);

	g_now = time(NULL);

	StorageEnv::Open();
	StorageEnv::Close();
	StorageEnv::Open();
	TDataBase tdatabase;
	tdatabase.AddFilter("filter");
	tdatabase.Load();
	tdatabase.CalFactionData();
	tdatabase.FillOld();
	if( bQuery )
		tdatabase.Dump();
	NewToptable new_toptable(&tdatabase);
	new_toptable.Update();
	new_toptable.FillMaster();
	tdatabase.Clear();
	new_toptable.FillName();
	new_toptable.FillDetail();

	if( bQuery )
	{
		new_toptable.WriteToStdout();
		StorageEnv::Close();
	}
	else
	{
		if(bQueryOneTable)
			new_toptable.PrepareWrite(onetableid);
		else
			new_toptable.PrepareWriteAll();
		StorageEnv::Close();
		{
			GameDBClient *manager = GameDBClient::GetInstance();
			manager->newtoptable = &new_toptable;
			manager->SetAccumulate(atoi(conf->find(manager->Identification(), "accumulate").c_str()));
			Protocol::Client(manager);
		}
		Thread::Pool::AddTask(PollIO::Task::GetInstance());
		Thread::Pool::Run(&s_policy);
	}
	return 0;
}

