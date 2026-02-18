#ifndef __GNET_BATTLEMANAGER_H
#define __GNET_BATTLEMANAGER_H

#include <vector>
#include <map>

#include "thread.h"
#include "staticmap.h"
#include "battlecommon.h"
#include "gfieldinfo"
#include "topmanager.h"
#include "battlegetlist_re.hpp"
#include "battlegetfield_re.hpp"
#include "battlefieldinfo"

namespace GNET
{
	enum
	{
		BATTLEFIELD_TYPE_NORMAL 		= 0, //普通战场
		BATTLEFIELD_TYPE_CRSSVR 		= 1, //非匿名跨服战场
		BATTLEFIELD_TYPE_REBORN 		= 3, //飞升战场
		BATTLEFIELD_TYPE_SONJIN 		= 4, //楚汉军魂
		BATTLEFIELD_TYPE_SONJINREBORN 	= 5, //楚汉军魂（飞升）
		BATTLEFIELD_TYPE_ARENA			= 6, //演武阵
		BATTLEFIELD_TYPE_ARENAREBORN	= 7, //幻世演武阵
		BATTLEFIELD_TYPE_ANONCRSSVR		= 8, //匿名跨服战场
		BATTLEFIELD_TYPE_CHALLENGESPACE 	= 9, //鸿蒙试炼
		BATTLEFIELD_TYPE_CRSSVRTEAM 		= 10, //跨服小队pk（6vs6）
		BATTLEFIELD_TYPE_FLOW_CRSSVR		= 11, // 流水席战场
	};


	class BattleServer;
	class BattleStartNotify;
	class BattleField
	{
	public:
		typedef static_multimap<int, BattleRole> TeamType;
		typedef std::map<int, TagInfo> TagInfos;

		enum
		{
			SWITCH_TIMEOUT  = 60,
			ENTER_TIMEOUT  	= 60,
		};

		enum STATUS
		{
			ST_NONE		= 0,
			ST_QUEUEING 	= 1,
			ST_SENDSTART 	= 2,
			ST_FIGHTING 	= 3,
			ST_WAITINGEND	= 4,
		};

		enum STATUS_OPEN
		{
			OPEN_NONE	= 0,
			OPEN_UNFORMAL	= 1,
			OPEN_FORMAL	= 2,
		};

	private:
		typedef std::map<char, BattleField *> STUBMAP;	
		static STUBMAP stubmap;
	protected:
		BattleField(char _field_type)
			: map_id(0), level_min(0), level_max(0), server(NULL), counter(0), b_open(false)
			, open_status(OPEN_NONE), field_type(_field_type), fighting_tag(0), starttime(0), endtime(0)
		{
			if (GetStub(field_type) == NULL)
				stubmap[field_type] = this;
		}
		virtual ~BattleField() { }

		static BattleField *GetStub(char field_type) 
		{ 
			if (stubmap.find(field_type) == stubmap.end())
				return NULL;
			return stubmap[field_type];
		}

		virtual BattleField *Clone() = 0;

	public:
		static BattleField *Create(char field_type)
		{
			BattleField *stub = GetStub(field_type);
			return stub==NULL ? NULL : stub->Clone();
		}

