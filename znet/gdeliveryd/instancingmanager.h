#ifndef __GNET_INSTANCING_MANAGER_HPP__
#define __GNET_INSTANCING_MANAGER_HPP__

#include <vector>
#include <map>

#include "thread.h"
#include "staticmap.h"
#include "battlecommon.h"
#include "instancingfieldinfo"
#include "ginstancingfieldinfo"
#include "gfieldinfo"
#include "localmacro.h"
#include "mapuser.h"
#include "gmailsyncdata"

namespace GNET
{
	enum COOLDOWN_TYPE
	{
		CD_ACTIVATE,
		CD_ENTER,
	};

	typedef BattleRole InstancingRole;
	class InstancingServer;
	class InstancingField
	{
	public:
		typedef std::vector<InstancingRole> TeamType;
		enum
		{
			SWITCH_TIMEOUT  = 60,
			ENTER_TIMEOUT  	= 60,
			ENTER_MAX_PERDAY = 10,
		};

		enum STATUS
		{
			ST_QUEUEING 	= 1,
			ST_SENDSTART 	= 2,
			ST_FIGHTING 	= 3,
			ST_PREPARING 	= 4,
			ST_WAITQUEUE 	= 5,
		};

		InstancingField()
			: map_id(0), battle_id(0), tid(0), server(NULL)
			, status(ST_WAITQUEUE), start_time(0),queuing_time(0),
			fighting_time(0),preparing_time(0)
		{
		}
		void 	Init(const InstancingFieldInfo& info, InstancingServer* server);
		int		Accept(int roleid, short accept, int target);
		int		Appoint(int roleid, int target);
		int		Kick(int roleid, int target);
		int 	JoinTeam(InstancingRole & role, unsigned char team, GMailSyncData & syncdata);
		void 	OnActivate(int retcode, InstancingRole & role, unsigned char team, GMailSyncData & syncdata);
		int 	LeaveTeam(int roleid, bool force);
		void 	Broadcast(Protocol & data, unsigned int & localsid);
		void 	GetMember(int roleid, unsigned char team, std::vector<GBattleRole> & members, std::vector<GBattleRole> & req_members);
		int		GetAttackerNumber() const { return team_attacker.size(); }
		int 	GetDefenderNumber() const { return team_defender.size(); }
		int 	GetLookerNumber()   const { return team_looker.size(); }
		int 	BattleStart(int roleid);
		void 	OnBattleStart(int retcode);
		int  	OnBattleEnter(int roleid, int &endtime);
		bool	OnBattleEnd(int tag);
		void 	OnChangeStatus(int world_tag, int status);
		void 	Update(time_t now, bool & b_notify);
		void 	AddTag(int tag);
		int    	TryStart(bool manual);

	//private:
		
		bool    SizePolicy(bool manual)
		{
			if (manual)
				return ((!attacker_info.manual_start_player_num||team_attacker.size() >= (unsigned int)attacker_info.manual_start_player_num)
				&& (!defender_info.manual_start_player_num||team_defender.size()>=(unsigned int)defender_info.manual_start_player_num));
			else
				return ((!attacker_info.auto_start_player_num||team_attacker.size() >= (unsigned int)attacker_info.auto_start_player_num)
				&& (!defender_info.auto_start_player_num||team_defender.size()>=(unsigned int)defender_info.auto_start_player_num));
		}

		int 	JoinReqTeam(InstancingRole & role, unsigned char teamid, TeamType & team_req, TeamType &team_queue, InstancingSideInfo & sideinfo, GMailSyncData & syncdata);
		int 	Activate(InstancingRole & role, unsigned char teamid, TeamType & team_req, TeamType &team_queue, InstancingSideInfo & sideinfo, GMailSyncData & syncdata);
		void 	OnActivate(int retcode, InstancingRole & role, unsigned char teamid, TeamType & team_req, TeamType &team_queue, InstancingSideInfo & sideinfo, GMailSyncData & syncdata);
		int		Accept(TeamType & team_req, TeamType & team_queue, unsigned char teamid, InstancingSideInfo & sideinfo, short accept, int target);
		int		Appoint(TeamType &team_queue, unsigned char teamid, int target);

