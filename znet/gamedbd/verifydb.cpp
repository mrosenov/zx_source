#include "gamedbmanager.h"
#include "storagetool.h"
#include "user"
#include "gfactioninfo"
#include "gfolk"
#include "familyid"
#include "gfamily"
#include "grolebase"
#include "grolestatus"
#include "grolestorehouse"
#include "gmailbox"
#include "waitdelkey"
#include "gsect"
#include "gdisciple"
#include "accessdb.h"

namespace GNET
{
static int	usercount = 0;
static int	rolecount = 0;
static int	rolenamecount = 0;
static int	familycount = 0;
static int	familynamecount = 0;
static int	factioncount = 0;
static int	factionnamecount = 0;
static int	waitdelcount = 0;
static int	mailboxcount = 0;
static int	sectcount = 0;

static  std::map<int, unsigned int>	uniquelogicuidmap;

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

class PrepareUniqueQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream key_os, value_os;
			key_os = key;
			value_os = value;
			int userid = -1, logicuid = -1;
			unsigned int roles = 0;
			key_os >> userid;
			if (value_os.size() == 8)
				value_os >> roles >> logicuid;
			else
				logicuid = userid;
			uniquelogicuidmap[userid] = logicuid;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "PrepareUniqueQuery, exception\n" );
		}
		return true;
	}
};

void PrepareUnique( )
{
	LOG_TRACE( "\nPrepare Unique:\n" );

	PrepareUniqueQuery q;
	try
	{
		DBStandalone * pstandalone = new DBStandalone("uidrole");
		pstandalone->init();
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,"uidrole",pstandalone,new StorageEnv::Uncompressor());
			cursor.walk( q );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			DbException ee( DB_OLD_VERSION );
			txn.abort( ee );
			throw ee;
		}
		pstandalone->checkpoint();
		delete pstandalone;
		pstandalone = NULL;
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "PrepareUnique, error when walk for PrepareUniqueQuery, what=%s\n", e.what() );
	}
}

class UserQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value);
				int userid = -1;
				User	user;
				os_key >> userid;
				usercount ++;
				os_value >> user;
				unsigned int logicuid = user.logicuid;
				RoleList rolelist(user.rolelist);
				int role = -1;
				//consistency check
				while ((role = rolelist.GetNextRole()) >= 0)
				{
					unsigned int roleid = logicuid+role;
					Marshal::OctetsStream os_base;
					GRoleBase base;
					if (pbase->find(Marshal::OctetsStream()<<roleid,
						os_base, txn))
					{
						os_base >> base;
						int gen_userid = -1;
						if (base.userid == 0)
							gen_userid = LOGICUID(roleid);
						else
							gen_userid = base.userid;
						if (gen_userid != userid)
						{
							LOG_TRACE("Userid[%d] of role[%u] does not match with user[%d]\n", base.userid, roleid, userid);
						}
						if (base.id != roleid)
						{
							LOG_TRACE("Role.id[%u] does not match with [%d+%d] (logicuid+role)\n", base.id, logicuid, role);
						}
					}
					else
					{
						LOG_TRACE ("Can not find roleid[%u] in base\n", roleid);
					}
					Marshal::OctetsStream os_status;
					GRoleStatus status;
					if (pstatus->find(Marshal::OctetsStream()<<roleid,
						os_status, txn))
					{
						os_status >> status;
						if (status.id != roleid)
						{
							LOG_TRACE("status.id does not match\n");
						}
					}
					else
					{
						LOG_TRACE ("Can not find role [%u] in status\n", roleid);
					}

				}
				//validity check
				if (user.cash < 0 || user.cash > 1000000)
				{
					LOG_TRACE ("User[%d] cash[%d] invalid\n", userid, user.cash);
				}
				if (user.money < 0 || user.money > 200000000)
				{
					LOG_TRACE ("User[%d] money[%d] invalid\n", userid, user.money);
				}
				if (user.cash_buy > 100000000)
				{
					LOG_TRACE ("User[%d] cash_buy[%u] invalid\n", userid, user.cash_buy);
				}
				if (user.cash_sell > 100000000)
				{
					LOG_TRACE ("User[%d] cash_sell[%u] invalid\n", userid, user.cash_sell);
				}
				if (user.cash_add > 100000000)
				{
					LOG_TRACE ("User[%d] cash_add[%u] invalid\n", userid, user.cash_add);
				}
				//Check logicuid with uidrole table on uniquename Server
				if (uniquelogicuidmap.size() > 0)
				{
					std::map<int, unsigned int>::iterator it = uniquelogicuidmap.find(userid);
					if (it == uniquelogicuidmap.end())
						LOG_TRACE ("Can not find logicuid for user %d in uidrole table\n", userid);
					else
					{
						if (it->second != logicuid)
							LOG_TRACE ("logicuids do not match, userid %d, user.logicuid %d, uidrole.logicuid %d\n", userid, logicuid, it->second);
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
			Log::log( LOG_ERR, "UserheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void UserCheck( )
{
	LOG_TRACE( "\nUserCheck:\n" );
	UserQuery q;
	try
	{
		TableWrapper tab_user((StorageEnv::get_datadir() + "/user").c_str());
		if (!tab_user.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open user failed");
			return;
		}
		//StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction	txn;
		try
		{
		//	StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "User query, error when walk, what=%s\n", e.what() );
	}
}

class BaseQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
			StorageEnv::Storage * ppocket = StorageEnv::GetStorage("inventory");
			StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
			StorageEnv::Storage * psect = StorageEnv::GetStorage("sect");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_user, os_family, os_spouse, os_pocket, os_storehouse, os_sect;
				unsigned int roleid;
				GRoleBase base, spouse;
				os_key >> roleid;
				os_value >> base;
				rolecount ++;
				int userid = -1;
				if (base.userid == 0)
					userid = LOGICUID(roleid);
				else
					userid = base.userid;
				//consistency check
				if (puser->find(Marshal::OctetsStream()<<userid, os_user,txn))
				{
					User user;
					os_user >> user;
					RoleList rolelist(user.rolelist);
					int role = roleid - user.logicuid;
					if (!rolelist.IsRoleExist(role))
					{
						LOG_TRACE ("User[%d] role[%d] does not exist roleid=%d\n", userid, role, roleid);
					}
				}
				else
				{
					LOG_TRACE ("Can not find user[%d] for role[%u]\n", userid, roleid);
				}
				if (base.familyid != 0)
				{
					if (!pfamily->find(Marshal::OctetsStream()<<base.familyid, os_family, txn))
					{
						LOG_TRACE ("Can not find family[%u] in family table for role[%u]\n",
							base.familyid, roleid);
					}
					else
					{
						GFamily  family;
						os_family >> family;
						GFolkVector::iterator it, ite;
						for (it = family.member.begin(), ite = family.member.end(); it != ite; ++it )
						{
							if (it->rid == roleid)
								break;
						}
						if (it == ite)
						{
							LOG_TRACE ("Role[%u] is not a member of role.family[%u]", roleid, base.familyid);
						}
					}
				}
				Marshal::OctetsStream os_roleid;
				Octets name = base.name;
				if (prolename->find(name, os_roleid,txn))
				{
					unsigned int roleid2;
					os_roleid >> roleid2;
					if (roleid2 != roleid)
					{
						Octets  namegbk, nameesc;
						CharsetConverter::conv_charset_u2l( name, namegbk );
						EscapeCSVString( namegbk, nameesc );
						LOG_TRACE ("Roleid[%u] of rolename %.*s does not match with roleid[%u] of base\n",
							roleid2, nameesc.size(), (char*)nameesc.begin(), roleid);
					}
				}
				else
				{
					Octets  namegbk, nameesc;
					CharsetConverter::conv_charset_u2l( name, namegbk );
					EscapeCSVString( namegbk, nameesc );
					LOG_TRACE ("Can not find name %.*s in rolename for role[%u]\n",
					nameesc.size(), (char*)nameesc.begin(), roleid);
				}
				if (base.sectid != 0)
				{
					if (!psect->find(Marshal::OctetsStream()<<base.sectid, os_sect, txn))
					{
						LOG_TRACE ("Can not sect[%u] in sect for role[%u]\n", base.sectid, roleid); 	
					}
					else
					{
						GSect sect;
						os_sect >> sect;
						if ((unsigned int)base.sectid == roleid)
						{
							if (sect.name != base.name)
							{
								LOG_TRACE ("Sect[%u] name does not match with master[%u] name\n", roleid, roleid); 
							}
						}
						else
						{
							GDiscipleVector::iterator it, ite;
							for (it = sect.disciples.begin(), ite = sect.disciples.end(); it != ite; it++)
							{
								if ((unsigned int)it->roleid == roleid)
								{
									if (it->name != base.name)
									{
										LOG_TRACE ("Disciple[%u] name of sect[%u] does not match with role[%u] name\n", roleid, base.sectid, roleid);
									}
									break;
								}
							}
							if (it == ite)
								LOG_TRACE ("Role[%u] is not a member of role.sectid[%u]", roleid, base.sectid);
						}
					}
				}
						
				//validity check
				if (base.id != roleid)
				{
					LOG_TRACE ("Base.id[%u] does not match with key[%u]\n", base.id, roleid);
				}
				if (base.familyid == 0 && base.title != 0)
				{
					LOG_TRACE ("Base: familyid[%d] title[%d] does not match\n", base.familyid, base.title);
				}
				if (base.spouse != 0)
				{
					if (pbase->find(Marshal::OctetsStream()<<base.spouse, os_spouse, txn))
					{	
						os_spouse >> spouse;
						if (spouse.spouse != roleid)
						{
							LOG_TRACE ("Role[%u] spouse[%u], spouse.spouse[%u] does not match\n",
									roleid, base.spouse, spouse.spouse);
						}
					}
					else
					{
						LOG_TRACE ("Can not find spouse[%u] in base for role[%u]\n", base.spouse, roleid);
					}
				}
				size_t rolestuffsize = 0;
				if (ppocket->find(Marshal::OctetsStream()<<roleid, os_pocket, txn))
				{
					rolestuffsize += os_pocket.size();
				}
				if (pstorehouse->find(Marshal::OctetsStream()<<roleid, os_storehouse, txn))
				{
					rolestuffsize += os_storehouse.size();
				}
				if (rolestuffsize >= 500000)
				{
					LOG_TRACE ("Role[%u]: pocket and storehouse size[%u] is too large\n", roleid, rolestuffsize);
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
			Log::log( LOG_ERR, "BaseCheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void BaseCheck( )
{
	LOG_TRACE( "\nBaseCheck:\n" );
	BaseQuery q;
	try
	{
		TableWrapper tab_base((StorageEnv::get_datadir() + "/base").c_str());
		if (!tab_base.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open base failed");
			return;
		}
//		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn;
		try
		{
//			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Base query, error when walk, what=%s\n", e.what() );
	}
}

class RolenameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_base;
				Octets name = key;
				unsigned int roleid;
				os_value >> roleid;
				rolenamecount ++;
				//consistency check
				if (pbase->find(Marshal::OctetsStream()<<roleid, os_base,txn))
				{

					GRoleBase base;
					os_base >> base;
					if (base.name != name)
					{
						Octets  namegbk, namegbk2, nameesc, nameesc2;
						CharsetConverter::conv_charset_u2l( name, namegbk );
						CharsetConverter::conv_charset_u2l( base.name, namegbk2 );
						EscapeCSVString( namegbk, nameesc );
						EscapeCSVString( namegbk2, nameesc2 );
						LOG_TRACE ("Name %.*s of role[%u] does not match with name %.*s in rolename\n",
							nameesc2.size(), (char*)nameesc2.begin(), roleid, nameesc.size(), 
							(char*)nameesc.begin());
					}

				}
				else
				{
					Octets  namegbk, nameesc;
					CharsetConverter::conv_charset_u2l( name, namegbk );
					EscapeCSVString( namegbk, nameesc );
					LOG_TRACE ("Can not find role[%u] of name %.*s in rolename\n",
						roleid,	nameesc.size(), (char*)nameesc.begin());
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
			Log::log( LOG_ERR, "RoleNameCheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void RoleNameCheck( )
{
	LOG_TRACE( "\nRoleNameCheck:\n" );
	RolenameQuery q;
	try
	{
		TableWrapper tab_rolename((StorageEnv::get_datadir() + "/rolename").c_str());
		if (!tab_rolename.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open rolename failed");
			return;
		}
//		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "rolename" );
		StorageEnv::AtomTransaction	txn;
		try
		{
//			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Rolename query, error when walk, what=%s\n", e.what() );
	}
}

class FamilyQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pfamilyname = StorageEnv::GetStorage("familyname");
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_fid, os_master, os_faction;
				unsigned int fid;
				GFamily family;
				os_key >> fid;
				os_value >> family;
				familycount ++;
				Octets fname = family.name;
				//consistency check
				if (pfamilyname->find(Marshal::OctetsStream()<<fname, os_fid,txn))
				{
					unsigned int fid2;
					os_fid >> fid2;
					if (fid2 != fid)
					{
						Octets  namegbk, nameesc;
						CharsetConverter::conv_charset_u2l( fname, namegbk );
						EscapeCSVString( namegbk, nameesc );
						LOG_TRACE ("Familyid[%u] of familyname %.*s does not match with familyid[%u] of family\n",
								fid2, nameesc.size(), (char*)nameesc.begin(), fid);
					}
				}
				else
				{
					Octets  namegbk, nameesc;
					CharsetConverter::conv_charset_u2l( fname, namegbk );
					EscapeCSVString( namegbk, nameesc );
					LOG_TRACE ("Can not find name %.*s in familyname for family[%u]\n",
							nameesc.size(), (char*)nameesc.begin(), fid);
				}
				unsigned int master = family.master;
				if (pbase->find(Marshal::OctetsStream()<<master, os_master, txn))
				{
					GRoleBase masterinfo;
					os_master >> masterinfo;
					if (masterinfo.familyid != fid)
					{
						LOG_TRACE("Familyid[%u] of master[%u] does not match with family.id[%u]\n",
							masterinfo.familyid, master, fid);
					}
					if (masterinfo.title <= 1 || masterinfo.title >= 6)
					{
						LOG_TRACE("Title value[%u] of master[%u] of family[%u] is invalid\n",
							masterinfo.title, master, fid);
					}
				}
				else
				{
					LOG_TRACE ("Can not find master[%u] of family[%u] in base\n", master, fid);
				}
				for (GFolkVector::iterator it = family.member.begin(), ite = family.member.end(); it != ite; ++it )
				{
					Marshal::OctetsStream os_member;
					if (pbase->find(Marshal::OctetsStream()<<it->rid, os_member, txn))
					{
						GRoleBase memberinfo;
						os_member >> memberinfo;
						if (memberinfo.familyid != fid)
						{
							LOG_TRACE("Familyid[%u] of member[%u] does not match with family.id[%u]\n", 
							memberinfo.familyid, it->rid, fid);
						}
						if (it->rid != family.master && memberinfo.title != 6)
						{
							LOG_TRACE("Title value[%u] of member[%u] of family[%u] is invalid\n",
							memberinfo.title, it->rid, fid);
						}
						if (it->name != memberinfo.name)
							LOG_TRACE("folk name of family %d does not match base[%d].name\n", fid, it->rid);

					}
					else
					{
						LOG_TRACE("Can not find member[%u] of family[%u] in base\n", it->rid, fid);
					}
				}
				if (family.factionid != 0)
				{
					if (!pfaction->find(Marshal::OctetsStream()<<family.factionid, os_faction, txn))
					{
						LOG_TRACE ("Can not find faction[%u] in faction table for family [%u]\n",
							family.factionid, fid);
					}
					else
					{
						GFactionInfo faction;
						os_faction >> faction;
						FamilyIdVector::iterator it, ite;
						for (it = faction.member.begin(), ite = faction.member.end(); it != ite; ++it )
						{
							if (it->fid == fid)
								break;
						}
						if (it == ite)
						{
							LOG_TRACE ("Family[%u] is not a member of family.faction[%u]\n", fid, family.factionid);
						}
					}
				}
				//validity check
				if (family.id != fid)
				{
					LOG_TRACE ("family.id[%u] does not match with key[%u]\n", family.id, fid);
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
			Log::log( LOG_ERR, "FamilyCheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void FamilyCheck( )
{
	LOG_TRACE( "\nFamilyCheck:\n" );
	FamilyQuery q;
	try
	{
		TableWrapper table((StorageEnv::get_datadir() + "/family").c_str());
		if (!table.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open family failed");
			return;
		}
		//StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "family" );
		StorageEnv::AtomTransaction	txn;
		try
		{
		//	StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Family query, error when walk, what=%s\n", e.what() );
	}
}

class FamilynameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_family;
				Octets fname;
				unsigned int fid;
				os_key >> fname;
				os_value >> fid;
				familynamecount ++;
				//consistency check
				if (pfamily->find(Marshal::OctetsStream()<<fid, os_family,txn))
				{
					GFamily family;
					os_family >> family;
					if (family.name != fname)
					{
						Octets  namegbk, namegbk2, nameesc, nameesc2;
						CharsetConverter::conv_charset_u2l( fname, namegbk );
						CharsetConverter::conv_charset_u2l( family.name, namegbk2 );
						EscapeCSVString( namegbk, nameesc );
						EscapeCSVString( namegbk2, nameesc2 );
						LOG_TRACE ("Name %.*s of family[%u] does not match with name %.*s in familyname\n",
							nameesc2.size(), (char*)nameesc2.begin(), fid, nameesc.size(), 
							(char*)nameesc.begin());
					}
				}
				else
				{
					Octets  namegbk, nameesc;
					CharsetConverter::conv_charset_u2l( fname, namegbk );
					EscapeCSVString( namegbk, nameesc );
					LOG_TRACE ("Can not find family[%u] of name %.*s in familyname\n",
						fid,	nameesc.size(), (char*)nameesc.begin());
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
			Log::log( LOG_ERR, "FamilyNameCheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void FamilyNameCheck( )
{
	LOG_TRACE( "\nFamilyNameCheck:\n" );
	FamilynameQuery q;
	try
	{
		TableWrapper table((StorageEnv::get_datadir() + "/familyname").c_str());
		if (!table.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open faction failed");
			return;
		}

		//StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "familyname" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			//StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Familyname query, error when walk, what=%s\n", e.what() );
	}
}

class FactionQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pfactionname = StorageEnv::GetStorage("factionname");
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_fid, os_master;
				unsigned int fid;
				GFactionInfo faction;
				os_key >> fid;
				os_value >> faction;
				factioncount ++;
				Octets fname = faction.name;
				//consistency check
				if (pfactionname->find(Marshal::OctetsStream()<<fname, os_fid,txn))
				{
					unsigned int fid2;
					os_fid >> fid2;
					if (fid2 != fid)
					{
						Octets  namegbk, nameesc;
						CharsetConverter::conv_charset_u2l( fname, namegbk );
						EscapeCSVString( namegbk, nameesc );
						LOG_TRACE ("Factionid[%u] of factionname %.*s does not match with factionid[%u] of faction\n",
								fid2, nameesc.size(), (char*)nameesc.begin(), fid);
					}
				}
				else
				{
					Octets  namegbk, nameesc;
					CharsetConverter::conv_charset_u2l( fname, namegbk );
					EscapeCSVString( namegbk, nameesc );
					LOG_TRACE ("Can not find name %.*s in factionname for faction[%u]\n",
								nameesc.size(), (char*)nameesc.begin(), fid);
				}
				unsigned int master = faction.master;
				if (pbase->find(Marshal::OctetsStream()<<master, os_master, txn))
				{
					GRoleBase masterinfo;
					os_master >> masterinfo;
					FamilyIdVector::iterator it, ite;
					for (it = faction.member.begin(), ite = faction.member.end(); it != ite; ++it )
					{
						if (it->fid == masterinfo.familyid)
							break;
					}
					if (it == ite)
					{
						LOG_TRACE("Family[%u] of master[%u] of faction[%u] does not belong to this faction",
							masterinfo.familyid, master, fid);
					}
					if (masterinfo.title != 2)
					{
						LOG_TRACE("Title value[%u] of master[%u] of faction[%u] is invalid\n",
							masterinfo.title, master, fid);
					}
				}
				else
				{
					LOG_TRACE ("Can not find master[%u] of faction[%u] in base\n", master, fid);
				}
				for (FamilyIdVector::iterator it = faction.member.begin(), ite = faction.member.end(); it != ite; ++it )
				{
					Marshal::OctetsStream os_member;
					if (pfamily->find(Marshal::OctetsStream()<<it->fid, os_member, txn))
					{
						GFamily memberinfo;
						os_member >> memberinfo;
						if (memberinfo.factionid != fid)
						{
							LOG_TRACE("Factionid[%u] of member[%u] does not match with faction.id[%u]\n",
							memberinfo.factionid, it->fid, fid);
						}
					}
					else
					{
						LOG_TRACE("Can not find member[%u] of faction[%u] in family\n", it->fid, fid);
					}
				}
				//validity check
				if (faction.fid != fid)
				{
					LOG_TRACE ("Faction.id[%u] does not match with key[%u]\n", faction.fid, fid);
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
			Log::log( LOG_ERR, "FactionCheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void FactionCheck( )
{
	LOG_TRACE( "\nFactionCheck:\n" );
	FactionQuery q;
	try
	{
		TableWrapper table((StorageEnv::get_datadir() + "/faction").c_str());
		if (!table.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open faction failed");
			return;
		}

//		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "faction" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			//StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Faction query, error when walk, what=%s\n", e.what() );
	}
}

class FactionnameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_faction;
				Octets fname;
				unsigned int fid;
				os_key >> fname;
				os_value >> fid;
				factionnamecount ++;
				//consistency check
				if (pfaction->find(Marshal::OctetsStream()<<fid, os_faction,txn))
				{
					GFactionInfo faction;
					os_faction >> faction;
					if (faction.name != fname)
					{
						Octets  namegbk, namegbk2, nameesc, nameesc2;
						CharsetConverter::conv_charset_u2l( fname, namegbk );
						CharsetConverter::conv_charset_u2l( faction.name, namegbk2 );
						EscapeCSVString( namegbk, nameesc );
						EscapeCSVString( namegbk2, nameesc2 );
						LOG_TRACE ("Name %.*s of faction[%u] does not match with name %.*s in factionname\n",
							nameesc2.size(), (char*)nameesc2.begin(), fid, nameesc.size(), 
							(char*)nameesc.begin());
					}
				}
				else
				{
					Octets  namegbk, nameesc;
					CharsetConverter::conv_charset_u2l( fname, namegbk );
					EscapeCSVString( namegbk, nameesc );
					LOG_TRACE ("Can not find faction[%u] of name %.*s in factionname\n",
						fid,	nameesc.size(), (char*)nameesc.begin());
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
			Log::log( LOG_ERR, "FactionNameCheckQuery, what=%s\n", e.what() );
		}
		static size_t counter = 1;
		Auto_Checkpoint(counter);
		return true;
	}
};

void FactionNameCheck( )
{
	LOG_TRACE( "\nFactionNameCheck:\n" );
	FactionnameQuery q;
	try
	{
		TableWrapper table((StorageEnv::get_datadir() + "/factionname").c_str());
		if (!table.Open(25000, 20000, false))
		{
			Log::log(LOG_ERR, "open factionname failed");
			return;
		}

//		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factionname" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			//StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Factionname query, error when walk, what=%s\n", e.what() );
	}
}

class WaitDelQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_info;
				WaitDelKey	wdk;
				int deltime = -1;
				unsigned int id;
				os_key >> wdk;
				os_value >> deltime;
				waitdelcount ++;
				id = wdk.id;
				//consistency check
				switch (wdk.type)
				{
					case TYPE_ROLE:
						if (pbase->find(Marshal::OctetsStream()<<id, os_info, txn))
						{
							GRoleBase base;
							os_info >> base;
							if (base.status != _ROLE_STATUS_READYDEL)
							{
								LOG_TRACE ("Role[%u] waitting to be deleted, status[%d] invalid\n",
									id, base.status);
							}
							if (base.delete_time != deltime)
							{
								LOG_TRACE ("Delete time[%d] of role[%u] does not match with waitdel.deletetime[%d]",
										base.delete_time, id, deltime);
							}
						}
						else
						{
							LOG_TRACE ("Can not find role[%u] to be deleted in base\n", id);
						}
						break;
					case TYPE_FACTION:
						if (pfaction->find(Marshal::OctetsStream()<<id, os_info, txn))
						{
							GFactionInfo faction;
							os_info >> faction;
							if (faction.status != _ROLE_STATUS_READYDEL)
							{
								LOG_TRACE ("Faction[%u] waitting to be deleted, status[%d] invalid\n",
									id, faction.status);
							}
							if (faction.deletetime != deltime)
							{
								LOG_TRACE ("Delete time[%d] of faction[%u] does not match with waitdel.deletetime[%d]",
										faction.deletetime, id, deltime);
							}
						}
						else
						{
							LOG_TRACE ("Can not find faction[%u] to be deleted in faction\n", id);
						}
						break;
					case TYPE_FAMILY:
						if (pfamily->find(Marshal::OctetsStream()<<id, os_info, txn))
						{
							GFamily family;
							os_info >> family;
							if (family.deletetime != deltime)
							{
								LOG_TRACE ("Delete time[%d] of family[%u] does not match with waitdel.deletetime[%d]",
										family.deletetime, id, deltime);
							}
						}
						else
						{
							LOG_TRACE ("Can not find family[%u] to be deleted in family\n", id);
						}
						break;
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
			Log::log( LOG_ERR, "WaitDelCheckQuery, what=%s\n", e.what() );
		}
		return true;
	}
};

void WaitDelCheck( )
{
	LOG_TRACE( "\nWaitDelCheck:\n" );
	WaitDelQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "waitdel" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "WaitDel query, error when walk, what=%s\n", e.what() );
	}
}

class MailBoxQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);
			unsigned int roleid;
			GMailBox box;
			os_key >> roleid;
			os_value >> box;
			mailboxcount ++;
			if (box.mails.size() > 128)
			{
				LOG_TRACE ("Role[%u] mails size[%u] is too large\n", roleid, box.mails.size());
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "MailBoxCheckQuery, exception\n" );
		}
		return true;
	}
};

void MailBoxCheck( )
{
	LOG_TRACE( "\nMailBoxCheck:\n" );
	MailBoxQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "mailbox" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Mailbox query, error when walk, what=%s\n", e.what() );
	}
}

class SectQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			try
			{
				Marshal::OctetsStream	os_key(key), os_value(value), os_base;
				unsigned int roleid;
				GSect sect;
				os_key >> roleid;
				os_value >> sect;
				sectcount ++;
				//consistency check
				if (!pbase->find(Marshal::OctetsStream()<<roleid, os_base,txn))
				{
					LOG_TRACE ("Can not find master[%u] in base for sect[%u]\n", roleid, roleid);
				}
				else
				{
					GRoleBase base;
					os_base >> base;
					if ((unsigned int)base.sectid != roleid)
						LOG_TRACE ("Master's sectid[%u] does not match with its roleid[%u]\n", base.sectid, roleid);
					if (base.name != sect.name)
						LOG_TRACE("sect.name does not match master %d's name\n", roleid);
				}
				GDiscipleVector::iterator it, ite;
				for (it = sect.disciples.begin(), ite = sect.disciples.end(); it != ite; it++)
				{
					Marshal::OctetsStream os_dis_base;
					if (!pbase->find(Marshal::OctetsStream()<<it->roleid, os_dis_base, txn))
						LOG_TRACE ("Can not find member[%u] in base for sect[%u]\n", it->roleid, roleid);
					else
					{
						GRoleBase dis_base;
						os_dis_base >> dis_base; 
						if ((unsigned int)dis_base.sectid != roleid)
							LOG_TRACE ("Sectid[%u] of role[%u] does not match with sect[%u]\n", dis_base.sectid, it->roleid, roleid);
						if (it->name != dis_base.name)
							LOG_TRACE("disciple %d's name does not match base.name, sectid %d\n", it->roleid, roleid);
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
			Log::log( LOG_ERR, "SectCheckQuery, what=%s\n", e.what() );
		}
		return true;
	}
};

void SectCheck( )
{
	LOG_TRACE( "\nSectCheck:\n" );
	SectQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "sect" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
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
		Log::log( LOG_ERR, "Sect query, error when walk, what=%s\n", e.what() );
	}
}

void VerifyDB( )
{
	timeval tv_start, tv_end, tv_in, tv_out;
	timeval tv_user, tv_base, tv_rolename, tv_family, tv_familyname, tv_faction, tv_factionname, tv_waitdel, tv_mailbox, tv_sect, tv_total;
	PrepareUnique();
	LOG_TRACE ("uidrole size %d\n", uniquelogicuidmap.size());
	//consistency and validity check
	gettimeofday(&tv_start, NULL);
	tv_in = tv_start;
	UserCheck( );
	gettimeofday(&tv_end, NULL);
	tv_user.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_user.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	BaseCheck( );
	gettimeofday(&tv_end, NULL);
	tv_base.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_base.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	RoleNameCheck( );
	gettimeofday(&tv_end, NULL);
	tv_rolename.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_rolename.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	FamilyCheck( );
	gettimeofday(&tv_end, NULL);
	tv_family.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_family.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	FamilyNameCheck( );
	gettimeofday(&tv_end, NULL);
	tv_familyname.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_familyname.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	FactionCheck( );
	gettimeofday(&tv_end, NULL);
	tv_faction.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_faction.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	FactionNameCheck( );
	gettimeofday(&tv_end, NULL);
	tv_factionname.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_factionname.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );
	//only validity check
	gettimeofday(&tv_start, NULL);
	WaitDelCheck( );
	gettimeofday(&tv_end, NULL);
	tv_waitdel.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_waitdel.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	MailBoxCheck( );
	gettimeofday(&tv_end, NULL);
	tv_mailbox.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_mailbox.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	gettimeofday(&tv_start, NULL);
	SectCheck( );
	gettimeofday(&tv_end, NULL);
	tv_sect.tv_sec = tv_end.tv_sec- tv_start.tv_sec -  ( tv_end.tv_sec >= tv_start.tv_sec ? 0 : 1 );
	tv_sect.tv_usec = tv_end.tv_usec-tv_start.tv_usec + ( tv_end.tv_usec>= tv_start.tv_usec ? 0 : 1000000 );

	tv_out = tv_end;
	tv_total.tv_sec = tv_out.tv_sec- tv_in.tv_sec -  ( tv_out.tv_sec >= tv_in.tv_sec ? 0 : 1 );
	tv_total.tv_usec = tv_out.tv_usec-tv_in.tv_usec + ( tv_out.tv_usec >= tv_in.tv_usec ? 0 : 1000000 );

	//	StorageEnv::checkpoint();
	//	StorageEnv::Close();

	//	StorageEnv::Open();
	//	StorageEnv::checkpoint( );
	//	StorageEnv::removeoldlogs( );

	LOG_TRACE( "\nVerify Report:\n");
	LOG_TRACE( "\tINFO:usercount        = %8d\ttime used: %8ld sec %8ld usec\n", 
			usercount, tv_user.tv_sec, tv_user.tv_usec);
	LOG_TRACE( "\tINFO:rolecount        = %8d\ttime used: %8ld sec %8ld usec\n", 
			rolecount, tv_base.tv_sec, tv_base.tv_usec);
	LOG_TRACE( "\tINFO:rolenamecount    = %8d\ttime used: %8ld sec %8ld usec\n",
			rolenamecount, tv_rolename.tv_sec, tv_rolename.tv_usec);
	LOG_TRACE( "\tINFO:familycount      = %8d\ttime used: %8ld sec %8ld usec\n",
			familycount, tv_family.tv_sec, tv_family.tv_usec);
	LOG_TRACE( "\tINFO:familynamecount  = %8d\ttime used: %8ld sec %8ld usec\n",
			familynamecount, tv_familyname.tv_sec, tv_familyname.tv_usec);
	LOG_TRACE( "\tINFO:factioncount     = %8d\ttime used: %8ld sec %8ld usec\n", 
			factioncount, tv_faction.tv_sec, tv_faction.tv_usec);
	LOG_TRACE( "\tINFO:factionnamecount = %8d\ttime used: %8ld sec %8ld usec\n", 
			factionnamecount, tv_factionname.tv_sec, tv_factionname.tv_usec);
	LOG_TRACE( "\tINFO:waitdelcount     = %8d\ttime used: %8ld sec %8ld usec\n",
			waitdelcount, tv_waitdel.tv_sec, tv_waitdel.tv_usec);
	LOG_TRACE( "\tINFO:mailboxcount     = %8d\ttime used: %8ld sec %8ld usec\n", 
			mailboxcount, tv_mailbox.tv_sec, tv_mailbox.tv_usec);
	LOG_TRACE( "\tINFO:sectcount        = %8d\ttime used: %8ld sec %8ld usec\n", 
			sectcount, tv_sect.tv_sec, tv_sect.tv_usec);
	LOG_TRACE( "\n\tINFO:time used totally: %8ld sec %8ld usec\n\n",
			tv_total.tv_sec, tv_total.tv_usec);
}

};

