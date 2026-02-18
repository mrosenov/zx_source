#ifndef __GNET_CRSSVRTEAMS_MANAGER_H
#define __GNET_CRSSVRTEAMS_MANAGER_H

#include "mapuser.h"
#include "gcrssvrteamsrole"
#include "gcrssvrteamsinfo"
#include "gcrssvrteamsdata"
#include "crssvrteamsrolelist_re.hpp" 
#include "crssvrteamsteambase_re.hpp"
#include "synccrssvrteams.hrp"
#include "synccrssvrteamsrole.hrp"
#include "crssvrteamsinfo.h"
#include "gtoptable"
#include "crssvrteamsbasemanager.h"

namespace GNET
{
	struct CrssvrTeamsSaver
	{
		CrssvrTeamsSaver() :curcor(0), isWork(false) {}
		void Reset() { curcor = 0; }
		void Begin() { isWork = true; for(int i = 0; i < 10 && isWork; i ++) DoSave(); }
		void OnSync(int fid) { DoSave(); }
		void DoSave();
	private:
		int curcor;
		bool isWork;
	};
		
	class CrssvrTeamsManager : public CrssvrTeamsBaseManager, public IntervalTimer::Observer //原服战队管理器
	{
		friend class CrssvrTeamsSaver;
	private:
		unsigned int cursor;
		CrssvrTeamsSaver saver;

		CrssvrTeamsManager() : cursor(0) { Update();}  
	public:

		virtual ~CrssvrTeamsManager() 
		{ 
		}

		static CrssvrTeamsManager* Instance() { static CrssvrTeamsManager instance; return &instance;}
		bool Initialize();
		bool Update();

		int  OnLoad(int roleid, GCrssvrTeamsData& info, bool isUpdateStatus=true);
		int  OnCreate(int roleid, GCrssvrTeamsData& info);
		//int  OnUpdate(GCrssvrTeamsData& info);
		int  OnDeleteRole(int teamid, int roleid);
		int  OnJoin(int teamid, GCrssvrTeamsRole& roleinfo);
		int  OnDismiss(int teamid);
		int  OnGradeChg(int teamid, int roleid, int level);
		int  OnNameChange(int teamid, int changetime, const Octets & newname);
		int  OnChgCaptain(int teamid, int master_old, int master_new);
		int  OnLogin(int roleid, int teamid = -1, bool isfromcentral = false);
		int  OnLogout(int roleid, int level, unsigned char occupation);
		
		// 战队操作的与判断
		int  CanDismiss(int teamid, int master);
		int  CanKick(int teamid, int master);
		int  CanChgCaptain(int teamid, int master_old, int master_new);
		int  CanChangeName(int teamid, int roleid, const Octets & newname);
		int  CanRecruit(int teamid, int inviter, int invitee, int& invitee_linksid, int& invitee_localsid);
		int  CanCreate(int master);

		// 排行榜相关
		//void SendSeasonTopSave(int updatetime, const GTopTable & toptable); // 保存赛季榜
		void UpdateSeasonPos(int updatetime, const CrssvrTeamsTop& pos_map);// 更新赛季榜
		virtual void OnTopUpdate(const GTopTable & toptable, int toptype); 

		virtual void Save2DB();
		void OnSync(int teamid);

		int  SyncRoleInfo2Central(int roleid, int teamid);
		bool UpdateRoleOnline(int teamid, int roleid, char status);// 更新玩家在线状态
		bool UpdateRolesOnline(int teamid, char status);// 更新玩家在线状态
		bool IsRoleCrossOnline(int teamid); // 是否有玩家跨服在线
		//bool NewSeasonUpdate(int roleid, int teamid);// 新赛季的一些处理

		void SendTeamGetReq(int roleid, int teamid, bool isfromcentral);
	private:
		void UpdateTopTable();

	};

};
#endif

