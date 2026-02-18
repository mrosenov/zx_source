#include <sys/types.h>
#include <dirent.h>

#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <functional>
#include <stdio.h>

#include "macros.h"
#include "accessdb.h"
#include "localmacro.h"

#include "stocklog"
#include "user"
#include "gfactioninfo"
#include "gfolk"
#include "gcrssvrteamsdata"
#include "familyid"
#include "gfamily"
#include "gmember"
#include "guserfaction"
#include "grolebase"
#include "grolestatus"
#include "groleinventory"
#include "grolestorehouse"
#include "gshoplog"
#include "gsyslog"
#include "gmailbox"
#include "message"
#include "gfriendinfo"
#include "gfriendlist"
#include "gsect"
#include "gdisciple"
#include "gsnsmessage"
#include "gsnsplayerinfocache"
#include "commonvalue.h"
#include "clearroledata"
#include "grolebase2"
#include "greferral"
#include "itemowner.h"
#include "ghometowninfo"
#include "gfarmcompoinfo"
#include "gfarmfield"
#include "gconsigndb"
#include "grolenamehis"
#include "dbclearconsumable.hrp"
#include "utilfunction.h"
#include "gkingdominfo"
#include "gcitystore"
#include "gterritorystore"
#include "gcrssvrteamsseasontop"
#include "gfactionbase"

namespace GNET
{

/*
 * 为了减少内存开销:
 * 1 将原来的 g_mapRoleid 和 g_mapRoleid2Userid 合并为一个 g_mapRoleid;
 * 2 在NameMap只存合服前后发生变化的名字
 * 3 增加函数ReOpenDB()调用
 * 4 cursor Update过程中Auto_checkpoint()
 * 5 ClearCircleHisQuery合并到PrepareLogicuid()中处理 setRole类型由set改为vector;如果vector能满足需求就不要使用map或 set
 *						20101117 liuyue
 * 为了加快DB访问速度:
 * 引入函数PreLoadTable()
 *						20101125 liuyue
 */


enum {
	CHANGE_ROLENAME_ITEMID = 0,
	CHANGE_FAMILYNAME_ITEMID,
	CHANGE_FACTIONNAME_ITEMID,
};
int g_gifttype[][3] = 
{ 	//{itemid, proc_type, max_count} 
	{52765,		19,	1}, //CHANGE_ROLENAME_ITEMID
	{53572,		19,	1}, //CHANGE_FAMILYNAME_ITEMID
	{53574,		19,	1}, //CHANGE_FACTIONNAME_ITEMID
};
inline void ReOpenDB()
{
	StorageEnv::checkpoint();
	StorageEnv::Close();
	StorageEnv::Open();
}

inline void Auto_Checkpoint(size_t & processed_count)//目前只能在Update函数内部调用 并且walk当前表是一个standalone表
{
	if ((processed_count%131072) == 0)
	{
		time_t now = time(NULL);
		LOG_TRACE("Processed %d items time:%s", processed_count, ctime(&now));
		StorageEnv::checkpoint();
	}
	processed_count++;
}

void PreLoadTable(const char* path)
{       
	time_t now = time(NULL);
	LOG_TRACE("Begin PreLoadTable %s time:%s", path, ctime(&now));
	FILE* fp = fopen(path,"r");
	if (!fp)
		return;	
	char buf[524300];
	while(!feof(fp))
		fread(buf,1,524288,fp);
	fclose(fp);
	now = time(NULL);
	LOG_TRACE("End PreLoadTable %s time:%s", path, ctime(&now));
}

struct lt_Octets
{
	bool operator() (const Octets & o1, const Octets & o2) const
	{
		if( o1.size() < o2.size() )
			return true;
		if( o1.size() > o2.size() )
			return false;
		return memcmp(o1.begin(),o2.begin(),o2.size()) < 0;
	}
};

static bool	g_force = false;
static int	server2_usercount = 0;
static int	same_usercount = 0;
static int	server2_rolecount = 0;
static int	same_rolecount = 0;

static int	dup_roleid = 0;
static int	dup_factionid = 0;
static int	dup_familyid = 0;
static int	dup_crssvrteamsid = 0;

static int	dup_rolename = 0;
static int	dup_factionname = 0;
static int	dup_familyname = 0;
static int	dup_crssvrteamsname = 0;


typedef std::map<int, std::pair<int, int> > MapRoleId; // old_roleid => <new_roleid, new_userid>
static  MapRoleId g_mapRoleid; // 被合服的集合，原服不用变
static	std::map<int,int>	g_mapFactionid;
static	std::map<int,int>	g_mapFamilyid;
static	std::map<int,int>	g_mapCrssvrTeamsid;

static  std::map<int/*newroleid*/, std::map<int/*itemid*/, int/*itemcount*/> > g_luckyroles;

class NameMap
{
	size_t raw_size;
	std::map<Octets, Octets, lt_Octets>	different_name_map;//只存变化的name
public:
	NameMap() : raw_size(0) {}
	void Insert(const Octets & old_name, const Octets & new_name)
	{
		raw_size++;
		if (old_name.size() == 0)
			return;
		if (new_name != old_name)
			different_name_map[old_name] = new_name;
	}
	Octets Find(const Octets & old_name)
	{
		std::map<Octets, Octets, lt_Octets>::const_iterator it;
		it = different_name_map.find(old_name);
		if (it == different_name_map.end()) 
			return old_name;
		else
			return it->second;
	}
	size_t RawSize() { return raw_size; }
	size_t RealSize() { return different_name_map.size(); }
	void Dump()
	{
		for( std::map<Octets, Octets, lt_Octets>::iterator it=different_name_map.begin(), ite=different_name_map.end(); it != ite; ++it )
		{
			Octets	n1 = it->first, n2 = it->second;
			Octets	name1, name2;
			CharsetConverter::conv_charset_u2l( n1, name1 );
			CharsetConverter::conv_charset_u2l( n2, name2 );
			LOG_TRACE( "%.*s,%.*s", name1.size(), (char*)name1.begin(), name2.size(), (char*)name2.begin() );
		}
	}
};
static	NameMap g_mapRolename;
static	NameMap g_mapFactionname;
static  NameMap g_mapFamilyname;
static  NameMap g_mapCrssvrTeamsname;

static	std::string	g_strRoleNamePrefix;
static	std::string	g_strRoleNameSuffix;
static	std::string	g_strFactionNamePrefix;
static	std::string	g_strFactionNameSuffix;
static	std::string	g_strFamilyNamePrefix;
static	std::string	g_strFamilyNameSuffix;
static	std::string	g_strCrssvrTeamsNamePrefix;
static	std::string	g_strCrssvrTeamsNameSuffix;

static	Octets		g_octRoleNamePrefix;
static	Octets		g_octRoleNameSuffix;
static	Octets		g_octFactionNamePrefix;
static	Octets		g_octFactionNameSuffix;
static	Octets		g_octFamilyNamePrefix;
static	Octets		g_octFamilyNameSuffix;
static	Octets		g_octCrssvrTeamsNamePrefix;
static	Octets		g_octCrssvrTeamsNameSuffix;


static	std::set<int>	g_setLogicuid;
inline bool IsLogicuidAvailable( int logicuid )
{
	return ( g_setLogicuid.end() == g_setLogicuid.find( logicuid ) );
}

//当源数据库账号的logicuid被使用时，用于分配从ABNORMAL_LOGICUID_START起始的logicuid
int AllocAbnormalLogicuid()
{
	const static int ABNORMAL_LOGICUID_START=(INT_MAX-10000000)&0xfffffff0;
	static int logicuid = ABNORMAL_LOGICUID_START;
	while (g_setLogicuid.end()!=g_setLogicuid.find(logicuid) && logicuid<=INT_MAX-16)
		logicuid += 16;
	if (logicuid > INT_MAX-16)
		return -1;
	g_setLogicuid.insert(logicuid);
	return logicuid;
}

//手动为user分配新的Logicuid用 解决两个user在两个服务器上分配了同一个logicuid而其中之一不能跨服的情况
int AllocAbnormalLogicuid2(int default_start)
{
	const static int ABNORMAL_LOGICUID_START2=(INT_MAX-5000000)&0xfffffff0;
	static int next_logicuid = default_start > ABNORMAL_LOGICUID_START2 ? (default_start&0xfffffff0) : ABNORMAL_LOGICUID_START2;
	if (next_logicuid > INT_MAX-16)
		return -1;
	else
	{
		next_logicuid += 16;
		return next_logicuid-16;
	}
}

static ItemIdMan g_itemIdMan;
//accessdb.cpp 中的 CheckItemOwner 要同步修改 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
static bool CheckItemOwner(const MapRoleId & maproleid, const string &container, GRoleInventory &inv, int oldroleid)
{
	int item_type = g_itemIdMan.GetItemType(inv.id);
	MapRoleId::const_iterator it;
	if (item_type != -1)
	{
		int ownerid = GetOwnerID(item_type, inv.data);
		if (ownerid > 0)
		{
			if ((it = maproleid.find(ownerid)) != maproleid.end())
			{
				int newownerid = it->second.first;
				if (ownerid != newownerid)
				{
					SetOwnerID(item_type, inv.data, newownerid);
					//			Log::log(LOG_INFO, "The owner's roleid of item id=%d in pos %d of %s of role id=%d is changed from %d to %d\n", inv.id, inv.pos, container.c_str(), oldroleid, ownerid, newownerid);
					return true;
				}
			}
			else
				Log::log(LOG_INFO, "Can't map owner's roleid %d of item id=%d in pos %d of %s of role id=%d to newroleid\n", ownerid, inv.id, inv.pos, container.c_str(), oldroleid);
		}
	}
	else
	{
		int specialid = GetSpecialItemID(inv.id, inv.data);
		if (specialid != -1)
		{
			item_type = g_itemIdMan.GetItemType(specialid);
			if (item_type != -1)
			{
				int ownerid = GetSpecialItemOwnerID(inv.id, item_type, inv.data);
				if (ownerid > 0)
				{
					if ((it = maproleid.find(ownerid)) != maproleid.end())
					{
						int newownerid = it->second.first;
						if (ownerid != newownerid)
						{
							SetSpecialItemOwnerID(inv.id, item_type, inv.data, newownerid);
							LOG_TRACE("The item(id=%d in pos %d of %s)'s specialownerid is changed from %d to %d, on role %d 's body\n", inv.id, inv.pos, container.c_str(), ownerid, newownerid, oldroleid);
							return true;
						}
					}
					else
						LOG_TRACE("can't map item(id=%d in pos %d of %s)'s specialownerid is changed from %d, on role %d 's body\n", inv.id, inv.pos, container.c_str(), ownerid, oldroleid);
				}
			}
		}
	}
	return false;
}

class DupLogicuidQuery : public StorageEnv::IQuery
{
	typedef  bool (DupLogicuidQuery::*UpdateFunc)(StorageEnv::Transaction &, Octets &, Octets &);
	UpdateFunc m_updatefunc;
	std::multimap<int, int> m_mapLogicuid2Userid;
	MapRoleId m_mapRoleid;
	std::map<int, GRoleBase> m_mapBase;
	std::map<int, GRoleStatus> m_mapStatus;
	std::map<int, GFamily> m_mapFamily;
	std::map<int, GCrssvrTeamsData> m_mapCrssvrTeams;

	std::map<int, GFactionInfo> m_mapFaction;
	std::map<int, GMailBox> m_mapMailbox;
	std::map<int, GFriendList> m_mapFriendList;
	std::map<int, GSect> m_mapSect;
	std::map<int64_t, GSNSMessage> m_mapSNSMessage;
	std::map<int64_t, GSysLog> m_mapSyslog;
	std::map<int, User> m_mapUser;
	std::map<int64_t, GConsignDB> m_mapConsign;

	bool WalkTable(const char *tablename, UpdateFunc func)
	{
		m_updatefunc = func;
		try
		{
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage(tablename);
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk( *this );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery walktable error '%s' , what=%s\n", tablename, e.what() );
			return false;
		}
		return true;
	}

	bool UpdateUser(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			User user;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> user;
			if (user.logicuid == 0) 
			{
				RoleList r(user.rolelist);
				if (r.GetRoleCount() != 0)
				{
					Log::log(LOG_ERR, "DupLogicuidQuery::UpdateUser, user %d with logicuid==0 but has roles\n", id);
				}
			}
			else
				m_mapLogicuid2Userid.insert(std::multimap<int, int>::value_type(user.logicuid, id));
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateUser, exception\n" );
		}
		return true;
	}

	bool WalkUser()
	{
		return WalkTable("user", &DupLogicuidQuery::UpdateUser);
	}

	bool UpdateUser2(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			User user;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> user;

			if (user.referrer!=0 && m_mapRoleid.find(user.referrer)!=m_mapRoleid.end())
			{
				int oldreferrer = user.referrer;
				user.referrer = m_mapRoleid[oldreferrer].first;
				m_mapUser[id] = user;
				Log::log(LOG_INFO, "DupLogicuidQuery::UpdateUser2: referrer of user %d is changed from %d to %d\n", id, oldreferrer, user.referrer);
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateUser2, exception\n" );
		}
		return true;
	}

	bool WalkUser2()
	{
		return WalkTable("user", &DupLogicuidQuery::UpdateUser2);
	}

