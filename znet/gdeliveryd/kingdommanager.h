#ifndef __GNET_KINGDOMMANAGER_H
#define __GNET_KINGDOMMANAGER_H

#include "gkingdominfo"
#include "kingdombattleinfo"
#include "mapuser.h"

#include "kingdominformation"
#include "mapgameattr.h"
#include "kingdomclientinfo"
namespace GNET
{

	void LoadRefFaction(int fid);
	void ReleaseFaction(int fid);
	class KingdomGetTitle_Re;
	enum
	{	
		KINGDOM_FIELD_NULL = 0,
		KINGDOM_FIELD_MAIN = 1,
		KINGDOM_FIELD_SUB = 2,
		KINGDOM_FIELD_COUNT,
	};
	class KingdomBattleField //国战战场信息
	{
	private:
		const char fieldtype; //1 主战场 2 辅战场
		const int gsid;
		const int tagid;
		const bool dominant; //是否为主导战场 战场的进展是否对全局有主导影响
		int b_state;

		typedef std::map<char/*stub type*/, KingdomBattleField *> STUBMAP;
		static STUBMAP stubmap;
		static KingdomBattleField * GetStub(char _type)
		{
			STUBMAP::iterator it = stubmap.find(_type);
			if (it == stubmap.end())
				return NULL;
			else
				return it->second;
		}
		virtual KingdomBattleField * Clone(char type, int gs, int tag) = 0;
	protected:
		KingdomBattleField(char _type, int _gs, int _tag, bool _dom)
			: fieldtype(_type), gsid(_gs), tagid(_tag), dominant(_dom)
		{
			b_state = BSTATE_IDLE;
			if (GetStub(_type) == NULL)
				stubmap[_type] = this;
		}
		void StateChange(int st)
		{
			LOG_TRACE("kingdom field type %d state change from %d to %d",
					fieldtype, b_state, st);
			b_state = st;
		}
		int GetState() { return b_state; }
	public:
		enum
		{
			BSTATE_IDLE = 0,
			BSTATE_SEND_OPEN,
			BSTATE_OPEN_FAILED,
			BSTATE_PREPARE,    //准备阶段 一级战场可进入 二级战场不可进入
			BSTATE_FIRST_HALF, //上半场
			BSTATE_SECOND_HALF,
		};
		static KingdomBattleField * Create(char type, int gs, int tag)
		{
			KingdomBattleField * stub = GetStub(type);
			return stub == NULL ? NULL : stub->Clone(type, gs, tag);
		}
		char GetType() { return fieldtype; }
		int GetGS() { return gsid; }
		int GetTag() { return tagid; }
		void OnStart(int ret, int tag, int def, const std::vector<int> & att, const Octets & def_name);
		bool CheckReady()
		{ 
			return b_state == BSTATE_PREPARE;
//			|| b_state == BSTATE_FIRST_HALF || b_state == BSTATE_SECOND_HALF;
		}
		bool SendStart(int _def, const Octets & def_name, const std::set<int> _attackers);
		void SendStop();
		bool StartFight();
		bool OnDisconnect();
		bool OnEnd(int winner);

		virtual ~KingdomBattleField() {}
		virtual void Update(time_t now);
		virtual void SetBattleSide(int defender, const Octets & def_name, const std::vector<int> & attackers) {}
		virtual int Enter(PlayerInfo * pinfo) = 0;
		virtual void DelRole(int roleid) = 0;
		virtual void EnterFail(int roleid);
		virtual int TryLeave(int tag, int roleid) { return ERR_SUCCESS; }
		virtual void Leave(int roleid);
		virtual void OnLogout(int roleid) {}
		virtual void OnAttackerFail(int tag, int failer)
		{
			Log::log(LOG_ERR, "default OnAttackerFail is called, tag %d", tag);
		}
		virtual bool OnHalf(int tag, char res, std::vector<int> _failers)
		{
			Log::log(LOG_ERR, "default OnHalf is called, tag %d", tag);
			return false;
		}
		virtual void OnFieldEnd(int win_attacker) {}
	};
	class MainKingdomBattle : public KingdomBattleField
	{
		struct BattleSide
		{
			bool fail;
			std::set<int> roles;
			BattleSide() : fail(false) {}
		};
		int defender;
		Octets def_fac_name;
		BattleSide defenderside;
		std::map<int/*factionid*/, BattleSide> attackers;
		std::multimap<int/*time*/, int/*roleid*/> offlineroles;

