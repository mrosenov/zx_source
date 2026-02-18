#include <sys/types.h>
#include <dirent.h>

#include <set>
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "macros.h"
#include "accessdb.h"
#include "stocklog"
#include "user"
#include "gauctionitem"
#include "gauctiondetail"
#include "gmail"
#include "gmailbox"
#include "gsyslog"
#include "utilfunction.h"
#include "storagetool.h"
#include "xmlcoder.h"
#include "db.h"
#include "localmacro.h"
#include "itemowner.h"
#include "gcitystore"
#include "dbclearconsumable.hrp"
#include "grolebase2old"

namespace GNET
{

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


typedef std::map<int, int> tUserIdExchangeMap;
static tUserIdExchangeMap g_UserIdExchangeTable;

static int checkpoint_threshold = 10000;

int GetUserIdFromTable(int userid_old)
{
	if(userid_old == 16)// 默认角色userid，不用转换
	{
		return userid_old;
	}
	else
	{
		tUserIdExchangeMap::iterator iter_seller = g_UserIdExchangeTable.find( userid_old );
		if(iter_seller == g_UserIdExchangeTable.end())
		{
			// 没有查到要转换的userid ，au倒得表有问题？
			printf("%d (userid )not find in UserIdExchangeTable\n", userid_old);
			return -1;
		}						
		return iter_seller->second;
	}

	return -1;
}

void ReadDBType(bool& is_centraldb);
class PrintLogicuidQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_userid, value_user;
		key_userid = key;
		value_user = value;

		static int count = 0;
		count ++;

		unsigned int userid = 0;
		User	user;
		try
		{
			key_userid >> userid;
			value_user >> user;
			int logicuid = user.logicuid;
			if( user.logicuid && (user.logicuid!=userid || (user.rolelist&0xFFFF)))
			{
				// LOGICUID启用前没创建过角色的帐号logicuid==userid，但这种帐号在唯一名上不会有记录
				// 这个id有可能被唯一名分配给其他帐号
				printf( "%d,%d\n", userid, logicuid );
			}
		}
		catch( ... )
		{
			printf( "PrintLogicuidQuery, error marshal, userid=%d\n", userid );
		}

		return true;
	}
};

void PrintLogicuid( )
{
	printf("#userid");
	printf(",logicuid");
	printf("\n");

	PrintLogicuidQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
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
		Log::log( LOG_ERR, "PrintLogicuid, error when walk, what=%s\n", e.what() );
	}
}

class PrintUnameroleQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_roleid, value_base;
		key_roleid = key;
		value_base = value;

		static int count = 0;
		count ++;

		int roleid = -1;
		GRoleBase	base;
		try
		{
			key_roleid >> roleid;
			value_base >> base;
			Octets  namegbk, nameesc;
			CharsetConverter::conv_charset_u2l( base.name, namegbk );
			EscapeCSVString( namegbk, nameesc );
			printf( "%.*s,%d,%d,2,0\n", nameesc.size(), (char*)nameesc.begin(), zoneid, roleid );
		}
		catch( ... )
		{
			printf( "PrintUnameroleQuery, error marshal, roleid=%d\n", roleid );
		}

		return true;
	}
};

void PrintUnamerole( int zoneid )
{
	printf("#name");
	printf(",zoneid");
	printf(",roleid");
	printf(",status");
	printf(",time");
	printf("\n");

	PrintUnameroleQuery q;
	q.zoneid = zoneid;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
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
		Log::log( LOG_ERR, "PrintUnamerole, error when walk, what=%s\n", e.what() );
	}
}

class PrintUnamefactionQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_fname, value_fid;
		key_fname = key;
		value_fid = value;

		static int count = 0;
		count ++;

		int fid = -1;
		Octets	fname;
		try
		{
			key_fname >> fname;
			value_fid >> fid;
			Octets  namegbk, nameesc;
			CharsetConverter::conv_charset_u2l( fname, namegbk );
			EscapeCSVString( namegbk, nameesc );
			printf( "%.*s,%d,%d,2,0\n", nameesc.size(), (char*)nameesc.begin(), zoneid, fid );	
		}
		catch( ... )
		{
			printf( "PrintUnamefactionQUery, error marshal, fid=%d\n", fid );
		}

		return true;
	}
};

void PrintUnamefaction( int zoneid )
{
	printf("#name");
	printf(",zoneid");
	printf(",factionid");
	printf(",status");
	printf(",time");
	printf("\n");

	PrintUnamefactionQuery q;
	q.zoneid = zoneid;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factionname" );
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
		Log::log( LOG_ERR, "PrintUnamefaction, error when walk, what=%s\n", e.what() );
	}
}

class PrintUnamefamilyQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_fname, value_fid;
		key_fname = key;
		value_fid = value;

		static int count = 0;
		count ++;

		int fid = -1;
		Octets	fname;
		try
		{
			key_fname >> fname;
			value_fid >> fid;
			Octets  namegbk, nameesc;
			CharsetConverter::conv_charset_u2l( fname, namegbk );
			EscapeCSVString( namegbk, nameesc );
			printf( "%.*s,%d,%d,2,0\n", nameesc.size(), (char*)nameesc.begin(), zoneid, fid );	
		}
		catch( ... )
		{
			printf( "PrintUnamefamilyQuery, error marshal, fid=%d\n", fid );
		}

		return true;
	}
};

void PrintUnamefamily( int zoneid )
{
	printf("#name");
	printf(",zoneid");
	printf(",fmilyid");
	printf(",status");
	printf(",time");
	printf("\n");

	PrintUnamefamilyQuery q;
	q.zoneid = zoneid;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "familyname" );
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
		Log::log( LOG_ERR, "PrintUnamefamily, error when walk, what=%s\n", e.what() );
	}
}

class GenNameIdxQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "generating rolename records counter %d...\n", count );

		RoleId		id;
		try
		{
			StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
			try
			{
				GRoleBase	base;

				key_os >> id;
				value_os >> base;

				prolename->insert( base.name, key_os, txn );
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
			Log::log( LOG_ERR, "GenNameIdxQuery, roleid=%d, what=%s\n", id.id, e.what() );
		}
		return true;
	}
};

void GenNameIdx()
{
	GenNameIdxQuery q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
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
		Log::log( LOG_ERR, "GenNameIdx, error when walk, what=%s\n", e.what() );
	}

	StorageEnv::checkpoint();
}

class ExportUniqueNamefactionQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;
	int max_factionid;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_unamefaction, key_nextid;
		key_os = key;
		value_os = value;

		int		id;
		Octets	name;
		try
		{
			key_os >> name;
			int nextid = 0;
			key_nextid << nextid;
			if( key_nextid == name )
				return true;

			value_os >> id;
			if (id > max_factionid)
				max_factionid = id;

			int status = 2;	// UNIQUENAME_USED;
			value_unamefaction << zoneid << id << status << (int)Timer::GetTime();
			StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
			Octets com_val = compressor->Update(value_unamefaction);
			pstandalone->put( name.begin(), name.size(), com_val.begin(), com_val.size() );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueNamefactionQuery, error unmarshal, fid=%d.", id );
			return true;
		}
		return true;
	}
};

class ExportUniqueNamefamilyQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;
	int max_familyid;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_unamefamily, key_nextid;
		key_os = key;
		value_os = value;

		int		id;
		Octets	name;
		try
		{
			key_os >> name;
			int nextid = 0;
			key_nextid << nextid;
			if( key_nextid == name )
				return true;

			value_os >> id;
			if (id > max_familyid)
				max_familyid = id;

			int status = 2;	// UNIQUENAME_USED;
			value_unamefamily << zoneid << id << status << (int)Timer::GetTime();
			StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
			Octets com_val = compressor->Update(value_unamefamily);
			pstandalone->put( name.begin(), name.size(), com_val.begin(), com_val.size() );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueNamefamilyQuery, error unmarshal, fid=%d.", id );
			return true;
		}
		return true;
	}
};

class ExportUniqueIdroleQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_uidrole;
		key_os = key;
		value_os = value;

		unsigned int id;
		User			user;
		try
		{
			key_os >> id;
			value_os >> user;
			if( 0 != user.logicuid && (user.logicuid!=id || (user.rolelist&0xFFFF)))
			{
				value_uidrole << user.rolelist << user.logicuid;
				StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
				Octets com_val = compressor->Update(value_uidrole);
				pstandalone->put( key.begin(), key.size(), com_val.begin(), com_val.size() );
			}
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueUidroleQuery, error unmarshal, uid=%d.", id );
			return true;
		}
		return true;
	}
};

class ExportUniqueLogicuidQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;
	int max_logicuid;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, key_logicuid, value_userid;
		key_os = key;
		value_os = value;

		unsigned int	userid;
		int	logicuid;
		User	user;
		try
		{
			key_os >> userid;
			value_os >> user;
			logicuid = user.logicuid;
			if( 0 != logicuid && (user.logicuid!=userid || (user.rolelist&0xFFFF)))
			{
				key_logicuid << logicuid;
				value_userid << userid;

				StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
				Octets com_val = compressor->Update(value_userid);
				pstandalone->put( key_logicuid.begin(), key_logicuid.size(), com_val.begin(), com_val.size() );
			}
			if (logicuid == 0)
				logicuid = userid;
			if (logicuid > max_logicuid)
				max_logicuid = logicuid;
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueLogicuidQuery, error unmarshal, uid=%d.", userid );
			return true;
		}
		return true;
	}
};

class ExportUniqueNameroleQuery : public StorageEnv::IQuery
{
public:
	int zoneid;
	DBStandalone * pstandalone;
	StorageEnv::Storage * pbase;

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os, value_unamerole, value_base;
		value_os = value;

		int		id;
		Octets	name;
		try
		{
			name = key;
			value_os >> id;

			int status = 3;	// UNIQUENAME_OBSOLETE; 旧名字
			if (pbase->find(Marshal::OctetsStream()<<id, value_base, txn))
			{
				GRoleBase base;
				value_base >> base;
				if (base.name == name)
					status = 2;	// UNIQUENAME_USED;
			}
			value_unamerole << zoneid << id << status << (int)Timer::GetTime();
			StorageEnv::Compressor	* compressor = new StorageEnv::Compressor();
			Octets com_val = compressor->Update(value_unamerole);
			pstandalone->put( name.begin(), name.size(), com_val.begin(), com_val.size() );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportUniqueNameroleQuery, error unmarshal, fid=%d.", id );
			return true;
		}
		return true;
	}
};

bool SetLogicuidNextid(DBStandalone * pstandalone, int nextid )
{                       
	StorageEnv::Uncompressor * uncompressor = new StorageEnv::Uncompressor();
	StorageEnv::Compressor * compressor = new StorageEnv::Compressor();
	try
	{
		Marshal::OctetsStream value_logicuid, value_nextid;
		int temp = 0;
		Marshal::OctetsStream t;
		t << temp;
		size_t val_len;
		if ( void *val = pstandalone->find( t.begin(), t.size(), &val_len ) )
		{       
			GNET::Octets dbval = uncompressor->Update(GNET::Octets(val, val_len));
			free(val);
			int logicuid;
			Marshal::OctetsStream(dbval) >> logicuid;
			printf( "old nextid record: logicuid = %d\n", logicuid );
		}
		else
		{
			printf( "old nextid not found.\n" );
		}
		value_nextid << nextid;
		Octets com_val = compressor->Update(value_nextid);
		pstandalone->put( t.begin(), t.size(), com_val.begin(), com_val.size() );
	}
	catch ( Marshal::Exception &)
	{
		Log::log( LOG_ERR, "SetLogicuidNextid, unmarshall error");
		delete uncompressor;
		delete compressor;
		return false;
	}
	delete uncompressor;
	delete compressor;
	return true;
}
bool SetFactionNextid(DBStandalone * pstandalone, int nextid )
{
	StorageEnv::Uncompressor * uncompressor = new StorageEnv::Uncompressor();
	StorageEnv::Compressor * compressor = new StorageEnv::Compressor();
	try
	{
		Marshal::OctetsStream  value_nextid;
		int temp = 0;
		Marshal::OctetsStream t;
		t << temp;

		size_t val_len;
		if ( void *val = pstandalone->find( t.begin(), t.size(), &val_len ) )
		{       
			GNET::Octets dbval = uncompressor->Update(GNET::Octets(val, val_len));
			free(val);
			int zoneid, factionid, status, time;
			Marshal::OctetsStream(dbval) >> zoneid >> factionid >> status >> time;
			printf( "old nextid record: zoneid = %d, factionid = %d, status=%d, time=%d\n",	zoneid, factionid, status, time );
		}
		else
		{
			printf( "old nextid not found.\n" );
		}
		int status = 2;//UNIQUENAME_USED
		value_nextid << temp << nextid << status << temp;
		Octets com_val = compressor->Update(value_nextid);
		pstandalone->put( t.begin(), t.size(), com_val.begin(), com_val.size() );
		//			pstandalone->insert( t, value_nextid, txn );
	}
	catch (Marshal::Exception &)
	{
		Log::log(LOG_ERR, "Set nextid, error unmarshal");
		delete uncompressor;
		delete compressor;
		return false;
	}
	delete uncompressor;
	delete compressor;
	return true;
}
void ExportUnique( int zoneid )
{
	{
		printf( "export unique unamefaction.\n" );
		ExportUniqueNamefactionQuery q;
		q.zoneid = zoneid;
		q.max_factionid = 0;
		q.pstandalone = new DBStandalone( "unamefaction" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "factionname" );
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
			Log::log( LOG_ERR, "ExportUnique unamefaction, error when walk, what=%s\n", e.what() );
		}
		int next_factionid = q.max_factionid + 1;
		printf( "Set next factionid to be allocated %d\n",next_factionid );
		SetFactionNextid(q.pstandalone, next_factionid );
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique unamefamily.\n" );
		ExportUniqueNamefamilyQuery q;
		q.zoneid = zoneid;
		q.max_familyid = 0;
		q.pstandalone = new DBStandalone( "unamefamily" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "familyname" );
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
			Log::log( LOG_ERR, "ExportUnique unamefamily, error when walk, what=%s\n", e.what() );
		}
		int next_familyid = q.max_familyid + 1;
		printf( "Set next familyid to be allocated %d\n",next_familyid);
		SetFactionNextid(q.pstandalone, next_familyid);
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique uidrole.\n" );
		ExportUniqueIdroleQuery q;
		q.zoneid = zoneid;
		q.pstandalone = new DBStandalone( "uidrole" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
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
			Log::log( LOG_ERR, "ExportUnique uidrole, error when walk, what=%s\n", e.what() );
		}
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique logicuid.\n" );
		ExportUniqueLogicuidQuery q;
		q.zoneid = zoneid;
		q.max_logicuid = 0;
		q.pstandalone = new DBStandalone( "logicuid" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
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
			Log::log( LOG_ERR, "ExportUnique logicuid, error when walk, what=%s\n", e.what() );
		}
//		int next_logicuid = q.max_logicuid + 16;
//		printf( "Set next logicuid to be allocated %d\n",next_logicuid);
//		SetLogicuidNextid(q.pstandalone, next_logicuid);
//		保持 nextid 为 null, 唯一名会自动从 LOGICUID_START 搜寻可用的 logicuid
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}

	{
		printf( "export unique unamerole.\n" );
		ExportUniqueNameroleQuery q;
		q.zoneid = zoneid;
		q.pstandalone = new DBStandalone( "unamerole" );
		q.pstandalone->init();
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "rolename" );
			StorageEnv::Storage * pbase = StorageEnv::GetStorage( "base" );
			q.pbase = pbase;
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
			Log::log( LOG_ERR, "ExportUnique unamerole, error when walk, what=%s\n", e.what() );
		}
		q.pstandalone->checkpoint();
		delete q.pstandalone;
		q.pstandalone = NULL;
	}
}

void DisplayRoleInfo(GRoleBase& base, GRoleDetail& role)
{
	Octets name;
	CharsetConverter::conv_charset_u2l( role.name, name );

	time_t dt = base.delete_time, ct = base.create_time, lt = base.lastlogin_time;

	printf("\nBase:\n");
	printf("\tID:%d\n",role.id);
	printf("\tname:%.*s\n",name.size(),(char*)name.begin());
	printf("\tgender:%d\n",base.gender);
	printf("\tstatus:%d\n",base.status);
	printf("\tdelete_time:%s",dt>0 ? ctime(&dt) : "-\n");
	printf("\tcreate_time:%s",ct>0 ? ctime(&ct) : "-\n");
	printf("\tlastlogin_time:%s",lt>0 ? ctime(&lt) : "-\n");
	printf("\tforbid.size:%d",base.forbid.size());

	printf("\nStatus:\n");
	printf("\tlevel:%d\n",role.status.level);
	printf("\texp:%lld\n",role.status.exp);
	printf("\tpp:%d\n",role.status.pp);
	printf("\thp:%d\n",role.status.hp);
	printf("\tmp:%d\n",role.status.mp);
	printf("\tposx:%4.1f\n",role.status.posx);
	printf("\tposy:%4.1f\n",role.status.posy);
	printf("\tposz:%4.1f\n",role.status.posz);
	printf("\tpkvalue:%d\n",role.status.pkvalue);
	printf("\tworldtag:%d\n",role.status.worldtag);
	printf("\ttime_used:%d\n",role.status.time_used);
	printf("\treputation:%d\n",role.status.reputation);
	printf("\tproduceskill:%d\n",role.status.produceskill);
	printf("\tproduceexp:%d\n",role.status.produceexp);
	printf("\tcustom_status.size:%d\n", role.status.custom_status.size() );
	printf("\tfilter_data.size:%d\n",role.status.filter_data.size());
	printf("\tcharactermode.size:%d\n",role.status.charactermode.size());
	printf("\tinstancekeylist.size:%d\n",role.status.instancekeylist.size());
	printf("\tdbltime_data.size:%d\n", role.status.coolingtime.size() );
	printf("\tpetcorral.size:%d\n",role.status.petcorral.size());
	printf("\tvar_data.size:%d\n", role.status.var_data.size() );
	printf("\tskills.size:%d\n", role.status.skills.size() );
	printf("\tstorehousepasswd.size:%d\n", role.status.storehousepasswd.size() );
	printf("\tcoolingtime.size:%d\n", role.status.coolingtime.size() );
	printf("\trecipes.size:%d\n", role.status.recipes.size() );
	printf("\twaypointlist.size:%d\n", role.status.waypointlist.size() );

	printf("\bStorehouse:\n");
	printf("\tstorehouse.money:%d\n", role.storehouse.money );
	printf("\tstorehouse.size:%d\n\n", role.storehouse.items.size() );

	printf("\nInventory:\n");
	printf("\tinventory.money:%d\n", role.inventory.money );
	printf("\tinventory.size:%d\n", role.inventory.items.size() );

	printf("\nEquipment:\n");
	printf("\tequipment.size:%d\n", role.inventory.equipment.size() );

	printf("\nTaskinventory:\n");
	printf("\ttaskinventory.size:%d\n", role.task.task_inventory.size() );

	printf("\nTaskdata:\n");
	printf("\ttask_data.size:%d\n", role.task.task_data.size() );

	printf("\nTaskcomplete:\n");
	printf("\ttask_complete.size:%d\n", role.task.task_complete.size() );

	printf("\n");
}

