#ifndef __GNET_CRSSVRTEAMS_INFO_H
#define __GNET_CRSSVRTEAMS_INFO_H

#include "mapuser.h"
#include "gcrssvrteamsrole"
#include "gcrssvrteamsinfo"
#include "gcrssvrteamsdata"
#include "crssvrteamsrolelist_re.hpp" 
#include "crssvrteamsteambase_re.hpp"
#include "synccrssvrteams.hrp"
#include "synccrssvrteamsrole.hrp"
#include "teamraidrolebrief"
#include "gcrssvrteamsscore"
#include "gcrssvrteamspostscore.hpp"
#include "raidmanager.h"

namespace GNET
{
	bool SerialCrssvrTeamsRole(int roleid, GCrssvrTeamsRole& roleinfo, int & gameid, unsigned int & linksid, unsigned int & localsid);

	typedef std::vector<TeamRaidRoleBrief> TTeamRaidRoleBrief;
	enum
	{
		ECT_SERVER = 1, // 原服
		ECT_CROSS_SERVER = 2, // 跨服
	};

	struct SCTDataGroup
	{
		int ct_leave_cool;
		int ct_teamid;
		int ct_score;
		int ct_last_max_score;
		int ct_last_punish_time;
		int ct_last_battle_count;
	};

	const static int ONEWEEK_SECOND = (3600 * 24 * 7);

	class CrssvrTeamsRole //跨服战队队员信息
	{
		friend class CrssvrTeams_Info;
		
		GCrssvrTeamsRole roleinfo;
		TeamRaidRoleBrief rolebrief;
		GCrssvrTeamsScore data_score;
		int dirty_mask;
	public:
		CrssvrTeamsRole(/*int gameid, unsigned int linksid, unsigned int localsid*/);
		void Initialize(GCrssvrTeamsRole& roleinfo);

		inline bool IsOnline() const
		{
			return roleinfo.isonline;
		}

		inline void SetOnline()
		{
			roleinfo.isonline = true;
		}

		void SetDirty(int flag)
		{
			dirty_mask |= flag;
		}

		void ClearDirty()
		{
			dirty_mask = 0;
		}
		void PrintRoles();
	};

	class CrssvrTeamsBaseManager;
	class CrssvrTeams_Info //跨服战队信息
	{
		friend class CrssvrTeamsBaseManager;
		friend class CrssvrTeamsManager;
		friend class CrossCrssvrTeamsManager;

		//const static int OCCUPATION_PLAYER_LIMIT=2;//每种职业人数不能超过2个
		
		typedef std::pair<int, int> ScorePeriod;
                typedef std::map<ScorePeriod, int> OpenScorePeriods;
		typedef std::map<unsigned char/*occup*/, int/*count*/> OccupCount;
		typedef std::list<CrssvrTeamsRole> RoleList;

		RoleList rolecache;//队员集合
		GCrssvrTeamsInfo info;
		OccupCount occup_count;
		int updatetime;
		int dirty_mask;
		bool loaded; // 是否加载完

		int servertype;// 1是原服，2是跨服
		OpenScorePeriods scoreperiod;// 结算基础分段系数

		CrssvrTeamsBaseManager* m_pmanager;
	public:
		enum
		{
			MAX_MEMBER_PERPAGE = 20,
			LOAD_TIMEOUT = 10,
		};

		CrssvrTeams_Info(int servertype, GCrssvrTeamsData& data, CrssvrTeamsBaseManager* pmanager);
		~CrssvrTeams_Info();

		inline int CalcScoreModules(int diff_value) { return 1/(1+10^(diff_value/400)); }
		int GetScorePeriod(int score)
		{
			OpenScorePeriods::const_iterator it, ite = scoreperiod.end();
			for (it = scoreperiod.begin(); it != ite; ++it)
			{
				if (score >= it->first.first && score <= it->first.second)
				{
					//LOG_TRACE("GetScorePeriod, score=%d, it->first.first=%d, it->first.second=%d, it->second=%d", score, it->first.first, it->first.second, it->second);
					return it->second;
				}
			}
			
			return 0;
		}

		bool IsLoadFinish();
		void SetLoadFinish(bool isload);
		
		void SendMsg(int roleid, Protocol& data, unsigned int &localsid);
		void Broadcast(Protocol& data, unsigned int &localsid);
		void BroadcastGS(Protocol& data, int & roleid);