		MainKingdomBattle(char _type, int _gs, int _tag)
			: KingdomBattleField(_type, _gs, _tag, true)
		{
			defender = 0;
			LOG_TRACE("a main kingdombattle was created, gs %d tag %d",
					_gs, _tag);
		}
		static MainKingdomBattle stub;
	public:
		enum
		{
			DEFENDER_LIMIT = 40,
			ATTACKER_LIMIT = 20,
			PUNISH_TIME   = 60, //异常退出 惩罚 1 min
		};
		KingdomBattleField * Clone(char type, int gs, int tag)
		{
			return new MainKingdomBattle(type, gs, tag);
		}  
		bool IsRoleIn(int roleid);
		virtual void DelRole(int roleid);
		virtual void Update(time_t now);
		virtual void SetBattleSide(int _def, const Octets & def_name, const std::vector<int> & _attackers)
		{
			if (!attackers.empty())
				Log::log(LOG_ERR, "kingdom attackers size %d", attackers.size());
			ClearAttackerFacs();

			defender = _def;
			def_fac_name = def_name;
			defenderside = BattleSide();
			std::vector<int>::const_iterator it, ite = _attackers.end();
			for (it = _attackers.begin(); it != ite; ++it)
			{
				attackers[*it] = BattleSide();
				LoadRefFaction(*it);
			}
		}
		void ClearAttackerFacs()
		{
			defender = 0;
			def_fac_name.clear();
			defenderside = BattleSide();

			std::map<int, BattleSide>::iterator ait, aite = attackers.end();
			for (ait = attackers.begin(); ait != aite; ++ait)
				ReleaseFaction(ait->first);
			attackers.clear();

			offlineroles.clear();
		}
		virtual void OnAttackerFail(int tag, int failer)
		{
			if (GetTag() != tag)
			{
				Log::log(LOG_ERR, "OnAttackerFaild tagid %d(%d) not match",
						tag, GetTag());
				return;
			}
			//attackers.erase(failer);
			std::map<int, BattleSide>::iterator it = attackers.find(failer);
			if (it != attackers.end())
				it->second.fail = true;
		}
		virtual bool OnHalf(int tag, char res, std::vector<int> _failers);
		virtual int Enter(PlayerInfo * pinfo);
//		virtual void EnterFail(int roleid); 
		virtual int TryLeave(int tag, int roleid);
//		virtual void Leave(int roleid);
		virtual void OnLogout(int roleid);
		virtual void OnFieldEnd(int win_attacker);

