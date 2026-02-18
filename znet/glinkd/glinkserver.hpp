#ifndef __GNET_GLINKSERVER_HPP
#define __GNET_GLINKSERVER_HPP

#include "protocol.h"
#include "timermanager.h"
#include "macros.h"
#include "cache.h"
#include <set>
#include "groleforbid"
#include "matrixchecker.h"
#include "privilege.hxx"
#include "requestlimiter.h"
#include "localmacro.h"

#define __USERCOUNT_MIN         1000    
#define __USERCOUNT_THRESHOLD   50
#define TIMER_INTERVAL          5
#define ACREPORT_TIMER          180

#define __HALFLOGIN_DEFAULT     30
#define __HALFLOGIN_THRESHOLD   15

namespace GNET
{
struct ProtoStat
{
	int keepalive;
	int gamedatasend;
	int publicchat;
	int remain_time;

	ProtoStat() : keepalive(0),gamedatasend(0),publicchat(0),remain_time(ACREPORT_TIMER) 
	{ 
	}
	void Reset() 
	{
		keepalive=0;
		gamedatasend=0;
		publicchat=0;
		remain_time=ACREPORT_TIMER;
	}
};

struct RoleData
{
	Protocol::Manager::Session::ID sid;
	int     userid;
	int     roleid;
	time_t  logintime;
	int gs_id;
	int src_zoneid;
	int status;

	RoleData() : sid(_SID_INVALID),userid(0),roleid(0),gs_id(_GAMESERVER_ID_INVALID),src_zoneid(0),status(_STATUS_ONLINE)
	{
	}
	RoleData(Protocol::Manager::Session::ID _sid,int _userid, int _roleid,int _gs_id,int _zone,int _status=_STATUS_ONLINE)
		: sid(_sid),userid(_userid),roleid(_roleid),gs_id(_gs_id),src_zoneid(_zone),status(_status)
	{
	}
};

struct SessionInfo
{
	Octets challenge;
	Octets identity;
	Octets response;
	int    userid;
	int    roleid;
	time_t login_time;
	int    gsid;
	bool   ingame;
	SockAddr local;
	SockAddr peer;
	MatrixChecker * checker;
	ProtoStat protostat;
	RequestLimiter policy;
	int algorithm;

	Octets mid;
/*
	enum
	{
		USE_PASSWD,
		USE_ELECNUMBER,
	};
	int login_method;
*/	
	Octets iseckey;
	Octets oseckey;
	Octets elec_num; // usbkey2 ºÍ phonetoken Âß¼­¸´ÓÃ´Ë×Ö¶Î

	SessionInfo() : userid(0),roleid(0),login_time(0),gsid(0),ingame(0),local(0),peer(0),checker(NULL),algorithm(0)/*, login_method(USE_PASSWD)*/
	{
		login_time = 0;
	}

	void SetLocal(SockAddr addr) { local = addr;}
	void SetPeer(SockAddr addr) { peer = addr;}
	SockAddr GetLocal() const { return local;}
	SockAddr GetPeer() const { return peer;}
	const struct in_addr& GetPeerAddr() const {  return ((const struct sockaddr_in*)peer)->sin_addr; }
};

class GLinkServer : public TimerManager , public Timer::Observer 
{
	static GLinkServer instance;

public:
	typedef Session::ID 				SessionID;

	typedef std::map<SessionID, SessionInfo>	SessionInfoMap;
	typedef SessionInfoMap::iterator		Iterator;
	typedef SessionInfoMap::const_iterator		CIterator;
	typedef std::set<SessionID>			SessionSet;

	typedef std::map<int, RoleData>			RoleInfoMap;
	typedef std::map<SessionID, int>		TimeoutMap;
	typedef std::map<int /*id*/,GRoleForbid>	ForbidMap;

	typedef std::map<int/*userid*/, ByteVector>     PrivilegeMap;
	typedef std::map<int/*userid*/, Octets> 	ElecNumberMap;

	friend class CheckTimer;
	friend class LineTimer;

private:
	size_t		accumulate_limit;
	size_t		user_count_limit;
	size_t		halflogin_limit;

	const Session::State *GetInitState() const;
	bool OnCheckAccumulate(size_t size) const { return accumulate_limit == 0 || size < accumulate_limit; }
	void OnAddSession(Session::ID sid);
	void OnDelSession(Session::ID sid) { }
	void OnDelSession(Session::ID sid, int status);
	void OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer);

	PassiveIO* 	passiveio;
	string 		section_num;

	// gamecontent version number
	unsigned int version;
	Octets edition;

	//server attribute
	Thread::Mutex 	locker_sa_; 
	int64_t server_attr;

	//alive keeper timer map
	mutable Thread::Mutex 	locker_alive;
	TimeoutMap 		alivetimemap;
	TimeoutMap 		readyclosemap;

	//forbid chat timer map 
	ForbidMap 		muteusers;
	ForbidMap 		muteroles;

	//user privilege map
	PrivilegeMap 	privilegemap;

	// user elecnumber map
//	ElecNumberMap 	elecnumber_map;

