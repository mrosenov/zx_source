#ifndef __GNET_MAPUSER_H
#define __GNET_MAPUSER_H

#include <map>
#include <set>
#include <vector>

#include "maperaser.h"
#include "thread.h"
#include "groleforbid"
#include "grolebase"
#include "groleinfo"
#include "roleinfo"
#include "ggroupinfo"
#include "gfriendinfo"
#include "genemyinfo"
#include "hashstring.h"
#include "cooldown.h"
#include "gamemaster.h"
#include "gpair"
#include "localmacro.h"
#include "gcirclehistory"
#include "senddataandidentity.hpp"
#include "userssoinfo"

#define ROLELIST_DEFAULT	0x80000000
#define MAX_ROLE_COUNT		16
#define REMOTE_ONLINE_FLAG	500

namespace GNET
{

//typedef std::vector<GCircleHistory> GCircleHistoryVector;	
class AddictionControl;
class PlayerLogout;
enum AUTOLOCK_KEY
{
        LOCKTIME_NOW = 1,
	LOCKTIME_NEW = 2,
        LOCKSET_TIME = 3,
};
enum{
	GMSTATE_ACTIVE   = 0x01
};
class PairSet
{
	std::vector<GPair> list;
public: 
	PairSet() {}
	PairSet(std::vector<GPair>& v) : list(v) {}
	PairSet& operator = (const std::vector<GPair>& v)
	{       
		list = v;
		return *this;
	}       
	int GetValue(int key)
	{
		for(std::vector<GPair>::iterator it=list.begin();it!=list.end();++it)
		{
			if(it->key==key)
				return it->value;
		} 
		return -1;
	}       
	void SetValue(int key, int value)
	{       
		for(std::vector<GPair>::iterator it=list.begin();it!=list.end();++it)
		{
			if(it->key==key)
			{
				it->value = value;
				return;
			}
		}
		list.push_back(GPair(key,value));
	}
	std::vector<GPair>& GetList()
	{
		return list;
	}
};
struct point_t	{		
	point_t(){ x = y = z = 0;}		
	float x;		
	float y;		
	float z;	
};	
class PlayerInfo;
class UserInfo
{
public:
	class RoleList
	{
		unsigned int rolelist;
		int count;
		int cur_role;
	public:
		RoleList() : rolelist(0),count(0),cur_role(0) { }
		RoleList(unsigned int r) : rolelist(r),count(0),cur_role(0) { }
		~RoleList() { }
		void operator=(const RoleList& rhs) { rolelist=rhs.rolelist; }
		bool IsRoleListInitialed()
		{
			return (rolelist & ROLELIST_DEFAULT) != 0;
		}
		bool IsRoleExist(int roleid)
		{
			return (rolelist & (1<<(roleid % MAX_ROLE_COUNT))) != 0;
		}
		void InitialRoleList()
		{
			rolelist = ROLELIST_DEFAULT;
		}
		unsigned int GetRoleList() 
		{
			return rolelist;
		}
		int GetRoleCount()
		{
			if (!IsRoleListInitialed()) return -1;  //rolelist is not initialized
			count=0;
			for (int id=0;id<MAX_ROLE_COUNT;id++)
			{
				if (IsRoleExist(id)) count++;
			}
			return count;
		}
		int AddRole()
		{
			if (!IsRoleListInitialed()) { return -1; }
			if (GetRoleCount()==MAX_ROLE_COUNT) { return -1; }
			int id=0;
			for (;id<MAX_ROLE_COUNT && IsRoleExist(id);id++);
			rolelist +=(1<<id);
			return id;	
		}		
		int AddRole(int roleid)
		{
			if (!IsRoleListInitialed()) { return -1; }
			if (IsRoleExist(roleid)) { return roleid; } //the role will be overlayed
			if (GetRoleCount()==MAX_ROLE_COUNT) { return -1; }
			if (roleid<0 || roleid >MAX_ROLE_COUNT-1) { return -1;}
			rolelist +=(1<<roleid);  
			return roleid;
		}
		bool DelRole(int roleid)
		{
			if (!IsRoleListInitialed()) return false;
			if (!IsRoleExist(roleid)) return false;
			return (rolelist -= 1<<(roleid % MAX_ROLE_COUNT)) != 0;
		}
		int GetNextRole()
		{
			while (cur_role<MAX_ROLE_COUNT)
				if (IsRoleExist(cur_role++)) return cur_role-1;
			return -1;
		}
		void SeekToBegin()
		{
			cur_role=0;
		}	
	};
public:
	int userid;
	int logicuid;
	int roleid;
	unsigned int linksid;
	unsigned int localsid;
	int status;
	int linkid;
	int gameid;
	int ip;
	int lastlogin_ip;
	Octets iseckey;
	Octets oseckey;
	Octets account; //缓存玩家完美通信证账户名 传给跨服用
	UserSSOInfo ssoinfo;//联合登录账号信息
	int rewardtype;
	int rewarddata;
	int gmstatus;
        time_t      actime;
        time_t      logintime; //目前只用来计算安全锁超时
	RoleList    rolelist;
	ByteVector  privileges;
	PairSet     autolock;
	PlayerInfo* role;
	cd_manager  cd_man;
	AddictionControl*  acstate;
	int real_referrer;				//该用户已经绑定的上线
	int au_suggest_districtid;		//AU发过来的上线区号，可能不是本区
	int au_suggest_referrer;		//AU发过来的上线roleid，可能不是本服务器的
	IntVector	merchantlist;		//绑定的快捷充值商家id列表