		void FirstHalfEndAnnounce(char res);
	};
	class SubKingdomBattle : public KingdomBattleField
	{
		std::set<int> battleroles;
		SubKingdomBattle(char _type, int _gs, int _tag)
			: KingdomBattleField(_type, _gs, _tag, false)
		{
			LOG_TRACE("a sub kingdombattle was created, gs %d tag %d",
					_gs, _tag);
		}
		static SubKingdomBattle stub;
	public:
		enum
		{
			BATTLE_LIMIT = 300,
		};
		KingdomBattleField * Clone(char type, int gs, int tag)
		{
			return new SubKingdomBattle(type, gs, tag);
		}  
		virtual void SetBattleSide(int _def, const Octets & def_name, const std::set<int> _attackers)
		{
			LOG_TRACE("clear sub battle roles");
			battleroles.clear();
		}
		virtual int Enter(PlayerInfo * pinfo);
		bool IsRoleIn(int roleid);
		virtual void DelRole(int roleid);
		virtual void OnLogout(int roleid);
		virtual void OnFieldEnd(int win_attacker);
		virtual void Update(time_t now);
	};
	class KingdomInfo;
	class KingdomBattle
	{
		KingdomInfo * parent;
		int state;
		time_t state_timeout;
		typedef std::map<char, KingdomBattleField *> FieldList;
		FieldList fields;
		std::set<int> debug_attackers;
		bool debug_start;
		time_t special_start_time;//明确指定最近一次国战开启日期
	public:
		static unsigned int ATTACKER_COUNT;
		static int PREPARE_TIME;
		enum
		{
			START_TIME = 72000,//开始时间与周日0:00之间的时间间隔
			SWITCH_TIME = 120,
			//PREPARE_TIME = 300,
			FIGHTING_MAX_TIME = 9000, //所有战场的最长持续时间
			//ATTACKER_COUNT = 4,//4打1
		};
		enum
		{
			STATE_IDLE = 0,
			STATE_WAIT_OPEN,//数据加载完毕 等待开启
			STATE_SEND_OPEN,
			STATE_PREPARE,
			STATE_FIGHTING,
		};
		KingdomBattle ()
		{
			parent = NULL;
			state = STATE_IDLE;
			state_timeout = 0;
			debug_start = false;
			special_start_time = 0;
		}
		void SetParent(KingdomInfo * p) { parent = p; }
		void Register(int gsid, char type, int tag);
		void OnKingdomReady();
		static time_t GetNextSundayNight(time_t now); //获取下一个周日晚上8:00的时间戳
		static bool IsLastSundayOfMon(time_t sunday); //参数为某个周日晚8:00的时间戳 判断该周日是否为当月最后一个 周日
		void StateChange(int st);
		bool SendStart();
		void StartFight();
		void SendStop(char exceptiontype = 0);
		void Update(time_t now);
		bool CheckFieldsReady();
		void OnBattleStart(int ret, char fieldtype, int tag, int def, const std::vector<int> & attackers, const Octets & def_name);
		void OnBattleAttackerFail(char fieldtype, int tag, int failer);
//		void EndAnnounce(char res, int newkingfactioin);
		bool OnBattleHalf(char type, int tag, char res, std::vector<int> _failers);
		bool OnBattleEnd(char type, int tag, int winner);
		int TryEnterBattle(char type, PlayerInfo * pinfo);
		int TryLeaveBattle(char type, int tag, int roleid);
		void OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map);
		void OnLoginBattle(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map);
		void OnLogout(int roleid, int gsid, int map);
		void OnDisconnect(int gsid);
		void DebugAddAttacker(int fid);
		void DebugClearAttacker();
		void DebugStartBattle();
		void SetSpecialStartTime(time_t t) { special_start_time = t; }
	};
	class KingdomLimit
	{
	public:
		enum
		{
			TRIPLE_EXP = 1, //3倍经验的冷却
			DOUBLE_DROP,	//双倍掉落的冷却
			OPEN_BATH, 	//开启泡澡
			CALL_GUARDS, 	//召唤亲卫
		};
		enum
		{
			PERIOD_DAY = 1,
		};
	protected:
		GKingdomPeriodLimit limit;
	public:
		virtual ~KingdomLimit() {}
		void LoadCD(const GKingdomPeriodLimit & l) { limit = l; }
		GKingdomPeriodLimit GetCD() { return limit; }
		void ClearCD()
		{
			limit.period_type = 0;
			limit.period_times = 0;
			limit.last_timestamp = 0;
		}
		int GetRemainTimes() const
		{
			return GetTimesLimit() - limit.period_times;
		}

		virtual bool CheckCD() { return true; }
		virtual void SetCD() {}
		virtual int GetCDType() { return 0; }
		virtual int GetTimesLimit() const { return 0; } //周期内次数限制
	};
	class KingCallGuardsLimit : public KingdomLimit
	{
	public:
		virtual bool CheckCD();
		virtual void SetCD();
		virtual int GetCDType() { return CALL_GUARDS; }
		virtual int GetTimesLimit() const { return 5; }
	};
	class KingdomAttrSet : public KingdomLimit
	{
	protected:
		bool ongoing;
		time_t timeout;
//	protected:
//		GKingdomPeriodLimit limit;
	public:
		KingdomAttrSet() : ongoing(false), timeout(0) {}
		int Set();
		//void LoadCD(const GKingdomPeriodLimit & l) { limit = l; }
		//GKingdomPeriodLimit GetCD() { return limit; }
		virtual void Update(time_t now);
		bool IsOnGoing() { return ongoing; }
		bool SetAttrGeneral(GameAttrMap::attri_t, const GameAttrMap::value_t & value);

		virtual ~KingdomAttrSet() {}
//		virtual bool CheckCD() { return true; }
//		virtual void SetCD() {}
		virtual int OnSet() = 0;
		virtual void OnCancel() = 0;
		virtual int Cost() { return 0; }
		virtual int LastTime() { return 0; }
//		virtual int GetCDType() { return 0; }
		virtual int CheckOpen() { return -1; }
	};
	class KingdomTripleExp : public KingdomAttrSet
	{
	public:
		virtual bool CheckCD();
		virtual void SetCD();
		virtual int OnSet();
		virtual void OnCancel();
		virtual int Cost() { return 990; }
		virtual int LastTime() { return 3600; }
		virtual int GetCDType() { return TRIPLE_EXP; }
		virtual int GetTimesLimit() const { return 1; } //周期内次数限制
	};
	class KingdomDoubleDrop : public KingdomAttrSet
	{
	public:
		virtual bool CheckCD();
		virtual void SetCD();
		virtual int OnSet();
		virtual void OnCancel();
		virtual int Cost() { return 9990; }
		virtual int LastTime() { return 3600; }
		virtual int GetCDType() { return DOUBLE_DROP; }
		virtual int GetTimesLimit() const { return 1; } //周期内次数限制
	};
	class KingdomOpenBath : public KingdomAttrSet
	{
		enum
		{
			SYS_START_TIME = 590400, //周六晚八点
			FORBID_BEGIN_TIME = 585000, //周六晚上 18:30
			FORBID_END_TIME = 595800, //周六晚上 21:30
		};
		time_t t_base; //本周日凌晨零点
	public:
		KingdomOpenBath();
		virtual void Update(time_t now);
		virtual bool CheckCD();
		virtual void SetCD();
		virtual int OnSet();
		virtual void OnCancel();
		virtual int Cost() { return 699; }
		virtual int LastTime() { return 3600; }
		virtual int GetCDType() { return OPEN_BATH; }
		virtual int CheckOpen();
		virtual int GetTimesLimit() const { return 1; } //周期内次数限制
	};
	class KingdomInfo //国家信息
	{
		KingdomFunctionary king;
		char kinggender;
		KingdomFunctionary queen;
		Octets kingdom_name; //国号
		int king_faction; //国王所在帮
		Octets king_fac_name;
		Octets announce;
		int points; //国家积分
		int win_times;
		int occupy_time;
		int reward_mask;
		std::map<int/*roleid*/, KingdomFunctionary> functionaries;
		int task_status;
		int task_type;
		int task_issue_time;
		int task_points;
		
		//in memory attributes
		int db_status;
		bool dirty;
		std::map<char/*title*/, int/*count*/> func_count;
		KingdomBattle battle;
		std::map<int/*attr_flag*/, KingdomAttrSet *> game_attr;
		std::map<int/*limit_type*/, KingdomLimit *>limits;
	public:
		enum
		{
			SK_DATAREADY = 0x01,
			SK_SYNC_COMMONDATA = 0x02,
		};
		enum
		{
			ANNOUNCE_POINTS = 490,
			TASK_OPEN_PERIOD = 518400, //任务开放时间6天，6天内可接
			TASK_LAST_PERIOD = WEEK_SECONDS,//任务持续时间7天，7天后结算
//			TASK_ISSUE_CD = TASK_LAST_PERIOD + 300, //国王发布任务冷却时间
		};
		KingdomInfo()
		{
			kinggender = 0;
			king_faction = 0;
			points = 0;
			win_times = 0;
			occupy_time = 0;
			reward_mask = 0xffffffff;
			task_status = KINGDOM_TASK_NONE;
			task_type = 0;
			task_issue_time = 0;
			task_points = 0;
			db_status = 0;
			dirty = false;
		}
		//调用DBSetWinner(攻方胜利) 之前的最后一次延迟写信息 如果没有及时写入 则会丢失 小概率事件
		void OnSet(const GKingdomInfo & info)
		{
			king = LoadFunc(info.king);
			kinggender = info.kinggender;
			queen = LoadFunc(info.queen);
			kingdom_name = info.kingdomname;
			king_faction = info.kingfaction;
			king_fac_name = info.kingfacname;
			announce = info.announce;
			points = info.points;
			win_times = info.win_times;
			occupy_time = info.occupy_time;
			reward_mask = info.reward_mask;
			task_status = info.taskstatus,
			task_type = info.tasktype;
			task_issue_time = info.taskissuetime;
			task_points = info.taskpoints;

			functionaries.clear();
			func_count.clear();
			std::vector<GKingdomFunctionary>::const_iterator it, ite = info.functionaries.end();
			for (it = info.functionaries.begin(); it != ite; ++it)
			{
				functionaries[it->roleid] = LoadFunc(*it);
				func_count[it->title]++;
			}
			/*
			std::vector<GKingdomPeriodLimit>::const_iterator lit, lite = info.limits.end();
			for (lit = info.limits.begin(); lit != lite; ++lit)
			{
				KingdomLimit * limit = FindLimit(lit->func_type);
				if (limit == NULL)
				{
					Log::log(LOG_ERR, "invalid limit type %d", lit->func_type);
					continue;
				}
				limit->LoadCD(*lit);
			}
			*/
			std::map<int, KingdomLimit *>::iterator lit, lite = limits.end();
			for (lit = limits.begin(); lit != lite; ++lit)
			{
				if (lit->second == NULL)
					continue;
				std::vector<GKingdomPeriodLimit>::const_iterator it2, ite2 = info.limits.end();
				for (it2 = info.limits.begin(); it2 != ite2; ++it2)
				{
					if (it2->func_type == lit->first)
						break;
				}
				if (it2 != ite2)
					lit->second->LoadCD(*it2);
				else
					lit->second->LoadCD(GKingdomPeriodLimit()); //清除 limit
			}
			if (king_faction)
				LoadRefFaction(king_faction);
		}
		void GetSaveInfo(GKingdomInfo & info) const
		{
			info.kingfaction = king_faction;
			info.kingdomname = kingdom_name;
			//king_fac_name = info.kingfacname;
			info.announce = announce;
			info.points = points;
			info.taskpoints = task_points;
			//win_times = info.win_times;
			//occupy_time = info.occupy_time;
			//rewarded = info.rewarded;
			std::map<int, KingdomLimit *>::const_iterator it, ite = limits.end();
			for (it = limits.begin(); it != ite; ++it)
			{
				if (it->second != NULL)
					info.limits.push_back(it->second->GetCD());
			}
		}


		int GetClientInfo(int roleid, KingdomClientInfo & client) const
		{
			if (!IsReady())
				return ERR_KINGDOM_NOT_INIT;
			const KingdomFunctionary * func = FindFunc(roleid);
			if (func != NULL)
			{
				client.self_title = func->title;
				client.privileges_mask = func->privileges_mask;
			}
			else
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
				if (pinfo && (int)pinfo->factionid==king_faction)
					client.self_title = KINGDOM_TITLE_MEMBER;
			}
			client.king = king;
			client.kinggender = kinggender;
			client.queen = queen;
			client.kingdomname = kingdom_name;
			client.king_faction = king_faction;
			client.kingfacname = king_fac_name;
			client.announce = announce;
			client.points = points;
			client.win_times = win_times;
			client.reward_mask = reward_mask;
			std::map<int, KingdomFunctionary>::const_iterator it, ite = functionaries.end();
			for (it = functionaries.begin(); it != ite; ++it)
			{
				if (client.self_title == KINGDOM_TITLE_KING)
				{
					KingdomFunctionary tmp(it->second);
					PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(it->first);
					tmp.online = (pinfo==NULL?0:1);
					client.functionaries.push_back(tmp);
				}
				else
					client.functionaries.push_back(it->second);
			}
			return ERR_SUCCESS;
		}
		void GetInfoForGS(KingdomInformation & info) const
		{
			info.kingfaction = king_faction;
			info.points = points;
			info.task_type = task_type;
			if (king.roleid)
				info.functionaries.push_back(king);
			if (queen.roleid)
				info.functionaries.push_back(queen);
			std::map<int, KingdomFunctionary>::const_iterator it, ite = functionaries.end();
			for (it = functionaries.begin(); it != ite; ++it)
				info.functionaries.push_back(it->second);
		}
		KingdomFunctionary LoadFunc(const GKingdomFunctionary & info)
		{
			return KingdomFunctionary(info.roleid, info.title, info.rolename, info.privileges_mask);
		}
		void Load(const GKingdomInfo & info)
		{
			if (IsLoad())
			{
				Log::log(LOG_ERR, "kingdom data already load, db_status=%d", db_status);
				return;
			}
			OnSet(info);

			db_status |= SK_DATAREADY;
			/*
			battle.OnKingdomLoad();
			SyncKingdomInfo();
			*/
		}
		KingdomBattle & GetBattle() { return battle; }
		void Init(int special_time)
		{
			battle.SetParent(this);
			battle.SetSpecialStartTime(special_time);

			KingdomAttrSet * attr = new KingdomTripleExp();
			game_attr[KingdomLimit::TRIPLE_EXP] = attr;
			limits[KingdomLimit::TRIPLE_EXP] = attr;

			attr = new KingdomDoubleDrop();
			game_attr[KingdomLimit::DOUBLE_DROP] = attr;
			limits[KingdomLimit::DOUBLE_DROP] = attr;

			attr = new KingdomOpenBath();
			game_attr[KingdomLimit::OPEN_BATH] = attr;
			limits[KingdomLimit::OPEN_BATH] = attr;

			KingdomLimit * limit = new KingCallGuardsLimit();
			limits[KingdomLimit::CALL_GUARDS] = limit;
		}
		static int MapTaskCompleteTime(int type, int _task_points)
		{
			const static int points_per_task[] = {5, 10, 20, 50}; //对应 task_type 1 2 3 4
			if (type > 0 && type <= (int)sizeof(points_per_task))
			{
				if (points_per_task[type-1] > 0)
					return _task_points/points_per_task[type-1];
			}
			return 0;
		}
		bool IsLoad() const { return db_status & SK_DATAREADY; }
		bool IsReady() const { return (db_status&SK_DATAREADY) && (db_status&SK_SYNC_COMMONDATA); }
		/*
		bool IsGettingTaskPoint const { return db_status & SK_GETTING_TASKPOINT; } 
		void SetGettingTaskPoint(bool b)
		{
			if (b)
				db_status |= SK_GETTING_TASKPOINT;
			else
				db_status &= ~SK_GETTING_TASKPOINT;
		}
		*/
		int GetKing() const { return king.roleid; }
		int GetQueen() const { return queen.roleid; }
		int GetKingFaction() const { return king_faction; }
		const Octets & GetKingName() const { return king.rolename; }
		const Octets & GetQueenName() const { return queen.rolename; }
		const Octets & GetKingFactionName() const { return king_fac_name; }
		void SetKing(int k, int kf) {  king = k; king_faction = kf; }
		void SetDirty(bool d) { dirty = d; }
		bool IsDirty() { return dirty; }
		void Update(time_t now);
		void SyncKingdomInfo();
		void SyncKingdomInfo(unsigned int sid);
		void OnPointChange(int delta);
		void OnGSConnect(unsigned int sid);
		void OnSetWinner(char result, const GKingdomInfo & newinfo);

		void NewWinnerAnnounce(const Octets & win_fac_name, const Octets & new_king_name);
		void SetNewWinner(char res, int win_fac, int _task_points);
		bool CheckFuncCount(char title)
		{
			if (title == KINGDOM_TITLE_GENERAL)
				return (func_count[title] < 3);
			else if (title == KINGDOM_TITLE_MINISTER)
				return (func_count[title] < 2);
			else if (title == KINGDOM_TITLE_GUARD)
				return (func_count[title] < 10);
			else
				return false;
		}	
		KingdomFunctionary * FindFunc(int roleid);
		const KingdomFunctionary * FindFunc(int roleid) const ;
		int Announce(int roleid, const Octets & ann);
		int Appoint(int roleid, const Octets & candidate_name, char title);
		void UpdateTitle(int candidate_id, char title);//title=-1 表示解除国家头衔
		void OnDBAppoint(char title, int candidate_id, const Octets & candidate_name);
		int Discharge(int roleid, int candidate, char title);
		void OnDBDischarge(int candidate_id, char oldtitle);
		KingdomAttrSet * FindAttrSet(int flag);
		KingdomLimit * FindLimit(int flag);
		int SetGameAttr(int roleid, int flag);
		bool CheckPrivilege(int roleid, unsigned int privilege)
		{
			unsigned int mask = 0;
			KingdomFunctionary * func = FindFunc(roleid);
			if (func)
				mask = func->privileges_mask;
			return (mask & privilege);
		}
		bool CheckPoints(int cost) const { return points >= cost; }
		int GetTitle(int roleid, KingdomGetTitle_Re & re) const;
		void UpdateQueen(const GKingdomFunctionary & qu);
		void OnRolenameChange(int roleid, const Octets & rolename);
		void OnFacNameChange(int fid, const Octets & newname);
		void OnFactionChange(int oldfid, int fid, int roleid);
		void OnBattleHalf(char type, int tag, char res, std::vector<int> _failers);
		void OnBattleEnd(char type, int tag, int winner);
		void ClearTitles();
		void GenerateTitles();
		void BroadcastNewKing(int newking);
		int TryCallGuards(int roleid, int lineid, int mapid, float x, float y, float z, int & remain_times);
		int TryOpenBath(int roleid);
		int OpenBath();
		int IssueTask(int roleid, int task_type);
		int GetReward(int roleid);
		void OnGetReward(int mask);
		void OnDBTaskIssue(int task_type, int task_issue_time);
		int GetTask(int roleid, int & task_type, int & complete_times, int & _status, int & _time);
		void DebugClearLimits();
		void DebugSetWinner(int res, int new_winner);
		void CloseTask();
		void EndTask();
	};
	class KingdomManager : public IntervalTimer::Observer
	{
		KingdomInfo kingdom;
		KingdomManager() {}
	public:
		enum
		{
			UPDATE_INTERVAL = 15,
		//	WEEK_SECONDS = 604800,
		};
		~KingdomManager() { }
		static KingdomManager * GetInstance() { static KingdomManager instance; return &instance;}
		bool LoadConf(time_t & special_time);
		bool Initialize();
		bool OnDBConnect(Protocol::Manager *manager, int sid);
		void RegisterField(int gsid, const KingdomBattleInfoVector & fields);
		void LoadKingdom(const GKingdomInfo & info);
		bool Update();
		void OnBattleStart(int ret, char fieldtype, int tag, int def, const std::vector<int> & attackers, const Octets & def_name);
		void OnBattleAttackerFail(char type, int tag, int failer);
		void OnBattleHalf(char type, int tag, char res, std::vector<int> _failers);
		void OnBattleEnd(char type, int tag, int winner);
		int TryEnterBattle(char type, PlayerInfo * pinfo);
		int TryLeaveBattle(char type, int roleid, int tag);
		void OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map);
		void OnLoginBattle(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map);
		void OnLogout(int roleid, int gsid, int map);
		void OnDisconnect(int gsid);
		void DebugAddAttacker(int fid);
		void DebugClearAttacker();
		void DebugStartBattle();
		void DebugClearLimits();
		void OnPointChange(int delta);
		void OnGSConnect(unsigned int sid);
		void OnSetWinner(char result, const GKingdomInfo & newinfo);
		int Announce(int roleid, const Octets & announce);
		int Appoint(int roleid, const Octets & candidate_name, char title);
		void OnDBAppoint(char title, int candidate_id, const Octets & candidate_name);
		int Discharge(int roleid, int candidate, char oldtitle);
		void OnDBDischarge(int candidate, char oldtitle);
		int SetGameAttr(int roleid, int flag);
		int GetClientInfo(int roleid, KingdomClientInfo & client) const;
		int GetTitle(int roleid, KingdomGetTitle_Re & re);
		int GetKingFaction() const { return kingdom.GetKingFaction(); }
		void UpdateQueen(const GKingdomFunctionary & qu);
		void OnRolenameChange(int roleid, const Octets & rolename);
		void OnFacNameChange(int fid, const Octets & newname);
		void OnFactionChange(int oldfid, int fid, int roleid);
		int TryCallGuards(int roleid, int lineid, int mapid, float x, float y, float z, int & remain_times);
		int TryOpenBath(int roleid);
		int OpenBath();
		int IssueTask(int roleid, int task_type);
		int GetReward(int roleid);
		void OnGetReward(int mask);
		void OnDBTaskIssue(int task_type, int task_issue_time);
		int GetTask(int roleid, int & task_type, int & complete_times, int & status, int & time);
		void DebugSetWinner(int res, int new_winner);
		void DebugCloseTask();
		void DebugEndTask();
	};
};
#endif