	//forbid Complain2GM cache
	Cache<int/*userid*/,int/*userid*/> forbidcomplaincache;
	
	// gauthd version(for perfectworld protected second generation)
	int auth_version; // 0: matrixpasswd 1: matrixpasswd2
public:

	//session maps
	mutable Thread::RWLock 	locker_map; 
	SessionInfoMap		sessions;
	RoleInfoMap		roleinfomap;
	SessionSet		halfloginset;
	char challenge_algo;

	static GLinkServer *GetInstance() { return &instance; }
	void SetSectionNum(const char* sn) { section_num=sn; }
	std::string Identification() const { return string("GLinkServer")+section_num; }
	void SetAccumulate(size_t size) { accumulate_limit = size; }

	GLinkServer() : accumulate_limit(0), user_count_limit(__USERCOUNT_MIN), halflogin_limit(__HALFLOGIN_DEFAULT), version(100), challenge_algo(ALGO_MD5)
	{ 
		passiveio=NULL; 
		forbidcomplaincache.init(102400,-1,3600); 
		server_attr=0; 
		auth_version = 0;
	}

	void SetVersion(unsigned int _ver) { version = _ver;}
	void SetAuthVersion(int version) { auth_version = version; }
	int GetAuthVersion() { return auth_version; }
	unsigned int GetVersion() { return version; }
	void SetEdition(Octets& _edition) { edition = _edition;}
	void StartListen() { if (NULL==passiveio) passiveio = Protocol::Server(this); }
	bool IsListening() { return passiveio!=NULL; }
	void SetUserCountLimit(size_t size) { user_count_limit = size > __USERCOUNT_MIN ? size:__USERCOUNT_MIN; }
	void SetHalfLoginLimit(size_t size) { halflogin_limit = size > __HALFLOGIN_DEFAULT ? size: __HALFLOGIN_DEFAULT; }

	bool ExceedUserLimit(size_t size) { return size >= user_count_limit+__USERCOUNT_THRESHOLD; }
	bool UnderUserLimit(size_t size) { return size <= user_count_limit-__USERCOUNT_THRESHOLD; }

	void SessionError(Session::ID sid, int errcode=0, const char *info=NULL);
	void SendErrorInfo(Session::ID sid, int errcode, const char *info);

	//server attribute
	void SetServerAttr(int64_t attr) {
		Thread::Mutex::Scoped l(locker_sa_);
		server_attr=attr;
	}
	int64_t GetServerAttr() {
		Thread::Mutex::Scoped l(locker_sa_);
		return server_attr;
	}
	void SetChallengeAlgo(char algo) { challenge_algo = algo;}

	void SetUserPrivilege(int userid, ByteVector& priv)
	{
		privilegemap[userid].swap(priv);
	}
	void GetUserPrivilege(int userid, ByteVector& priv) const
	{
		PrivilegeMap::const_iterator it = privilegemap.find(userid);
		if ( it != privilegemap.end())
		{
			priv = it->second;
		}
	}
	bool PrivilegeCheck(int userid, unsigned char privilege) const
	{
		PrivilegeMap::const_iterator it = privilegemap.find(userid);
		if(it==privilegemap.end())
			return false; 
		const ByteVector& v = it->second;
		for(ByteVector::const_iterator i=v.begin();i!=v.end();++i)
			if(*i==privilege)
				return true;
		return false;
	}
	bool PrivilegeCheck(Session::ID sid,int roleid,unsigned char privilege)
	{
		Iterator it = instance.sessions.find(sid);
		if(it==instance.sessions.end())
			return false;
		if(it->second.roleid!=roleid)
			return false;
		return PrivilegeCheck(it->second.userid, privilege);
	}

	bool IsForbidChat(int userid, int roleid, GRoleForbid & forbid) const
	{
		ForbidMap::const_iterator it;
		it = muteusers.find(userid);
		if (it != muteusers.end())
		{
			forbid = it->second;
			return true;
		}
		it = muteroles.find(roleid); 
		if (it != muteroles.end())
		{
			forbid = it->second;
			return true;
		}
		return false;
	}

	void SetAliveTime(SessionID sid, int time)
	{
		Thread::Mutex::Scoped l(locker_alive);
		alivetimemap[sid] = time;
	}

	void SetReadyCloseTime(SessionID sid, int time)
	{
		Thread::Mutex::Scoped l(locker_alive);
		readyclosemap[sid] = time;
	}

	void ForbidRoleChat(int role, int forbid_time, const Octets & reason)
	{
		muteroles[role] = GRoleForbid(Privilege::PRV_FORBID_TALK,forbid_time,0,reason);
	}
	void ForbidUserChat(int userid, int forbid_time, const Octets & reason)
	{
		muteusers[userid] = GRoleForbid(Privilege::PRV_FORBID_TALK,forbid_time,0,reason);
	}

	void AddChecker(SessionID sid, MatrixChecker * checker)
	{
		Thread::RWLock::RDScoped l(locker_map);
		Iterator it = sessions.find(sid);
		if (it != sessions.end())
		{
			SessionInfo & info = it->second;
			if (info.checker) delete info.checker;
			info.checker = checker; 
		}
	}