		virtual void Init(const BattleFieldInfo &info, BattleServer *server) = 0;
		virtual int 	JoinTeam(BattleRole & role, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata) = 0;
		virtual int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata) = 0;
		virtual int 	LeaveTeam(int roleid) = 0;
		virtual void 	GetMember(unsigned char team, std::vector<GBattleRole> & members) = 0;
		virtual int		GetRedNumber() = 0;
		virtual int 	GetBlueNumber() = 0;
		virtual int  	OnBattleEnter(int roleid, bool b_gm) = 0;
		virtual int  	OnBattleEnterFail(int roleid);
		virtual void	OnBattleEnd(int tag) = 0;
		virtual void 	OnDBBattleJoin(int retcode, int roleid) {}
		virtual void 	OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify) = 0;
		virtual bool 	CheckFormalOpenCond(const GTopTable & table, int level_min) = 0;
		virtual bool 	CheckUnFormalOpenCond(const GTopTable & table, int level_min) = 0;
		virtual bool 	TryBeginQueue() = 0;
		virtual void 	ServerAppointTeam(TeamType &candiates){}
		virtual bool 	IsRoleInTeam(int roleid) = 0;
		virtual bool 	IsServerAppointTeam() { return false; }
		virtual size_t	GetTeamLimit()	{ return 0; }
		virtual bool	TimePolicy() { return true; }
		virtual int		GetFightingTime() = 0;
		virtual int 	GetCooldownTime() { return 0; }
		virtual int 	GetEnterMaxPerDay() { return -1; }

		int		TopPolicy();
		int 	LeaveGame(int roleid);
		//void 	Broadcast(Protocol & data, unsigned int & localsid);
		void 	Broadcast(BattleStartNotify & data, unsigned int & localsid, int tag = 0);
		void 	StartBattleError();
		void 	SendStartNotify(int roleid, BattleStartNotify &notify, int team, int endtime, int tag = 0);
		void 	OnBattleStart(int retcode);
		void 	Update(time_t now, bool & b_notify);
		void 	AddTag(int tag);
		void 	OnDisconnect();
		void 	Destroy() { delete this; }
		char 	GetFieldType() { return field_type; }

		bool	JoinPolicy(int level)
		{
			if (level >= level_min && level <= level_max)
				return true;
			return false;
		}

		
		int map_id;
		int level_min;
		int level_max;
		BattleServer * server;
		TeamType team_red_fighting;
		TeamType team_blue_fighting;
		TeamType team_looker_fighting;
		TeamType team_red;
		TeamType team_blue;
		//竞技战场报名需要扣物品，在访问DB前将玩家加入此列表
		//进入成功后再真正加入到team_red中，防止报名人数超上限
		TeamType potential_team_red;
		int counter;
		TagInfos taginfos;
		bool	b_open;
		short  	open_status;
		char	field_type;
		int 	fighting_tag;
		int	starttime;
		int	endtime;

		friend class BattleServer;
	};

	enum
	{
		NORMAL_FIGHTING_TIME   = 2700,
		CRSSVR_FIGHTING_TIME   = 1800,
		SONJIN_FIGHTING_TIME   = 3600,	//宋金战场持续1小时
		ARENA_FIGHTING_TIME    = 3600,	//竞技场持续1小时
		CHALLENGESPACE_FIGHTING_TIME = 1800, //挑战空间持续时间，如果GS配置文件中的战斗时间修改此处也需要同步修改
	};
	enum
	{
		NORMAL_BATTLEFIELD_START_DURATION 	= 180,
		CHALLENGESPACE_BATTLEFIELD_START_DURATION = 120,
		COOLDOWN_CHECK_MAX 					= 5,
		NORMAL_BATTLEFIELD_COOL_TIME 		= 300,
		NORMAL_BATTLEFIELD_ENTER_MAX_PERDAY = 10,
		SONJIN_BATTLEFIELD_ENTER_MAX_PERDAY = 1,
	};
	enum
	{
		TIMEOFFSET_BETWEEN_GS_DELI = 120,
	};

	class CrssvrBattleField : public BattleField
	{
		typedef std::map<int/*roleid*/, TEAM> TReEnterMap;
	protected:
		TReEnterMap reenter_map; // 重新进入集合 
	protected:
		BattleField * Clone() { return new CrssvrBattleField(*this); }
	public:
		CrssvrBattleField(char field_type) : BattleField(field_type) {} 
		void Init(const BattleFieldInfo &info, BattleServer *server);
		int 	JoinTeam(BattleRole & role, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	LeaveTeam(int roleid);
		void 	GetMember(unsigned char team, std::vector<GBattleRole> & members);
		int	GetRedNumber();
		int 	GetBlueNumber();
		int  	OnBattleEnter(int roleid, bool b_gm);
		void	OnBattleEnd(int tag);
		void 	OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify);
		bool 	CheckFormalOpenCond(const GTopTable & table, int level_min);
		bool 	CheckUnFormalOpenCond(const GTopTable & table, int level_min);
		bool 	TryBeginQueue();
		bool 	IsRoleInTeam(int roleid);
		size_t	GetTeamLimit();
		int 	GetFightingTime() { return CRSSVR_FIGHTING_TIME; }
		void 	InsertReEnter(int roleid, bool reenter, TEAM team);
		bool	CheckReEnter(int roleid, bool reenter, TEAM team);
		bool 	IsReEnter(int roleid, TEAM team);
		virtual ~CrssvrBattleField();
	};

	class CrssvrTeamBattleField : public CrssvrBattleField
	{
		int fighting_time; 		// 每场战斗持续的时间
		int maxplayer_perround; 	// 每轮允许加入的玩家数
		const static int ADD_FIGHTING_TIME = 60;
	protected:
		BattleField * Clone() { return new CrssvrTeamBattleField(*this); }
	public:
		virtual ~CrssvrTeamBattleField();
		CrssvrTeamBattleField(char field_type) : CrssvrBattleField(field_type),fighting_time(0),maxplayer_perround(0) {} 
		void Init(const BattleFieldInfo &info, BattleServer *server);
		void	OnBattleEnd(int tag);
		void 	OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify);
		bool 	CheckFormalOpenCond(const GTopTable & table, int level_min){return true;}
		bool 	CheckUnFormalOpenCond(const GTopTable & table, int level_min){return true;}
		size_t	GetTeamLimit() { return maxplayer_perround;}
		int 	GetFightingTime() { return fighting_time; }
		bool    IsRoleInTheTeam(int roleid,unsigned char team);
		static int GetQueueingTime(int totaltime){return (totaltime > 4*ADD_FIGHTING_TIME) ? (totaltime/4 - ADD_FIGHTING_TIME):0;}
	};

	class NormalBattleField : public BattleField
	{
	protected:
		BattleField * Clone() { return new NormalBattleField(*this); }
	public:
		NormalBattleField(char field_type) : BattleField(field_type) {} 
		void Init(const BattleFieldInfo &info, BattleServer *server);
		int 	JoinTeam(BattleRole & role, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	LeaveTeam(int roleid);
		void 	GetMember(unsigned char team, std::vector<GBattleRole> & members);
		int		GetRedNumber();
		int 	GetBlueNumber();
		int  	OnBattleEnter(int roleid, bool b_gm);
		void	OnBattleEnd(int tag);
		void 	OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify);
		bool 	CheckFormalOpenCond(const GTopTable & table, int level_min);
		bool 	CheckUnFormalOpenCond(const GTopTable & table, int level_min);
		bool 	TryBeginQueue();
		bool 	IsRoleInTeam(int roleid);
		size_t	GetTeamLimit();
		bool	TimePolicy();
		int 	GetFightingTime() { return NORMAL_FIGHTING_TIME; }
		int 	GetCooldownTime() { return NORMAL_BATTLEFIELD_COOL_TIME; }
		int 	GetEnterMaxPerDay() { return NORMAL_BATTLEFIELD_ENTER_MAX_PERDAY; }
	};

	class ChallengeSpaceBattleField : public BattleField
	{
	protected:
		int reborn_count_need; 		// 是否要求玩家飞升才能进入
		int fighting_time; 		// 每场战斗持续的时间
		int maxplayer_perround; 	// 每轮允许加入的玩家数
		int entermax_perday; 	 	// 玩家每天能进入挑战空间的次数
		int cooldown_time; 		// 两次进入挑战空间的时间间隔
		
		BattleField *Clone()
		{
			return new ChallengeSpaceBattleField(*this);
		}
	public:
		ChallengeSpaceBattleField(char field_type) : BattleField(field_type)
		{
		}
		void Init(const BattleFieldInfo &info, BattleServer *server);
		int JoinTeam(BattleRole &role, unsigned char team, int &starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData &syncdata);
		int JoinTeam(BattleRole &role, TeamType &team, unsigned char tid, int &_starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData &syncdata);
		int LeaveTeam(int roleid);
		void GetMember(unsigned char team, std::vector<GBattleRole> &member);
		int GetRedNumber();
		int GetBlueNumber();
		int OnBattleEnter(int roleid, bool b_gm);
		void OnBattleEnd(int tag);
		void OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify);
		bool CheckFormalOpenCond(const GTopTable &table, int level_min);
		bool CheckUnFormalOpenCond(const GTopTable &table, int level_min);
		bool TryBeginQueue();
		bool IsRoleInTeam(int roleid);
		bool TimePolicy();
		size_t GetTeamLimit()
		{
			return maxplayer_perround;
		}
		int GetFightingTime()
		{
			return fighting_time;
		}
		int GetCooldownTime()
		{
			return cooldown_time;
		}
		int GetEnterMaxPerDay()
		{
			return entermax_perday;
		}
	};

	class RebornBattleField : public BattleField
	{
	protected:
		BattleField * Clone() { return new RebornBattleField(*this); }
	public:
		RebornBattleField(char field_type) : BattleField(field_type) {} 
		void Init(const BattleFieldInfo &info, BattleServer *server);
		int 	JoinTeam(BattleRole & role, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	LeaveTeam(int roleid);
		void 	GetMember(unsigned char team, std::vector<GBattleRole> & members);
		int		GetRedNumber();
		int 	GetBlueNumber();
		int  	OnBattleEnter(int roleid, bool b_gm);
		void	OnBattleEnd(int tag);
		void 	OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify);
		bool 	CheckFormalOpenCond(const GTopTable & table, int level_min);
		bool 	CheckUnFormalOpenCond(const GTopTable & table, int level_min);
		bool 	TryBeginQueue();
		void 	ServerAppointTeam(TeamType &candiates);
		bool 	IsRoleInTeam(int roleid);
		bool 	IsServerAppointTeam() { return true; }
		size_t	GetTeamLimit();
		bool	TimePolicy();
		int 	GetFightingTime() { return NORMAL_FIGHTING_TIME; }
		int 	GetCooldownTime() { return NORMAL_BATTLEFIELD_COOL_TIME; }
		int 	GetEnterMaxPerDay() { return NORMAL_BATTLEFIELD_ENTER_MAX_PERDAY; }
	};

	class SonjinBattleField : public RebornBattleField
	{
	protected:
		BattleField * Clone() { return new SonjinBattleField(*this); }
	public:
		SonjinBattleField(char field_type) : RebornBattleField(field_type) {}
		void Init(const BattleFieldInfo &info, BattleServer *server);
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		void 	OnQueueingUpdate(time_t now, TagInfo &tag, bool &b_notify);
		bool 	TryBeginQueue();
		int  	OnBattleEnter(int roleid, bool b_gm);
		size_t	GetTeamLimit();
		bool	TimePolicy();
		int 	GetFightingTime() { return SONJIN_FIGHTING_TIME; }
		int 	GetCooldownTime() { return 0; }
		int 	GetEnterMaxPerDay() { return SONJIN_BATTLEFIELD_ENTER_MAX_PERDAY; }
		virtual bool StartPolicy();
	};

	class SonjinRebornBattleField : public SonjinBattleField
	{
	protected:
		BattleField * Clone() { return new SonjinRebornBattleField(*this); }
	public:
		SonjinRebornBattleField(char field_type) : SonjinBattleField(field_type) {}
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
	};

	class ArenaBattleField : public SonjinBattleField
	{
	protected:
		BattleField * Clone() { return new ArenaBattleField(*this); }
	public:
		ArenaBattleField(char field_type) : SonjinBattleField(field_type) {}
		int  	OnBattleEnter(int roleid, bool b_gm);
		size_t	GetTeamLimit();
		bool	TimePolicy();
		int 	GetFightingTime() { return ARENA_FIGHTING_TIME; }
		bool 	StartPolicy();
		bool 	IsServerAppointTeam() { return false; }
		bool 	IsRoleInTeam(int roleid);
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
		int 	LeaveTeam(int roleid);
		void 	OnDBBattleJoin(int retcode, int roleid);
	};

	class ArenaRebornBattleField : public ArenaBattleField
	{
	protected:
		BattleField * Clone() { return new ArenaRebornBattleField(*this); }
	public:
		ArenaRebornBattleField(char field_type) : ArenaBattleField(field_type) {}
		int 	JoinTeam(BattleRole & roleid, TeamType & team, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata);
	};
	
	typedef std::map<int /*map_id*/, BattleField *> BattleMap;
	class BattleServer
	{
	public:
		BattleServer() : gs_id(0), sid(0), starttime(0), challenge_starttime(0)
		{
		}
		~BattleServer()
		{
			BattleMap::iterator it, ie = battlemap.end();
			for (it = battlemap.begin(); it != ie; ++it)
			{
				it->second->Destroy();
			}
		}

		void 	Init(int _gs_id, int _sid, time_t starttime, const std::vector<BattleFieldInfo> &info, time_t challenge_starttime);
		void 	Update(time_t now);

		time_t 	GetStartTime() const { return starttime;}
		time_t 	GetChallengeTime() const { return challenge_starttime; }
		void 	GetMap(BattleGetList_Re & re, char battle_type);
		void 	GetFieldInfo(int map_id, int roleid, BattleGetField_Re & re);

		int 	JoinTeam(BattleRole & role, int map_id, unsigned char team, int & starttime, bool b_gm, int itemid, int item_pos, const GMailSyncData & syncdata,char &battle_type);
		int		LeaveTeam(int roleid);
		void 	OnBattleStart(int map_id, int retcode);
		void	OnBattleEnd(int map_id, int tag);
		int 	OnBattleEnter(int roleid, int map_id, int & endtime, char &battle_type, bool b_gm);
		int 	OnBattleEnterFail(int roleid, int map_id);
		void    OnLeaveGame(int map_id, int tag, int roleid, int reason);
		void	OnLogout(int roleid);
		void 	OnDBBattleJoin(int retcode, int roleid, int map_id);
		void 	OnDisconnect();
		char	GetFieldType(int mapid)
		{
			BattleMap::iterator it = battlemap.find(mapid);
			if (it==battlemap.end() || it->second==NULL)
				return -1;
			else
				return it->second->GetFieldType();
		}

		BattleField *GetBattleField(int mapid)
		{
			BattleMap::iterator it = battlemap.find(mapid);
			return (it == battlemap.end() ? NULL : it->second);
		}
		
	//private:
		friend class BattleField;
		bool 	IsRoleInTeam(int roleid);

		BattleMap battlemap;
		int gs_id;
		int sid;
		time_t starttime;
		time_t challenge_starttime;
	};

	class BattleManager : public IntervalTimer::Observer
	{
	public:
		typedef std::map<int /*gs_id*/, BattleServer> ServerMap;

		static BattleManager * GetInstance()
		{
			static BattleManager inst;
			return &inst;
		}
		bool	Initialize();
		void 	RegisterServer(int sid, int gs_id, const std::vector<BattleFieldInfo> & info);
		void 	GetMap(int roleid, char battle_type, int linksid, int localsid);
		void 	GetFieldInfo(int gs_id, int map_id, int roleid, BattleGetField_Re & re);
		int 	JoinTeam(BattleRole & role, int gs_id, int map_id, unsigned char team, int & starttime, int & cooltime, char &battle_type, bool b_gm = false, int itemid = -1, int item_pos = -1, const GMailSyncData & syncdata = GMailSyncData() );
		int 	LeaveTeam(int roleid, int gs_id);

		void 	OnBattleStart(int gs_id, int tag, int mapid, int retcode);
		void 	OnBattleEnd(int gs_id, int tag, int mapid);
		int 	OnBattleEnter(int roleid, int gs_id, int map_id, int &endtime, char &battle_type, bool b_gm = false);
		int	OnBattleEnterFail(int roleid, int gs_id, int map_id);
		void 	OnLogout(int roleid, int gs_id);
		void    OnLeaveGame(int gs_id, int map_id, int tag, int roleid, int reason);
		void 	OnDisconnect(int gs_id);
		void 	OnDBBattleJoin(int retcode, int roleid, int gs_id, int map_id);
		bool 	Update();
		time_t 	GetTime();
		int		GetTimeBase() { return t_base; }

		void 	SetRoleCoolDown(char field_type, int roleid);
		void 	DecRoleEnterCnt(char field_type, int roleid);
		int IsRoleInCoolDown(char field_type, int roleid, BattleField *field, int & remain_cooltime);
		void SetDebugMode();
	private:
		BattleManager()
			:cursor_fieldtype(0), cursor_roleid(0), t_forged(0), t_base(0)
		{
		}
		time_t AllocStartTime();
		time_t AllocChallengeTime();
	private:
		friend class BattleField;
		void 	SetForgedTime(int forgetime) { t_forged = forgetime;}

		typedef std::pair<time_t, short> RoleInfo;
		typedef std::map<int, RoleInfo> RoleTimer;
		typedef std::map<char, RoleTimer> FieldRoleTimer;
		FieldRoleTimer rolemap;
		char cursor_fieldtype;
		int cursor_roleid;
		ServerMap servers;
		int t_forged;
		int t_base;
	private:
		friend class DebugCommand;
		void 	DebugStartBattle(int gs_id, int map_id);
		void 	DebugBattleEnter(int gs_id, int map_id, int roleid);
		void 	DebugBattleJoin(int gs_id, int map_id, int roleid, unsigned char team);
		void 	DebugBattleLeave(int gs_id, int roleid);
		void 	DebugList(int gs_id);
	};
} // namespace GNET

#endif