	int netbar_master; //说明是网吧管理员自己登录了 =0 说明是其他人再网吧登录 判断吧主用
	Octets netbar_title;    // 网吧称号,进人游戏的该网吧玩家 头顶该称号（3级网吧有）
	int netbar_level;   // 网吧等级, 1-3
	int award_type;     // 奖励类型, 1-4

	Octets rand_key;	//临时缓存玩家跨服登录使用的随机key
	Octets playerlogin_re_pack;
	int src_zoneid;		//跨服中玩家来自的zoneid
	int chgds_lineid;	//跨服后的目标线号
	int64_t touchpoint; //缓存的 touch 点

	Octets checksum;
	
	UserInfo(int uid,unsigned int sid,unsigned int lsid,int st)
		: userid(uid),linksid(sid),localsid(lsid),status(st)
	{
		logicuid = 0;
		roleid = 0;
		linkid = 0;
		gameid = _GAMESERVER_ID_INVALID;
		ip = 0;
		lastlogin_ip = 0;
		rewardtype = 0;
		rewarddata = 0;
		gmstatus = 0;
		actime = 0;
		logintime = 0;
		role = 0;
		acstate = 0;
		real_referrer = 0;
		au_suggest_districtid = 0;
		au_suggest_referrer = 0;
		netbar_master = 0;
		netbar_level = -1;
		award_type = -1;
		src_zoneid = 0;
		chgds_lineid = _GAMESERVER_ID_INVALID;
		touchpoint = 0;
	}
	UserInfo() : userid(0), linksid(0), localsid(0), status(0)
	{
		logicuid = 0;
		roleid = 0;
		linkid = 0;
		gameid = _GAMESERVER_ID_INVALID;
		ip = 0;
		lastlogin_ip = 0;
		rewardtype = 0;
		rewarddata = 0;
		gmstatus = 0;
		actime = 0;
		logintime = 0;
		role = 0;
		acstate = 0;
		real_referrer = 0;
		au_suggest_districtid = 0;
		au_suggest_referrer = 0;
		netbar_master = 0;
		netbar_level = -1;
		award_type = -1;
		src_zoneid = 0;
		chgds_lineid = _GAMESERVER_ID_INVALID;
		touchpoint = 0;
	}
	~UserInfo();
	bool TestCoolDown(int id)
	{
		return cd_man.TestCoolDown(id);
	}