	bool VerifyMatrix(SessionID sid, int response) 
	{
		bool result = true;
		Thread::RWLock::RDScoped l(locker_map);
		Iterator it = sessions.find(sid);
		if (it != sessions.end())
		{
			SessionInfo & info = it->second;
			if (info.checker)
			{
				result = info.checker->Verify(response);
				delete info.checker;
				info.checker = NULL;
			}
		}
		return result;
	}

	static bool ValidSid(SessionID sid) 
	{
		return instance.sessions.find(sid) != instance.sessions.end();
	}
	static bool ValidUser(SessionID sid, int userid) 
	{
		Thread::RWLock::RDScoped l(instance.locker_map);
		CIterator it = instance.sessions.find(sid);
		return it != instance.sessions.end() && it->second.userid == userid;
	}
	static bool ValidRole(SessionID sid,int roleid) 
	{
		Thread::RWLock::RDScoped l(instance.locker_map);
		RoleInfoMap::const_iterator it = instance.roleinfomap.find(roleid);
		return it != instance.roleinfomap.end() && it->second.sid==sid;
	}
	static bool IsRoleOnGame(Session::ID sid )
	{
		Iterator it = instance.sessions.find(sid);
		if (it != instance.sessions.end() && it->second.ingame)
			return true;
		return false;    
	}
	static bool ValidLocalsid(Session::ID localsid, int roleid);

	SessionInfo * GetSessionInfo(SessionID sid)
	{
		Iterator it = sessions.find(sid);
		if (it != sessions.end())
			return &(it->second);
		return NULL;
	}

	RoleData * GetRoleInfo(int roleid)
	{
		RoleInfoMap::iterator it = roleinfomap.find(roleid);
		if (it != roleinfomap.end())
			return &(it->second);
		return NULL;
	}

	bool IsForbidComplain(int roleid)
	{
		return forbidcomplaincache.get(roleid);
	}

	void ForbidComplain(int roleid)
	{
		forbidcomplaincache.put(std::make_pair(roleid,roleid));
	}

	//Update
	void Update()
	{
		static int checktimer=TIMER_INTERVAL;
		if (checktimer--) return;
		CheckSessionTimePolicy();
		checktimer=TIMER_INTERVAL;
	}
	void TriggerListen(Session::ID sid, bool insert);
	void RoleLogin(Session::ID localsid, int roleid, int gsid, ByteVector& auth, int src_zoneid);
	bool RoleLogout(Session::ID localsid, int roleid);
	bool SetOnline(int roleid, Session::ID sid)
	{
		Iterator it = sessions.find(sid);
		if(it==sessions.end())
			return false;
		it->second.ingame = 1;
		RoleData * uinfo = GetRoleInfo(roleid);
		if (!uinfo)
		{ 
			DEBUG_PRINT("LinkServer::EnterWorld_1: Active Close Session(%d)(roleid=%d)\n",sid,roleid);
			Close(sid);
			return false;
		}
		if (uinfo->sid!=sid || uinfo->roleid!=roleid)
		{ 
			DEBUG_PRINT("LinkServer::EnterWorld_2: Active Close Session(%d)(roleid=%d)\n",sid,roleid);
			Close(sid);
			return false;
		}
		uinfo->status = _STATUS_ONGAME;
		return true;
	}
	bool SetSaveISecurity(SessionID id, Security::Type type, const Octets &key)
	{	
		if (Manager::SetISecurity(id, type, key))
		{
			Iterator it = sessions.find(id);
			if (it != sessions.end())
				it->second.iseckey = key;
			return it != sessions.end();
		}
		else
			return false;
	}
	bool SetSaveOSecurity(SessionID id, Security::Type type, const Octets &key)
	{	
		if (Manager::SetOSecurity(id, type, key))
		{
			Iterator it = sessions.find(id);
			if (it != sessions.end())
				it->second.oseckey = key;
			return it != sessions.end();
		}
		else
			return false;
	}
	bool GetSecurityKeys(SessionID id, Octets & ikey, Octets & okey)
	{
		ikey = okey = Octets();
		Iterator it = sessions.find(id);
		if (it != sessions.end())
		{
			ikey = it->second.iseckey;
			okey = it->second.oseckey;
		}
		return it != sessions.end();
	}
/*
	void SetUsedElecNumber(int userid, const Octets &elec_number)
	{
		elecnumber_map[userid] = elec_number;
	}

	void GetUsedElecNumber(int userid, Octets &elec_number)
	{
		if(elecnumber_map.find(userid) != elecnumber_map.end())
		{
			elec_number = elecnumber_map[userid];
		}
	}
*/
};

class CheckTimer : public Thread::Runnable
{
	int update_time;
public:
	CheckTimer(int _time,int _proir=1) : Runnable(_proir),update_time(_time) { }
	void Run();
private:
	void CheckConnection();
	void CheckForbid();
	void CheckProtoStat();
};

class LineTimer : public Thread::Runnable
{
	int update_time;
public:
	LineTimer(int _time,int _proir=1) : Runnable(_proir),update_time(_time) { }
	void Run();
};
};
#endif
