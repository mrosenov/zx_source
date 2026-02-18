#ifndef __GNET_FACTIONMANAGER_H
#define __GNET_FACTIONMANAGER_H

#include <vector>
#include <map>
#include <list>

#include "thread.h"
#include "gfolk"
#include "gmember"
#include "gfamilyskill"
#include "gfamily"
#include "guserfaction"
#include "factionlevel"
#include "familyid"
#include "hostilefaction"
#include "hostileinfo"
#include "gfactioninfo"
#include "guserfaction"
#include "gfactionbaseinfo"
#include "gfamilybrief"
#include "factionlist_re.hpp"
#include "matcher.h"
#include "dbfactionupdatearg"
#include "dbfactionsyncarg"
#include "dbfactionappointarg"
#include "dbhostileupdatearg"
#include "dbhostileprotectarg"
#include "factionhostiles"
#include "syncfactionhostiles.hpp"
#include "factiondynamic.h"
#include "localmacro.h"

namespace GNET
{
void UpdatePlayerFaction(int roleid, int factionid, int familyid, int title, const Octets & facname);
void UpdatePlayerFaction(class PlayerInfo* pinfo, int roleid, int factionid, int familyid, int title, const Octets & facname);

typedef std::map<unsigned int, class FamilyInfo*> FamilyContainer;

enum 
{ 
	E_LOADFINISH, 
	E_ONLISTEN,
};

struct IFactionListener
{
	virtual bool OnEvent(int event, class FactionDetailInfo *) = 0;
};

struct RefListener : public IFactionListener
{
	bool OnEvent(int event, class FactionDetailInfo *);
};

struct BaseInfoListener : public IFactionListener
{
        int roleid;
	BaseInfoListener(int id) : roleid(id) 
	{
	}

        bool OnEvent(int event, class FactionDetailInfo * faction);
};

struct FamilyInfoListener : public IFactionListener
{
	unsigned int familyid;
        bool OnEvent(int event, class FactionDetailInfo * faction);
};

struct SiegeListener : public IFactionListener
{
        bool OnEvent(int event, class FactionDetailInfo * faction);
};

struct TerritoryListener : public IFactionListener
{
        bool OnEvent(int event, class FactionDetailInfo * faction);
};

struct LoginListener : public IFactionListener
{
	int  roleid;
        int  factiontitle;
        int  contribution;
        int  devotion;
	unsigned int familyid;
	char familytitle;
        bool OnEvent(int event, class FactionDetailInfo * faction);
};

class FamilyRole;

class FactionRole 
{
public:
	FactionRole(unsigned int _familyid, FamilyRole &_role)
		: role(&_role), familyid(_familyid)
	{
	}
	FactionRole(unsigned int _familyid, FamilyRole *_role)
		: role(_role), familyid(_familyid)
	{
	}
	FamilyRole * role;
	unsigned int familyid;
};


class FactionDetailInfo
{
	int dirty_mask;
	FactionDynamic dynamic;
	std::map<int/*roleid*/, int/*time_t*/> rolemsg; //记录了当天发过留言的角色的留言时间
	int activity;//活跃度 每周周排行榜排出后清 0 
	int act_uptime;
	int base_status; //基地状态
	bool creating_base;
public:
	enum
	{
		MAX_MEMBER_PERPAGE = 20,
		LOAD_TIMEOUT = 10,
		MSG_PAGE_SIZE = 10,
		MAX_MSG_LENGTH = 1000, //500 汉字
		POST_MSG_CD = 60, //留言冷却 1 分钟
		//MAX_MSG_SIZE = 100, //最多100条留言
	};
	enum HostileDefine
	{
		HOSTILE_MAX = 5,
		HOSTILE_WAIT_TIME = 300,
	};

	GFactionInfo info;
	FamilyContainer families;
	int online;
	int updatetime;
	int access_num;
	int expel_count;
	unsigned int family_count;
	time_t loadtime;
//	bool dirty;
	int ref_cnt;

	typedef std::list<FactionRole> RoleList;
	RoleList rolecache;
	int money_updatetime;

