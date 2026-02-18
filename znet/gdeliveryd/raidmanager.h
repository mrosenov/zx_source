#ifndef __GNET_RAID_MANAGER_H__
#define __GNET_RAID_MANAGER_H__

#include <vector>
#include <map>
#include <math.h>

#include "thread.h"
#include "battlecommon.h"
#include "gproviderserver.hpp"

#include "raidgetroom_re.hpp"
#include "raidopen_re.hpp"
#include "gfactionpkroominfo"
#include "graidroominfo"
#include "raidfieldinfo"
#include "localmacro.h"
#include "mapuser.h"
#include "gmailsyncdata"
#include "gfactionpkraidrecord"
#include "teamraidmatchdata"
#include "teamraidapplyingrole"

namespace GNET
{
	typedef BattleRole RaidRole;
//	class TeamRaidUpdateChar;
	class PlayerInfo;
	class FactionPkRaidGetScore_Re;
	/*管理一条线上的实例数*/
	class RaidNumber
	{
		friend class RaidMap_Info;
//		friend class RaidManager;
//		friend class RaidBasic_Info;
		const int max_instance;//max instance number, this raidmap, this line
		int instance_number;
		int applying_number;
	public:	
		RaidNumber(int max):max_instance(max),instance_number(0),applying_number(0){}
		bool Apply_Instance_Finish(bool success){
			//check number
			applying_number--;
			if(success){
				instance_number++;
			}
			LOG_TRACE("RaidNumber apply finish instance_number=%d applying_number=%d max_instance=%d",instance_number,applying_number,max_instance);
			return true;
		}
		void Dec_Instance(){
			instance_number--;
			LOG_TRACE("RaidNumber after dec instance_number=%d applying_number=%d max_instance=%d",instance_number,applying_number,max_instance);
		}
		bool Add_Applying_Instance(){
			if(GetFreeNum()>0){
				applying_number++;
				return true;
			}
			else
				return false;
		}
		int GetFreeNum(){return max_instance - instance_number - applying_number;}
	};

	/*管理一个副本地图，可以在多条线创建实例*/
	class RaidMap_Info
	{
//		friend class RaidManager;
//		friend class RaidBasic_Info;
	public:
		RaidMap_Info(const RaidFieldInfo& finfo,int gs_id)
			:group1_info(finfo.group1_limit), raid_type(finfo.raid_type), queuing_time(finfo.queuing_time),\
			 room_number(0),raid_template_id(finfo.raid_template_id)
		{
			if(finfo.raid_max_instance>=0){
				raidnumber.insert(std::make_pair(gs_id,RaidNumber(finfo.raid_max_instance)));
				room_limit = finfo.raid_max_instance;
			}
		}
		bool    StartPolicy(int player_num) const;
		bool	JoinPolicy(int before_player_num) const {
			return (before_player_num<group1_info.max_player_limit);
		}
		int RaidType() const {return raid_type;}
		bool Add_Room(){ 
			if(room_number<room_limit){
				room_number++;
				//LOG_TRACE("RaidRoom %d after add room_number=%d room_limit=%d ",raid_template_id,room_number,room_limit);
				return true;
			}
			else
				return false;
		}
		void Dec_Room(){
			room_number--;
			//LOG_TRACE("RaidRoom %d after dec room_number=%d room_limit=%d ",raid_template_id,room_number,room_limit);
		}
		int GetLineNum() const {return raidnumber.size();}
		int GetQueuingTime()const {return queuing_time;}
		int GetTemplateId()const {return raid_template_id;}
		int GetPlayerLimit()const{return group1_info.max_player_limit;}

		bool AddLine(const RaidFieldInfo& finfo,int gs_id);
		bool DelLine(int gs_id);
		bool Apply_Instance_Finish(int gs_id,bool success);
		bool Dec_Instance(int gs_id);
		bool Try_Get_Instance(int &gs_id);
	private:
		RaidGroupLimit group1_info; 
		int raid_type;
		int queuing_time;
		typedef std::map<int/*gsid*/,RaidNumber> RaidNumberMap;
		RaidNumberMap raidnumber;
		int room_limit;
		int room_number;
		int raid_template_id;
	};
	enum RaidKickReason
	{
		RAID_ROOMCLOSE =0, //副本房间关闭
		RAID_INSTANCE_START_ERR = 1,//开启副本失败
	        RAID_INSTANCE_ENTER_ERR = 2,//进入副本失败
	        RAID_INSTANCE_CLOSE = 3,//副本关闭
	        RAID_KICKOUT = 4,//被房主踢出
	};
	class RaidVote;
	/*管理一个副本房间，房间在副本开启后与副本实例绑定*/
	class RaidBasic_Info
	{
	protected:
		typedef std::vector<RaidRole> TeamType;		
		typedef std::set<int/*roleid*/> RoleSet;
		int raidroom_id;//副本房间号
		int map_id;
		int gs_id;
		int index;//instance's world_tag
		int status_end_time;//状态结束时间
		TeamType team1;//正式队伍
		bool gs_has_close_instance;

		int status;
		RaidMap_Info& map_info;
		RoleSet role_need_auto_enter;//在进入副本计时结束时，需要自动拉进副本的role
		RoleSet role_need_enter_re;//正式队伍中尚未收到GRaidEnterSuccess的role
		Octets raid_data;
		char raid_difficulty;
	public:
		const static int SWITCH_TIMEOUT  = 120;
		const static int ENTER_TIMEOUT  = 20;
		const static int NO_TIMEOUT = -1;
//		const static int ENTER_6V6_TEAM_RAID=2;//playerchangegs reason, normal = 0, vote kick =1
		const static int REENTER_LIMIT = 600;
		enum STATUS
		{
			RAID_INIT 	= 0,
		//	RAID_QUEUEING 	= 1,
			RAID_SENDSTART 	= 2,
			RAID_SENDSTART_NOTIFY 	= 3,
			RAID_FIGHTING 	= 4,
			RAID_BEGIN = RAID_FIGHTING+1,	
		};
		RaidBasic_Info(int raidroomid,int mapid,RaidMap_Info & mapinfo,char difficulty = 0)
			: raidroom_id(raidroomid), map_id(mapid),gs_id(-1),index(-1),
			gs_has_close_instance(false),status(RAID_INIT),map_info(mapinfo),raid_difficulty(difficulty)
		{
			status_end_time = NO_TIMEOUT;
		}
		