		bool 	TeamSizePolicy(TeamType &team, InstancingSideInfo & info);
		

		int		LevelPolicy(int level, int reborn_cnt, InstancingSideInfo & info)
		{
			if (info.reborn_limit!=-1 && info.reborn_limit!=reborn_cnt) 
			{
				if (info.reborn_limit>0)
					return ERR_BATTLE_REBORN_CNT;			//飞升等级不满足要求
				else
					return ERR_BATTLE_MUSTNOT_REBORN;		//没有飞升的才能报名
			}

			if ((info.min_level_limit==0 || level>=info.min_level_limit) 
				&& (info.max_level_limit==0 || level<=info.max_level_limit))
				return ERR_SUCCESS;
			else
				return ERR_BATTLE_LEVEL_LIMIT;
		}

		bool  	OccupationPolicy(int occupation, TeamType & team, InstancingSideInfo& info)
		{
			if (info.max_prof_count_limit == 0) return true;

			int count = 0;
			for (TeamType::iterator it = team.begin(); it != team.end(); ++it) 
			{
				if (it->occupation == occupation)
				{
					count++;
					if (count == info.max_prof_count_limit)
						return false;
				}
			}
			return true;
		}

		int	JoinPolicy(const InstancingRole &role, InstancingSideInfo & info, TeamType & team)
		{
			if (!TeamSizePolicy(team, info)) 
				return ERR_BATTLE_TEAM_FULL;
			GRoleInfo *roleinfo = RoleInfoCache::Instance().Get(role.roleid);
			if (roleinfo == NULL) return ERR_BATTLE_TEAM_FULL;
			int retcode = LevelPolicy(roleinfo->level, roleinfo->reborn_cnt, info);
			if (retcode != ERR_SUCCESS) return retcode;
			if (!OccupationPolicy(roleinfo->occupation, team, info)) 
				return ERR_BATTLE_OCCUPATION;
			return ERR_SUCCESS;
		}
		void 	GetFieldInfo(int roleid, class InstancingGetField_Re & re);
		
		int map_id;
		int battle_id;
		int tid;
		InstancingServer* server;
		InstancingSideInfo attacker_info;
		InstancingSideInfo defender_info;
		InstancingSideInfo looker_info;
		TeamType team_attacker;
		TeamType team_defender;
		TeamType team_looker;
		TeamType team_attacker_req;
		TeamType team_defender_req;
		TeamType team_looker_req;
		TagInfo  tag;
		short  	status;
		int	start_time;
		int check_time;
		int queuing_time;
		int fighting_time;
		int preparing_time;
		friend class InstancingServer;
	private:
		void 	ClearTeam(TeamType & team);
		bool 	OnBattleEnterTeam(int roleid, TeamType & team, unsigned char team_id);
		void 	BroadcastTeam(TeamType & team, Protocol & data, unsigned int & localsid);
		bool 	LeaveTeam(InstancingField::TeamType & team, int roleid, bool force);
	};

	typedef std::map<int /*map_id*/, InstancingField> InstancingMap;
	typedef std::map<int /*tid*/, std::vector<InstancingField *> > TidMap;
	class InstancingServer 
	{
	public:
		InstancingServer() : gs_id(0), sid(0)
		{
		}

		void 	Init(int _gs_id, int _sid, const std::vector<InstancingFieldInfo> &info);
		void 	Update(time_t now);

		void 	GetMap(int battle_tid, std::vector<GInstancingFieldInfo> & fields);
		void 	GetFieldInfo(int roleid, int map_id, class InstancingGetField_Re & re);