	bool SetCoolDown(int id, int cooldown_msec)
	{
		return cd_man.SetCoolDown(id, cooldown_msec);
	}
        void GetLocktime(int& locktime, int& timeout)
	{
		time_t now = Timer::GetTime();
		int settime = autolock.GetValue(LOCKSET_TIME);
		locktime = autolock.GetValue(LOCKTIME_NOW);
		if(settime!=-1)
		{
			int timenew = autolock.GetValue(LOCKTIME_NEW);
			if(timenew>locktime || now-settime>86400*3)
				locktime = timenew;
		}
		if(locktime<0)
			locktime = 300;

		timeout = locktime - now + logintime;
		if(timeout<0)
			timeout = 0;
	}
        int SetLocktime(int locktime)
	{
		time_t now = Timer::GetTime();
		int settime = autolock.GetValue(LOCKSET_TIME);
		if(settime>0)
		{
			if(now-settime<60)
				return -1;
			int timenew = autolock.GetValue(LOCKTIME_NEW);
			int timenow = autolock.GetValue(LOCKTIME_NOW);
			if(now-settime>86400*3 || timenew>timenow)
				autolock.SetValue(LOCKTIME_NOW, timenew);
		}
		autolock.SetValue(LOCKTIME_NEW, locktime);
		autolock.SetValue(LOCKSET_TIME,now);
		return 0;
	}
	bool GenSSOInfo(const Octets & account_str);
};
class PlayerInfo
{
public:
	int roleid;
	int userid;
	UserInfo* user;
	Octets    name;
	bool ingame;
	int  gameid;
	int  access_time;  
	unsigned int linksid;
	unsigned int localsid;
	unsigned int factionid;
	unsigned int familyid;
	unsigned int battlescore;
	int sectid;
	unsigned char factiontitle;
	unsigned char level;
	unsigned char occupation;
	unsigned char emotion;
	int jointime;

	int chgs_dstlineid;	
	int chgs_mapid;	
	point_t chgs_pos;
	char chgs_territoryrole;
	int old_gs;
	int world_tag;
	

	// friend system relative data
	int friend_ver; // -1:unavailable, 0:untouched, >0: modified
	GGroupInfoVector  groups;
	GFriendInfoVector friends;
	GEnemyInfoVector  enemies;

	PlayerInfo(UserInfo* u, int rid) : roleid(rid), user(u), ingame(0)
	{
		userid = u->userid;
		gameid = u->gameid;
		linksid = u->linksid;
		localsid = u->localsid;
		friend_ver = -1;
		emotion = 0;
		access_time = 0;
		factiontitle = 0;
		factionid = 0;
		familyid = 0;
		jointime = 0;
		level = 0;
		occupation = 0;
		battlescore = 0;
		sectid = 0;
		chgs_dstlineid = 0;
		chgs_mapid = 0;
		chgs_territoryrole = 0;
		old_gs = _GAMESERVER_ID_INVALID;
		world_tag = 0;
		circleinfo_valid = false;
		circlejoin_processing = false;
	}
	PlayerInfo()  { }
	~PlayerInfo() { }
	
	bool IsGM() {  return (user->gmstatus&GMSTATE_ACTIVE)!=0; }
	
	unsigned int GetCircleID(){ return circlehistory.size()>0 ? circlehistory.back().circleid :0;}
//	unsigned char GetCircleTitlemask(){ return circlehistory.size()>0 ? circlehistory.back().titlemask :0;}
        void AddCircleHis(GCircleHistory& circlehisnode){ circlehistory.push_back(circlehisnode);}
	void SetCircleTitlemask(unsigned char titlemask){ circlehistory.back().titlemask = titlemask;} 		
	void SetCircleGradTime(int grad_time){ circlehistory.back().graduate_time = grad_time;}
	void InitCircleHis(std::vector<GCircleHistory> & history){ circlehistory.swap(history);circleinfo_valid = true; }
	std::vector<GCircleHistory> circlehistory;
	bool circleinfo_valid;
	bool circlejoin_processing;
};
#define MAX_PLAYER_NUM_DEFAULT 10000

class UserContainer
{
	//usermap
	typedef std::map<int,UserInfo>     UserMap;
	typedef std::map<int,PlayerInfo*>  RoleMap;
	Thread::RWLock	locker;
	UserMap usermap;
	RoleMap rolemap;

	//GM map( all GM online is in this map)
	typedef std::set<int/*userid*/> GMSet;
	GMSet gmset;

	typedef std::map<int, int>  UidMap;
	UidMap uidmap;
	std::set<int>	remoteonlineset;//目前只做统计用

	Thread::Mutex	locker_rolename;
	typedef __gnu_cxx::hash_map<Octets,int> RolenameMap;
	RolenameMap rolenamemap;