		int  GetMasterID();
		void GetMembers(std::vector<GCrssvrTeamsRole>& roles);
		void GetMemberBriefs(TTeamRaidRoleBrief& rolebrief);
		void GetRoleList(CrssvrTeamsRoleList_Re& ret);
		void GetTeamBase(CrssvrTeamsTeamBase_Re& ret);
		int  GetMemberCount();
		//int  GetPrepCaptain(int roleid);
		int  GetMemberCapacity();
		void GetCrssvrTeamsInfo(GCrssvrTeamsInfo& tinfo);
		void GetCrssvrTeamsData(GCrssvrTeamsData& tinfo);
		void GetCrssvrTeamsData(GCrssvrTeamsData& tinfo, TTeamRaidRoleBrief& rolebrief);
		void GetCrssvrTeamsData(const TeamRoleList& rolelist, GCrssvrTeamsData& tinfo, TTeamRaidRoleBrief& rolebrief);
		//bool GetScore(int roleid, GCrssvrTeamsPostScore& re);

			
		bool IsRoleCrossOnline(); // 是否有玩家跨服在线
		bool IsRoleSrcOnline(); // 是否有玩家本服在线
		bool IsMemberExsit(int roleid);
		int  GetOccupCount(unsigned char occup)
		{
			return occup_count[occup];
		}

		int  Join(GCrssvrTeamsRole& groleinfo);
		int  UpdateRole(const GCrssvrTeamsRole& roleinfo, const GCrssvrTeamsScore& data_score);
		void ChgCaptain(int master_new);
		void DeleteRole(int roleid);
		void Dismiss();
		
		void SetRoleScore(int roleid, int final_score);
		void SetRoleScore(int roleid, int final_score, CrssvrTeamsRole& role, bool isFormula=false/*是否是玩家结算*/);
		void SetPunishRoleScore(int roleid, int final_score);
		void SetTeamScore(int final_score);
		void UpdateRoleLastMaxScore();
		void UpdateRoleLastMaxScore(int roleid);
		void SetRoleBrief(const TeamRaidRoleBrief& rolebrief);
		void SyncRoleInfo2Central(int roleid);

		void UpdateRoleLvl(int roleid, int new_lvl);
		void UpdateRoleName(int roleid, const Octets& new_name);
		void UpdateRoleOnline(int roleid, char online);
		void UpdateRolesOnline(char online);
		void UpdateRoleOccup(int roleid, unsigned char occup); // 职业修改通过Update和玩家下线刷

		void SendRoleListRe(int retcode);
		void SendTeamBaseRe(int retcode);

		void SetDirty(int flag)
		{
			dirty_mask |= flag;
		}

		void ClearDirty()
		{
			dirty_mask = 0;
		}

		void Update();
		void Save2DB(bool isbatch = false);
		void SaveRoles();

		void ObsoleteCache() { rolecache.clear(); }
		
		//积分结算
		int  BattleFormula(int a_score, int b_score, int result);
		int  CalcTeamScore(int b_score, int result);
		int  CalcRoleScore(std::vector<int>& team_roles, int b_score, int result);

		// 战队惩罚
		void CalcTeamPunish(int roleid, int now);

		void ClearRenameCD();// 清除队伍改名cd
		void DebugRoleScore(int roleid, int score);
		void DebugRoleLastMaxScore(int roleid, int score);	
		void DebugRoleTeamScore(int roleid, int score);
		void DebugRolesScore(int roleid, int score);
		void DebugTeamWin(int roleid, int times);
		void DebugTeamPunish(int roleid, int times);
		void DebugRolePunish(int roleid, int times);

		void PrintData(const char* s);
		void PrintData(const char* s, int roleid);
		void PrintRolesData(const char* s);
		void PrintRolesData(const char* s, int roleid);
		void PrintTeamData(const char* s);
	private:
		void SyncRolePunishScore2DB(int roleid, int role_score, int battle_count, int last_battle_time);
		void SyncRoleScore2DB(const CrssvrTeamsRole& role);
		void SyncRoleScore2Central(const CrssvrTeamsRole& role);
		void SyncRoleScoreAndInfo2Central(const CrssvrTeamsRole& role);
		void UpdateLastMaxScore(CrssvrTeamsRole& role);
	};

};
#endif

