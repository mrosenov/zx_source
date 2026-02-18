#ifndef __GNET_BLOCKEDROLE_H
#define __GNET_BLOCKEDROLE_H
#include "rpcdata/dbconsignprepostrolearg"
#include "rpcdata/dbconsignsoldarg"

namespace GNET
{

class GConsignRole;
class GConsignDB;
class GConsignGsRoleInfo;

enum LockRoleStatus //result of lock role action
{
	LockRoleOK,
	LockRoleLocking,
	LockRoleFail,
	LockRoleNotFound,
};

class BlockedRole //role not permit to login to gs
{
public:
	BlockedRole(){}
	~BlockedRole(){}
	static BlockedRole* GetInstance() { static BlockedRole instance; return &instance; }
	LockRoleStatus LockRole(int roleid);//return LockRoleStatus
	LockRoleStatus RoleLockStatus(int roleid);
	bool CompleteRoleLock(int roleid);

	void UnLockRole(int roleid);
	bool IsRoleBlocked(int roleid);
	LockRoleStatus UpdateLockStatus(int roleid);//check if role could be set to LockRoleOK status
public:	typedef std::map<int,LockRoleStatus> BlockedRoleMap;
private:
	enum RoleLoginStatus
	{
		RoleBusy,
		RoleOnGame,
		RoleNotInUse,
	};	
	BlockedRoleMap rolemap;//roleid
	int CheckLoginStatus(int roleid);
};

class RoleTask //make sure role is locked then task is executed
{
protected:
	enum TaskStatus
	{
		TaskInit,
		RoleLocking,
//		RoleLocked,
		WaitDBResp,
//		TaskDone,
		TaskFail,
	};
	void SetStatus(TaskStatus s,int now){
	//	if(s == TaskFail)
	//		OnFinish(taskstatus);
		taskstatus = s;
		status_start_time=now;
	}
private:
	const static int STATUS_TIME_LIMIT=60;
	int taskid;
	int roleid;
	TaskStatus taskstatus;
	int status_start_time;
	//	LockRoleStatus 	
	virtual void UpdateOnInit(int now)=0;
	virtual void UpdateOnRoleLocking(int now)=0;
//	virtual void UpdateOnWaitDBResp()=0;
	virtual void OnFinish(TaskStatus current)=0;
	void CheckTimeOut(int now);
public:
	bool IsFail(){
	//	return taskstatus==TaskDone || taskstatus==TaskFail;
		return taskstatus==TaskFail;
	};
	void Update(int now);
	int GetRoleID() const{return roleid;}
	int GetTaskID() const{return taskid;}
//	virtual int OnDBResp();//logic in consignmanager
	RoleTask(int tid,int rid):taskid(tid),roleid(rid),taskstatus(TaskInit){}
	TaskStatus GetStatus(){return taskstatus;}
	bool IsLocking(){return taskstatus==RoleLocking;}
	virtual ~RoleTask(){}//relese role lock on destruction
};

class SoldRoleTask: public RoleTask
{
	DBConsignSoldArg data;
	int zoneid_; 
       	int64_t timestamp_;
	int userid_;
	bool userid_mismatch_;
	bool roleid_mismatch_;
private:
	virtual void UpdateOnInit(int now);
	virtual void UpdateOnRoleLocking(int now);
//	virtual void UpdateOnWaitDBResp();
	virtual void OnFinish(TaskStatus current);
public:
	SoldRoleTask(int taskid,const DBConsignSoldArg& msg,int zoneid,int64_t timestamp,int userid,bool userid_mismatch,\
			bool roleid_mismatch):RoleTask(taskid,msg.roleid),data(msg),zoneid_(zoneid),timestamp_(timestamp),\
			      userid_(userid),userid_mismatch_(userid_mismatch),roleid_mismatch_(roleid_mismatch)
	{}
	~SoldRoleTask(){
	//	if(!IsFail())
			OnFinish(GetStatus());
	}	
};

class ConsignRoleTask: public RoleTask
{
	DBConsignPrePostRoleArg data;
private:
	virtual void UpdateOnInit(int now);
	virtual void UpdateOnRoleLocking(int now);
//	virtual void UpdateOnWaitDBResp();
	virtual void OnFinish(TaskStatus current);
public:
	ConsignRoleTask(int taskid,const DBConsignPrePostRoleArg & arg):RoleTask(taskid,arg.roleid),data(arg){}
	~ConsignRoleTask(){
	//	if(!IsFail())
			OnFinish(GetStatus());
	}
	void SetGSRoleInfo(const GConsignGsRoleInfo& gsroleinfo);
};

class RoleTaskManager: public IntervalTimer::Observer
{
	typedef std::map<int /*taskid*/,RoleTask*> RoleTaskMap;
	RoleTaskMap taskmap;
	static const int ROLETASK_MANAGER_UPDATE_INTERVAL=1;//seconds
	static const int ROLETASK_INITIAL_ID=0;
	int current_taskid;
private:
	int GetTaskID(){return current_taskid++;}
public:
	bool Update();
	bool Initialize();   
	RoleTaskManager(){current_taskid=ROLETASK_INITIAL_ID;}
	static RoleTaskManager* GetInstance() { static RoleTaskManager instance; return &instance; }
	int SoldRole(const DBConsignSoldArg& msg,int zoneid,int64_t timestamp,int userid,bool userid_mismatch,bool roleid_mismatch);
	int DBConsignSoldRoleResp(int taskid,int roleid, int64_t sn,int64_t timestamp);
	int DBConsignSoldRoleFail(int taskid);
	
	int ConsignRole(const DBConsignPrePostRoleArg& msg);
	int DBConsignRoleResp(int taskid,int retcode,const GConsignDB& detail,const GConsignRole& role);
	int ConsignRoleOfflineAddData(int roleid, const GConsignGsRoleInfo& gsroleinfo);
};

};
#endif