	bool AssignNewLogicuid(int userid, int newlogicuid)
	{
		if (newlogicuid <= 0)
		{
			Log::log(LOG_ERR, "Invalid abnormal logcuid %d for userid %d\n", newlogicuid, userid);
			return false;
		}
		try
		{
			static StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
			static StorageEnv::Storage *pbase = StorageEnv::GetStorage("base");
			StorageEnv::CommonTransaction txn;
			Marshal::OctetsStream key, value_base;
			User user;

			try
			{
				key << userid;
				Marshal::OctetsStream(puser->find(key, txn)) >> user;

				RoleList rolelist(user.rolelist);
				int role = -1;
				while ((role=rolelist.GetNextRole()) != -1)
				{
					int roleid = user.logicuid+role;
					if (pbase->find(Marshal::OctetsStream()<<roleid, value_base, txn))
					{
						GRoleBase base;
						Marshal::OctetsStream(value_base) >> base;
						if (base.userid == 0)
							base.userid = LOGICUID(roleid);

						if (base.userid != userid)
						{
							rolelist.DelRole(role);
							Log::log(LOG_INFO, "Role %d does not belong to user %d but user %d, clear the bit from rolelis of user %d\n", roleid, userid, base.userid, userid);
						}
						else
						{
							m_mapRoleid[roleid] = std::make_pair(newlogicuid+role, 0);
						}
					}
					else
					{
						rolelist.DelRole(role);
						Log::log(LOG_INFO, "Role %d does not exist, clear the bit from rolelis of user %d\n", roleid, userid);
					}
				}

				user.logicuid = newlogicuid;
				user.rolelist = rolelist.GetRoleList();
				puser->insert(key, Marshal::OctetsStream()<<user, txn);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::AssignNewLogicuid error, what=%s\n", e.what() );
			return false;
		}
		Log::log(LOG_INFO, "userid %d is assigned abnormal logicuid %d\n", userid, newlogicuid);
		return true;
	}

	bool UpdateBase(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			Marshal::OctetsStream(key) >> id;
			GRoleBase base;
			Marshal::OctetsStream(value) >> base;
			bool need_repair = false;
			if (m_mapRoleid.find(id) != m_mapRoleid.end())
			{
				id = m_mapRoleid[id].first;
				base.id = id;
				need_repair = true;
			}
			if (m_mapRoleid.find(base.spouse) != m_mapRoleid.end())
			{
				base.spouse = m_mapRoleid[base.spouse].first;
				need_repair = true;
			}
			if (m_mapRoleid.find(base.sectid) != m_mapRoleid.end())
			{
				base.sectid = m_mapRoleid[base.sectid].first;
				need_repair = true;
			}
			if (need_repair)
			{
				m_mapBase[id] = base;
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateBase, exception\n" );
		}
		return true;
	}

	bool WalkBase()
	{
		return WalkTable("base", &DupLogicuidQuery::UpdateBase);
	}

	bool UpdateFamily(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GFamily family;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> family;
			std::vector<GFolk>::iterator it, ie = family.member.end();
			bool need_repair = false;
			for (it = family.member.begin(); it != ie; ++it)
			{
				if (m_mapRoleid.find(it->rid) != m_mapRoleid.end())
				{
					it->rid = m_mapRoleid[it->rid].first;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapFamily[id] = family;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateFamily, exception\n" );
		}
		return true;
	}

	bool WalkFamily()
	{
		return WalkTable("family", &DupLogicuidQuery::UpdateFamily);
	}

	bool UpdateCrssvrTeams(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GCrssvrTeamsData crssvrteams;
			Marshal::OctetsStream(key) >> id;
			if(id <= 0) return true;
			Marshal::OctetsStream(value) >> crssvrteams;
			std::vector<GCrssvrTeamsRole>::iterator it, ie = crssvrteams.roles.end();
			bool need_repair = false;
			for (it = crssvrteams.roles.begin(); it != ie; ++it)
			{
				if (m_mapRoleid.find(it->roleid) != m_mapRoleid.end())
				{
					it->roleid = m_mapRoleid[it->roleid].first;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapCrssvrTeams[id] = crssvrteams;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateCrssvrTeama, exception\n" );
		}
		return true;
	}

	bool WalkCrssvrTeams()
	{
		return WalkTable("crssvrteams", &DupLogicuidQuery::UpdateCrssvrTeams);
	}


	bool UpdateConsign(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int64_t sn;
			GConsignDB detail;
			Marshal::OctetsStream(key) >> sn;
			if(sn == 0)
				return true;
			Marshal::OctetsStream(value) >> detail;
			bool need_repair = false;
			//check item owner
			if(CheckItemOwner(m_mapRoleid, "consign", detail.item, detail.seller_roleid))
				need_repair = true;

			if (m_mapRoleid.find(detail.seller_roleid) != m_mapRoleid.end())
			{
				detail.seller_roleid = m_mapRoleid[detail.seller_roleid].first;
				need_repair = true;
			}
			if (m_mapRoleid.find(detail.buyer_roleid) != m_mapRoleid.end())
			{
				detail.buyer_roleid = m_mapRoleid[detail.buyer_roleid].first;
				need_repair = true;
			}
			
			if (need_repair)
				m_mapConsign[sn] = detail;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateConsign, exception\n" );
		}
		return true;
	}

	bool WalkConsign()
	{
		return WalkTable("consign", &DupLogicuidQuery::UpdateConsign);
	}

	bool UpdateFaction(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GFactionInfo faction;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> faction;
			if (m_mapRoleid.find(faction.master) != m_mapRoleid.end())
			{
				faction.master = m_mapRoleid[faction.master].first;
				m_mapFaction[id] = faction;
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateFaction, exception\n" );
		}
		return true;
	}

	bool WalkFaction()
	{
		return WalkTable("faction", &DupLogicuidQuery::UpdateFaction);
	}

	bool UpdateMailbox(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GMailBox mailbox;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> mailbox;
			GMailVector::iterator it, ie = mailbox.mails.end();
			bool need_repair = false;
			if (m_mapRoleid.find(id) != m_mapRoleid.end())
			{
				id = m_mapRoleid[id].first;
				need_repair = true;
			}
			for (it = mailbox.mails.begin(); it != ie; ++it)
			{
				if(it->header.sndr_type == _MST_LOCAL_CONSIGN)//skip sender change for consign mail
					continue;
				if (m_mapRoleid.find(it->header.sender) != m_mapRoleid.end())
				{
					it->header.sender = m_mapRoleid[it->header.sender].first;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapMailbox[id] = mailbox;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateMailbox, exception\n" );
		}
		return true;
	}

	bool WalkMailbox()
	{
		return WalkTable("mailbox", &DupLogicuidQuery::UpdateMailbox);
	}

	bool UpdateFriendList(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GFriendList friendlist;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> friendlist;
			bool need_repair = false;
			if (m_mapRoleid.find(id) != m_mapRoleid.end())
			{
				id = m_mapRoleid[id].first;
				need_repair = true;
			}

			GFriendInfoVector::iterator fit, fie = friendlist.friends.end();
			for (fit = friendlist.friends.begin(); fit != fie; ++fit)
			{
				if (m_mapRoleid.find(fit->rid) != m_mapRoleid.end())
				{
					fit->rid = m_mapRoleid[fit->rid].first;
					need_repair = true;
				}
			}

			GEnemyInfoVector::iterator eit, eie = friendlist.enemies.end();
			for (eit = friendlist.enemies.begin(); eit != eie; ++eit)
			{
				if (m_mapRoleid.find(eit->rid) != m_mapRoleid.end())
				{
					eit->rid = m_mapRoleid[eit->rid].first;
					need_repair = true;
				}
			}
			
			if (need_repair)
				m_mapFriendList[id] = friendlist;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateFriendList, exception\n" );
		}
		return true;
	}

	bool WalkFriendList()
	{
		return WalkTable("friends", &DupLogicuidQuery::UpdateFriendList);
	}

	bool UpdateSect(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GSect sect;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> sect;
			bool need_repair = false;
			if (m_mapRoleid.find(id) != m_mapRoleid.end())
			{
				id = m_mapRoleid[id].first;
				need_repair = true;
			}

			std::vector<GDisciple>::iterator it, ie = sect.disciples.end();
			for (it = sect.disciples.begin(); it != ie; ++it)
			{
				if (m_mapRoleid.find(it->roleid) != m_mapRoleid.end())
				{
					it->roleid = m_mapRoleid[it->roleid].first;
					need_repair = true;
				}
			}

			if (need_repair)
				m_mapSect[id] = sect;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateSect, exception\n" );
		}
		return true;
	}

	bool WalkSect()
	{
		return WalkTable("sect", &DupLogicuidQuery::UpdateSect);
	}

	bool UpdateSyslog(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int64_t nkey;
			GSysLog syslog;
			Marshal::OctetsStream(key) >> nkey;
			Marshal::OctetsStream(value) >> syslog;
			if (m_mapRoleid.find(syslog.roleid) != m_mapRoleid.end())
			{
				syslog.roleid = m_mapRoleid[syslog.roleid].first;
				m_mapSyslog[nkey] = syslog;
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateSect, exception\n" );
		}
		return true;
	}

	bool WalkSyslog()
	{
		return WalkTable("syslog", &DupLogicuidQuery::UpdateSyslog);
	}

	bool UpdateSNSMessage(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int64_t nkey;
			GSNSMessage msg;
			Marshal::OctetsStream(key) >> nkey;
			Marshal::OctetsStream(value) >> msg;
			bool need_repair = false;
			if (m_mapRoleid.find(msg.roleid) != m_mapRoleid.end())
			{
				msg.roleid = m_mapRoleid[msg.roleid].first;
				need_repair = true;
			}
			GApplyInfoVector::iterator it, ie = msg.apply_list.end();
			for (it = msg.apply_list.begin(); it != ie; ++it)
			{
				if (m_mapRoleid.find(it->roleid) != m_mapRoleid.end())
				{
					it->roleid = m_mapRoleid[it->roleid].first;
					need_repair = true;
				}
			}
			ie = msg.agree_list.end();
			for (it = msg.agree_list.begin(); it != ie; ++it)
			{
				if (m_mapRoleid.find(it->roleid) != m_mapRoleid.end())
				{
					it->roleid = m_mapRoleid[it->roleid].first;
					need_repair = true;
				}
			}
			ie = msg.deny_list.end();
			for (it = msg.deny_list.begin(); it != ie; ++it)
			{
				if (m_mapRoleid.find(it->roleid) != m_mapRoleid.end())
				{
					it->roleid = m_mapRoleid[it->roleid].first;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapSNSMessage[nkey] = msg;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::UpdateSect, exception\n" );
		}
		return true;
	}

	bool WalkSNSMessage()
	{
		return WalkTable("snsmessage", &DupLogicuidQuery::UpdateSNSMessage);
	}

public:
	bool RepairUser()
	{
		LOG_TRACE("DupLogicuidQuery::RepairUser");
		if (!WalkUser()) return false;
		std::multimap<int, int>::iterator it = m_mapLogicuid2Userid.begin(), ie = m_mapLogicuid2Userid.end();
		while (it != ie)
		{
			int logicuid = it->first;
			int userid = it->second;
			++it;
			while (it!=ie && it->first==logicuid)
			{
				Log::log(LOG_INFO, "userid %d and userid %d both has logicuid %d\n", userid, it->second, logicuid);
				int newlogicuid = AllocAbnormalLogicuid();
				if (newlogicuid <= 0)
				{
					Log::log(LOG_ERR, "Alloc abnormal logicuid for user %d failed", it->second);
					return false;
				}
				if (!AssignNewLogicuid(it->second, newlogicuid))
					return false;
				++it;
			}
		}
		StorageEnv::checkpoint();
		return true;
	}
	bool RepairSpecifiedUser(const std::set<int> & user_set, int default_start)
	{
		LOG_TRACE("DupLogicuidQuery::RepairUser");
		std::set<int>::const_iterator it, ite = user_set.end();
		for (it = user_set.begin(); it != ite; ++it)
		{
			int newlogicuid = AllocAbnormalLogicuid2(default_start);
			if (newlogicuid <= 0)
			{
				Log::log(LOG_ERR, "Alloc abnormal logicuid for user %d failed", *it);
				return false;
			}
			Log::log(LOG_INFO, "Assign new logicuid %d for user %d", newlogicuid, *it);
			if (!AssignNewLogicuid(*it, newlogicuid))
				return false;
		}
		StorageEnv::checkpoint();
		return true;
	}
	bool RepairRoleData()
	{
		LOG_TRACE("DupLogicuidQuery::RepairRoleData");
		if (!WalkBase()) return false;
		
		//修复base
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pbase = StorageEnv::GetStorage("base");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					pbase->del(Marshal::OctetsStream()<<it->first, txn);
				}
				std::map<int, GRoleBase>::iterator bit, bie=m_mapBase.end();
				for (bit=m_mapBase.begin(); bit != bie; ++bit)
				{
					pbase->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairBase error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复rolename
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *prolename = StorageEnv::GetStorage("rolename");

			try
			{
				std::map<int, GRoleBase>::iterator bit, bie=m_mapBase.end();
				for (bit=m_mapBase.begin(); bit != bie; ++bit)
				{
					prolename->insert(bit->second.name, Marshal::OctetsStream()<<bit->first, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairRolename error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复base2
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pbase2 = StorageEnv::GetStorage("base2");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pbase2->find(key, value, txn))
					{
						pbase2->del(key, txn);
						GRoleBase2 base2;
						Marshal::OctetsStream(value) >> base2;
						base2.id = it->second.first;
						pbase2->insert(Marshal::OctetsStream()<<it->second.first, Marshal::OctetsStream()<<base2, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairBase2 error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复status
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pstatus = StorageEnv::GetStorage("status");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pstatus->find(key, value, txn))
					{
						pstatus->del(key, txn);
						GRoleStatus status;
						Marshal::OctetsStream(value) >> status;
						status.id = it->second.first;
						pstatus->insert(Marshal::OctetsStream()<<it->second.first, Marshal::OctetsStream()<<status, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepaiStatus error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复inventory
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pinv = StorageEnv::GetStorage("inventory");

			try
			{
				//只遍历了换了roleid的角色自身的包裹 没有遍历所有玩家包裹 太费了
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pinv->find(key, value, txn))
					{
						pinv->del(key, txn);
						GRolePocket pocket;
						Marshal::OctetsStream(value) >> pocket;
						GRoleInventoryVector::iterator cit = pocket.items.begin();
						for (; cit != pocket.items.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);

						cit = pocket.equipment.begin();
						for (; cit != pocket.equipment.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);

						cit = pocket.petbadge.begin();
						for (; cit != pocket.petbadge.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);

						cit = pocket.petequip.begin();
						for (; cit != pocket.petequip.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);
						
						cit = pocket.fashion.begin();
						for (; cit != pocket.fashion.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);
						
						if (pocket.mountwing.size())
						{
							Marshal::OctetsStream os_mountwing(pocket.mountwing);
							GRoleInventoryVector mountwing;
							short mountwingsize = 0;
							os_mountwing >> mountwingsize;
							os_mountwing >> mountwing;
							cit = mountwing.begin();
							for (; cit != mountwing.end(); ++ cit)
								CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);
							os_mountwing.clear();
							os_mountwing << mountwingsize;
							os_mountwing << mountwing;
							pocket.mountwing = os_mountwing;
						}

						cit = pocket.gifts.begin();
						for (; cit != pocket.gifts.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "inventory", *cit, it->first);

						pinv->insert(Marshal::OctetsStream()<<it->second.first, Marshal::OctetsStream()<<pocket, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairinventory error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复storehouse
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pstorehouse = StorageEnv::GetStorage("storehouse");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pstorehouse->find(key, value, txn))
					{
						pstorehouse->del(key, txn);
						GRoleStorehouse store;
						Marshal::OctetsStream(value) >> store;
						GRoleInventoryVector::iterator cit = store.items.begin();
						for (; cit != store.items.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "storehouse", *cit, it->first);

						cit = store.items2.begin();
						for (; cit != store.items2.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "storehouse", *cit, it->first);
						
						cit = store.fuwen.begin();
						for (; cit != store.fuwen.end(); ++ cit)
							CheckItemOwner(m_mapRoleid, "storehouse", *cit, it->first);
	
						pstorehouse->insert(Marshal::OctetsStream()<<it->second.first, Marshal::OctetsStream()<<store, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairstorehouse error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复task
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *ptask = StorageEnv::GetStorage("task");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (ptask->find(key, value, txn))
					{
						ptask->del(key, txn);
						ptask->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairtask error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		//修复成就
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pachieve = StorageEnv::GetStorage("achievement");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pachieve->find(key, value, txn))
					{
						pachieve->del(key, txn);
						pachieve->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairachievement error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复waitdel
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pwaitdel = StorageEnv::GetStorage("waitdel");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << WaitDelKey(it->first, TYPE_ROLE);
					if (pwaitdel->find(key, value, txn))
					{
						pwaitdel->del(key, txn);
						pwaitdel->insert(Marshal::OctetsStream()<<WaitDelKey(it->second.first, TYPE_ROLE), value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairwaitdel error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		//修复clrrole
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pclrrole = StorageEnv::GetStorage("clrrole");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pclrrole->find(key, value, txn))
					{
						pclrrole->del(key, txn);
						pclrrole->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairclrrole error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pgt = StorageEnv::GetStorage("gtactivate");
			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pgt->find(key, value, txn))
					{
						pgt->del(key, txn);
						pgt->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairgt error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pcallback = StorageEnv::GetStorage("friendcallback");
			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (pcallback->find(key, value, txn))
					{
						pcallback->del(key, txn);
						pcallback->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairfriendcallback error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *paward = StorageEnv::GetStorage("award");
			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (paward->find(key, value, txn))
					{
						paward->del(key, txn);
						paward->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairaward error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *prolenamehis = StorageEnv::GetStorage("rolenamehis");
			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it = m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (prolenamehis->find(key, value, txn))
					{
						prolenamehis->del(key, txn);
						prolenamehis->insert(Marshal::OctetsStream()<<it->second.first, value, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::repairrolenamehis error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();

		m_mapBase.clear();
		return true;
	}

	bool RepairFamily()
	{
		LOG_TRACE("DupLogicuidQuery::RepairFamily");
		if (!WalkFamily()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pfamily = StorageEnv::GetStorage("family");

			try
			{
				std::map<int, GFamily>::iterator bit, bie=m_mapFamily.end();
				for (bit=m_mapFamily.begin(); bit != bie; ++bit)
				{
					pfamily->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairFamily error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapFamily.clear();
		return true;
	}

	bool RepairFaction()
	{
		LOG_TRACE("DupLogicuidQuery::RepairFaction");
		if (!WalkFaction()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pfaction = StorageEnv::GetStorage("faction");

			try
			{
				std::map<int, GFactionInfo>::iterator bit, bie=m_mapFaction.end();
				for (bit=m_mapFaction.begin(); bit != bie; ++bit)
				{
					pfaction->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairFaction error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapFaction.clear();
		return true;
	}

	bool RepairCrssvrTeams()
	{
		LOG_TRACE("DupLogicuidQuery::RepairCrssvrTeams");
		if (!WalkCrssvrTeams()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pcrssvrteams = StorageEnv::GetStorage("crssvrteams");

			try
			{
				std::map<int, GCrssvrTeamsData>::iterator bit, bie=m_mapCrssvrTeams.end();
				for (bit=m_mapCrssvrTeams.begin(); bit != bie; ++bit)
				{
					pcrssvrteams->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairCrssvrTeams error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapCrssvrTeams.clear();
		return true;
	}

	bool RepairMailbox()
	{
		LOG_TRACE("DupLogicuidQuery::RepairMailbox");
		if (!WalkMailbox()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pmailbox = StorageEnv::GetStorage("mailbox");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it=m_mapRoleid.begin(); it != ie; ++it)
				{
					pmailbox->del(Marshal::OctetsStream()<<it->first, txn);
				}

				std::map<int, GMailBox>::iterator bit, bie=m_mapMailbox.end();
				for (bit=m_mapMailbox.begin(); bit != bie; ++bit)
				{
					pmailbox->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairMailbox error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapMailbox.clear();
		return true;
	}
	
	bool RepairFriendList()
	{
		LOG_TRACE("DupLogicuidQuery::RepairFriendlist");
		if (!WalkFriendList()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pfriendlist = StorageEnv::GetStorage("friends");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it=m_mapRoleid.begin(); it != ie; ++it)
				{
					pfriendlist->del(Marshal::OctetsStream()<<it->first, txn);
				}

				std::map<int, GFriendList>::iterator bit, bie=m_mapFriendList.end();
				for (bit=m_mapFriendList.begin(); bit != bie; ++bit)
				{
					pfriendlist->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairFriendList error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapFriendList.clear();
		return true;
	}
	
	bool RepairSect()
	{
		LOG_TRACE("DupLogicuidQuery::RepairSect");
		if (!WalkSect()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *psect = StorageEnv::GetStorage("sect");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it=m_mapRoleid.begin(); it != ie; ++it)
				{
					psect->del(Marshal::OctetsStream()<<it->first, txn);
				}

				std::map<int, GSect>::iterator bit, bie=m_mapSect.end();
				for (bit=m_mapSect.begin(); bit != bie; ++bit)
				{
					psect->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairSect error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapSect.clear();
		return true;
	}

	bool RepairSyslog()
	{
		LOG_TRACE("DupLogicuidQuery::RepairSyslog");
		if (!WalkSyslog()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *psyslog = StorageEnv::GetStorage("syslog");

			try
			{
				std::map<int64_t, GSysLog>::iterator it, ie = m_mapSyslog.end();
				for (it=m_mapSyslog.begin(); it != ie; ++it)
				{
					psyslog->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairSyslog error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapSyslog.clear();
		return true;
	}

	bool RepairSNS()
	{
		LOG_TRACE("DupLogicuidQuery::RepairSNS");
		if (!WalkSNSMessage()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pmessage = StorageEnv::GetStorage("snsmessage");

			try
			{
				std::map<int64_t, GSNSMessage>::iterator it, ie = m_mapSNSMessage.end();
				for (it=m_mapSNSMessage.begin(); it != ie; ++it)
				{
					pmessage->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairSNSMessage error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapSNSMessage.clear();

		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *psnsplayer = StorageEnv::GetStorage("snsplayerinfo");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it=m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (psnsplayer->find(key, value, txn))
					{
						psnsplayer->del(key, txn);
						GSNSPlayerInfoCache info;
						Marshal::OctetsStream(value) >> info;
						if (m_mapRoleid.find(info.roleinfo.spouse) != m_mapRoleid.end())
							info.roleinfo.spouse = m_mapRoleid[info.roleinfo.spouse].first;
						if (m_mapRoleid.find(info.playerinfo.roleid) != m_mapRoleid.end())
							info.playerinfo.roleid = m_mapRoleid[info.playerinfo.roleid].first;
						psnsplayer->insert(Marshal::OctetsStream()<<it->second.first, Marshal::OctetsStream()<<info, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairSNSPlayerInfo error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		return true;
	}

	bool RepairReference()
	{
		LOG_TRACE("DupLogicuidQuery::RepairReference");
		if (!WalkUser2()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *puser = StorageEnv::GetStorage("user");

			try
			{
				std::map<int, User>::iterator it, ie = m_mapUser.end();
				for (it=m_mapUser.begin(); it != ie; ++it)
				{
					puser->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairReference error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapUser.clear();

		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *preferral = StorageEnv::GetStorage("referral");

			try
			{
				MapRoleId::iterator it, ie = m_mapRoleid.end();
				for (it=m_mapRoleid.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream key, value;
					key << it->first;
					if (preferral->find(key, value, txn))
					{
						preferral->del(key, txn);
						GReferral referral;
						Marshal::OctetsStream(value) >> referral;
						referral.roleid = it->second.first;
						preferral->insert(Marshal::OctetsStream()<<it->second.first, Marshal::OctetsStream()<<referral, txn);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairReferral error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		return true;
	}

	bool RepairConsign()
	{
		LOG_TRACE("DupLogicuidQuery::RepairConsign");
		if(m_mapRoleid.size()==0)
			return true;
		if (!WalkConsign()) return false;
		try
		{
			StorageEnv::CommonTransaction txn;
			StorageEnv::Storage *pconsign = StorageEnv::GetStorage("consign");

			try
			{
				std::map<int64_t, GConsignDB>::iterator bit, bie=m_mapConsign.end();
				for (bit=m_mapConsign.begin(); bit != bie; ++bit)
				{
					if(bit->first!=0)
						pconsign->insert(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second, txn);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "DupLogicuidQuery::RepairConsign error, what=%s\n", e.what() );
			return false;
		}
		StorageEnv::checkpoint();
		m_mapConsign.clear();
		return true;
	}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		return (this->*m_updatefunc)(txn, key, value);
	}
	unsigned int GetMapSize()
	{
		return m_mapRoleid.size();
	}
};

bool CheckDupLogicuid()
{
	LOG_TRACE( "CheckDupLogicuid:" );
	PreLoadTable((StorageEnv::get_datadir()+"/user").c_str());
	DupLogicuidQuery q;
	if (!q.RepairUser())
		return false;
	if (q.GetMapSize() == 0)
	{
		LOG_TRACE("No duplicate logicuid");
		return true;
	}
	return (q.RepairRoleData()
			&& q.RepairFamily()
			&& q.RepairFaction()
			&& q.RepairCrssvrTeams()
			&& q.RepairMailbox()
			&& q.RepairFriendList()
			&& q.RepairSect()
			&& q.RepairSyslog()
			&& q.RepairSNS()
			&& q.RepairReference()
			&& q.RepairConsign());
}

class PrepareLogicuidQuery : public StorageEnv::IQuery
{
	std::vector<int>& setRole;
public:
	PrepareLogicuidQuery(std::vector<int> &set):setRole(set){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int id = -1;
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);

			os_key >> id;
			g_setLogicuid.insert(LOGICUID(id));
			GRoleBase base;
			os_value >> base;
			if (base.status == _ROLE_STATUS_FROZEN)
			{
				Log::log(LOG_ERR, "PrepareLogicuidQuery, roleid %d status is _ROLE_STATUS_FROZEN", id);
				if (!g_force)
					throw Marshal::Exception();
			}
			if (base.circletrack.size() != 0)
				setRole.push_back(id);
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "PrepareLogicuidQuery, exception, roleid=%d\n", id);
			throw e;
		}
		return true;
	}
};

class CheckFrozenQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);

			int id = -1;
			os_key >> id;
			GRoleBase base;
			os_value >> base;
			if (base.status == _ROLE_STATUS_FROZEN)
			{
				Log::log(LOG_ERR, "CheckFrozenQuery, roleid %d status is _ROLE_STATUS_FROZEN", id);
				if (!g_force)
					throw Marshal::Exception();
			}
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "CheckFrozenQuery, exception\n" );
			throw e;
		}
		return true;
	}
};

int GetDstDBType()
{
	try {   
		StorageEnv::Storage *pstorage = StorageEnv::GetStorage("config");
		StorageEnv::CommonTransaction txn;
		try {
			DBConfig config;
			Marshal::OctetsStream(pstorage->find(Marshal::OctetsStream()<<(int)100, txn)) >> config;
			LOG_TRACE("Dst DB type %d", config.is_central_db);
			return config.is_central_db;
		} catch (DbException &e) { 
			throw;  
		} catch (...) { 
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}       
	}
	catch ( DbException e )
	{       
		Log::log( LOG_ERR, "ReadDBConfig(dst) exception, what=%s\n", e.what() );
	}
	return -1;
}

int GetDBType(TableWrapper & tab_config)
{
	try
	{   
		DBConfig config;
		Marshal::OctetsStream value;
		if (!tab_config.Find(Marshal::OctetsStream()<<(int)100, value))
			return -1;
		value >> config;
		LOG_TRACE("DB type %d", config.is_central_db);
		return config.is_central_db;
	}
	catch ( ... )
	{       
		Log::log( LOG_ERR, "Read DBConfig exception");
	}
	return -1;
}

bool CheckDBType(TableWrapper & tab_config, TableWrapper & tab_base)
{
	if (GetDstDBType() == 1)
		return false;
	if (GetDBType(tab_config) == 1)
		return false;
	PreLoadTable(tab_base.name.c_str());
	CheckFrozenQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_base.name.c_str(), tab_base.ptable, tab_base.uncompressor);
			cursor.walk( q );
		}
		catch (...)
		{
			throw;
		}
	}
	catch (...)
	{
		Log::log(LOG_ERR, "CheckFrozen exception");
		if (!g_force)
			return false;
	}
	return true;
}

bool PrepareLogicuid()
{
	LOG_TRACE( "Prepare Logicuid:" );
	PreLoadTable((StorageEnv::get_datadir()+"/base").c_str());
	try
	{
		std::vector<int> setNeedClearCircle;
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			PrepareLogicuidQuery q(setNeedClearCircle);
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		LOG_TRACE( "ClearDstCircleHis: process %d roles", setNeedClearCircle.size());
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			StorageEnv::CommonTransaction     txn;
			try
			{
				std::vector<int>::iterator it, ie = setNeedClearCircle.end();

				for (it = setNeedClearCircle.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream	os_key;
					os_key<<*it;
					Marshal::OctetsStream os_value(pstorage->find(os_key,txn));
					GRoleBase role;
					os_value >> role;
					role.circletrack.clear();
					pstorage->insert( os_key, Marshal::OctetsStream()<<role, txn );
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "PrepareLogicuid, error when walk for PrepareLogicuidQuery, what=%s\n", e.what() );
		if (!g_force)
			return false;
	}
	StorageEnv::checkpoint();
	return true;
}

class MergeUserQuery : public StorageEnv::IQuery
{
public:
	std::vector<int> m_setUserNeedSetReferrer;	//需要重新指定referrer的账号列表
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int userid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("user");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			userid = -1;
			User	user1, user2;
			os_key >> userid;
			if( userid < 128 )
				return true;

			server2_usercount ++;

			os_value2 >> user2;
			if( pstorage->find( key, os_value1, txn ) )
			{
				os_value1 >> user1;
				if( 0 == user1.logicuid && (user1.rolelist&0xFFFF))
				{
					Log::log(LOG_INFO, "\tERROR:cannot determine logicuid for user1, discard roles, userid=%d,rolelist1=%x",
							userid, user1.rolelist);
					user1.rolelist = ROLELIST_DEFAULT;
				}
				if(user1.logicuid && !(user1.rolelist&0xFFFF)) 
					user1.logicuid = 0;

				if(0 == user2.logicuid && (user2.rolelist&0xFFFF))
				{
					Log::log(LOG_INFO, "\tERROR:cannot determine logicuid for user2, discard roles, userid=%d,rolelist2=%x",
							userid, user2.rolelist);
					user2.rolelist = ROLELIST_DEFAULT;
				}
				if(user2.logicuid && !(user2.rolelist&0xFFFF)) 
					user2.logicuid = 0;

				if( !user1.logicuid && user2.logicuid ) 
				{
					if(IsLogicuidAvailable( user2.logicuid ) )
					{
						user1.logicuid = user2.logicuid;
						g_setLogicuid.insert( user2.logicuid );
					}
					else
					{
						int logicuid = AllocAbnormalLogicuid();
						if (logicuid != -1)
						{
							user1.logicuid = logicuid;
							Log::log(LOG_INFO, "\tWARN:logicuid already used, alloc abnormal logicuid, userid=%d,abnormal_logicuid=%d, logicuid2=%d,rolelist1=%x,rolelist2=%x",
									userid, user1.logicuid, user2.logicuid, user1.rolelist, user2.rolelist );
						}
						else
						{
							Log::log(LOG_INFO, "\tERROR:logicuid already used, fail to alloc abnormal logicuid, userid=%d, logicuid1=%d, logicuid2=%d,rolelist1=%x,rolelist2=%x",
									userid, user1.logicuid, user2.logicuid, user1.rolelist, user2.rolelist );
							user2.rolelist = ROLELIST_DEFAULT;
						}
					}
				}

				int _same = user1.rolelist & user2.rolelist;
				int _sum  = user1.rolelist | user2.rolelist;
				int _more = (_sum & ~user1.rolelist) | ROLELIST_DEFAULT;
				RoleList r1(user1.rolelist), r2(user2.rolelist), r_same(_same), r_sum(_sum), r_more(_more);


				same_usercount ++;
				same_rolecount += r_same.GetRoleCount();

				int role = -1;
				while ( (role = r_more.GetNextRole()) >= 0 )
				{
					g_mapRoleid[user2.logicuid+role] = std::make_pair(user1.logicuid+role, userid);
					if( user2.logicuid && user2.logicuid!=user1.logicuid )
					{
						Log::log(LOG_INFO, "\tWARN:logicuid not equal:userid=%d,roleid1=%d,roleid2=%d",
								userid, user1.logicuid+role, user2.logicuid+role );
						dup_roleid ++;
					}
				}
				role = -1;
				while ( (role = r_same.GetNextRole()) >= 0 )
				{
					int newrole = -1;
					if( (newrole = r_sum.AddRole()) >= 0 )
					{
						if( user2.logicuid!=user1.logicuid )
						{
							Log::log(LOG_INFO, "\tWARN:logicuid not equal:userid=%d,roleid1=%d,roleid2=%d",
									userid, user1.logicuid+newrole, user2.logicuid+role );
						}
						g_mapRoleid[user2.logicuid+role] = std::make_pair(user1.logicuid+newrole, userid);
						dup_roleid ++;
					}
					else
					{
						Log::log(LOG_INFO, "\tINFO:rolelist full.u=%d,logicuid1=%d,logicuid2=%d,r1=%x,r2=%x,discard=%d,sum=%x.",
								userid, user1.logicuid, user2.logicuid, user1.rolelist,
								user2.rolelist, role, r_sum.GetRoleList() );
						g_mapRoleid[user2.logicuid+role] = std::make_pair(0, userid);
					}
				}
				user1.rolelist = r_sum.GetRoleList();
				user1.cash += user2.cash;
				user1.money += user2.money;
				user1.cash_add += user2.cash_add;
				user1.data_group[USER_DATA_CASHADD2] += user2.data_group[USER_DATA_CASHADD2];
				user1.cash_buy += user2.cash_buy;
				user1.cash_sell += user2.cash_sell;
				user1.cash_used += user2.cash_used;
				user1.add_serial = std::max(user1.add_serial, user2.add_serial);
				user1.use_serial = std::max(user1.use_serial, user2.use_serial);
				user1.exg_log.insert( user1.exg_log.end(), user2.exg_log.begin(), user2.exg_log.end() );
				while( user1.exg_log.size() > 80 )
					user1.exg_log.erase( user1.exg_log.begin() );
				if( user1.autolock.size() < user2.autolock.size() )
					user1.autolock = user2.autolock;
				if( user1.cash_password.size() == 0 && user2.cash_password.size() > 0 )
					user1.cash_password	=	user2.cash_password;
				GRoleForbidVector::iterator forbid_it2, forbid_ite2, forbid_it, forbid_ite; 
				for (forbid_it2 = user2.forbid.begin(), forbid_ite2 = user2.forbid.end(); forbid_it2 != forbid_ite2; forbid_it2++)	
				{
					if (forbid_it2->type == FORBID_USER_LOGIN)
					{
						for (forbid_it = user1.forbid.begin(), forbid_ite = user1.forbid.end(); forbid_it != forbid_ite; forbid_it++)
						{
							if (forbid_it->type == FORBID_USER_LOGIN)
							{
								if ( (forbid_it2->createtime+forbid_it2->time) > (forbid_it->createtime+forbid_it->time) )
									*forbid_it = *forbid_it2;
								break;
							}
						}
						if (forbid_it == forbid_ite)
							user1.forbid.push_back(*forbid_it2);
						break;
					}
				}
				//检查该账号在两个服务器的推广人
				if (user2.referrer!=0 && user1.referrer==0)
				{
					user1.referrer = user2.referrer;
					m_setUserNeedSetReferrer.push_back(userid);
				}
				user1.unprocessed_orders.insert(user1.unprocessed_orders.end(), user2.unprocessed_orders.begin(), user2.unprocessed_orders.end());
				//vector 去重 决不允许有重复 orderid 情况出现
				std::sort(user1.unprocessed_orders.begin(), user1.unprocessed_orders.end());
				std::vector<int64_t>::iterator end_uniq = std::unique(user1.unprocessed_orders.begin(), user1.unprocessed_orders.end());
				if (end_uniq != user1.unprocessed_orders.end())
				{
					Log::log(LOG_ERR, "user %d has duplicate unprocessed weborders", userid);
					user1.unprocessed_orders.erase(end_uniq, user1.unprocessed_orders.end());
				}
				pstorage->insert( key, Marshal::OctetsStream()<<user1, txn );
			}
			else
			{
				RoleList r2(user2.rolelist);

				if(user2.logicuid && !(user2.rolelist&0xFFFF))
					user2.logicuid = 0;

				int newlogicuid = user2.logicuid;
				if( user2.logicuid > 0 )
				{
					if( IsLogicuidAvailable( user2.logicuid ) )
						g_setLogicuid.insert( user2.logicuid );
					else
					{
						int logicuid = AllocAbnormalLogicuid();
						if (logicuid != -1)
						{
							newlogicuid = logicuid;
							Log::log(LOG_INFO, "\tWARN:logicuid already used, alloc abnormal logicuid, userid=%d,abnormal_logicuid=%d, logicuid2=%d, rolelist2=%x",
									userid, newlogicuid, user2.logicuid, user2.rolelist );
						}
						else
						{
							Log::log(LOG_INFO, "\tERROR:logicuid already used, fail to alloc abnormal logicuid, userid=%d, logicuid2=%d, rolelist2=%x",
									userid, user2.logicuid, user2.rolelist );
							r2 = RoleList();
							user2.rolelist = ROLELIST_DEFAULT;
						}
					}
				}
				else if(user2.rolelist&0xFFFF)
				{
					Log::log(LOG_INFO, "\tERROR:cannot determine logicuid for user2, discard roles, userid=%d,rolelist2=%x",
							userid, user2.rolelist);
					r2 = RoleList();
					user2.rolelist = ROLELIST_DEFAULT;
				}

				int role = -1;
				while ( (role = r2.GetNextRole()) >= 0 )
				{
					g_mapRoleid[user2.logicuid+role] = std::make_pair(newlogicuid+role, userid);
				}

				user2.logicuid = newlogicuid;
				if (user2.referrer != 0)
				{
					m_setUserNeedSetReferrer.push_back(userid);
				}
				pstorage->insert( key, Marshal::OctetsStream()<<user2, txn );
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: user date error, userid=%d",userid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

class MergeDropCounterQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("dropcounter");
			Marshal::OctetsStream os_value1, os_value2(value);

			int count2 = 0;

			os_value2 >> count2;
			if (pstorage->find(key, os_value1, txn))
			{
				int count1 = 0;
				os_value1 >> count1;
				count2 += count1;
			}
			pstorage->insert(key, Marshal::OctetsStream()<<count2, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: dropcounter data error");
		}
		return true;
	}
};

void MergeUser(TableWrapper & tab_user)
{
	LOG_TRACE( "Merge user:" );

	PreLoadTable((StorageEnv::get_datadir()+"/user").c_str());
	PreLoadTable(tab_user.name.c_str());
	MergeUserQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_user.name.c_str(), tab_user.ptable, tab_user.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeUser, error when walk, what=%s\n", e.what() );
	}

	//为需要重新指定referrer的账号设置新的referrer
	LOG_TRACE("m_setUserNeedSetReferrer.size %d", q.m_setUserNeedSetReferrer.size());
	try
	{
		StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
		StorageEnv::CommonTransaction txn;
		Marshal::OctetsStream value;

		try
		{
			std::vector<int>::iterator it = q.m_setUserNeedSetReferrer.begin(), ie = q.m_setUserNeedSetReferrer.end();
			for (; it != ie; ++it)
			{
				Marshal::OctetsStream key;
				key << *it;
				if (puser->find(key, value, txn))
				{
					User user;
					Marshal::OctetsStream(value) >> user;
					if (user.referrer!=0 && g_mapRoleid.find(user.referrer)!=g_mapRoleid.end())
					{
						int oldreferrer = user.referrer;
						user.referrer = g_mapRoleid[oldreferrer].first;
						if (oldreferrer != user.referrer)
						{
							puser->insert(key, Marshal::OctetsStream()<<user, txn);
							Log::log(LOG_INFO, "Referrer of user %d is changed from %d to %d\n", *it, oldreferrer, user.referrer);
						}
						/*
						else
							Log::log(LOG_INFO, "User %d is assigned a referrer %d\n", *it, user.referrer);
						*/
					}
				}
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeUser, error when modify referrer, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
	q.m_setUserNeedSetReferrer.clear();
}

void MergeDropCounter(TableWrapper & tab_dropcounter)
{
	LOG_TRACE( "Merge dropcounter:" );

	MergeDropCounterQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_dropcounter.name.c_str(), tab_dropcounter.ptable, tab_dropcounter.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeDropCounter, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void MergeKingdom(TableWrapper & tab_kingdom)
{
	LOG_TRACE( "Merge kingdom:" );
	Marshal::OctetsStream key, value;
	key << 0;
	if (!tab_kingdom.Find(key, value))
	{
		Log::log(LOG_ERR, "src_kingdom can not be found");
		return;
	}
	GKingdomInfo src_kingdom;
	try
	{
		value >> src_kingdom;
	}
	catch(...)
	{
		Log::log(LOG_ERR, "src_kingdom unmarshal error");
		return;
	}
	LOG_TRACE("src_kingdom points %d win_times %d occupy_time %d task_status %d task_type %d issue_time %d task_points %d", src_kingdom.points, src_kingdom.win_times, src_kingdom.occupy_time, src_kingdom.taskstatus, src_kingdom.tasktype, src_kingdom.taskissuetime, src_kingdom.taskpoints);
	src_kingdom.king.roleid = g_mapRoleid[src_kingdom.king.roleid].first;
	src_kingdom.king.rolename = g_mapRolename.Find(src_kingdom.king.rolename);
	src_kingdom.queen.roleid = g_mapRoleid[src_kingdom.queen.roleid].first;
	src_kingdom.queen.rolename = g_mapRolename.Find(src_kingdom.queen.rolename);
	std::vector<GKingdomFunctionary>::iterator it, ite = src_kingdom.functionaries.end();
	for (it = src_kingdom.functionaries.begin(); it != ite; ++it)
	{
		it->roleid = g_mapRoleid[it->roleid].first;
		it->rolename = g_mapRolename.Find(it->rolename);
	}
	src_kingdom.kingfaction = g_mapFactionid[src_kingdom.kingfaction];
	src_kingdom.kingfacname = g_mapFactionname.Find(src_kingdom.kingfacname);
	std::vector<GKingdomHistory>::iterator hit, hite = src_kingdom.history.end();
	for (hit = src_kingdom.history.begin(); hit != hite; ++hit)
		hit->faction = g_mapFactionid[hit->faction];
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("kingdom");
		StorageEnv::AtomTransaction txn;
		{
			bool use_src = false;
			GKingdomInfo dst_kingdom;
			try
			{
				Marshal::OctetsStream dst_os;
				if (!pstorage->find(key, dst_os, txn))
				{
					Log::log(LOG_ERR, "dst_kingdom can not be found");
					use_src = true;
				}
				else
				{
					dst_os >> dst_kingdom;
					use_src = src_kingdom.points > dst_kingdom.points ||
						(src_kingdom.points == dst_kingdom.points &&
						 (src_kingdom.win_times > dst_kingdom.win_times ||
						  (src_kingdom.win_times == dst_kingdom.win_times  &&
						   (src_kingdom.occupy_time < dst_kingdom.occupy_time))));
				}
				LOG_TRACE("use_src=%d dst_kingdom points %d win_times %d occupy_time %d task_status %d task_type %d issue_time %d task_points %d", use_src, dst_kingdom.points, dst_kingdom.win_times, dst_kingdom.occupy_time, dst_kingdom.taskstatus, dst_kingdom.tasktype, dst_kingdom.taskissuetime, dst_kingdom.taskpoints);
				if (use_src)
				{
					src_kingdom.history.insert(src_kingdom.history.end(), dst_kingdom.history.begin(), dst_kingdom.history.end());
					if (dst_kingdom.taskstatus != KINGDOM_TASK_NONE)
						dst_kingdom.points += (dst_kingdom.taskpoints>KINGDOM_TASK_POINTS_LIMIT?KINGDOM_TASK_POINTS_LIMIT:dst_kingdom.taskpoints);
					src_kingdom.history.push_back(GKingdomHistory(dst_kingdom.kingfaction, dst_kingdom.points, dst_kingdom.win_times));
				}
				else
				{
					dst_kingdom.history.insert(dst_kingdom.history.end(), src_kingdom.history.begin(), src_kingdom.history.end());
					if (src_kingdom.taskstatus != KINGDOM_TASK_NONE)
						src_kingdom.points += (src_kingdom.taskpoints>KINGDOM_TASK_POINTS_LIMIT?KINGDOM_TASK_POINTS_LIMIT:src_kingdom.taskpoints);
					dst_kingdom.history.push_back(GKingdomHistory(src_kingdom.kingfaction, src_kingdom.points, src_kingdom.win_times));
				}
			}
			catch(...)
			{
				Log::log(LOG_ERR, "dst_kingdom unmarshal error");
				use_src = true;
			}
			if (use_src)
				pstorage->insert(key, Marshal::OctetsStream()<<src_kingdom, txn );
			else
				pstorage->insert(key, Marshal::OctetsStream()<<dst_kingdom, txn );
		}
	}
	catch ( ... )
	{
		Log::log(LOG_ERR, "unmarshal dst_kingdom error");
	}
	StorageEnv::checkpoint();
}

void MergeConfig(TableWrapper & tab_config)
{
	LOG_TRACE( "Merge config:" );
	Marshal::OctetsStream key, value;
	key << CONFIG_KEY_TOUCH_ORDER;
	if (tab_config.Find(key, value))
	{
		int64_t src_touch_orderid = 0;
		int64_t dst_touch_orderid = 0;
		try
		{
			value >> src_touch_orderid;
		}
		catch(...)
		{
			Log::log(LOG_ERR, "src_touch_orderid unmarshal error");
		}
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("config");
			StorageEnv::AtomTransaction txn;
			{
				Marshal::OctetsStream dst_value;
				if (pstorage->find(key, dst_value, txn))
				{
					try
					{
						dst_value >> dst_touch_orderid;
					}
					catch(...)
					{
						Log::log(LOG_ERR, "dst_touch_orderid unmarshal error");
					}
				}
				LOG_TRACE("src_touch_orderid %lld dst_touch_orderid %lld", src_touch_orderid, dst_touch_orderid);
				if (src_touch_orderid > dst_touch_orderid)
					pstorage->insert(key, Marshal::OctetsStream()<<src_touch_orderid, txn);
			}
		}
		catch ( ... )
		{
			Log::log(LOG_ERR, "merge touch orderid error");
		}
	}
	StorageEnv::checkpoint();
}

class ClearCircleHisQuery : public StorageEnv::IQuery
{
	int roleid;
	std::vector<int>& setRole;
public:
	ClearCircleHisQuery(std::vector<int> &set_Role):roleid(0),setRole(set_Role){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);

			GRoleBase role;
			os_value >> role;
			roleid=role.id;
			
			if(role.circletrack.size()!=0)
			{
				setRole.push_back(roleid);	
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: ClearCircleHisQuery error, roleid=%d",roleid );
		}
		return true;
	}
};

void ClearDstCircleHis( )
{
	LOG_TRACE( "ClearDstCircleHis:" );

	try
	{
		std::vector<int> setRole;
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			ClearCircleHisQuery q(setRole);
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		LOG_TRACE( "ClearDstCircleHis: process %d roles", setRole.size());
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			StorageEnv::CommonTransaction     txn;
			try
			{
				std::vector<int>::iterator it, ie = setRole.end();

				for (it = setRole.begin(); it != ie; ++it)
				{
					Marshal::OctetsStream	os_key;
					os_key<<*it;
					Marshal::OctetsStream os_value(pstorage->find(os_key,txn));
					GRoleBase role;
					os_value >> role;
			
				//	if(role.circletrack.size()!=0)
				//	{
						role.circletrack.clear();
						pstorage->insert( os_key, Marshal::OctetsStream()<<role, txn );
				//	}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ClearDstCircleHis, error when walk for ClearCircleHisQuery, what=%s\n", e.what() );
	}
}

class MergeRolenameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("rolename");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_value2 >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;

			Octets	oldname = key;
			Octets	newname = oldname;

			if( pstorage->find( key, os_value1, txn ) )
			{
				dup_rolename ++;
				int i = 1;
				while( true )
				{
					Octets testname = oldname;
					testname.insert( testname.begin(), g_octRoleNamePrefix.begin(), g_octRoleNamePrefix.size() );
					testname.insert( testname.end(), g_octRoleNameSuffix.begin(), g_octRoleNameSuffix.size() );
					if( i > 1 )
					{
						char buffer[64];
						sprintf( buffer, "%d", i );
						Octets oct_count_local(buffer,strlen(buffer)), oct_count;
						CharsetConverter::conv_charset_l2u( oct_count_local, oct_count );
						testname.insert( testname.end(), oct_count.begin(), oct_count.size() );
					}

					Octets value_tmp;
					if( !pstorage->find( testname, value_tmp, txn ) )
					{
						newname = testname;
						break;
					}
					i ++;
				}
				Log::log(LOG_INFO, "MergeRolename add prefix/suffix automatically oldroleid %d newroleid %d i=%d", oldroleid, newroleid, i);
			}
			g_mapRolename.Insert(oldname, newname);
			pstorage->insert( newname, Marshal::OctetsStream()<<newroleid, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: rolename date error, roleid=%d",oldroleid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeRolename(TableWrapper & tab_rolename)
{
	LOG_TRACE( "Merge rolename:" );

	PreLoadTable((StorageEnv::get_datadir()+"/rolename").c_str());
	PreLoadTable(tab_rolename.name.c_str());
	MergeRolenameQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_rolename.name.c_str(), tab_rolename.ptable, tab_rolename.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeRolename, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeRolenamehisQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("rolenamehis");
			Marshal::OctetsStream	os_key(key), os_value(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if (oldroleid < 128 || newroleid == 0)
				return true;
			GRoleNameHisVector rolenamehis_vec;
			os_value >> rolenamehis_vec;
			GRoleNameHisVector::iterator rit, rite = rolenamehis_vec.end();
			for (rit = rolenamehis_vec.begin(); rit != rite; ++rit)
				rit->name = g_mapRolename.Find(rit->name);
			pstorage->insert(Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<rolenamehis_vec, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: rolenamehis data error, roleid=%d",oldroleid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeRolenamehis(TableWrapper & tab_rolenamehis)
{
	LOG_TRACE( "Merge rolenamehis:" );

	PreLoadTable((StorageEnv::get_datadir()+"/rolenamehis").c_str());
	PreLoadTable(tab_rolenamehis.name.c_str());
	MergeRolenamehisQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_rolenamehis.name.c_str(), tab_rolenamehis.ptable, tab_rolenamehis.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeRolenamehis, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class FindMaxQuery : public StorageEnv::IQuery
{
public:
	int maxid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);

			int id = -1;
			os_key >> id;
			if( id > maxid )	maxid	=	id;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "FindMaxQuery, exception\n" );
		}
		return true;
	}
};

class MergeFamilyQuery : public StorageEnv::IQuery
{
public:
	int maxid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid = -1, newfid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("family");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			GFamily	info1, info2;
			os_key >> fid;
			os_value2 >> info2;
			if( pstorage->find( key, os_value1, txn ) )
			{	newfid = ++maxid;	dup_familyid ++;	}
			else
				newfid = fid;

			g_mapFamilyid[fid] = newfid;
			info2.id = newfid;
			info2.master = g_mapRoleid[info2.master].first;
			for( GFolkVector::iterator it = info2.member.begin(), ite = info2.member.end(); it != ite; ++it )
			{
				it->rid = g_mapRoleid[it->rid].first;
				it->name = g_mapRolename.Find(it->name);
			}

			pstorage->insert( Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info2, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: family date error, familyid=%d",fid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeFamily(TableWrapper & tab_family)
{
	LOG_TRACE( "Merge family:" );

	FindMaxQuery fmq;
	fmq.maxid = 0;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "family" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
			cursor.walk( fmq );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFamily, error when walk for FindMaxQuery, what=%s\n", e.what() );
	}

	MergeFamilyQuery q;
	q.maxid = fmq.maxid;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_family.name.c_str(), tab_family.ptable, tab_family.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFamily, error when walk for MergeFamilyQuery, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeCrssvrTeamsQuery : public StorageEnv::IQuery
{
public:
	int maxid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid = -1, newfid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("crssvrteams");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			GCrssvrTeamsData info1, info2;
			os_key >> fid;
			if(fid <= 0) return true;
			os_value2 >> info2;
			if( pstorage->find( key, os_value1, txn ) )
			{	newfid = ++maxid;	dup_crssvrteamsid ++;	}
			else
				newfid = fid;

			
			//printf("MergeCrssvrTeamsQuery oldfid=%d, newfid=%d\n", fid, newfid);
			g_mapCrssvrTeamsid[fid] = newfid;
			info2.info.teamid = newfid;
			info2.info.master = g_mapRoleid[info2.info.master].first;
			for( std::vector<GCrssvrTeamsRole>::iterator it = info2.roles.begin(), ite = info2.roles.end(); it != ite; ++it )
			{
				it->roleid = g_mapRoleid[it->roleid].first;
				it->name = g_mapRolename.Find(it->name);
			}

			pstorage->insert( Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info2, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: CrssvrTeams date error, ct_id=%d",fid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

class MergeCrssvrTeamsSeasonTopQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		LOG_TRACE("MergeCrssvrTeamsSeasonTopQuery");
		try
		{
			StorageEnv::Storage * pcrssvrteams = StorageEnv::GetStorage("crssvrteams");
			StorageEnv::AtomTransaction	txn;
			try
			{
				Marshal::OctetsStream	os_key(key), os_value1(value);

				int teamid = 0;
				os_key >> teamid;

				if(teamid == 0)
				{
					LOG_TRACE( "MergeCrssvrTeamsSeasonTopQuery teamid=%d, g_mapRoleid.size=%d", 0, g_mapRoleid.size() );
					/*for( MapRoleId::iterator it=g_mapRoleid.begin(), ite=g_mapRoleid.end(); it != ite; ++it )
					{
						LOG_TRACE( "%d, <%d, %d>", it->first, it->second.first, it->second.second );
					}*/

					GCrssvrTeamsSeasonTop dst_seasontoptable;
					
					os_value1 >> dst_seasontoptable;

					std::map<int, int> new_pos_map;
					std::map<int, int>& pos_map = dst_seasontoptable.pos_map;
					std::map<int, int>::iterator it, ie = pos_map.end();
					for(it=pos_map.begin(); it!=ie; ++it)
					{
						int oldroleid = it->first;
						int newroleid = g_mapRoleid[oldroleid].first;					
						new_pos_map.insert(std::make_pair(newroleid, it->second));
						LOG_TRACE("MergeCrssvrTeamsSeasonTopQuery oldroleid=%d, newroleid=%d, pos=%d", oldroleid, newroleid, it->second);
					}
					LOG_TRACE("MergeCrssvrTeamsSeasonTopQuery pos_map.size=%d", dst_seasontoptable.pos_map.size());
					dst_seasontoptable.pos_map.clear();
					dst_seasontoptable.pos_map = new_pos_map;
					pcrssvrteams->insert( Marshal::OctetsStream()<<teamid, Marshal::OctetsStream()<<dst_seasontoptable, txn );

					return false;
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException e( DB_OLD_VERSION );
				txn.abort( e );
				throw e;
			}

		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeCrssvrTeamsSeasonTopQuery, error when walk, what=%s\n", e.what() );
		}
		return true;
	}
};

void UpdateCrssvrTeamsSeasonTop(TableWrapper & tab_crssvrteams)
{
	LOG_TRACE( "UpdateCrssvrTeamsSeasonTop:" );
	MergeCrssvrTeamsSeasonTopQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_crssvrteams.name.c_str(), tab_crssvrteams.ptable, tab_crssvrteams.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "UpdateCrssvrTeamsSeasonTop, error when walk for MergeCrssvrTeamsSeasonTopQuery, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();

}

void MergeCrssvrTeams(TableWrapper & tab_crssvrteams)
{
	LOG_TRACE( "Merge crssvrteams:" );

	FindMaxQuery fmq;
	fmq.maxid = 0;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "crssvrteams" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
			cursor.walk( fmq );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeCrssvrTeams, error when walk for FindMaxQuery, what=%s\n", e.what() );
	}

	MergeCrssvrTeamsQuery q;
	q.maxid = fmq.maxid;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_crssvrteams.name.c_str(), tab_crssvrteams.ptable, tab_crssvrteams.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeCrssvrTeams, error when walk for MergeCrssvrTeamsQuery, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeFactionQuery : public StorageEnv::IQuery
{
public:
	int maxid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid = -1, newfid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("faction");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			GFactionInfo	info1, info2;
			os_key >> fid;
			os_value2 >> info2;
			if( pstorage->find( key, os_value1, txn ) )
			{	newfid = ++maxid;	dup_factionid ++;	}
			else
				newfid = fid;

			g_mapFactionid[fid] = newfid;
			info2.fid = newfid;
			info2.master = g_mapRoleid[info2.master].first;

			pstorage->insert( Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info2, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: faction date error, factionid=%d",fid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeFaction(TableWrapper & tab_faction)
{
	LOG_TRACE( "Merge faction:" );

	FindMaxQuery fmq;
	fmq.maxid = 0;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "faction" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
			cursor.walk( fmq );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFaction, error when walk for FindMaxQuery, what=%s\n", e.what() );
	}

	MergeFactionQuery q;
	q.maxid = fmq.maxid;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_faction.name.c_str(), tab_faction.ptable, tab_faction.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFaction, error when walk for MergeFactionQuery, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeFactionnameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int newfactionid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("factionname");
			StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_value2 >> newfactionid;
			newfactionid = g_mapFactionid[newfactionid];

			Octets	oldname;
			os_key >> oldname;
			Octets	newname = oldname;

			if( pstorage->find( key, os_value1, txn ) )
			{
				dup_factionname ++;
				int i = 1;
				while( true )
				{
					Octets testname = oldname;
					testname.insert( testname.begin(), g_octFactionNamePrefix.begin(), g_octFactionNamePrefix.size() );
					testname.insert( testname.end(), g_octFactionNameSuffix.begin(), g_octFactionNameSuffix.size() );
					if( i > 1 )
					{
						char buffer[64];
						sprintf( buffer, "%d", i );
						Octets oct_count_local(buffer,strlen(buffer)), oct_count;
						CharsetConverter::conv_charset_l2u( oct_count_local, oct_count );
						testname.insert( testname.end(), oct_count.begin(), oct_count.size() );
					}

					Octets value_tmp;
					if( !pstorage->find( Marshal::OctetsStream()<<testname, value_tmp, txn ) )
					{
						newname = testname;
						break;
					}
					i ++;
				}
			}

			Marshal::OctetsStream	os_faction;
			if( pfaction->find( Marshal::OctetsStream()<<newfactionid, os_faction, txn ) )
			{
				GFactionInfo	info;
				os_faction >> info;
				if (info.name == oldname)	//此条记录处理的可能是帮派的一个曾用名
				{
					info.name	=	newname;
					for( FamilyIdVector::iterator it = info.member.begin(), ite = info.member.end(); it != ite; ++it )
						it->fid = g_mapFamilyid[it->fid];
					info.hostileinfo.clear();
					pfaction->insert( Marshal::OctetsStream()<<newfactionid, Marshal::OctetsStream()<<info, txn );
				}
			}

			g_mapFactionname.Insert(oldname, newname);
			pstorage->insert( Marshal::OctetsStream()<<newname, Marshal::OctetsStream()<<newfactionid, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: factionname error, fid=%d", newfactionid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeFactionname(TableWrapper & tab_factionname)
{
	LOG_TRACE( "Merge factionname:" );
	MergeFactionnameQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_factionname.name.c_str(), tab_factionname.ptable, tab_factionname.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFactionname, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeFamilynameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int newfamilyid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("familyname");
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_value2 >> newfamilyid;
			newfamilyid = g_mapFamilyid[newfamilyid];

			Octets	oldname;
			os_key >> oldname;
			Octets	newname = oldname;

			if( pstorage->find( key, os_value1, txn ) )
			{
				dup_familyname ++;
				int i = 1;
				while( true )
				{
					Octets testname = oldname;
					testname.insert( testname.begin(), g_octFamilyNamePrefix.begin(), g_octFamilyNamePrefix.size() );
					testname.insert( testname.end(), g_octFamilyNameSuffix.begin(), g_octFamilyNameSuffix.size() );
					if( i > 1 )
					{
						char buffer[64];
						sprintf( buffer, "%d", i );
						Octets oct_count_local(buffer,strlen(buffer)), oct_count;
						CharsetConverter::conv_charset_l2u( oct_count_local, oct_count );
						testname.insert( testname.end(), oct_count.begin(), oct_count.size() );
					}

					Octets value_tmp;
					if( !pstorage->find( Marshal::OctetsStream()<<testname, value_tmp, txn ) )
					{
						newname = testname;
						break;
					}
					i ++;
				}
			}

			Marshal::OctetsStream	os_family;
			if( pfamily->find( Marshal::OctetsStream()<<newfamilyid, os_family, txn ) )
			{
				GFamily	info;
				os_family >> info;
				if (info.name == oldname)
				{
					info.name	=	newname;
					info.factionid	=	g_mapFactionid[info.factionid];
					pfamily->insert( Marshal::OctetsStream()<<newfamilyid, Marshal::OctetsStream()<<info, txn );
				}
			}

			g_mapFamilyname.Insert(oldname, newname);
			pstorage->insert( Marshal::OctetsStream()<<newname, Marshal::OctetsStream()<<newfamilyid, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: factionname data error, fid=%d", newfamilyid);
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeFamilyname(TableWrapper & tab_familyname)
{
	LOG_TRACE( "Merge familyname:" );
	MergeFamilynameQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_familyname.name.c_str(), tab_familyname.ptable, tab_familyname.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFamilyname, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeCrssvrTeamsnameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int newcrssvrteamsid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("crssvrteamsname");
			StorageEnv::Storage * pcrssvrteams = StorageEnv::GetStorage("crssvrteams");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_value2 >> newcrssvrteamsid;
			newcrssvrteamsid = g_mapCrssvrTeamsid[newcrssvrteamsid];

			Octets	oldname;
			os_key >> oldname;
			Octets	newname = oldname;

			if( pstorage->find( key, os_value1, txn ) )
			{
				dup_crssvrteamsname ++;
				int i = 1;
				while( true )
				{
					Octets testname = oldname;
					testname.insert( testname.begin(), g_octCrssvrTeamsNamePrefix.begin(), g_octCrssvrTeamsNamePrefix.size() );
					testname.insert( testname.end(), g_octCrssvrTeamsNameSuffix.begin(), g_octCrssvrTeamsNameSuffix.size() );
					if( i > 1 )
					{
						char buffer[64];
						sprintf( buffer, "%d", i );
						Octets oct_count_local(buffer,strlen(buffer)), oct_count;
						CharsetConverter::conv_charset_l2u( oct_count_local, oct_count );
						testname.insert( testname.end(), oct_count.begin(), oct_count.size() );
					}

					Octets value_tmp;
					if( !pstorage->find( Marshal::OctetsStream()<<testname, value_tmp, txn ) )
					{
						newname = testname;
						break;
					}
					i ++;
				}
			}

			Marshal::OctetsStream	os_crssvrteams;
			if( pcrssvrteams->find( Marshal::OctetsStream()<<newcrssvrteamsid, os_crssvrteams, txn ) )
			{
				GCrssvrTeamsData	info;
				os_crssvrteams >> info;
				if (info.info.name == oldname)
				{
					info.info.name	=	newname;
					pcrssvrteams->insert( Marshal::OctetsStream()<<newcrssvrteamsid, Marshal::OctetsStream()<<info, txn );
				}
			}

			g_mapCrssvrTeamsname.Insert(oldname, newname);
			pstorage->insert( Marshal::OctetsStream()<<newname, Marshal::OctetsStream()<<newcrssvrteamsid, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: crssvrteamsname data error, fid=%d", newcrssvrteamsid);
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeCrssvrTeamsname(TableWrapper & tab_crssvrteamsname)
{
	LOG_TRACE( "Merge crssvrteamsname:" );
	MergeCrssvrTeamsnameQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_crssvrteamsname.name.c_str(), tab_crssvrteamsname.ptable, tab_crssvrteamsname.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeCrssvrTeamsname, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeBaseQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("base");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if( oldroleid && oldroleid < 128 )
				return true;
			GRoleBase base;
			os_value2 >> base;
			if( 0 == newroleid )
			{
				Log::log(LOG_INFO, "\tERROR:role missed in User.rolelist or logicuid not available,skip roleid=%d(userid=%d).",
						oldroleid, base.userid);
				return true;
			}
			if (base.status == _ROLE_STATUS_FROZEN)
				Log::log(LOG_ERR, "MergeBaseQuery role data is frozen oldroleid %d newroleid %d!", oldroleid, newroleid);
			server2_rolecount ++;
			base.id			=	newroleid;
			if (base.userid == 0)
			{
				base.userid = g_mapRoleid[oldroleid].second;
			}
			base.spouse		=	g_mapRoleid[base.spouse].first;
			base.sectid	=	g_mapRoleid[base.sectid].first;
			base.familyid	=	g_mapFamilyid[base.familyid];
			//LOG_TRACE("KEY_DATAGOURP_ROLE_CT_TEAMID src=%d, new=%d", base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID], g_mapCrssvrTeamsid[base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID]]);
			base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID] = g_mapCrssvrTeamsid[base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID]];
			
			//clear circle history
			base.circletrack.clear();
			/*
			if( g_mapRolename[base.name].size() > 0 )
				base.name	=	g_mapRolename[base.name];
			*/
			base.name = g_mapRolename.Find(base.name);
			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeBaseQuery Error, overwrite base. oldroleid = %d, newroleid = %d.",oldroleid,newroleid );
			pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<base, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: base data error, roleid = %d", oldroleid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeBase(TableWrapper & tab_base)
{
	LOG_TRACE( "Merge base:" );
	PreLoadTable((StorageEnv::get_datadir()+"/base").c_str());
	PreLoadTable(tab_base.name.c_str());
	MergeBaseQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_base.name.c_str(), tab_base.ptable, tab_base.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeBase, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeStatusQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("status");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if( oldroleid < 128 || 0 == newroleid )
				return true;

			GRoleStatus status;
			os_value2 >> status;
			status.id = newroleid;
			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeStatusQuery Error,oldroleid = %d, newroleid = %d.",
						oldroleid, newroleid );

			pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<status, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: status data error, roleid = %d", oldroleid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeStatus(TableWrapper & tab_status)
{
	LOG_TRACE( "Merge Status:");

	PreLoadTable((StorageEnv::get_datadir()+"/status").c_str());
	PreLoadTable(tab_status.name.c_str());
	MergeStatusQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_status.name.c_str(), tab_status.ptable, tab_status.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeStatus, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeRoleDataQuery : public StorageEnv::IQuery
{
public:
	std::string	tablename;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( tablename.c_str() );
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if( oldroleid < 128 || 0 == newroleid )
				return true;

			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeRoleDataQuery Error, overwrite %s. oldroleid = %d, newroleid = %d.",
						tablename.c_str(), oldroleid, newroleid );

			if (tablename == "inventory")
			{
				GRolePocket pocket;
				Marshal::OctetsStream(value) >> pocket;
				bool modified = false;
				GRoleInventoryVector::iterator cit = pocket.items.begin();
				for (; cit != pocket.items.end(); ++ cit)
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				cit = pocket.equipment.begin();
				for (; cit != pocket.equipment.end(); ++ cit)
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				cit = pocket.petbadge.begin();
				for (; cit != pocket.petbadge.end(); ++ cit)
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				cit = pocket.petequip.begin();
				for (; cit != pocket.petequip.end(); ++ cit)
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				cit = pocket.fashion.begin();
				for (; cit != pocket.fashion.end(); ++ cit)
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				if (pocket.mountwing.size())
				{
					Marshal::OctetsStream os_mountwing(pocket.mountwing);
					GRoleInventoryVector mountwing;
					short mountwingsize = 0;
					os_mountwing >> mountwingsize;
					os_mountwing >> mountwing;
					cit = mountwing.begin();
					for (; cit != mountwing.end(); ++ cit)
					{
						if (modified)
							CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
						else
							modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					}
					os_mountwing.clear();
					os_mountwing << mountwingsize;
					os_mountwing << mountwing;
					pocket.mountwing = os_mountwing;
				}

				cit = pocket.gifts.begin();
				for (; cit != pocket.gifts.end(); ++ cit)
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				if (modified)
				{
					value.clear();
					value.swap(Marshal::OctetsStream()<<pocket);
				}
			}
			else if (tablename == "storehouse")
			{
				GRoleStorehouse storehouse;
				Marshal::OctetsStream(value) >> storehouse;
				bool modified = false;
				GRoleInventoryVector::iterator cit = storehouse.items.begin();
				for (; cit != storehouse.items.end(); ++ cit) 
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				cit = storehouse.items2.begin();
				for (; cit != storehouse.items2.end(); ++ cit) 
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				cit = storehouse.fuwen.begin();
				for (; cit != storehouse.fuwen.end(); ++ cit) 
				{
					if (modified)
						CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
					else
						modified = CheckItemOwner(g_mapRoleid, tablename, *cit, oldroleid);
				}

				if (modified)
				{
					value.clear();
					value.swap(Marshal::OctetsStream()<<storehouse);
				}
			}

			pstorage->insert( Marshal::OctetsStream()<<newroleid, value, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: role data error, roleid=%d", oldroleid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeRoleData(TableWrapper & tab_role, const char * tablename )
{
	LOG_TRACE( "Merge %s:", tablename );

	PreLoadTable((StorageEnv::get_datadir()+"/"+tablename).c_str());
	PreLoadTable(tab_role.name.c_str());
	MergeRoleDataQuery q;
	q.tablename = tablename;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_role.name.c_str(), tab_role.ptable, tab_role.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeRoleData, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeWaitdelQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldid = -1, newid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("waitdel");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			WaitDelKey	wdk;
			os_key >> wdk;
			oldid	=	wdk.id;
			switch (wdk.type)
			{
				case TYPE_ROLE:
					newid = g_mapRoleid[oldid].first;
					if (oldid < 128 || 0 == newid)
						return true;
					break;
				case TYPE_FAMILY:
					newid = g_mapFamilyid[oldid];
					break;
				case TYPE_FACTION:
					newid = g_mapFactionid[oldid];
					break;
			}	
			wdk.id		=	newid;

			if( pstorage->find( Marshal::OctetsStream()<<wdk, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeWaitdelQuery Error, overwrite waitdel. oldid = %d, newid = %d, type %d.",
						oldid, newid, wdk.type );

			pstorage->insert( Marshal::OctetsStream()<<wdk, value, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: waitdel data error, id=%d", oldid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeWaitdel(TableWrapper & tab_waitdel)
{
	LOG_TRACE( "Merge waitdel:" );
	MergeWaitdelQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_waitdel.name.c_str(), tab_waitdel.ptable, tab_waitdel.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeWaitdel, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeMailboxQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("mailbox");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			//[newroleid == 0] means can not find a host in base for this box, so discard it!
			if( oldroleid < 128 || 0 == newroleid )
				return true;

			GMailBox	box;
			os_value2 >> box;
			for( GMailVector::iterator it = box.mails.begin(), ite = box.mails.end(); it != ite; ++it )
			{
				if(it->header.sndr_type != _MST_LOCAL_CONSIGN)//skip sender change for consign mail
				{
					it->header.sender = g_mapRoleid[it->header.sender].first;
					it->header.sender_name = g_mapRolename.Find(it->header.sender_name);
				}

				//修改邮件中物品可能的主人roleid
				if(it->header.attribute & (1 << _MA_ATTACH_OBJ))
					CheckItemOwner(g_mapRoleid, "mailbox", it->attach_obj, oldroleid);
			}
			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeMailboxQuery Error, overwrite mailbox. oldroleid = %d, newroleid = %d.",
						oldroleid, newroleid );

			pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<box, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: mail data error, roleid=%d", oldroleid );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeMailbox(TableWrapper & tab_mail)
{
	LOG_TRACE( "Merge mailbox:" );
	MergeMailboxQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_mail.name.c_str(), tab_mail.ptable, tab_mail.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeMailbox, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeMessagesQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("messages");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if( oldroleid < 128 || 0 == newroleid )
				return true;

			MessageVector	msgs;
			os_value2 >> msgs;
			for( MessageVector::iterator it = msgs.begin(), ite = msgs.end(); it != ite; ++it )
			{
				it->srcroleid = g_mapRoleid[it->srcroleid].first;
				it->dstroleid = g_mapRoleid[it->dstroleid].first;
				it->src_name  = g_mapRolename.Find(it->src_name);
				it->dst_name  = g_mapRolename.Find(it->dst_name);
			}
			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeMessagesQuery Error, overwrite messages. oldroleid = %d, newroleid = %d.",
						oldroleid, newroleid );

			pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<msgs, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: message data error, roleid=%d", oldroleid);
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeMessages(TableWrapper & tab_msg)
{
	LOG_TRACE( "Merge messages:" );
	MergeMessagesQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_msg.name.c_str(), tab_msg.ptable, tab_msg.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeMessages, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeFriendsQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("friends");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if( oldroleid < 128 || 0 == newroleid )
				return true;

			GFriendList	fl;
			os_value2 >> fl;
			for( GFriendInfoVector::iterator it = fl.friends.begin(), ite = fl.friends.end(); it != ite; ++it )
			{
				it->rid		=	g_mapRoleid[it->rid].first;
				it->name	=	g_mapRolename.Find(it->name);
			}
			for( GEnemyInfoVector::iterator it = fl.enemies.begin(), ite = fl.enemies.end(); it != ite; ++it )
			{
				it->rid		=	g_mapRoleid[it->rid].first;
				it->name	=	g_mapRolename.Find(it->name);
			}
			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
				Log::log(LOG_INFO, "WARN:MergeFriendsQuery Error, overwrite friends. oldroleid = %d, newroleid = %d.",
						oldroleid, newroleid );

			pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<fl, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: friend data error, roleid=%d", oldroleid);
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeFriends(TableWrapper & tab_friends)
{
	LOG_TRACE( "Merge friends:" );

	PreLoadTable((StorageEnv::get_datadir()+"/friends").c_str());
	PreLoadTable(tab_friends.name.c_str());
	MergeFriendsQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,tab_friends.name.c_str(), tab_friends.ptable, tab_friends.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFriends, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeSectQuery : public StorageEnv::IQuery
{
public:
	int sectsum;
	int skipped;
	int overwrite;
	MergeSectQuery():sectsum(0),skipped(0),overwrite(0) { } 
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("sect");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			sectsum++;
			os_key >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;
			if( oldroleid < 128 || 0 == newroleid )
			{
				skipped++;
				return true;
			}

			GSect	sect;
			os_value2 >> sect;
			sect.name = g_mapRolename.Find(sect.name);
			for( GDiscipleVector::iterator it = sect.disciples.begin(), ite = sect.disciples.end(); it != ite; ++it )
			{
				it->roleid	=	g_mapRoleid[it->roleid].first;
				it->name	=	g_mapRolename.Find(it->name);
			}
			if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_value1, txn ) )
			{
				Log::log(LOG_INFO, "\tWARN:MergeSectQuery Error, overwrite sect. oldroleid = %d, newroleid = %d.",
						oldroleid, newroleid );
				overwrite++;
			}

			pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<sect, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: sect data error, roleid=%d", oldroleid);
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeSect(TableWrapper & tab_sect)
{
	LOG_TRACE( "Merge sect:" );
	MergeSectQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,tab_sect.name.c_str(),tab_sect.ptable,tab_sect.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeSect, error when walk, what=%s\n", e.what() );
	}
	fprintf( stderr, "\tINFO:sect number in server2=%d,skipped=%d,overwrite=%d\n",q.sectsum,q.skipped,q.overwrite );
	StorageEnv::checkpoint();
}

class MergeShoplogQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("shoplog");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			std::vector<GShopLog>	vsl;
			os_value2 >> vsl;
			for( std::vector<GShopLog>::iterator it = vsl.begin(), ite = vsl.end(); it != ite; ++it )
				it->roleid	=	g_mapRoleid[it->roleid].first;

			int64_t	shoplogid = 0, shoplogidnew = 0;
			static int64_t shoplogid_inc = 0;
			os_key >> shoplogid;
			shoplogidnew = shoplogid + shoplogid_inc;

			while( true )
			{
				if( !pstorage->find( Marshal::OctetsStream()<<shoplogidnew, os_value1, txn ) )
				{
					pstorage->insert( Marshal::OctetsStream()<<shoplogidnew, Marshal::OctetsStream()<<vsl, txn );
					break;
				}
				shoplogid_inc	+= 0x0000000100000000LL;
				shoplogidnew	+= shoplogid_inc;
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: shoplog data error");
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeShoplog(TableWrapper & tab_shoplog)
{
	LOG_TRACE( "Merge shoplog:" );
	MergeShoplogQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,tab_shoplog.name.c_str(),tab_shoplog.ptable,tab_shoplog.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeShoplog, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeSyslogQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("syslog");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			GSysLog	log;
			os_value2 >> log;
			log.roleid	=	g_mapRoleid[log.roleid].first;

			int64_t	syslogid = 0, syslogidnew = 0;
			static int64_t syslogid_inc = 0;
			os_key >> syslogid;
			syslogidnew = syslogid + syslogid_inc;

			while( true )
			{
				if( !pstorage->find( Marshal::OctetsStream()<<syslogidnew, os_value1, txn ) )
				{
					pstorage->insert( Marshal::OctetsStream()<<syslogidnew, Marshal::OctetsStream()<<log, txn );
					break;
				}
				syslogid_inc	+= 0x0000000100000000LL;
				syslogidnew	+= syslogid_inc;
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: syslog data error");
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeSyslog(TableWrapper  & tab_syslog)
{
	LOG_TRACE( "Merge syslog:" );
	MergeSyslogQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,tab_syslog.name.c_str(), tab_syslog.ptable,tab_syslog.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeSyslog, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MergeFacBaseQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldfid = -1, newfid = -1;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage("factionbase");
			Marshal::OctetsStream	os_key(key), os_value1, os_value2(value);

			os_key >> oldfid;
			newfid = g_mapFactionid[oldfid];
			if( 0 == newfid )
			{
				Log::log(LOG_INFO, "\tERROR:fid missed, skip fid=%d", oldfid);
				return true;
			}
			GFactionBase base;
			os_value2 >> base;
			GFactionAuctionVector::iterator ait, aite = base.auctions.end();
			for (ait = base.auctions.begin(); ait != aite; ++ait)
			{
				ait->winner = g_mapRoleid[ait->winner].first;
				ait->rolename = g_mapRolename.Find(ait->rolename);
			}
			GFactionAuctionFailerVector::iterator fit, fite = base.auction_failers.end();
			for (fit = base.auction_failers.begin(); fit != fite; ++fit)
				fit->roleid = g_mapRoleid[fit->roleid].first;
			GFactionAuctionHistoryVector::iterator hit, hite = base.auction_history.end();
			for (hit = base.auction_history.begin(); hite != hite; ++hit)
				hit->rolename = g_mapRolename.Find(hit->rolename);

			if( pstorage->find( Marshal::OctetsStream()<<newfid, os_value1, txn ) )
				Log::log(LOG_INFO, "\tWARN:MergeFacBaseQuery Error, overwrite facbase. oldfid = %d, newfid = %d.", oldfid, newfid );
			pstorage->insert( Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<base, txn );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: facbase data error, fid = %d", oldfid );
		}
//		static size_t counter = 1;
//		Auto_Checkpoint(counter);
		return true;
	}
};

void MergeFacBase(TableWrapper & tab_facbase)
{
	LOG_TRACE( "Merge facbase:" );
	PreLoadTable((StorageEnv::get_datadir()+"/facbase").c_str());
	PreLoadTable(tab_facbase.name.c_str());
	MergeFacBaseQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_facbase.name.c_str(), tab_facbase.ptable, tab_facbase.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MergeFacBase, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void LoadConfig( )
{
	// load name prefix and suffix
	g_strRoleNamePrefix		=	Conf::GetInstance()->find( "gamedbd", "rolenameprefix" );
	g_strRoleNameSuffix		=	Conf::GetInstance()->find( "gamedbd", "rolenamesuffix" );
	g_strFactionNamePrefix	=	Conf::GetInstance()->find( "gamedbd", "factionnameprefix" );
	g_strFactionNameSuffix	=	Conf::GetInstance()->find( "gamedbd", "factionnamesuffix" );
	g_strFamilyNamePrefix	=	Conf::GetInstance()->find( "gamedbd", "familynameprefix" );
	g_strFamilyNameSuffix	=	Conf::GetInstance()->find( "gamedbd", "familynamesuffix" );
	g_strCrssvrTeamsNamePrefix	=	Conf::GetInstance()->find( "gamedbd", "crssvrteamsnameprefix" );
	g_strCrssvrTeamsNameSuffix	=	Conf::GetInstance()->find( "gamedbd", "crssvrteamsnamesuffix" );

	Octets      l_octRoleNamePrefix( g_strRoleNamePrefix.c_str(), g_strRoleNamePrefix.length() );
	Octets      l_octRoleNameSuffix( g_strRoleNameSuffix.c_str(), g_strRoleNameSuffix.length() );
	Octets      l_octFactionNamePrefix( g_strFactionNamePrefix.c_str(), g_strFactionNamePrefix.length() );
	Octets      l_octFactionNameSuffix( g_strFactionNameSuffix.c_str(), g_strFactionNameSuffix.length() );
	Octets      l_octFamilyNamePrefix( g_strFamilyNamePrefix.c_str(), g_strFamilyNamePrefix.length() );
	Octets      l_octFamilyNameSuffix( g_strFamilyNameSuffix.c_str(), g_strFamilyNameSuffix.length() );
	Octets      l_octCrssvrTeamsNamePrefix( g_strCrssvrTeamsNamePrefix.c_str(), g_strCrssvrTeamsNamePrefix.length() );
	Octets      l_octCrssvrTeamsNameSuffix( g_strCrssvrTeamsNameSuffix.c_str(), g_strCrssvrTeamsNameSuffix.length() );



	CharsetConverter::conv_charset_l2u( l_octRoleNamePrefix, g_octRoleNamePrefix );
	CharsetConverter::conv_charset_l2u( l_octRoleNameSuffix, g_octRoleNameSuffix );
	CharsetConverter::conv_charset_l2u( l_octFactionNamePrefix, g_octFactionNamePrefix );
	CharsetConverter::conv_charset_l2u( l_octFactionNameSuffix, g_octFactionNameSuffix );
	CharsetConverter::conv_charset_l2u( l_octFamilyNamePrefix, g_octFamilyNamePrefix );
	CharsetConverter::conv_charset_l2u( l_octFamilyNameSuffix, g_octFamilyNameSuffix );
	CharsetConverter::conv_charset_l2u( l_octCrssvrTeamsNamePrefix, g_octCrssvrTeamsNamePrefix );
	CharsetConverter::conv_charset_l2u( l_octCrssvrTeamsNameSuffix, g_octCrssvrTeamsNameSuffix );

	fprintf( stderr, "\nLoad Configuration:\n" );
	fprintf( stderr, "\tINFO:role name prefix(local):" );		l_octRoleNamePrefix.dump();
	fprintf( stderr, "\tINFO:role name suffix(local):" );		l_octRoleNameSuffix.dump();
	fprintf( stderr, "\tINFO:faction name prefix(local):" );	l_octFactionNamePrefix.dump();
	fprintf( stderr, "\tINFO:faction name suffix(local):" );	l_octFactionNameSuffix.dump();
	fprintf( stderr, "\tINFO:family name prefix(local):" );		l_octFamilyNamePrefix.dump();
	fprintf( stderr, "\tINFO:family name suffix(local):" );		l_octFamilyNameSuffix.dump();
	fprintf( stderr, "\tINFO:crssvrteams name prefix(local):" );		l_octCrssvrTeamsNamePrefix.dump();
	fprintf( stderr, "\tINFO:crssvrteams name suffix(local):" );		l_octCrssvrTeamsNameSuffix.dump();

	fprintf( stderr, "\tINFO:role name prefix(UTF-16LE):" );		g_octRoleNamePrefix.dump();
	fprintf( stderr, "\tINFO:role name suffix(UTF-16LE):" );		g_octRoleNameSuffix.dump();
	fprintf( stderr, "\tINFO:faction name prefix(UTF-16LE):");		g_octFactionNamePrefix.dump();
	fprintf( stderr, "\tINFO:faction name suffix(UTF-16LE):");		g_octFactionNameSuffix.dump();
	fprintf( stderr, "\tINFO:family name prefix(UTF-16LE):");		g_octFamilyNamePrefix.dump();
	fprintf( stderr, "\tINFO:family name suffix(UTF-16LE):");		g_octFamilyNameSuffix.dump();
	fprintf( stderr, "\tINFO:crssvrteams name prefix(UTF-16LE):");		g_octCrssvrTeamsNamePrefix.dump();
	fprintf( stderr, "\tINFO:crssvrteams name suffix(UTF-16LE):");		g_octCrssvrTeamsNameSuffix.dump();

}

void PrintMap( )
{
	{
		LOG_TRACE( "g_mapRoleid size=%d:", g_mapRoleid.size() );
		for( MapRoleId::iterator it=g_mapRoleid.begin(), ite=g_mapRoleid.end(); it != ite; ++it )
		{
			//if( it->first != it->second )
				LOG_TRACE( "%d, <%d, %d>", it->first, it->second.first, it->second.second );
		}
	}

	{
		LOG_TRACE( "g_mapFactionid size=%d:\n", g_mapFactionid.size() );
		for( std::map<int,int>::iterator it=g_mapFactionid.begin(), ite=g_mapFactionid.end(); it != ite; ++it )
		{
			//if( it->first != it->second )
				LOG_TRACE( "%d,%d\n", it->first, it->second );
		}
	}

	{
		LOG_TRACE( "g_mapFamilyid size=%d:\n", g_mapFamilyid.size() );
		for( std::map<int,int>::iterator it=g_mapFamilyid.begin(), ite=g_mapFamilyid.end(); it != ite; ++it )
		{
			//if( it->first != it->second )
				LOG_TRACE( "%d,%d\n", it->first, it->second );
		}
	}

	{
		LOG_TRACE( "g_mapCrssvrTeamsid size=%d:\n", g_mapCrssvrTeamsid.size() );
		for( std::map<int,int>::iterator it=g_mapCrssvrTeamsid.begin(), ite=g_mapCrssvrTeamsid.end(); it != ite; ++it )
		{
			//if( it->first != it->second )
				LOG_TRACE( "%d,%d\n", it->first, it->second );
		}
	}

	{
		LOG_TRACE( "g_mapRolename raw_size=%d: real_size=%d\n", g_mapRolename.RawSize(), g_mapRolename.RealSize() );
		g_mapRolename.Dump();
	}

	{
		LOG_TRACE( "g_mapFactionname raw_size=%d: real_size=%d\n", g_mapFactionname.RawSize(), g_mapFactionname.RealSize() );
		g_mapFactionname.Dump();
	}

	{
		LOG_TRACE( "g_mapFamilyname raw_size=%d: real_size=%d\n", g_mapFamilyname.RawSize(), g_mapFamilyname.RealSize() );
		g_mapFamilyname.Dump();
	}

	{
		LOG_TRACE( "g_mapCrssvrTeamsname raw_size=%d: real_size=%d\n", g_mapCrssvrTeamsname.RawSize(), g_mapCrssvrTeamsname.RealSize() );
		g_mapCrssvrTeamsname.Dump();
	}

}

class MergeCommonDataQuery : public StorageEnv::IQuery
{
	private:
		class ValueBinaryOp
		{
			public:
				virtual void operator() (const Value &op1, const Value &op2, Value &result) const =0;
		};

		class ValueAdd : public ValueBinaryOp
		{
			public:
				static const ValueAdd *GetInstance() { static ValueAdd instance; return &instance; }

				void operator() (const Value &op1, const Value &op2, Value &result) const
				{
					result = op1;
					result.Add(const_cast<Value &>(op2));
				}
		};

		class ValueMax : public ValueBinaryOp
		{
			public:
				static const ValueMax *GetInstance() { static ValueMax instance; return &instance; }

				void operator() (const Value &op1, const Value &op2, Value &result) const
				{
					if (const_cast<Value &>(op1).IsInRange(const_cast<Value &>(op1), const_cast<Value &>(op2)))
						result = op2;
					else
						result = op1;
				}
		};

		class ValueMin : public ValueBinaryOp
		{
			public:
				static const ValueMin *GetInstance() { static ValueMin instance; return &instance; }

				void operator() (const Value &op1, const Value &op2, Value &result) const
				{
					if (const_cast<Value &>(op1).IsInRange(const_cast<Value &>(op1), const_cast<Value &>(op2)))
						result = op1;
					else
						result = op2;
				}
		};

		class ValueZero : public ValueBinaryOp
		{
			public:
				static const ValueZero *GetInstance() { static ValueZero instance; return &instance; }

				void operator() (const Value &op1, const Value &op2, Value &result) const
				{
					result = MergeCommonDataQuery::GetZero(op1.GetType());
				}
		};

		static const Value &GetZero(Value::Type type)
		{
			return Value::GetStub(type);
		}

		static bool IsZero(const Value &value)
		{
			if (value.GetType() != Value::TYPE_INT)
				return true;
			else
				return const_cast<Value &>(value).ToInteger()==0;
			//return const_cast<Value &>(value) == GetZero(value.GetType());
		}

		static struct ValueBinaryOpRule
		{
			int key_left;
			int key_right;
			const ValueBinaryOp *bin_op;
		} rules[6];

	public:
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int64_t key64;
			try
			{
				StorageEnv::Storage * pstorage = StorageEnv::GetStorage("commondata");
				Marshal::OctetsStream(key) >> key64;

				int scole = (int)(key64 >> 32);
				int key_dat = (int)(key64 & 0xffffffff);

				if (scole != 0)
				{
					Log::log(LOG_ERR, "scole is not 0, scole=%d, key=%d, valuesize=%d\n", scole, key_dat, value.size());
				}
				else
				{
					const ValueBinaryOp *bin_op = FindOperation(key_dat);
					if (bin_op != NULL)
					{
						Value vres, vsrc;
						DataStream(value) >> vsrc;

						Value vdst;
						Octets value_dst;
						if (pstorage->find(key, value_dst, txn))
							DataStream(value_dst) >> vdst;
						else
							vdst = GetZero(vsrc.GetType());

						if (vsrc.GetType()!=Value::TYPE_INT || vdst.GetType()!=Value::TYPE_INT)
							Log::log(LOG_ERR, "Value type is not TYPE_INT, abandoned! scole=%d, key=%d\n", scole, key_dat);
						else
						{
							(*bin_op)(vsrc, vdst, vres);

							if (IsZero(vres))
								pstorage->del(key, txn);
							else
								pstorage->insert(key, (DataStream()<<vres).GetData(), txn);
						}
					}
					else
						Log::log(LOG_ERR, "Fail to process common value, abandoned! scole=%d, key=%d\n", scole, key_dat);
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log(LOG_INFO, "\tERROR: commondata error, key=%lld", key64);
			}
			static size_t counter = 1;
			Auto_Checkpoint(counter);
			return true;
		}

	private:
		const ValueBinaryOp *FindOperation(int key)
		{
			for (int i = 0; i < (int)(sizeof(rules)/sizeof(rules[0])); ++i)
			{
				if ((rules[i].key_left==-1 || key>=rules[i].key_left)
						&& (rules[i].key_right==-1 || key<=rules[i].key_right))
				{
					return rules[i].bin_op;
				}
			}
			return NULL;
		}
};

	MergeCommonDataQuery::ValueBinaryOpRule MergeCommonDataQuery::rules[6] = 
	{
		{1, 10000, ValueAdd::GetInstance()}, 
		{10001, 20000, ValueMax::GetInstance()}, 
		{20001, 30000, ValueMin::GetInstance()}, 
		{30001, 40000, ValueZero::GetInstance()}, 
		{40001, -1, ValueZero::GetInstance()},
		{-1, -1, NULL},
	};    

	void MergeCommonData(TableWrapper & tab_commondata)
	{
		LOG_TRACE( "Merge CommonData:" );
		MergeCommonDataQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn,tab_commondata.name.c_str(), tab_commondata.ptable, tab_commondata.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeCommonData, error when walk, what=%s\n", e.what() );
		}
	}

	class MergeSNSPlayerInfoQuery : public StorageEnv::IQuery
	{
		public:
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				GSNSPlayerInfoCache	infocache;
				try
				{
					StorageEnv::Storage * pstorage = StorageEnv::GetStorage("snsplayerinfo");
					Marshal::OctetsStream(value) >> infocache;

					GSNSRoleInfo &roleinfo = infocache.roleinfo;
					roleinfo.spouse = g_mapRoleid[roleinfo.spouse].first;
					roleinfo.rolename = g_mapRolename.Find(roleinfo.rolename);
					roleinfo.sectname = g_mapRolename.Find(roleinfo.sectname);
					roleinfo.familyname = g_mapFamilyname.Find(roleinfo.familyname);
					roleinfo.factionname = g_mapFactionname.Find(roleinfo.factionname);

					GSNSPlayerInfo &playerinfo = infocache.playerinfo;
					playerinfo.roleid = g_mapRoleid[playerinfo.roleid].first;
					playerinfo.rolename = g_mapRolename.Find(playerinfo.rolename);

					pstorage->insert(Marshal::OctetsStream()<<playerinfo.roleid, Marshal::OctetsStream()<<infocache, txn);
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					Log::log( LOG_ERR, "MergeSNSPlayerInfo, roleid=%d\n", infocache.playerinfo.roleid );
				}
				static size_t counter = 1;
				Auto_Checkpoint(counter);
				return true;
			}
	};

	void MergeSNSPlayerInfo(TableWrapper & tab_snsplayer)
	{
		LOG_TRACE( "Merge SNSPlayerInfo:" );
		MergeSNSPlayerInfoQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_snsplayer.name.c_str(), tab_snsplayer.ptable, tab_snsplayer.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeSNSPlayerInfo, error when walk, what=%s\n", e.what() );
		}
	}
	class MergeConsignQuery : public StorageEnv::IQuery
	{
		int64_t sn;
		public:
			MergeConsignQuery():sn(0){}
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				GConsignDB detail;
				try
				{
					StorageEnv::Storage * pstorage = StorageEnv::GetStorage("consign");
					
					Marshal::OctetsStream(key) >> sn;
					if(sn == 0)
						return true;

					Marshal::OctetsStream(value) >> detail;
				
					CheckItemOwner(g_mapRoleid, "consign", detail.item, detail.seller_roleid);
					detail.seller_roleid = g_mapRoleid[detail.seller_roleid].first;
					detail.seller_name = g_mapRolename.Find(detail.seller_name);
					detail.buyer_roleid = g_mapRoleid[detail.buyer_roleid].first;
					detail.buyer_name = g_mapRolename.Find(detail.buyer_name);

					pstorage->insert(Marshal::OctetsStream()<<sn, Marshal::OctetsStream()<<detail, txn,DB_NOOVERWRITE);
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					Log::log( LOG_ERR, "MergeConsignQuery, sn=%lld\n", sn);
				}
				static size_t counter = 1;
				Auto_Checkpoint(counter);
				return true;
			}
	};

	void MergeConsign(TableWrapper & tab_consign)
	{
		LOG_TRACE( "Merge Consign:" );
		MergeConsignQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_consign.name.c_str(), tab_consign.ptable, tab_consign.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeConsign, error when walk, what=%s\n", e.what() );
		}
	}
	class MergeFinishedConsignQuery : public StorageEnv::IQuery
	{
		int64_t sn;
		public:
			MergeFinishedConsignQuery():sn(0){}
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				GConsignDB detail;
				try
				{
					StorageEnv::Storage * pstorage = StorageEnv::GetStorage("finished_consign");
					Marshal::OctetsStream(key) >> sn;
					if(sn == 0)
						return true;
					Marshal::OctetsStream(value) >> detail;

					detail.seller_roleid = g_mapRoleid[detail.seller_roleid].first;
					detail.seller_name = g_mapRolename.Find(detail.seller_name);
					detail.buyer_roleid = g_mapRoleid[detail.buyer_roleid].first;
					detail.buyer_name = g_mapRolename.Find(detail.buyer_name);

					pstorage->insert(Marshal::OctetsStream()<<sn, Marshal::OctetsStream()<<detail, txn,DB_NOOVERWRITE);
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					Log::log( LOG_ERR, "MergeFinishedConsignQuery, sn=%lld\n", sn);
				}
				static size_t counter = 1;
				Auto_Checkpoint(counter);
				return true;
			}
	};

	void MergeFinishedConsign(TableWrapper & tab_finishconsign)
	{
		LOG_TRACE( "Merge Finished_Consign:" );
		MergeFinishedConsignQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_finishconsign.name.c_str(), tab_finishconsign.ptable, tab_finishconsign.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeFinishedConsign, error when walk, what=%s\n", e.what() );
		}
	}

	class MergeWebOrderQuery : public StorageEnv::IQuery
	{
		int64_t orderid;
	public:
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			try
			{
				StorageEnv::Storage * pstorage = StorageEnv::GetStorage("weborder");
				orderid = -1;
				Marshal::OctetsStream(key) >> orderid;
				Marshal::OctetsStream os_order;
				if (pstorage->find(key, os_order, txn))
				{
					WebOrder order1;
					os_order >> order1;
					Log::log(LOG_ERR, "MergeWebOrder orderid %lld repeat, status %d", orderid, order1.status);
					if (order1.status == WEB_ORDER_UNPROCESSED)
					{
						Log::log(LOG_ERR, "MergeWebOrder ignore order %lld", orderid);
						return true;
					}
				}
				WebOrder weborder;
				Marshal::OctetsStream(value) >> weborder;
				weborder.roleid = g_mapRoleid[weborder.roleid].first;
				pstorage->insert(key, Marshal::OctetsStream()<<weborder, txn);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log( LOG_ERR, "MergeWebOrder, orderid=%lld\n", orderid);
			}
			static size_t counter = 1;
			Auto_Checkpoint(counter);
			return true;
		}
	};

	void MergeWebOrder(TableWrapper & tab_weborder)
	{
		LOG_TRACE( "Merge WebOrder:" );
		MergeWebOrderQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_weborder.name.c_str(), tab_weborder.ptable, tab_weborder.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeWebOrder, error when walk, what=%s\n", e.what() );
		}
	}

	typedef std::multimap<int, GSNSMessage> MSGSORTMAP;
	typedef std::multimap<int, GSNSMessage, std::greater<int> > MSGREVSORTMAP;
	static MSGSORTMAP g_timeSortMap[SNS_MESSAGE_TYPE_MAX];

	typedef std::vector<Octets> MSGKEYVEC;
	static MSGKEYVEC g_keyMsgToDel;

	static void ConvertApplyIDName(GApplyInfoVector &list)
	{
		GApplyInfoVector::iterator it, ie = list.end();
		for (it = list.begin(); it != ie; ++it)
		{
			it->roleid = g_mapRoleid[it->roleid].first;
			it->rolename = g_mapRolename.Find(it->rolename);
		}
	}

	class MergeSRCSNSMessageQuery : public StorageEnv::IQuery
	{
		public:
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				try
				{
					GSNSMessage	msg;
					Marshal::OctetsStream(value) >> msg;

					msg.roleid = g_mapRoleid[msg.roleid].first;
					msg.rolename = g_mapRolename.Find(msg.rolename);

					GLeaveMessageVector::iterator it, ie = msg.leave_msgs.end();
					for (it = msg.leave_msgs.begin(); it != ie; ++it)
					{
						it->roleid = g_mapRoleid[it->roleid].first;
						it->rolename = g_mapRolename.Find(it->rolename);
					}

					ConvertApplyIDName(msg.apply_list);
					ConvertApplyIDName(msg.agree_list);
					ConvertApplyIDName(msg.deny_list);

					g_timeSortMap[(int)msg.message_type].insert(MSGSORTMAP::value_type(msg.press_time, msg));

				}
				catch ( ... )
				{
					Log::log( LOG_ERR, "MergeSRCSNSMessageQuery, marshal exception\n");
				}
				return true;
			}
	};

	class MergeDSTSNSMessageQuery : public StorageEnv::IQuery
	{
		public:
			StorageEnv::Storage *pstorage;

		public:
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				try
				{
					GSNSMessage	msg;
					Marshal::OctetsStream(value) >> msg;

					g_timeSortMap[(int)msg.message_type].insert(MSGSORTMAP::value_type(msg.press_time, msg));
					g_keyMsgToDel.push_back(key);

				}
				catch ( ... )
				{
					Log::log( LOG_ERR, "MergeDSTSNSMessageQuery, marshal exception\n");
				}
				return true;
			}
	};

	void MergeSNSMessage(TableWrapper & tab_snsmessage)
	{
		LOG_TRACE( "Merge SNSMessage:" );
		try
		{
			MergeSRCSNSMessageQuery srcq;
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_snsmessage.name.c_str(), tab_snsmessage.ptable, tab_snsmessage.uncompressor);
				cursor.walk( srcq );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeSNSMessage, error when walk, what=%s\n", e.what() );
		}
		try
		{
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage("snsmessage");
			{
				MergeDSTSNSMessageQuery dstq;
				StorageEnv::CommonTransaction	txn;
				try
				{
					StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
					cursor.walk(dstq);
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					DbException ee( DB_OLD_VERSION );
					txn.abort( ee );
					throw ee;
				}
			}

			{
				StorageEnv::CommonTransaction	txn;

				try
				{
					//删除目标数据库中所有消息
					MSGKEYVEC::iterator keyvit, keyvie = g_keyMsgToDel.end();
					for (keyvit = g_keyMsgToDel.begin(); keyvit != keyvie; ++keyvit)
						pstorage->del(*keyvit, txn);

					for (int i = 0; i < SNS_MESSAGE_TYPE_MAX; i++)
					{
						std::vector<GSNSMessage> vmsg;
						MSGSORTMAP &msgmap = g_timeSortMap[i];
						int sz = (int)msgmap.size();
						LOG_TRACE("MergeSNSMessage, message_type=%d, total before merged =%d\n", i, sz);

						if (sz > SNS_LIMIT_MSGNUM)
						{
							MSGSORTMAP::iterator it, ie = msgmap.end();
							int count;
							MSGREVSORTMAP charmmap;
							for (it=msgmap.begin(), count=0; count<sz-10; ++it, ++count)
								charmmap.insert(MSGREVSORTMAP::value_type(it->second.charm, it->second));

							for (; it != ie; ++it)
								vmsg.push_back(it->second);

							MSGREVSORTMAP::iterator cit, cite = charmmap.end();
							for (cit=charmmap.begin(), count=vmsg.size(); cit!=cite && count<SNS_LIMIT_MSGNUM; ++count, ++cit)
								vmsg.push_back(cit->second);
						}
						else
						{
							MSGSORTMAP::iterator it, ie = msgmap.end();
							for (it = msgmap.begin(); it != ie; ++it)
							{
								vmsg.push_back(it->second);
							}
						}

						LOG_TRACE("MergeSNSMessage, message_type=%d, total after merged =%d\n", i, vmsg.size());

						std::vector<GSNSMessage>::iterator vit, vite = vmsg.end();
						int message_id;
						for (vit = vmsg.begin(), message_id=0; vit != vite; ++vit, ++message_id)
						{
							vit->message_id = message_id;
							int64_t key = ((int64_t)vit->message_type<<32)|(int64_t)message_id;
							pstorage->insert(Marshal::OctetsStream()<<key, Marshal::OctetsStream()<<*vit, txn);
						}
					}
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					DbException ee( DB_OLD_VERSION );
					txn.abort( ee );
					throw ee;
				}
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeSNSMessage2, error when walk, what=%s\n", e.what() );
		}
		StorageEnv::checkpoint();
	}

	class MergeClrRoleQuery : public StorageEnv::IQuery
	{
	public:
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int oldroleid = -1, newroleid = -1;
			try
			{
				StorageEnv::Storage * pstorage = StorageEnv::GetStorage("clrrole");
				Marshal::OctetsStream	os_key(key), os_temp;

				os_key >> oldroleid;
				newroleid = g_mapRoleid[oldroleid].first;
				if( oldroleid < 128 || 0 == newroleid )
					return true;

				if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_temp, txn ) )
					Log::log(LOG_INFO, "\tWARN:MergeClrRoleQuery Error,oldroleid = %d, newroleid = %d.",
							oldroleid, newroleid );

				pstorage->insert( Marshal::OctetsStream()<<newroleid, value, txn );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log(LOG_INFO, "\tERROR: ClrRole data error, roleid = %d", oldroleid );
			}
			static size_t counter = 1;
			Auto_Checkpoint(counter);
			return true;
		}
	};

	void MergeClrRole(TableWrapper & tab_clrrole)
	{
		LOG_TRACE( "Merge ClrRole:" );
		MergeClrRoleQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn,tab_clrrole.name.c_str(), tab_clrrole.ptable, tab_clrrole.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeClrRole, error when walk, what=%s\n", e.what() );
		}
	}

	class MergeBase2Query : public StorageEnv::IQuery
	{
	public:
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int oldroleid = -1, newroleid = -1;
			try
			{
				StorageEnv::Storage * pstorage = StorageEnv::GetStorage("base2");
				Marshal::OctetsStream	os_key(key), os_temp;

				os_key >> oldroleid;
				newroleid = g_mapRoleid[oldroleid].first;
				if( oldroleid < 128 || 0 == newroleid )
					return true;

				if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_temp, txn ) )
					Log::log(LOG_INFO, "\tWARN:MergeBase2 overwrite base2,oldroleid = %d, newroleid = %d.",
							oldroleid, newroleid );

				GRoleBase2 base2;
				Marshal::OctetsStream(value) >> base2;
				base2.id = newroleid;
				pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<base2, txn );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log(LOG_INFO, "\tERROR: Base2 data error, roleid = %d", oldroleid );
			}
			static size_t counter = 1;
			Auto_Checkpoint(counter);
			return true;
		}
	};

	void MergeBase2(TableWrapper & tab_base2)
	{
		LOG_TRACE( "Merge Base2:" );
		PreLoadTable((StorageEnv::get_datadir()+"/base2").c_str());
		PreLoadTable(tab_base2.name.c_str());
		MergeBase2Query q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_base2.name.c_str(), tab_base2.ptable, tab_base2.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeBase2, error when walk, what=%s\n", e.what() );
		}
	}

	class MergeReferralQuery : public StorageEnv::IQuery
	{
	public:
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int oldroleid = -1, newroleid = -1;
			try
			{
				StorageEnv::Storage * pstorage = StorageEnv::GetStorage("referral");
				Marshal::OctetsStream	os_key(key), os_temp;

				os_key >> oldroleid;
				newroleid = g_mapRoleid[oldroleid].first;
				if( oldroleid < 128 || 0 == newroleid )
					return true;

				if( pstorage->find( Marshal::OctetsStream()<<newroleid, os_temp, txn ) )
					Log::log(LOG_INFO, "\tWARN:MergeReferral overwrite base2,oldroleid = %d, newroleid = %d.",
							oldroleid, newroleid );

				GReferral referral;
				Marshal::OctetsStream(value) >> referral;
				referral.roleid = newroleid;
				pstorage->insert( Marshal::OctetsStream()<<newroleid, Marshal::OctetsStream()<<referral, txn );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log(LOG_INFO, "\tERROR: Referral data error, roleid = %d", oldroleid );
			}
			static size_t counter = 1;
			Auto_Checkpoint(counter);
			return true;
		}
	};

	void MergeReferral(TableWrapper & tab_referral)
	{
		LOG_TRACE( "Merge Referral:" );
		MergeReferralQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_referral.name.c_str(), tab_referral.ptable, tab_referral.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeReferral, error when walk, what=%s\n", e.what() );
		}
	}

	void MergeHometownInfo(Octets & value)
	{
		//目前GHometownInfo没有需要改的
	}

	void MergeFarmCompoInfo(Octets & value)
	{
		GFarmCompoInfo farm_compo;
		Marshal::OctetsStream(value) >> farm_compo;
		std::vector<GFarmField>::iterator it = farm_compo.farm.fields.begin(), ie = farm_compo.farm.fields.end();
		for (; it != ie; ++it)
		{
			GFarmField &field = *it;
			if (field.friend_id >= 128)
			{
				int newfriend_id = g_mapRoleid[field.friend_id].first;
				if (0 != newfriend_id)
					field.friend_id = newfriend_id;
			}
		}
		Marshal::OctetsStream newvalue;
		newvalue << farm_compo;
		value.swap(newvalue);
	}

	class MergeHometownQuery : public StorageEnv::IQuery
	{
	public:
		bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
		{
			int oldroleid = -1, newroleid = -1;
			int compo_id = -1;
			int64_t key64;
			Marshal::OctetsStream oct_newkey;

			try
			{
				StorageEnv::Storage * pstorage = StorageEnv::GetStorage("hometown");
				Marshal::OctetsStream	os_key(key), os_temp;

				os_key >> key64;
				oldroleid = (int)(key64>>32);
				compo_id = (int)(key64&0xffffffff);
				newroleid = g_mapRoleid[oldroleid].first;
				if( oldroleid < 128 || 0 == newroleid )
					return true;

				switch (compo_id)
				{
				case 0:
					MergeHometownInfo(value);
					break;
				case 1:
					MergeFarmCompoInfo(value);
					break;
				default:
					break;
				}

				oct_newkey << (((int64_t)newroleid<<32)+(int64_t)compo_id);
				if( pstorage->find(oct_newkey, os_temp, txn ) )
					Log::log(LOG_INFO, "\tWARN:MergeHometown overwrite,oldroleid = %d, newroleid = %d, compo_id=%d.",
							oldroleid, newroleid, compo_id);
				pstorage->insert( oct_newkey, value, txn );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				Log::log(LOG_INFO, "\tERROR: Hometown data error, roleid = %d", oldroleid );
			}
			static size_t counter = 1;
			Auto_Checkpoint(counter);
			return true;
		}
	};

	void MergeHometown(TableWrapper & tab_hometown)
	{
		LOG_TRACE( "Merge Hometown:" );
		MergeHometownQuery q;
		try
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_hometown.name.c_str(), tab_hometown.ptable, tab_hometown.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			StorageEnv::checkpoint();
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "MergeHometown, error when walk, what=%s\n", e.what() );
		}
	}

	void MergeDBAll( const char * srcpath, bool force)
	{
		enum
		{
			cache_low = 20000,
			cache_high = 25000,
		};
		g_force = force;
		std::string data_dir = StorageEnv::get_datadir();
		std::string src_dbdata = srcpath;

		LoadConfig( );
		if (!g_itemIdMan.LoadItemId()) return;

		TableWrapper tab_user((src_dbdata + "/user").c_str());
		TableWrapper tab_base((src_dbdata + "/base").c_str());
		TableWrapper tab_status((src_dbdata + "/status").c_str());
		TableWrapper tab_rolename((src_dbdata + "/rolename").c_str());
		TableWrapper tab_family((src_dbdata + "/family").c_str());
		TableWrapper tab_faction((src_dbdata + "/faction").c_str());
		TableWrapper tab_crssvrteams((src_dbdata + "/crssvrteams").c_str());
		TableWrapper tab_familyname((src_dbdata + "/familyname").c_str());
		TableWrapper tab_factionname((src_dbdata + "/factionname").c_str());
		TableWrapper tab_crssvrteamsname((src_dbdata + "/crssvrteamsname").c_str());
		TableWrapper tab_config((src_dbdata + "/config").c_str());
		TableWrapper tab_inventory((src_dbdata + "/inventory").c_str());
		TableWrapper tab_storehouse((src_dbdata + "/storehouse").c_str());
		TableWrapper tab_task((src_dbdata + "/task").c_str());
		TableWrapper tab_achievement((src_dbdata + "/achievement").c_str());
		TableWrapper tab_waitdel((src_dbdata + "/waitdel").c_str());
		TableWrapper tab_mailbox((src_dbdata + "/mailbox").c_str());
		TableWrapper tab_message((src_dbdata + "/messages").c_str());
		TableWrapper tab_friends((src_dbdata + "/friends").c_str());
		TableWrapper tab_sect((src_dbdata + "/sect").c_str());
		TableWrapper tab_syslog((src_dbdata + "/syslog").c_str());
		TableWrapper tab_commondata((src_dbdata + "/commondata").c_str());
		TableWrapper tab_snsplayer((src_dbdata + "/snsplayerinfo").c_str());
		TableWrapper tab_snsmessage((src_dbdata + "/snsmessage").c_str());
		TableWrapper tab_clrrole((src_dbdata + "/clrrole").c_str());
		TableWrapper tab_base2((src_dbdata + "/base2").c_str());
		TableWrapper tab_referral((src_dbdata + "/referral").c_str());
		TableWrapper tab_hometown((src_dbdata + "/hometown").c_str());
		TableWrapper tab_consign((src_dbdata + "/consign").c_str());
		TableWrapper tab_finishconsign((src_dbdata + "/finished_consign").c_str());
		TableWrapper tab_gt((src_dbdata + "/gtactivate").c_str());
		TableWrapper tab_callback((src_dbdata + "/friendcallback").c_str());
		TableWrapper tab_award((src_dbdata + "/award").c_str());
		TableWrapper tab_rolenamehis((src_dbdata + "/rolenamehis").c_str());
		TableWrapper tab_dropcounter((src_dbdata + "/dropcounter").c_str());
		TableWrapper tab_kingdom((src_dbdata + "/kingdom").c_str());
		TableWrapper tab_weborder((src_dbdata + "/weborder").c_str());
		TableWrapper tab_facbase((src_dbdata + "/factionbase").c_str());

		if (!tab_user.Open(cache_high, cache_low, true) ||
				!tab_base.Open(cache_high, cache_low, true) || !tab_status.Open(cache_high, cache_low, true) ||
				!tab_rolename.Open(cache_high, cache_low, true) || !tab_family.Open(cache_high, cache_low, true) ||
				!tab_faction.Open(cache_high, cache_low, true) || !tab_familyname.Open(cache_high, cache_low, true) ||
				!tab_factionname.Open(cache_high, cache_low, true) || !tab_config.Open(cache_high, cache_low, true) ||
				!tab_inventory.Open(cache_high, cache_low, true) || !tab_storehouse.Open(cache_high, cache_low, true) ||
				!tab_task.Open(cache_high, cache_low, true) || !tab_achievement.Open(cache_high, cache_low, true) ||
				!tab_waitdel.Open(cache_high, cache_low, true) || !tab_mailbox.Open(cache_high, cache_low, true) ||
				!tab_message.Open(cache_high, cache_low, true) || !tab_friends.Open(cache_high, cache_low, true) ||
				!tab_sect.Open(cache_high, cache_low, true) || !tab_syslog.Open(cache_high, cache_low, true) ||
				!tab_commondata.Open(cache_high, cache_low, true) || !tab_snsplayer.Open(cache_high, cache_low, true) ||
				!tab_snsmessage.Open(cache_high, cache_low, true) || !tab_clrrole.Open(cache_high, cache_low, true) ||
				!tab_base2.Open(cache_high, cache_low, true) || !tab_referral.Open(cache_high, cache_low, true) ||
				!tab_hometown.Open(cache_high, cache_low, true) || !tab_consign.Open(cache_high, cache_low, true) ||
				!tab_finishconsign.Open(cache_high, cache_low, true) || !tab_gt.Open(cache_high, cache_low, true) ||
				!tab_callback.Open(cache_high, cache_low, true) || !tab_award.Open(cache_high, cache_low, true) ||
				!tab_rolenamehis.Open(cache_high, cache_low, true) || !tab_dropcounter.Open(cache_high, cache_low, true) ||
				!tab_kingdom.Open(cache_high, cache_low, true) || !tab_weborder.Open(cache_high, cache_low, true) ||
				!tab_crssvrteams.Open(cache_high, cache_low, true) || !tab_crssvrteamsname.Open(cache_high, cache_low, true)
				|| !tab_facbase.Open(cache_high, cache_low, true)
		   )
		{
			Log::log(LOG_ERR, "Error: Open DB Failed");
			return;
		}

		// determine map
		if (!CheckDBType(tab_config, tab_base))
			return;
		if (!PrepareLogicuid())
			return;
		if (!CheckDupLogicuid()) //CheckDupLogicuid要在PrepareLogicuid()之后调用
		{
			Log::log(LOG_ERR, "CheckDupLogicuid failed\n");
			return;
		}

		LOG_TRACE("Begin Merge");
		//ClearDstCircleHis(); 合并到PrepareLogicuid()中处理
		//Clear src db's circlehistory in MergeBase

		MergeUser(tab_user);
		ReOpenDB();
		MergeRolename(tab_rolename);
		ReOpenDB();

		MergeFamily(tab_family);
		MergeFaction(tab_faction);
		MergeFamilyname(tab_familyname);
		MergeFactionname(tab_factionname);
		MergeCrssvrTeams(tab_crssvrteams);
		MergeCrssvrTeamsname(tab_crssvrteamsname);
		UpdateCrssvrTeamsSeasonTop(tab_crssvrteams);
		ReOpenDB();

		LOG_TRACE( "\nMap Size:\n\tINFO:mapRoleid.size = %d\n\tINFO:mapRolename.size = %d(%d)\n\tINFO:mapFactionid.size = %d\n\tINFO:mapFactionname.size = %d(%d)\n\tINFO:mapFamilyid.size = %d\n\tINFO:mapFamilyname.size = %d(%d)\n\tINFO:mapCrssvrTeamsid.size = %d\n\tINFO:mapCrssvrTeamsname.size = %d(%d)\n", 
				g_mapRoleid.size(), g_mapRolename.RawSize(), g_mapRolename.RealSize(), 
				g_mapFactionid.size(), g_mapFactionname.RawSize(), g_mapFactionname.RealSize(), 
				g_mapFamilyid.size(), g_mapFamilyname.RawSize(), g_mapFamilyname.RealSize(),
				g_mapCrssvrTeamsid.size(), g_mapCrssvrTeamsname.RawSize(), g_mapCrssvrTeamsname.RealSize() );
 
		// key改roleid
		MergeBase(tab_base);
		MergeStatus(tab_status);
		MergeRoleData(tab_inventory, "inventory" );
		MergeRoleData(tab_storehouse, "storehouse" );
		MergeRoleData(tab_task, "task" );
		MergeRoleData(tab_achievement, "achievement" );
		MergeRoleData(tab_gt, "gtactivate" );
		MergeRoleData(tab_callback, "friendcallback" ); //callback内部的roleid变换没有处理 忽略
		MergeRoleData(tab_award, "award" );

		MergeWaitdel(tab_waitdel);

		// key改roleid,value改roleid,factionid,name
		MergeMailbox(tab_mailbox);
		MergeMessages(tab_message);
		ReOpenDB();
		MergeFriends(tab_friends);
		MergeSect(tab_sect);

		MergeSyslog(tab_syslog);

		MergeCommonData(tab_commondata);
		//合并交友平台相关表
		MergeSNSPlayerInfo(tab_snsplayer);
		MergeSNSMessage(tab_snsmessage);

		MergeClrRole(tab_clrrole);

		//合并线上推广相关表
		MergeBase2(tab_base2);
		MergeReferral(tab_referral);

		//合并庄园各种组件信息
		MergeHometown(tab_hometown);

		//merge 寄售相关表
		MergeConsign(tab_consign);
		MergeFinishedConsign(tab_finishconsign);

		MergeDropCounter(tab_dropcounter);
		MergeRolenamehis(tab_rolenamehis);
		MergeKingdom(tab_kingdom);
		MergeConfig(tab_config);
		MergeWebOrder(tab_weborder);
		MergeFacBase(tab_facbase);
		//注意！！！添加新表合服操作时需同步修改 MoveDBTo 函数

		StorageEnv::checkpoint();
		StorageEnv::Close();

		system( ("/bin/rm -f " + data_dir + "/order").c_str() );
		system( ("/bin/rm -f " + data_dir + "/citystore").c_str() );
		system( ("/bin/rm -f " + data_dir + "/gtask").c_str() );
		system( ("/bin/rm -f " + data_dir + "/top").c_str() );
		system( ("/bin/rm -f " + data_dir + "/uniqueauction").c_str() );
		system( ("/bin/rm -f " + data_dir + "/circle").c_str() );

		StorageEnv::Open();
		StorageEnv::checkpoint( );
		StorageEnv::removeoldlogs( );

		LOG_TRACE( "\nMap Size:\n\tINFO:mapRoleid.size = %d\n\tINFO:mapRolename.size = %d(%d)\n\tINFO:mapFactionid.size = %d\n\tINFO:mapFactionname.size = %d(%d)\n\tINFO:mapFamilyid.size = %d\n\tINFO:mapFamilyname.size = %d(%d)\n\tINFO:mapCrssvrTeamsid.size = %d\n\tINFO:mapCrssvrTeamsname.size = %d(%d)\n", 
				g_mapRoleid.size(), g_mapRolename.RawSize(), g_mapRolename.RealSize(), 
				g_mapFactionid.size(), g_mapFactionname.RawSize(), g_mapFactionname.RealSize(), 
				g_mapFamilyid.size(), g_mapFamilyname.RawSize(), g_mapFamilyname.RealSize(),
				g_mapCrssvrTeamsid.size(), g_mapCrssvrTeamsname.RawSize(), g_mapCrssvrTeamsname.RealSize() );
		LOG_TRACE( "\nMerge Report:\n\tINFO:server2_usercount = %d\n\tINFO:same_usercount = %d\n\tINFO:server2_rolecount = %d\n\tINFO:same_rolecount = %d\n\tINFO:dup_roleid = %d\n\tINFO:dup_factionid = %d\n\tINFO:dup_familyid = %d\n\tINFO:dup_rolename = %d\n\tINFO:dup_factionname = %d\n\tINFO:dup_familyname = %d\n\tINFO:dup_crssvrteamsid = %d\n\tINFO:dup_crssvrteamsname = %d\n", 
			server2_usercount, same_usercount, server2_rolecount, same_rolecount, dup_roleid, dup_factionid, 
			dup_familyid, dup_rolename, dup_factionname, dup_familyname, dup_crssvrteamsid, dup_crssvrteamsname );
	}

	class FetchExpireFrozenIdQuery : public StorageEnv::IQuery
	{
		public:
			int day;
			std::map<int/*roleid*/, int/*userid*/> frozen_ids;
			FetchExpireFrozenIdQuery(int _day):day(_day){ frozen_ids.clear(); }
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				static time_t now = time(NULL);
				try
				{
					Marshal::OctetsStream   os_key(key), os_value(value);
					int id = -1;
					os_key >> id;
					GRoleBase base;
					os_value >> base;
					if (base.status == _ROLE_STATUS_FROZEN && now-base.lastlogin_time>day*86400)
					{
						int userid = (base.userid==0 ? LOGICUID(id) : base.userid);
						frozen_ids.insert(std::make_pair(id, userid));
					}
				}
				catch ( Marshal::Exception e )
				{
					Log::log( LOG_ERR, "FetchExpireFrozenIdQuery, exception\n" );
//					throw e;
				}
				return true;
			}
	};
	class FetchZonePlayerIdQuery : public StorageEnv::IQuery
	{
		public:
			int zoneid;
			TableWrapper & tab_base2;
			std::map<int/*roleid*/, int/*userid*/> zone_ids;
			FetchZonePlayerIdQuery(int _zone, TableWrapper & _base2):zoneid(_zone), tab_base2(_base2) { zone_ids.clear(); }
			bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
			{
				try
				{
					Marshal::OctetsStream   os_key(key), os_value(value), os_base2;
					int id = -1;
					os_key >> id;
					GRoleBase base;
					os_value >> base;
					if (base.status != _ROLE_STATUS_FROZEN)
						Log::log(LOG_ERR, "FetchZonePlayerIdQuery role %d status %d invalid", id, base.status);
					if (!tab_base2.Find(os_key, os_base2))
					{
						Log::log(LOG_ERR, "FetchZonePlayerIdQuery role %d can not find base2", id);
						return true;
					}
					GRoleBase2 base2;
					os_base2 >> base2;
					if (base2.src_zoneid == zoneid)
					{
						int userid = (base.userid==0 ? LOGICUID(id) : base.userid);
						zone_ids.insert(std::make_pair(id, userid));
					}
				}
				catch ( Marshal::Exception e )
				{
					Log::log( LOG_ERR, "FetchZonePlayerIdQuery, exception\n" );
//					throw e;
				}
				return true;
			}
	};

	void _DelPlayers(const std::map<int, int> & idmap,
			TableWrapper & tab_user,
			TableWrapper & tab_base,
			TableWrapper & tab_base2,
			TableWrapper & tab_status,
			TableWrapper & tab_inventory,
			TableWrapper & tab_storehouse,
			TableWrapper & tab_task,
			TableWrapper & tab_achieve,
			TableWrapper & tab_friends,
			TableWrapper & tab_rolename,
			TableWrapper & tab_rolenamehis,
			TableWrapper & tab_award,
			TableWrapper & tab_family,
			TableWrapper & tab_crssvrteams,
			TableWrapper & tab_callback,
			TableWrapper & tab_referral,
			TableWrapper & tab_gt,
			TableWrapper & tab_waitdel,
			TableWrapper & tab_home,
			TableWrapper & tab_snsplayer,
			TableWrapper & tab_mail)
	{
		#define CHECKPOINT_THRESHOLD 4096
		std::map<int, int>::const_iterator it = idmap.begin(), ite = idmap.end();
		bool finished = false;
		int del_success_count = 0;
		std::map<int, int> total_items;
		time_t now = time(NULL);
		while (!finished)
		{
			try
			{
				int i = 0;
				for (; it != ite && i < CHECKPOINT_THRESHOLD; ++it,++i)
				{
					Marshal::OctetsStream os_key, os_base, os_rolenamehis, key64_roleid, os_inventory, os_store;
					os_key << it->first;

					bool is360role = false;
					std::map<int,int> items;
					GRoleBase base;
					if (tab_base.Find(os_key, os_base))
					{
						try
						{
							os_base >> base;
						}
						catch(...)
						{
							Log::log(LOG_ERR, "unmarshal base err, role %d",
									it->first);
						}
						is360role = (base.lastlogin_time && now - base.lastlogin_time > 365*24*3600);
						if (base.name.size())
							tab_rolename.Del(base.name);
						if (base.familyid > 0)
						{
							Marshal::OctetsStream kfamily, os_family;
							kfamily << base.familyid;
							if (tab_family.Find(kfamily, os_family))
							{
								GFamily family;
								os_family >> family;
								GFolkVector::iterator fit, fite = family.member.end();
								for (fit = family.member.begin(); fit != fite; ++fit)
								{
									if ((int)fit->rid == it->first)
									{
										family.member.erase(fit);
										break;
									}
								}
								if ((int)family.master == it->first)
									family.master = 0;
								tab_family.Put(kfamily, Marshal::OctetsStream() << family);
							}
						}
						if (base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID] > 0)
						{
							Marshal::OctetsStream kcrssvrteams, os_crssvrteams;
							kcrssvrteams << base.datagroup[KEY_DATAGOURP_ROLE_CT_TEAMID];
							if (tab_crssvrteams.Find(kcrssvrteams, os_crssvrteams))
							{
								GCrssvrTeamsData crssvrteams;
								os_crssvrteams >> crssvrteams;
								std::vector<GCrssvrTeamsRole>::iterator fit, fite = crssvrteams.roles.end();
								for (fit = crssvrteams.roles.begin(); fit != fite; ++fit)
								{
									if ((int)fit->roleid == it->first)
									{
										crssvrteams.roles.erase(fit);
										break;
									}
								}
								if ((int)crssvrteams.info.master == it->first)
									crssvrteams.info.master = 0;
								tab_crssvrteams.Put(kcrssvrteams, Marshal::OctetsStream() << crssvrteams);
							}
						}

						if (base.spouse > 0)
						{
							Marshal::OctetsStream kspouse, os_spouse;
							kspouse << base.spouse;
							if (tab_base.Find(kspouse, os_spouse))
							{
								GRoleBase base_spouse;
								os_spouse >> base_spouse;
								if ((int)base_spouse.spouse == it->first)
								{
									LOG_TRACE("clear spouse(%d).spouse of deleting role %d", base.spouse, it->first);
									base_spouse.spouse = 0;
									tab_base.Put(kspouse, Marshal::OctetsStream() << base_spouse);
								}
							}
						}
					}
					tab_base.Del(os_key);
					tab_base2.Del(os_key);
					tab_status.Del(os_key);
					if (tab_inventory.Find(os_key, os_inventory))
					{
						GRolePocket pocket;
						try
						{
							os_inventory >> pocket;
						}
						catch(...)
						{
						}
						/*
						for (GRoleInventoryVector::iterator pit = pocket.items.begin(); pit != pocket.items.end(); pit++)
						{
							if(DBClearConsumable::IsConsumable(pit->id))
							{
								items[pit->id] += pit->count;
								if (is360role)
									total_items[pit->id] += pit->count;
							}
						}
						*/
						GetCashItems(pocket, items);
						tab_inventory.Del(os_key);
					}
					if (tab_storehouse.Find(os_key, os_store))
					{
						GRoleStorehouse store;
						try
						{
							os_store >> store;
						}
						catch(...)
						{
						}
						/*
						for (GRoleInventoryVector::iterator pit = store.items.begin(); pit != store.items.end(); pit++)
						{
							if(DBClearConsumable::IsConsumable(pit->id))
							{
								items[pit->id] += pit->count;
								if (is360role)
									total_items[pit->id] += pit->count;
							}
						}
						*/
						GetCashItems(store, items);
						tab_storehouse.Del(os_key);
					}
					tab_task.Del(os_key);
					tab_achieve.Del(os_key);
					tab_friends.Del(os_key);
					tab_award.Del(os_key);
					tab_callback.Del(os_key);
					tab_referral.Del(os_key);
					tab_achieve.Del(os_key);
					tab_gt.Del(os_key);
					tab_snsplayer.Del(os_key);
					tab_mail.Del(os_key);
					tab_waitdel.Del(Marshal::OctetsStream() << WaitDelKey(it->first, TYPE_ROLE)); 

					key64_roleid<<((int64_t)it->first<<32);
					tab_home.Del(key64_roleid);

					Marshal::OctetsStream key_all, value_all, value_rolenamehis;
					key_all << (int)0;
					if (tab_rolenamehis.Find(key_all, value_all))
					{
						std::map<int, Octets> name_map;
						value_all >> name_map;
						if (name_map.erase(it->first) > 0)
						{
							LOG_TRACE("erase roleid %d from global namemap", it->first);
							tab_rolenamehis.Put(key_all, Marshal::OctetsStream() << name_map);
						}
					}
					GRoleNameHisVector rolenamehis_vec;
					if (tab_rolenamehis.Find(os_key, os_rolenamehis))
					{
						os_rolenamehis >> rolenamehis_vec;
						GRoleNameHisVector::const_iterator rit, rite = rolenamehis_vec.end();
						for (rit = rolenamehis_vec.begin(); rit != rite; ++rit)
						{
							Marshal::OctetsStream os_rolename;
							if (tab_rolename.Find(rit->name, os_rolename))
							{
								int tmpid = 0;
								os_rolename >> tmpid; 
								if (it->first == tmpid)
									tab_rolename.Del(rit->name);
							}
						}
						tab_rolenamehis.Del(os_key);
					}

					Marshal::OctetsStream user_key, os_user;
					user_key << it->second;
					if (tab_user.Find(user_key, os_user))
					{
						User user;
						os_user >> user;
						if (user.logicuid == (unsigned int)LOGICUID(it->first))
						{
							RoleList t(user.rolelist);
							t.DelRole(it->first);
							user.rolelist = t.GetRoleList();
							tab_user.Put(user_key, Marshal::OctetsStream()<<user);
						}
						else
							Log::log(LOG_INFO, "INFO:logicuid %d of user %d does not match roleid %d", user.logicuid, it->second, it->first);
					}
					else
						Log::log(LOG_ERR, "Can not find userid %d for role %d", it->second, it->first);
					std::string str;
					if (items.size())
					{
						for(std::map<int,int>::const_iterator iit=items.begin();iit!=items.end();++iit)
						{
							char buf[32];
							sprintf(buf, "%d:%d;", iit->first, iit->second);
							str += buf;

							if (is360role)
								total_items[iit->first] += iit->second;
						}
					}
					if (is360role)
						LOG_TRACE("Del360Role roleid=%d:userid=%d:logintime=%d:items:%s", it->first, it->second, base.lastlogin_time, str.c_str());
					else
						LOG_TRACE("DelRole roleid=%d:userid=%d:logintime=%d:items:%s", it->first, it->second, base.lastlogin_time, str.c_str());

					del_success_count++; 
				}
				if (it == ite)
					finished = true;
			}
			catch (...)
			{
				Log::log(LOG_ERR, "_DelPlayers, exception roleid %d", it->first);
				++it;
			}
			//StorageEnv::checkpoint();
			tab_user.Checkpoint();
			tab_base.Checkpoint();
			tab_base2.Checkpoint();
			tab_status.Checkpoint();
			tab_inventory.Checkpoint();
			tab_storehouse.Checkpoint();
			tab_task.Checkpoint();
			tab_achieve.Checkpoint();
			tab_friends.Checkpoint();
			tab_rolename.Checkpoint();
			tab_rolenamehis.Checkpoint();
			tab_award.Checkpoint();
			tab_family.Checkpoint();
			tab_crssvrteams.Checkpoint();
			tab_callback.Checkpoint(); 
			tab_referral.Checkpoint(); 
			tab_gt.Checkpoint(); 
			tab_waitdel.Checkpoint(); 
			tab_home.Checkpoint(); 
			tab_snsplayer.Checkpoint();
			tab_mail.Checkpoint();

			LOG_TRACE("checkpoint delete %d success", del_success_count);
		}
		std::string total_str;
		if (total_items.size())
		{
			for(std::map<int,int>::iterator it=total_items.begin();it!=total_items.end();++it)
			{
				char buf[32];
				sprintf(buf, "%d:%d;", it->first, it->second);
				total_str += buf;
			}
		}
		LOG_TRACE("Del360RoleItemsTotal items:%s", total_str.c_str());
		LOG_TRACE("_DelPlayers end, delete %d players successfully", del_success_count);
	}

	void DelZonePlayers(const char * _dbdata, int zoneid)
	{
		enum
		{
			cache_low = 20000,
			cache_high = 25000,
		};
		std::string dbdata = _dbdata;
		if (access(dbdata.c_str(), F_OK))
		{
			Log::log(LOG_ERR, "Error: cannot find %s", dbdata.c_str());
			return;
		}
		TableWrapper tab_config((dbdata + "/config").c_str());
		TableWrapper tab_user((dbdata + "/user").c_str());
		TableWrapper tab_base((dbdata + "/base").c_str());
		TableWrapper tab_base2((dbdata + "/base2").c_str());
		TableWrapper tab_status((dbdata + "/status").c_str());
		TableWrapper tab_inventory((dbdata + "/inventory").c_str());
		TableWrapper tab_storehouse((dbdata + "/storehouse").c_str());
		TableWrapper tab_task((dbdata + "/task").c_str());
		TableWrapper tab_achieve((dbdata + "/achievement").c_str());
		TableWrapper tab_friends((dbdata + "/friends").c_str());
		TableWrapper tab_rolename((dbdata + "/rolename").c_str());
		TableWrapper tab_rolenamehis((dbdata + "/rolenamehis").c_str());
		TableWrapper tab_award((dbdata + "/award").c_str());
		TableWrapper tab_family((dbdata + "/family").c_str());
		TableWrapper tab_crssvrteams((dbdata + "/crssvrteams").c_str());
		TableWrapper tab_callback((dbdata + "/callback").c_str());
		TableWrapper tab_referral((dbdata + "/referral").c_str());
		TableWrapper tab_gt((dbdata + "/gtactivate").c_str());
		TableWrapper tab_waitdel((dbdata + "/waitdel").c_str());
		TableWrapper tab_home((dbdata + "/hometown").c_str());
		TableWrapper tab_snsplayer((dbdata + "/snsplayerinfo").c_str());
		TableWrapper tab_mail((dbdata + "/mailbox").c_str());

		if (!tab_config.Open(cache_high, cache_low, true) || !tab_user.Open(cache_high, cache_low, true) ||
			!tab_base.Open(cache_high, cache_low, true) || !tab_base2.Open(cache_high, cache_low, true) ||
			!tab_status.Open(cache_high, cache_low, true) || !tab_inventory.Open(cache_high, cache_low, true) ||
			!tab_storehouse.Open(cache_high, cache_low, true) || !tab_task.Open(cache_high, cache_low, true) ||
			!tab_achieve.Open(cache_high, cache_low, true) || !tab_friends.Open(cache_high, cache_low, true) ||
			!tab_rolename.Open(cache_high, cache_low, true) || !tab_rolenamehis.Open(cache_high, cache_low, true) ||
			!tab_award.Open(cache_high, cache_low, true) || !tab_callback.Open(cache_high, cache_low, true) ||
			!tab_referral.Open(cache_high, cache_low, true) || !tab_gt.Open(cache_high, cache_low, true) ||
			!tab_waitdel.Open(cache_high, cache_low, true) || !tab_home.Open(cache_high, cache_low, true) ||
			!tab_snsplayer.Open(cache_high, cache_low, true) || !tab_family.Open(cache_high, cache_low, true) ||
			!tab_mail.Open(cache_high, cache_low, true) || !tab_crssvrteams.Open(cache_high, cache_low, true) )
		{
			Log::log(LOG_ERR, "Error: Open DB Failed");
			return;
		}
		if (GetDBType(tab_config) != 1)
		{
			Log::log(LOG_ERR, "Invalid DB type");	
			return;
		}
		if (zoneid <= 0)
		{
			Log::log(LOG_ERR, "Invalid parameter zoneid %d", zoneid);
			return;
		}
		LOG_TRACE("del frozen players of zone %d", zoneid);
		LOG_TRACE("FetchZonePlayerId begin...");
		FetchZonePlayerIdQuery q(zoneid, tab_base2);
		try
		{
			StorageEnv::AtomTransaction     txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_base.name.c_str(), tab_base.ptable, tab_base.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "AbstractPlayers, error when walk for FetchZonePlayerIdQuery, what=%s\n", e.what() );
		}
		LOG_TRACE("FetchZonePlayerId end, total set size %d", q.zone_ids.size()); 
		_DelPlayers(q.zone_ids, tab_user, tab_base, tab_base2, tab_status, tab_inventory, tab_storehouse,
				tab_task, tab_achieve, tab_friends, tab_rolename, tab_rolenamehis, tab_award,
			tab_family, tab_crssvrteams, tab_callback, tab_referral, tab_gt, tab_waitdel, tab_home, tab_snsplayer, tab_mail);
	}

	void DelFrozenPlayers(const char * _dbdata, int day)
	{
		enum
		{
			cache_low = 20000,
			cache_high = 25000,
		};
		std::string dbdata = _dbdata;
		if (access(dbdata.c_str(), F_OK))
		{
			Log::log(LOG_ERR, "Error: cannot find %s", dbdata.c_str());
			return;
		}
		TableWrapper tab_config((dbdata + "/config").c_str());
		TableWrapper tab_user((dbdata + "/user").c_str());
		TableWrapper tab_base((dbdata + "/base").c_str());
		TableWrapper tab_base2((dbdata + "/base2").c_str());
		TableWrapper tab_status((dbdata + "/status").c_str());
		TableWrapper tab_inventory((dbdata + "/inventory").c_str());
		TableWrapper tab_storehouse((dbdata + "/storehouse").c_str());
		TableWrapper tab_task((dbdata + "/task").c_str());
		TableWrapper tab_achieve((dbdata + "/achievement").c_str());
		TableWrapper tab_friends((dbdata + "/friends").c_str());
		TableWrapper tab_rolename((dbdata + "/rolename").c_str());
		TableWrapper tab_rolenamehis((dbdata + "/rolenamehis").c_str());
		TableWrapper tab_award((dbdata + "/award").c_str());
		TableWrapper tab_family((dbdata + "/family").c_str());
		TableWrapper tab_crssvrteams((dbdata + "/crssvrteams").c_str());
		TableWrapper tab_callback((dbdata + "/callback").c_str());
		TableWrapper tab_referral((dbdata + "/referral").c_str());
		TableWrapper tab_gt((dbdata + "/gtactivate").c_str());
		TableWrapper tab_waitdel((dbdata + "/waitdel").c_str());
		TableWrapper tab_home((dbdata + "/hometown").c_str());
		TableWrapper tab_snsplayer((dbdata + "/snsplayerinfo").c_str());
		TableWrapper tab_mail((dbdata + "/mailbox").c_str());

		if (!tab_config.Open(cache_high, cache_low, true) || !tab_user.Open(cache_high, cache_low, true) ||
			!tab_base.Open(cache_high, cache_low, true) || !tab_base2.Open(cache_high, cache_low, true) ||
			!tab_status.Open(cache_high, cache_low, true) || !tab_inventory.Open(cache_high, cache_low, true) ||
			!tab_storehouse.Open(cache_high, cache_low, true) || !tab_task.Open(cache_high, cache_low, true) ||
			!tab_achieve.Open(cache_high, cache_low, true) || !tab_friends.Open(cache_high, cache_low, true) ||
			!tab_rolename.Open(cache_high, cache_low, true) || !tab_rolenamehis.Open(cache_high, cache_low, true) ||
			!tab_award.Open(cache_high, cache_low, true) || !tab_callback.Open(cache_high, cache_low, true) ||
			!tab_referral.Open(cache_high, cache_low, true) || !tab_gt.Open(cache_high, cache_low, true) ||
			!tab_waitdel.Open(cache_high, cache_low, true) || !tab_home.Open(cache_high, cache_low, true) ||
			!tab_snsplayer.Open(cache_high, cache_low, true) || !tab_family.Open(cache_high, cache_low, true) ||
			!tab_mail.Open(cache_high, cache_low, true) || !tab_crssvrteams.Open(cache_high, cache_low, true) )

		{
			Log::log(LOG_ERR, "Error: Open DB Failed");
			return;
		}
		if (GetDBType(tab_config) != 1)
		{
			Log::log(LOG_ERR, "Invalid DB type");	
			return;
		}
		if (day <= 0)
		{
			Log::log(LOG_ERR, "Invalid parameter day %d", day);
			return;
		}
		LOG_TRACE("Frozen player who has not logged in for at least %d days will be deleted from DB", day);
		LOG_TRACE("FetchExpireFrozenId begin...");
		FetchExpireFrozenIdQuery q(day);
		try
		{
			StorageEnv::AtomTransaction     txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, tab_base.name.c_str(), tab_base.ptable, tab_base.uncompressor);
				cursor.walk( q );
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "AbstractPlayers, error when walk for FetchExpireFrozenIdQuery, what=%s\n", e.what() );
		}
		LOG_TRACE("FetchExpireFrozenId end, totalfrozen set size %d", q.frozen_ids.size()); 
		_DelPlayers(q.frozen_ids, tab_user, tab_base, tab_base2, tab_status, tab_inventory, tab_storehouse,
				tab_task, tab_achieve, tab_friends, tab_rolename, tab_rolenamehis, tab_award,
				tab_family, tab_crssvrteams, tab_callback, tab_referral, tab_gt, tab_waitdel, tab_home, tab_snsplayer, tab_mail);
	}

	void AllocNewLogicuid(const char * useridfile, int default_start)
	{
		if (access(useridfile, R_OK) != 0)
                {
                        Log::log(LOG_ERR, "Can't access %s\n", useridfile);
                        return;
                } 
		if (!g_itemIdMan.LoadItemId()) return;

		std::set<int> user_set;
		std::ifstream ifs(useridfile);
		string line;
		while (std::getline(ifs, line))
		{
			int uid = atoi(line.c_str());
			if( uid > 0 )
				user_set.insert(uid);
		}
		LOG_TRACE("Fetch %d users from %s", user_set.size(), useridfile);
		if (user_set.size() == 0)
			return;

		DupLogicuidQuery q;
		q.RepairSpecifiedUser(user_set, default_start)
				&& q.RepairRoleData()
				&& q.RepairFamily()
				&& q.RepairFaction()
				&& q.RepairCrssvrTeams()
				&& q.RepairMailbox()
				&& q.RepairFriendList()
				&& q.RepairSect()
				&& q.RepairSyslog()
				&& q.RepairSNS()
				&& q.RepairReference()
				&& q.RepairConsign();
	}

class LogicuidAllocator
{
	static LogicuidAllocator * instance;
	std::vector<int>   idset;
	int  startid;
	TableWrapper & plogicuid;
	TableWrapper & puidrole;
	TableWrapper & punamefamily;
	TableWrapper & punamefaction;
	TableWrapper & punamecrssvrteams;
	LogicuidAllocator(TableWrapper & _plogicuid, TableWrapper & _puidrole, TableWrapper & _pfamily, TableWrapper & _pfac, TableWrapper & _pcrssvrteams) : startid(0), plogicuid(_plogicuid), puidrole(_puidrole), punamefamily(_pfamily), punamefaction(_pfac), punamecrssvrteams(_pcrssvrteams) { }
public:
	static LogicuidAllocator * GetInstance(TableWrapper & _t1, TableWrapper & _t2, TableWrapper & _t3, TableWrapper & _t4, TableWrapper & _t5)
	{
		if (instance == NULL)
			instance = new LogicuidAllocator(_t1, _t2, _t3, _t4, _t5);
		return instance;
	}
	static LogicuidAllocator * GetInstance()
	{
		return instance;
	}
	size_t  FindFreeLogicuid()
	{
		int count = 0;
		int firstid = 0;
		size_t sizelogic=0,sizeuser=0;
		if (idset.size())
			firstid = idset.front();
		count = 256 - idset.size();
		clock_t start = clock();
		std::vector<int> list;
		try
		{
			Marshal::OctetsStream key_default;
			sizelogic = plogicuid.ptable->record_count();
			sizeuser  = puidrole.ptable->record_count();
			key_default << 0;
			if(!startid)
			{
				Marshal::OctetsStream value;
				if (plogicuid.Find(key_default, value))
					value >> startid;
				else
					startid = LOGICUID_START;
				startid = LOGICUID(startid);
			}
			for(int i=0; count>0 && startid<LOGICUID_MAX && i<4096; startid+=16,i++)
			{
				Marshal::OctetsStream value, value_logic;
				if (plogicuid.Find(Marshal::OctetsStream()<<startid, value_logic))
					continue;
				if (puidrole.Find(Marshal::OctetsStream()<<startid, value))
				{
					unsigned int roles = 0, logicuid = 0;
					if(value.size()==8)
						value >> roles >> logicuid;
					else
						continue;
					if((int)logicuid==startid)
						continue;
				}
				count--;
				list.push_back(startid);
			}
			if(!firstid && list.size())
				firstid = list.front();
			if(startid>=LOGICUID_MAX)
				firstid = LOGICUID_START; // Largest logicuid reached, seek from beginning
			if(firstid)
				plogicuid.Put(key_default, Marshal::OctetsStream()<<firstid);
		}
		catch ( DbException e )
		{
			Log::log(LOG_ERR,"FindFreeLogicuid: what=%s.", e.what());
		}
		catch ( ... )
		{
			Log::log(LOG_ERR,"FindFreeLogicuid: unknown error.");
		}
		clock_t used = clock()-start;
		LOG_TRACE("uidrole=%d:logicuid=%d:firstid=%d:idsfind=%d:timeused=%.3f",
				sizeuser,sizelogic,firstid,list.size(),(float)used/CLOCKS_PER_SEC);
		idset.insert(idset.end(), list.begin(), list.end());
		return idset.size();
	}
	int     AllocLogicuid()
	{
		if(idset.size()<=10)
			FindFreeLogicuid();
		int id = 0;
		if(idset.size())
		{
			id = idset.front();
			idset.erase(idset.begin());
		}
		return id;
	}
	int GetNextFamilyId()
	{
		int temp = 0, nextid = 1;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid;
			key_nextid << (int)0;
			if (!punamefamily.Find(key_nextid, value_nextid))
			{
				Log::log(LOG_ERR, "can not find nextid for family");
				return nextid;
			}
			value_nextid >> temp >> nextid;
		}
		catch (...)
		{
			Log::log(LOG_ERR, "GetNextFamilyId exception");
			assert(false);
			return 0;
		}
		LOG_TRACE("GetNextFamilyId %d", nextid);
		return nextid;
	}
	void SaveNextFamilyId(int fid)
	{
		LOG_TRACE("SaveNextFamilyId %d", fid);
		int temp = 0, oldnextid = 0;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid;
			key_nextid << (int)0;
			if (punamefamily.Find(key_nextid, value_nextid))
				value_nextid >> temp >> oldnextid;
			if (fid < oldnextid)
				Log::log(LOG_ERR, "SaveNextFamilyId new nextid %d is less than old %d", fid, oldnextid);
			value_nextid.clear();
			value_nextid << temp << fid << (int)UNIQUENAME_USED << temp;
			punamefamily.Put(key_nextid, value_nextid);
		}
		catch (...)
		{
			Log::log(LOG_ERR, "SaveNextFamilyId exception");
			assert(false);
		}
	}
	int GetNextFactionId()
	{
		int temp = 0, nextid = 1;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid;
			key_nextid << (int)0;
			if (!punamefaction.Find(key_nextid, value_nextid))
			{
				Log::log(LOG_ERR, "can not find nextid for faction");
				return nextid;
			}
			value_nextid >> temp >> nextid;
		}
		catch (...)
		{
			Log::log(LOG_ERR, "GetNextFactionId exception");
			assert(false);
			return 0;
		}
		LOG_TRACE("GetNextFactionId %d", nextid);
		return nextid;
	}
	void SaveNextFactionId(int fid)
	{
		LOG_TRACE("SaveNextFactionId %d", fid);
		int temp = 0, oldnextid = 0;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid;
			key_nextid << (int)0;
			if (punamefaction.Find(key_nextid, value_nextid))
				value_nextid >> temp >> oldnextid;
			if (fid < oldnextid)
				Log::log(LOG_ERR, "SaveNextFactionId new nextid %d is less than old %d", fid, oldnextid);
			value_nextid.clear();
			value_nextid << temp << fid << (int)UNIQUENAME_USED << temp;
			punamefaction.Put(key_nextid, value_nextid);
		}
		catch (...)
		{
			Log::log(LOG_ERR, "SaveNextFactionId exception");
			assert(false);
		}
	}
	int GetNextCrssvrTeamsId()
	{
		int temp = 0, nextid = 1;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid;
			key_nextid << (int)0;
			if (!punamecrssvrteams.Find(key_nextid, value_nextid))
			{
				Log::log(LOG_ERR, "can not find nextid for crssvrteams");
				return nextid;
			}
			value_nextid >> temp >> nextid;
		}
		catch (...)
		{
			Log::log(LOG_ERR, "GetNextCrssvrTeamsId exception");
			assert(false);
			return 0;
		}
		LOG_TRACE("GetNextCrssvrTeamsId %d", nextid);
		return nextid;
	}
	void SaveNextCrssvrTeamsId(int fid)
	{
		LOG_TRACE("SaveNextCrssvrTeamsId %d", fid);
		int temp = 0, oldnextid = 0;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid;
			key_nextid << (int)0;
			if (punamecrssvrteams.Find(key_nextid, value_nextid))
				value_nextid >> temp >> oldnextid;
			if (fid < oldnextid)
				Log::log(LOG_ERR, "SaveNextCrssvrTeamsId new nextid %d is less than old %d", fid, oldnextid);
			value_nextid.clear();
			value_nextid << temp << fid << (int)UNIQUENAME_USED << temp;
			punamecrssvrteams.Put(key_nextid, value_nextid);
		}
		catch (...)
		{
			Log::log(LOG_ERR, "SaveNextCrssvrTeamsId exception");
			assert(false);
		}
	}

};

LogicuidAllocator * LogicuidAllocator::instance = NULL;

class MoveUserQuery : public StorageEnv::IQuery
{
	TableWrapper & tab_uidrole;
	TableWrapper & tab_logicuid;
public:
	std::vector<int> setUserNeedSetReferrer;	//需要重新指定referrer的账号列表
//	StorageEnv::Storage * puser;
	MoveUserQuery(TableWrapper & _uidrole, TableWrapper & _logic) : tab_uidrole(_uidrole), tab_logicuid(_logic) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int userid = -1;
		try
		{
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			Marshal::OctetsStream(key) >> userid;
			if( userid < 128 )
				return true;
			//server2_usercount ++;
			User	user;
			Marshal::OctetsStream(value) >> user;
			if (user.referrer != 0)
				setUserNeedSetReferrer.push_back(userid);
			RoleList src_rolelist(user.rolelist);
			if (src_rolelist.GetRoleCount() == 0)
			{
				LOG_TRACE("user %d rolelist %d empty logicuid=%d, continue", userid, user.rolelist, user.logicuid);
				user.logicuid = 0;
				puser->insert(key, Marshal::OctetsStream() << user, txn);
				return true;
			}
#if 0
			ToDelList del_list;
			OverflowUsers::iterator it = overflow_users.find(userid), ite = overflow_users.end();
			/*
			if (it != ite && it->second.second == true)
			{
				LOG_TRACE("all roles of user %d need to be deleted, continue", userid);
				user.logicuid = 0;
				user.rolelist = 0;
				tab_dst_user.Put(key, Marshal::OctetsStream() << user);
				return true;
			}
			*/
			if (it != ite)
				del_list = it->second;
			ToDelList::const_iterator dit, dite = del_list.end();
#endif
			unsigned int roles = 0, logicuid = 0;
			Marshal::OctetsStream os_uidrole;
			if (tab_uidrole.Find(key, os_uidrole))
			{
				os_uidrole >> roles;
				if(os_uidrole.size()==8)
					os_uidrole >> logicuid;
				else
					logicuid = userid;
			}
			if (!logicuid)
			{
				logicuid = LogicuidAllocator::GetInstance()->AllocLogicuid();
				assert(logicuid > 0);
				Marshal::OctetsStream value;
				assert(!tab_logicuid.Find(Marshal::OctetsStream()<<logicuid, value));
			}
			tab_logicuid.Put(Marshal::OctetsStream()<<logicuid, key);
			RoleList uniq_rolelist(roles);
			if (!uniq_rolelist.IsRoleListInitialed())
				uniq_rolelist.InitialRoleList();

			int role = -1;
			RoleList new_rolelist;
			new_rolelist.InitialRoleList();
			while ((role = src_rolelist.GetNextRole()) >= 0 )
			{
				/*
				for (dit = del_list.begin(); dit != dite; ++dit)
				{
					if (dit->second == user.logicuid+role)
						break;
				}
				if (dit != dite)
				{
					LOG_TRACE("User %d roleid %d is deleting, ignore it",
							userid, user.logicuid+role);
					continue;
				}
				*/
				int newrole = -1;
				if ((newrole = uniq_rolelist.AddRole()) >= 0 )
				{
					new_rolelist.AddRole(newrole);
					g_mapRoleid[user.logicuid+role] = std::make_pair(logicuid+newrole, userid);
				}
				else
				{
					Log::log(LOG_ERR, "rolelist full.u=%d,logicuid1=%d,logicuid2=%d,r1=%x,r2=%x,discard=%d,uniq_count=%d.",
							userid, user.logicuid, logicuid, user.rolelist,
							roles, role, uniq_rolelist.GetRoleCount());
					//roleid_map[user.logicuid+role] = std::make_pair(0, userid);
				}
			}
			user.logicuid = logicuid;
			user.rolelist = new_rolelist.GetRoleList();
			//tab_dst_user.Put(key, Marshal::OctetsStream() << user);
			puser->insert(key, Marshal::OctetsStream() << user, txn);
			tab_uidrole.Put(key, Marshal::OctetsStream()<<uniq_rolelist.GetRoleList()<<logicuid);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: user data error, userid=%d",userid );
		}
		static size_t counter = 1;
		if ((counter%131072) == 0)
		{
			tab_uidrole.Checkpoint();
			tab_logicuid.Checkpoint();
		}
		Auto_Checkpoint(counter);
		return true;
	}
};

class MoveRolenameQuery : public StorageEnv::IQuery
{
	int zoneid;
	TableWrapper & tab_unamerole;
	TableWrapper & tab_mailbox;
	TableWrapper & tab_rolenamehis;
	TableWrapper & tab_base;
	TableWrapper & tab_duprolename;
public:
	int preallocate_hit;
	MoveRolenameQuery(int _zoneid, TableWrapper & _uname, TableWrapper & _mail, TableWrapper & _his, TableWrapper & _base, TableWrapper & _duprolename) : zoneid(_zoneid), tab_unamerole(_uname), tab_mailbox(_mail), tab_rolenamehis(_his), tab_base(_base), tab_duprolename(_duprolename), preallocate_hit(0) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldroleid = -1, newroleid = -1;
		try
		{
			StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
			Marshal::OctetsStream(value) >> oldroleid;
			newroleid = g_mapRoleid[oldroleid].first;

			Octets newname = key;
			Marshal::OctetsStream os_value, os_base;
			bool name_inuse = false; //当前正在使用的名字
			if (tab_base.Find(value, os_base))
			{
				GRoleBase base;
				os_base >> base;
				name_inuse = (key == base.name);
			}
//			time_t now = time(NULL);
			if (tab_unamerole.Find(key, os_value))
			{
				int tmpzoneid, tmproleid, tmpstatus, tmptime;
				os_value >> tmpzoneid >> tmproleid >> tmpstatus >> tmptime;
				if (tmpstatus == UNIQUENAME_PREALLOCATE)
				{
					preallocate_hit++;
					if (tmproleid != oldroleid)
						LOG_TRACE("preallocate hit! but roleid not match %d:%d", tmproleid, oldroleid);
				}
				else
				{
					tab_duprolename.Put(key, Marshal::OctetsStream()<<(int)0);
					dup_rolename++;

					char * suffix = "*";
					Octets suffix_l(suffix, strlen(suffix)), suffix_u;
					CharsetConverter::conv_charset_t2u(suffix_l, suffix_u);
					newname.insert(newname.end(), suffix_u.begin(), suffix_u.size());
					Marshal::OctetsStream tmp_value;
					int i = '*';
					if (tab_unamerole.Find(newname, tmp_value))
					{
						tmp_value >> tmpzoneid >> tmproleid >> tmpstatus >> tmptime;
						if (tmpstatus == UNIQUENAME_PREALLOCATE)
						{
							preallocate_hit++;
							if (tmproleid != oldroleid)
								LOG_TRACE("preallocate hit2! but roleid not match %d:%d", tmproleid, oldroleid);
						}
						else
						{
							tab_duprolename.Put(newname, Marshal::OctetsStream()<<(int)0);
							i = 'a';
							while(i < 'z')
							{
								Octets testname = newname;
								char buffer[64];
								sprintf(buffer, "%c", i);
								Octets sub_suffix_l(buffer,strlen(buffer)), sub_suffix_u;
								CharsetConverter::conv_charset_t2u(sub_suffix_l, sub_suffix_u);
								testname.insert(testname.end(), sub_suffix_u.begin(), sub_suffix_u.size());
								tmp_value.clear();
								if (!tab_unamerole.Find(testname, tmp_value))
								{
									newname = testname;
									break;
								}
								else
								{
									tmp_value >> tmpzoneid >> tmproleid >> tmpstatus >> tmptime;
									if (tmpstatus == UNIQUENAME_PREALLOCATE)
									{
										preallocate_hit++;
										if (tmproleid != oldroleid)
											LOG_TRACE("preallocate hit3! but roleid not match %d:%d", tmproleid, oldroleid);
										newname = testname;
										break;
									}
									else
										tab_duprolename.Put(testname, Marshal::OctetsStream()<<(int)0);
								}
								i++;
							}
							assert(i < 'z');
						}
					}
					if (i != '*')
						LOG_TRACE("rolename dup, oldid %d newid %d, add suffix %c success", oldroleid, newroleid, i);
				}
			}
			g_mapRolename.Insert(key, newname);
			if (name_inuse && newname != key) //正在使用的名字被修改 才需要发放道具
			{
				if (oldroleid == 0)
					Log::log(LOG_ERR, "MoveRolenameQuery oldroleid is 0");
				else
				{
					Marshal::OctetsStream krole, value_mail;
					krole << oldroleid;
					/*
					Marshal::OctetsStream value_rolenamehis
					GRoleNameHisVector rolenamehis_vec;
					if (tab_rolenamehis.Find(krole, value_rolenamehis))
						value_rolenamehis >> rolenamehis_vec;
					rolenamehis_vec.push_back(GRoleNameHis(key, now));
					tab_rolenamehis.Put(krole, Marshal::OctetsStream() << rolenamehis_vec);
					*/
					//发放改名道具
					GMailBox box;
					if (tab_mailbox.Find(krole, value_mail))
						value_mail >> box;
					if (MailSender::DBSendObj(box, g_gifttype[CHANGE_ROLENAME_ITEMID][0],
									g_gifttype[CHANGE_ROLENAME_ITEMID][1],
									g_gifttype[CHANGE_ROLENAME_ITEMID][2],
									1, _MST_GAMEDB))
						tab_mailbox.Put(krole, Marshal::OctetsStream() << box);
					else
						Log::log(LOG_ERR, "role %d rolename dup, but mailbox full, newroleid %d", oldroleid, newroleid);
				}
			}

			Marshal::OctetsStream value_uname;
			int status = name_inuse ? UNIQUENAME_USED : UNIQUENAME_OBSOLETE; 
			value_uname << zoneid << newroleid << status << (int)time(NULL);
			tab_unamerole.Put(newname,  value_uname);
			prolename->insert(newname, Marshal::OctetsStream()<<newroleid, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: MoveRolenameQuery error, oldroleid=%d", oldroleid);
		}
		static size_t counter = 1;
		if ((counter%131072) == 0)
		{
			tab_unamerole.Checkpoint();
			tab_mailbox.Checkpoint();
			tab_rolenamehis.Checkpoint();
			tab_base.Checkpoint();
			tab_duprolename.Checkpoint();
/*
			tab_unamerole.ReOpen();
			tab_mailbox.ReOpen();
			tab_rolenamehis.ReOpen();
			tab_base.ReOpen();
			*/
		}
		Auto_Checkpoint(counter);
		return true;
	}
};

class MoveFamilynameQuery : public StorageEnv::IQuery
{
	int zoneid;
	TableWrapper & tab_unamefamily;
	StorageEnv::Storage * pfamily;
	StorageEnv::Storage * pfamilyname;
public:
	int preallocate_hit;
	MoveFamilynameQuery(int _zoneid, TableWrapper & _uname, StorageEnv::Storage * _pfamily, StorageEnv::Storage * _pfname) : zoneid(_zoneid), tab_unamefamily(_uname), pfamily(_pfamily), pfamilyname(_pfname), preallocate_hit(0) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldfid = -1, newfid = -1;
		try
		{
			Marshal::OctetsStream(value) >> oldfid;
			newfid = g_mapFamilyid[oldfid];

			Octets oldname;
			Marshal::OctetsStream(key) >> oldname;
			Octets newname = oldname;

			Marshal::OctetsStream os_value;
			GFamily info;
			Marshal::OctetsStream(pfamily->find(Marshal::OctetsStream()<<newfid, txn)) >> info;
			bool name_inuse = (info.name == oldname);
			if (tab_unamefamily.Find(oldname, os_value))
			{
				int tmpzoneid, tmpid, tmpstatus, tmptime;
				os_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
				if (tmpstatus == UNIQUENAME_PREALLOCATE)
				{
					preallocate_hit++;
					if (tmpid != oldfid)
						LOG_TRACE("preallocate hit! but familyid not match %d:%d", tmpid, oldfid);
				}
				else
				{
					dup_familyname++;

					char * suffix = "*";
					Octets suffix_l(suffix, strlen(suffix)), suffix_u;
					CharsetConverter::conv_charset_t2u(suffix_l, suffix_u);
					newname.insert(newname.end(), suffix_u.begin(), suffix_u.size());
					Marshal::OctetsStream tmp_value;
					int i = '*';
					if (tab_unamefamily.Find(newname, tmp_value))
					{
						tmp_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
						if (tmpstatus == UNIQUENAME_PREALLOCATE)
						{
							preallocate_hit++;
							if (tmpid != oldfid)
								LOG_TRACE("preallocate hit2! but familyid not match %d:%d", tmpid, oldfid);
						}
						else
						{
							i = 'a';
							while(i < 'z')
							{
								Octets testname = newname;
								char buffer[64];
								sprintf(buffer, "%c", i);
								Octets sub_suffix_l(buffer,strlen(buffer)), sub_suffix_u;
								CharsetConverter::conv_charset_t2u(sub_suffix_l, sub_suffix_u);
								testname.insert(testname.end(), sub_suffix_u.begin(), sub_suffix_u.size());
								tmp_value.clear();
								if (!tab_unamefamily.Find(testname, tmp_value))
								{
									newname = testname;
									break;
								}
								else
								{
									tmp_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
									if (tmpstatus == UNIQUENAME_PREALLOCATE)
									{
										preallocate_hit++;
										if (tmpid != oldfid)
											LOG_TRACE("preallocate hit2! but familyid not match %d:%d", tmpid, oldfid);
										newname = testname;
										break;
									}
								}
								i++;
							}
							assert(i < 'z');
						}
					}
					if (i != '*')
						LOG_TRACE("familyname dup, oldid %d newid %d, add suffix %c success", oldfid, newfid, i);
				}
			}
			g_mapFamilyname.Insert(oldname, newname);
			if (name_inuse)
			{
				info.name = newname;
				info.factionid = g_mapFactionid[info.factionid];
				pfamily->insert(Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info, txn);
				if (info.master != 0 && newname != oldname)
					g_luckyroles[info.master][CHANGE_FAMILYNAME_ITEMID] += 1;
			}
			Marshal::OctetsStream value_uname;
			int status = name_inuse ? UNIQUENAME_USED : UNIQUENAME_OBSOLETE; 
			value_uname << zoneid << newfid << status << (int)time(NULL);
			tab_unamefamily.Put(newname, value_uname);
			pfamilyname->insert(Marshal::OctetsStream() << newname, Marshal::OctetsStream()<<newfid, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: MoveFamilynameQuery error, oldfid=%d", oldfid);
		}
		return true;
	}
};

class MoveFactionnameQuery : public StorageEnv::IQuery
{
	int zoneid;
	TableWrapper & tab_unamefaction;
	StorageEnv::Storage * pfaction;
	StorageEnv::Storage * pfactionname;
public:
	int preallocate_hit;
	MoveFactionnameQuery(int _zoneid, TableWrapper & _uname, StorageEnv::Storage * _pfaction, StorageEnv::Storage * _pfname) : zoneid(_zoneid), tab_unamefaction(_uname), pfaction(_pfaction), pfactionname(_pfname), preallocate_hit(0) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldfid = -1, newfid = -1;
		try
		{
			Marshal::OctetsStream(value) >> oldfid;
			newfid = g_mapFactionid[oldfid];

			Octets oldname;
			Marshal::OctetsStream(key) >> oldname;
			Octets newname = oldname;

			Marshal::OctetsStream os_value;
			GFactionInfo info;
			Marshal::OctetsStream(pfaction->find(Marshal::OctetsStream()<<newfid, txn)) >> info;
			bool name_inuse = (info.name == oldname);
			if (tab_unamefaction.Find(oldname, os_value))
			{
				int tmpzoneid, tmpid, tmpstatus, tmptime;
				os_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
				if (tmpstatus == UNIQUENAME_PREALLOCATE)
				{
					preallocate_hit++;
					if (tmpid != oldfid)
						LOG_TRACE("preallocate hit! but factioinid not match %d:%d", tmpid, oldfid);
				}
				else
				{
					dup_factionname++;

					char * suffix = "*";
					Octets suffix_l(suffix, strlen(suffix)), suffix_u;
					CharsetConverter::conv_charset_t2u(suffix_l, suffix_u);
					newname.insert(newname.end(), suffix_u.begin(), suffix_u.size());
					Marshal::OctetsStream tmp_value;
					int i = '*';
					if (tab_unamefaction.Find(newname, tmp_value))
					{
						tmp_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
						if (tmpstatus == UNIQUENAME_PREALLOCATE)
						{
							preallocate_hit++;
							if (tmpid != oldfid)
								LOG_TRACE("preallocate hit2! but faction not match %d:%d", tmpid, oldfid);
						}
						else
						{
							i = 'a';
							while(i < 'z')
							{
								Octets testname = newname;
								char buffer[64];
								sprintf(buffer, "%c", i);
								Octets sub_suffix_l(buffer,strlen(buffer)), sub_suffix_u;
								CharsetConverter::conv_charset_t2u(sub_suffix_l, sub_suffix_u);
								testname.insert(testname.end(), sub_suffix_u.begin(), sub_suffix_u.size());
								tmp_value.clear();
								if (!tab_unamefaction.Find(testname, tmp_value))
								{
									newname = testname;
									break;
								}
								else
								{
									tmp_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
									if (tmpstatus == UNIQUENAME_PREALLOCATE)
									{
										preallocate_hit++;
										if (tmpid != oldfid)
											LOG_TRACE("preallocate hit2! but familyid not match %d:%d", tmpid, oldfid);
										newname = testname;
										break;
									}
								}
								i++;
							}
							assert(i < 'z');
						}
					}
					if (i != '*')
						LOG_TRACE("factionname dup, oldid %d newid %d, add suffix %c success", oldfid, newfid, i);
				}
			}
			g_mapFactionname.Insert(oldname, newname);
			if (name_inuse)
			{
				info.name = newname;
				for( FamilyIdVector::iterator it = info.member.begin(), ite = info.member.end(); it != ite; ++it )
					it->fid = g_mapFamilyid[it->fid];
				info.hostileinfo.clear();
				pfaction->insert(Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info, txn);
				if (info.master != 0 && newname != oldname)
					g_luckyroles[info.master][CHANGE_FACTIONNAME_ITEMID] += 1;
			}
			Marshal::OctetsStream value_uname;
			int status = name_inuse ? UNIQUENAME_USED : UNIQUENAME_OBSOLETE; 
			value_uname << zoneid << newfid << status << (int)time(NULL);
			tab_unamefaction.Put(newname, value_uname);
			pfactionname->insert(Marshal::OctetsStream() << newname, Marshal::OctetsStream()<<newfid, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: MoveFactionnameQuery error, oldfid=%d", oldfid);
		}
		return true;
	}
};

class MoveFamilyQuery : public StorageEnv::IQuery
{
public:
	int nextid;
	StorageEnv::Storage * pfamily;
	MoveFamilyQuery() {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid = -1, newfid = -1;
		try
		{
			GFamily info;
			Marshal::OctetsStream(key) >> fid;
			Marshal::OctetsStream(value) >> info;
			newfid = nextid++;

			g_mapFamilyid[fid] = newfid;
			info.id = newfid;
			info.master = g_mapRoleid[info.master].first;
			for( GFolkVector::iterator it = info.member.begin(), ite = info.member.end(); it != ite; ++it )
			{
				it->rid = g_mapRoleid[it->rid].first;
				it->name = g_mapRolename.Find(it->name);
			}
			pfamily->insert(Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: family data error, familyid=%d", fid);
		}
		return true;
	}
};

class MoveFactionQuery : public StorageEnv::IQuery
{
public:
	int nextid;
	StorageEnv::Storage * pfaction;
	MoveFactionQuery() {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid = -1, newfid = -1;
		try
		{
			GFactionInfo info;
			Marshal::OctetsStream(key) >> fid;
			Marshal::OctetsStream(value) >> info;
			newfid = nextid++;

			g_mapFactionid[fid] = newfid;
			info.fid = newfid;
			info.master = g_mapRoleid[info.master].first;
			pfaction->insert(Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: faction data error, factionid=%d", fid);
		}
		return true;
	}
};

class MoveCrssvrTeamsnameQuery : public StorageEnv::IQuery
{
	int zoneid;
	TableWrapper & tab_unamecrssvrteams;
	StorageEnv::Storage * pcrssvrteams;
	StorageEnv::Storage * pcrssvrteamsname;
public:
	int preallocate_hit;
	MoveCrssvrTeamsnameQuery(int _zoneid, TableWrapper & _uname, StorageEnv::Storage * _pcrssvrteams, StorageEnv::Storage * _pcrssvrteamsname) : zoneid(_zoneid), tab_unamecrssvrteams(_uname), pcrssvrteams(_pcrssvrteams), pcrssvrteamsname(_pcrssvrteamsname), preallocate_hit(0) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int oldfid = -1, newfid = -1;
		try
		{
			Marshal::OctetsStream(value) >> oldfid;
			newfid = g_mapCrssvrTeamsid[oldfid];

			Octets oldname;
			Marshal::OctetsStream(key) >> oldname;
			Octets newname = oldname;

			Marshal::OctetsStream os_value;
			GCrssvrTeamsData info;
			Marshal::OctetsStream(pcrssvrteams->find(Marshal::OctetsStream()<<newfid, txn)) >> info;
			bool name_inuse = (info.info.name == oldname);
			if (tab_unamecrssvrteams.Find(oldname, os_value))
			{
				int tmpzoneid, tmpid, tmpstatus, tmptime;
				os_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
				if (tmpstatus == UNIQUENAME_PREALLOCATE)
				{
					preallocate_hit++;
					if (tmpid != oldfid)
						LOG_TRACE("preallocate hit! but crssvrteamsid not match %d:%d", tmpid, oldfid);
				}
				else
				{
					dup_crssvrteamsname++;

					char * suffix = "*";
					Octets suffix_l(suffix, strlen(suffix)), suffix_u;
					CharsetConverter::conv_charset_t2u(suffix_l, suffix_u);
					newname.insert(newname.end(), suffix_u.begin(), suffix_u.size());
					Marshal::OctetsStream tmp_value;
					int i = '*';
					if (tab_unamecrssvrteams.Find(newname, tmp_value))
					{
						tmp_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
						if (tmpstatus == UNIQUENAME_PREALLOCATE)
						{
							preallocate_hit++;
							if (tmpid != oldfid)
								LOG_TRACE("preallocate hit2! but crssvrteamsid not match %d:%d", tmpid, oldfid);
						}
						else
						{
							i = 'a';
							while(i < 'z')
							{
								Octets testname = newname;
								char buffer[64];
								sprintf(buffer, "%c", i);
								Octets sub_suffix_l(buffer,strlen(buffer)), sub_suffix_u;
								CharsetConverter::conv_charset_t2u(sub_suffix_l, sub_suffix_u);
								testname.insert(testname.end(), sub_suffix_u.begin(), sub_suffix_u.size());
								tmp_value.clear();
								if (!tab_unamecrssvrteams.Find(testname, tmp_value))
								{
									newname = testname;
									break;
								}
								else
								{
									tmp_value >> tmpzoneid >> tmpid >> tmpstatus >> tmptime;
									if (tmpstatus == UNIQUENAME_PREALLOCATE)
									{
										preallocate_hit++;
										if (tmpid != oldfid)
											LOG_TRACE("preallocate hit2! but crssvrteamsid not match %d:%d", tmpid, oldfid);
										newname = testname;
										break;
									}
								}
								i++;
							}
							assert(i < 'z');
						}
					}
					if (i != '*')
						LOG_TRACE("crssvrteamsname dup, oldid %d newid %d, add suffix %c success", oldfid, newfid, i);
				}
			}
			g_mapCrssvrTeamsname.Insert(oldname, newname);
			if (name_inuse)
			{
				info.info.name = newname;
				pcrssvrteams->insert(Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info, txn);
			}
			Marshal::OctetsStream value_uname;
			int status = name_inuse ? UNIQUENAME_USED : UNIQUENAME_OBSOLETE; 
			value_uname << zoneid << newfid << status << (int)time(NULL);
			tab_unamecrssvrteams.Put(newname, value_uname);
			pcrssvrteamsname->insert(Marshal::OctetsStream() << newname, Marshal::OctetsStream()<<newfid, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: MoveCrssvrTeamsnameQuery error, oldfid=%d", oldfid);
		}
		return true;
	}
};

class MoveCrssvrTeamsQuery : public StorageEnv::IQuery
{
public:
	int nextid;
	StorageEnv::Storage * pcrssvrteams;
	MoveCrssvrTeamsQuery() {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int fid = -1, newfid = -1;
		try
		{
			GCrssvrTeamsData info;
			Marshal::OctetsStream(key) >> fid;
			LOG_TRACE("MoveCrssvrTeamsQuery fid=%d", fid);
			if(fid <= 0) return true;
			Marshal::OctetsStream(value) >> info;
			newfid = nextid++;

			g_mapCrssvrTeamsid[fid] = newfid;
			info.info.teamid = newfid;
			info.info.master = g_mapRoleid[info.info.master].first;
			for( std::vector<GCrssvrTeamsRole>::iterator it = info.roles.begin(), ite = info.roles.end(); it != ite; ++it )
			{
				it->roleid = g_mapRoleid[it->roleid].first;
				it->name = g_mapRolename.Find(it->name);
			}
			pcrssvrteams->insert(Marshal::OctetsStream()<<newfid, Marshal::OctetsStream()<<info, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "ERROR: crssvrteams data error, teamsid=%d", fid);
		}
		return true;
	}
};


struct DelKey   
{                       
	bool deleting;  
	bool selling; //角色是否处于寄售状态
	int level_rank;      
	bool isheader;          
	int lastlogin_rank;
	int true_level;
	DelKey(bool _d = false, bool _s = false, int _lev_rank = 0, bool _h = false, int _time_rank = 0, int _lev = 0) : deleting(_d), selling(_s), level_rank(_lev_rank), isheader(_h), lastlogin_rank(_time_rank), true_level(_lev) {}
	bool operator < (const DelKey &rhs) const
	{                               
		return deleting < rhs.deleting ||
			deleting == rhs.deleting &&
			(selling > rhs.selling ||
			 (selling == rhs.selling &&
			  (level_rank > rhs.level_rank ||
			   (level_rank == rhs.level_rank &&
			    (isheader > rhs.isheader ||
			     (isheader == rhs.isheader &&
			      (lastlogin_rank > rhs.lastlogin_rank ||
			       lastlogin_rank == rhs.lastlogin_rank &&
			       true_level > rhs.true_level)))))));
	}
};
typedef std::multimap<DelKey, int> ToDelList;
typedef std::map<int/*user*/, ToDelList> OverflowUsers;
class GetOverflowUserQuery : public StorageEnv::IQuery
{
	//OverflowUsers & overflow_users;
	TableWrapper & tab_uidrole;
	TableWrapper & tab_base;
	TableWrapper & tab_status;
public:
	std::map<int, int> overflow_ids;
	GetOverflowUserQuery(TableWrapper & _uidrole, TableWrapper & _base, TableWrapper & _status) : tab_uidrole(_uidrole), tab_base(_base), tab_status(_status) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int userid = -1;
		try
		{
			Marshal::OctetsStream(key) >> userid;
			if (userid < 128)
				return true;
			User user;
			Marshal::OctetsStream(value) >> user;
			RoleList src_rolelist(user.rolelist);
			int src_count = src_rolelist.GetRoleCount();
			if (src_count <= 0)
				return true;
			Marshal::OctetsStream os_uidrole;
			if (!tab_uidrole.Find(key, os_uidrole))
				return true;
			unsigned int roles = 0;
			os_uidrole >> roles;
			RoleList uniq_rolelist(roles);
			int uniq_count = uniq_rolelist.GetRoleCount();
			if (src_count + uniq_count <= MAX_ROLE_COUNT)
				return true;
			ToDelList list;
			int r = -1;
//			int reserved_count = 0;
			time_t now = time(NULL);
			while( (r = src_rolelist.GetNextRole()) >= 0 )
			{
				int roleid = user.logicuid + r;
				Marshal::OctetsStream key_role, os_base, os_status;
				key_role << roleid;
				if (!tab_base.Find(key_role, os_base))
				{
					Log::log(LOG_ERR, "user %d can not find role %d src base", userid, roleid);
//					DelKey (deleting, selling , level_rank, isheader, login_rank, true_level)
					list.insert(std::make_pair(DelKey(true, false, 0, false, 0, 0), roleid));
					continue;
				}
				if (!tab_status.Find(key_role, os_status))
				{
					Log::log(LOG_ERR, "user %d can not find role %d src status", userid, roleid);
					list.insert(std::make_pair(DelKey(true, false, 0, false, 0, 0), roleid));
					continue;
				}
				GRoleBase base;
				GRoleStatus status;
				try 
				{
					os_base >> base;
					os_status >> status;
				}
				catch ( Marshal::Exception e )
				{
					Log::log( LOG_ERR, "GetOverflowUserQuery, exception role %d of user %d in srcdb\n", roleid, userid);
					list.insert(std::make_pair(DelKey(true, false, 0, false, 0, 0), roleid));
					continue;
				}
				int true_level = status.level+status.reborndata.size()/sizeof(int)*200;
				int level_rank = 0;
				if (true_level <= 90)
					level_rank = 1;
				else
					level_rank = 2;
				int login_rank = 0;
				int cold_time = now - base.lastlogin_time;
				if (cold_time > 3600*24*365)
					login_rank = 1;
				else if (cold_time > 3600*24*90)
					login_rank = 2;
				else
					login_rank = 3;
				list.insert(std::make_pair(DelKey(base.status==_ROLE_STATUS_READYDEL, base.status==_ROLE_STATUS_SELLING,
								level_rank, base.title>=TITLE_MASTER&&base.title<=TITLE_HEADER, login_rank, true_level), roleid));
			}
			LOG_TRACE("Overflow user %d", userid);
			ToDelList::iterator it, ite = list.end();
			for (it = list.begin(); it != ite; ++it)
			{
				LOG_TRACE("\troleid %d : <deleting %d selling %d level_rank %d isheader %d login_rank %d true_level %d>",
						it->second, it->first.deleting, it->first.selling, it->first.level_rank, it->first.isheader, it->first.lastlogin_rank, it->first.true_level);
			}
			assert((int)list.size() > MAX_ROLE_COUNT-uniq_count);
			it = list.begin();
			advance(it, MAX_ROLE_COUNT-uniq_count);
			list.erase(list.begin(), it);
			LOG_TRACE("User %d to del list: src_count %d uniq_count %d", userid, src_count, uniq_count);
			for (it = list.begin(); it != list.end(); ++it)
			{
				
				overflow_ids.insert(std::make_pair(it->second, userid));
				LOG_TRACE("\tdelroleid %d userid %d: <deleting %d selling %d level_rank %d isheader %d login_rank %d true_level %d>",
						it->second, userid, it->first.deleting, it->first.selling, it->first.level_rank, it->first.isheader, it->first.lastlogin_rank, it->first.true_level);
			}
			//overflow_users.insert(make_pair(usernid, make_pair(list, MAX_ROLE_COUNT==uniq_count)));
			//overflow_users.insert(std::make_pair(userid, list));
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "GetOverflowUserQuery, exception user %d\n", userid);
//			throw e;
		}
		return true;
	}
};

void MoveUser(int zoneid, TableWrapper & tab_user, TableWrapper & tab_uidrole, TableWrapper & tab_logicuid, TableWrapper & tab_base, TableWrapper & tab_status,
		TableWrapper & tab_base2, TableWrapper & tab_inventory, TableWrapper & tab_storehouse, TableWrapper & tab_task,
		TableWrapper & tab_achieve, TableWrapper & tab_friends, TableWrapper & tab_rolename, TableWrapper & tab_rolenamehis, TableWrapper & tab_award,
		TableWrapper & tab_family, TableWrapper & tab_crssvrteams, TableWrapper & tab_callback, TableWrapper & tab_referral, TableWrapper & tab_gt, TableWrapper & tab_waitdel,
		TableWrapper & tab_home, TableWrapper & tab_snsplayer, TableWrapper & tab_mail)

{
	LOG_TRACE( "GetOverflowUsers: begin" );
	PreLoadTable((StorageEnv::get_datadir()+"/user").c_str());
	PreLoadTable(tab_user.name.c_str());
	GetOverflowUserQuery q(tab_uidrole, tab_base, tab_status);
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_user.name.c_str(), tab_user.ptable, tab_user.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkUser, error when walk, what=%s\n", e.what() );
	}
	LOG_TRACE( "GetOverflowUsers: role count %d", q.overflow_ids.size());

	_DelPlayers(q.overflow_ids, tab_user, tab_base, tab_base2, tab_status, tab_inventory, tab_storehouse,
			tab_task, tab_achieve, tab_friends, tab_rolename, tab_rolenamehis, tab_award,
			tab_family, tab_crssvrteams, tab_callback, tab_referral, tab_gt, tab_waitdel, tab_home, tab_snsplayer, tab_mail);
	MoveUserQuery q2(tab_uidrole, tab_logicuid);
	try
	{
//		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
//		q2.puser = puser;
		StorageEnv::AtomTransaction	txn;

		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_user.name.c_str(), tab_user.ptable, tab_user.uncompressor);
			cursor.walk( q2 );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkUser2, error when walk, what=%s\n", e.what() );
	}
	tab_uidrole.Checkpoint();
	tab_logicuid.Checkpoint();

	char buf[32];
	snprintf(buf, sizeof(buf), "roleidmap.%d", zoneid);
	FILE *fp = fopen(buf, "w+");
	if (fp == NULL)
		fp = stderr;
	fprintf(fp, "<oldroleid,userid>->newroleid\n");
	MapRoleId::const_iterator it, ite = g_mapRoleid.end();
	for (it = g_mapRoleid.begin(); it != ite; ++it)
		fprintf(fp, "<%d,%d>->%d\n", it->first, it->second.second, it->second.first);
	if (fp != stderr)
		fclose(fp);
	LOG_TRACE("setUserNeedSetReferrer.size %d", q2.setUserNeedSetReferrer.size());
	try
	{
		StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
		StorageEnv::CommonTransaction txn;
		try
		{
			std::vector<int>::iterator it = q2.setUserNeedSetReferrer.begin(), ie = q2.setUserNeedSetReferrer.end();
			for (; it != ie; ++it)
			{
				Marshal::OctetsStream key, value;
				key << *it;
				if (puser->find(key, value, txn))
				{
					User user;
					value >> user;
					if (user.referrer!=0)// && g_mapRoleid.find(user.referrer)!=g_mapRoleid.end())
					{
						int oldreferrer = user.referrer;
						user.referrer = g_mapRoleid[oldreferrer].first;
						if (oldreferrer != user.referrer)
						{
							puser->insert(key, Marshal::OctetsStream()<<user, txn);
							//Log::log(LOG_INFO, "Referrer of user %d is changed from %d to %d\n", *it, oldreferrer, user.referrer);
						}
					}
				}
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MoveUser, error when modify referrer, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void MoveRolename(int zoneid, TableWrapper & tab_unamerole, TableWrapper & tab_rolename, TableWrapper & tab_mailbox, TableWrapper & tab_rolenamehis, TableWrapper & tab_base, TableWrapper & tab_duprolename)
{
	LOG_TRACE( "Move rolename:" );
	PreLoadTable((StorageEnv::get_datadir()+"/rolename").c_str());
	PreLoadTable(tab_rolename.name.c_str());
	try
	{
//		StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
		StorageEnv::AtomTransaction	txn;

		MoveRolenameQuery q(zoneid, tab_unamerole, tab_mailbox, tab_rolenamehis, tab_base, tab_duprolename);
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_rolename.name.c_str(), tab_rolename.ptable, tab_rolename.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
		LOG_TRACE("preallocate hit %d", q.preallocate_hit);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkRolename, error when walk, what=%s\n", e.what() );
	}
	LOG_TRACE("duprolename count %d", tab_duprolename.Count());
}

void MoveFamilyName(int zoneid, TableWrapper & tab_unamefamily, TableWrapper & tab_familyname)
{
	LOG_TRACE( "Move familyname:" );
	try
	{
		StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
		StorageEnv::Storage * pfamilyname = StorageEnv::GetStorage("familyname");
		StorageEnv::AtomTransaction	txn;
		MoveFamilynameQuery q(zoneid, tab_unamefamily, pfamily, pfamilyname);
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_familyname.name.c_str(), tab_familyname.ptable, tab_familyname.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
		LOG_TRACE("preallocate hit %d", q.preallocate_hit);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkFamilyname, error when walk, what=%s\n", e.what() );
	}
}

void MoveFactionName(int zoneid, TableWrapper & tab_unamefaction, TableWrapper & tab_factionname)
{
	LOG_TRACE( "Move factionname:" );
	try
	{
		StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
		StorageEnv::Storage * pfactionname = StorageEnv::GetStorage("factionname");
		StorageEnv::AtomTransaction	txn;
		MoveFactionnameQuery q(zoneid, tab_unamefaction, pfaction, pfactionname);
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_factionname.name.c_str(), tab_factionname.ptable, tab_factionname.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
		LOG_TRACE("preallocate hit %d", q.preallocate_hit);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkFactionname, error when walk, what=%s\n", e.what() );
	}
}

void MoveCrssvrTeamsName(int zoneid, TableWrapper & tab_unamecrssvrteams, TableWrapper & tab_crssvrteamsname)
{
	LOG_TRACE( "Move crssvrteamsname:" );
	try
	{
		StorageEnv::Storage * pcrssvrteams = StorageEnv::GetStorage("crssvrteams");
		StorageEnv::Storage * pcrssvrteamsname = StorageEnv::GetStorage("crssvrteamsname");
		StorageEnv::AtomTransaction	txn;
		MoveCrssvrTeamsnameQuery q(zoneid, tab_unamecrssvrteams, pcrssvrteams, pcrssvrteamsname);
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_crssvrteamsname.name.c_str(), tab_crssvrteamsname.ptable, tab_crssvrteamsname.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
		LOG_TRACE("preallocate hit %d", q.preallocate_hit);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkCrssvrteamsname, error when walk, what=%s\n", e.what() );
	}
}

void MoveFamily(TableWrapper & tab_family, int zoneid)
{
	LOG_TRACE( "Move family:" );
	MoveFamilyQuery q;
	q.nextid = LogicuidAllocator::GetInstance()->GetNextFamilyId();
	try
	{
		StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
		q.pfamily = pfamily;
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_family.name.c_str(), tab_family.ptable, tab_family.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkFamily, error when walk, what=%s\n", e.what() );
	}
	LogicuidAllocator::GetInstance()->SaveNextFamilyId(q.nextid);

	char buf[32];
	snprintf(buf, sizeof(buf), "familyidmap.%d", zoneid);
	FILE *fp = fopen(buf, "w+");
	if (fp == NULL)
		fp = stderr;
	fprintf(fp, "oldfamilyid->newfamilyid\n");
	std::map<int, int>::const_iterator it, ite = g_mapFamilyid.end();
	for (it = g_mapFamilyid.begin(); it != ite; ++it)
		fprintf(fp, "%d->%d\n", it->first, it->second);
	if (fp != stderr)
		fclose(fp);
}

void MoveFaction(TableWrapper & tab_faction, int zoneid)
{
	LOG_TRACE( "Move faction:" );
	MoveFactionQuery q;
	q.nextid = LogicuidAllocator::GetInstance()->GetNextFactionId();
	try
	{
		StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
		q.pfaction = pfaction;
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_faction.name.c_str(), tab_faction.ptable, tab_faction.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkFaction, error when walk, what=%s\n", e.what() );
	}
	LogicuidAllocator::GetInstance()->SaveNextFactionId(q.nextid);

	char buf[32];
	snprintf(buf, sizeof(buf), "factionidmap.%d", zoneid);
	FILE *fp = fopen(buf, "w+");
	if (fp == NULL)
		fp = stderr;
	fprintf(fp, "oldfactionid->newfactionid\n");
	std::map<int, int>::const_iterator it, ite = g_mapFactionid.end();
	for (it = g_mapFactionid.begin(); it != ite; ++it)
		fprintf(fp, "%d->%d\n", it->first, it->second);
	if (fp != stderr)
		fclose(fp);
}

void MoveCrssvrTeams(TableWrapper & tab_crssvrteams, int zoneid)
{
	LOG_TRACE( "Move crssvrteams:" );
	MoveCrssvrTeamsQuery q;
	q.nextid = LogicuidAllocator::GetInstance()->GetNextCrssvrTeamsId();
	try
	{
		StorageEnv::Storage * pcrssvrteams = StorageEnv::GetStorage("crssvrteams");
		q.pcrssvrteams = pcrssvrteams;
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_crssvrteams.name.c_str(), tab_crssvrteams.ptable, tab_crssvrteams.uncompressor);
			cursor.walk( q );

			try{
				Marshal::OctetsStream kteamid, value;
				GCrssvrTeamsSeasonTop seasontoptable;
				kteamid << 0;
				
				if( pcrssvrteams->find(kteamid, value, txn) )
				{
					value >> seasontoptable;

					GTopTable &toptable = seasontoptable.table;
					for(std::vector<GTopItem>::iterator it1 = toptable.items.begin(),
						ie1 = toptable.items.end(); it1 != ie1; ++it1)
					{
						GTopItem &topitem = (*it1);
						topitem.id = g_mapCrssvrTeamsid[topitem.id];
						topitem.name = g_mapFamilyname.Find(topitem.name);
					}
					
					for(std::vector<GTopDetail>::iterator it2 = toptable.items_detail.begin();
						it2 != toptable.items_detail.end();)
					{
						GTopDetail &topdetail = (*it2);
						try
						{		
							Marshal::OctetsStream key;
							topdetail.id = g_mapCrssvrTeamsid[topdetail.id];
							key << topdetail.id;
							GCrssvrTeamsData finfo;

							Marshal::OctetsStream(pcrssvrteams->find( key, txn )) >> finfo;
							std::vector<GCrssvrTeamsRole>& roles = finfo.roles;
							Marshal::OctetsStream top_os;
							top_os << roles;
							topdetail.content = top_os;

							++it2;
						}
						catch( ... )
						{
							printf("error here, topdetail.id=%d\n", topdetail.id);
							it2 = toptable.items_detail.erase(it2);
						}

					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException e( DB_OLD_VERSION );
				txn.abort( e );
				throw e;
			}

		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "WalkCrssvrTeams, error when walk, what=%s\n", e.what() );
	}
	LogicuidAllocator::GetInstance()->SaveNextCrssvrTeamsId(q.nextid);

	char buf[32];
	snprintf(buf, sizeof(buf), "crssvrteamsidmap.%d", zoneid);
	FILE *fp = fopen(buf, "w+");
	if (fp == NULL)
		fp = stderr;
	fprintf(fp, "oldcrssvrteamsid->newcrssvrteamsid\n");
	std::map<int, int>::const_iterator it, ite = g_mapCrssvrTeamsid.end();
	for (it = g_mapCrssvrTeamsid.begin(); it != ite; ++it)
		fprintf(fp, "%d->%d\n", it->first, it->second);
	if (fp != stderr)
		fclose(fp);
}


static void SendGifts()
{
	LOG_TRACE("g_luckyroles.size %d", g_luckyroles.size());
	try
	{
		StorageEnv::Storage * pmail = StorageEnv::GetStorage("mailbox");
		StorageEnv::CommonTransaction txn;
		try
		{
			std::map<int, std::map<int, int> >::const_iterator it, ite = g_luckyroles.end();
			for (it = g_luckyroles.begin(); it != ite; ++it)
			{
				Marshal::OctetsStream key, value;
				GMailBox box;
				key << it->first;
				if (pmail->find(key, value, txn))
					value >> box;
				std::map<int, int>::const_iterator iit, iite = it->second.end();
				for (iit = it->second.begin(); iit != iite; ++iit)
				{
					if (iit->second > 1)
						Log::log(LOG_ERR, "send role %d gift %d count %d great than 1", it->first, iit->first, iit->second);
					if (!MailSender::DBSendObj(box, g_gifttype[iit->first][0],
									 g_gifttype[iit->first][1],
									 g_gifttype[iit->first][2],
									 iit->second,_MST_GAMEDB))
						Log::log(LOG_ERR, "send role %d gift %d count %d, but mailbox full", it->first, iit->first, iit->second);
				}
				pmail->insert(key, Marshal::OctetsStream()<<box, txn);
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "SendGift, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

class MoveTableQuery : public StorageEnv::IQuery
{
public:
	std::string	tablename;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( tablename.c_str() );
			pstorage->insert(key, value, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: data error");
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void MoveTable(TableWrapper & table, const char * tablename )
{
	LOG_TRACE( "Move %s:", tablename );

//	PreLoadTable((StorageEnv::get_datadir()+"/"+tablename).c_str());
//	PreLoadTable(tab_role.name.c_str());
	MoveTableQuery q;
	q.tablename = tablename;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, table.name.c_str(), table.ptable, table.uncompressor);
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "MoveTable, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void MoveCity(TableWrapper & table_city)
{
	LOG_TRACE("Move city:");
	GCityStore city_store;
	Marshal::OctetsStream key_city, key_terri, value_city, value_terri;
	key_city << 0;
	if (table_city.Find(key_city, value_city))
	{
		try
		{
			value_city >> city_store;

			std::vector<GCity>::iterator it, ite = city_store.cities.end();
			for (it = city_store.cities.begin(); it != ite; ++it)
			{
				if (it->info.owner.fid != 0)
				{
					it->info.owner.fid = g_mapFactionid[it->info.owner.fid];
					LOG_TRACE("map city owner to %d", it->info.owner.fid);
				}
				if (it->info.owner.master != 0)
				{
					it->info.owner.master = g_mapRoleid[it->info.owner.master].first;
					LOG_TRACE("map city owner master to %d", it->info.owner.master);
				}
				std::vector<GChallenger>::iterator cit, cite = it->challengers.end();
				for (cit = it->challengers.begin(); cit != cite; ++cit)
				{
					if (cit->challenger.fid != 0)
					{
						cit->challenger.fid = g_mapFactionid[cit->challenger.fid];
						LOG_TRACE("map challenger to %d", cit->challenger.fid);
					}
					if (cit->challenger.master != 0)
					{
						cit->challenger.master = g_mapRoleid[cit->challenger.master].first;
						LOG_TRACE("map challenger master to %d", cit->challenger.master);
					}
					if (cit->assistant.fid != 0)
					{
						cit->assistant.fid = g_mapFactionid[cit->assistant.fid];
						LOG_TRACE("map assistant to %d", cit->assistant.fid);
					}
					if (cit->assistant.master != 0)
					{
						cit->assistant.master = g_mapRoleid[cit->assistant.master].first;
						LOG_TRACE("map assistant master  to %d", cit->assistant.master);
					}
				}
			}
		}
		catch(...)
		{
			Log::log(LOG_ERR, "unmarshal city_store error");
		}
	}
	key_terri << 1;
	GTerritoryStore terri_store;
	if (table_city.Find(key_terri, value_terri))
	{
		try
		{
			value_terri >> terri_store;
			GTerritoryInfoVector::iterator it, ite = terri_store.tlist.end();
			for (it = terri_store.tlist.begin(); it != ite; ++it)
			{
				if (it->owner != 0)
				{
					it->owner = g_mapFactionid[it->owner];
					LOG_TRACE("map territory %d owner to %d", it->id, it->owner);
				}
				GTChallengeVector::iterator cit, cite = it->challengelist.end();
				for (cit = it->challengelist.begin(); cit != cite; ++cit)
				{
					if (cit->factionid != 0)
					{
						cit->factionid = g_mapFactionid[cit->factionid];
						LOG_TRACE("map territory %d challenger to %d", it->id, cit->factionid);
					}
				}
				if (it->defender != 0)
				{
					it->defender = g_mapFactionid[it->defender];
					LOG_TRACE("map territory %d defender to %d", it->id, it->defender);
				}
				if (it->success_challenge.factionid != 0)
				{
					it->success_challenge.factionid = g_mapFactionid[it->success_challenge.factionid];
					LOG_TRACE("map territory %d success_challenge to %d", it->id, it->success_challenge.factionid);
				}
			}
		}
		catch(...)
		{
			Log::log(LOG_ERR, "unmarshal terri_store error");
		}
	}

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("citystore");
		StorageEnv::AtomTransaction txn;
		try
		{
			pstorage->insert(key_city, Marshal::OctetsStream()<<city_store, txn);
			pstorage->insert(key_terri, Marshal::OctetsStream()<<terri_store, txn);
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException e( DB_OLD_VERSION );
			txn.abort( e );
			throw e;
		}
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "Move city error, what=%s.\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void MoveDBTo(const char * unique_dbhome, const char * srcdbhome, int zoneid)
{
	enum
	{
		cache_low  = 17000,
		cache_high = 20000,
	};
	if (zoneid <= 0)
	{
		Log::log(LOG_ERR, "MoveDB Invalid zoneid %d", zoneid);
		return;
	}
	std::string unique_dbdata = std::string(unique_dbhome) + "/dbdata";
	if (access(unique_dbdata.c_str(), F_OK))
	{
		Log::log(LOG_ERR, "Error: cannot find %s", unique_dbdata.c_str());
		return;
	}
	std::string src_dbdata = std::string(srcdbhome) + "/dbdata";
	if (access(src_dbdata.c_str(), F_OK))
	{
		Log::log(LOG_ERR, "Error: cannot find %s", src_dbdata.c_str());
		return;
	}
	//检查目标库/当前库是否为空
	try
	{
		 StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		 StorageEnv::CommonTransaction   txn; //txn析构时 释放 StorageVec
		 if (pbase == NULL || pbase->count() > 0)
		 {
			 Log::log(LOG_ERR, "base %p is not empty", pbase);
			 return;
		 }
	}
	catch(...)
	{
		Log::log(LOG_ERR, "get Storage base exception");
		return;
	}

	TableWrapper tab_logicuid((unique_dbdata + "/logicuid").c_str());
	TableWrapper tab_uidrole((unique_dbdata + "/uidrole").c_str());
	TableWrapper tab_unamerole((unique_dbdata + "/unamerole").c_str());
	TableWrapper tab_unamefamily((unique_dbdata + "/unamefamily").c_str());
	TableWrapper tab_unamefaction((unique_dbdata + "/unamefaction").c_str());
	TableWrapper tab_unamecrssvrteams((unique_dbdata + "/unamecrssvrteams").c_str());

	TableWrapper tab_user((src_dbdata + "/user").c_str());
	TableWrapper tab_base((src_dbdata + "/base").c_str());
	TableWrapper tab_status((src_dbdata + "/status").c_str());
	TableWrapper tab_rolename((src_dbdata + "/rolename").c_str());
	TableWrapper tab_family((src_dbdata + "/family").c_str());
	TableWrapper tab_faction((src_dbdata + "/faction").c_str());
	TableWrapper tab_crssvrteams((src_dbdata + "/crssvrteams").c_str());
	TableWrapper tab_familyname((src_dbdata + "/familyname").c_str());
	TableWrapper tab_factionname((src_dbdata + "/factionname").c_str());
	TableWrapper tab_crssvrteamsname((src_dbdata + "/crssvrteamsname").c_str());

	TableWrapper tab_inventory((src_dbdata + "/inventory").c_str());
	TableWrapper tab_storehouse((src_dbdata + "/storehouse").c_str());
	TableWrapper tab_task((src_dbdata + "/task").c_str());
	TableWrapper tab_achievement((src_dbdata + "/achievement").c_str());
	TableWrapper tab_waitdel((src_dbdata + "/waitdel").c_str());
	TableWrapper tab_mailbox((src_dbdata + "/mailbox").c_str());
	TableWrapper tab_message((src_dbdata + "/messages").c_str());
	TableWrapper tab_friends((src_dbdata + "/friends").c_str());
	TableWrapper tab_sect((src_dbdata + "/sect").c_str());
	TableWrapper tab_syslog((src_dbdata + "/syslog").c_str());
	TableWrapper tab_commondata((src_dbdata + "/commondata").c_str());
	TableWrapper tab_snsplayer((src_dbdata + "/snsplayerinfo").c_str());
	TableWrapper tab_snsmessage((src_dbdata + "/snsmessage").c_str());
	TableWrapper tab_clrrole((src_dbdata + "/clrrole").c_str());
	TableWrapper tab_base2((src_dbdata + "/base2").c_str());
	TableWrapper tab_referral((src_dbdata + "/referral").c_str());
	TableWrapper tab_hometown((src_dbdata + "/hometown").c_str());
	TableWrapper tab_consign((src_dbdata + "/consign").c_str());
	TableWrapper tab_finishconsign((src_dbdata + "/finished_consign").c_str());
	TableWrapper tab_rolenamehis((src_dbdata + "/rolenamehis").c_str());
	TableWrapper tab_award((src_dbdata + "/award").c_str());
	TableWrapper tab_callback((src_dbdata + "/friendcallback").c_str());
	TableWrapper tab_gt((src_dbdata + "/gtactivate").c_str());
	TableWrapper tab_dropcounter((src_dbdata + "/dropcounter").c_str());
	TableWrapper tab_kingdom((src_dbdata + "/kingdom").c_str());
	TableWrapper tab_config((src_dbdata + "/config").c_str());
	TableWrapper tab_city((src_dbdata + "/citystore").c_str());
	TableWrapper tab_operationcmd((src_dbdata + "/operationcmd").c_str());
	TableWrapper tab_weborder((src_dbdata + "/weborder").c_str());
	TableWrapper tab_facbase((src_dbdata + "/factionbase").c_str());

	TableWrapper tab_duprolename("./duprolename");//把重复的角色名导出到此表中

	if (!tab_logicuid.Open(cache_high, cache_low, true) || !tab_uidrole.Open(cache_high, cache_low, true) ||
			!tab_unamerole.Open(cache_high, cache_low, true) || !tab_unamefamily.Open(cache_high, cache_low, true) ||
			!tab_unamefaction.Open(cache_high, cache_low, true) || !tab_user.Open(cache_high, cache_low, true) ||
			!tab_base.Open(cache_high, cache_low, true) || !tab_status.Open(cache_high, cache_low, true) ||
			!tab_rolename.Open(cache_high, cache_low, true) || !tab_family.Open(cache_high, cache_low, true) ||
			!tab_faction.Open(cache_high, cache_low, true) || !tab_familyname.Open(cache_high, cache_low, true) ||
			!tab_factionname.Open(cache_high, cache_low, true) || !tab_task.Open(cache_high, cache_low, true) || 
			!tab_inventory.Open(cache_high, cache_low, true) || !tab_storehouse.Open(cache_high, cache_low, true) ||
			!tab_waitdel.Open(cache_high, cache_low, true) || !tab_mailbox.Open(cache_high, cache_low, true) ||
			!tab_message.Open(cache_high, cache_low, true) || !tab_friends.Open(cache_high, cache_low, true) ||
			!tab_sect.Open(cache_high, cache_low, true) || !tab_syslog.Open(cache_high, cache_low, true) ||
			!tab_commondata.Open(cache_high, cache_low, true) || !tab_snsplayer.Open(cache_high, cache_low, true) ||
			!tab_snsmessage.Open(cache_high, cache_low, true) || !tab_clrrole.Open(cache_high, cache_low, true) ||
			!tab_base2.Open(cache_high, cache_low, true) || !tab_referral.Open(cache_high, cache_low, true) ||
			!tab_hometown.Open(cache_high, cache_low, true) || !tab_consign.Open(cache_high, cache_low, true) ||
			!tab_finishconsign.Open(cache_high, cache_low, true) || !tab_achievement.Open(cache_high, cache_low, true) ||
			!tab_rolenamehis.Open(cache_high, cache_low, true) || !tab_award.Open(cache_high, cache_low, true) ||
			!tab_callback.Open(cache_high, cache_low, true) || !tab_gt.Open(cache_high, cache_low, true) ||
			!tab_dropcounter.Open(cache_high, cache_low, true) || !tab_config.Open(cache_high, cache_low, true) ||
			!tab_duprolename.Open(cache_high, cache_low, true) || !tab_kingdom.Open(cache_high, cache_low, true) ||
			!tab_city.Open(cache_high, cache_low, true) || !tab_operationcmd.Open(cache_high, cache_low, true) ||
			!tab_weborder.Open(cache_high, cache_low, true) || !tab_unamecrssvrteams.Open(cache_high, cache_low, true) ||
			!tab_crssvrteams.Open(cache_high, cache_low, true) || !tab_crssvrteamsname.Open(cache_high, cache_low, true)
			|| !tab_facbase.Open(cache_high, cache_low, true)
			)
	{
		Log::log(LOG_ERR, "Error: Open DB Failed");
		return;
	}
	if (tab_duprolename.Count() > 0)
	{
		Log::log(LOG_ERR, "duprolename not empty,count=%d", tab_duprolename.Count());
		return;
	}
	LOG_TRACE("Start MoveDB ... zoneid %d", zoneid);
	LogicuidAllocator::GetInstance(tab_logicuid, tab_uidrole, tab_unamefamily, tab_unamefaction, tab_unamecrssvrteams);
	LoadConfig( );
	if (!g_itemIdMan.LoadItemId()) return;
//	if (!PrepareLogicuid())
//		return;
	//是否必须？？ 这个都是针对当前库的
	/*
	if (!CheckDupLogicuid()) //CheckDupLogicuid要在PrepareLogicuid()之后调用
	{
		Log::log(LOG_ERR, "CheckDupLogicuid failed\n");
		return;
	}
	*/
	MoveUser(zoneid, tab_user, tab_uidrole, tab_logicuid, tab_base, tab_status, tab_base2, tab_inventory, tab_storehouse, tab_task, tab_achievement, tab_friends, tab_rolename, tab_rolenamehis, tab_award,
			tab_family, tab_crssvrteams, tab_callback, tab_referral, tab_gt, tab_waitdel, tab_hometown, tab_snsplayer, tab_mailbox);
	ReOpenDB();
	MoveRolename(zoneid, tab_unamerole, tab_rolename, tab_mailbox, tab_rolenamehis, tab_base, tab_duprolename);
	ReOpenDB();
	MoveFamily(tab_family, zoneid);
	MoveFaction(tab_faction, zoneid);
	MoveFamilyName(zoneid, tab_unamefamily, tab_familyname);
	MoveFactionName(zoneid, tab_unamefaction, tab_factionname);
	MoveCrssvrTeams(tab_crssvrteams, zoneid);
	MoveCrssvrTeamsName(zoneid, tab_unamecrssvrteams, tab_crssvrteamsname);
	UpdateCrssvrTeamsSeasonTop(tab_crssvrteams);
	ReOpenDB();

	LOG_TRACE( "\nMap Size:\n\tINFO:mapRoleid.size = %d\n\tINFO:mapRolename.size = %d(%d)\n\tINFO:mapFactionid.size = %d\n\tINFO:mapFactionname.size = %d(%d)\n\tINFO:mapFamilyid.size = %d\n\tINFO:mapFamilyname.size = %d(%d)\n\tINFO:mapCrssvrTeamsid.size = %d\n\tINFO:mapCrssvrTeamsname.size = %d(%d)\n", 
			g_mapRoleid.size(), g_mapRolename.RawSize(), g_mapRolename.RealSize(), 
			g_mapFactionid.size(), g_mapFactionname.RawSize(), g_mapFactionname.RealSize(), 
			g_mapFamilyid.size(), g_mapFamilyname.RawSize(), g_mapFamilyname.RealSize(), 
			g_mapCrssvrTeamsid.size(), g_mapCrssvrTeamsname.RawSize(), g_mapCrssvrTeamsname.RealSize() );

	// key改roleid
	MergeBase(tab_base);
	ReOpenDB();
	MergeStatus(tab_status);
	ReOpenDB();
	MergeRoleData(tab_inventory, "inventory" );
	ReOpenDB();
	MergeRoleData(tab_storehouse, "storehouse" );
	ReOpenDB();
	MergeRoleData(tab_task, "task" );
	ReOpenDB();
	MergeRoleData(tab_achievement, "achievement" );
	ReOpenDB();
	MergeRoleData(tab_gt, "gtactivate" );
	MergeRoleData(tab_callback, "friendcallback" ); //callback内部的roleid变换没有处理 忽略
	MergeRoleData(tab_award, "award" );

	MergeWaitdel(tab_waitdel);
	ReOpenDB();
	MergeMailbox(tab_mailbox); //邮件Merge到dst db之后可以 SendGifts()
	SendGifts();
	ReOpenDB();
	MergeMessages(tab_message);
	ReOpenDB();
	MergeFriends(tab_friends);
	ReOpenDB();
	MergeSect(tab_sect);
	MergeSyslog(tab_syslog);
//	MergeCommonData(tab_commondata);
	MoveTable(tab_commondata, "commondata");
	MergeSNSPlayerInfo(tab_snsplayer);
	MergeSNSMessage(tab_snsmessage);
	MergeClrRole(tab_clrrole);
	ReOpenDB();
	MergeBase2(tab_base2);
	MergeReferral(tab_referral);
	MergeHometown(tab_hometown);
	MergeConsign(tab_consign);
	MergeFinishedConsign(tab_finishconsign);
	MergeDropCounter(tab_dropcounter);
	MergeRolenamehis(tab_rolenamehis);
	MergeKingdom(tab_kingdom);

	MoveTable(tab_config, "config");
	MoveCity(tab_city); //移服时 领土战和凌霄城数据要保留
	MoveTable(tab_operationcmd, "operationcmd");
	MergeWebOrder(tab_weborder);
	MergeFacBase(tab_facbase);

	StorageEnv::checkpoint();
	StorageEnv::Close();

	StorageEnv::Open();
	StorageEnv::checkpoint( );
	StorageEnv::removeoldlogs( );

	LOG_TRACE( "\nMap Size:\n\tINFO:mapRoleid.size = %d\n\tINFO:mapRolename.size = %d(%d)\n\tINFO:mapFactionid.size = %d\n\tINFO:mapFactionname.size = %d(%d)\n\tINFO:mapFamilyid.size = %d\n\tINFO:mapFamilyname.size = %d(%d)\n\tINFO:mapCrssvrTeamsid.size = %d\n\tINFO:mapCrssvrTeamsname.size = %d(%d)\n", 
			g_mapRoleid.size(), g_mapRolename.RawSize(), g_mapRolename.RealSize(), 
			g_mapFactionid.size(), g_mapFactionname.RawSize(), g_mapFactionname.RealSize(), 
			g_mapFamilyid.size(), g_mapFamilyname.RawSize(), g_mapFamilyname.RealSize(),
			g_mapCrssvrTeamsid.size(), g_mapCrssvrTeamsname.RawSize(), g_mapCrssvrTeamsname.RealSize());
	LOG_TRACE( "\nMerge Report:\n\tINFO:server2_usercount = %d\n\tINFO:same_usercount = %d\n\tINFO:server2_rolecount = %d\n\tINFO:same_rolecount = %d\n\tINFO:dup_roleid = %d\n\tINFO:dup_factionid = %d\n\tINFO:dup_familyid = %d\n\tINFO:dup_rolename = %d\n\tINFO:dup_factionname = %d\n\tINFO:dup_familyname = %d\n\tINFO:dup_crssvrteamsid = %d\n\tINFO:dup_crssvrteamsname = %d\n", 
			server2_usercount, same_usercount, server2_rolecount, same_rolecount, dup_roleid, dup_factionid, 
			dup_familyid, dup_rolename, dup_factionname, dup_familyname, dup_crssvrteamsid, dup_crssvrteamsname );

}

};

