#ifndef __GNET_GATEROLECACHE_H
#define __GNET_GATEROLECACHE_H
#include <map>
#include "localmacro.h"
#include "ggateroleinfo"
#include "gtteambean"
#include "gproviderserver.hpp"
#include "gtplatformclient.hpp"
namespace GNET
{
class GGateOnlineInfo;
class RoleInfo
{
	struct RoleDetailData
	{
		RoleDetailData(const GGateRoleInfo &d,time_t t):role_detail(d),access_time(t){}
		GGateRoleInfo role_detail;
		time_t access_time;//last access time, only valid for game offline roles
	};
	bool online;//game online status
	unsigned char gt_status;
	time_t off_line_time;//game and gt offline
	RoleDetailData *detail;
public:
	RoleInfo(bool gameonline,unsigned char status):online(gameonline),gt_status(status&0xFE),\
		detail(NULL){SetOfflineTime();}
	RoleInfo(bool gameonline,unsigned char status,const GGateRoleInfo& data):online(gameonline),\
	    gt_status(status&0xFE),detail(NULL)
	{
		SetOfflineTime();
		detail=new RoleDetailData(data,0);
		SetAccessTime();
	}
	~RoleInfo(){if(detail)delete detail;}
	bool GameOnline()const{return online;}
	void SetGameStatus(bool isonline){
		if(online==isonline)
			DEBUG_PRINT("GameStatus already online=%d",online);
		online=isonline;
		SetOfflineTime();
		SetAccessTime();
	}
	void ClearGameOnline(time_t timevalue){
		online = false;
		SetOfflineTime(timevalue);
	}
	void SetGTStatus(unsigned char newstatus){
		gt_status = newstatus&0xFE;
		SetOfflineTime();
	}
	unsigned char GetGTStatus()const{return gt_status;}
	bool GetRoleData(GGateRoleInfo& ret){
		if(detail){
			ret = detail->role_detail;
			SetAccessTime();
			return true;
		}
		else
			return false;
	}
	int GetGameId()
	{
		if (detail)
			return detail->role_detail.gameid;
		else
			return -1;
	}
	bool SetRoleData(const GGateRoleInfo& data){
		if(detail)
			detail->role_detail = data;
		else
			detail=new RoleDetailData(data,0);	
		SetAccessTime();
		return true;
	}
	bool IsDataExpired(time_t oldest_valid_time)const{
		if(!GameOnline() && detail!=NULL && detail->access_time!=0)
		{
			if(detail->access_time < oldest_valid_time)
				return true;
		}
		return false;
	}
	//do not expire entry while role data cache exist
	bool IsEntryExpired(time_t oldest_valid_time)const{
		if(off_line_time!=0 && off_line_time<oldest_valid_time && !GameOnline() && gt_status==0 ){
			if(detail==NULL)return true;
		}
		return false;
	}
	void FreeData(){ if(detail){ delete detail; detail=NULL;} }
private:
	RoleInfo(const RoleInfo& rhs);//forbid this func
	void operator=(const RoleInfo& rhs);//forbid this func
	void SetOfflineTime(){
		if(online==false && gt_status==0)
			off_line_time = Timer::GetTime();
		else
			off_line_time = 0;
	}
	void SetOfflineTime(time_t timevalue){
		if(online==false && gt_status==0)
			off_line_time = timevalue;
		else
			off_line_time = 0;
	}
	void SetAccessTime(){
		if(detail){
			time_t tmp;
			if(GameOnline())
				tmp=0;
			else
				tmp=Timer::GetTime();
			detail->access_time=tmp;
		}
	}
};
class RoleContainer: public IntervalTimer::Observer
{
	typedef std::map<int,RoleInfo*>  RoleMap;
	RoleMap rolemap;
	int cursor;
	static const int UPDATE_INTERVAL=5;//second
	static const int UPDATE_COUNT=100;//update 100 entry each time
	//update the whole map, for example 18000 entries, in about 15 minutes
	static const time_t DATA_EXPIRE_INTERVAL=300;//86400;//second;
	static const time_t ENTRY_EXPIRE_INTERVAL=300;//900;//second;
	static const unsigned int ROLE_CACHE_MAX_SIZE=500000;
public:
	RoleContainer():cursor(-1){Initialize();}
	virtual ~RoleContainer();
	static RoleContainer & GetInstance() { static RoleContainer instance;return instance; }
	size_t Size(){return rolemap.size();}
	void Initialize(){
		IntervalTimer::Attach(this,UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());
	}
	int AddRole(int roleid,bool gameonline,unsigned char gt_status)
	{
		if(rolemap.size()>ROLE_CACHE_MAX_SIZE)
			return -1;
		RoleInfo* prole = new RoleInfo(gameonline,gt_status);
		if(rolemap.insert(std::make_pair(roleid,prole)).second)
			return 0;
		else 
		{
			delete prole;
			return -1;
		}
	}
	int AddRole(int roleid,bool gameonline,unsigned char gt_status,const GGateRoleInfo& data)
	{
		if(rolemap.size()>ROLE_CACHE_MAX_SIZE)
			return -1;
		RoleInfo* prole = new RoleInfo(gameonline,gt_status,data);
		if(rolemap.insert(std::make_pair(roleid,prole)).second)
			return 0;
		else 
		{
			delete prole;
			return -1;
		}
	}
	int GameOnlineSync(char start,char finish,const std::vector<GGateOnlineInfo>& data);
	int ClearGameOnlineStatus();
/*	int UpdateEntry(int roleid,RoleInfo& role,time_t data_expire_time,time_t entry_expire_time)
	{
		if(role.IsDataExpired(data_expire_time))
			role.FreeData();
		if(role.IsEntryExpired())
			;
	}*/
	void PartlyWalk( int& beginid, int count);
	int GetOnlineRolelist(int startrole,int count,std::vector<int>& rolelist,char & finish);	
	RoleInfo* FindRole( int roleid )
	{
		RoleMap::iterator it = rolemap.find(roleid);
		if(it!=rolemap.end())
			return it->second;
		return NULL;
	}
	bool Update()
	{
		PartlyWalk(cursor,UPDATE_COUNT);
		return true;
	}	
};

class TeamCache
{
	typedef std::set<int64_t> TeamSet;
	typedef std::map<int/*gsid*/, TeamSet>  TeamMap;
	TeamMap teammap;
public:
	TeamCache(){}
	static TeamCache * GetInstance() { static TeamCache instance;return &instance; }
	/*
	size_t Size(int gsid)
	{
		return teammap[gsid].size();
	}
	*/
	void OnTeamSync(int gsid, const std::vector<GTTeamBean> & teams);
	void OnTeamCreate(int gsid, int64_t teamid);
	void OnTeamDismiss(int gsid, int64_t teamid);
	void OnGTDisconnect();
	void OnGTConnect();
	void OnGSDisconnect(int gsid);
};

};
#endif