	FactionDetailInfo() : dirty_mask(0), activity(0), act_uptime(0), base_status(0), creating_base(false), access_num(0), expel_count(0), family_count(0), ref_cnt(0)
	{
		updatetime = loadtime = Timer::GetTime();
		money_updatetime = updatetime;
	}
	FactionDetailInfo(GFactionInfo& _info);
	~FactionDetailInfo();

	void SetKeepAlive() { ref_cnt++; }
	void ClrKeepAlive() { ref_cnt--; }

	void Update(bool & isSilent);
	unsigned int ComputeProsperity();
	bool RegFamily(class FamilyInfo * finfo);
	void OnFamilyDismiss(class FamilyInfo * finfo);
	void OnDeleteRole(int roleid, int contribution);
	void OnRoleLeaveFamily(int roleid, int contribution);
	void OnAppoint(int rid, int candidate, char title, char oldtitle);
	void Broadcast(Protocol& data, unsigned int &localsid);
	void BroadcastGS(Protocol& data, int & roleid);
	void SyncMaster(int fid, int roleid, bool set);
	void OnDBFactionAddMoney(unsigned int new_money);
	void OnDBFactionWithDrawMoney(unsigned int new_money);
	int  WithdrawFactionMoney(int roleid);

	bool IsSilent();
	int GetMasterID();

	void GetOnlineMember(std::vector<int> &roles, int gsid);
	void GetMember();
	void GetMemberFlat(int page, FactionList_Re& ret);
	void GetMemberTree(FactionList_Re& ret);
	int GetMemberCount();
	void UpdateRole(int roleid, char title);
	int GetCharm() { return info.charm; }
	void UpdateCharm(int charm);

	void ObsoleteCache() { rolecache.clear(); online = 0;}
	
	void LoadFamilies();
	void Save2DB(bool &isSilent, bool isbatch = false);
	
	typedef std::set<IFactionListener*> ListenerContainer;
	ListenerContainer listeners;
	
	void AddListener(IFactionListener* listener)
	{
		listeners.insert(listener);	
		Event(E_ONLISTEN);
		// 检查是否load完毕，保险措施
		if (IsLoadFinish())
			Event(E_LOADFINISH);
		else 
		{
			time_t now = Timer::GetTime();
			if (now - loadtime > LOAD_TIMEOUT)
			{
				LoadFamilies();
				loadtime = now;
			}
		}

	}
	
	void Event(int event)
	{
		ListenerContainer::iterator it = listeners.begin();
		for (; it != listeners.end(); ) 
		{
			if ((*it)->OnEvent(event, this))
			{
				delete *it;
				listeners.erase(it++);
			}
			else
				++it;

		}
	}
	
	bool IsLoadFinish();
	void CheckIntegration();
	void CheckHostile();
	bool GetMemberinfo(int rid, int& familyid, char& title);
	void OnLoadErrFamily(int familyid);

	void OnDeleteHostile(unsigned int  fid);
	void OnDeleteHostile(std::vector<unsigned int> & hostile);
	void OnAddHostile(int  hostile, Octets & name, bool active = false);
	bool NeedUpdateHostile(time_t now, unsigned char & actionpoint);	
	void GetFactionHostiles(SyncFactionHostiles &sync);

	void SendSiegeMessage(int type, Octets & msg);
	friend class FamilyManager;

