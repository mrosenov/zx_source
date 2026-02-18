#ifndef __GNET_FAMILYMANAGER_H
#define __GNET_FAMILYMANAGER_H

#include <vector>
#include <map>
#include <list>

#include "thread.h"
#include "gfamilyskill"
#include "gfolk"
#include "gfamily"
#include "factionmanager.h"
#include "asyncdata"
#include "hashstring.h"
#include "TaskAsyncData.h"
#include "localmacro.h"

namespace GNET
{

typedef std::list<class FamilyRole> RoleContainer;

class FamilyRole
{
public:
	int gameid;
	unsigned int linksid;
	unsigned int localsid;
	unsigned char level;
	unsigned char occupation;
	double exp;
	bool dirty;
	int reborn_cnt;
	short cultivation;
	int64_t fac_coupon_add;
	GFolk role;

	FamilyRole(GFolk& r) : gameid(-1),linksid(0),localsid(0),level(r.level),occupation(r.occupation),exp(0),dirty(0),reborn_cnt(-1),cultivation(-1),fac_coupon_add(0),role(r)
	{ 
	}

	FamilyRole(const FamilyRole& r) 
		: gameid(r.gameid),linksid(r.linksid),localsid(r.localsid),level(r.level),
		 occupation(r.occupation),exp(r.exp),dirty(r.dirty),reborn_cnt(r.reborn_cnt),cultivation(r.cultivation),fac_coupon_add(r.fac_coupon_add),role(r.role)
	{ 
	}

	~FamilyRole() 
	{ 
	}

	void SetOnline(int gid,int sid,int lid,int reborn, short cult, int64_t coup)
	{
		gameid = gid;
		linksid = sid;
		localsid = lid;
		reborn_cnt = reborn;
		cultivation = cult;
		fac_coupon_add = coup;
	}

	void SetOffline()
	{
		gameid = -1;
		linksid = 0;
		localsid = 0;
		reborn_cnt = -1;
		cultivation = -1;
	}
	bool IsOnline()
	{
		return gameid!=-1;
	}
};

class FamilyInfo
{
	struct Packer
	{
		static char version;
		static void Pack(FamilyInfo & info, Octets& dst);
		static void Unpack(const FamilyInfo & info, Octets & src);
		
	};
public:
	enum
	{
		TASK_KILLTHRESHOLD = 5000,
	};

	FamilyInfo()
		: pfaction(NULL), online(0), access_num(0), expel_count(0)
		, contribution_cache(-1), dirty(false)
	{

		updatetime = Timer::GetTime();
		expcounter = 0;
		expfactor  = 0.0;
		star = 0;
	}

	FamilyInfo(GFamily & family) 
		: pfaction(NULL), online(0), access_num(0), expel_count(0)
		, contribution_cache(-1), dirty(false)
	{
		for(std::vector<GFolk>::iterator it = family.member.begin(),ie = family.member.end();it!=ie;++it)
                        list.push_back(FamilyRole(*it));
		updatetime = Timer::GetTime();
                family.member.clear();
		Packer::Unpack(*this, family.task_data);
		family.task_data.clear();
                info = family;
		expcounter = 0;
		if(info.skills.size()==0)
		{
			for(int i=1;i<=FAMILYSKILL_SUM;i++)
				info.skills.push_back(GFamilySkill(i,1,0,0));
		}
		expfactor  = 0.0;
		UpdateExpFactor();
		star = 0;
		for(std::vector<int>::iterator it=info.task_record.begin(),ie=info.task_record.end();it!=ie;++it)
			if(*it>=TASK_KILLTHRESHOLD)
				star++;
	}

	~FamilyInfo() { pfaction = NULL;} 

	bool RegFaction(FactionDetailInfo * pf) 
	{ 
		if (!pf) 
			return false;
		if (info.factionid == 0 || pf->info.fid == info.factionid)
		{
			pfaction = pf; 
			return true;
		}
		return false;
	}
	bool UnregFaction(FactionDetailInfo * pf) 
	{ 
		if (pf != pfaction)
			return false;
		pfaction = NULL; 
		return true;
	}

	unsigned int GetContribution() 
	{ 
		if (contribution_cache>=0)
			return contribution_cache;
		unsigned int contribution = 0;
		for(RoleContainer::iterator it = list.begin(); it != list.end(); ++ it)
			contribution += it->role.contribution;
		contribution_cache = contribution;
		return contribution;
	}
	int GetOnline() { return online; }
	void IncOnline() 
	{ 
		//LOG_TRACE("IncOnline, familyid=%d", info.id);
		online++;
	}
	void DecOnline() 
	{ 
		//LOG_TRACE("DecOnline, familyid=%d", info.id);
		online--;
	}

	bool IsSilent();
	void Update(bool &needsync, bool isbatch = false);
	void Broadcast(Protocol& data, unsigned int &localsid);
	void BroadcastGS(Protocol& data, int & roleid);
	void JoinFaction(FactionDetailInfo * pf);
	void LeaveFaction();
	void OnDelete();
	void OnFactionDelete();
	void UpdateExpFactor();
	void SendTaskRecord(int sid, int localsid);
	void SendMessage(int msgid, int data1, int data2);

