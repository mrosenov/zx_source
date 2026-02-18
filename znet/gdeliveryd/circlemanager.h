#ifndef __GNET_CIRCLEMANAGER_H
#define __GNET_CIRCLEMANAGER_H

#include <vector>
#include <map>
#include <list>

#include "thread.h"
#include "gcirclememberdb"
#include "gcircledb"
#include "circleasyncdata"
#include "hashstring.h"
#include "TaskAsyncData.h"
#include "localmacro.h"
#include "circlelist_re.hpp"
#include "gcirclebaseinfo"
#include "dbcirclehisget.hrp"
#include "gdeliveryserver.hpp"
namespace GNET
{

class CircleMember
{
public:
	int index; // node index in circleinfo's cinfo.member vector
	int gameid;
	unsigned int linksid;
	unsigned int localsid;
//	bool dirty;
	bool is_graduated;
	GCircleMemberDB  crole;

	CircleMember(const GCircleMemberDB & r) : index(-1),gameid(-1),linksid(0),localsid(0),is_graduated(0),crole(r)
	{ 
	}

	CircleMember(const CircleMember& r) 
		:index(r.index),gameid(r.gameid),linksid(r.linksid),localsid(r.localsid),is_graduated(r.is_graduated),crole(r.crole)
	{ 
	}

	~CircleMember() 
	{ 
	}

	void SetOnline(int idx,int gid,int sid,int lid)
	{
		index = idx;
		gameid = gid;
		linksid = sid;
		localsid = lid;
//		LOG_TRACE("CircleInfo::SetOnline: index=%d gameid= %d linksid=%d, localsid=%d", idx, gid, sid, lid);
	}

	void SetOffline()
	{
		gameid = -1;
		linksid = 0;
		localsid = 0;
	}
};
typedef std::map<unsigned int, CircleMember> CircleRoleContainer;

class CircleInfo
{
public:
	static const int PER_ROLE_POINT=10;
	CircleInfo()
		: online(0), access(false), dirty(false)
	{

		updatetime = Timer::GetTime();
	}

	CircleInfo(const GCircleDB& circleinfo) 
		: online(0), access(false), dirty(false)
	{
		updatetime = Timer::GetTime();
                cinfo = circleinfo;
	}

	~CircleInfo() {} 
	
	void OnAppoint(unsigned int roleid, CircleTitle title)
	{
		if(title == CIRCLE_MASTER)
		{
			cinfo.master_rid = roleid;
		//	dirty = true;
		}
	}
	bool MasterCanChange(){return cinfo.master_rid == 0;}
	CircleRoleContainer::iterator FindOnlineRole(unsigned int roleid);

	std::vector<GCircleMemberDB>::iterator FindMember(unsigned int roleid);
	int MemberIndex(std::vector<GCircleMemberDB>::iterator position)
	{ 
		int index =  distance( cinfo.member.begin() , position); 
		return (index < static_cast<int>(cinfo.member.size()) && index >= 0) ?index :-1;
	}	
	int GetOnline() { return online; }
	void IncOnline() 
	{ 
		//LOG_TRACE("IncOnline, circleid=%d", cinfo.circleid);
		online++;
	}
	void DecOnline() 
	{ 
		//LOG_TRACE("DecOnline, circleid=%d", cinfo.circleid);
		online--;
	}
	void AddOnline( const GCircleMemberDB& crole,int index,int gameid,int linksid,int localsid );
	void RemoveOnline( unsigned int roleid);
	bool IsSilent();
	bool Update(bool isbatch=false);
//	void Broadcast(Protocol& data); 
	template<class T> //Protocol only
	void Broadcast(T &data)
       	{
//		LOG_TRACE("CircleChat Broadcast, circleid=%d",cinfo.circleid);
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		access = true;
	       	for(CircleRoleContainer::iterator it=online_list.begin(),ie=online_list.end();it!=ie;++it)
	        {
			data.localsid = it->second.localsid;
                	dsm->Send(it->second.linksid,data);
        	}
	}
	int GetDefaultTaskpoint();

	bool CheckPointThreshold(unsigned int delta);
	void SendMessage(unsigned int msgid, int data1, int data2);

	bool SyncMember(unsigned int roleid, unsigned char level, unsigned char occupation);
	
	GCircleDB cinfo;
	CircleRoleContainer online_list;
	int online;
	bool access;
	bool dirty;	
	mutable int updatetime;
};

struct CircleSaver //save all circle, for shutdown
{
	CircleSaver() :index(0), still_not_sync_all(false) {}
	void Reset() { index = 0; }
	void Begin(){
	       	still_not_sync_all = true; 
		for (int i = 0; i < 10 && still_not_sync_all; i++ ) DoSave();//dont send too many rpc in one time
       	}
	void DoSave();
private:
	int index;
	bool still_not_sync_all;
};

class CircleManager : public IntervalTimer::Observer
{
	static const int NO_GRADE=0;
	static const unsigned int CIRCLE_MIN_LEVEL=10;
	static const int UPDATE_INTERVAL = 300;
	static int zone_off;
	static bool circle_open;
		