	size_t		max_player_num;
	size_t		fake_max_player_num;
	Thread::Mutex locker_maxplayer;

	static UserContainer	instance;
	UserContainer() : max_player_num(MAX_PLAYER_NUM_DEFAULT), fake_max_player_num(MAX_PLAYER_NUM_DEFAULT)
	{ }
public:
	~UserContainer() { }
	static UserContainer & GetInstance() { return instance; }
	size_t Size()
	{
		Thread::RWLock::RDScoped l(locker);
		return usermap.size();
		//int local_online = usermap.size()-remoteonlineset.size();
		//return (local_online < 0 ? 0:local_online);	
	}
	size_t SizeNoLock() { return usermap.size();	}

	Thread::RWLock & GetLocker() { return locker; }

	bool FindRoleId( const Octets& name, int& roleid )
	{
		Thread::Mutex::Scoped l(locker_rolename);
		RolenameMap::iterator it = rolenamemap.find(name);
		if(it!=rolenamemap.end())
		{
			roleid = (*it).second;
			return true;
		}
		return false;
	}
	void InsertName( const Octets& name, int roleid )
	{
		Thread::Mutex::Scoped l(locker_rolename);
		rolenamemap[name] = roleid;
	}

	void EraseName( const Octets& name )
	{
		Thread::Mutex::Scoped l(locker_rolename);
		RolenameMap::iterator it=rolenamemap.find(name);
		if ( it!=rolenamemap.end() )
		{
			rolenamemap.erase( it );
		}
	}

	bool ExistsUser( int userid )
	{
		Thread::RWLock::RDScoped l(locker);
		UserMap::iterator it = usermap.find(userid);
		return (it!=usermap.end());
	}

	bool FindUser( int userid, UserInfo& info )
	{
		Thread::RWLock::RDScoped l(locker);
		UserMap::iterator it = usermap.find(userid);
		if(it!=usermap.end())
		{
			info = (*it).second;
			return true;
		}
		return false;
	}

	void InsertUser( int userid, UserInfo& info )
	{
		Thread::RWLock::WRScoped l(locker);
		usermap[userid] = info;
	}

	void EraseUser( int userid )
	{
		Thread::RWLock::WRScoped l(locker);
		usermap.erase( userid );
	}

	void EraseUserNoLock( int userid )
	{
		usermap.erase( userid );
	}

	class IQueryUser
	{   
	public:
		virtual ~IQueryUser() { } 
		virtual bool Update( int userid, UserInfo & info ) = 0;
	};
	void WalkUser( IQueryUser& q )
	{
		Thread::RWLock::RDScoped l(locker);
		for( UserMap::iterator it = usermap.begin(), ite = usermap.end(); it != ite; ++it )
		{
			q.Update( (*it).first, (*it).second );
		}
	}
	void DeleteWalkUser( IQueryUser& q )
	{
		Thread::RWLock::RDScoped l(locker);
		{
			MapEraser<UserMap> e(usermap);
			for( UserMap::iterator it = usermap.begin(), ite = usermap.end(); it != ite; ++it )
			{
				if( !q.Update( (*it).first, (*it).second ) )
					e.push( (*it).first );
			}
		}
	}
	void PartlyWalkUser( int& beginid, int count, IQueryUser& q )
	{
		Thread::RWLock::RDScoped l(locker);
		UserMap::iterator it;
		UserMap::iterator ite = usermap.end();
		if( -1 == beginid )
			it = usermap.begin();
		else
			it = usermap.lower_bound(beginid);
		int n = 0;
		for( ; it != ite && n < count; ++n, ++it )
		{
			q.Update( (*it).first, (*it).second );
		}
		beginid = ( it != usermap.end() ? (*it).first : -1 );
	}

	UserInfo* FindUser( int userid )
	{
		UserMap::iterator it = usermap.find(userid);
		if(it!=usermap.end())
			return &((*it).second);
		return NULL;
	}

	PlayerInfo* FindRole( int roleid )
	{
		RoleMap::iterator it = rolemap.find(roleid);
		if(it!=rolemap.end())
			return it->second;
		return NULL;
	}

	PlayerInfo* FindRoleOnline( int roleid )
	{
		PlayerInfo* pinfo = NULL;
		RoleMap::iterator it = rolemap.find(roleid);
		if(it!=rolemap.end())
			pinfo = it->second;
		return (pinfo && pinfo->ingame) ? pinfo : NULL;
	}