	void TaskGetSkill(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec);
	void TaskSetSkill(TaskFamilyAsyncData& request);
	void TaskAdd(TaskFamilyAsyncData& request);
	void TaskDone(TaskFamilyAsyncData& request);
	void TaskGet(TaskFamilyAsyncData& request,  std::vector<TaskPairData>& vec);
	void TaskRemove(TaskFamilyAsyncData& request);
	void TaskGetRecord(TaskFamilyAsyncData& request,  std::vector<TaskPairData>& vec);
	void TaskGetFinished(TaskFamilyAsyncData& request,  std::vector<TaskPairData>& vec);
	void TaskUpdateRecord(TaskFamilyAsyncData& request);
	void TaskGetKill(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec);
	void TaskUpdateKill(TaskFamilyAsyncData& request);
	void TaskGetCommon(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec);
	void TaskAddCommon(TaskFamilyAsyncData& request);
	void BroadAsyncData(TaskFamilyAsyncData& data);
	void OnAsyncData(TaskFamilyAsyncData& request, std::vector<TaskPairData>& vec);
	void OnAppoint(int appointer, int appointee, char title);

	bool DeleteCheck();

	typedef std::map<int, TaskPairData> PAIRMAP;
	typedef std::map<int, std::vector<TaskPairData> > PAIRLISTMAP;
	GFamily info;
	RoleContainer list;
	FactionDetailInfo * pfaction;
	int online;
	int access_num;
	int expel_count;
	int   expcounter;
	float expfactor;
	int contribution_cache;
	bool dirty;	
	mutable int updatetime;
	int star;
	PAIRMAP task_done;
	PAIRMAP task_doing;
	PAIRLISTMAP task_counter;
	//std::vector<int> task_record;
};
struct FamilySaver
{
	FamilySaver() :curcor(0), isWork(false) {}
	void Reset() { curcor = 0; }
	void Begin() { isWork = true; for (int i = 0; i < 10 && isWork; i++ ) DoSave(); }
	void OnSync(int fid) { DoSave(); }
	void DoSave();
private:
	int curcor;
	bool isWork;
};
class FamilyManager : public IntervalTimer::Observer
{
	typedef std::map<unsigned int,FamilyInfo*>  Map;
	Map families;
	unsigned int cursor;
	FamilySaver saver;
	bool open;
	typedef __gnu_cxx::hash_map<Octets, int> Name2ID;
	//Name2ID name2id;

	FamilyManager() : cursor(0), open(false){}  
	friend class FamilySaver;
public:
	enum
	{
		MAX_MEMBER_PERPAGE = 20,
		UPDATE_INTERVAL    = 300   
	};

	~FamilyManager(); 

	static FamilyManager* Instance() 
	{ 
		static FamilyManager instance; 
		return &instance;
	}

	size_t Size() 
	{ 
		return families.size(); 
	}

	bool Initialize();
	bool Update();
	FamilyInfo * Find(unsigned int familyid);
	unsigned int GetFactionId(unsigned int familyid);
	bool GetBaseInfo(int fid, GFactionBaseInfo& info);

	void OnCreate(GFamily& info);
	void OnDelete(int fid);
	void OnLoad(GFamily& family);
	void OnLogin(int roleid,int factionid,char title,int familyid,
			int contribution,int devotion, int gameid,int linksid,int localsid );
	void OnLogout(int roleid, int factionid, int familyid, int level, int occupation);
	int  OnRecruit(int familyid, Octets& familyname);
	void OnJoin(int fid, int inviter, int invitee, int level, int occupation, Octets& name);
	void OnAppoint(int fid, int appointer, int appointee, char title);
	void OnLeave(int fid, int roleid, int master);
	void OnDeleteRole(int fid, int roleid);
	void OnSync(int fid);
	void OnAsyncData(AsyncData* arg, AsyncData* res);

	int  CanDismiss(int fid);
	bool CanLeave(int roleid, int familyid);
	bool CanExpel(int roleid, int familyid, int expellee);

	void SetAnnounce(int fid, int roleid, Octets & announce);
	bool SetNickname(int fid, int roleid, int receiver, Octets & nickname, GFolk & info);
	
	void Sync2Game(int fid, int roleid, int contribution, int devotion);
	void SyncMaster(int fid, int roleid, GFolk& info);
	
	void GetMember(int fid, FactionList_Re	&list, char init);
	bool IsMember(int familyid, unsigned int roleid);
	
	void SetDelete(int fid, time_t deletetime);
	bool SyncGameOnLogin(int fid, std::vector<GFamilySkill>& list, int linksid, int localsid);
	void UseSkill(int fid, int skill, int ability);
	void ExpShare(int fid, int roleid, int64_t exp, int level);
	bool GetTaskRecord(int fid,int& star, std::vector<int>& list);
	void Save2DB();
	void OnRolenameChange(int fid, int roleid, const Octets & newname);
	int CanChangeName(int fid, int roleid, const Octets & newname);
	void OnNameChange(int fid, const Octets & newname, char scale);
	void DebugClearCD(int fid);
};

};
#endif