	typedef std::map<unsigned int,CircleInfo*>  CircleMap;
	CircleMap circlemap;
	unsigned int cursor;
	CircleSaver saver;

	CircleManager() : cursor(0){
		time_t now = Timer::GetTime();
		struct tm dt;
		localtime_r(&now,&dt);
		zone_off = dt.tm_gmtoff;
	}  
	friend class CircleSaver;
public:
	~CircleManager(); 

	static CircleManager* Instance() 
	{ 
		static CircleManager instance; 
		return &instance;
	}

	size_t Size(){ return circlemap.size(); }
	bool Initialize();
	bool Update();
	CircleInfo * Find(unsigned int circleid);

	void InitAppoint(unsigned int roleid,unsigned int circleid,CircleTitle title);
	void OnLoad(GCircleDB& circleinfo,unsigned int roleid);
	void OnLogin(unsigned int roleid,int circleid, int gameid,int linksid,int localsid );//,unsigned int level);
	void OnLogout(unsigned int roleid, int circleid, int level, int occupation);
	
	//bool isCircleMasterLevel(unsigned int level) {return  ((level)%15>=10 && level>16 );}//25,40,55
	//bool isNewGradeLevel(unsigned int level) {return  level==CIRCLE_MIN_LEVEL || ((level>30)&&((level-1)%15==0));}
	int GetGrade(unsigned int level){return (level>30) ?((level-1)/15) :1;}
	int GradeMinLv(int grade){return (grade>1)?(15*grade+1):CIRCLE_MIN_LEVEL;}
	int GradeMaxLv(int grade){return 15*(grade+1);}
	//bool OverLastGrade(unsigned int level){return level>135;}
	
	void OnLevelUp(int roleid, int circleid, unsigned int level, int reborn_cnt);
	void InitJoinCircle(int newgrade, int oldcircleid, int roleid, Jointype type);
	void OnJoin(int circleid, const GCircleMemberDB& crole);
	void OnAppoint(unsigned int roleid, unsigned int circleid, CircleTitle title,unsigned char titlemask);
	void OnLeave(int circleid, unsigned int roleid,int time);
	void OnDeleteRole(unsigned int circleid, unsigned int roleid);
	void OnSyncBatch(){ saver.DoSave();}
	void SyncRoleCircleHis(unsigned int roleid,Jointype type,GCircleHistory& circlehisnode,int gradtime);
	void OnAsyncData(CircleAsyncData* arg, CircleAsyncData* res);

	void GetFullInfo(unsigned int circleid, unsigned int roleid, CircleList_Re &list);
	void GetBaseInfo(IntVector& idlist, std::vector<GCircleBaseInfo> & infolist);	
	void Save2DB(){ saver.Reset(); saver.Begin(); }
	void ResetTaskpoint();
	bool AddPoint(unsigned int circleid, unsigned int delta);
	bool GetPoint(unsigned int circleid,unsigned int& circlepoint);	
	void NotifyGSNewCircle(unsigned int roleid, unsigned int circleid, unsigned char titlemask, int gsid);
	void NotifyClient(CircleMsgType type, unsigned int roleid, unsigned int circleid, unsigned int localsid,\
		       unsigned int linksid, unsigned int name=0, unsigned char grade=0);
	bool GetGradBonus(unsigned int circleid, unsigned int roleid, int64_t & bonus,bool clear);	
	int64_t CalcGradBonus(unsigned char count,unsigned char classgrade);//{unsigned int i =100;return i*count*classgrade;}
	int GetZoneOff(){return zone_off;}
	bool IsCircleOpen(){return circle_open;}
	void OpenCircle(){circle_open = true;}
	void OnRolenameChange(unsigned int circleid, int roleid, const Octets & newname);
};

/* clear all circle taskpoint in 00:00 */  
class CircleCleaner : public Thread::Runnable
{
	static const int interval=86400;
	int update_time;
	CircleCleaner(int _time,int _proir=1) : Runnable(_proir),update_time(_time) { }
public:
	static CircleCleaner* GetInstance(int updatetime = 0,int _proir=1)
       	{
		
		static CircleCleaner instance(updatetime,_proir);
		return &instance;
	}	
	int calc_seconds_to_time(int time)
	{
		int now = Timer::GetTime();
		int value = (interval - now%interval + (interval+time-CircleManager::Instance()->GetZoneOff())%interval)%interval;
		return value>5 ? value : interval;
	}
	
	void Run()
	{
		CircleManager::Instance()->ResetTaskpoint();
		Thread::HouseKeeper::AddTimerTask(this,calc_seconds_to_time(update_time));
	}

};
};
#endif