		virtual ~RaidBasic_Info();
		bool 	IsRoomRaid(){return map_info.RaidType() == ROOM_RAID || map_info.RaidType() == TOWER_RAID || map_info.RaidType() == LEVEL_RAID || map_info.RaidType() == TRANSFORM_RAID || map_info.RaidType() == STEP_RAID;}
		bool 	IsTeamRaid(){return map_info.RaidType() == TEAM_RAID || map_info.RaidType() == HIDEANDSEEK_RAID || map_info.RaidType() == FENGSHEN_RAID;}
		bool 	IsCrssvrTeamsRaid(){return map_info.RaidType() == CRSSVRTEAMS_RAID;}
		bool 	IsFactionPkRaid(){return map_info.RaidType() == FACTION_PK_RAID;}
		bool 	ISFighting(){return status == RAID_FIGHTING || status == RAID_SENDSTART_NOTIFY;}
		void 	GS_Close_Instance(){gs_has_close_instance = true;}
		bool 	NeedClose(){return team1.size()==0 && (!IsFactionPkRaid());}//for faction pk raid, keep open while no role inside
		bool 	AcceptRole(const RaidRole & role){ 
			if(FindRole(team1,role.roleid) != team1.end())
				return false;
			team1.push_back(role);
			return true;
		}
		int 	GetMapid(){return map_id;}
		int 	GetStatusEndTime(){return status_end_time;}
		int 	GetGs(){return gs_id;}
		bool 	EnterNotPermit(int roleid){return role_need_enter_re.find(roleid)!=role_need_enter_re.end();}
		int 	ReEnter(int roleid,int& gs_id_,int& index_);//断线重入
		int 	SetReenterRole(int roleid, const PlayerInfo& role);
		int 	Start();//申请开始副本
		virtual int 	RaidEnter(int roleid,bool autoenter);//申请进入副本gs
		int 	OnGsRaidEnter(int roleid,int gs_now);//gs答复进入副本申请
		void 	SetRaidData(const Octets & data){raid_data = data;}
		virtual int 	OnGsRaidEnterSuccess(int roleid,int gs_now,int index_);//gs通知角色进入副本成功
		virtual int 	DelRole(int roleid,unsigned char teamid);//删除人员
		virtual int 	UpdateRoomToMaster(){return 0;}//更新房间给房主
		virtual int	Appoint(int roleid, int target,bool auto_appoint){return 0;}//更换房主
		virtual int 	OnGsStart(int gs_id,int index,bool success)=0;//gs答复开始副本申请
//		virtual	int 	TryLeave(int roleid, int reason) { return ERR_SUCCESS; }
		virtual int 	Update(int now)=0;//更新状态
		virtual int 	GetReenterLimit() { return REENTER_LIMIT; }
		void 	RoomClose();//clear all role in raidrole_map
	private:
		virtual bool 	CheckStateChange(int target_state)=0;
	protected:
		virtual void 	SetStatusEndTime(int status_)=0;
		void 	AutoEnter();
		TeamType::iterator FindRole(TeamType &team,int roleid);
		void 	TeamBroadcast(TeamType & team, Protocol & data, unsigned int & localsid, int & t_roleid);
		int 	SetStatus(int status_);
		int	KickNotifyAll(int reason);
	};

	//6v6 team raid
	class RaidTeamBase;
	class TeamRaid_Info:public RaidBasic_Info
	{
		typedef std::vector<TeamRaidRoleData> RoleVec;
		TeamRaidMatchData teamdata;
		RoleSet role_need_mapping_re;//role need TeamRaidMappingSuccess_Re
		RaidTeamBase * teama;
		RaidTeamBase * teamb;
	public:
		const static int ACK_TIMEOUT = 30;
		const static int MIN_ENTER_NUM = 4;
		const static int TEAM_REENTER_LIMIT = 300;
		enum STATUS_EX
		{
			RAID_WAITACK 	= RAID_BEGIN,
//			RAID_WAITCHOOSECHAR,
			RAID_STARTFAIL,
			RAID_MAX,
		};
	
		TeamRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo, const TeamRaidMatchData& team_data, RaidTeamBase * a, RaidTeamBase * b);
		virtual ~TeamRaid_Info();
		virtual int 	Update(int now);//更新状态
		void Init(RaidTeamBase * teama, RaidTeamBase * teamb);
		int OnMappingSuccess_Re(int roleid, char agree);//同意参加
		virtual int OnGsStart(int gs_id,int index,bool success){return RaidBasic_Info::OnGsStart(gs_id,index,success);}//gs答复开始副本申请
		virtual int 	OnGsRaidEnterSuccess(int roleid,int gs_now,int index_);
		virtual int 	DelRole(int roleid,unsigned char teamid);//删除人员
		virtual int     RaidEnter(int roleid,bool autoenter);//申请进入副本gs
//		virtual	int 	TryLeave(int roleid, int reason);
//		int OnTeamRaidUpdateChar(TeamRaidUpdateChar msg);//选择形象或技能
		virtual int 	GetReenterLimit() { return TEAM_REENTER_LIMIT; }
		void RoomClose();
		void SendBattleResult(int result);// 发送战斗结果给原服
		void DelCrssvrTeamsApplying();
	private:
		virtual void 	SetStatusEndTime(int status_);
		virtual bool 	CheckStateChange(int target_state);
		int 	StartRaid();
//		int 	AutoChooseChar();
		int 	MappingSuccess_Re_Timeout();
	};

	//faction pk raid
	class FactionPkRaid_Info:public RaidBasic_Info
	{
		int attacker;
		int defender;
		std::vector<int> invites;//roleids
		int inviter_roleid;
		typedef std::map<int/*roleid*/,int/*groupid*/> RoleGroup;
		RoleGroup role_group;
		int group_size[RAIDTEAM_END+1];
		Octets defrole_name;
		Octets attfaction_name;
		Octets deffaction_name;
		typedef std::map<int/*roleid*/,GFactionPkRaidRecord> Scores;
		Scores attacker_score;
		Scores defender_score;
		int end_time;
		unsigned int bonus;
	public:
		const static int INVITE_TIMEOUT = 300;
		const static int INIT_PK_TIME = 3300;
		const static unsigned int FACTION_MONEY_MAX=2000000000;
		enum STATUS_EX
		{
			RAID_WAIT_INVITE_RE = RAID_BEGIN,
			RAID_ENDING,
			RAID_MAX,
		};
		enum INVITE_ANSWER
		{
			RAID_REFUSE_INVITE=0,
			RAID_AGREE_INVITE,
		};
	
		FactionPkRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo, int attacker_fid, int defender_fid,int inviter,\
				const Octets& def_role_name, const Octets& att_fac_name,  const Octets& def_fac_name);
		
		virtual ~FactionPkRaid_Info();
		virtual int 	Update(int now);//更新状态
		virtual int 	OnGsStart(int gs_id,int index,bool success);//gs答复开始副本申请
		int 	OnInviteRe(int roleid, int attacker, int defender, int map_id, char result);
		int 	OnCancelChallenge(int cancel_fid, int dst_factionid, int map_id);
		int 	OnFactionPkCancelChallenge(int roleid, unsigned int dst_factionid, int map_id);
		int 	OnFactionPkKill(int killer, int victim, int victim_fid, int map_id);
		int 	FactionPkJoin(const RaidRole& role, int mapid,int groupid);
		void 	AddInvited(int roleid){invites.push_back(roleid);}//收到帮战请求的人，帮主或副帮主
		virtual int 	DelRole(int roleid,unsigned char teamid);//删除人员
		bool GetRoomBrief(GFactionPkRoomInfo& info,int mapid);
		int SetRoleGroup(int roleid,char& groupid);//设置角色帮战阵营
		int FactionPkRaidGetScore(int mapid,int roleid,FactionPkRaidGetScore_Re& msg);
		int OnFactionPkNotify(int end_time, bool is_start);//gs通知帮战时间
		int NotifyRolePkFaction(int factionid,int roleid);
		void SetFactionName(int fid, const Octets & newname);
		void AddBonus(unsigned int delta)//增加帮战奖金
		{
			if(delta < FACTION_MONEY_MAX)
			{
				bonus += delta;
				if(bonus > FACTION_MONEY_MAX)
					bonus = FACTION_MONEY_MAX;
			}
			else
				bonus = FACTION_MONEY_MAX;
		}
	private:
		virtual void 	SetStatusEndTime(int status_);
		virtual bool 	CheckStateChange(int target_state);
		bool IsValidKill(int killer_fid,int victim_fid);
		void 	RoomClose();//override
		void BroadCastMsg(int messageid);
		void BroadcastResult();
		int 	UpdateScore(int roleid, int factionid, bool iskiller);
		int 	StartRaid();
		int 	PkInvite_Re_Timeout();
		void 	HandleBonus();
		int 	GetRemainTime();
	};

	// 普通副本
	class RaidRoom_Info:public RaidBasic_Info
	{
		typedef RaidBasic_Info::TeamType TeamType;		
		int leader;//master roleid
		int leader_line;//master line id
		TeamType team1_apply;//报名队伍

		const Octets roomname;
		RaidVote *pvote;
		int lastvote_begin_time;
		char permit_vote;
	public:
		const static int VOTE_TIMEOUT = 45;
		const static int VOTE_INTERVAL = 120;
		enum VOTE_RESULT
		{
			VOTE_FAIL 	= 0,	
			VOTE_PASS 	= 1,
		};
		enum STATUS_EX
		{
			RAID_QUEUEING 	= RAID_BEGIN,
			RAID_MAX,
		};
		RaidRoom_Info(int raidroomid,int mapid,int leaderid,int leaderline,const Octets& room_name, RaidMap_Info & mapinfo,char can_vote, char difficulty)
			: RaidBasic_Info(raidroomid,mapid,mapinfo,difficulty),leader(leaderid),leader_line(leaderline),roomname(room_name),pvote(NULL),\
			  lastvote_begin_time(0),permit_vote(can_vote)
		{
		 	SetStatus(RAID_QUEUEING);
		}
		
		virtual ~RaidRoom_Info();
		bool 	PermitVote(){return permit_vote;}
		unsigned int 	GetRoleNum(){return team1.size()+team1_apply.size();}
		virtual int 	OnGsRaidEnterSuccess(int roleid,int gs_now,int index_);//gs通知角色进入副本成功
		int 	DelRole(int roleid,unsigned char teamid);//删除人员
		int 	UpdateRoomToMaster();//更新房间给房主
		int     JoinRaid(const RaidRole& role, int map_id);//申请加入
		int	Kick(int roleid, int target, int reason);//房主踢人
		int	OnAccept(int roleid, unsigned char accept, int target);//房主是否批准加入
		int	Appoint(int roleid, int target,bool auto_appoint);//更换房主
		int 	Start(int roleid);//申请开始副本
		bool 	GetRoomBrief(GRaidRoomInfo& info);//查询房间详细信息
		int 	GetRoomInfo(int roleid, RaidGetRoom_Re & re);//查询房间列表
		virtual int 	Update(int now);//更新状态
		void 	GetVoteResult();
		void 	VoteTimeoutResult();//投票超时
		int 	OnVote(int voted_roleid,int voter,bool agree);
		int 	BeginVote(int voted_roleid,int begin_roleid);
		virtual int OnGsStart(int gs_id,int index,bool success);//gs答复开始副本申请
private:
		virtual void 	SetStatusEndTime(int status_);
		virtual bool 	CheckStateChange(int target_state);
		int	KickNotifyAll(int reason);//override
		void  	ClearApplyList();
		int 	AutoStart();
		void 	SendReq(int roleid);
		void 	RoomClose();//override
		bool 	AddApplyRole(const RaidRole & role){ 
			if(FindRole(team1_apply,role.roleid)!=team1_apply.end()||FindRole(team1,role.roleid)!=team1.end())
				return false;
			team1_apply.push_back(role);
			return true;
		}

	};

	// 捉迷藏类副本
	class HideSeekRaidBase;
	class HideSeekRaid_Info:public RaidBasic_Info
	{
		RoleSet role_need_mapping_re;//role need TeamRaidMappingSuccess_Re
		//HideSeekRaidBase * teama;
	public:
		const static int ACK_TIMEOUT = 30;
		const static int MIN_ENTER_NUM = 4;
		const static int TEAM_REENTER_LIMIT = 300;
		enum STATUS_EX
		{
			RAID_WAITACK 	= RAID_BEGIN,
			RAID_STARTFAIL,
			RAID_MAX,
		};
	
		HideSeekRaid_Info(int raidroomid,int mapid, RaidMap_Info & mapinfo);
		virtual ~HideSeekRaid_Info();
		void Init(HideSeekRaidBase * teama);
		virtual int 	Update(int now);//更新状态
		int OnMappingSuccess_Re(int roleid, char agree);//同意参加
		virtual int OnGsStart(int gs_id,int index,bool success){return RaidBasic_Info::OnGsStart(gs_id,index,success);}//gs答复开始副本申请
		virtual int 	DelRole(int roleid,unsigned char teamid);//删除人员
		virtual int 	GetReenterLimit() { return TEAM_REENTER_LIMIT; }
	private:
		int     StartRaid();
		virtual void 	SetStatusEndTime(int status_);
		virtual bool 	CheckStateChange(int target_state);
		int 	MappingSuccess_Re_Timeout();
	};

	class RaidVote
	{
		friend class RaidRoom_Info;
		typedef std::map<int /*role_id*/, bool> RoomVoteMap;
		RoomVoteMap room_vote_map;
		int voted_roleid;//接受投票决定是否踢出的玩家
		void ClearRole(int roleid){room_vote_map.erase(roleid);}
		int GetVotedRoleid() const{return voted_roleid;}
		RaidVote(int voted):voted_roleid(voted){}
		~RaidVote(){}
	};
	/*参与副本的玩家的信息*/
	class RoleRaid_Info  
	{
		int raidroom_id;
		int timeout;//状态结束时刻
		unsigned char count;//断线重入计数
		const static int ROLE_NOTIMEOUT=-1;
	public:
		RoleRaid_Info(int roomid,int timeout_):raidroom_id(roomid),timeout(timeout_),count(0){}
		int GetRoomId()const{
//			LOG_TRACE("Raid GetRoomId = %d",raidroom_id);
			return raidroom_id;
		}
		void SetTimeout(int t){timeout=t;}
		bool CheckCountLimit(){
			LOG_TRACE("Raid CheckCountLimit count = %d raidroom_id=%d",count,raidroom_id);
			if(count>2)return false;
			count++;
			return true;
		}
		void ClearTimeout(){count=0;timeout=ROLE_NOTIMEOUT;}
		bool IsTimeout(int now){
			if(timeout == ROLE_NOTIMEOUT) return false;
			return now>timeout;
		}
		int GetTimeout(){return timeout;}
	};

	class ApplyingRole
	{
	public:
		TeamRaidApplyingRole role;
		ApplyingRole(){}
		ApplyingRole(const TeamRaidApplyingRole & role_, int map_id);
		virtual ~ApplyingRole();//release lock
		int GetRoleid() const {return role.roleid;}
	};

	//6v6 Team Raid
	class CrssvrTeamsApplyingRole : public ApplyingRole
	{
	public:
		CrssvrTeamsApplyingRole(const TeamRaidApplyingRole & role_, int map_id, int teamid);
		virtual ~CrssvrTeamsApplyingRole();//release lock
		int GetTeamId() const {return teamid;}
		int teamid;
	};

	const static int TEAMRAID_SCORE_RANGE_BASE = 50;
	typedef std::vector<ApplyingRole*> TeamRoleList;

	// 捉迷藏类副本 - 人数达到xx开启
	class WaitingHideSeekContainer;
	class HideSeekRaidBase
	{
	private:
		int id;
		TeamRoleList rolelist; 
		int type;
		WaitingHideSeekContainer * container;
		time_t create_time;
		static int nextid;
	public:
		explicit HideSeekRaidBase(const TeamRoleList & roles, int type, WaitingHideSeekContainer * con)
			: rolelist(roles)
			, type(type)
			, container(con)
		{
			id = nextid++;
			create_time = Timer::GetTime();
		}
		virtual ~HideSeekRaidBase(){/*LOG_TRACE("HideSeekRaidBase::~HideSeekRaidBase");*/}
		virtual int GetType() {  return type; }
		int GetId() const {return id;}
		size_t GetSize() const {return rolelist.size();}
		time_t GetCreateTime() { return create_time; }
		const TeamRoleList & GetRoleList() const {return rolelist;}

		bool DelRole(int roleid)
		{
			//LOG_TRACE("HideSeekRaidBase::DelRole roleid=%d", roleid);
			TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
			for (; it != ite; ++it)
			{
				if ((*it)->GetRoleid() == roleid)
				{
					delete *it;
					rolelist.erase(it);
					return true;
				}
			}
			return false;
		}
		void DeleteRoleList()
		{
			LOG_TRACE("HideSeekRaidBase::DeleteRoleList id=%d, role size=%d", GetId(), GetSize());
			TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
			for (; it != ite; ++it)
				delete *it;
			rolelist.clear();
		}

		/*bool BroadCast(Protocol & data, unsigned int & localsid, int & t_roleid)
		{
			GDeliveryServer * dsm = GDeliveryServer::GetInstance();
			
			TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
			for (; it != ite; ++it)
			{
				PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);

				localsid = it->localsid;
		                t_roleid = (*it)->GetRoleid();
	                        dsm->Send(it->linksid, data);
			}
			return true;
		}*/

	};


	// 两队匹配类副本
	class WaitingTeamContainer;
	class RaidTeamBase
	{
	public:
		int id;
		int ave_score;
		int score_range_delta;
		TeamRoleList rolelist;
		WaitingTeamContainer * container;
//		bool del_from_pool;//是否要从pool中删除
		time_t create_time;
		std::map<unsigned char, int> occup_count;
		virtual void CalAveScore() = 0;
		virtual int  GetTeamId() { return 0; }
		virtual int GetSrcZoneId() { return 0; }
		virtual int GetTeamScore() { return 0; }

		void CalOccupCount()
		{
			occup_count.clear();
			TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
			for (; it != ite; ++it)
				occup_count[(*it)->role.occupation]++;
		}

		static int nextid;
	public:
		const static int OCCUPATION_PLAYER_LIMIT=2;//每种职业人数不能超过2个
		const static int STRICT_MATCH_TIMES=6;//前6次只扩大自己的匹配区间 超过之后会扩大目标的匹配区间
		explicit RaidTeamBase(const TeamRoleList & roles, /*int t,*/WaitingTeamContainer * con);
		virtual ~RaidTeamBase();
		virtual int GetType() { assert(false); return 0; }
		int GetId() const {return id;}
		int GetScore() const { return ave_score; }
		WaitingTeamContainer * GetContainer() { return container; }
		time_t GetCreateTime() { return create_time; }
		const TeamRoleList & GetRoleList() const {return rolelist;}

		void GetRoleIdList( std::vector<int>& roleid_vec )
		{
			TeamRoleList::iterator rit, rite = rolelist.end();
			for (rit = rolelist.begin(); rit != rite; ++rit)
			{
				roleid_vec.push_back((*rit)->GetRoleid());
			}
		}

		size_t GetSize() const {return rolelist.size();}
		std::map<unsigned char, int> GetOccupCount() { return occup_count; }
		virtual void ExtendRange() = 0;
		virtual int GetExtraDelta() = 0;
		bool ScoreMatch(int score, int extra_delta = 0) const
		{
			//LOG_TRACE("RaidTeamBase::ScoreMatch ave_score=%d, score_range_delta=%d, score=%d, extra_delta=%d, a=%d, b=%d", ave_score, score_range_delta, score, extra_delta, ave_score-score_range_delta-extra_delta, ave_score+score_range_delta+extra_delta);

			return score >= ave_score-score_range_delta-extra_delta
				&& score <= ave_score+score_range_delta+extra_delta;
		}
		bool DelRole(int roleid)
		{
			LOG_TRACE("RaidTeamBase::DelRole roleid=%d", roleid);
			TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
			for (; it != ite; ++it)
			{
				if ((*it)->GetRoleid() == roleid)
				{
					delete *it;
					rolelist.erase(it);
					CalAveScore();
					CalOccupCount();
					return true;
				}
			}
			return false;
		}
		void DeleteRoleList()
		{
			LOG_TRACE("RaidTeamBase::DeleteRoleList teamid=%d", GetId());
			TeamRoleList::iterator it = rolelist.begin(), ite = rolelist.end();
			for (; it != ite; ++it)
				delete *it;
			rolelist.clear();
		}
		void BroadRoleList(Protocol & data, unsigned int & localsid, int & t_roleid);
		bool OccupMatch(const std::map<unsigned char, int> & occup_count_r)
		{
			std::map<unsigned char, int> occup_count_total(occup_count_r);
			std::map<unsigned char, int>::const_iterator it, ite = occup_count.end();
			for (it = occup_count.begin(); it != ite; ++it)
			{
				occup_count_total[it->first] += it->second;
				if (occup_count_total[it->first] > OCCUPATION_PLAYER_LIMIT)
				{
					//LOG_TRACE("RaidTeamBase::OccupMatch retur false");
					return false;
				}
			}
			//LOG_TRACE("RaidTeamBase::OccupMatch retur true");

			return true;
		}

	};

	class RaidTeamFrag : public RaidTeamBase //可能是队伍片段 也可能是一只满人队
	{
		virtual int GetType() {  return TEAM_RAID; }
		virtual void CalAveScore()
		{
			if (rolelist.empty())
			{
				ave_score = 0;
				return;
			}
			int score_sum = 0;
			TeamRoleList::iterator it, ite = rolelist.end();
			for (it = rolelist.begin(); it!=ite; ++it)
				score_sum += (*it)->role.score;
			ave_score = score_sum/rolelist.size();
			LOG_TRACE("RaidTeamFrag::CalAveScore score=%d teamsize=%d", ave_score, rolelist.size());
		}
	public:
		explicit RaidTeamFrag(const TeamRoleList & roles, /*int t,*/WaitingTeamContainer * con) : RaidTeamBase(roles, con)
		{
			CalAveScore();
			CalOccupCount();
		}
		virtual ~RaidTeamFrag(){}
		virtual void ExtendRange()
		{
			score_range_delta += TEAMRAID_SCORE_RANGE_BASE;
			//LOG_TRACE("RaidTeamFrag::ExtendRange score_range_delta=%d", score_range_delta);
		}
		virtual int GetExtraDelta()
		{
			//LOG_TRACE("RaidTeamFrag::GetExtraDelta");
			int extend_times = score_range_delta / TEAMRAID_SCORE_RANGE_BASE;
			if (extend_times > STRICT_MATCH_TIMES)
				return (extend_times-STRICT_MATCH_TIMES) * TEAMRAID_SCORE_RANGE_BASE;
			else
				return 0;
		}
	};

	class CrssvrTeamsFrag : public RaidTeamBase // 跨服战队6V6
	{
		typedef std::pair<int, int> ScorePeriod;
		typedef std::map<ScorePeriod, int> OpenScorePeriods;
	public:
		int teamid;
		int src_zoneid;
		int teamscore;
		OpenScorePeriods scoreperiod;

		virtual int GetSrcZoneId() { return src_zoneid; }
		virtual int GetTeamScore() { return teamscore; }
		virtual int GetType() {  return CRSSVRTEAMS_RAID; }
		virtual void CalAveScore()
		{
			ave_score = teamscore;
			LOG_TRACE("CrssvrTeamsFrag::CalAveScore score=%d", ave_score);
		}
	public:
		explicit CrssvrTeamsFrag(const TeamRoleList & roles, /*int t,*/WaitingTeamContainer * con, int teamid, int zoneid, int teamscore) : RaidTeamBase(roles, con), teamid(teamid), src_zoneid(zoneid), teamscore(teamscore)
		{
			CalAveScore();
			CalOccupCount();
			scoreperiod[std::make_pair(0   , 1200)] = 300;
			scoreperiod[std::make_pair(1201, 1500)] = 150;
			scoreperiod[std::make_pair(1501, 1900)] = 50;
			scoreperiod[std::make_pair(1901, 2000)] = 30;
			scoreperiod[std::make_pair(2001, 2200)] = 15;
			scoreperiod[std::make_pair(2201, 9999)] = 10;
		}
		virtual ~CrssvrTeamsFrag(){}

		int GetScorePeriod(int score)
		{
			OpenScorePeriods::const_iterator it, ite = scoreperiod.end();
			for (it = scoreperiod.begin(); it != ite; ++it)
			{
				if (score >= it->first.first && score <= it->first.second)
				{
					return it->second;
				}
			}

			return 0;
		}

		virtual void ExtendRange()
		{
			score_range_delta += GetScorePeriod(teamscore);
			LOG_TRACE("CrssvrTeamsFrag::ExtendRange=%d, teamscore=%d", score_range_delta, teamscore);
		}
		virtual int GetExtraDelta()
		{
			int score_range = GetScorePeriod(teamscore);
			if(score_range<=0)
			{
				Log::log( LOG_ERR,"CrssvrTeamsFrag::GetScorePeriod=%d, teamscore=%d", score_range, teamscore);
				return 0;
			}
			int extend_times = score_range_delta / score_range;
			LOG_TRACE("CrssvrTeamsFrag::GetExtraDelta, score_range_delta=%d, extend_times=%d", score_range_delta, extend_times);
			if (extend_times > STRICT_MATCH_TIMES)
			{
				LOG_TRACE("CrssvrTeamsFrag::GetExtraDelta return %d", (extend_times-STRICT_MATCH_TIMES) * score_range);
				return (extend_times-STRICT_MATCH_TIMES) * score_range;
			}
			else
			{
				LOG_TRACE("CrssvrTeamsFrag::GetExtraDelta return 0");
				return 0;
			}
		}
		virtual int  GetTeamId() { return teamid; }
	
	};

	//const static size_t TEAMRAID_MEMBER_LIMIT = 6;//each side
	enum
	{
		TEAM_RAID_NORMAL,// 普通战场
		TEAM_RAID_REBORN,// 飞升战场
	};

	// 捉迷藏类副本的管理器
	class WaitingHideSeekContainer
	{
	public:
		const static int CHECK_INTERVAL = 10;
		const static int CLOSING_TIME = 600;
		const static int PRINT_INFO_INTERVAL = 30;
		typedef std::list<HideSeekRaidBase*> TeamList; 
		typedef std::list<HideSeekRaidBase*>::iterator TeamListIT;

		typedef TeamList FullTeamList;
		//typedef std::map<int/*roleid*/> SubTeamReverseMap;
		
		typedef std::map<int /*id*/, HideSeekRaidBase* > SubTeamReverseMultMap;
		typedef std::map<int /*roleid*/, int /*id*/> RoleRaidBaseMap;
		//typedef std::multimap<int /*time*/, HideSeekRaidBase* > SubTeamReverseMultMap;
		
	public:
		WaitingHideSeekContainer(int map_id, int type, int member_limit) : map_id(map_id), type(type), limit_member(member_limit), subteamcount(0)
		{
			next_print_time = Timer::GetTime() + PRINT_INFO_INTERVAL;
		}
		~WaitingHideSeekContainer()
		{
			subteamcount = 0;
			ClearAll();
		}
		
		void 	SetMemberLimit(int limit) { limit_member = limit; }
		int 	AddTeam(HideSeekRaidBase * team, bool match_later=false);
		int 	AddFullTeam(HideSeekRaidBase * fullteam, bool match_later=false);
		int 	Update(int now);
		int 	RoleQuit(int roleid);
		void 	ClearAll();
	private:
		int map_id;
		int type;
		int limit_member;
		//FullTeamList fullteamlist; // 已满的队伍列表
		SubTeamReverseMultMap subteam_reverse_map; // 未满的队伍列表 - 最终此队列是要满的
		int subteamcount;
		SubTeamReverseMultMap prepare_reverse_map; // 等待的队伍列表
		RoleRaidBaseMap role_raidbase_map; // roleid 对应的HideSeekRaidBase id 映射, 只在等待匹配时有效
		int next_print_time;	
	private:
		HideSeekRaidBase * CreateFullTeam();
		void 	DelFromPrepareList(int id, int roleid);
		void 	DelFromSubList(int id, int roleid);
		void 	DelAllFromPrepareList(int id);
		void 	DelAllFromSubList(int id);


		int 	GetPrepareMapSize();// 等待队伍列表中玩家个数
		int AddSubMap(HideSeekRaidBase* subteam);
		int AddPrepareMap(HideSeekRaidBase* subteam);
		void AddRoleRaidBaseMap(HideSeekRaidBase* subteam);
		void EraseRoleRaidBaseMap(HideSeekRaidBase* subteam);
		void EraseRoleRaidBaseMap(HideSeekRaidBase* subteam, int roleid);

		int GetRoleHideSeekRaidId(int roleid);
		HideSeekRaidBase* GetHideSeekRaid(int id);
	};

	// 6v6类的队伍管理器
	class WaitingTeamContainer
	{
	public:
		enum
		{
			STATUS_CLOSED,
			STATUS_OPEN,
			STATUS_CLOSING,
		};
		const static int CHECK_INTERVAL = 10;
		const static int CLOSING_TIME = 600;
		typedef std::list<RaidTeamBase*> TeamList;
		typedef std::list<RaidTeamBase*>::iterator TeamListIT;
		typedef std::map<int/*team size*/, TeamList> SubTeamPool;//不完整team
		typedef TeamList FullTeamList;
		typedef std::multiset<int> PolicySet;
		typedef std::vector<PolicySet> PolicyList;
		typedef std::map<int, PolicyList > TeamPolicy;
		typedef std::multimap<int /*time*/,std::pair<int/*size*/, int/*teamfrag_id*/> > SubTeamReverseMap;
		typedef std::multimap<int /*time*/,int/*teamfrag_id*/> FullTeamReverseMap;
		typedef std::pair<int/*begin_time*/, int/*end_time*/> TimePeriod;
		typedef std::vector<TimePeriod> OpenTimePeriods;

		int 	AddTeam(RaidTeamBase * team, bool match_later=false);
		int 	AddSubTeam(RaidTeamBase * subteam, bool match_later=false);
		int 	AddFullTeam(RaidTeamBase * fullteam, bool match_later=false);
		WaitingTeamContainer(int mapid):map_id(mapid)
		{/*BuildTeamMappingPolicy(teampolicies);*/
			//必须按时间顺序从小到大
			open_times.push_back(std::make_pair(43200, 50400));//12:00-14:00
			open_times.push_back(std::make_pair(57600, 64800));//16:00-18:00
			open_times.push_back(std::make_pair(72000, 79200));//20:00-22:00
			time_t now = Timer::GetTime();
			time_t endtime = 0;
			if (IsOpenTime(now, endtime))
			{
				status = STATUS_OPEN;
				status_end_time = endtime;
			}
			else
			{
				status = STATUS_CLOSED;
				status_end_time = FindNextOpenTime(now);
			}
		}
		int 	Update(int now);
		int 	RoleQuit(int roleid);
		int	CrssvrTeamsQuit(int id);
		~WaitingTeamContainer();
		void 	DelFromSubTimeList(const std::set<int> & ids);
		void 	DelFromFullTimeList(int id);
		bool 	IsOpenTime(time_t now, time_t & end_time);
		time_t 	FindNextOpenTime(time_t now);
		void 	OnClosing(time_t now);
		void 	OnClosed(time_t now);
		void 	OnOpen(time_t now);
		void 	UpdateStatus(time_t now);
		void 	ClearAll();
		bool	IsJoinTime();
		static TeamPolicy teampolicies;
		static void 	BuildTeamMappingPolicy(/*TeamPolicy & policies*/);
	private:
		int map_id;
		int status;
		time_t status_end_time;
		SubTeamPool subteampool;
		FullTeamList fullteamlist;
		SubTeamReverseMap subteam_reverse_map;
		FullTeamReverseMap fullteam_reverse_map;
		OpenTimePeriods open_times;
		RaidTeamBase * CreateFullTeam(const TeamList & hybrid_team);
	};

	/*副本全局信息管理*/
	class RaidManager : public IntervalTimer::Observer
	{
	public:
		typedef std::map<int /*map_id*/, RaidMap_Info*> RaidMap;
		typedef std::map<int /*raidteam_id*/,RaidBasic_Info*> RaidRoomMap;
		typedef std::map<int /*roleid*/, RoleRaid_Info> RaidRoleMap;
		typedef std::map<int64_t /*gsid<<32|index*/, int /*raidteam_id*/> RaidInstanceMap; 
		typedef std::multimap<int /*time*/,int/*raidroom_id*/> RaidRoomReverseMap;
		typedef std::multimap<int /*time*/,int/*roleid*/> RaidRoleReverseMap;
		typedef std::multimap<int /*time*/,int/*raidroom_id*/> RaidVoteReverseMap;
		typedef std::set<int /*raidroom_id*/> RaidRoomTimeoutSet;
		typedef std::set<int /*gsid*/> RaidLineSet;
		typedef std::set<int /*roleid*/> OpeningRoomRoleSet;
		typedef std::map<int /*map_id*/, std::map<int/*type*/, WaitingTeamContainer*> > RaidWaitingMap;
		typedef std::map<int /*map_id*/, std::map<int/*type*/, WaitingHideSeekContainer*> > RaidHideSeekWaitingMap;
		typedef std::map<int /*roleid*/,int /*map_id*/> TeamApplyingRoleSet;
		typedef std::set<int /*teamid*/> TeamApplyingCrssvrTeamsSet;

		typedef std::map<int /*faction*/,int /*starttime*/> FactionCoolingMap;
		typedef std::map<int/*roleid*/,int /*starttime*/> TeamApplyCoolingMap;
		typedef std::map<int /*faction*/,int /*raidroom_id*/> FactionRoomMap;
		struct AsyncRoleLeave
		{
			int roleid;
			int raidroom_id;
			AsyncRoleLeave(int roleid_, int roomid_):roleid(roleid_),raidroom_id(roomid_){}
		};
		typedef std::vector<AsyncRoleLeave> AsyncLeavingRoleList;
	private:
		const static int INITIAL_ROOM_ID=1;//初始房间号
//		const static int RAID_REENTER_LIMIT=600;//seconds
		const static int ROLE_NOTIMEOUT=-1;
		const static int RAID_ROOMNAME_LIMIT=26;//12 wchar and 1 end token
		const static int FACTION_CHALLENGE_COOLDOWN_LIMIT=300;//seconds
		const static int TEAM_APPLY_COOLDOWN_LIMIT=600;//seconds
		const static int TEAM_APPLY_COOLDOWN_LIMIT_DEBUG=180;//seconds
		const static int FACTION_CHALLENGE_PROSPERITY=50;//seconds
		RaidMap raidmap;//副本地图表
		RaidRoomMap raidroom_map;//副本房间表
		RaidRoleMap raidrole_map;//参与副本的角色表
		RaidInstanceMap raidinstance_map;//副本实例索引
		RaidRoomReverseMap raidroom_reverse_map;//room事件观察表
		RaidRoleReverseMap raidrole_reverse_map;//role事件观察表
		RaidVoteReverseMap raidvote_reverse_map;//vote事件观察表
		AsyncLeavingRoleList async_leaving_role_list;//需要踢出的角色
		RaidLineSet line_set;//副本线
		OpeningRoomRoleSet opening_room_role_set;//正在申请开房间的角色加锁表
		int next_roomid;//下一个可用房间号
		RaidWaitingMap crssvrteams_waiting_map; // 跨服6v6战队等待队伍表
		RaidWaitingMap raid_waiting_map;//普通6v6等待队伍表
		RaidHideSeekWaitingMap raid_hideseek_waiting_map; // 捉迷藏类副本等待队伍列表
		TeamApplyingRoleSet team_applying_role_set;//排队参加组队副本的角色表
		TeamApplyingCrssvrTeamsSet crssvrteams_applying_set; // 排队参加跨服6v6战队副本的teamid表
		FactionCoolingMap coolings;//帮派战冷却
		TeamApplyCoolingMap team_coolings;//组队副本报名冷却
		FactionRoomMap faction_room_map;//帮派与帮派战房间映射表
	public:
		static RaidManager * GetInstance(){
			static RaidManager inst;
			return &inst;
		}
		static int GetRoleInfo(int roleid,RaidRole& role);
		virtual ~RaidManager();

		bool	Initialize();
		bool 	Update();
//		bool 	CheckAll(int now);

		bool SetTimeout(int roleid,int t);
		int 	RegisterServer(int sid, int gs_id, const std::vector<RaidFieldInfo> & info);//注册副本线
		int 	AsyncLeaveRoom(int roleid,int raidroom_id);
		int 	OnGsRaidEnterSuccess(int roleid,int gs_now,int index);//gs通知角色进入副本成功
		void 	TryAbnormalOffline(int roleid, int gs_id);//异常掉线处理
		void 	AddRoomEvent(int raidroom_id,int eventtime);//add room事件
		void 	DelRoomEvent(int raidroom_id,int eventtime);//delete room事件
		int 	LeaveRoom(int roleid, int raidroom_id, unsigned char team);//role离开房间
		bool 	TryReenterRaid(int roleid, int& gs_id, int& world_tag);//断线重入
		void 	SetReenterRole(int roleid, const PlayerInfo& role);
		int 	GSRoleLeave(int gs_id, int roleid);//gs通知角色离开副本
//		int 	TryLeaveRaid(int gs_id, int roleid, int reason);
		int 	RaidStart(int roleid, int raidroom_id, int raid_type, const Octets & raid_data); //申请开启副本
		int 	OnGSRaidStart(int gs_id, int raidroom_id, int index, int retcode);//gs答复开启副本申请
		int 	RaidEnter(int roleid, int raidroom_id);//申请进入副本
		int 	OnGsRaidEnter(int roleid,int gs_now);//gs答复进入副本申请
		void 	OnLogout(int roleid, int gs_id);//role logout
		int 	OnGSEnterErr(int gs_id, int index, int roleid);//gs否决进入副本
		int     DebugRaidEnd(int roleid, int result);// 通过debug命令调整胜利场次
		int 	OnRaidEnd(int gs_id,int index,int mapid, int result);//gs通知副本结束
		void 	OnDisconnect(int gs_id);//gs断开
		bool 	GetMapid(int roleid,int & raid_mapid);
		bool 	IsRaidLine(int gs_id){return line_set.find(gs_id)!= line_set.end();}
		int64_t GetInstanceMapKey(int gs_id,int index){return (int64_t(gs_id)<<32) + index;}
		void 	ClearRole(int roleid){
			LOG_TRACE("Raid ClearRole %d",roleid);
			raidrole_map.erase(roleid);
		}
		void 	ClearInstance(int gs_id,int index){
			int64_t key = GetInstanceMapKey(gs_id,index);
			raidinstance_map.erase(key);
		}
		int     AddRaidRoom(int map_id,int type);
		int     DecRaidRoom(int map_id,int type);
		int 	OnGRaidJoin(int roleid, int map_id, int raidroom_id, int groupid=0);//申请加入房间
		//method for RoomRaid
		void 	AddVoteEvent(int raidroom_id,int eventtime);
		void 	DelVoteEvent(int raidroom_id,int eventtime);
		int 	GetRoomList(int roleid, int map_id, std::vector<GRaidRoomInfo> & fields);//查询房间列表
		int 	GetRoomInfo(int roleid, int raidroom_id, RaidGetRoom_Re & re);//查询房间详细信息
		int 	OpenRaid(int roleid, int gs_id, int map_id, int item_id, int item_count,GMailSyncData & syncdata, Octets& roomname,char can_vote, char difficulty = 0);//申请新建房间
		int     OnDBRaidOpen(int retcode,int roleid, int gs_id, int map_id,GMailSyncData & syncdata, Octets& roomname,char can_vote, char difficulty = 0);//db响应新建房间
		int	Kick(int roleid, int raidroom_id, int target, int reason);//房主踢人
		int	OnAccept(int masterid, unsigned char accept, int raidroom_id, int target);//房主是否同意target加入
		int	Appoint(int roleid, int raidroom_id, int target, bool auto_appoint);//房主更换
		int 	OnVote(int voted_roleid,int voter, bool agree);
		int 	BeginVote(int voted_roleid,int begin_roleid, int raidroom_id);
		//method for Team Apply Raid
		int 	OnTeamRaidMappingSuccess_Re(int roleid, int raidroom_id, char agree);
		int 	OnTeamRaidMappingSuccess(RaidTeamBase * teama, RaidTeamBase * teamb, int map_id);
//		int 	OnTeamRaidUpdateChar(const TeamRaidUpdateChar& msg);
		void    OnRaidTeamApply(int map_id,const std::vector<TeamRaidApplyingRole>& rawroles, bool reborn, int sid, int src_zoneid, int src_gsid);// 普通6v6
		void 	OnRaidTeamApply(int map_id,const std::vector<TeamRaidApplyingRole>& rawroles, bool reborn, bool iscrssvrteams, int sid, int src_zoneid, int src_gsid, int teamid, int issrc);// 战队6v6
                void    AddTeamApplyingRole(int roleid,int map_id){/*LOG_TRACE("RaidManager::AddTeamApplyingRole roleid=%d, map_id=%d",roleid, map_id);*/team_applying_role_set.insert(std::make_pair(roleid,map_id));}
		int     DelTeamApplyingRole(int roleid){/*LOG_TRACE("RaidManager::DelTeamApplyingRole roleid=%d",roleid);*/return team_applying_role_set.erase(roleid);}
                void    AddCrssvrTeamsApplying(int teamid, int roleid=0){LOG_TRACE("RaidManager::AddCrssvrTeamsApplying teamid=%d, roleid=%d",teamid, roleid);crssvrteams_applying_set.insert(teamid);}
		int     DelCrssvrTeamsApplying(int teamid, int roleid=0){LOG_TRACE("RaidManager::DelCrssvrTeamsApplying teamid=%d, roleid=%d",teamid, roleid);return crssvrteams_applying_set.erase(teamid);}

		int 	TeamApplyingRoleQuit(int roleid);
		int 	TeamApplyingRoleQuit(int roleid, int teamid);

		int 	AddTeamRaidRoom(int map_id, int type){return AddRaidRoom(map_id,type);}
		int 	DecTeamRaidRoom(int map_id, int type){return DecRaidRoom(map_id,type);}
		int 	SetTeamRaidLimit(int number);//for debug command
		int     SetHideSeekRaidLimit(int type, int number);//for debug command
		//method for HideSeek Apply Raid
		int 	OnHideSeekRaidMappingSuccess_Re(int roleid, int raidroom_id, char agree);
		int 	OnHideSeekRaidMappingSuccess(HideSeekRaidBase* teama, int map_id);
		void    OnHideSeekRaidApply(int map_id,const std::vector<TeamRaidApplyingRole>& rawroles, HideSeekType raidtype, bool reborn, int sid, int src_zoneid, int src_gsid);
                //void    AddHideSeekApplyingRole(int roleid,int map_id){LOG_TRACE("RaidManager::AddHideSeekApplyingRole roleid=%d, map_id=%d",roleid, map_id);team_applying_role_set.insert(std::make_pair(roleid,map_id));}
		//int     DelHideSeekApplyingRole(int roleid){LOG_TRACE("RaidManager::DelHideSeekApplyingRole roleid=%d",roleid);return team_applying_role_set.erase(roleid);}

		int 	HideSeekApplyingRoleQuit(int roleid);

		//method for Faction Combat Raid
		int 	FactionPkChallenge(int roleid, Octets defender_name, int map_id);
		int 	OnFactionPkCancelChallenge(int roleid, unsigned int dst_factionid, int map_id);
		int 	OnFactionPkInviteRe(int roleid, int attacker, int defender, int map_id, char result);
		int 	OnFactionPkNotify(int end_time, bool is_start, int raidroom_id, int map_id);
		int 	OnFactionPkKill(int killer, int victim, int victim_fid, int index, int map_id);
		void 	AddPkFaction(int factionid,int raidroom_id){faction_room_map[factionid]=raidroom_id;}
		void 	DelPkFaction(int factionid){faction_room_map.erase(factionid);}
		bool 	IsPkFaction(int factionid){
				FactionRoomMap::iterator it = faction_room_map.find(factionid);
				return it!=faction_room_map.end();
		}
		bool 	IsCTWaiting(int roleid){
				RaidWaitingMap::iterator it = crssvrteams_waiting_map.find(roleid);
				return it!=crssvrteams_waiting_map.end();
		}

		int 	SetRoleGroup(int roleid,char& groupid);
		int 	GetFacPkList(int roleid, int map_id, std::vector<GFactionPkRoomInfo> & fields);
		int 	FactionPkRaidGetScore(int raidroom_id,int map_id,int roleid,FactionPkRaidGetScore_Re& msg);
		int 	NotifyRolePkFaction(int factionid,int roleid);
		int 	FactionPkRaidDeposit(int gsid,int index, int map_id, unsigned int delta_money);
		void 	OnFactionNameChange(int fid, const Octets & newname);
//		void 	AddWaitingTeam(RaidTeamFrag * team);
		int 	CheckTeamApplyCD(int roleid);
		void 	SetTeamApplyCD(int roleid);
		void 	ClearTeamApplyCD(int roleid);
//		int 	TryLeaveRoom(int roleid, int raidroom_id, int reason);
		void 	SetDebugMode(bool debug);
	private:
		int 	CanFactionPkChallenge(int attacker_fac, int defender_fac);
		bool ValidTitle(int title){
			return title == TITLE_VICEMASTER || title == TITLE_MASTER;
		}
		bool 	AddRaidMap(int gs_id,const RaidFieldInfo& finfo);
		void 	AddRoleEvent(int roleid,int eventtime);
		void 	DelRoleEvent(int roleid,int eventtime);

		RaidManager():next_roomid(INITIAL_ROOM_ID){}
		void 	AddOpeningRole(int roleid){opening_room_role_set.insert(roleid);}
		int 	DelOpeningRole(int roleid){return opening_room_role_set.erase(roleid);}
		TeamApplyingRoleSet::iterator FindTeamApplyingRole(int roleid){return team_applying_role_set.find(roleid);}
		TeamApplyingCrssvrTeamsSet::iterator FindCrssvrTeamsApplying(int teamid){return crssvrteams_applying_set.find(teamid);}

		void 	AddRaidLine(int gs_id){line_set.insert(gs_id);}
		void 	DelRaidLine(int gs_id){line_set.erase(gs_id);}
		bool 	AddRoleInfo(int roleid, int room_id, int timeout_){
			//LOG_TRACE("Raid AddRoleInfo %d roomid=%d",roleid,room_id);
			return raidrole_map.insert(std::make_pair(roleid,RoleRaid_Info(room_id,timeout_))).second;
		}
		RaidInstanceMap::iterator GetGsInstanceBegin(int gs_id){
			return raidinstance_map.lower_bound((int64_t(gs_id))<<32);
		}
		RaidInstanceMap::iterator GetGsInstanceEnd(int gs_id){
			return raidinstance_map.upper_bound((int64_t(gs_id+1))<<32);
		}
		bool 	GsInstanceRangeValid(RaidInstanceMap::iterator it,RaidInstanceMap::iterator ite){
			return (it!=raidinstance_map.end() && it!=ite);
		}
		int 	GetNextRoomId(){return next_roomid++;}
		int 	GetRoleRoomId(int roleid){
			RaidRoleMap::iterator it = raidrole_map.find(roleid);
			if(it==raidrole_map.end()){
//				LOG_TRACE("Raid GetRole %d RoomId err",roleid);
				return -1;
			}
			return it->second.GetRoomId();
		}
		RaidBasic_Info* GetRoom(int raidroom_id){
			RaidRoomMap::iterator it= raidroom_map.find(raidroom_id);
			if(it== raidroom_map.end())
				return NULL;
			else
				return it->second;
		}
		int 	GetIndexRoomId(int gs_id,int index){
			int64_t key = GetInstanceMapKey(gs_id,index);
			RaidInstanceMap::iterator it = raidinstance_map.find(key);
			if(it==raidinstance_map.end())
				return -1;
			return it->second;
		}
		bool 	AddInstance(int gs_id,int index,int roomid){
			int64_t key = GetInstanceMapKey(gs_id,index);
			return raidinstance_map.insert(std::make_pair(key,roomid)).second;
		}
		int 	CheckExistRole(int roleid);
	};

} // namespace GNET

#endif