	bool IsRemoteOnline( int roleid )
	{
		PlayerInfo* pinfo = NULL;
		RoleMap::iterator it = rolemap.find(roleid);
		if(it!=rolemap.end())
			pinfo = it->second;
		
		if(pinfo)
		{
			//LOG_TRACE("IsRemoteOnline");
			UserInfo* _pUser = pinfo->user;
			if(_pUser)
			{
				//LOG_TRACE("IsRemoteOnline _pUser->status:%d", _pUser->status);
				if(_pUser->status == _STATUS_REMOTE_LOGIN)
				{
					return true;
				}
			}
		}

		return false;
	}

	UserInfo* FindUser(int userid,unsigned int link_sid,unsigned int localsid);
	
	std::string GetUserIP( int userid );

	void SetPlayerLimit(size_t num,size_t fake_num)
	{
		Thread::Mutex::Scoped l(locker_maxplayer);	
		max_player_num = num; 
		fake_max_player_num = fake_num;
	}

	size_t GetPlayerLimit() 
	{ 
		Thread::Mutex::Scoped l(locker_maxplayer);	
		return max_player_num;
	}
	size_t GetFakePlayerLimit()
	{
		Thread::Mutex::Scoped l(locker_maxplayer);	
		return fake_max_player_num;
	}
	size_t GetLinePlayerNumber(int lineid)
	{
		Thread::RWLock::RDScoped l(locker);
		size_t r = 0;
		for(UserMap::const_iterator it = usermap.begin(), ie = usermap.end(); it != ie; ++it)
		{
			if( (*it).second.gameid == lineid ) ++r;
		}
		return r;
	}
	unsigned char GetChatEmotion(int roleid)
	{
		Thread::RWLock::RDScoped l(locker);
		PlayerInfo * pinfo = FindRole(roleid);
		return pinfo?pinfo->emotion:0;
	}
	void UserLogin( int userid, int linksid, int localsid, bool isgm, int type, int data, int ip, const Octets & iseckey, const Octets & oseckey, const Octets & account, bool announce=true);
	void UserLogout(UserInfo * pinfo, char kicktype=0, bool force=false);
	void RoleLogout(UserInfo * pinfo, bool forward_to_cds=false);
	void UserDisconnect(UserInfo * pinfo);
	void ContinueLogin(int userid, bool result);
	void UserReconnect(UserInfo * pinfo, int userid, int roleid, int localsid, int linksid, const Octets & checksum, int loginip);
	void ReconnectLogin(int userid, bool result);
	bool OnPlayerLogout(PlayerLogout& cmd);
	int GetOnlineRolelist(int startrole,int count,std::vector<int>& rolelist,char & finish);
	PlayerInfo* RoleLogin( UserInfo* user, int roleid)
	{
		if(user->role)
		{
			rolemap.erase(user->role->roleid);
			delete user->role;
		}
		PlayerInfo* role = new PlayerInfo(user, roleid);
		user->role = role;
		user->roleid = roleid;
		user->status = _STATUS_READYGAME;
		rolemap[roleid] = role;
		return role;
	}
	static int Roleid2Userid(int roleid)
	{
		UidMap::iterator it = instance.uidmap.find(LOGICUID(roleid));
		if(it!=instance.uidmap.end())
			return it->second;
		return 0;
	}
	void InsertLogicuid(int userid, int logicuid)
	{
		if (logicuid > 0)
			uidmap[logicuid] = userid;
	}
	int DisconnectLinkUsers(unsigned int linksid);
	int ClearRemoteUsers();
	int DisconnectZoneUsers(int zoneid);
	static void LegacyFetchUserid(Protocol::Manager* manager, Protocol::Manager::Session::ID sid, Protocol::Type type, Protocol * cmd, int roleid);
	void InsertRemoteOnline(int userid)
	{
		remoteonlineset.insert(userid);
	}
	void EraseRemoteOnline(int userid)
	{
		remoteonlineset.erase(userid);
	}
	size_t RemoteOnlineSize()
	{
		return remoteonlineset.size();
	}
};

enum CacheStatus
{
	CACHESTATUS_COMPLETE = 0x01,
	CACHESTATUS_LOCKED   = 0x02,
};

//user abnormal logout, cache the user info and allow the user to reconnect
//after the timeout, execute the logout process for the user
class UserInfoCache : public Timer::Observer
{
	std::map<int, int> cache_users;

public:
	UserInfoCache()
	{
		Timer::Attach(this);
	}
	