bool QueryRole( int roleid )
{
	printf( "QueryRole Begin\n" );
	GRoleDetail	detail;

	Marshal::OctetsStream key_userid, key, value_user, value_base, value_status;
	Marshal::OctetsStream value_inventory, value_equipment, value_task, value_storehouse;

	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::AtomTransaction txn;
		try
		{
			key << roleid;
			GRoleBase	base;
			value_base = pbase->find( key, txn );
			value_base >> base;
			int userid = (0==base.userid ? (LOGICUID(base.id)) : base.userid);
			key_userid << userid;

			if( puser->find( key_userid, value_user, txn ) )
			{
				User	user;
				value_user >> user;
				printf( "userid = %d, user rolelist = %x.\n", userid, user.rolelist );
			}
			else
			{
				printf( "no user found for userid %d.\n", userid );
			}

			GRoleStatus	status;

			if (base.status == _ROLE_STATUS_READYDEL)
			{
				if(GameDBManager::GetInstance()->MustDelete(base))
					base.status =  _ROLE_STATUS_MUSTDEL;
			}
			GRoleBaseToDetail( base, detail );
			printf( "QueryRole base, size=%d\n", value_base.size() );

			if( pstatus->find( key, value_status, txn ) )
			{
				value_status >> detail.status;
			}
			printf( "QueryRole status, size=%d\n", value_status.size() );

			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;
			printf( "QueryRole inventory, size=%d\n", value_inventory.size() );

			if(ptask->find( key, value_task, txn ))
			{
				value_task >> detail.task;
			}
			printf( "QueryRole task, size=%d\n", value_task.size() );

			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;
			printf( "QueryRole storehouse, size=%d\n", value_storehouse.size() );

			printf( "QueryRole task, size=%d\n", detail.task.task_data.size() );

			printf( "QueryRole taskcomplete, size=%d\n", detail.task.task_complete.size() );

			printf( "QueryRole display\n" );
			DisplayRoleInfo( base, detail );
			return true;
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
		Log::log( LOG_ERR, "QueryRole, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

void DisplayRoleInfoCsv(GRoleBase& base, GRoleDetail& role)
{
	Octets name, name2;
	CharsetConverter::conv_charset_u2l( role.name, name );
	EscapeCSVString( name, name2 );

	time_t dt = base.delete_time, ct = base.create_time, lt = base.lastlogin_time;

	printf("%d",role.id);
	printf(",%d",0==base.userid ? (LOGICUID(base.id)) : base.userid );
	printf(",%.*s",name2.size(),(char*)name2.begin());
	printf(",%d",role.status.occupation);
	printf(",%d",role.gender);
	printf(",%d",base.status);
	if( dt > 0 )
	{
		struct tm * l = localtime(&dt);
		printf(",%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+l->tm_year,1+l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	}
	else
		printf(",");
	if( ct > 0 )
	{
		struct tm * l = localtime(&ct);
		printf(",%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+l->tm_year,1+l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	}
	else
		printf(",");
	if( lt > 0 )
	{
		struct tm * l = localtime(&lt);
		printf(",%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
				1900+l->tm_year,1+l->tm_mon,l->tm_mday,l->tm_hour,l->tm_min,l->tm_sec);
	}
	else
		printf(",");
	printf(",%d",base.forbid.size());

	printf(",%d",role.status.level);
	printf(",%lld",role.status.exp);
	printf(",%d",role.status.pp);
	printf(",%d",role.status.hp);
	printf(",%d",role.status.mp);
	printf(",%4.1f",role.status.posx);
	printf(",%4.1f",role.status.posy);
	printf(",%4.1f",role.status.posz);
	printf(",%d",role.status.pkvalue);
	printf(",%d",role.status.worldtag);
	printf(",%d",role.status.time_used);
	printf(",%d",role.status.reputation);
	printf(",%d",role.status.produceskill);
	printf(",%d",role.status.produceexp);
	printf(",%d",role.status.custom_status.size());
	printf(",%d",role.status.filter_data.size());
	printf(",%d",role.status.charactermode.size());
	printf(",%d",role.status.instancekeylist.size());
	printf(",%d",role.status.dbltime_data.size());
	printf(",%d",role.status.petcorral.size());
	printf(",%d", role.status.var_data.size() );
	printf(",%d", role.status.skills.size() );
	printf(",%d", role.status.storehousepasswd.size() );
	printf(",%d", role.status.coolingtime.size() );
	printf(",%d", role.status.recipes.size() );
	printf(",%d", role.status.waypointlist.size() );

	printf(",%d", role.storehouse.money );
	printf(",%d", role.storehouse.items.size() );

	printf(",%d", role.inventory.money );
	printf(",%d", role.inventory.items.size() );

	printf(",%d", role.inventory.equipment.size() );

	printf(",%d", role.task.task_inventory.size() );

	printf(",%d", role.task.task_data.size() );

	printf(",%d", role.task.task_complete.size() );

	printf("\n");
}

bool QueryRoleCsv( int roleid, StorageEnv::Transaction& txn )
{
	GRoleDetail	detail;

	Marshal::OctetsStream key, value_base, value_status;
	Marshal::OctetsStream value_inventory, value_equipment, value_task, value_storehouse;

	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		try
		{
			key << roleid;

			GRoleBase	base;
			GRoleStatus	status;

			value_base = pbase->find( key, txn );
			value_base >> base;
			if (base.status == _ROLE_STATUS_READYDEL)
			{
				if(GameDBManager::GetInstance()->MustDelete(base))
					base.status =  _ROLE_STATUS_MUSTDEL;
			}
			GRoleBaseToDetail( base, detail );

			if( pstatus->find( key, value_status, txn ) )
				value_status >> detail.status;

			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;

			if( ptask->find( key, value_task, txn ) )
				value_task >> detail.task;

			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;

			DisplayRoleInfoCsv( base, detail );
			return true;
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
		Log::log( LOG_ERR, "QueryRoleCsv, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

class ListRoleCsvQuery : public StorageEnv::IQuery
{
public:
	int count;
	bool hasmore;
	Octets handle;
	ListRoleCsvQuery():count(0),hasmore(false){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		if(++count % checkpoint_threshold == 0)
		{       
			handle = key;
			hasmore = true;
			return false;
		} 
		Marshal::OctetsStream key_os;
		key_os = key;

		RoleId		id;
		try
		{
			key_os >> id;
			QueryRoleCsv( id.id, txn );
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListRoleCsvQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListRole( )
{
	printf("roleid");
	printf(",userid");
	printf(",name");
	printf(",occupation");
	printf(",gender");
	printf(",status");
	printf(",delete_time");
	printf(",create_time");
	printf(",lastlogin_time");
	printf(",forbid_size");

	printf(",level");
	printf(",exp");
	printf(",pp");
	printf(",hp");
	printf(",mp");
	printf(",posx");
	printf(",posy");
	printf(",posz");
	printf(",pkvalue");
	printf(",worldtag");
	printf(",time_used");
	printf(",reputation");
	printf(",produceskill");
	printf(",produceexp");
	printf(",custom_status_size");
	printf(",filter_data_size");
	printf(",charactermode_size");
	printf(",instancekeylist_size");
	printf(",dbltime_data_size");
	printf(",petcorral_size");
	printf(",var_data_size");
	printf(",skills_size");
	printf(",storehousepasswd_size");
	printf(",coolingtime_size");
	printf(",recipes");
	printf(",waypointlist_size");

	printf(",storehouse_money");
	printf(",storehouse_size");

	printf(",inventory_money");
	printf(",inventory_size");

	printf(",equipment_size");

	printf(",taskinventory_size");

	printf(",task_data_size");

	printf(",task_complete_size");

	printf("\n");

	ListRoleCsvQuery q;
	do
	{
		q.hasmore = false;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
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
			Log::log( LOG_ERR, "ListRole, error when walk, what=%s\n", e.what() );
		}

		StorageEnv::checkpoint();
	}while(q.hasmore);
}

void DisplayRoleInfoBrief(GRoleDetail& role)
{
	Octets name, name2;
	CharsetConverter::conv_charset_u2l( role.name, name );
	EscapeCSVString( name, name2 );

	printf("%d",role.id);
	printf(",%d",0==role.userid ? (LOGICUID(role.id)) : role.userid );
	printf(",%.*s",name2.size(),(char*)name2.begin());
	printf(",%d",role.status.occupation);
	printf(",%d",role.status.level);
	printf(",%lld",role.status.exp);
	printf(",%d",role.inventory.money+role.storehouse.money);
	printf(",%d",role.status.reputation);
	printf(",%d",role.status.reborndata.size()/sizeof(int));
	printf(",%d",role.status.updatetime);
	printf(",%d",role.status.worldtag);
	printf(",%f",role.status.posx);
	printf(",%f",role.status.posy);
	printf(",%f",role.status.posz);
	printf(",%lld",role.base2.comsumption);
	printf(",%d",role.gender);
	printf("\n");
}

bool QueryRoleBrief( int roleid, GRoleBase & base, int & money, StorageEnv::Transaction& txn )
{
	GRoleDetail	detail;

	Marshal::OctetsStream key, value_base, value_status;
	Marshal::OctetsStream value_inventory, value_storehouse, value_base2;

	try
	{
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * pinventory  = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
		try
		{
			key << roleid;

			GRoleBaseToDetail( base, detail );

			if( pstatus->find( key, value_status, txn ) )
				value_status >> detail.status;
 
			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;

			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;

			if( pbase2->find(key, value_base2, txn) )
				value_base2 >> detail.base2;

 			money = detail.inventory.money + detail.storehouse.money;
			DisplayRoleInfoBrief( detail );
			return true;
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
		Log::log( LOG_ERR, "QueryRoleBrief, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}

class ListRoleBriefQuery : public StorageEnv::IQuery
{
public:
	time_t listthresh;
	int64_t	money_total;
	int count;
	bool hasmore;
	Octets handle;
	ListRoleBriefQuery():listthresh(0),money_total(0),count(0),hasmore(false){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId		id;
		GRoleBase	base;
		try
		{
			key_os >> id;
			value_os >> base;
			if (base.lastlogin_time < listthresh)
				return true;
			if(++count % checkpoint_threshold == 0)
			{       
				handle = key;
				hasmore = true;
				return false;
			}
			int money;
			QueryRoleBrief( id.id, base, money, txn );
			money_total += money;

		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListRoleBriefQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListRoleBrief(const char * date)
{
	time_t listthreshold = 0; //只list上次登录时间>=listthreshold的玩家 listthreshold为0表示list全部玩家
	if (date != NULL)
	{
		struct tm dt;
		memset(&dt,0,sizeof(dt));
		if (!strptime(date,"%Y-%m-%d", &dt) || dt.tm_year<102)
		{
			LOG_TRACE("Error: invalid date %s used", date);
			return;
		}
		listthreshold = mktime(&dt);
	}
	if (listthreshold == 0)
		LOG_TRACE("list all roles");
	else
		LOG_TRACE("list roles that logged in after %s, listthreshold=%d", date, listthreshold);
	printf("roleid");
	printf(",userid");
	printf(",name");
	printf(",occupation");
	printf(",level");
	printf(",exp");
	printf(",moneyall");
	printf(",reputation");
	printf(",type");
	printf(",save_time");
	printf(",mapid");
	printf(",x");
	printf(",y");
	printf(",z");
	printf(",consumption");
	printf(",gender");
	printf("\n");

	ListRoleBriefQuery q;
	q.listthresh = listthreshold;
	do
	{
		q.hasmore = false;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
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
			Log::log( LOG_ERR, "ListRoleBrief, error when walk, what=%s\n", e.what() );
		}

		StorageEnv::checkpoint();
	}while(q.hasmore);

	printf("0,0,\"\",0,0,0,%lld\n",q.money_total);
}

struct ListRoleMoneyConsignQuery : public StorageEnv::IQuery
{
	std::map<int,int64_t>& money_map;//roleid->money
	ListRoleMoneyConsignQuery(std::map<int,int64_t> &moneymap):money_map(moneymap){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os;
		key_os = key;
		Marshal::OctetsStream value_os;
		value_os = value;

		int64_t sn;
		GConsignDB detail;
		try
		{
			key_os >> sn;
			if(sn==0)
				return true;
			value_os >> detail;
					
			if(detail.info.consign_type == CONSIGNTYPE_MONEY)
			{
				if(detail.info.state != DSTATE_SOLD || detail.mail_status != BUYER_SENDED)
				{
					int roleid = (detail.info.state != DSTATE_SOLD)? detail.seller_roleid: detail.buyer_roleid;
					money_map[roleid]+=detail.info.money;
				}
				//calc margin for done consign
				if(detail.info.state == DSTATE_POST_FORCE_CANCEL ||\
					detail.info.state == DSTATE_POST_FAIL ||\
					(detail.info.state == DSTATE_SOLD && detail.mail_status !=SELLER_SENDED))
				{
					money_map[detail.seller_roleid]+=detail.margin;
				}
			}

		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListRoleMoneyConsignQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};

struct ListRoleMoneyQuery : public StorageEnv::IQuery
{
	std::map<int,int64_t>& money_map;//roleid->money
	StorageEnv::Storage *pinventory;
	StorageEnv::Storage *pstorehouse;
	StorageEnv::Storage *pmailbox;
	StorageEnv::Storage *pbase2;
	int64_t money_total;
	int zoneid;
	bool is_central;

	ListRoleMoneyQuery(std::map<int,int64_t> &moneymap,StorageEnv::Storage *pinv ,StorageEnv::Storage *pstore,StorageEnv::Storage *pmail,StorageEnv::Storage *pb2,int zid,bool central):money_map(moneymap),pinventory(pinv),pstorehouse(pstore),pmailbox(pmail),pbase2(pb2),zoneid(zid),is_central(central)
	{ money_total = 0;}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		GRoleBase base;	
		GRolePocket pocket;
		GRoleStorehouse storehouse;
		GMailBox mailbox;
		GRoleBase2 base2;
		int roleid=0;
		int64_t money=0;
		Marshal::OctetsStream keyos, value_base;
		Marshal::OctetsStream value_inventory, value_storehouse, value_mailbox, value_base2;
	
		try
		{
			try
			{
				keyos=key;
				keyos >> roleid;
				if(roleid>0 && roleid<128)
					return true;
	
				value_base=value;
				value_base>>base;
	
				if(is_central && base.status == _ROLE_STATUS_FROZEN)//skip off-server role in central server
					return true;
		
				int pocketmoney=0;
				int storemoney=0;
	 			//skip off-server role's pocket and storehouse 
				if (base.status != _ROLE_STATUS_FROZEN)
				{
					if( pinventory->find( keyos, value_inventory, txn ) )
						value_inventory >> pocket;
	
					if( pstorehouse->find( keyos, value_storehouse, txn ) )
						value_storehouse >> storehouse;
					pocketmoney=pocket.money;
					storemoney=storehouse.money;
	 				money += pocketmoney;
					money += storemoney;
				}
	
				int64_t conmoney=0;
				int64_t mailmoney=0;
				int real_zoneid=zoneid;
				//calc mail and consign money for normal server
				if(!is_central)
				{
					std::map<int,int64_t>::iterator monit=money_map.find(roleid);
					if(monit!=money_map.end())
						conmoney=monit->second;
	
					if( pmailbox->find( keyos, value_mailbox, txn ) )
						value_mailbox >> mailbox;
					GMailVector::iterator mit = mailbox.mails.begin();
					for (; mit != mailbox.mails.end(); ++mit)
					{
						if(mit->header.attribute & (1 << _MA_ATTACH_MONEY))
							mailmoney+=mit->attach_money;
					}
					money+=conmoney;
					money+=mailmoney;
				}
				else //if in central server, get src server zoneid
				{
					if( pbase2->find( keyos, value_base2, txn ) )
					{
						value_base2>>base2;
						real_zoneid = base2.src_zoneid;
					}
				}
				money_total += money;
	
//				printf("roleid=%d,leave_server=%d,zoneid=%d,",roleid,base.status == _ROLE_STATUS_FROZEN,real_zoneid);
//				printf("money=%lld\n",money);
				printf("%d,%d,%d,",roleid,base.status == _ROLE_STATUS_FROZEN,real_zoneid);
				printf("%lld\n",money);
				return true;
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
			Log::log( LOG_ERR, "ListRoleMoneyQuery, roleid=%d, what=%s\n", roleid, e.what() );
			return true;
		}
		return true;
	}
};

void ReadDBType(bool& is_centraldb)
{
	try {   
		StorageEnv::Storage *pstorage = StorageEnv::GetStorage("config");
		StorageEnv::CommonTransaction txn;
		Marshal::OctetsStream value;
		try {
			DBConfig config;
			Marshal::OctetsStream(pstorage->find(Marshal::OctetsStream()<<(int)100, txn)) >> config;
			is_centraldb = config.is_central_db;
			if (is_centraldb)
				//printf("#DB type : Central DB\n");
				std::cerr << "#DB type : Central DB" << std::endl;
			else
				//printf("#DB type : Normal DB\n");
				std::cerr << "#DB type : Normal DB" << std::endl;
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
		Log::log( LOG_ERR, "ReadDBType exception, what=%s\n", e.what() );
		is_centraldb = false;
	}
}

void ListRoleMoney(int zoneid)
{
	bool is_centraldb=false;
	ReadDBType(is_centraldb);
	printf("roleid");
	printf(",leave_server");//0 in server,1 leave server
	printf(",zoneid");
	printf(",money");
	printf("\n");

	int64_t money_total = 0;
	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage( "base" );
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage( "inventory" );
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage( "storehouse" );
		StorageEnv::Storage * pmailbox = StorageEnv::GetStorage( "mailbox" );
		StorageEnv::Storage * pconsign = StorageEnv::GetStorage( "consign" );
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage( "base2" );
		StorageEnv::AtomTransaction	txn;
		
		std::map<int/*roleid*/,int64_t/*money*/> role_consign_money;
		ListRoleMoneyConsignQuery qconsign(role_consign_money);
		ListRoleMoneyQuery q(role_consign_money,pinventory,pstorehouse,pmailbox,pbase2,zoneid,is_centraldb);
		q.money_total = money_total;
		
		try
		{
			if(!is_centraldb){
				StorageEnv::Storage::Cursor cursor = pconsign->cursor( txn );
				cursor.walk(qconsign);//calc money in consign table and save to role_consign_money map
			}

			StorageEnv::Storage::Cursor cursor1 = pbase->cursor( txn );
			cursor1.walk(q);//calc total money for each role
			money_total = q.money_total;
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
		Log::log( LOG_ERR, "ListRoleMoney, error when walk, what=%s\n", e.what() );
	}

	printf("total money:%lld\n",money_total);
}

void PrintInventorySnapshotField()
{
	printf("roleid");
	printf(",item_position");
	printf(",item_id");
	printf(",item_data"); // 记录的是加密后的数据
	printf(",item_checksum");
	printf("\n");
}

enum ITEM_POSITION
{
	PLAYER_POCKET = 0,
	PLAYER_EQUIPMENT,
	PLAYER_PETBADGE,
	PET_EQUIPMENT,
	PLAYER_FASHIONPOCKET,
	PLAYER_STOREHOUSE,
	PET_POCKET,
	PLAYER_MAILBOX,
	PLAYER_CONSIGN,
	MAX_ITEM_POSITION,
};

void PrintInventoryInfo(GRoleInventory &inventory);
bool IsValuables(int itemid);
bool IsLoginRecentWeek(int roleid);
class InventoryQuery : public StorageEnv::IQuery
{
	int roleid; //是否只扫描指定角色
public:
	InventoryQuery(int r) : roleid(r) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GRolePocket inventory;
		try
		{
			key_os >> id;
			if(!IsLoginRecentWeek(id.id))
			{
				return roleid == 0;
			}
			value_os >> inventory;
//			for(ITEM_POSITION i = PLAYER_POCKET; i < PLAYER_STOREHOUSE; (((int)i)++))
			for(int i = PLAYER_POCKET; i < PLAYER_STOREHOUSE; i++)
			{
				GRoleInventoryVector &items = GetRoleInventoryVector(inventory, i);
				for(unsigned int j = 0; j < items.size(); j++)
				{
					if((PLAYER_POCKET == i && !IsValuables(items[j].id)))
				      		continue;
					printf("%d,", id.id);
					printf("%d,", (int)i);
					PrintInventoryInfo(items[j]);
				}
			}
			GPocketInventoryVector &pet_pocket = inventory.pocket_items;
			for(size_t pos = 0; pos < pet_pocket.size(); pos++)
			{
				printf("%d,", id.id);
				printf("%d,", PET_POCKET);
				printf("%d,", pet_pocket[pos].id);
				printf(",");
				printf("\n");
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "InventoryQuery, error unmarshal, roleid=%d.", id.id);
			return roleid == 0;
		}
		return roleid == 0;
	}

//	GRoleInventoryVector &GetRoleInventoryVector(GRolePocket &inventory, ITEM_POSITION pos)
	GRoleInventoryVector &GetRoleInventoryVector(GRolePocket &inventory, int pos)
	{
		switch(pos)
		{
		case PLAYER_POCKET:
			return inventory.items;
		case PLAYER_EQUIPMENT:
			return inventory.equipment;
		case PLAYER_PETBADGE:
			return inventory.petbadge;
		case PET_EQUIPMENT:
			return inventory.petequip;
		case PLAYER_FASHIONPOCKET:
		default:
			return inventory.fashion;
		}
	}
};

class StorehouseQuery : public StorageEnv::IQuery
{
	int roleid; //是否只扫描指定角色
public:
	StorehouseQuery(int r) : roleid(r) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GRoleStorehouse storehouse;
		try
		{
			key_os >> id;
			if(!IsLoginRecentWeek(id.id))
			{
				return roleid == 0;
			}
			value_os >> storehouse;

			for(unsigned int i = 0; i < storehouse.items.size(); i++)
			{
				if(IsValuables(storehouse.items[i].id))
				{
					printf("%d,", id.id);
					printf("%d,", PLAYER_STOREHOUSE);
					PrintInventoryInfo(storehouse.items[i]);
				}
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "StorehouseQuery, error unmarshal, roleid=%d.", id.id);
			return roleid == 0;
		}
		return roleid == 0;
	}
};

class MailboxQuery : public StorageEnv::IQuery
{
	int roleid; //是否只扫描指定角色
public:
	MailboxQuery(int r) : roleid(r) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId id;
		GMailBox mailbox;
		try
		{
			key_os >> id;
			if(!IsLoginRecentWeek(id.id))
			{
				return roleid == 0;
			}
			value_os >> mailbox;

			for(unsigned int i = 0; i < mailbox.mails.size(); i++)
			{
				if(IsValuables(mailbox.mails[i].attach_obj.id))
				{
					printf("%d,", id.id);
					printf("%d,", PLAYER_MAILBOX);
					PrintInventoryInfo(mailbox.mails[i].attach_obj);
				}
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "MailboxQuery, error unmarshal, roleid=%d.", id.id);
			return roleid == 0;
		}
		return roleid == 0;
	}
};

class ConsignQuery : public StorageEnv::IQuery
{
	int roleid; //是否只扫描指定角色
public:
	ConsignQuery(int r) : roleid(r) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		int64_t sn;
		GConsignDB consign;
		try
		{
			key_os >> sn;
			if(0 == sn)
			{
				return true;
			}
			value_os >> consign;
			if(!IsLoginRecentWeek(consign.seller_roleid))
			{
				return true;
			}

			if(IsValuables(consign.item.id))
			{
				if (roleid == 0 || roleid == consign.seller_roleid)
				{
					printf("%d,", consign.seller_roleid);
					printf("%d,", PLAYER_CONSIGN);
					PrintInventoryInfo(consign.item);
				}
			}
		}
		catch(Marshal::Exception &)
		{
			Log::log(LOG_ERR, "ConsignQuery, error unmarshal, sn=%d.", sn);
			return true;
		}
		return true;
	}
};

class QueryFactory
{
public:
	static StorageEnv::IQuery* Create(const string &table_name, int roleid)
	{
		if("inventory" == table_name)
		{
			return new InventoryQuery(roleid);
		}
		else if("storehouse" == table_name)
		{
			return new StorehouseQuery(roleid);
		}
		else if("mailbox" == table_name)
		{
			return new MailboxQuery(roleid);
		}
		else if("consign" == table_name)
		{
			return new ConsignQuery(roleid);
		}
		else
		{
			return NULL;
		}
	}
};

class QueryManager
{
	int roleid;
public:
#define SECONDS_PERDAY 86400
#define DAYS_PERWEEK 7
	typedef std::map<StorageEnv::Storage*, StorageEnv::IQuery*>::iterator Iterator;
	QueryManager(int r) : roleid(r) {}
	~QueryManager()
	{
		if(transaction)
		{
			delete transaction;
			transaction = NULL;
		}
		for(Iterator it = storages.begin(); it != storages.end(); it++) // 删除IQuery
		{
			delete it->second;
		}
		storages.clear();
	}
	void LoadStorage(std::vector<string> &table_names)
	{
		for(size_t i = 0; i < table_names.size(); i++)
		{
			if(table_names[i] == "base")
			{
				pbase = StorageEnv::GetStorage("base");
				continue;
			}
			storages[StorageEnv::GetStorage(table_names[i].c_str())] = QueryFactory::Create(table_names[i], roleid);
		}
		transaction = new StorageEnv::Transaction;
	}
	void BuildSnapshot()
	{
		for(Iterator it = storages.begin(); it != storages.end(); it++)
		{
			StorageEnv::Storage::Cursor cursor = it->first->cursor(*transaction);
			if (dynamic_cast<const ConsignQuery *>(it->second) || roleid == 0)
				cursor.walk(*(it->second));//consign 信息遍历全表
			else
				cursor.walk(Marshal::OctetsStream()<<roleid, *(it->second));
		}
	}
	static void InitSecurity(Security::Type type, Octets &key)
	{
		sec_key = key;
		sec_type = type;
	}
	static void LoadValuablesList(const char *filename)
	{
		std::ifstream ifs(filename);
		string line;
		if(!valuables_list.empty())
		{
			valuables_list.clear();
		}
		while(std::getline(ifs, line))
		{
			int itemid = atoi(line.c_str());
			if(0 != itemid)
			{
				valuables_list.insert(itemid);
			}
		}
	}
	static Octets &Encrypt(Octets &value)
	{
		Security *sec = Security::Create(sec_type);
		sec->SetParameter(sec_key);
		sec->Update(value);
		sec->Destroy();
		return value;
	}
	static bool IsValuables(int itemid)
	{
		return valuables_list.find(itemid) == valuables_list.end() ? false : true;
	}
	static bool IsLoginRecentWeek(int roleid)
	{
		Marshal::OctetsStream key, value;
		try
		{
			try
			{
				key << roleid;
				GRoleBase base;
				if(!pbase->find(key, value, *transaction))
				{
					return false;
				}
				value >> base;

				int now = time(NULL);
				int time = base.lastlogin_time;
				return ((time != 0) && (now - time <= DAYS_PERWEEK * SECONDS_PERDAY)) ? true : false;
			}
			catch(DbException e)
			{
				throw;
			}
			catch(...)
			{
				DbException e(DB_OLD_VERSION);
				transaction->abort(e);
				throw e;
			}
		}
		catch(DbException e)
		{
			Log::log(LOG_ERR, "listroleinventory, roleid=%d, what=%s\n", roleid, e.what());
			return false;
		}
		return false;
	}
private:
	std::map<StorageEnv::Storage*, StorageEnv::IQuery *> storages;
	static StorageEnv::Transaction *transaction;
	static Octets sec_key;
	static Security::Type sec_type;
	static std::set<int> valuables_list;
	static StorageEnv::Storage *pbase;
};

std::set<int> QueryManager::valuables_list;
Security::Type QueryManager::sec_type = NULLSECURITY;
Octets QueryManager::sec_key;
StorageEnv::Transaction *QueryManager::transaction = NULL;
StorageEnv::Storage *QueryManager::pbase = NULL;

bool IsValuables(int itemid)
{
	return QueryManager::IsValuables(itemid);
}

bool IsLoginRecentWeek(int roleid)
{
	return QueryManager::IsLoginRecentWeek(roleid);
}

void PrintInventoryInfo(GRoleInventory &inventory)
{
	printf("%d,", inventory.id);
	Marshal::OctetsStream os;
	os << inventory;
	Octets checksum = MD5Hash::Digest(os);
	QueryManager::Encrypt(os);
	XmlCoder coder;
	printf("%s,", coder.toString(os).c_str());
	printf("%s\n", coder.toString(checksum).c_str());
}

void ListRoleInventory(int roleid) //roleid = 0 表示扫描全库
{
	PrintInventorySnapshotField();
	try
	{
		std::vector<string> table_names;
		table_names.push_back("inventory");
		table_names.push_back("storehouse");
		table_names.push_back("mailbox");
		table_names.push_back("consign");	
		table_names.push_back("base");	

		char key_buf[128] = {0xbf, 0x61, 0x09, 0x72, 0x1d, 0xbb, 0x6e, 0xe9, 0x9a, 0x01, 0x3d, 0x47, 0x0a, 0x11, 0x60, 0xbe, 
				     0xf8, 0x48, 0x36, 0x3f, 0xdf, 0xf5, 0x9a, 0xc5, 0xca, 0x1c, 0x79, 0xcb, 0xbb, 0x90, 0x96, 0xde, 
				     0x58, 0xff, 0x60, 0xbe, 0x8d, 0x4e, 0xd8, 0xe1, 0x83, 0x47, 0xd0, 0x57, 0x6d, 0x62, 0x49, 0x66, 
				     0x71, 0x0d, 0x6a, 0xfe, 0x3b, 0xdc, 0xd2, 0x33, 0xd7, 0x8f, 0x5c, 0x5e, 0x4b, 0x86, 0x25, 0xd0, 
				     0x6f, 0xd2, 0xd0, 0x8c, 0x53, 0x46, 0xd0, 0xc4, 0x3a, 0x10, 0xae, 0xcf, 0x75, 0xcd, 0xfd, 0x74, 
				     0xe8, 0xfe, 0x80, 0x0b, 0x14, 0xf9, 0x94, 0x66, 0xcc, 0x19, 0x7d, 0xda, 0x89, 0x27, 0x91, 0xab, 
				     0x5c, 0x73, 0xb0, 0x78, 0xc9, 0x1c, 0x48, 0xd9, 0xe2, 0xc3, 0xfc, 0xfa, 0x3e, 0x74, 0xb9, 0xec, 
				     0x81, 0x66, 0x5f, 0x5b, 0x3e, 0x73, 0x1e, 0xd2, 0x31, 0x99, 0xc7, 0xb5, 0xe7, 0x3e, 0xbc, 0x02,}; 
		Octets key(key_buf, 128);
		QueryManager::InitSecurity(ARCFOURSECURITY, key);
		QueryManager::LoadValuablesList("valuables_list.txt");

		QueryManager manager(roleid);
		manager.LoadStorage(table_names);
		manager.BuildSnapshot();
	}
	catch(DbException e)
	{
		Log::log(LOG_ERR, "listroleinventory, error when walk, what=%s\n", e.what());
	}
	catch(...)
	{
		Log::log(LOG_ERR, "listroleinventory, error when walk, what=OLD_VERSION");
	}
}

struct ListDeityRoleQuery : public StorageEnv::IQuery
{
	StorageEnv::Storage *pstatus;
	StorageEnv::Storage *pbase;
	int zoneid;
	bool is_central;

	ListDeityRoleQuery(StorageEnv::Storage *pstat,StorageEnv::Storage *pb,int zid,bool central):pstatus(pstat),pbase(pb),zoneid(zid),is_central(central)
	{}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		GRoleBase base;	
		GRoleBase2 base2;
		GRoleStatus status;
		int roleid=0;
		Marshal::OctetsStream keyos, value_base2;
		Marshal::OctetsStream value_base,value_status;
	
		try
		{
			try
			{
				keyos=key;
				keyos >> roleid;
				if(roleid>0 && roleid<128)
					return true;
	
				value_base2=value;
				value_base2>>base2;
	
				if(base2.deity_level==0)
					return true;
		
				if( pbase->find( keyos, value_base, txn ) )
				{
					value_base>>base;
					//skip off-server role in central server
					if(is_central && base.status == _ROLE_STATUS_FROZEN)
						return true;
				}
				else
					return true;
				
				if( pstatus->find( keyos, value_status, txn ) )
					value_status >> status;
				int real_zoneid=zoneid;
				if(is_central)//if in central server, get src server zoneid
					real_zoneid = base2.src_zoneid;
//				printf("roleid=%d,leave_server=%d,zoneid=%d,",roleid,base.status == _ROLE_STATUS_FROZEN,real_zoneid);
//				printf("level=%d,reborn=%d,deity_level=%d\n",status.level,status.reborndata.size()/sizeof(int),base2.deity_level);
				printf("%d,%d,%d,",roleid,base.status == _ROLE_STATUS_FROZEN,real_zoneid);
				printf("%d,%d,%d\n",status.level,status.reborndata.size()/sizeof(int),base2.deity_level);
				return true;
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
			Log::log( LOG_ERR, "ListDeityRoleQuery, roleid=%d, what=%s\n", roleid, e.what() );
			return true;
		}
		return true;
	}
};

void ListDeityRole(int zoneid)
{
	bool is_centraldb=false;
	ReadDBType(is_centraldb);
	printf("roleid");
	printf(",leave_server");//0 in server,1 leave server
	printf(",zoneid");
	printf(",level");
	printf(",reborn");
	printf(",deity_level");
	printf("\n");

	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage( "base" );
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage( "status" );
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage( "base2" );
		StorageEnv::AtomTransaction	txn;
		
		ListDeityRoleQuery q(pstatus,pbase,zoneid,is_centraldb);
		
		try
		{
			StorageEnv::Storage::Cursor cursor = pbase2->cursor( txn );
			cursor.walk(q);
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
		Log::log( LOG_ERR, "ListDeityRole, error when walk, what=%s\n", e.what() );
	}
}

struct ListPKBetQuery : public StorageEnv::IQuery
{
	StorageEnv::Storage *pstatus;
	StorageEnv::Storage *pbase;
	int zoneid;
	bool is_central;
	
	struct pk_bet
	{
		int bet_1st_num;
		char top3; 
	}_pk_bet[8];
	int pk_bet_sum[8];
	ListPKBetQuery(StorageEnv::Storage *pstat,bool central):pstatus(pstat),is_central(central)
	{
		for(int i=0; i<8; i++)
		{
			pk_bet_sum[i]=0;
		}
	}
	int GetSum(int i)
	{
		if(i<0 ||i >=8) 
			return 0;
		return pk_bet_sum[i];
	}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		GRoleBase base;	
		GRoleStatus status;
		int roleid=0;
		Marshal::OctetsStream keyos;
		Marshal::OctetsStream value_base,value_status;
	
		try
		{
			try
			{
				keyos=key;
				keyos >> roleid;
				if(roleid>0 && roleid<128)
					return true;
	
				value_base=value;
				value_base>>base;
	
				if(base.status == _ROLE_STATUS_FROZEN)
					return true;
		
				if( pstatus->find( keyos, value_status, txn ) )
				{
					value_status >> status;
					if(status.five_year.size() >0)
					{
						int* pkbet_os = static_cast<int *>(status.five_year.begin());
						int version = 0;
						version = *pkbet_os++;
	                        		//printf("five_year size %d version %d\n",status.five_year.size() ,version);
						if(version==1)
						{
							for(int i=0;i<8;i++)
							{
								_pk_bet[i].bet_1st_num = *pkbet_os++;
								pkbet_os++;
								pk_bet_sum[i]+=_pk_bet[i].bet_1st_num;
							}

						}

					}
		
				}
				return true;
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
			Log::log( LOG_ERR, "ListPKBetQuery, roleid=%d, what=%s\n", roleid, e.what() );
			return true;
		}
		return true;
	}
};
void ListPKBet(int zoneid)
{
	bool is_centraldb=false;
	ReadDBType(is_centraldb);

	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage( "base" );
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage( "status" );
		StorageEnv::AtomTransaction	txn;
		
		ListPKBetQuery q(pstatus,is_centraldb);
		
		try
		{
			StorageEnv::Storage::Cursor cursor = pbase->cursor( txn );
			cursor.walk(q);
			std::stringstream fname;
			fname<<zoneid<<"_";
			std::string suf("total.txt");
			fname<<suf;
			std::string filename;
			fname >> filename;

	                std::ofstream ofs(filename.c_str());
	                if (!ofs)
	                {
	                        printf("Err: output file total.txt open failed\n" );
	                        return;
	                }	
			for(int i=0;i<8;i++)
			{
				ofs << i+1 << ":" << q.GetSum(i) << "\r\n";
			}

			ofs.close();
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
		Log::log( LOG_ERR, "ListPKBet, error when walk, what=%s\n", e.what() );
	}
}

void DisplayUserInfoBrief(User& user)
{
	int cash_cur;
	//include cash stored in stockexchage
	cash_cur = user.cash_add + user.cash_buy - user.cash_sell - user.cash_used;
	printf("%d",user.logicuid);
	printf(",%x",user.rolelist);
	printf(",%d",user.cash);
	printf(",%d",user.money);
	printf(",%d",user.cash_add);
	printf(",%d",user.cash_buy);
	printf(",%d",user.cash_sell);
	printf(",%d",user.cash_used);
	printf(",%d",user.add_serial);
	printf(",%d",user.use_serial);
	printf(",%d",cash_cur);
	printf(",%d",user.exg_log.size());
	printf("\n");
}

class ListUserBriefQuery : public StorageEnv::IQuery
{
public:
	int64_t curcash_total;
	int64_t cash_add_total;
	int64_t cash_used_total;
	int64_t cash_total;
	int64_t cash_sell_total;
	int64_t money_total;
	int64_t cash_buy_total;
	int64_t numberofuser;
	ListUserBriefQuery():curcash_total(0),cash_add_total(0),cash_used_total(0),cash_total(0),cash_sell_total(0),money_total(0),
		cash_buy_total(0),numberofuser(0)
	{
	}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		UserID	 id;
		User	   user;
		try
		{
			key_os >> id;
			value_os >> user;
			if(!user.cash_add && !user.cash_buy && !user.cash_sell && !user.cash_used)
				return true;
			DisplayUserInfoBrief(user);
			cash_add_total += (int)user.cash_add;
			int cash_cur = user.cash_add + user.cash_buy - user.cash_sell - user.cash_used;
			curcash_total += cash_cur;
			cash_total += (int)user.cash;
			cash_used_total += (int)user.cash_used;
			cash_sell_total += (int)user.cash_sell;
			cash_buy_total += (int)user.cash_buy;
			money_total += (int)user.money;
			numberofuser++;

		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListUserBriefQuery, error unmarshal, userid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListUserBrief( )
{

	//printf("cash:		 cash count that stored in stock exchange. \n");
	//printf("cash_add:	 cash count which user has bought by RMB. \n");
	//printf("cash_buy:	 cash count which role has bought by money in game. \n");
	//printf("cash_sell:	cash count which role has sold  by money in game. \n");
	//printf("cash_used:	cash count which role has spent in game. \n");
	//printf("cash_current: cash count which user now owns in game. \n");
	//printf("money:		game money which role has stored in stock exchange. \n");
	//printf(" All roles of the user share money and cash in stock exchange.\n");


	/* All roles of the user share money and cash in stock exchange */

	printf("userid");
	printf(",rolelist");
	//cash count that stored in stock exchange
	printf(",cash");
	//game money which role has stored in stock exchange
	printf(",money");
	//cash count which user has bought by RMB
	printf(",cash_add");
	//cash count which role has bought by money in game
	printf(",cash_buy");
	//cash count which role has sold  by money in game
	printf(",cash_sell");
	//cash count which role has spent in game
	printf(",cash_used");
	printf(",add_serial");
	printf(",use_serial");
	//cash count which user now owns in game. cash_current = cash_add + cash_buy - cash_sell - cash_used
	printf(",cash_current");
	printf(",exg_logcount");

	printf("\n");


	ListUserBriefQuery q;

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction	 txn;
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
		Log::log( LOG_ERR, "ListUserBrief, error when walk, what=%s\n", e.what() );
	}

	printf("\ncash_add_total  : %lld\n",q.cash_add_total);
	printf("cash_now_total  : %lld\n",q.curcash_total);
	q.cash_used_total += q.cash_sell_total - q.cash_buy_total; 
	printf("cash_used_total : %lld\n",q.cash_used_total);
	if(q.curcash_total > (q.cash_add_total - q.cash_used_total)){
		printf("WARNING: cash_now_total > cash_add_total - cash_used_total\n");
	}

	printf("\ncash_buy_total : %lld\n",q.cash_buy_total);
	printf("cash_sell_total  : %lld\n",q.cash_sell_total);	
	if(q.cash_sell_total - q.cash_buy_total < q.cash_buy_total*0.02 ){
		printf("WARNING: cash_buy_total*1.02 > cash_sell_total\n");	
	}	

	printf("\ncash_total in stock exchange  : %lld\n",q.cash_total);
	printf("money_total in stock exchange : %lld\n",q.money_total);
	printf("number of user : %lld\n",q.numberofuser);	
}

class ListCrssvrteamsRolesQuery : public StorageEnv::IQuery
{
public:
	int team_count;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		value_os = value;

		GCrssvrTeamsData crssvrteams;
		try
		{
			value_os >> crssvrteams;

			//printf("ListCrssvrteamsRolesQuery, teamid=%d\n", crssvrteams.info.teamid);
			if(crssvrteams.info.teamid <= 0)
			{
				return true;
			}

			typedef std::map<unsigned char/*occup*/, int/*count*/> OccupCount;
			OccupCount occup_count;
			
			std::vector<GCrssvrTeamsRole>::iterator it_roles = crssvrteams.roles.begin(), ie_roles=crssvrteams.roles.end();
			for(;it_roles!=ie_roles;++it_roles)
			{
				int t_roleid = (*it_roles).roleid;
				unsigned char occup = (*it_roles).occupation;
				occup_count[occup]++;
				//printf("occup is limit, teamid=%d, roleid=%d, occup=%d, occup_count=%d\n", crssvrteams.info.teamid, t_roleid, occup, occup_count[occup]);

				if(occup_count[occup] + 1 > OCCUPATION_PLAYER_LIMIT)
				{
					printf("occup is limit, teamid=%d, roleid=%d, occup=%d, occup_count=%d\n", crssvrteams.info.teamid, t_roleid, occup, occup_count[occup]);
					continue;
				}
			}
			team_count++;
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListCrssvrteamsRolesQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};

void ListCrssvrteamsRole( )
{
	ListCrssvrteamsRolesQuery q;
	q.team_count = 0;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "crssvrteams" );
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
		printf("ListCrssvrteamsQuery, query ok, team_count=%d\n", q.team_count);
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "ListCrssvrteamsQuery, error when walk, what=%s\n", e.what() );
	}
}


class ListFactionQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId			id;
		GFactionInfo	faction;
		try
		{
			key_os >> id;
			if( 0 == id.id )
				return true;

			value_os >> faction;

			Octets name, name2;
			CharsetConverter::conv_charset_u2l( faction.name, name );
			EscapeCSVString( name, name2 );

			printf("%d",faction.fid);
			printf(",%.*s",name2.size(),(char*)name2.begin() );
			printf(",%d",faction.level);
			printf(",%d",faction.master);
			printf(",2");
			printf(",%d",faction.population);
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListFactionQuery, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListFaction( )
{
	printf("fid");
	printf(",name");
	printf(",level");
	printf(",masterid");
	printf(",masterrole");
	printf(",member_size");
	printf("\n");

	ListFactionQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "faction" );
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
		Log::log( LOG_ERR, "ListFaction, error when walk, what=%s\n", e.what() );
	}
}

class ListFamilyQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		value_os = value;

		GFamily	info;
		try
		{
			value_os >> info;

			Octets name, name2;
			CharsetConverter::conv_charset_u2l( info.name, name );
			EscapeCSVString( name, name2 );

			printf("%d",info.id);
			printf(",%.*s",name2.size(),(char*)name2.begin() );
			printf(",%d",info.master);
			printf(",%d",info.factionid);
			printf(",%d",info.createtime);
			printf(",%d",info.jointime);
			printf(",%d",info.deletetime);
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListFamilyQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};

void ListFamily( )
{
	printf("id");
	printf(",name");
	printf(",master");
	printf(",factionid");
	printf(",createtime");
	printf(",jointime");
	printf(",deletetime");
	printf("\n");

	ListFamilyQuery q;

	try
	{
		StorageEnv::Storage * pfaction = StorageEnv::GetStorage( "family" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pfaction->cursor( txn );
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
		Log::log( LOG_ERR, "ListFamily, error when walk, what=%s\n", e.what() );
	}
}

class ListFamilyUserQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		value_os = value;

		GFamily info;
		try
		{
			value_os >> info;

			for(std::vector<GFolk>::iterator it=info.member.begin(),ie=info.member.end();it!=ie;++it)
			{
				Octets name, name2, nickname, nickname2;
				CharsetConverter::conv_charset_u2l( it->name, name );
				CharsetConverter::conv_charset_u2l( it->nickname, nickname );
				EscapeCSVString( name, name2 );
				EscapeCSVString( nickname, nickname2 );

				printf("%d",it->rid);
				printf(",%d",info.id);
				printf(",%.*s",name2.size(),(char*)name2.begin() );
				printf(",%.*s",nickname2.size(),(char*)nickname2.begin() );
				printf(",%d",it->level);
				printf(",%d",it->title);
				printf(",%d",it->occupation);
				printf(",%d",it->contribution);
				printf(",%d",it->jointime);
				printf(",%d",it->devotion);
				printf("\n");
			}
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListFamilyUserQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};

void ListFamilyUser( )
{
	printf("rid");
	printf(",familyid");
	printf(",name");
	printf(",nickname");
	printf(",level");
	printf(",title");
	printf(",occupation");
	printf(",contribution");
	printf(",jointime");
	printf(",devotion");
	printf("\n");

	ListFamilyUserQuery q;

	try
	{
		StorageEnv::Storage * pfaction = StorageEnv::GetStorage( "family" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pfaction->cursor( txn );
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
		Log::log( LOG_ERR, "ListFamilyUser, error when walk, what=%s\n", e.what() );
	}
}

class ListShopLogQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os = value;

		std::vector<GShopLog> logs;
		try
		{
			value_os >> logs;

			for(std::vector<GShopLog>::iterator it=logs.begin(),ie=logs.end();it!=ie;++it)
			{
				printf("%d", it->roleid);
				printf(",%d", it->order_id);
				printf(",%d", it->item_id);
				printf(",%d", it->expire);
				printf(",%d", it->item_count);
				printf(",%d", it->order_count);
				printf(",%d", it->cash_need);
				printf(",%d", it->time);
				printf(",%d", it->guid1);
				printf(",%d", it->guid2);
				printf("\n");
			}
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListShopLogQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};

void ListShopLog( )
{
	printf("roldid");
	printf(",order_id");
	printf(",item_id");
	printf(",expire");
	printf(",item_count");
	printf(",order_count");
	printf(",cash_need");
	printf(",time");
	printf(",guid1");
	printf(",guid2");
	printf("\n");

	ListShopLogQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "shoplog" );
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
		Log::log( LOG_ERR, "ListShopLog, error when walk, what=%s\n", e.what() );
	}
}

void ModifyInventory( GRoleBase & base, GRolePocket & pocket, GRoleStorehouse & storehouse, GMailBox & box )
{
	struct inv_t {
		unsigned int	id;
		char			name[64];
		int				count;
	};

	struct inv_t invs[] = {
		{ 1657, "幻真令", 0 },
		{ 1658, "紫仙令", 0 },
		{ 1659, "太虚令", 0 },
		{ 2523, "榴莲", 0 },
		{ 2524, "糖葫芦", 0 },
		{ 2525, "女儿红", 0 },
		{ 2526, "银票", 0 },
		{ 2527, "玉器", 0 },
		{ 1260, "盟主旗", 0 },
		{ 1842, "生命小瞬回药", 0 },
		{ 1843, "生命中瞬回药", 0 },
		{ 1847, "魔力小瞬回药", 0 },
		{ 1848, "魔力中瞬回药", 0 },
		{ 1853, "保留符", 0 },
		{ 2953, "强运符", 0 },
		{ 0, "", 0 }
	};

	GRoleInventoryVector::iterator it, ite;
	int n = 0;
	for( it=pocket.items.begin(), ite=pocket.items.end(); it != ite; )
	{
		bool b = false;
		for( n=0; invs[n].id != 0; n++ )
		{
			if( it->id == invs[n].id )
			{
				invs[n].count += it->count;
				it = pocket.items.erase(it);
				ite = pocket.items.end();
				b = true;
				break;
			}
		}
		if( !b )	++it;
	}

	for( it=pocket.equipment.begin(), ite=pocket.equipment.end(); it != ite; )
	{
		bool b = false;
		for( n=0; invs[n].id != 0; n++ )
		{
			if( it->id == invs[n].id )
			{
				invs[n].count += it->count;
				it = pocket.equipment.erase(it);
				ite = pocket.equipment.end();
				b = true;
				break;
			}
		}
		if( !b )	++it;
	}

	for( it=storehouse.items.begin(), ite=storehouse.items.end(); it != ite; )
	{
		bool b = false;
		for( n=0; invs[n].id != 0; n++ )
		{
			if( it->id == invs[n].id )
			{
				invs[n].count += it->count;
				it = storehouse.items.erase(it);
				ite = storehouse.items.end();
				b = true;
				break;
			}
		}
		if( !b )	++it;
	}


	GMailVector::iterator im=box.mails.begin(),ie=box.mails.end();
	for(;im!=ie;im++)
	{
		if(im->header.attribute & (1<<_MA_ATTACH_OBJ))
		{
			for( n=0; invs[n].id != 0; n++ )
			{
				if( im->attach_obj.id == invs[n].id )
				{
					invs[n].count += im->attach_obj.count;
					im->attach_obj.count = 0;
					im->header.attribute ^= (1<<_MA_ATTACH_OBJ);
					break;
				}
			}
		}
	}

	Octets name, name2;
	CharsetConverter::conv_charset_u2l( base.name, name );
	EscapeCSVString( name, name2 );

	printf("%d",base.id);
	printf(",%d",base.userid ? base.userid : LOGICUID(base.id));
	printf(",%.*s",name2.size(),(char*)name2.begin());

	for( n=0; invs[n].id != 0; n++ )
	{
		printf(",%d", invs[n].count);
	}
	printf("\n");
}

class UpdateRolesQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_base, value_status;
		key_os = key;

		static int count = 0;
		count ++;
		if( ! (count % 1000) )
			fprintf( stderr, "updating roles %d...\n", count );

		RoleId	id;
		try
		{
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			try
			{
				GRoleStatus	status;

				key_os >> id;
				if( pstatus->find( key_os, value_status, txn ) )
				{
					value_status >> status;

					status.reputation = 6*status.level*status.level - 6*status.level;

					value_status.clear();
					value_status << status;
					pstatus->insert( key_os, value_status, txn );
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
			Log::log( LOG_ERR, "UpdateRolesQuery, roleid=%d, what=%s\n", id.id, e.what() );
		}
		return true;
	}
};

void MakeTaskMap( std::map< int, std::pair<int,int> > &	maptask)
{
	maptask[2328] = std::make_pair(8 ,72  );
	maptask[2329] = std::make_pair(8 ,99  );
	maptask[2330] = std::make_pair(8 ,99  );
	maptask[2331] = std::make_pair(8 ,99  );
	maptask[2332] = std::make_pair(8 ,130 );
	maptask[2333] = std::make_pair(8 ,130 );
	maptask[2334] = std::make_pair(8 ,130 );
	maptask[2335] = std::make_pair(8 ,170 );
	maptask[2336] = std::make_pair(8 ,170 );
	maptask[2337] = std::make_pair(8 ,170 );
	maptask[2338] = std::make_pair(8 ,210 );
	maptask[2339] = std::make_pair(8 ,210 );
	maptask[2340] = std::make_pair(8 ,210 );
	maptask[2342] = std::make_pair(8 ,250 );
	maptask[2343] = std::make_pair(8 ,250 );
	maptask[2281] = std::make_pair(7 ,10  );
	maptask[2282] = std::make_pair(7 ,24  );
	maptask[2283] = std::make_pair(7 ,24  );
	maptask[2284] = std::make_pair(7 ,24  );
	maptask[2286] = std::make_pair(7 ,48  );
	maptask[2287] = std::make_pair(7 ,48  );
	maptask[2288] = std::make_pair(7 ,72  );
	maptask[2289] = std::make_pair(7 ,72  );
	maptask[2290] = std::make_pair(7 ,72  );
	maptask[2291] = std::make_pair(7 ,99  );
	maptask[2292] = std::make_pair(7 ,99  );
	maptask[2293] = std::make_pair(7 ,99  );
	maptask[2294] = std::make_pair(7 ,130 );
	maptask[2295] = std::make_pair(7 ,130 );
	maptask[2296] = std::make_pair(7 ,130 );
	maptask[2297] = std::make_pair(7 ,170 );
	maptask[2301] = std::make_pair(7 ,170 );
	maptask[2302] = std::make_pair(7 ,170 );
	maptask[2303] = std::make_pair(7 ,210 );
	maptask[2304] = std::make_pair(7 ,210 );
	maptask[2305] = std::make_pair(7 ,210 );
	maptask[2306] = std::make_pair(7 ,250 );
	maptask[2307] = std::make_pair(7 ,250 );
	maptask[2395] = std::make_pair(10, 10 );
	maptask[2398] = std::make_pair(10, 24 );
	maptask[2411] = std::make_pair(10, 130);
	maptask[2412] = std::make_pair(10, 130);
}

void UpdateRoles( )
{
	return;

	int count = 0, errcount = 0;

	if( 0 != access("reproles.txt",R_OK) )
	{
		fprintf( stderr, "no reproles.txt find.\n" );
		return;
	}

	std::map< int, std::map<int,int> >	maprole;
	std::map< int, std::pair<int,int> >	maptask;
	MakeTaskMap(maptask);

	std::ifstream	ifs( "reproles.txt" );
	while( !ifs.eof() )
	{
		char	line[256];
		memset( line, 0, sizeof(line) );
		ifs.getline( line, sizeof(line) );
		line[sizeof(line)-1] = 0;
		if( !ifs.eof() && strlen(line) > 0 )
		{
			int roleid = 0, taskid = 0;
			sscanf( line, "%d %d", &roleid, &taskid );

			if( maptask.find( taskid ) == maptask.end() )
				continue;

			maprole[roleid][taskid] += 1;
		}
	}
	ifs.close();

	try
	{
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::AtomTransaction	txn;
		try
		{
			for( std::map< int, std::map<int,int> >::iterator it = maprole.begin(), ite = maprole.end(); it != ite; ++ it )
			{
				Marshal::OctetsStream key_os, value_status, value_status_new;
				int roleid = it->first;
				key_os << roleid;
				GRoleStatus	status;
				if( pstatus->find( key_os, value_status, txn ) )
				{
					value_status >> status;
					int * pRep = (int*) status.credit.begin();
					assert(status.credit.size() == sizeof(int)* 10);
					std::map<int,int> & task_list = it->second;
					for(std::map<int,int>::iterator ita = task_list.begin(); ita != task_list.end(); ++ita)
					{
						int task_id = ita->first;
						int count = ita->second;
						assert(count > 0);
						
						int region = maptask[task_id].first;
						int rep = maptask[task_id].second;
						assert(region >= 1 && region <=10);
						
						int rep_offset = rep * count;
						pRep[region-1] -= rep_offset;
						printf( "update role %d task %d region %d reputation dec %d (%d).\n", roleid , task_id, region, rep_offset,pRep[region-1]);
						if(pRep[region-1] < 0)
						{
							printf("role %d reputation region %d< 0\n", roleid, region);
							pRep[region-1] = 0;
							errcount ++;
						}
					}
					

					value_status_new << status;
					pstatus->insert( key_os, value_status_new, txn );
					printf( "update role %d successfully.\n", roleid );
					count ++;
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
		printf( "update role %d error, what=%s\n", -1, e.what() );
		errcount ++;
	}

	StorageEnv::checkpoint();

	printf( "update count = %d. errcount = %d\n", count, errcount );
	return;
/*
	printf("roleid");
	printf(",userid");
	printf("\n");

	UpdateRolesQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
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
		Log::log( LOG_ERR, "UpdateRoles, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
*/
}

class ConvertStatusDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream os_key, os_old, os_new;
		os_key = key;
		os_old = value;
		GRoleStatus user;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\tconverting database records counter %d...\n", count );

		try
		{
			StorageEnv::Storage * pconvtemp = StorageEnv::GetStorage("status");
			try
			{
				os_old >> user;
				user.exp = 0;
				os_new << user;
				pconvtemp->insert( key, os_new, txn );
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
			Log::log( LOG_ERR, "ConvertStatusDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};
class ConvertUserDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream os_key, os_old, os_new;
		os_key = key;
		os_old = value;
		User user;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\tconverting database records counter %d...\n", count );

		try
		{
			StorageEnv::Storage * pconvtemp = StorageEnv::GetStorage("user");
			try
			{
				os_old >> user;
				user.exg_log.clear();
				os_new << user;
				pconvtemp->insert( key, os_new, txn );
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
			Log::log( LOG_ERR, "ConvertUserDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};
class ConvertBaseDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream os_key, os_old, os_new;
		os_key = key;
		os_old = value;
		GRoleBase user;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\tconverting database records counter %d...\n", count );

		try
		{
			StorageEnv::Storage * pconvtemp = StorageEnv::GetStorage("base");
			try
			{
				os_old >> user;
				os_new << user;
				pconvtemp->insert( key, os_new, txn );
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
			Log::log( LOG_ERR, "ConvertStatusDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};
class ConvertFactionDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream os_key, os_old, os_new;
		os_key = key;
		os_old = value;
		GFactionInfo user;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\tconverting database records counter %d...\n", count );

		try
		{
			StorageEnv::Storage * pconvtemp = StorageEnv::GetStorage("faction_temp");
			try
			{
				os_old >> user;
				os_new << user;
				pconvtemp->insert( key, os_new, txn );
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
			Log::log( LOG_ERR, "ConvertStatusDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};
class ConvertCacheDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream os_key, os_old, os_new;
		os_key = key;
		os_old = value;
		GFactionInfo status;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\tconverting database records counter %d...\n", count );

		try
		{
			StorageEnv::Storage * pconvtemp = StorageEnv::GetStorage("cache_temp");
			try
			{
				os_old >> status;
				os_new << status;
				pconvtemp->insert( key, os_new, txn );
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
			Log::log( LOG_ERR, "ConvertStatusDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};

void ConvertDB( )
{
	return;

	printf( "\nconvert database\n" );

	std::string data_dir = StorageEnv::get_datadir();
	ConvertUserDBQuery quser;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( quser );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
		}
	}
	catch ( DbException e )
	{
	}

	ConvertBaseDBQuery qbase;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( qbase );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
		}
	}
	catch ( DbException e )
	{
	}

	ConvertStatusDBQuery qstatus;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "status" );
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
			cursor.walk( qstatus );
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
		}
	}
	catch ( DbException e )
	{
	}

	StorageEnv::checkpoint();
	StorageEnv::Close();
	//if( 0 == access( (data_dir + "/conv_temp").c_str(), R_OK ) )	
	//	system( ("/bin/mv -f " + data_dir + "/conv_temp " + data_dir + "/status").c_str() );
	StorageEnv::Open();
}

class RepairDBQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_roleid, key_userid, value_base, value_user;
		key_roleid = key;
		value_base = value;

		static int count = 0;
		if( ! (count++ % 100000) ) printf( "\trepairing database records counter %d...\n", count );

		int roleid;

		try
		{
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			try
			{
				GRoleBase	base;
				User		user;
				key_roleid >> roleid;
				value_base >> base;
				if( 0 == base.userid )  
					base.userid = LOGICUID(roleid);
				key_userid << base.userid;

				if( roleid < 32 )
					return true;

				if( puser->find( key_userid, value_user, txn ) )
				{
					value_user >> user;
					RoleList rolelist(user.rolelist);
					if( !rolelist.IsRoleExist( roleid ) )
					{
						if( -1 == rolelist.AddRole( roleid % MAX_ROLE_COUNT ) )
							Log::log( LOG_ERR, "Repair roleid=%d, roleliest full=%x", roleid, user.rolelist );
						user.logicuid = LOGICUID(roleid);
						user.rolelist = rolelist.GetRoleList();
						value_user.clear();
						value_user << user;
						puser->insert( key_userid, value_user, txn );
						Log::log( LOG_INFO, "Repair roleid=%d, new user roleliest=%x", roleid, user.rolelist );
					}
				}
				else
				{
					RoleList rolelist;
					rolelist.InitialRoleList();

					rolelist.AddRole( roleid % MAX_ROLE_COUNT );
					user.logicuid = LOGICUID(roleid);
					user.rolelist = rolelist.GetRoleList();
					value_user.clear();
					value_user << user;
					puser->insert( key_userid, value_user, txn );
					Log::log( LOG_INFO, "Repair roleid=%d, new user roleliest=%x", roleid, user.rolelist );
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
			Log::log( LOG_ERR, "RepairDBQuery, roleid=%d, what=%s\n", roleid, e.what() );
		}
		return true;
	}
};

void RepairDB( )
{
	return;

	printf( "\nrepair database\n" );

	RepairDBQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base" );
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
		Log::log( LOG_ERR, "RepairDB, error when walk, what=%s\n", e.what() );
	}
	printf( "repair finished.\n" );
}

class TimeoutRoleQuery : public StorageEnv::IQuery
{
	int		  ncount;	
	WaitDelKeyVector readylist;
public:
	TimeoutRoleQuery() { ncount=0; }
	WaitDelKeyVector& GetReadyList() { return readylist; }
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		WaitDelKey 	waitdel;
		int 		deltime;
		try
		{
			key_os >> waitdel;
			value_os >> deltime;
			if( Timer::GetTime()- deltime > GameDBManager::GetInstance()->GetDeleteTimeout() )
			{
				//announce delivery server
				readylist.add(waitdel);
				LOG_TRACE( "TimeoutRoleQuery, id=%d type=%d.\n",waitdel.id, waitdel.type );
				ncount++;
				if (ncount>=100/*maxium number of roles in one request*/) 
					return false;
			}
		} catch ( Marshal::Exception ) {
			Log::log( LOG_ERR, "TimeoutRoleQuery, error unmarshal, id=%d type=%d .", waitdel.id, waitdel.type);
		}
		return true;
	}
};

void GetTimeoutRole( WaitDelKeyVector& rolelist )
{
	TimeoutRoleQuery q;
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
		Log::log( LOG_ERR, "GetTimeoutRole, error when walk, what=%s\n", e.what() );
	}
	rolelist.GetVector().swap(q.GetReadyList().GetVector());
}

bool ExportUser( int userid )
{
	try
	{
		Marshal::OctetsStream key;
		User user;
		StorageEnv::CommonTransaction txn;
		try
		{
			StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
			key << userid;
			Marshal::OctetsStream(puser->find(key, txn)) >> user;

			XmlCoder encoder;
			encoder.append_header();
			encoder.append("user", user);
			puts(encoder.c_str());
			return true;
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		Log::log(LOG_ERR, "ExportUser failed, userid=%d, what=%s\n", userid, e.what());
		return false;
	}
	return true;
}

bool ExportRole( int roleid )
{
	try
	{
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
		StorageEnv::Storage * pachieve = StorageEnv::GetStorage("achievement");
		StorageEnv::AtomTransaction txn;

		Marshal::OctetsStream key;
		GRoleData data;
		GRoleAchievement achieve;
		try
		{
			key << roleid;
			Marshal::OctetsStream(pbase->find(key,txn)) >> data.base;
			Marshal::OctetsStream(pstatus->find(key,txn)) >> data.status;
			Marshal::OctetsStream(pinventory->find(key,txn)) >> data.pocket;
			Marshal::OctetsStream(pstorehouse->find(key,txn)) >> data.storehouse;
			Marshal::OctetsStream(ptask->find(key,txn)) >> data.task;
			Marshal::OctetsStream(pbase2->find(key,txn)) >> data.base2;
			Marshal::OctetsStream(pachieve->find(key,txn)) >> data.achievement;

			XmlCoder coder;
			coder.append_header();
			coder.append("role", data);
			puts(coder.c_str());
			return true;
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
		Log::log( LOG_ERR, "ExportRole, roleid=%d, what=%s\n", roleid, e.what() );
	}
	return false;
}
class ListSysLogQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os = value;
		Marshal::OctetsStream key_os = key;

		GSysLog log;
		try
		{
			int64_t id;
			key_os >> id;
			value_os >> log;
			printf("%lld", id);
			printf(",%d", log.roleid);
			printf(",%d", log.time);
			printf(",%d", log.ip);
			printf(",%d", log.source);
			printf(",%d", log.money);
			for(GRoleInventoryVector::iterator it=log.items.begin(),ie=log.items.end();it!=ie;++it)
			{
				printf(",%d:%d:%d", it->id, it->pos, it->count);
			}
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListSysLogQuery, error unmarshal.");
			return true;
		}
		return true;
	}
};
void ListSysLog( )
{
	printf("guid");
	printf(",roleid");
	printf(",time");
	printf(",ip");
	printf(",source");
	printf(",money");
	printf(",items");
	printf("\n");

	ListSysLogQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "syslog" );
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
		Log::log( LOG_ERR, "ListSysLog, error when walk, what=%s\n", e.what() );
	}
}
void SetCashInvisible(const char* file)
{
	std::ifstream ifs(file);
	std::string line;
	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage( "user" );
		StorageEnv::AtomTransaction txn;
		try
		{
			User user;
			while (std::getline(ifs, line))
			{
				Marshal::OctetsStream key, value;
				int userid = atoi(line.c_str());
				key << userid;
				if(puser->find(key,value,txn))
				{
					value >> user;
					if((user.status&STATUS_CASHINVISIBLE)!=0)
					{
						user.status &= ~STATUS_CASHINVISIBLE;
						puser->insert(key,Marshal::OctetsStream()<<user,txn);
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
		Log::log( LOG_ERR, "SetCashInvisible, error updating, what=%s\n", e.what() );
	}
}

static unsigned int g_nextlogicuid = 1024;

class UserLogicUIDQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		User user;
		Marshal::OctetsStream(value) >> user;

		if (g_nextlogicuid <= user.logicuid)
			g_nextlogicuid = user.logicuid+16;

		return true;
	}
};

void InitNextLogicUID()
{
	UserLogicUIDQuery q;
	try
	{
		StorageEnv::Storage *puser = StorageEnv::GetStorage("user");
		StorageEnv::AtomTransaction txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = puser->cursor(txn);
			cursor.walk(q);
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
		Log::log( LOG_ERR, "InitNextLogicUID, error when walk, what=%s\n", e.what() );
	}
}

static ItemIdMan g_itemIdMan;
//mergedb.cpp 中的 CheckItemOwner 要同步修改 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
static void CheckItemOwner(const std::string &container, int oldroleid, int newroleid, GRoleInventoryVector &items)
{
	for (size_t i = 0; 	i < items.size(); i++)
	{
		int item_type = g_itemIdMan.GetItemType(items[i].id);
		if (item_type != -1)
		{
			int ownerid = GetOwnerID(item_type, items[i].data);
			if (ownerid > 0)
			{
				if (ownerid == oldroleid)
				{
					SetOwnerID(item_type, items[i].data, newroleid);
					Log::log(LOG_INFO, "The owner's roleid of item id=%d in pos %d of %s of role id=%d is changed to %d\n", items[i].id, items[i].pos, container.c_str(), oldroleid, newroleid);
				}
			}
		}
		else
		{
			int specialid = GetSpecialItemID(items[i].id, items[i].data);
			if(specialid == -1) continue;
			item_type = g_itemIdMan.GetItemType(specialid);
			if (item_type != -1)
			{
				int ownerid = GetSpecialItemOwnerID(items[i].id, item_type, items[i].data);
				if (ownerid == oldroleid)
				{
					SetSpecialItemOwnerID(items[i].id, item_type, items[i].data, newroleid);
					Log::log(LOG_INFO, "The owner's roleid of  item id=%d special id =%d in pos %d of %s of role id=%d is changed to %d\n", items[i].id, specialid, items[i].pos, container.c_str(), oldroleid, newroleid);
				}
			}
		}
	}
}

bool ImportCsvUserIdExchange( const char * filename )
{
	FILE* fp = fopen(filename, "r");
	if(fp)
	{
		char buf[4096];
		while(fgets(buf, sizeof(buf), fp))
		{
			if(buf[0]=='#')
				continue;
			int olduserid, newuserid;
			char name[2048];
			memset(name,0,sizeof(name));

			std::string src = buf;
			std::vector<std::string> r;
			if( GNET::ParseStrings( src, r ) && r.size() >= 2 )
			{
				int len = r.size();
				newuserid = atoi( r[len-1].c_str() );
				olduserid   = atoi( r[len-2].c_str() );
				//printf("'%d','%d'\n", olduserid, newuserid);	
				//g_UserIdExchangeTable[olduserid] = newuserid;
				
				std::pair< tUserIdExchangeMap::iterator, bool> ret;
				ret = g_UserIdExchangeTable.insert(std::make_pair(olduserid,newuserid));
				if(!ret.second)
				{
					printf("ImportCvsUserIdExchange ,key:%d repeated\n", olduserid);
				}
			}
		}
		printf("ImportCvsUserIdExchange g_UserIdExchangeTable size is %d\n", g_UserIdExchangeTable.size());

		fclose(fp);
		return true;
	}
	else
	{
		return false;
	}
}


class ImportRoleListQuery : public IQueryData
{
public:
	int count;

	bool update( const void *key, size_t key_len, const void *val, size_t val_len )
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");
		StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");

		GRoleData roledata;
		Octets dummy(val, val_len);
		Marshal::OctetsStream(dummy) >> roledata;

		Marshal::OctetsStream osuserid, osroleid, osuser;
		osuserid << roledata.base.userid;

		try
		{
			User user;
			StorageEnv::AtomTransaction txn;

			try
			{
				if (puser->find(osuserid, osuser, txn))
				{
					osuser >> user;
				}
				else
				{
					user.logicuid = g_nextlogicuid;
					g_nextlogicuid += 16;
				}

				RoleList rolelist(user.rolelist);
				if (!rolelist.IsRoleListInitialed())
					rolelist.InitialRoleList();
				if (rolelist.GetRoleCount() >= 8)
				{
					Log::log(LOG_INFO, "ImportRoleListQuery, skip roleid=%d because role number exceeds 8", roledata.base.id);
					return true;
				}
				int oldroleid = roledata.base.id;
				int roleid = rolelist.AddRole();
				roleid += user.logicuid;
				osroleid << roleid;
				user.rolelist = rolelist.GetRoleList();
				roledata.base.id = roleid;
				roledata.status.id = roleid;
				roledata.base2.id = roleid;

				Octets dummyroleid, newrolename=roledata.base.name;
				int postfix = 1;
				while (prolename->find(newrolename, dummyroleid, txn))
				{
					char bufpostfix[32] = {0};
					snprintf(bufpostfix, 16, "%d", postfix);
					postfix++;
					int len = (int)strlen(bufpostfix);
					for (int i=len-1; i>=0; i--)
					{
						bufpostfix[i*2] = bufpostfix[i];
						bufpostfix[i*2+1] = 0;
					}
					newrolename = roledata.base.name;
					newrolename.insert(newrolename.end(), bufpostfix, 2*len);
				}
				roledata.base.name = newrolename;
				prolename->insert(roledata.base.name, osroleid, txn);

				puser->insert(osuserid, Marshal::OctetsStream()<<user, txn);
				pbase->insert( osroleid, Marshal::OctetsStream() << roledata.base, txn );
				pstatus->insert( osroleid, Marshal::OctetsStream() << roledata.status, txn );
				pbase2->insert( osroleid, Marshal::OctetsStream() << roledata.base2, txn );

				if (oldroleid != roleid)
				{
					//inventory
					CheckItemOwner("inventory", oldroleid, roleid, roledata.pocket.items);
					CheckItemOwner("inventory", oldroleid, roleid, roledata.pocket.equipment);
					CheckItemOwner("inventory", oldroleid, roleid, roledata.pocket.petbadge);
					CheckItemOwner("inventory", oldroleid, roleid, roledata.pocket.petequip);
					CheckItemOwner("inventory", oldroleid, roleid, roledata.pocket.fashion);
					if (roledata.pocket.mountwing.size())
					{
						Marshal::OctetsStream os_mountwing(roledata.pocket.mountwing);
						GRoleInventoryVector mountwing;
						short mountwingsize = 0;
						try
						{
							os_mountwing >> mountwingsize;
							os_mountwing >> mountwing;
							CheckItemOwner("inventory", oldroleid, roleid, mountwing);
							os_mountwing.clear();
							os_mountwing << mountwingsize;
							os_mountwing << mountwing;
							roledata.pocket.mountwing = os_mountwing;
						}
						catch(...)
						{
							Log::log(LOG_ERR, "mountwing unmarshal error, oldroleid=%d", oldroleid);
						}
					}
					CheckItemOwner("inventory", oldroleid, roleid, roledata.pocket.gifts);

					//storehouse
					CheckItemOwner("storehouse", oldroleid, roleid, roledata.storehouse.items);
					CheckItemOwner("storehouse", oldroleid, roleid, roledata.storehouse.items2);
					CheckItemOwner("storehouse", oldroleid, roleid, roledata.storehouse.fuwen);
				}

				pinventory->insert( osroleid, Marshal::OctetsStream() << roledata.pocket, txn );
				pstorehouse->insert( osroleid, Marshal::OctetsStream() << roledata.storehouse,txn);

				ptask->insert(osroleid, Marshal::OctetsStream() << roledata.task, txn);

				count++;
			}
			catch (DbException e) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch (DbException e)
		{
			Log::log(LOG_ERR, "ImportRoleListQuery, error when find user, what=%s\n", e.what());
		}
		return true;
	}
};

void ImportRoleList(const char *rolelistfiles[], int filecount)
{
	for (int i = 0; i < filecount; i++)
	{
		if( 0 != access(rolelistfiles[i], R_OK) )
		{
			Log::log(LOG_ERR, "Fail to access rolelist file: %s", rolelistfiles[i]);
			return;
		}
	}

	InitNextLogicUID();
	
	if (!g_itemIdMan.LoadItemId())
	{
		Log::log(LOG_ERR, "Failed to read item id from 'itemid.txt'\n");
		return;
	}

	int count = 0;
	for (int i = 0; i < filecount; i++)
	{
		DBStandalone * pdb = new DBStandalone( rolelistfiles[i] );
		if(!pdb)
		{
			Log::log( LOG_ERR, "Cannot open table %s.", rolelistfiles[i] );
			return;
		}
		pdb->init();

		ImportRoleListQuery q;
		q.count = 0;

		try
		{
			try
			{
				pdb->walk(&q);
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "ImportRoleList, error when walk, what=%s\n", e.what() );
		}

		delete pdb;
		count += q.count;

		Log::log( LOG_INFO, "Import %d roles from %s", q.count, rolelistfiles[i]);
	}

	StorageEnv::checkpoint();
	Log::log( LOG_INFO, "Totally import %d roles.", count);
}

void ReadRoleIDFile(const char *roleidfile, std::set<unsigned int> &roleidset)
{
	roleidset.clear();
	unsigned int roleid;

	std::ifstream ifs(roleidfile);
	if (ifs.fail())
		return;

	while (!ifs.eof())
	{
		ifs >> roleid;
		if (ifs.fail())
			break;
		roleidset.insert(roleid);
	}
}

class ExportCsvUserIdExchangeQuery : public StorageEnv::IQuery
{
public:
	std::string	destdbname;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value )
	{
		try
		{
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage(destdbname.c_str());
			try
			{
				if( 0 == strcmp("user", destdbname.c_str()) )
				{					
					Marshal::OctetsStream key_os(key), value_new, value_old(value),value1;
					{
						// key 为userid，只需改key
						int userid_old, userid_new;
						Marshal::OctetsStream key_new;
						key_os >> userid_old;
					
						if(-1 == (userid_new = GetUserIdFromTable(userid_old)))
						{
							return true;
						}

						key_new << userid_new; 

						pstorage->insert( key_new, value, txn );
					}
				}
				else if( 0 == strcmp("base", destdbname.c_str()) )
				{
					Marshal::OctetsStream key_os(key), value_new, value_old(value), value1;
					{
						int userid_old;
						int roleid;
						key_os >> roleid;
						
						GRoleBase base;
						try{
							value_old >> base;
						}
						catch( Marshal::Exception & )
						{
							printf("%s database Marshal GRoleBase error,key roleid=%d \n", destdbname.c_str(),roleid);
							return true;
						}

						if(0==base.userid)
						{
							//printf("base.userid == 0, LOGICUID(base.userid)=%d\n",LOGICUID(base.userid));
							userid_old = LOGICUID(base.userid);
						}
						else
						{ 
							userid_old = base.userid;
						}

						if(-1 == (base.userid = GetUserIdFromTable(userid_old)))
						{
							return true;
						}
						
						value_new << base;
						pstorage->insert( key, value_new, txn );
					}
				}
				else if( 0 == strcmp("consign", destdbname.c_str()) || 0 == strcmp("finished_consign", destdbname.c_str()) )
				{
					Marshal::OctetsStream key_os(key), value_new, value_old(value), value1;
					{
						int userid_seller_old, userid_buyer_old;
						int64_t sn;
						key_os >> sn;
					
						if( sn == 0 )
						{
							value_new << sn;
							pstorage->insert( key, value_new, txn );
							return true;
						}

						GConsignDB detail;;
						try{
							value_old >> detail;
						}
						catch( Marshal::Exception & )
						{
							printf("%s database Marshal GConsignDB error,key sn=%lld \n", destdbname.c_str(),sn);
							return true;
						}
						
						if(0==detail.seller_userid)
						{
							//printf("seller_userid == 0, LOGICUID(detail.seller_userid)=%d\n",LOGICUID(detail.seller_userid));
							userid_seller_old = LOGICUID(detail.seller_userid);
						}
						else
						{ 
							userid_seller_old = detail.seller_userid;
						}

						if(0==detail.buyer_userid)
						{
							//printf("buyer_userid == 0, LOGICUID(detail.buyer_userid)=%d\n",LOGICUID(detail.buyer_userid));
							userid_buyer_old = LOGICUID(detail.buyer_userid);
						}
						else
						{
							userid_buyer_old = detail.buyer_userid;
						}
					
						if(-1 == (detail.seller_userid = GetUserIdFromTable(userid_seller_old)))
						{
							return true;
						}
						
						if(-1 == (detail.buyer_userid = GetUserIdFromTable(userid_buyer_old)))
						{
							return true;
						}


						value_new << detail;

						pstorage->insert( key, value_new, txn );
					}
				}
				else if( 0 == strcmp("order", destdbname.c_str()) )
				{
					Marshal::OctetsStream key_os(key), value_new, value_old(value), value1;
					{
						int userid_old;
						
						StockOrder base;
						try{
							value_old >> base;
						}
						catch( Marshal::Exception & )
						{
							printf("%s database Marshal GRoleBase error \n",destdbname.c_str());
							return true;
						}

						userid_old =  base.userid;		
						if(-1 == (base.userid = GetUserIdFromTable(userid_old)))
						{
							return true;
						}
						
						value_new << base;
						pstorage->insert( key, value_new, txn );
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
			Log::log( LOG_ERR, "ExportCsvUserIdExchangeQuery, error when walk, what=%s\n", e.what() );
		}

		return true;
	}
};

void ExportCsvUserIdExchange( const char * srcpath, const char * srcdbname, const char * destdbname )
{
	printf( "\nmerge database %s/%s to %s:\n", srcpath, srcdbname, destdbname );

	std::string src_dir = srcpath;

	ExportCsvUserIdExchangeQuery q;
	q.destdbname = destdbname;
	try
	{		
    		DBStandalone * pstandalone = new DBStandalone( (src_dir+"/"+srcdbname).c_str() );
		pstandalone->init();
		printf("src database (%s) data count %d\n", srcdbname, pstandalone->record_count());
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,(src_dir+"/"+srcdbname).c_str(),pstandalone,new StorageEnv::Uncompressor());
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
		Log::log( LOG_ERR, "MergeDB, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint( );

}

void CheckUserIdExchange( const char * destdbhome )
{
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( destdbhome );
		printf("dest database (%s) data count %d\n", destdbhome, pstorage->count());
	}
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "CheckUserIdExchange, error when walk, what=%s\n", e.what() );
	}

}

void ExportCsvUserIdExchange( const char * srcpath)
{
	std::string src_dir = srcpath;
	if(!ImportCsvUserIdExchange( (src_dir+"/mlzx_alterid.csv").c_str() ))
	{
		printf("ImportCsvUserIdExchange fail\n");
		return;
	}

	ExportCsvUserIdExchange(srcpath, "user", "user");
	CheckUserIdExchange("user");
	ExportCsvUserIdExchange(srcpath, "base", "base");
	CheckUserIdExchange("base");
	ExportCsvUserIdExchange(srcpath, "consign", "consign");
	CheckUserIdExchange("consign");
	ExportCsvUserIdExchange(srcpath, "finished_consign", "finished_consign");
	CheckUserIdExchange("finished_consign");
	ExportCsvUserIdExchange(srcpath, "order", "order");
	CheckUserIdExchange("order");


	StorageEnv::checkpoint();
	StorageEnv::Close();

	StorageEnv::Open();
	StorageEnv::checkpoint( );
	StorageEnv::removeoldlogs( );
}

class CmpUserIdExchangeQuery : public StorageEnv::IQuery
{
public:
	std::string	destdbname;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value )
	{
		try
		{
			StorageEnv::Storage *pstorage = StorageEnv::GetStorage(destdbname.c_str());
			try
			{
				{					
					Marshal::OctetsStream key_os(key), value_new, value_old(value),value1;
					//printf("dest database (%s) data count %d\n", destdbname.c_str(), pstorage->count());

					if(pstorage->find( key, value1, txn ))
					{
						if(value != value1)
						{							
							printf("value_old != value1\n");
						}
					}
					else
					{
						printf("not find key\n");
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
			Log::log( LOG_ERR, "CmpUserIdExchangeQuery, error when walk, what=%s\n", e.what() );
		}

		return true;
	}
};

void CmpUserIdExchange( const char * srcpath, const char * srcdbname, const char * destdbname )
{
	printf( "\ncheck database %s/%s and %s begin.\n", srcpath, srcdbname, destdbname );

	std::string src_dir = srcpath;

	CmpUserIdExchangeQuery q;
	q.destdbname = destdbname;
	try
	{		
    		DBStandalone * pstandalone = new DBStandalone( (src_dir+"/"+srcdbname).c_str() );
		pstandalone->init();
		printf("src database (%s) data count %d\n", srcdbname, pstandalone->record_count());
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,(src_dir+"/"+srcdbname).c_str(),pstandalone,new StorageEnv::Uncompressor());
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
		Log::log( LOG_ERR, "CmpUserIdExchange, error when walk, what=%s\n", e.what() );
	}
	StorageEnv::checkpoint( );
	printf( "check database %s/%s and %s finished.\n", srcpath, srcdbname, destdbname );
}

void CmpUserIdExchange( const char * srcpath )
{
	std::string src_dir = srcpath;

	CmpUserIdExchange(srcpath, "user", "user");
	CmpUserIdExchange(srcpath, "base", "base");
	CmpUserIdExchange(srcpath, "consign", "consign");
	CmpUserIdExchange(srcpath, "finished_consign", "finished_consign");
	CmpUserIdExchange(srcpath, "order", "order");

	
	StorageEnv::checkpoint();
	StorageEnv::Close();

	StorageEnv::Open();
	StorageEnv::checkpoint( );
	StorageEnv::removeoldlogs( );
}


void ExportRoleList(const char *roleidfile, const char *rolelistfile)
{
	if( 0 != access(roleidfile, R_OK) )
	{
		Log::log(LOG_ERR, "Fail to access roleid file: %s", roleidfile);
		return;
	}

	DBStandalone * pdb = new DBStandalone(rolelistfile);
	if(!pdb)
	{
		Log::log( LOG_ERR, "Cannot open table %s.", rolelistfile);
		return;
	}
	pdb->init();

	std::set<unsigned int> roleidset;
	ReadRoleIDFile(roleidfile, roleidset);

	int count = 0;
	StorageEnv::AtomTransaction txn;
	StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
	StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
	StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
	StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
	StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
	StorageEnv::Storage * ptask = StorageEnv::GetStorage("task");

	for (std::set<unsigned int>::iterator itr=roleidset.begin(); itr!=roleidset.end(); itr++)
	{
		try
		{
			Marshal::OctetsStream oskey, osroledata;
			oskey << RoleId(*itr);

			GRoleData roledata;
			Octets ocvalue;
			try
			{
				if(!pbase->find(oskey, ocvalue, txn))
				{
					Log::log( LOG_ERR, "ExportRoleList, roleid=%d not found", *itr);
					continue;
				}
				Marshal::OctetsStream(ocvalue) >> roledata.base;
				if (!roledata.base.userid)
					roledata.base.userid = LOGICUID(roledata.base.id);
				roledata.base.familyid = 0;
				roledata.base.spouse = 0;
				roledata.base.title = 0;
				roledata.base.jointime = 0;
				roledata.base.sectid = 0;

				if (pstatus->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.status;
				if (pinventory->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.pocket;
				if (pstorehouse->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.storehouse;
				if (ptask->find(oskey, ocvalue, txn))
					Marshal::OctetsStream(ocvalue) >> roledata.task;
                                if (pbase2->find(oskey, ocvalue, txn))
                                {
                                        Marshal::OctetsStream(ocvalue) >> roledata.base2;
                                        roledata.base2.bonus_withdraw = 0;
                                        roledata.base2.bonus_reward = 0;
                                        roledata.base2.bonus_used = 0;
                                        roledata.base2.exp_withdraw_today = 0;
                                        roledata.base2.exp_withdraw_time = 0;
                                }
				osroledata << roledata;
				pdb->put( oskey.begin(), oskey.size(), osroledata.begin(), osroledata.size() );

				count++;
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
			Log::log( LOG_ERR, "ExportRoleList, error when find roleid=%d, what=%s\n", *itr, e.what());
		}
	}

	pdb->checkpoint();
	delete pdb;

	Log::log( LOG_INFO, "Export %d roles to %s", count, rolelistfile );
}

void ListTerritory( )
{
	try
	{
		StorageEnv::Storage *pcity = StorageEnv::GetStorage("citystore");
		StorageEnv::CommonTransaction txn;

		Marshal::OctetsStream key, value;
		GTerritoryStore store;
		try
		{
			key << 1;
			if(pcity->find(key, value, txn))
				value >> store;
			GTerritoryInfoVector::iterator tit, tite;
			for (tit=store.tlist.begin(),tite=store.tlist.end(); tit!=tite; tit++)
			{
				printf("tid=%d, owner=%d\n", tit->id, tit->owner);
			}
		}
		catch (DbException &e) { throw; }
		catch ( ... )
		{
			DbException ee(DB_OLD_VERSION);
			txn.abort(ee);
			throw ee;
		}
	}
	catch (DbException &e)
	{
		Log::log(LOG_ERR, "ListTerritory failed, what=%s\n", e.what());
	}
}

class SyncNameChangeQuery : public StorageEnv::IQuery
{
	enum
	{
		cache_low = 20000,
		cache_high = 25000,
	};
	typedef  bool (SyncNameChangeQuery::*UpdateFunc)(StorageEnv::Transaction &, Octets &, Octets &);
	typedef std::map<int, Octets> NameMap;
	UpdateFunc m_updatefunc;
	NameMap name_map;

	TableWrapper tab_rolenamehis;
	TableWrapper tab_friends;
	TableWrapper tab_mail;
	TableWrapper tab_message;
	TableWrapper tab_consign;
	TableWrapper tab_snsmessage;

	std::map<int, GFriendList> m_mapFriendList;
	std::map<int, GMailBox> m_mapMailbox;
	std::map<int64_t, GSNSMessage> m_mapSNSMessage;
	std::map<int, MessageVector> m_mapMessage;
	std::map<int64_t, GConsignDB> m_mapConsign;

	bool WalkTable(TableWrapper & table, UpdateFunc func)
	{
		m_updatefunc = func;
		try
		{
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, table.name.c_str(), table.ptable,
						table.uncompressor);
				cursor.walk(*this);
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
			Log::log( LOG_ERR, "SyncNameChange walktable error '%s' , what=%s\n", table.name.c_str(), e.what() );
			return false;
		}
		return true;
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
			NameMap::iterator it = name_map.find(detail.seller_roleid);
			if (it != name_map.end() && (detail.seller_name != it->second))
			{
				detail.seller_name = it->second;
				need_repair = true;
			}
			it = name_map.find(detail.buyer_roleid);
			if (it != name_map.end() && (detail.buyer_name != it->second))
			{
				detail.buyer_name = it->second;
				need_repair = true;
			}
			if (need_repair)
				m_mapConsign[sn] = detail;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "SyncNameChangeQuery::UpdateConsign, exception\n" );
		}
		return true;
	}

	bool WalkConsign()
	{
		return WalkTable(tab_consign, &SyncNameChangeQuery::UpdateConsign);
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
			for (it = mailbox.mails.begin(); it != ie; ++it)
			{
				if(it->header.sndr_type == _MST_LOCAL_CONSIGN)//skip sender change for consign mail
					continue;
				NameMap::iterator nit = name_map.find(it->header.sender);
				if (nit != name_map.end() && it->header.sender_name != nit->second)
				{
					it->header.sender_name = nit->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapMailbox[id] = mailbox;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "SyncNameChangeQuery::UpdateMailbox, exception\n" );
		}
		return true;
	}

	bool WalkMailbox()
	{
		return WalkTable(tab_mail, &SyncNameChangeQuery::UpdateMailbox);
	}

	bool UpdateMessage(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			MessageVector msgs;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> msgs;
			MessageVector::iterator it, ie = msgs.end();
			bool need_repair = false;
			for (it = msgs.begin(); it != ie; ++it)
			{
				NameMap::iterator nit = name_map.find(it->srcroleid);
				if (nit != name_map.end() && it->src_name != nit->second)
				{
					it->src_name = nit->second;
					need_repair = true;
				}
				nit = name_map.find(it->dstroleid);
				if (nit != name_map.end() && it->dst_name != nit->second)
				{
					it->dst_name = nit->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapMessage[id] = msgs;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "SyncNameChangeQuery::UpdateMessage, exception\n" );
		}
		return true;
	}
	bool WalkMessage()
	{
		return WalkTable(tab_message, &SyncNameChangeQuery::UpdateMessage);
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
			NameMap::iterator it, ite = name_map.end();
			GFriendInfoVector::iterator fit, fie = friendlist.friends.end();
			for (fit = friendlist.friends.begin(); fit != fie; ++fit)
			{
				it = name_map.find(fit->rid);
				if (it != ite && fit->name != it->second)
				{
					fit->name = it->second;
					need_repair = true;
				}
			}
			GEnemyInfoVector::iterator eit, eie = friendlist.enemies.end();
			for (eit = friendlist.enemies.begin(); eit != eie; ++eit)
			{
				it = name_map.find(eit->rid);
				if (it != ite && eit->name != it->second)
				{
					eit->name = it->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapFriendList[id] = friendlist;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "SyncNameChangeQuery::UpdateFriendList, exception\n" );
		}
		return true;
	}

	bool WalkFriendList()
	{
		return WalkTable(tab_friends, &SyncNameChangeQuery::UpdateFriendList);
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
			NameMap::iterator nit, nite = name_map.end();
			nit = name_map.find(msg.roleid);
			if (nit != nite && msg.rolename != nit->second)
			{
				msg.rolename = nit->second;
				need_repair = true;
			}
			GLeaveMessageVector::iterator lit, lite = msg.leave_msgs.end();
			for (lit = msg.leave_msgs.begin(); lit != lite; ++lit)
			{
				nit = name_map.find(lit->roleid);
				if (nit != nite && lit->rolename != nit->second)
				{
					lit->rolename = nit->second;
					need_repair = true;
				}
			}
			GApplyInfoVector::iterator it, ie = msg.apply_list.end();
			for (it = msg.apply_list.begin(); it != ie; ++it)
			{
				nit = name_map.find(it->roleid);
				if (nit != nite && it->rolename != nit->second)
				{
					it->rolename = nit->second;
					need_repair = true;
				}
			}
			ie = msg.agree_list.end();
			for (it = msg.agree_list.begin(); it != ie; ++it)
			{
				nit = name_map.find(it->roleid);
				if (nit != nite && it->rolename != nit->second)
				{
					it->rolename = nit->second;
					need_repair = true;
				}
			}
			ie = msg.deny_list.end();
			for (it = msg.deny_list.begin(); it != ie; ++it)
			{
				nit = name_map.find(it->roleid);
				if (nit != nite && it->rolename != nit->second)
				{
					it->rolename = nit->second;
					need_repair = true;
				}
			}
			if (need_repair)
				m_mapSNSMessage[nkey] = msg;
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "SyncNameChangeQuery::UpdateSect, exception\n" );
		}
		return true;
	}

	bool WalkSNSMessage()
	{
		return WalkTable(tab_snsmessage, &SyncNameChangeQuery::UpdateSNSMessage);
	}

public:

	bool RepairMailbox()
	{
		if (!WalkMailbox()) return false;
		LOG_TRACE("SyncNameChange::RepairMailbox count=%d", m_mapMailbox.size());
		try
		{
			std::map<int, GMailBox>::iterator bit, bie=m_mapMailbox.end();
			for (bit=m_mapMailbox.begin(); bit != bie; ++bit)
			{
				tab_mail.Put(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncNameChange::RepairMailbox error, what=%s\n", e.what() );
			return false;
		}
		tab_mail.Checkpoint();
		m_mapMailbox.clear();
		return true;
	}

	bool RepairMessage()
	{
		if (!WalkMessage()) return false;
		LOG_TRACE("SyncNameChange::RepairMessage count=%d", m_mapMessage.size());
		try
		{
			std::map<int, MessageVector>::iterator bit, bie=m_mapMessage.end();
			for (bit=m_mapMessage.begin(); bit != bie; ++bit)
			{
				tab_message.Put(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncNameChange::RepairMessage error, what=%s\n", e.what() );
			return false;
		}
		tab_message.Checkpoint();
		m_mapMessage.clear();
		return true;
	}

	bool RepairFriendList()
	{
		if (!WalkFriendList()) return false;
		LOG_TRACE("SyncNameChange::RepairFriendlist count=%d", m_mapFriendList.size());
		try
		{
			std::map<int, GFriendList>::iterator bit, bie=m_mapFriendList.end();
			for (bit=m_mapFriendList.begin(); bit != bie; ++bit)
			{
				tab_friends.Put(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncNameChange::RepairFriendList error, what=%s\n", e.what() );
			return false;
		}
		tab_friends.Checkpoint();
		m_mapFriendList.clear();
		return true;
	}
	
	bool RepairSNS()
	{
		if (!WalkSNSMessage()) return false;
		LOG_TRACE("SyncNameChange::RepairSNS count=%d", m_mapSNSMessage.size());
		try
		{
			std::map<int64_t, GSNSMessage>::iterator it, ie = m_mapSNSMessage.end();
			for (it=m_mapSNSMessage.begin(); it != ie; ++it)
			{
				tab_snsmessage.Put(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncNameChange::RepairSNSMessage error, what=%s\n", e.what() );
			return false;
		}
		tab_snsmessage.Checkpoint();
		m_mapSNSMessage.clear();
		return true;
	}

	bool RepairConsign()
	{
		if (!WalkConsign()) return false;
		LOG_TRACE("SyncNameChange::RepairConsign count=%d", m_mapConsign.size());
		try
		{
			std::map<int64_t, GConsignDB>::iterator bit, bie=m_mapConsign.end();
			for (bit=m_mapConsign.begin(); bit != bie; ++bit)
			{
				if(bit->first!=0)
					tab_consign.Put(Marshal::OctetsStream()<<bit->first, Marshal::OctetsStream()<<bit->second);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "SyncNameChange::RepairConsign error, what=%s\n", e.what() );
			return false;
		}
		m_mapConsign.clear();
		tab_consign.Checkpoint();
		return true;
	}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		return (this->*m_updatefunc)(txn, key, value);
	}

	bool Init(const char * dbdata)
	{
		std::string db = dbdata;
		if (access(db.c_str(), F_OK))
		{
			Log::log(LOG_ERR, "Error: cannot find %s", db.c_str());
			return false;
		}
		tab_rolenamehis.SetName(db+"/rolenamehis");
		tab_friends.SetName(db+"/friends");
		tab_mail.SetName(db+"/mailbox");
		tab_message.SetName(db+"/messages");
		tab_consign.SetName(db+"/consign");
		tab_snsmessage.SetName(db+"/snsmessage");

		if (!tab_rolenamehis.Open(cache_high, cache_low, true) ||
				!tab_friends.Open(cache_high, cache_low, true) ||
				!tab_mail.Open(cache_high, cache_low, true) ||
				!tab_message.Open(cache_high, cache_low, true) ||
				!tab_consign.Open(cache_high, cache_low, true) ||
				!tab_snsmessage.Open(cache_high, cache_low, true))
		{
			Log::log(LOG_ERR, "open db error");
			return false;
		}
		Marshal::OctetsStream key_all, value_all;
		key_all << (int)0;
		name_map.clear();
		try
		{
			if (tab_rolenamehis.Find(key_all, value_all))
				value_all >> name_map;
		}
		catch(...)
		{
			Log::log(LOG_ERR, "Gen name map exception");
			return false;
		}
		if (name_map.empty())
		{
			Log::log(LOG_ERR, "name_map is empty ?!");
			return false;
		}
		LOG_TRACE("name_map.size %d", name_map.size());
		return true;
	}

	bool ClearNameMap()
	{
		Marshal::OctetsStream key_all;
		key_all << (int)0;
		name_map.clear();
		try
		{
			tab_rolenamehis.Put(key_all, Marshal::OctetsStream() << name_map);
		}
		catch(...)
		{
			Log::log(LOG_ERR, "clear name map exception");
			return false;
		}
		tab_rolenamehis.Checkpoint();
		return true;
	}
};

bool SyncNameChange(const char * dbdata)
{
	SyncNameChangeQuery q;
	return q.Init(dbdata) && q.RepairMailbox() && q.RepairMessage() && q.RepairFriendList()
		&& q.RepairSNS() && q.RepairConsign() &&
		q.ClearNameMap();
}

class VerifyNameQuery : public StorageEnv::IQuery
{
	enum
	{
		cache_low = 20000,
		cache_high = 25000,
	};
	typedef  bool (VerifyNameQuery::*UpdateFunc)(StorageEnv::Transaction &, Octets &, Octets &);
	typedef std::map<int, Octets> NameMap;
	UpdateFunc m_updatefunc;
	NameMap name_map;

	TableWrapper tab_rolenamehis;
	TableWrapper tab_friends;
	TableWrapper tab_mail;
	TableWrapper tab_message;
	TableWrapper tab_consign;
	TableWrapper tab_snsmessage;
	TableWrapper tab_base;
	TableWrapper tab_rolename;
	TableWrapper tab_family;
	TableWrapper tab_sect;
	TableWrapper tab_snsplayer;

	bool WalkTable(TableWrapper & table, UpdateFunc func)
	{
		m_updatefunc = func;
		try
		{
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor(&txn, table.name.c_str(), table.ptable,
						table.uncompressor);
				cursor.walk(*this);
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
			Log::log( LOG_ERR, "SyncNameChange walktable error '%s' , what=%s\n", table.name.c_str(), e.what() );
			return false;
		}
		return true;
	}

	//生成NameMap
	bool UpdateRolenameHis(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = 0;
		try
		{
			GRoleNameHisVector namehis;
			Marshal::OctetsStream(key) >> roleid;
			if(roleid == 0)
				return true;
			Marshal::OctetsStream os_base;
			if (!tab_base.Find(key, os_base))
			{
				Log::log(LOG_ERR, "VerifyNameQuery gen namemap, but can not find base %d", roleid);
				return true;
			}
			GRoleBase base;
			os_base >> base;
			name_map[roleid] = base.name;

			Marshal::OctetsStream os_rolename;
			if (!tab_rolename.Find(base.name, os_rolename))
			{
				Log::log(LOG_ERR, "VerifyNameQuery gen namemap, roleid %d rolename not match", roleid);
				return true;
			}
			int tmpid = 0;
			os_rolename >> tmpid;
			if (roleid != tmpid)
			{
				Log::log(LOG_ERR, "VerifyNameQuery gen namemap, roleid %d rolename not match2", roleid);
				return true;
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateRolenameHis, roleid %d exception\n", roleid);
		}
		return true;
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
			NameMap::iterator it = name_map.find(detail.seller_roleid);
			if (it != name_map.end() && (detail.seller_name != it->second))
				Log::log(LOG_ERR, "consign id %lld seller %d name not match", sn, detail.seller_roleid);
			it = name_map.find(detail.buyer_roleid);
			if (it != name_map.end() && (detail.buyer_name != it->second))
				Log::log(LOG_ERR, "consign id %lld seller %d name not match", sn, detail.buyer_roleid);
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateConsign, exception\n" );
		}
		return true;
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
			for (it = mailbox.mails.begin(); it != ie; ++it)
			{
				if(it->header.sndr_type == _MST_LOCAL_CONSIGN)//skip sender change for consign mail
					continue;
				NameMap::iterator nit = name_map.find(it->header.sender);
				if (nit != name_map.end() && it->header.sender_name != nit->second)
					Log::log(LOG_ERR, "roleid %d mail sender %d name not match", id, it->header.sender);
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateMailbox, exception\n" );
		}
		return true;
	}

	bool UpdateMessage(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			MessageVector msgs;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> msgs;
			MessageVector::iterator it, ie = msgs.end();
			for (it = msgs.begin(); it != ie; ++it)
			{
				NameMap::iterator nit = name_map.find(it->srcroleid);
				if (nit != name_map.end() && it->src_name != nit->second)
					Log::log(LOG_ERR, "roleid %d message srcroleid %d name not match", id, it->srcroleid);
				nit = name_map.find(it->dstroleid);
				if (nit != name_map.end() && it->dst_name != nit->second)
					Log::log(LOG_ERR, "roleid %d message dstroleid %d name not match", id, it->dstroleid);
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateMessage, exception\n" );
		}
		return true;
	}

	bool UpdateFriendList(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GFriendList friendlist;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> friendlist;
			NameMap::iterator it, ite = name_map.end();
			GFriendInfoVector::iterator fit, fie = friendlist.friends.end();
			for (fit = friendlist.friends.begin(); fit != fie; ++fit)
			{
				it = name_map.find(fit->rid);
				if (it != ite && fit->name != it->second)
					Log::log(LOG_ERR, "roleid %d friend %d name not match", id, fit->rid);
			}
			GEnemyInfoVector::iterator eit, eie = friendlist.enemies.end();
			for (eit = friendlist.enemies.begin(); eit != eie; ++eit)
			{
				it = name_map.find(eit->rid);
				if (it != ite && eit->name != it->second)
					Log::log(LOG_ERR, "roleid %d enemy %d name not match", id, eit->rid);
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateFriendList, exception\n" );
		}
		return true;
	}

	bool UpdateSNSMessage(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int64_t nkey;
			GSNSMessage msg;
			Marshal::OctetsStream(key) >> nkey;
			Marshal::OctetsStream(value) >> msg;
			NameMap::iterator nit, nite = name_map.end();
			nit = name_map.find(msg.roleid);
			if (nit != nite && msg.rolename != nit->second)
			{
				Octets name1, name11, name2, name22;
				CharsetConverter::conv_charset_u2l(msg.rolename, name1);
				EscapeCSVString( name1, name11);
				CharsetConverter::conv_charset_u2l(nit->second, name2);
				EscapeCSVString( name2, name22);
				Log::log(LOG_ERR, "%.*s : %.*s", name11.size(), (char*)name11.begin(), name22.size(), (char*)name22.begin());
				Log::log(LOG_ERR, "snsmessage key %lld roleid %d name not match %.*s : %.*s", nkey, msg.roleid, name11.size(), (char*)name11.begin(), name22.size(), (char*)name22.begin());
			}
			GLeaveMessageVector::iterator lit, lite = msg.leave_msgs.end();
			for (lit = msg.leave_msgs.begin(); lit != lite; ++lit)
			{
				nit = name_map.find(lit->roleid);
				if (nit != nite && lit->rolename != nit->second)
				{
					Octets name1, name11, name2, name22;
					CharsetConverter::conv_charset_u2l(lit->rolename, name1);
					EscapeCSVString( name1, name11);
					CharsetConverter::conv_charset_u2l(nit->second, name2);
					EscapeCSVString( name2, name22);
					Log::log(LOG_ERR, "%.*s : %.*s", name11.size(), (char*)name11.begin(), name22.size(), (char*)name22.begin());

					Log::log(LOG_ERR, "snsmessage key %lld  leave_msg roleid %d name not match", nkey, lit->roleid);
				}
			}
			GApplyInfoVector::iterator it, ie = msg.apply_list.end();
			for (it = msg.apply_list.begin(); it != ie; ++it)
			{
				nit = name_map.find(it->roleid);
				if (nit != nite && it->rolename != nit->second)
				{
					Octets name1, name11, name2, name22;
					CharsetConverter::conv_charset_u2l(it->rolename, name1);
					EscapeCSVString( name1, name11);
					CharsetConverter::conv_charset_u2l(nit->second, name2);
					EscapeCSVString( name2, name22);
					Log::log(LOG_ERR, "%.*s : %.*s", name11.size(), (char*)name11.begin(), name22.size(), (char*)name22.begin());

					Log::log(LOG_ERR, "snsmessage key %lld  apply roleid %d name not match", nkey, it->roleid);
				}
			}
			ie = msg.agree_list.end();
			for (it = msg.agree_list.begin(); it != ie; ++it)
			{
				nit = name_map.find(it->roleid);
				if (nit != nite && it->rolename != nit->second)
				{
					Octets name1, name11, name2, name22;
					CharsetConverter::conv_charset_u2l(it->rolename, name1);
					EscapeCSVString( name1, name11);
					CharsetConverter::conv_charset_u2l(nit->second, name2);
					EscapeCSVString( name2, name22);
					Log::log(LOG_ERR, "%.*s : %.*s", name11.size(), (char*)name11.begin(), name22.size(), (char*)name22.begin());

					Log::log(LOG_ERR, "snsmessage key %lld  agree roleid %d name not match", nkey, it->roleid);
				}
			}
			ie = msg.deny_list.end();
			for (it = msg.deny_list.begin(); it != ie; ++it)
			{
				nit = name_map.find(it->roleid);
				if (nit != nite && it->rolename != nit->second)
				{
					Octets name1, name11, name2, name22;
					CharsetConverter::conv_charset_u2l(it->rolename, name1);
					EscapeCSVString( name1, name11);
					CharsetConverter::conv_charset_u2l(nit->second, name2);
					EscapeCSVString( name2, name22);
					Log::log(LOG_ERR, "%.*s : %.*s", name11.size(), (char*)name11.begin(), name22.size(), (char*)name22.begin());

					Log::log(LOG_ERR, "snsmessage key %lld  deny roleid %d name not match", nkey, it->roleid);
				}
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateSect, exception\n" );
		}
		return true;
	}

	bool UpdateFamily(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GFamily family;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> family;
			NameMap::iterator it, ite = name_map.end();
			std::vector<GFolk>::const_iterator fit, fite = family.member.end();
			for (fit = family.member.begin(); fit != fite; ++fit)
			{
				it = name_map.find(fit->rid);
				if (it != ite && fit->name != it->second)
					Log::log(LOG_ERR, "family %d folk %d name not match", id, fit->rid);
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateFamily, exception\n" );
		}
		return true;
	}

	bool UpdateSect(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GSect sect;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> sect;
			NameMap::iterator it, ite = name_map.end();
			it = name_map.find(id);
			if (it != ite && sect.name != it->second)
				Log::log(LOG_ERR, "sect %d name not match", id);
			std::vector<GDisciple>::const_iterator dit, dite = sect.disciples.end();
			for (dit = sect.disciples.begin(); dit != dite; ++dit)
			{
				it = name_map.find(dit->roleid);
				if (it != ite && dit->name != it->second)
					Log::log(LOG_ERR, "sect %d disciple %d name not match", id, dit->roleid);
			}
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateSect, exception\n" );
		}
		return true;
	}

	bool UpdateSNSPlayer(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int id;
			GSNSPlayerInfoCache sns;
			Marshal::OctetsStream(key) >> id;
			Marshal::OctetsStream(value) >> sns;
			NameMap::iterator it, ite = name_map.end();
			it = name_map.find(id);
			if (it != ite && sns.roleinfo.rolename != it->second)
				Log::log(LOG_ERR, "snsplayer %d roleinfo.name not match", id);
			if (it != ite && sns.playerinfo.rolename != it->second)
				Log::log(LOG_ERR, "snsplayer %d playerinfo.name not match", id);
		}
		catch ( ... )
		{
			Log::log( LOG_ERR, "VerifyNameQuery::UpdateSNSPlayer, exception\n" );
		}
		return true;
	}

public:

	bool GenNameMap()
	{
		WalkTable(tab_rolenamehis, &VerifyNameQuery::UpdateRolenameHis);
		LOG_TRACE("name_map size %d", name_map.size());
		return name_map.size() > 0;
	}

	void CheckNameMap()
	{
		Marshal::OctetsStream key_all, value_all;
		key_all << (int)0;
		NameMap tmp_map;
		tmp_map.clear();
		try
		{
			if (tab_rolenamehis.Find(key_all, value_all))
				value_all >> tmp_map;
		}
		catch(...)
		{
			Log::log(LOG_ERR, "Gen tmp name map exception");
			return;
		}
		LOG_TRACE("tmp_name_map.size %d name_map.size %d", tmp_map.size(), name_map.size());
		NameMap::iterator it, ite = tmp_map.end();
		for (it = tmp_map.begin(); it != ite; ++it)
		{
			NameMap::iterator it2 = name_map.find(it->first);
			if (it2 == name_map.end() || it2->second != it->second)
				Log::log(LOG_ERR, "VerifyNameQuery roleid %d changename not match", it->first);
		}
	}

	bool VerifyConsign()
	{
		LOG_TRACE("Verify Consign");
		return WalkTable(tab_consign, &VerifyNameQuery::UpdateConsign);
	}

	bool VerifyMailBox()
	{
		LOG_TRACE("Verify Mailbox");
		return WalkTable(tab_mail, &VerifyNameQuery::UpdateMailbox);
	}

	bool VerifyMessage()
	{
		LOG_TRACE("Verify Message");
		return WalkTable(tab_message, &VerifyNameQuery::UpdateMessage);
	}

	bool VerifyFriendList()
	{
		LOG_TRACE("Verify FriendList");
		return WalkTable(tab_friends, &VerifyNameQuery::UpdateFriendList);
	}

	bool VerifySNSMessage()
	{
		LOG_TRACE("Verify SNSMessage");
		return WalkTable(tab_snsmessage, &VerifyNameQuery::UpdateSNSMessage);
	}

	bool VerifyFamily()
	{
		LOG_TRACE("Verify Family");
		return WalkTable(tab_family, &VerifyNameQuery::UpdateFamily);
	}

	bool VerifySect()
	{
		LOG_TRACE("Verify Sect");
		return WalkTable(tab_sect, &VerifyNameQuery::UpdateSect);
	}

	bool VerifySNSPlayer()
	{
		LOG_TRACE("Verify SNSPlayer");
		return WalkTable(tab_snsplayer, &VerifyNameQuery::UpdateSNSPlayer);
	}

	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		return (this->*m_updatefunc)(txn, key, value);
	}

	bool Init(const char * dbdata)
	{
		std::string db = dbdata;
		if (access(db.c_str(), F_OK))
		{
			Log::log(LOG_ERR, "Error: cannot find %s", db.c_str());
			return false;
		}
		tab_rolenamehis.SetName(db+"/rolenamehis");
		tab_friends.SetName(db+"/friends");
		tab_mail.SetName(db+"/mailbox");
		tab_message.SetName(db+"/messages");
		tab_consign.SetName(db+"/consign");
		tab_snsmessage.SetName(db+"/snsmessage");
		tab_base.SetName(db+"/base");
		tab_rolename.SetName(db+"/rolename");
		tab_family.SetName(db+"/family");
		tab_sect.SetName(db+"/sect");
		tab_snsplayer.SetName(db+"/snsplayer");

		if (!tab_rolenamehis.Open(cache_high, cache_low) ||
				!tab_friends.Open(cache_high, cache_low) ||
				!tab_mail.Open(cache_high, cache_low) ||
				!tab_message.Open(cache_high, cache_low) ||
				!tab_consign.Open(cache_high, cache_low) ||
				!tab_snsmessage.Open(cache_high, cache_low) ||
				!tab_base.Open(cache_high, cache_low) ||
				!tab_rolename.Open(cache_high, cache_low) ||
				!tab_family.Open(cache_high, cache_low) ||
				!tab_sect.Open(cache_high, cache_low) ||
				!tab_snsplayer.Open(cache_high, cache_low))
		{
			Log::log(LOG_ERR, "open db error");
			return false;
		}
		return true;
	}
};

void VerifyName(const char * dbdata)
{
	VerifyNameQuery q;
	if (!q.Init(dbdata))
		return;
	if (!q.GenNameMap())
		return;
	q.CheckNameMap();
	q.VerifyConsign();
	q.VerifyMailBox();
	q.VerifyMessage();
	q.VerifyFriendList();
	q.VerifySNSMessage();
	q.VerifyFamily();
	q.VerifySect();
	q.VerifySNSPlayer();
}

class WalkCollectKeysQuery : public StorageEnv::IQuery
{
public:
	std::vector<Octets> keys;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		keys.push_back(key);
		return true;
	}
};

void WalkCheckTable(const char * tablename )
{
	StorageEnv::Close();
	StorageEnv::Open();

	LOG_TRACE( "Walk %s:", tablename);
	PreLoadTable((StorageEnv::get_datadir()+"/"+tablename).c_str());
	WalkCollectKeysQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage(tablename);
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "Walk %s, error when walk, what=%s\n", tablename, e.what() );
	}
	LOG_TRACE( "Walk %s: size=%d", tablename, q.keys.size());
	
	std::vector<Octets>::const_iterator it = q.keys.begin(), ite = q.keys.end();
	bool finished = false;
	int count = 0;
	while (!finished)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage(tablename);
			StorageEnv::AtomTransaction	txn;
			try
			{
				int loop_count = 0;
				for (; it != ite && loop_count < 65536; ++it,++loop_count)
				{
					Marshal::OctetsStream value;
					if (!pstorage->find(*it, value, txn))
						Log::log(LOG_ERR, "can't find key, count = %d", count);
					else
						count++;
				}
				if (it == ite)
					finished = true;
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
			Log::log( LOG_ERR, "Check %s, error, what=%s\n", tablename, e.what() );
		}
		time_t now = time(NULL);
		LOG_TRACE("Processed %d items time:%s", count, ctime(&now));
		StorageEnv::checkpoint();
	}
}

void WalkTables()
{
	GNET::Conf *conf = GNET::Conf::GetInstance();
	std::vector<std::string>	tables;
	ParseStrings(conf->find( "storagewdb", "walktest" ), tables);
	std::vector<std::string>::const_iterator it, ite = tables.end();
	for (it = tables.begin(); it != ite; ++it)
		WalkCheckTable(it->c_str());
}

class DupRolenameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * prolename = StorageEnv::GetStorage("rolename");
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
			StorageEnv::Storage * pfamily = StorageEnv::GetStorage("family");
			StorageEnv::Storage * pfaction = StorageEnv::GetStorage("faction");
			Marshal::OctetsStream os_rolename, os_base, os_user, os_status, os_family, os_faction, krole;

			int roleid = 0;
			if (!prolename->find(key, os_rolename, txn))
				return true;
			os_rolename >> roleid;
			krole << roleid;
			if (!pbase->find(krole, os_base, txn))
			{
				Log::log(LOG_ERR, "can not find roleid %d", roleid);
				return true;
			}
			GRoleBase base;
			os_base >> base;
			if (base.name != key)
			{
				Log::log(LOG_ERR, "it is an old name of roleid %d", roleid);
				return true; //不是当前正在使用的名字
			}
			int userid = (0==base.userid ? (LOGICUID(base.id)) : base.userid);
			if (!puser->find(Marshal::OctetsStream()<<userid, os_user, txn))
			{
				Log::log(LOG_ERR, "can not find user %d for role %d", userid, roleid);
				return true;
			}
			User user;
			os_user >> user;
			GRoleStatus status;
			if (pstatus->find(krole, os_status, txn))
				os_status >> status;
			GFactionInfo faction;
			if (base.familyid)
			{
				GFamily family;
				if (pfamily->find(Marshal::OctetsStream()<<base.familyid, os_family, txn))
				{
					os_family >> family;
					if (pfaction->find(Marshal::OctetsStream()<<family.factionid, os_faction, txn))
						os_faction >> faction;
				}
			}
			Octets  namegbk, nameesc;
			CharsetConverter::conv_charset_u2l(key, namegbk );
			EscapeCSVString( namegbk, nameesc );
			struct tm dt_create, dt_lastlogin;
			localtime_r((const time_t *)&base.create_time, &dt_create);
			localtime_r((const time_t *)&base.lastlogin_time, &dt_lastlogin);
			//rolename roleid userid status cash_add level occupy fac_level fac_title create_time lastlogin_time
			LOG_TRACE("%.*s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d-%d-%d\t%d-%d-%d",
					nameesc.size(), (char*)nameesc.begin(),
					roleid, userid, base.status, user.cash_add, status.level+status.reborndata.size()/sizeof(int)*200,
					status.occupation, faction.level, base.title, dt_create.tm_year+1900, dt_create.tm_mon+1, dt_create.tm_mday,
					dt_lastlogin.tm_year+1900, dt_lastlogin.tm_mon+1, dt_lastlogin.tm_mday);
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



void ListDupRolename()
{
	TableWrapper tab_duprolename("./duprolename");
	if (!tab_duprolename.Open(20000, 25000, false))
	{
		Log::log(LOG_ERR, "open duprolename failed");
		return;
	}
	PreLoadTable((StorageEnv::get_datadir()+"/rolename").c_str());
	PreLoadTable((StorageEnv::get_datadir()+"/user").c_str());
	PreLoadTable((StorageEnv::get_datadir()+"/base").c_str());
	PreLoadTable((StorageEnv::get_datadir()+"/status").c_str());
	LOG_TRACE("rolename\troleid\tuserid\tstatus\tcash_add\tlevel\toccupation\tfac_level\tfac_title\tcreate_time\tlastlogin_time");
	DupRolenameQuery q;
	try
	{
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn, tab_duprolename.name.c_str(), tab_duprolename.ptable, tab_duprolename.uncompressor);
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
		Log::log( LOG_ERR, "walk table, error when walk, what=%s\n", e.what() );
	}
//	StorageEnv::checkpoint();
}

class ListBase2Query : public StorageEnv::IQuery
{
public:
	int count;
	int zoneid;
	bool hasmore;
	Octets handle;
	ListBase2Query():count(0), hasmore(false){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId		id;
		GRoleBase2	base2;
		try
		{
			key_os >> id;
			value_os >> base2;
			if(++count % checkpoint_threshold == 0)
			{       
				handle = key;
				hasmore = true;
				return false;
			}
			printf("%d", id.id);
			printf(",%d", zoneid);
			printf(",%d", base2.runescore);
			printf(",%lld", base2.comsumption);
			printf(",%d", base2.bonus_withdraw + base2.bonus_reward);
			printf(",%d", base2.bonus_withdraw);
			printf(",%d", base2.bonus_reward);
			printf(",%d\n", base2.bonus_used);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListBase2Query, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListBase2(int zoneid)
{
	printf("roleid");
	printf(",zoneid");
	printf(",runescore");
	printf(",consumption");
	printf(",bonus_count");// 下线鸿利+其他鸿利
	printf(",bonus_withdraw");// 下线鸿利
	printf(",bonus_reward");// 其他鸿利
	printf(",bonus_used");// 消费鸿利
	printf("\n");

	ListBase2Query q;
	do
	{
		q.hasmore = false;
		q.zoneid = zoneid;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "base2" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
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
			Log::log( LOG_ERR, "ListBase2, error when walk, what=%s\n", e.what() );
		}
		StorageEnv::checkpoint();
	}while(q.hasmore);
}

class ListInventoryQuery : public StorageEnv::IQuery
{
public:
	int count;
	bool hasmore;
	Octets handle;
	ListInventoryQuery():count(0), hasmore(false){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId		id;
		GRolePocket 	pocket;
		try
		{
			key_os >> id;
			value_os >> pocket;
			if(++count % checkpoint_threshold == 0)
			{       
				handle = key;
				hasmore = true;
				return false;
			}
			printf("%d", id.id);
			printf(",%d", pocket.capacity);
			printf(",%d", pocket.items.size());
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListInventory, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

class ListStorehouseQuery : public StorageEnv::IQuery
{
public:
	int count;
	bool hasmore;
	Octets handle;
	ListStorehouseQuery():count(0), hasmore(false){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;

		RoleId		id;
		GRoleStorehouse store;
		try
		{
			key_os >> id;
			value_os >> store;
			if(++count % checkpoint_threshold == 0)
			{       
				handle = key;
				hasmore = true;
				return false;
			}
			printf("%d", id.id);
			printf(",%d", store.capacity);
			printf(",%d", store.items.size());
			printf("\n");
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListStorehouse, error unmarshal, roleid=%d.", id.id );
			return true;
		}
		return true;
	}
};

void ListItemContainer()
{
	printf("roleid");
	printf(",inventory_capacity");
	printf(",inventory_size");
	printf("\n");

	ListInventoryQuery q;
	do
	{
		q.hasmore = false;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "inventory" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q.handle, q );
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
			Log::log( LOG_ERR, "ListInventory, error when walk, what=%s\n", e.what() );
		}
		StorageEnv::checkpoint();
	}while(q.hasmore);

	printf("roleid");
	printf(",storehouse_capacity");
	printf(",storehouse_size");
	printf("\n");

	ListStorehouseQuery q2;
	do
	{
		q2.hasmore = false;
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "storehouse" );
			StorageEnv::AtomTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor( txn );
				cursor.walk( q2.handle, q2 );
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
			Log::log( LOG_ERR, "ListStorehouse, error when walk, what=%s\n", e.what() );
		}
		StorageEnv::checkpoint();
	}while(q2.hasmore);
}

template <typename FaEntity>
class RepairFaNameQuery : public StorageEnv::IQuery
{
	StorageEnv::Storage * pname;
public:
	RepairFaNameQuery(StorageEnv::Storage * _pname) : pname(_pname) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int id = -1;
		try
		{
			Marshal::OctetsStream(key) >> id;
			FaEntity entity;
			Marshal::OctetsStream(value) >> entity;
			if (entity.namehis.size())
			{
				std::vector<Octets> namelist;
				try
				{
					Marshal::OctetsStream(entity.namehis) >> namelist;
				}
				catch (...)
				{
					Log::log(LOG_ERR, "RepairFaNameQuery error unmarshal namehis, fid=%d", id);
				}
				namelist.push_back(entity.name);
				std::vector<Octets>::const_iterator nit, nite = namelist.end();
				for (nit = namelist.begin(); nit != nite; ++nit)
				{
					Marshal::OctetsStream os_id;
					if (pname->find(*nit, os_id, txn))
					{
						pname->del(*nit, txn);
						LOG_TRACE("repair a name of fid %d, namesize %d", id, (*nit).size());
					}
					pname->insert(Marshal::OctetsStream()<<(*nit), Marshal::OctetsStream()<<id, txn);
				}
			}
		} catch (...) {
			Log::log( LOG_ERR, "RepairFaNameQuery, error unmarshal, fid=%d", id);
		}
		return true;
	}
};	

template <typename FaEntity>
void RepairFaNameTmpl(const std::string & ent_tab, const std::string &
name_tab)
{
	try
	{
		StorageEnv::Storage * pent = StorageEnv::GetStorage(ent_tab.c_str());
		StorageEnv::Storage * pname = StorageEnv::GetStorage(name_tab.c_str());
		StorageEnv::AtomTransaction txn;
		try{
			RepairFaNameQuery<FaEntity> q(pname);
			StorageEnv::Storage::Cursor cursor = pent->cursor( txn );
			cursor.walk(q);
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
		Log::log( LOG_ERR, "RepairFaNameTmpl, what=%s.\n", e.what() );
	}
}

void RepairFaName()
{
	LOG_TRACE("repair familyname begin");
	RepairFaNameTmpl<GFamily>("family", "familyname");
	LOG_TRACE("repair familyname end");

	LOG_TRACE("repair factionname begin");
	RepairFaNameTmpl<GFactionInfo>("faction", "factionname");
	LOG_TRACE("repair factionname end");

	StorageEnv::checkpoint();
	StorageEnv::Close();
	StorageEnv::Open();
}

static std::map<int, int> roleidmap;
static std::map<int, int> factionidmap;
class RewriteFlowerQuery : public StorageEnv::IQuery
{
	TableWrapper & tab_flower;
public:
	RewriteFlowerQuery(TableWrapper & table):tab_flower(table){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int id = -1;
		try
		{
			Marshal::OctetsStream	os_key(key);
			os_key >> id;
			if (id == 0)
				return true;
			std::map<int, int>::const_iterator it = roleidmap.find(id);
			if (it == roleidmap.end())
			{
				Log::log(LOG_ERR, "can't map roleid %d for flower data", id);
				return true;
			}
			if (it->second == 0)
			{
				Log::log(LOG_ERR, "map roleid %d to 0 for flower data", id);
				return true;
			}
			tab_flower.Put(Marshal::OctetsStream()<<it->second, value);
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "RewriteFlowerQuery, exception, roleid=%d", id);
			throw e;
		}
		return true;
	}
};

void RewriteFlowerRoleid(const char * roleidmapfile)
{
	if (access(roleidmapfile, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", roleidmapfile);
		return;
	}
	std::string flowerfile("./topflower.rewrite");
	if (access(flowerfile.c_str(), R_OK) == 0)
	{
		Log::log(LOG_ERR, "%s already exist", flowerfile.c_str());
		return;
	}
	std::ifstream ifs(roleidmapfile);
	string line;
	while (std::getline(ifs, line))
	{
		int oldroleid, userid, newroleid;
		oldroleid = userid = newroleid = 0;
		if (sscanf(line.c_str(), "<%d,%d>->%d", &oldroleid, &userid, &newroleid) == 3)
		{
			if (oldroleid > 0)
				roleidmap[oldroleid] = newroleid;
		}
		else
			Log::log(LOG_ERR, "invalid line %s", line.c_str());
	}
	LOG_TRACE("read roleidmap size %d", roleidmap.size());

	TableWrapper tab_flower(flowerfile.c_str());
	if (!tab_flower.Open(2500, 2000, true))
	{
		Log::log(LOG_ERR, "open %s fail", flowerfile.c_str());
		return;
	}
	RewriteFlowerQuery q(tab_flower);
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("topflower");
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "Walk topflower, error when walk, what=%s\n", e.what() );
	}
	tab_flower.Checkpoint();
}
/*
class RewriteCityQuery : public StorageEnv::IQuery
{
	std::map<int, int> & roleidmap;
	std::map<int, int> & fidmap;
	TableWrapper & tab_city;
public:
	RewriteCityQuery(std::map<int, int> & map1, std::map<int, int> & map2, TableWrapper & table):roleidmap(map1), fidmap(map2), tab_city(table){}
	bool RewriteFid(int * fid)
	{
		if (*fid != 0)
		{
			std::map<int, int>::const_iterator fit = fidmap.find(*fid);
			if (fit == fidmap.end())
			{
				Log::log(LOG_ERR, "can't map fid %d", *fid);
				return false;
			}
			else if (fit->second == 0)
			{
				Log::log(LOG_ERR, "map fid %d to 0", *fid);
				return false;
			}
			else
			{
				LOG_TRACE("map fid from %d to %d", *fid, fit->second);
				*fid = fit->second;
			}
		}
		return true;
	}
	bool RewriteRoleid(int * roleid)
	{
		if (*roleid != 0)
		{
			std::map<int, int>::const_iterator rit = roleidmap.find(*roleid);
			if (rit == roleidmap.end())
			{
				Log::log(LOG_ERR, "can't map roleid %d", *roleid);
				return false;
			}
			else if (rit->second == 0)
			{
				Log::log(LOG_ERR, "map roleid %d to 0", *roleid);
				return false;
			}
			else
			{
				LOG_TRACE("map roleid from %d to %d", *roleid, rit->second);
				*roleid = rit->second;
			}
		}
		return true;
	}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int id = -1;
		try
		{
			Marshal::OctetsStream(key) >> id;
			if (id == 0)
			{
				GCityStore store;
				Marshal::OctetsStream(value) >> store;
				std::vector<GCity>::iterator it, ite = store.cities.end();
				for (it = store.cities.begin(); it != ite; ++it)
				{
					if (!RewriteFid(&(it->info.owner.fid)))
						Log::log(LOG_ERR, "map city owner %d fail", it->info.owner.fid);
					if (!RewriteRoleid(&(it->info.owner.master)))
						Log::log(LOG_ERR, "map city owner master %d fail", it->info.owner.master);
					std::vector<GChallenger>::iterator cit, cite = it->challengers.end();
					for (cit = it->challengers.begin(); cit != cite; ++cit)
					{
						if (!RewriteFid(&(cit->challenger.fid)))
							Log::log(LOG_ERR, "map challenger %d fail", cit->challenger.fid);
						if (!RewriteRoleid(&(cit->challenger.master)))
							Log::log(LOG_ERR, "map challenger master %d fail", cit->challenger.master);
						if (!RewriteFid(&(cit->assistant.fid)))
							Log::log(LOG_ERR, "map assistant %d fail", cit->assistant.fid);
						if (!RewriteRoleid(&(cit->assistant.master)))
							Log::log(LOG_ERR, "map assistant master %d fail", cit->assistant.master);
					}
				}
				tab_city.Put(Marshal::OctetsStream()<<id, Marshal::OctetsStream()<<store);
			}
			else if (id == 1)
			{
				GTerritoryStore store;
				Marshal::OctetsStream(value) >> store;
				GTerritoryInfoVector::iterator it, ite = store.tlist.end();
				for (it = store.tlist.begin(); it != ite; ++it)
				{
					if (!RewriteFid((int *)&(it->owner)))
						Log::log(LOG_ERR, "map territory %d owner %d fail", it->id, it->owner);
					GTChallengeVector::iterator cit, cite = it->challengelist.end();
					for (cit = it->challengelist.begin(); cit != cite; ++cit)
					{
						if (!RewriteFid((int *)&(cit->factionid)))
							Log::log(LOG_ERR, "map territory %d challenger %d fail", it->id, cit->factionid);
					}
					if (!RewriteFid((int *)&(it->defender)))
						Log::log(LOG_ERR, "map territory %d defender %d fail", it->id, it->defender);
					if (!RewriteFid((int *)&(it->success_challenge.factionid)))
						Log::log(LOG_ERR, "map territory %d success_challenge %d fail", it->id, it->success_challenge.factionid);
					LOG_TRACE("territory it->id %d new owner %d", it->id, it->owner);
				}
				tab_city.Put(Marshal::OctetsStream()<<id, Marshal::OctetsStream()<<store);
			}
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "RewriteCityQuery, exception, id=%d", id);
			throw e;
		}
		return true;
	}
};
*/
static bool RewriteFid(int * fid)
{
	if (*fid != 0)
	{
		std::map<int, int>::const_iterator fit = factionidmap.find(*fid);
		if (fit == factionidmap.end())
		{
			Log::log(LOG_ERR, "can't map fid %d", *fid);
			return false;
		}
		else if (fit->second == 0)
		{
			Log::log(LOG_ERR, "map fid %d to 0", *fid);
			return false;
		}
		else
		{
			LOG_TRACE("map fid from %d to %d", *fid, fit->second);
			*fid = fit->second;
		}
	}
	return true;
}
/*
static bool RewriteRoleid(int * roleid)
{
	if (*roleid != 0)
	{
		std::map<int, int>::const_iterator rit = roleidmap.find(*roleid);
		if (rit == roleidmap.end())
		{
			Log::log(LOG_ERR, "can't map roleid %d", *roleid);
			return false;
		}
		else if (rit->second == 0)
		{
			Log::log(LOG_ERR, "map roleid %d to 0", *roleid);
			return false;
		}
		else
		{
			LOG_TRACE("map roleid from %d to %d", *roleid, rit->second);
			*roleid = rit->second;
		}
	}
	return true;
}
*/
void FixCityStore(const char * oldcitystore, const char * roleidmapfile, const char * factionidmapfile)
{
	if (access(roleidmapfile, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", roleidmapfile);
		return;
	}
	if (access(factionidmapfile, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", factionidmapfile);
		return;
	}
	{
		std::ifstream ifs(roleidmapfile);
		string line;
		while (std::getline(ifs, line))
		{
			int oldroleid, userid, newroleid;
			oldroleid = userid = newroleid = 0;
			if (sscanf(line.c_str(), "<%d,%d>->%d", &oldroleid, &userid, &newroleid) == 3)
			{
				if (oldroleid > 0)
					roleidmap[oldroleid] = newroleid;
			}
			else
				Log::log(LOG_ERR, "invalid line %s", line.c_str());
		}
		LOG_TRACE("read roleidmap size %d", roleidmap.size());
	}
	{
		std::ifstream ifs(factionidmapfile);
		string line;
		while (std::getline(ifs, line))
		{
			int oldfid, newfid;
			oldfid = newfid = 0;
			//2->67148
			if (sscanf(line.c_str(), "%d->%d", &oldfid, &newfid) == 2)
			{
				if (oldfid > 0)
					factionidmap[oldfid] = newfid;
			}
			else
				Log::log(LOG_ERR, "invalid line %s", line.c_str());
		}
		LOG_TRACE("read factionidmap size %d", factionidmap.size());
	}
	TableWrapper tab_old_city(oldcitystore);
	if (!tab_old_city.Open(2500, 2000, true))
	{
		Log::log(LOG_ERR, "open %s fail", oldcitystore);
		return;
	}
	Marshal::OctetsStream key, old_value;
	key << 1;
	if (!tab_old_city.Find(key, old_value))
	{
		Log::log(LOG_ERR, "can't find key 1 of oldcitystore");
		return;
	}
	GTerritoryStore store;
	try
	{
		old_value >> store;
	}
	catch(...)
	{
		Log::log(LOG_ERR, "unmarshal old citystore error");
		return;
	}

	GTerritoryInfoVector::iterator it, ite = store.tlist.end();
	for (it = store.tlist.begin(); it != ite; ++it)
	{
		if (!RewriteFid((int *)&(it->owner)))
			Log::log(LOG_ERR, "map territory %d owner %d fail", it->id, it->owner);
		GTChallengeVector::iterator cit, cite = it->challengelist.end();
		for (cit = it->challengelist.begin(); cit != cite; ++cit)
		{
			if (!RewriteFid((int *)&(cit->factionid)))
				Log::log(LOG_ERR, "map territory %d challenger %d fail", it->id, cit->factionid);
		}
		if (!RewriteFid((int *)&(it->defender)))
			Log::log(LOG_ERR, "map territory %d defender %d fail", it->id, it->defender);
		if (!RewriteFid((int *)&(it->success_challenge.factionid)))
			Log::log(LOG_ERR, "map territory %d success_challenge %d fail", it->id, it->success_challenge.factionid);
		LOG_TRACE("territory it->id %d new owner %d", it->id, it->owner);
	}

	try
	{
		StorageEnv::Storage * pcity = StorageEnv::GetStorage("citystore");
		StorageEnv::AtomTransaction txn;
		try{
			pcity->insert(key, Marshal::OctetsStream()<<store, txn);
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
		Log::log( LOG_ERR, "fixcitystore, what=%s\n", e.what() );
	}
}

static void CheckXueqiItemOwner(const string &container, GRoleInventory &inv, int roleid)
{
	int item_type = g_itemIdMan.GetItemType(inv.id);
	std::map<int, int>::const_iterator it;
	if (item_type != -1)
	{
		int ownerid = GetOwnerID(item_type, inv.data);
		//itemower 已经是自己 不再修复
		if (ownerid > 0 && ownerid != roleid && (it = roleidmap.find(ownerid)) != roleidmap.end())
		{
			int newownerid = it->second;
			if (roleid == newownerid) //能映射成自己 则修复
			{
				SetOwnerID(item_type, inv.data, newownerid);
				Log::log(LOG_INFO, "fix ownerid,The owner's roleid of item id=%d of %s is changed from %d to %d, on role %d 's body\n", inv.id, container.c_str(), ownerid, newownerid, roleid);
			}
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
				if (ownerid > 0 && ownerid != roleid && (it = roleidmap.find(ownerid)) != roleidmap.end())
				{
					int newownerid = it->second;
					if (roleid == newownerid)
					{
						SetSpecialItemOwnerID(inv.id, item_type, inv.data, newownerid);
						LOG_TRACE("fix special ownerid, The item(id=%d of %s)'s specialownerid is changed from %d to %d, on role %d 's body\n", inv.id, container.c_str(), ownerid, newownerid, roleid);
					}
				}
			}
		}
	}
}

class RewriteItemOwnerQuery : public StorageEnv::IQuery
{
	TableWrapper & table;
	std::string	tablename;
	int process_count;
public:
	RewriteItemOwnerQuery(TableWrapper & tab, const char * name) : table(tab), tablename(name), process_count(0) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = -1;
		try
		{
			Marshal::OctetsStream(key) >> roleid;
			if (roleid < 128)
				return true;
			if (tablename == "inventory")
			{
				GRolePocket pocket;
				Marshal::OctetsStream(value) >> pocket;
				GRoleInventoryVector::iterator cit = pocket.items.begin();
				for (; cit != pocket.items.end(); ++ cit)
					CheckXueqiItemOwner(tablename, *cit, roleid);
				cit = pocket.equipment.begin();
				for (; cit != pocket.equipment.end(); ++ cit)
					CheckXueqiItemOwner(tablename, *cit, roleid);
				cit = pocket.petbadge.begin();
				for (; cit != pocket.petbadge.end(); ++ cit)
					CheckXueqiItemOwner(tablename, *cit, roleid);
				cit = pocket.petequip.begin();
				for (; cit != pocket.petequip.end(); ++ cit)
					CheckXueqiItemOwner(tablename, *cit, roleid);
				cit = pocket.fashion.begin();
				for (; cit != pocket.fashion.end(); ++ cit)
					CheckXueqiItemOwner(tablename, *cit, roleid);
				if (pocket.mountwing.size())
				{
					Marshal::OctetsStream os_mountwing(pocket.mountwing);
					GRoleInventoryVector mountwing;
					short mountwingsize = 0;
					os_mountwing >> mountwingsize;
					os_mountwing >> mountwing;
					cit = mountwing.begin();
					for (; cit != mountwing.end(); ++ cit)
						CheckXueqiItemOwner(tablename, *cit, roleid);
					os_mountwing.clear();
					os_mountwing << mountwingsize;
					os_mountwing << mountwing;
					pocket.mountwing = os_mountwing;
				}
				cit = pocket.gifts.begin();
				for (; cit != pocket.gifts.end(); ++ cit)
					CheckXueqiItemOwner(tablename, *cit, roleid);
				table.Put(key, Marshal::OctetsStream()<<pocket);
			}
			else if (tablename == "storehouse")
			{
				GRoleStorehouse storehouse;
				Marshal::OctetsStream(value) >> storehouse;
				GRoleInventoryVector::iterator cit = storehouse.items.begin();
				for (; cit != storehouse.items.end(); ++ cit) 
					CheckXueqiItemOwner(tablename, *cit, roleid);
				cit = storehouse.items2.begin();
				for (; cit != storehouse.items2.end(); ++ cit) 
					CheckXueqiItemOwner(tablename, *cit, roleid);
				cit = storehouse.fuwen.begin();
				for (; cit != storehouse.fuwen.end(); ++ cit) 
					CheckXueqiItemOwner(tablename, *cit, roleid);

				table.Put(key, Marshal::OctetsStream()<<storehouse);
			}
			else
			{
				Log::log(LOG_ERR, "invalid table name %s", tablename.c_str());
				return false;
			}
		}
		catch ( DbException e ) { throw; }
		catch ( ... )
		{
			Log::log(LOG_INFO, "\tERROR: role data error, roleid=%d", roleid );
		}
		if (++process_count%131072 == 0)
			table.Checkpoint();
		return true;
	}
};

void RewriteItemOwner(const char * new_table_name, const char * tablename )
{       
	TableWrapper new_table(new_table_name);
	if (!new_table.Open(25000, 20000, true))
	{
		Log::log(LOG_ERR, "open %s fail", new_table_name);
		return;
	}
	LOG_TRACE( "Rewrite %s:", tablename );
	PreLoadTable((StorageEnv::get_datadir()+"/"+tablename).c_str());

	RewriteItemOwnerQuery q(new_table, tablename);
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage(tablename);
		StorageEnv::AtomTransaction     txn;
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "RewriteItemOwner, error when walk, what=%s\n", e.what() );
	}
}
void FixXueqiItemOwner(const char * roleidmapfile)
{
	if (access(roleidmapfile, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", roleidmapfile);
		return;
	}
	if (!g_itemIdMan.LoadItemId())
	{
		Log::log(LOG_ERR, "Failed to read item id from 'itemid.txt'\n");
		return;
	}
	std::string inventory_file("./inventory.rewrite");
	if (access(inventory_file.c_str(), R_OK) == 0)
	{
		Log::log(LOG_ERR, "%s already exist", inventory_file.c_str());
		return;
	}
	std::string storehouse_file("./storehouse.rewrite");
	if (access(storehouse_file.c_str(), R_OK) == 0)
	{
		Log::log(LOG_ERR, "%s already exist", storehouse_file.c_str());
		return;
	}
	std::ifstream ifs(roleidmapfile);
	string line;
	while (std::getline(ifs, line))
	{
		int oldroleid, userid, newroleid;
		oldroleid = userid = newroleid = 0;
		if (sscanf(line.c_str(), "<%d,%d>->%d", &oldroleid, &userid, &newroleid) == 3)
		{
			if (oldroleid > 0)
				roleidmap[oldroleid] = newroleid;
		}
		else
			Log::log(LOG_ERR, "invalid line %s", line.c_str());
	}
	LOG_TRACE("read roleidmap size %d", roleidmap.size());
	RewriteItemOwner(inventory_file.c_str(), "inventory");
	RewriteItemOwner(storehouse_file.c_str(), "storehouse");
}

// 一个账号下的角色要么全部删除 要么全部保留
typedef std::map<int/*userid*/, std::pair<int/*logicuid*/, int/*rolelist*/> >InactiveUsers;
InactiveUsers inactive_users;
struct InactiveRole
{
	int userid;
	int login_time;
	int level;
	InactiveRole(int _u = 0, int _login = 0, int _lev = 0) : userid(_u), login_time(_login), level(_lev) {}
};
typedef std::map<int/*roleid*/, InactiveRole> InactiveRoles;
InactiveRoles inactive_roles;

class DelWalkUserQuery : public StorageEnv::IQuery
{
public:
	bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int userid = -1;
		try
		{
			Marshal::OctetsStream(key) >> userid;
			User user;
			Marshal::OctetsStream(value) >> user;
			if (user.cash_add == 0 && user.add_serial == 0 && userid > 1024 && user.logicuid > 1024
					&& RoleList(user.rolelist).GetRoleCount() > 0)
				inactive_users.insert(std::make_pair(userid, std::make_pair(user.logicuid, user.rolelist)));
		}
		catch (...)
		{
			Log::log(LOG_ERR, "DelWalkUserQuery, unmarshal exception userid %d", userid);
		}
		return true;
	}
};

class DelWalkBaseQuery : public StorageEnv::IQuery
{
#define DEF_INACTIVE_DAYS 	365
	int cold_time;
	time_t now;
public:
	DelWalkBaseQuery(int d)
	{
		cold_time = (d > DEF_INACTIVE_DAYS ? d : DEF_INACTIVE_DAYS)*24*3600;
		now = time(NULL);
		LOG_TRACE("DelWalkBaseQuery cold_time=%d now=%d", cold_time, now);
	}
	bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = -1;
		try
		{
			Marshal::OctetsStream(key) >> roleid;
			GRoleBase base;
			Marshal::OctetsStream(value) >> base;
			int userid = (base.userid==0 ? LOGICUID(base.id) : base.userid);
			if (inactive_users.find(userid) == inactive_users.end())
				return true;
			if (now - base.lastlogin_time < cold_time)
			{
				inactive_users.erase(userid);
				return true;
			}
			if (base.status != _ROLE_STATUS_NORMAL)	
			{
				inactive_users.erase(userid);
				return true;
			}
			if (base.title!=TITLE_FREEMAN && base.title && base.familyid)
			{
				inactive_users.erase(userid);
				return true;
			}
			if (base.spouse || base.sectid)
			{
				inactive_users.erase(userid);
				return true;
			}
			inactive_roles.insert(std::make_pair(roleid, InactiveRole(userid, base.lastlogin_time, 0)));
		}
		catch (...)
		{
			Log::log(LOG_ERR, "DelWalkBaseQuery, unmarshal exception roleid %d", roleid);
		}
		return true;
	}
};

class DelWalkStatusQuery : public StorageEnv::IQuery
{
public:
	bool Update(StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = -1;
		try
		{
			Marshal::OctetsStream(key) >> roleid;
			GRoleStatus status;
			Marshal::OctetsStream(value) >> status;
			InactiveRoles::iterator it = inactive_roles.find(roleid);
			if (it == inactive_roles.end())
				return true;
			int true_level = status.level+status.reborndata.size()/sizeof(int)*200;
			if (true_level < 30)
				it->second.level = true_level;
			else
			{
				inactive_users.erase(it->second.userid);
				inactive_roles.erase(roleid);
			}
		}
		catch (...)
		{
			Log::log(LOG_ERR, "DelWalkStatuQuery, unmarshal exception roleid %d", roleid);
		}
		return true;
	}
};

//获取包裹里面的重要物品列表
void GetCashItems(const GRolePocket & pocket, std::map<int, int> & out)
{
	GRoleInventoryVector::const_iterator cit = pocket.items.begin();
	for (; cit != pocket.items.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	cit = pocket.equipment.begin();
	for (; cit != pocket.equipment.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	cit = pocket.petbadge.begin();
	for (; cit != pocket.petbadge.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	cit = pocket.petequip.begin();
	for (; cit != pocket.petequip.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	cit = pocket.fashion.begin();
	for (; cit != pocket.fashion.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	GPocketInventoryVector::const_iterator cpit = pocket.pocket_items.begin();
	for (; cpit != pocket.pocket_items.end(); ++ cpit)
	{
		if(DBClearConsumable::IsConsumable(cpit->id))
			out[cpit->id] += cpit->count;
	}

	if (pocket.mountwing.size())
	{
		Marshal::OctetsStream os_mountwing(pocket.mountwing);
		GRoleInventoryVector mountwing;
		short mountwingsize = 0;
		try
		{
			os_mountwing >> mountwingsize;
			os_mountwing >> mountwing;
		}
		catch (...)
		{
		}
		cit = mountwing.begin();
		for (; cit != mountwing.end(); ++ cit)
		{
			if(DBClearConsumable::IsConsumable(cit->id))
				out[cit->id] += cit->count;
		}
	}

	cit = pocket.gifts.begin();
	for (; cit != pocket.gifts.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}
}

void GetCashItems(const GRoleStorehouse & storehouse, std::map<int, int> & out)
{
	GRoleInventoryVector::const_iterator cit = storehouse.items.begin();
	for (; cit != storehouse.items.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	cit = storehouse.items2.begin();
	for (; cit != storehouse.items2.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}

	cit = storehouse.fuwen.begin();
	for (; cit != storehouse.fuwen.end(); ++ cit)
	{
		if(DBClearConsumable::IsConsumable(cit->id))
			out[cit->id] += cit->count;
	}
}

void DelInactiveUsers(const char * _dbdata, int day)
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
	TableWrapper tab_status((dbdata + "/status").c_str());
	TableWrapper tab_waitdel((dbdata + "/waitdel").c_str());
	TableWrapper tab_inventory((dbdata + "/inventory").c_str());
	TableWrapper tab_storehouse((dbdata + "/storehouse").c_str());

	if (!tab_user.Open(cache_high, cache_low, true) || !tab_base.Open(cache_high, cache_low, true) || !tab_status.Open(cache_high, cache_low, true) 
			|| !tab_config.Open(cache_high, cache_low, true) || !tab_waitdel.Open(cache_high, cache_low, true) 
			|| !tab_inventory.Open(cache_high, cache_low, true) || !tab_storehouse.Open(cache_high, cache_low, true))
	{
		Log::log(LOG_ERR, "Error: Open DB Failed");
		return;
	}
	if (GetDBType(tab_config) != 0)
	{
		Log::log(LOG_ERR, "Error: Invalid DB type");	
		return;
	}

	try
	{
		DelWalkUserQuery q;
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
		Log::log( LOG_ERR, "DelWalkUser, what=%s\n", e.what() );
		return;
	}
	LOG_TRACE("collect inactive %d users", inactive_users.size());

	try
	{
		DelWalkBaseQuery q(day);
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
		Log::log( LOG_ERR, "DelWalkBase, what=%s\n", e.what() );
		return;
	}
	LOG_TRACE("after base filter, inactive users %d, inactive roles %d", inactive_users.size(), inactive_roles.size());

	try
	{
		DelWalkStatusQuery q;
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
		Log::log( LOG_ERR, "DelWalkStatus, what=%s\n", e.what() );
		return;
	}
	LOG_TRACE("after status filter, inactive users %d, inactive roles %d", inactive_users.size(), inactive_roles.size());

	InactiveRoles::iterator it, ite = inactive_roles.end();
	for (it = inactive_roles.begin(); it != ite; )
	{
		if (inactive_users.find(it->second.userid) == inactive_users.end())
			inactive_roles.erase(it++);
		else
			++it;
	}
	LOG_TRACE("after re filter, inactive users %d, inactive roles %d", inactive_users.size(), inactive_roles.size());

	{
	//数据验证
		InactiveRoles::iterator it, ite = inactive_roles.end();
		for (it = inactive_roles.begin(); it != ite; ++it)
		{
			if (inactive_users.find(it->second.userid)==inactive_users.end())
				Log::log(LOG_ERR, "can not find user %d for role %d", it->second.userid, it->first);
		}
	}
	{
		InactiveUsers::iterator it, ite = inactive_users.end();
		for (it = inactive_users.begin(); it != ite; ++it)
		{
			RoleList rolelist(it->second.second);
			int role = -1;
			while ((role=rolelist.GetNextRole()) != -1)
			{
				int roleid = it->second.first + role;
				if (inactive_roles.find(roleid) == inactive_roles.end())
					Log::log(LOG_ERR, "can not find role %d for user %d", roleid, it->first);
			}
		}
	}

	time_t now = time(NULL);
	it = inactive_roles.begin();
	bool finished = false;
	int del_success_count = 0;
	std::map<int, int> total_items;
	while (!finished)
	{
		try
		{
			int i = 0;
			for (; it != ite && i < checkpoint_threshold; ++it,++i)
			{
				Marshal::OctetsStream os_key, os_base, os_inventory, os_store;
				os_key << it->first;
				GRoleBase base;
				if (tab_base.Find(os_key, os_base))
				{
					os_base >> base;
					base.delete_time = now-GameDBManager::GetInstance()->GetDeleteTimeout();
					base.status = _ROLE_STATUS_READYDEL;
					tab_base.Put(os_key, Marshal::OctetsStream() << base);

					Marshal::OctetsStream delkey;
					delkey << WaitDelKey(base.id, TYPE_ROLE);
					tab_waitdel.Put(delkey, Marshal::OctetsStream() << base.delete_time);
					del_success_count++;
					LOG_TRACE("del role %d, userid %d lastlog_time %d true_level %d", it->first, it->second.userid, it->second.login_time, it->second.level);
				}
				else
					Log::log(LOG_ERR, "can't find del rolebase %d", it->first);

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
					GetCashItems(pocket, total_items);
					//直接清空包裹
					tab_inventory.Put(os_key, Marshal::OctetsStream()<<GRolePocket(24));
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
					GetCashItems(store, total_items);
					//直接诶清空仓库
					tab_storehouse.Put(os_key, Marshal::OctetsStream()<<GRoleStorehouse(24));
				}
			}
			if (it == ite)
				finished = true;
		}
		catch (...)
		{
			Log::log(LOG_ERR, "del role, exception roleid %d", it->first);
			++it;
		}
		tab_base.Checkpoint();
		tab_waitdel.Checkpoint();
		tab_inventory.Checkpoint();
		tab_storehouse.Checkpoint();
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
	//删除的角色都至少是 365 天未登录的
	LOG_TRACE("Del360RoleItemsTotal items:%s", total_str.c_str());
}

class StockCancelDBQuery : public StorageEnv::IQuery
{
	StorageEnv::Storage * puser;
	std::set<int>& userid_map;
public:
	std::vector<int> order_tid_del;//准备删除的tid列表 
public:
	StockCancelDBQuery(StorageEnv::Storage* puser, std::set<int>& userid_map) : puser(puser), userid_map(userid_map){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream keyt, os_old, keyu, valueu;
		keyt = key;
		os_old = value;
		StockOrder order;
		User user;
		try
		{
			try
			{
				os_old >> order;

				int tid = 0;
				keyt >> tid;
				Log::log( LOG_INFO, "StockCancelDBQuery order userid=%d, order tid=%d, tid=%d, order price=%d, order volume=%d", order.userid, order.tid, tid, order.price, order.volume);	

				if(userid_map.find(order.userid) == userid_map.end())
				{
					return true;
				}
				
				keyu << order.userid;
				if(!puser->find(keyu,valueu,txn))
				{
					Log::log( LOG_INFO, "StockCancelDBQuery, puser not find userid=%d", order.userid );
					return true;
				}
				valueu >> user;
				//Log::log( LOG_INFO, "StockCancelDBQuery begin, userid=%d, cash=%d, money=%d", order.userid, user.cash, user.money );

				if(order.price>0)// 卖出元宝
				{
					//user.cash = 0; 这里的user.cash注释掉，放在后面统一处理比较好
				}
				else// 买入元宝
				{
					// 注释掉，则表示需要返还手续费，挂单时money账户没有改动
					//user.money += (int)((STOCK_TAX_RATE-1)*order.price*order.volume);
				}
				if(user.cash<0 || user.money<0)
				{
					Log::log( LOG_INFO, "StockCancelDBQuery, cash or money < 0 , cash=%d, money=%d", user.cash, user.money );
					throw DbException(DB_VERIFY_BAD);
				}
				
				int final_cash = user.cash_add+user.cash_buy-user.cash_sell-user.cash-user.cash_used;	
				Log::log( LOG_INFO, "StockCancelDBQuery end, userid=%d, cash=%d, money=%d, total cash=%d", order.userid, user.cash, user.money, final_cash );

				puser->insert(keyu,Marshal::OctetsStream()<<user,txn);
				order_tid_del.push_back(tid);
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
			Log::log( LOG_ERR, "StockCancelDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};

class StockCancelUserIDQuery : public StorageEnv::IQuery
{
	std::set<int>& userid_map;
public:
	std::map<int, User> userid_update;
public:
	StockCancelUserIDQuery(std::set<int>& userid_map) : userid_map(userid_map){}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		User user;
		int userid;
		try
		{
			try
			{
				Marshal::OctetsStream(key) >> userid;
				Marshal::OctetsStream(value) >> user;
				//Log::log( LOG_INFO, "StockCancelUserIDQuery, userid=%d, user.cash_add=%d,user.cash_buy=%d,user.cash_sell=%d,user.cash=%d", userid, user.cash_add,user.cash_buy,user.cash_sell,user.cash );
				
				if(userid_map.find(userid) == userid_map.end())
				{
					return true;
				}

				if(user.cash>=0)
				{
					// 等于0的情况，主要是为了后边的打印
					user.cash = 0;
				}
				else
				{
					Log::log(LOG_ERR, "StockCancelUserIDQuery, userid=%d, cash=%d < 0", userid, user.cash);
					return true;
				}

				userid_update[userid] = user;
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
			Log::log( LOG_ERR, "StockCancelUserIDQuery, what=%s\n", e.what() );
		}

		return true;
	}
};


bool ImportStockCancelUserId( const char* filename, std::set<int>& userid_map )
{
	FILE* fp = fopen(filename, "r");
	if(fp)
	{
		char buf[4096];
		while(fgets(buf, sizeof(buf), fp))
		{
			if(buf[0]=='#')
				continue;
			int userid;
			userid = atoi(buf);
			userid_map.insert(userid);
		}

		fclose(fp);
		return true;
	}
	else
	{
		return false;
	}
}


void RepairStockCancel(const char* filename)
{      
	if (access(filename, R_OK) != 0)
	{
		Log::log(LOG_ERR, "Can't access %s\n", filename);
		return;
	}

	std::set<int> userid_map;
	ImportStockCancelUserId(filename, userid_map);
	
	Log::log( LOG_INFO, "RepairStockCancel userid_map size=%d", userid_map.size());


	{
		try
		{
			StorageEnv::Storage * porder = StorageEnv::GetStorage("order");
			StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
			StorageEnv::AtomTransaction     txn;

			StockCancelDBQuery q(puser, userid_map);
			StockCancelUserIDQuery qu(userid_map);
			try
			{
				StorageEnv::Storage::Cursor cursor_order = porder->cursor(txn);
				cursor_order.walk( q );

				StorageEnv::Storage::Cursor cursor_user = puser->cursor(txn);
				cursor_user.walk( qu );

				std::vector<int>::iterator it = q.order_tid_del.begin();
				for(; it != q.order_tid_del.end(); ++it)
				{
					Log::log( LOG_INFO, "RepairStockCancel order table del tid=%d", *it);
					porder->del(Marshal::OctetsStream()<<*it, txn);
				}
			
				char buf[32];
				snprintf(buf, sizeof(buf), "exportstockcancel");
				FILE *fp = fopen(buf, "w+");
				if (fp == NULL)
					fp = stderr;

				std::map<int, User>::iterator it_user = qu.userid_update.begin();
				for(; it_user != qu.userid_update.end(); ++it_user)
				{
					User user = it_user->second;
					int userid = it_user->first;
					int final_cash = user.cash_add+user.cash_buy-user.cash_sell-user.cash -user.cash_used;
					fprintf(fp, "%d\t%d\n", userid, final_cash );

					puser->insert(Marshal::OctetsStream()<<it_user->first, Marshal::OctetsStream()<<it_user->second, txn);
				}
				
				if (fp != stderr)
					fclose(fp);
				q.order_tid_del.clear();
				qu.userid_update.clear();
				Log::log( LOG_INFO, "RepairStockCancel exec ok");

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
			Log::log( LOG_ERR, "RepairStockCancel, error when walk, what=%s\n", e.what() );
		}
	}
}

class GetReferralQuery : public StorageEnv::IQuery
{
	int referrer;
public:
	GetReferralQuery(int ref) : referrer(ref) {}
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			Marshal::OctetsStream	os_key(key), os_value(value);
			int userid = -1;
			os_key >> userid;
			User user;
			os_value >> user;
			if (user.referrer == referrer)
				LOG_TRACE("%d", userid);
		}
		catch ( Marshal::Exception e )
		{
			Log::log( LOG_ERR, "GetReferralQuery, exception\n" );
			throw e;
		}
		return true;
	}
};


void GetReferrals(int roleid)
{
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage("user");
		GetReferralQuery q(roleid);
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
	catch ( DbException e )
	{
		Log::log( LOG_ERR, "GetReferrals, error when walk what=%s\n", e.what() );
	}
}


void GetReferrer(int zoneid, const char * roleidfile)
{
	if (zoneid <= 0)
	{
		Log::log(LOG_ERR, "invalid zoneid %d", zoneid);
		return;
	}
	LOG_TRACE("get self zoneid %d", zoneid);

	std::set<int> zone_roles;
	FILE * fp = fopen(roleidfile, "r");
	if (fp == NULL)
	{
		Log::log(LOG_ERR, "can not open roleidfile %s", roleidfile);
		return;
	}
	char buf[4096];
	while (fgets(buf, sizeof(buf), fp))
	{
		int zone = 0, role = 0;
		int ret = sscanf(buf, "%d %d", &zone, &role);
		if (ret != 2)
		{
			LOG_TRACE("invalid line %s", buf);
			continue;
		}
		if (zone == zoneid)
			zone_roles.insert(role);
	}
	fclose(fp);
	LOG_TRACE("get zone roles %d", zone_roles.size());

	LOG_TRACE("zoneid roleid level reborn referrer_roleid referrer_bonus_withdraw referrer_userid referrer_user_cashadd");
	try
	{
		StorageEnv::Storage * puser = StorageEnv::GetStorage("user");
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::CommonTransaction txn;

		try
		{
			std::set<int>::const_iterator it, ite = zone_roles.end();
			for (it = zone_roles.begin(); it != ite; ++it)
			{
				Marshal::OctetsStream krole, kuser, os_base, os_status, os_user, os_referrer_base, os_referrer_base2, os_referrer_user;
				krole << *it;
				if (!pbase->find(krole, os_base, txn))
				{
					Log::log(LOG_ERR, "can not find base %d", *it);
					continue;
				}
				if (!pstatus->find(krole, os_status, txn))
				{
					Log::log(LOG_ERR, "can not find status %d", *it);
					continue;
				}
				GRoleBase base;
				GRoleStatus status;
				try
				{
					os_base >> base;
					os_status >> status;
				}
				catch (...)
				{
					Log::log(LOG_ERR, "unmarshal role data error %d", *it);
					continue;
				}
				int userid = (0==base.userid ? (LOGICUID(base.id)) : base.userid);
				kuser << userid;
				if (!puser->find(kuser, os_user, txn))
				{
					Log::log(LOG_ERR, "can not find user %d for role %d", userid, *it);
					continue;
				}
				User user;
				try
				{
					os_user >> user;
				}
				catch (...)
				{
					Log::log(LOG_ERR, "unmarshal user error %d for role %d", userid, *it);
					continue;
				}
				int referrer_roleid = user.referrer;
				GRoleBase referrer_base;
				GRoleBase2 referrer_base2;
				if (referrer_roleid > 0)
				{
					if (!pbase->find(Marshal::OctetsStream()<<referrer_roleid, os_referrer_base, txn))
					{
						Log::log(LOG_ERR, "can not find referrer base %d for role %d", referrer_roleid, *it);
						continue;
					}
					if (!pbase2->find(Marshal::OctetsStream()<<referrer_roleid, os_referrer_base2, txn))
					{
						Log::log(LOG_ERR, "can not find referrer base2 %d for role %d", referrer_roleid, *it);
						continue;
					}
					try
					{
						os_referrer_base >> referrer_base;
						os_referrer_base2 >> referrer_base2;
					}
					catch (...)
					{
						Log::log(LOG_ERR, "unmarshal referrer data error %d for role %d", referrer_roleid, *it);
						continue;
					}
				}
				int referrer_userid = (0==referrer_base.userid ? (LOGICUID(referrer_base.id)) : referrer_base.userid);
				User referrer_user;
				if (referrer_userid > 0)
				{
					if (!puser->find(Marshal::OctetsStream()<<referrer_userid, os_referrer_user, txn))
					{
						Log::log(LOG_ERR, "can not find user %d for referrer %d of role %d", referrer_userid, referrer_roleid, *it);
						continue;
					}
					try
					{
						os_referrer_user >> referrer_user;
					}
					catch (...)
					{
						Log::log(LOG_ERR, "unmarshal user error %d for referrer %d of role %d", referrer_userid, referrer_roleid, *it);
						continue;
					}
				}
				LOG_TRACE("%d %d %d %d %d %d %d %d", zoneid, *it, status.level, status.reborndata.size()/sizeof(int), referrer_roleid,
						referrer_base2.bonus_withdraw, referrer_userid, referrer_user.cash_add);
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
		Log::log( LOG_ERR, "Exception, what=%s\n", e.what() );
		return;
	}

}

class RewriteBase2Query : public StorageEnv::IQuery
{
public:
	std::map<int, GRoleBase2> base2_map;
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = -1;
		try
		{
			Marshal::OctetsStream(key) >> roleid;
			Octets value_bk = value;
			try
			{
				GRoleBase2 base2;
				Marshal::OctetsStream(value) >> base2;
			}
			catch(...)
			{
				LOG_TRACE("role %d base2 unmarshal error, prepare to rewrite", roleid);
				GRoleBase2old base2_old;
				try
				{
					Marshal::OctetsStream(value_bk) >> base2_old;
				}
				catch(...)
				{
					Log::log(LOG_ERR, "role %d base2-old marshal error ???!!!", roleid);
					return true;
				}
				GRoleBase2 base2_rewrite;
				base2_rewrite.id = base2_old.id;
				base2_rewrite.bonus_withdraw = base2_old.bonus_withdraw;
				base2_rewrite.bonus_reward = base2_old.bonus_reward;
				base2_rewrite.bonus_used = base2_old.bonus_used;
				base2_rewrite.exp_withdraw_today = base2_old.exp_withdraw_today;
				base2_rewrite.exp_withdraw_time = base2_old.exp_withdraw_time;
				base2_rewrite.composkills = base2_old.composkills;
				base2_rewrite.tower_raid = base2_old.tower_raid;
				base2_rewrite.deity_level = base2_old.deity_level;
				base2_rewrite.data_timestamp = base2_old.data_timestamp;
				base2_rewrite.src_zoneid = base2_old.src_zoneid;
				base2_rewrite.deity_exp = base2_old.deity_exp;
				base2_rewrite.dp = base2_old.dp;
				base2_rewrite.littlepet = base2_old.littlepet;
				base2_rewrite.flag_mask = base2_old.flag_mask;
				base2_rewrite.ui_transfer = base2_old.ui_transfer;
				base2_rewrite.collision_info = base2_old.collision_info;
				base2_rewrite.runescore = base2_old.runescore;
				base2_rewrite.comsumption = base2_old.comsumption;
				base2_rewrite.astrology_info = base2_old.astrology_info;
				base2_rewrite.liveness_info = base2_old.liveness_info;
				base2_rewrite.sale_promotion_info = base2_old.sale_promotion_info;
				base2_rewrite.propadd = base2_old.propadd;
				base2_rewrite.multi_exp = base2_old.multi_exp;
				base2_rewrite.fuwen_info = base2_old.fuwen_info;
				base2_rewrite.datagroup = base2_old.datagroup;
				base2_rewrite.phase = base2_old.phase;
				base2_rewrite.award_info_6v6 = base2_old.award_info_6v6;

				base2_map[roleid] = base2_rewrite;
			}
		}
		catch (...)
		{
			Log::log( LOG_ERR, "RewriteBase2Query, roleid=%d", roleid);
		}
		return true;
	}
};

void RewriteBase2()
{
	RewriteBase2Query q;
	try
	{
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
		StorageEnv::AtomTransaction     txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pbase2->cursor(txn);
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
		Log::log( LOG_ERR, "RewriteBase2, error when walk, what=%s\n", e.what() );
	}

	LOG_TRACE("rewrite base2 map size %d", q.base2_map.size());

	try
	{
		StorageEnv::Storage * pbase2 = StorageEnv::GetStorage("base2");
		StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
		StorageEnv::CommonTransaction txn;
		try
		{
			std::map<int, GRoleBase2>::const_iterator it, ite = q.base2_map.end();
			for (it = q.base2_map.begin(); it != ite; ++it)
			{
				pbase2->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<it->second, txn);
				LOG_TRACE("RewriteBase2 roleid %d", it->first);

				GRoleBase base;
				Marshal::OctetsStream(pbase->find(Marshal::OctetsStream()<<it->first, txn)) >> base;
				if (base.status == _ROLE_STATUS_FROZEN)
				{
					base.status = _ROLE_STATUS_NORMAL;
					pbase->insert(Marshal::OctetsStream()<<it->first, Marshal::OctetsStream()<<base, txn);
					LOG_TRACE("modify roleid %d status to %d", it->first, base.status);
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
		Log::log( LOG_ERR, "Exception, what=%s\n", e.what() );
		return;
	}
}

class CheckAbnormalRoleQuery : public StorageEnv::IQuery
{
public:
	std::map<int, GRoleBase2> base2_map;
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid = -1;
		try
		{
			Marshal::OctetsStream(key) >> roleid;
			Octets value_bk = value;
			try
			{
				int error_flag = 0;

				GRoleStatus role_status;
				Marshal::OctetsStream(value) >> role_status;

				if(role_status.id <= 0) 
				{
					error_flag = 1;
				}
				else if(role_status.occupation >= 200)
				{
					error_flag = 2;
				}
				else if(role_status.level <= 0 || role_status.level > 160)
				{
					error_flag = 3;
				}
				else if(role_status.pp < 0 || role_status.hp < 0 || role_status.mp < 0)
				{
					error_flag = 4;
				}
				else if(role_status.worldtag < 0 || role_status.worldtag > 1000)
				{
					error_flag = 5;
				}
				else if(role_status.pkvalue < 0 || role_status.reputation < 0 || role_status.produceskill < 0 || role_status.talismanscore < 0)
				{
					error_flag = 6;
				}

				if(error_flag)
				{
					LOG_TRACE("roleid=%d, error=%d", roleid, error_flag);
				}
			}
			catch(...)
			{
				Log::log( LOG_ERR, "CheckAbnormalRoleQuery error, roleid=%d", roleid);
			}
		}
		catch ( DbException e )
		{
			Log::log( LOG_ERR, "Exception, what=%s\n", e.what() );
			return true;
		}
		return true;
	}
};


void CheckAbnormalRole()
{
	CheckAbnormalRoleQuery q;
	try
	{
		StorageEnv::Storage * pstatus = StorageEnv::GetStorage("status");
		StorageEnv::AtomTransaction     txn;
		try
		{
			StorageEnv::Storage::Cursor cursor = pstatus->cursor(txn);
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
		Log::log( LOG_ERR, "CheckAbnormalRole, error when walk, what=%s\n", e.what() );
	}
}

// Youshuang add
static void split(const char * line, std::vector<std::string> & sub_strings, char div = ',')
{
	int len = strlen(line);
	char * s = const_cast<char*>(line);
	while (len > 0)
	{
		int i = 0;
		for (; i < len && s[i] != div; ++i) {}

		if (i > 0)
		{
			sub_strings.push_back(std::string(s, i));
		}

		++i;
		len -= i;
		s += i;
	}
}

void RepairItemOwner_FromFile( const char* badfile, int zoneid )
{
	g_itemIdMan.LoadItemId("consignroleitemid.txt");

	std::ifstream ifs( badfile );
	std::string line;
	while( std::getline(ifs, line) )
	{
		if( line.size() == 0 )
		{
			continue;
		}
		std::vector<std::string> subs;
		split( line.c_str(), subs );
		if( subs.size() < 3 ){ continue; }
		int idx = 0;
		int role_in_zone = atoi( subs[idx++].c_str() );
		int old_roleid = atoi( subs[idx++].c_str() );
		int new_roleid = atoi( subs[idx++].c_str() );
			
		if( zoneid != role_in_zone )
		{
			continue;
		}
		RepairItemOwner( old_roleid, new_roleid );
		StorageEnv::checkpoint();
	}
	
}

void RepairItemOwner( int old_roleid, int new_roleid )
{
	Log::log(LOG_INFO, "\n===============repair item owner, change old user id %d to new user id %d=====================\n", old_roleid, new_roleid);
	
	GRoleDetail	detail;
	Marshal::OctetsStream key;
	Marshal::OctetsStream value_inventory, value_equipment, value_storehouse;
	StorageEnv::AtomTransaction     txn;
	try
	{
		StorageEnv::Storage * pinventory = StorageEnv::GetStorage("inventory");
		StorageEnv::Storage * pstorehouse = StorageEnv::GetStorage("storehouse");
		try
		{
			key << new_roleid;

			if( pinventory->find( key, value_inventory, txn ) )
				value_inventory >> detail.inventory;

			if( pstorehouse->find( key, value_storehouse, txn ) )
				value_storehouse >> detail.storehouse;

			CheckItemOwner( "inventory", old_roleid, new_roleid, detail.inventory.items );
			CheckItemOwner( "inventory", old_roleid, new_roleid, detail.inventory.equipment );
			CheckItemOwner( "storehouse", old_roleid, new_roleid, detail.storehouse.items );

			pinventory->insert( key, Marshal::OctetsStream() << detail.inventory, txn );
			pstorehouse->insert( key, Marshal::OctetsStream() << detail.storehouse, txn );
			return;
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
		Log::log( LOG_ERR, "RepairItemOwner, roleid=%d, what=%s\n", new_roleid, e.what() );
	}
}
// end
};