		int		Accept(int roleid, short accept, int map_id, int target);
		int		Appoint(int roleid, int map_id, int target);
		int		Kick(int roleid, int map_id, int target);
		int 	JoinTeam(InstancingRole & role, int map_id, unsigned char team, GMailSyncData & syncdata);
		void 	OnActivate(int retcode, InstancingRole & role, int map_id, unsigned char team, GMailSyncData & syncdata);
		int		LeaveTeam(int roleid, int map_id);
		int 	BattleStart(int roleid, int mapid);
		void 	OnBattleStart(int map_id, int retcode);
		void	OnBattleEnd(int map_id, const InstancingField &field);
		int 	OnBattleEnter(int roleid, int map_id, int &endtime);
		int 	OnBattleEnterFail(int roleid, int map_id);
		void	OnLogout(int roleid);
		void 	OnChangeStatus(int battle_id, int map_id, int status);
		void 	OnLeaveGame(int battle_id, int map_id, int roleid, int reason);
		void 	OnDisconnect();
		void    GetBattleTidList(std::set<int> & tidlist)
		{
			TidMap::iterator it = tidmap.begin(), ie = tidmap.end();
			for (; it != ie; ++it)
				tidlist.insert(it->first);
		}

		//private:
		friend class InstancingField;

		InstancingMap battlemap;
		TidMap tidmap;
		int gs_id;
		int sid;
		typedef std::vector<int> RoleMaps;
		typedef std::map<int, RoleMaps> QueueRoles;
		QueueRoles queue_roles;
	private:
		void EraseMapID(int map_id, int roleid);
	};

	class InstancingManager : public IntervalTimer::Observer
	{
		enum
		{
			START_DURATION 		= 180,
			BATTLE_COOL_TIME 	= 900, // 10 minutes
			COOLDOWN_CHECK_MAX 	= 5,
		};
	public:
		typedef std::map<int /*gs_id*/, InstancingServer> ServerMap;

		static InstancingManager * GetInstance()
		{
			static InstancingManager inst;
			return &inst;
		}
		bool	Initialize();
		void 	RegisterServer(int sid, int gs_id, const std::vector<InstancingFieldInfo> & info);
		void 	GetMap(int roleid, int battle_tid, std::vector<GInstancingFieldInfo> & fields, int &enter_count);
		void 	GetFieldInfo(int roleid, int gs_id, int map_id, class InstancingGetField_Re & re);
		int 	JoinTeam(InstancingRole & role, int gs_id, int map_id, unsigned char team, GMailSyncData & syncdata);
		void 	OnActivate(int retcode, InstancingRole &role, int gs_id, int map_id, unsigned char team, GMailSyncData &syncdata);
		int 	LeaveTeam(int roleid, int gs_id, int map_id);
		int		Kick(int roleid, int gs_id, int map_id, int target);
		int		Accept(int roleid, short accept, int gs_id, int map_id, int target);
		int		Appoint(int roleid, int gs_id, int map_id, int target);
			

		int 	BattleStart(int roleid, int gs_id, int mapid);
		void 	OnBattleStart(int gs_id, int mapid, int retcode);
		int 	OnBattleEnter(int roleid, int gs_id, int map_id, int &endtime);
		int 	OnBattleEnterFail(int roleid, int gs_id, int map_id);
		void 	OnLogout(int roleid, int gs_id);
		void 	OnChangeStatus(int gs_id, int battle_id, int map_id, int status);
		void 	OnLeaveGame(int gs_id, int battle_id, int map_id, int roleid, int reason);
		bool 	Update();
		time_t 	GetTime();
		void 	OnDisconnect(int gs_id);
		void    GetBattleTidList(std::set<int> & tidlist)
		{       
			ServerMap::iterator it = servers.begin(), ie = servers.end();
			for (; it != ie; ++it)
				(it->second).GetBattleTidList(tidlist);
		}      

	private:
		InstancingManager()
			: t_forged(0), t_base(0)
		{
		}
	private:
		friend class InstancingField;
		ServerMap servers;
		int t_forged;
		int cursor;
		int t_base;
	};
} // namespace GNET

#endif