	static UserInfoCache & GetInstance() { static UserInfoCache instance; return instance; }

	void Insert(int uid, int cache_time)
	{
		std::map<int, int>::iterator it = cache_users.find(uid);
		if(it != cache_users.end())
			return;

		cache_users[uid] = cache_time;
		LOG_TRACE("Insert User Info Cache, user_id=%d, cache_time=%d", uid, cache_time);
	}

	void Remove(int uid)
	{
		cache_users.erase(uid);
	}
	
	void Update()
	{
		std::map<int, int>::iterator it;

		int now = time(NULL);
		for(it = cache_users.begin(); it != cache_users.end();)
		{
			if(now - it->second > 60)
			{
				int userid = it->first;
				UserInfo * pinfo = UserContainer::GetInstance().FindUser(userid);
				if(pinfo && pinfo->status == _STATUS_DISCONNECT)
				{
					LOG_TRACE("Remove User Info Cache, user logout, user_id=%d, cache_time=%d, status=%d", userid, it->second, pinfo->status);
					UserContainer::GetInstance().UserLogout(pinfo, 0, true); 
				}
				LOG_TRACE("Remove User Info Cache, user_id=%d, cache_time=%d", userid, it->second);
				cache_users.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
};

class RoleInfoCache : public IntervalTimer::Observer
{
	typedef std::map<int,GRoleInfo>   RoleInfoMap;
	typedef std::set<int>             InexistentSet;
	RoleInfoMap    roles;
	InexistentSet  invalidids;
	int cursor;
public:
	RoleInfoCache() : cursor(0) { }
	static RoleInfoCache & Instance() { static RoleInfoCache instance; return instance; }
	void Insert(int roleid, GRoleInfo& info) 
	{
		LOG_TRACE("Insert User GRoleInfo, roleid=%d, ct_teamid=%d, ct_score=%d, ct_last_max_score=%d", roleid, info.ct_teamid, info.ct_score, info.ct_last_max_score);
		info.status = CACHESTATUS_LOCKED | CACHESTATUS_COMPLETE;
		info.uptime = Timer::GetTime();
		roles[roleid] = info;
	}
	void Insert(int roleid, const GRoleBase& base) 
	{
		LOG_TRACE("Insert User GRoleBase, roleid=%d", roleid);
		RoleInfoMap::iterator it = roles.find(roleid);
		if(it!=roles.end())
			return;
		GRoleInfo info;
		info.id = roleid;
		info.name = base.name;
		info.faceid = base.faceid;
		info.hairid = base.hairid;
		info.gender = base.gender;
		info.familyid = base.familyid;
		info.title = base.title;
		info.spouse = base.spouse;
		info.sectid = base.sectid;
		info.fashionid = base.fashionid;

		/*
		const std::map<int, int>& gdatagroup = base.datagroup;
		std::map<int, int>::const_iterator it_cooltime = gdatagroup.find(KEY_DATAGOURP_ROLE_CT_COOLTIME);
		if(it_cooltime != gdatagroup.end())
			info.crssvrteams_leave_cool = it_cooltime->second;
	
		std::map<int, int>::const_iterator it_teamid = gdatagroup.find(KEY_DATAGOURP_ROLE_CT_TEAMID);
		if(it_teamid != gdatagroup.end())
			info.crssvrteams_teamid = it_teamid->second;

		std::map<int, int>::const_iterator it_last_max_score = gdatagroup.find(KEY_DATAGOURP_ROLE_CT_LAST_MAX_SCORE);
		if(it_last_max_score != gdatagroup.end())
			info.ct_last_max_score = it_last_max_score->second;
		*/
		info.status = 0;
		info.uptime = Timer::GetTime();
		roles[roleid] = info;
	}
	void Remove(int roleid)
	{
		roles.erase(roleid);
	}
	GRoleInfo* Get(int roleid)
	{
		RoleInfoMap::iterator it = roles.find(roleid);
		if(it!=roles.end())
		{
			it->second.uptime = Timer::GetTime();
			return &(it->second);
		}
		return NULL;
	}
	GRoleInfo* GetOnlogin(int roleid);
	GRoleInfo* GetOnlist(int roleid);
	size_t Size()
	{
		return roles.size();
	}

	bool IsInexistent(int roleid)
	{
		return (roleid<=0 || invalidids.find(roleid)!=invalidids.end());
	}
	void SetInexistent(int roleid)
	{
		if(invalidids.size()>524288)
			return;
		invalidids.insert(roleid);
	}
	void ClearInexistent(int roleid)
	{
		invalidids.erase(roleid);
	}
	void OnLogout(int logicuid, UserInfo::RoleList list)
	{
		//LOG_TRACE("******RoleCache: onlogout logicuid=%d,size=%d,list=%x",logicuid,roles.size(),list.GetRoleList());
		list.SeekToBegin();
		int roleid; 
		while((roleid=list.GetNextRole())!=-1)
		{
			roleid += logicuid;
			RoleInfoMap::iterator it = roles.find(roleid);
			if(it!=roles.end())
			{
				//LOG_TRACE("******RoleCache: onlogout set status roleid=%d,cur=%d",roleid,it->second.status);
				it->second.status &= ~CACHESTATUS_LOCKED;
			}
			//else
				//LOG_TRACE("******RoleCache: onlogout set status roleid=%d not found",roleid);
		}
	}

	static RoleInfo& ConvertRoleInfo(const GRoleInfo&, RoleInfo&);
	bool Update();
	void Initialize()
	{
		IntervalTimer::Attach(this,10000000/IntervalTimer::Resolution());
	}
	void SetDelete(int roleid, bool, int delete_time=0);
	void SetSelling(int roleid, bool sell);
	bool IsRoleSelling(int roleid);
	bool IsRoleDeleted(int roleid);
	bool IsRoleFrozen(int roleid);
	static GRoleInfo ChopRoleInfo(const GRoleInfo&);
};

class UserIdentityCache : public Timer::Observer

{
public:
	class Identity
	{
	public:
		int roleid;
		int src_zoneid;
		int ip;
		Octets iseckey;
		Octets oseckey;
		Octets account;
		Octets rand_key;
		int logintime; //切换服务器之前的账号登陆时间 用于安全锁继续计时
		int addtime;
		char au_isgm;
		int au_func;
		int au_funcparm;
		ByteVector auth;
		GRoleForbid forbid_talk;

		Identity(int _roleid = 0, int _zoneid = 0, int _ip = 0, const Octets & _ikey = Octets(), const Octets & _okey=Octets(), const Octets & _account=Octets(), const Octets & _rand=Octets(), int _login = 0,
				char _isgm = 0, int _func = 0, int _funcparm = 0, const ByteVector& _auth = ByteVector(), const GRoleForbid & _forbid=GRoleForbid())
			: roleid(_roleid), src_zoneid(_zoneid), ip(_ip), iseckey(_ikey), oseckey(_okey), account(_account), rand_key(_rand), logintime(_login),
				au_isgm(_isgm), au_func(_func), au_funcparm(_funcparm), auth(_auth), forbid_talk(_forbid) { addtime=time(NULL); }
		Identity(const Identity & _rhs) : roleid(_rhs.roleid), src_zoneid(_rhs.src_zoneid), ip(_rhs.ip),
				 iseckey(_rhs.iseckey), oseckey(_rhs.oseckey), account(_rhs.account), rand_key(_rhs.rand_key), logintime(_rhs.logintime),
				au_isgm(_rhs.au_isgm), au_func(_rhs.au_func), au_funcparm(_rhs.au_funcparm), auth(_rhs.auth), forbid_talk(_rhs.forbid_talk) { addtime=time(NULL);}
	};
	typedef std::map<int/*userid*/, Identity> IdentityMap;
private:
	enum {
		DEFAULT_CACHE_MAXTIME = 90,
	};
	IdentityMap identity_map;
	int cache_max_time;
	UserIdentityCache();
public:
	static UserIdentityCache * GetInstance() { static UserIdentityCache instance; return &instance; }
	bool Exist(int userid)
	{
		IdentityMap::const_iterator it = identity_map.find(userid);
		return (it != identity_map.end());
	}
	bool Find(int userid, Identity & iden)
	{
		IdentityMap::const_iterator it = identity_map.find(userid);
		if (it == identity_map.end())
			return false;
		iden = Identity(it->second);
		return true;
/*
		_roleid = it->second.roleid;
		_ip = it->second.ip;
		_ikey = it->second.iseckey;
		_okey = it->second.oseckey;
		_rand = it->second.rand_key;
		return true;
*/
	}
	void Insert(int userid, const Identity & iden)
	{
		identity_map[userid] = iden;
		LOG_TRACE("UserIdentityCache insert user %d addtime %d", userid, identity_map[userid].addtime);
	}
	void Remove(int userid)
	{
		identity_map.erase(userid);
		LOG_TRACE("UserIdentityCache remove user %d", userid);
	}
	void Update()
	{
//		LOG_TRACE("UserIdentityCache update cache size %d", identity_map.size());
		int now = time(NULL);
		IdentityMap::iterator it, ite = identity_map.end();
		for (it = identity_map.begin(); it != ite;)
		{
			if (now - it->second.addtime > cache_max_time)
			{
				LOG_TRACE("UserIdentityCache erase user %d role %d", it->first, it->second.roleid);
				STAT_MIN5("PlayerIdentityTimeout", 1);
				identity_map.erase(it++);
			}
			else
				++it;
		}
	}

};

class DelayRolelistTask : public Thread::Runnable
{
	int userid;
	int roleid;
	static std::set<int> roleidset;
	public:
	DelayRolelistTask(int uid, int rid) : Runnable(1), userid(uid), roleid(rid){ }
	static void PushID(int roleid)
	{
		roleidset.insert(roleid);
	}
	static bool PopID(int roleid)
	{
		std::set<int>::iterator it = roleidset.find(roleid);
		if(it==roleidset.end())
			return false;
		roleidset.erase(it);
		return true;
	}
	static void OnRecvInfo(int uid, int rid);
	void Run()
	{
		OnRecvInfo(userid, roleid);
		delete this;
	};
};

class Coupon
{
public:
	static Coupon *GetInstance()
	{
		static Coupon instance;
		return &instance;
	}

	void Init()
	{
		ParseStrings(Conf::GetInstance()->find("Coupon", "itemids"), _coupon_itemids);
		for(size_t i = 0; i < _coupon_itemids.size(); ++i)
		{
			Log::log(LOG_DEBUG, "Coupon:%d", _coupon_itemids[i]);
		}
	}

	bool NeedCheckCoupon() const
	{
		return _coupon_itemids.size() == 0 ? false : true;
	}

	void GetCoupons(std::vector<int> &itemids) const
	{
		itemids.clear();
		for(std::vector<int>::const_iterator it = _coupon_itemids.begin(); it != _coupon_itemids.end(); ++it)
		{
			itemids.push_back(*it);
		}
	}

	void InsertLocalContext(int userid, const Octets &local_context)
	{
		_localcontexts[userid] = local_context;
	}

	void GetLocalContext(int userid, Octets &local_context) const
	{
		std::map<int, Octets>::const_iterator it = _localcontexts.find(userid);
		if(it == _localcontexts.end())
		      return;
		local_context = it->second;
	}

	void EraseLocalContext(int userid)
	{
		_localcontexts.erase(userid);
	}
private:
	Coupon()
	{
	}
	~Coupon()
	{
	}

	bool ParseStrings(const std::string &src, std::vector<int> &result)
	{
		if( src.length() <= 0 )
			return false;

		char * delim = ",";
		char * buffer = new char[src.length()+1];
		if( NULL == buffer )
			return false;
		memcpy( buffer, src.c_str(), src.length() );
		buffer[src.length()] = 0;

		char * token = strtok( buffer, delim );
		int itoken = 0;
		while( NULL != token )
		{
			itoken = atoi(token);
			result.push_back(itoken);
			token = strtok( NULL, delim );
		}

		delete [] buffer;
		return true;
	}

	std::vector<int> _coupon_itemids;
	std::map<int/*userid*/, Octets> _localcontexts;
};

};

#endif