	FactionDynamic & GetDynamic()
	{
		return dynamic;
	}
	//角色今天发布留言的时间
	bool GetPostTime(int roleid, int & time) const;
	//角色今天是否发布  过留言
	bool RoleHasPost(int roleid) const;
	//pageid == 0 时才给客户端返回 haspost
	//void GetMsg(int roleid, int pageid, int & totalsize, std::vector<FactionMsgItem> & msgs, char & haspost) const;
	//int PostMsg(int roleid, const Octets & rolename, const Octets & msg, char repost);
	void SetNormalDirty()
	{
		dirty_mask |= FAC_DIRTY_NORMAL;
	}
	void SetDynamicDirty()
	{
		dirty_mask |= FAC_DIRTY_DYNAMIC;
	}
	/*
	void SetMsgsDirty()
	{
		dirty_mask |= FAC_DIRTY_MSGS;
	}
	*/
	void SetActDirty()
	{
		dirty_mask |= FAC_DIRTY_ACT;
	}
	void ClearDirty()
	{
		dirty_mask = 0;
	}
	void SetAct(int act, int act_time, int clear_time);
	void UpdateAct(int delta);
	void CheckClearAct(int clear_time);
	void SetBaseStatus(int s);
	bool IsBaseOpen();
	bool IsBaseStart();
	int CanCreateBase(int roleid);
	int CreateBase(int roleid);
	void OnBaseCreate(int roleid, int ret);
	void OnBaseStart();
	void OnBaseStop();
	void OnStartMultiExp(int multi, int end_time);
	bool GetMultiExp(int & multi, int & endtime);
	void OnDel(time_t deletetime)
	{
		info.status = _ROLE_STATUS_READYDEL;
		info.deletetime = deletetime;

		OnBaseStop(); //关闭基地
	}
	void OnUnDel()
	{
		info.status = _ROLE_STATUS_NORMAL;
		info.deletetime = 0;
	}
	bool IsDeleting()
	{
		return info.status == _ROLE_STATUS_READYDEL;
	}
};

struct FactionSaver
{
	FactionSaver() :curcor(0), isWork(false) {}
	void Reset() { curcor = 0; }
	void Begin() { isWork = true; for(int i = 0; i < 10 && isWork; i ++) DoSave(); }
	void OnSync(int fid) { DoSave(); }
	void DoSave();
private:
	int curcor;
	bool isWork;
};
class FactionManager : public IntervalTimer::Observer
{
	typedef std::map<unsigned int,FactionDetailInfo*>  Map;
	Map factions;
	unsigned int cursor;
	FactionSaver saver;
	bool open;	
	friend class FactionSaver;
	int act_clear_time;

	FactionManager() : cursor(0), open(false), act_clear_time(0), actiontime(0)/*, t_forged(0)*/ { Update();}  
public:
	enum
	{
		UPDATE_INTERVAL    = 300   
	};


	~FactionManager() 
	{ 
		for(Map::iterator it=factions.begin(),ie=factions.end();it!=ie;++it)
			delete it->second;
		factions.clear();
	}

	static FactionManager* Instance() { static FactionManager instance; return &instance;}
	bool Initialize();
	bool Update();
	size_t Size() { return factions.size(); }

	FactionDetailInfo * Find(unsigned int fid)
	{
		Map::iterator it = factions.find(fid);
		return (it == factions.end()) ? NULL : it->second;
	}

	void GetOnlineMember(int fid, std::vector<int> &roles, int gsid);

	void GetMemberFlat(int fid, int page, FactionList_Re& list);
	void GetMemberTree(int fid, FactionList_Re& list);
	int GetMemberCount(int fid);
	bool GetBaseInfo(int fid, GFactionBaseInfo& info);
	bool SetAnnounce(int fid, int rid, Octets& announce);
	int  GetLevels(std::vector<FactionLevel>& list);
	int  GetHostiles(SyncFactionHostiles& sync);
	
//	bool FillSyncArg(int fid, DBFactionSyncArg& arg);
	int  FillAppointArg(DBFactionAppointArg& arg, unsigned char mytitle);
	bool FillUpdateArg(int fid, int roleid, DBFactionUpdateArg& arg);

	void OnLoad(GFactionInfo& info, int act, int act_uptime, int bstatus);
	void OnCreate(GFactionInfo& info);
	void OnFamilyLoad(int fid, FamilyInfo * finfo);
	void OnLeave(int fid, int factionid, int master, int contribution);
	void OnAppoint(int fid, int rid, int candidate, char title, char oldtitle);
	void OnDelete(int fid);
	int  OnRecruit(int fid, Octets& factionname);
	void OnJoin(int inviter, unsigned int fid, unsigned int invitee, Octets& name);
	void OnLogin(int roleid,int factionid,char title,int familyid
			,int contribution,int devotion,int gameid,int linksid,int localsid);
	void OnLogout(int roleid, int factionid, int familyid, int level, int occupation);
	void OnGradeChg(int fid, int roleid, int level);
	bool Broadcast(int fid, Protocol& data, unsigned int &localsid);
	bool BroadcastGS(int fid, Protocol& data, int & roleid);

