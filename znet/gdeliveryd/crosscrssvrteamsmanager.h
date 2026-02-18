#ifndef __GNET_CROSSCRSSVRTEAMS_MANAGER_H
#define __GNET_CROSSCRSSVRTEAMS_MANAGER_H

#include "mapuser.h"
#include "gcrssvrteamsrole"
#include "gcrssvrteamsinfo"
#include "gcrssvrteamsdata"
#include "gtoptable"

#include "crssvrteamsinfo.h"
#include "crssvrteamsbasemanager.h"

namespace GNET
{
	struct CrossCrssvrTeamsSaver
	{
		CrossCrssvrTeamsSaver() :curcor(0), isWork(false) {}
		void Reset() { curcor = 0; }
		void Begin() { isWork = true; for(int i = 0; i < 10 && isWork; i ++) DoSave(); }
		void OnSync(int fid) { DoSave(); }
		void DoSave();
	private:
		int curcor;
		bool isWork;
	};
		
	class CrossCrssvrTeamsManager : public CrssvrTeamsBaseManager, public IntervalTimer::Observer //战队管理器
	{
		friend class CrossCrssvrTeamsSaver;
	private:
		unsigned int cursor;
		CrossCrssvrTeamsSaver saver;
		
		CrossCrssvrTeamsManager() : cursor(0) { Update();}  
	public:

		virtual ~CrossCrssvrTeamsManager() 
		{ 
		}

		static CrossCrssvrTeamsManager* Instance() { static CrossCrssvrTeamsManager instance; return &instance;}
		bool Initialize();
		bool Update();

		int  OnLoad(GCrssvrTeamsData& info);
		int  OnCreate(GCrssvrTeamsData& info, int zoneid);
		int  OnAddRole(int teamid, int zoneid, GCrssvrTeamsRole& roleinfo, GCrssvrTeamsScore& data_score);
		int  OnDelRole(int teamid, int roleid, int zoneid);
		int  OnDismiss(int teamid, int zoneid);
		int  OnNameChange(int teamid, const Octets & newname, int zoneid);
		int  OnChgCaptain(int teamid, int master_new, int zoneid);
		int  OnLogin(int roleid,int zoneid,int linksid,int localsid);
		int  OnLogout(int roleid, int zoneid);
		
		void SetRoleBriefs(int teamid, TTeamRaidRoleBrief& rolebrief);

		virtual void Save2DB();
		void OnSync(int teamid);

		// 排行榜相关
		void OnCrossTopUpdate(int table_type, int zoneid = 0);
		void SendSyncTopUpdate(const GTopTable& toptable, int table_type, int zoneid = 0);
		void UpdateTeamInfo(int teamid, GCrssvrTeamsInfo& teaminfo, int zoneid);

		void UpdateSeasonTop(int updatetime, const GTopTable & toptable);// 更新赛季榜
		virtual void OnTopUpdate(const GTopTable & toptable, int toptype);

	private:
		// 同步玩家跨服在线状态
		void SyncRoleOnlineStatus(int teamid, int roleid, char status, int zoneid);

		void SendTeamGetReq(int roleid, int teamid, int zoneid); 
		void SendRoleGetReq(int roleid, int teamid, int zoneid); 

	};

};
#endif