	bool CanLeave(int familyid, int fid, int& contribution);
	int  CanDismiss(int fid, int master);
	bool CanExpel(int roleid, int fid, int expellee, int& contribution);

	bool ValidName(const Octets& name);
	bool GetName(int fid, Octets& name);
	bool GetBrief(int fid, Octets& name, int& level);
	bool FindMaster(int fid, unsigned int &linksid, unsigned int &localsid);
	bool FindVice(int fid, unsigned int &linksid, unsigned int &localsid);
	bool FindMaster(int fid, int &roleid);
	bool FindVice(int fid, int &roleid);
	bool HasProsperity(int fid, unsigned int dec);
	bool DecProsperity(int fid, unsigned int dec);
	void SetDelete(int fid, time_t deletetime);

	int  GetLevel(int fid) const;
	bool OnProsperityChange(int fid, int pros_delta);
//	void ChatBroadCast(int fid, unsigned char channel, const Octets &msg);
	bool GetMemberinfo(int fid, int rid, int& familyid, char& title);
	void Sync2Game(int rid,int factionid,int familyid,int contribution, int devotion);
	int  GetNimbus(int fid);
	void UpdateNimbus(int fid, int addend);
	void AddFactionMoney(int fid, unsigned int delta_money);
	int  WithdrawFactionMoney(int fid, int roleid);
	void OnDBFactionAddMoney(int fid, unsigned int new_money);
	void OnDBFactionWithDrawMoney(int fid, unsigned int new_money);
	void Save2DB();
	void OnSync(int fid);
	void OnDeleteHostile(int factionid, std::vector<unsigned int> & hostile);
	void OnAddHostile(int factionid, int  hostile, Octets & name);
	void OnHostileUpdate(DBHostileUpdateArg &arg);
	void OnHostileProtect(DBHostileProtectArg &arg);
	bool GetHostileInfo(int fid, HostileInfo &hostileinfo);
	int  CanAddHostile(int fid, int hostile);
	int  CanDeleteHostile(int fid, int hostile);
	bool IsMaster(int fid, int roleid);

	void SendSiegeMessage(int fid, int type, Octets & msg);
	int CanChangeName(int fid, int roleid, const Octets & newname);
	void OnNameChange(int fid, const Octets & newname, char scale);
	void DebugClearCD(int fid);
	void GetDynamic(int fid, int pageid, int & totalsize, std::vector<Octets> & dynamic);
	//void GetMsg(int fid, int roleid, int pageid, int & totalsize, std::vector<FactionMsgItem> & msgs, char & haspost) const; 
	//int PostMsg(int fid, int roleid, const Octets & rolename, const Octets & msg, char repost);
	template <typename T>
	void RecordDynamic(int fid, int action, const T & dynamic)
	{
		Map::iterator it = factions.find(fid);
		if (it == factions.end())
			return;
		it->second->GetDynamic().RecordDynamic(Timer::GetTime(), action, dynamic);
	}
	void UpdateActivity(int fid, int delta)
	{
		Map::iterator it = factions.find(fid);
		if (it == factions.end())
		{
			Log::log(LOG_ERR, "updateactivity but fid %d does not load, delta %d", fid, delta);
			return;
		}
		it->second->UpdateAct(delta);
	}
	void OnActTopUpdate(int fac_act_toptime); //活跃度周榜更新 清空活跃度值
	int CreateBase(int fid, int roleid);
	void OnBaseCreate(int fid, int roleid, int ret);
	void OnBaseStart(int fid);
	void OnBaseStop(int fid);
	int CanCreateBase(int fid, int roleid);
	void OnStartMultiExp(int fid, int multi, int end_time);
	void DebugClearDynamic(int fid);
public:
	int actiontime;
	/*
	int  GetForged() { return t_forged;}
private:
	int t_forged;
	void SetForged(int delta)
	{
		if (delta == 0)
			t_forged = 0;
		else
			t_forged = delta;
	}
	friend class DebugCommand;
	*/

};

};
#endif

