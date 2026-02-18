#include <sys/types.h>
#include <dirent.h>

#include <map>
#include <fstream>

#include "rpcdefs.h"

#include "macros.h"
#include "accessdb.h"
#include "storage.h"
#include "conv_charset.h"
#include "parsestring.h"

#include "streamcompress.h"
#include "user"

#include "timer.h"
#include "uniquenameserver.hpp"
#include "storagetool.h"
#include "precreaterole.hrp"

namespace GNET
{
typedef std::map<int, int> tUserIdExchangeMap;
static tUserIdExchangeMap g_UserIdExchangeTable;

inline void PreLoadTable(const char* path)
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


bool ShowInfo( )
{
	printf( "ShowInfo Begin\n" );

	try
	{
		StorageEnv::Storage * plogicuid = StorageEnv::GetStorage("logicuid");
		StorageEnv::Storage * punamefaction = StorageEnv::GetStorage("unamefaction");
		StorageEnv::Storage * punamefamily = StorageEnv::GetStorage("unamefamily");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_unamefaction, value_unamefamily, value_logicuid;

			int temp = 0;
			Marshal::OctetsStream t;
			t << temp;

			if( plogicuid->find( t, value_logicuid, txn ) )
			{
				int logicuid;
				value_logicuid >> logicuid;
				printf( "logicuid nextid record: logicuid = %d\n", logicuid );
			}
			else
			{
				printf( "logicuid nextid not found.\n" );
			}

			if( punamefaction->find( t, value_unamefaction, txn ) )
			{
				int zoneid, factionid, status, time;
				value_unamefaction >> zoneid >> factionid >> status >> time;
				printf( "faction nextid record: zoneid = %d, factionid = %d, status=%d, time=%d\n",
						zoneid, factionid, status, time );
			}
			else
			{
				printf( "faction nextid not found.\n" );
			}

			if( punamefamily->find( t, value_unamefamily, txn ) )
			{
				int zoneid, familyid, status, time;
				value_unamefamily >> zoneid >> familyid >> status >> time;
				printf( "family nextid record: zoneid = %d, familyid = %d, status=%d, time=%d\n",
						zoneid, familyid, status, time );
			}
			else
			{
				printf( "family nextid not found.\n" );
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
		Log::log( LOG_ERR, "ShowInfo, what=%s\n", e.what() );
	}

	return false;
}

bool SetLogicuidNextid( int nextid )
{
	printf( "SetLogicuidNextid Begin. nextid = %d\n", nextid );

	try
	{
		StorageEnv::Storage * plogicuid = StorageEnv::GetStorage("logicuid");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_logicuid, value_nextid;

			int temp = 0;
			Marshal::OctetsStream t;
			t << temp;

			if( plogicuid->find( t, value_logicuid, txn ) )
			{
				int logicuid;
				value_logicuid >> logicuid;
				printf( "old nextid record: logicuid = %d\n", logicuid );
			}
			else
			{
				printf( "old nextid not found.\n" );
			}

			value_nextid << nextid;
			plogicuid->insert( t, value_nextid, txn );
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
		Log::log( LOG_ERR, "SetLogicuidNextid, what=%s\n", e.what() );
	}

	return false;
}

bool SetFactionNextid( int nextid )
{
	printf( "SetFactionNextid Begin. nextid = %d\n", nextid );

	try
	{
		StorageEnv::Storage * punamefaction = StorageEnv::GetStorage("unamefaction");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_unamefaction, value_nextid;

			int temp = 0;
			Marshal::OctetsStream t;
			t << temp;

			if( punamefaction->find( t, value_unamefaction, txn ) )
			{
				int zoneid, factionid, status, time;
				value_unamefaction >> zoneid >> factionid >> status >> time;
				printf( "old nextid record: zoneid = %d, factionid = %d, status=%d, time=%d\n",
						zoneid, factionid, status, time );
			}
			else
			{
				printf( "old nextid not found.\n" );
			}

			int status = UNIQUENAME_USED;
			value_nextid << temp << nextid << status << temp;
			punamefaction->insert( t, value_nextid, txn );
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
		Log::log( LOG_ERR, "SetFactionNextid, what=%s\n", e.what() );
	}

	return false;
}

bool SetFamilyNextid( int nextid )
{
	printf( "SetFamilyNextid Begin. nextid = %d\n", nextid );

	try
	{
		StorageEnv::Storage * punamefamily = StorageEnv::GetStorage("unamefamily");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_unamefamily, value_nextid;

			int temp = 0;
			Marshal::OctetsStream t;
			t << temp;

			if( punamefamily->find( t, value_unamefamily, txn ) )
			{
				int zoneid, familyid, status, time;
				value_unamefamily >> zoneid >> familyid >> status >> time;
				printf( "old nextid record: zoneid = %d, familyid = %d, status=%d, time=%d\n",
						zoneid, familyid, status, time );
			}
			else
			{
				printf( "old nextid not found.\n" );
			}

			int status = UNIQUENAME_USED;
			value_nextid << temp << nextid << status << temp;
			punamefamily->insert( t, value_nextid, txn );
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
		Log::log( LOG_ERR, "SetFamilyNextid, what=%s\n", e.what() );
	}

	return false;
}

bool QueryUser( int userid )
{
	printf( "QueryUser Begin\n" );

	try
	{
		StorageEnv::Storage * puidrole = StorageEnv::GetStorage("uidrole");
		StorageEnv::Storage * plogicuid = StorageEnv::GetStorage("logicuid");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream key, value_user, value_logicuid;
			key << userid;

			int logicuid = 0;
			if( puidrole->find( key, value_user, txn ) )
			{
				unsigned int rolelist;
				try {
					value_user >> rolelist >> logicuid;
				} catch ( Marshal::Exception & )	
				{	
					logicuid = -1;	
				}
				printf( "found:userid = %d, user rolelist = %x, logicuid=%d.\n", userid, rolelist, logicuid);
			}
			else
			{
				printf( "rolelist not found for userid %d.\n", userid );
			}

			if( plogicuid->find( Marshal::OctetsStream()<<logicuid, value_logicuid, txn ) )
			{
				unsigned int userid;
				value_logicuid >> userid;
				printf( "found:userid = %d, logicuid = %d.\n", userid, logicuid );
			}
			else
			{
				printf( "userid not found for logicuid %d.\n", logicuid );
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
		Log::log( LOG_ERR, "QueryUser, userid=%d, what=%s\n", userid, e.what() );
	}
	return false;
}

bool QueryRoleByName( const char * name )
{
	printf( "QueryRoleByName Begin\n" );
	if( NULL == name || strlen(name) <= 0 )
		return false;

	Octets	namegbk(name,strlen(name));
	Octets	nameucs2;
	CharsetConverter::conv_charset_l2u( namegbk, nameucs2 );

	try
	{
		StorageEnv::Storage * punamerole = StorageEnv::GetStorage("unamerole");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_unamerole;

			if( punamerole->find( nameucs2, value_unamerole, txn ) )
			{
				int zoneid, roleid, status, time;
				value_unamerole >> zoneid >> roleid >> status >> time;
				printf( "found:rolename=%s, zoneid = %d, roleid = %d, status=%d, time=%d\n",
						name, zoneid, roleid, status, time );
			}
			else
			{
				printf( "not found for rolename=%s.\n", name );
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
		Log::log( LOG_ERR, "QueryRoleByName, rolename=%s, what=%s\n", name, e.what() );
	}

	return false;
}

bool QueryFactionByName( const char * name )
{
	printf( "QueryFactionByName Begin\n" );
	if( NULL == name || strlen(name) <= 0 )
		return false;

	Octets	namegbk(name,strlen(name));
	Octets	nameucs2;
	CharsetConverter::conv_charset_l2u( namegbk, nameucs2 );

	try
	{
		StorageEnv::Storage * punamefaction = StorageEnv::GetStorage("unamefaction");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_unamefaction;

			if( punamefaction->find( nameucs2, value_unamefaction, txn ) )
			{
				int zoneid, factionid, status, time;
				value_unamefaction >> zoneid >> factionid >> status >> time;
				printf( "found:factionname=%s, zoneid = %d, factionid = %d, status=%d, time=%d\n",
						name, zoneid, factionid, status, time );
			}
			else
			{
				printf( "not found for factionname=%s.\n", name );
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
		Log::log( LOG_ERR, "QueryFactionByName, factionname=%s, what=%s\n", name, e.what() );
	}

	return false;
}

bool QueryFamilyByName( const char * name )
{
	printf( "QueryFamilyByName Begin\n" );
	if( NULL == name || strlen(name) <= 0 )
		return false;

	Octets	namegbk(name,strlen(name));
	Octets	nameucs2;
	CharsetConverter::conv_charset_l2u( namegbk, nameucs2 );

	try
	{
		StorageEnv::Storage * punamefamily = StorageEnv::GetStorage("unamefamily");
		StorageEnv::AtomTransaction txn;
		try
		{
			Marshal::OctetsStream value_unamefamily;

			if( punamefamily->find( nameucs2, value_unamefamily, txn ) )
			{
				int zoneid, familyid, status, time;
				value_unamefamily >> zoneid >> familyid >> status >> time;
				printf( "found:familyname=%s, zoneid = %d, familyid = %d, status=%d, time=%d\n",
						name, zoneid, familyid, status, time );
			}
			else
			{
				printf( "not found for familyname=%s.\n", name );
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
		Log::log( LOG_ERR, "QueryFamilyByName, familyname=%s, what=%s\n", name, e.what() );
	}

	return false;
}

bool AddLogicuid( int userid, int logicuid )
{
	logicuid = ( ~0x0000000F & logicuid );
	try
	{
		StorageEnv::Storage * plogicuid = StorageEnv::GetStorage("logicuid");
		StorageEnv::Storage * puidrole = StorageEnv::GetStorage("uidrole");
		StorageEnv::CommonTransaction txn;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid, key_logicuid, value_logicuid, key_uidrole, value_uidrole;
			key_logicuid << logicuid;
			value_logicuid << userid;
			plogicuid->insert( key_logicuid, value_logicuid, txn );

			int t = 0;
			int nextid = LOGICUID_START + 16;
			key_nextid << t;
			if( plogicuid->find( key_nextid, value_nextid, txn ) )
			{
				value_nextid >> nextid;
			}
			if( nextid < logicuid )
			{
				nextid = logicuid;
				value_nextid.clear();
				value_nextid << nextid;
				plogicuid->insert( key_nextid, value_nextid, txn );
			}

			key_uidrole << userid;
			if( puidrole->find( key_uidrole, value_uidrole, txn ) )
			{
				unsigned int roles;
				value_uidrole >> roles;
				value_uidrole.clear();
				value_uidrole << roles << logicuid;
				puidrole->insert( key_uidrole, value_uidrole, txn );
			}
			else
			{
				RoleList	rolelist;
				value_uidrole.clear();
				value_uidrole << rolelist.GetRoleList() << logicuid;
				puidrole->insert( key_uidrole, value_uidrole, txn );
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
		Log::log( LOG_ERR, "AddUser, userid=%d, logicuid=%d, what=%s\n",
						userid, logicuid, e.what() );
	}
	return false;
}

bool AddRole( const char * name, int zoneid, int roleid, int status, int time )
{
	Octets	namegbk(name,strlen(name));
	Octets	nameucs2;
	CharsetConverter::conv_charset_l2u( namegbk, nameucs2 );

	try
	{
		StorageEnv::Storage * plogicuid = StorageEnv::GetStorage("logicuid");
		StorageEnv::Storage * punamerole = StorageEnv::GetStorage("unamerole");
		StorageEnv::Storage * puidrole = StorageEnv::GetStorage("uidrole");
		StorageEnv::CommonTransaction txn;
		try
		{
			Marshal::OctetsStream key_logicuid, key_userid, value_unamerole, value_uidrole;
			value_unamerole << zoneid << roleid << status << time;
			punamerole->insert( nameucs2, value_unamerole, txn );

			key_logicuid << (roleid & ~0x0000000F);
			if( plogicuid->find( key_logicuid, key_userid, txn ) )
			{
				RoleList rolelist;
				unsigned int roles;
				if( puidrole->find( key_userid, value_uidrole, txn ) )
				{
					value_uidrole >> roles;
					rolelist.SetRoleList( roles );
				}
				rolelist.AddRole( roleid % MAX_ROLE_COUNT );
				value_uidrole.clear();
				value_uidrole << rolelist.GetRoleList() << (roleid & ~0x0000000F);
				puidrole->insert( key_userid, value_uidrole, txn );
			}
			else
				printf( "ERROR AddRole,userid not found by logicuid,name=%s,zoneid=%d,roleid=%d,status=%d,time=%d",
						name, zoneid, roleid, status, time );
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
		Log::log( LOG_ERR, "AddRole, zoneid=%d, roleid=%d, rolename=%s, what=%s\n",
						zoneid, roleid, name, e.what() );
	}
	return false;
}

bool AddFaction( const char * name, int zoneid, int fid, int status, int time )
{
	Octets	namegbk(name,strlen(name));
	Octets	nameucs2;
	CharsetConverter::conv_charset_l2u( namegbk, nameucs2 );

	try
	{
		StorageEnv::Storage * punamefaction = StorageEnv::GetStorage("unamefaction");
		StorageEnv::CommonTransaction txn;
		try
		{
			Marshal::OctetsStream key_nextid, value_nextid, key_userid, value_unamefaction;
			value_unamefaction << zoneid << fid << status << time;
			punamefaction->insert( nameucs2, value_unamefaction, txn );

			int n = 0;
			int nextid = 0;
			key_nextid << n;
			if( punamefaction->find( key_nextid, value_nextid, txn ) )
			{
				int z, s, t;
				value_nextid >> z >> nextid >> s >> t;
			}
			if( nextid < fid )
			{
				nextid = fid;
				value_nextid.clear();
				value_nextid << n << nextid << n << n;
				punamefaction->insert( key_nextid, value_nextid, txn );
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
		Log::log( LOG_ERR, "AddFaction, zoneid=%d, fid=%d, factionname=%s, what=%s\n",
						zoneid, fid, name, e.what() );
	}
	return false;
}

bool AddFamily( const char * name, int zoneid, int fid, int status, int time )
{
	Octets	namegbk(name,strlen(name));
	Octets	nameucs2;
	CharsetConverter::conv_charset_l2u( namegbk, nameucs2 );

	try
	{
		StorageEnv::Storage * punamefamily = StorageEnv::GetStorage("unamefamily");
		StorageEnv::CommonTransaction txn;
		try
		{
			Marshal::OctetsStream key_userid, key_nextid, value_nextid, value_unamefamily;
			value_unamefamily << zoneid << fid << status << time;
			punamefamily->insert( nameucs2, value_unamefamily, txn );

			int n = 0;
			int nextid = 0;
			key_nextid << n;
			if( punamefamily->find( key_nextid, value_nextid, txn ) )
			{
				int z, s, t;
				value_nextid >> z >> nextid >> s >> t;
			}
			if( nextid < fid )
			{
				nextid = fid;
				value_nextid.clear();
				value_nextid << n << nextid << n << n;
				punamefamily->insert( key_nextid, value_nextid, txn );
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
		Log::log( LOG_ERR, "AddFamily, zoneid=%d, fid=%d, familyname=%s, what=%s\n",
						zoneid, fid, name, e.what() );
	}
	return false;
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
				if( 0 == strcmp("uidrole", destdbname.c_str()) )
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
						pstorage->insert( key_new, value_old, txn );
					}
				}
				else if( 0 == strcmp("logicuid", destdbname.c_str()) )
				{
					Marshal::OctetsStream key_os(key), value_new, value_old(value), value1;
					{
						int userid_old, userid_new;
						int logicuid;

						key_os >> logicuid;

						value_old >> userid_old; 
						
						if(-1 == (userid_new = GetUserIdFromTable(userid_old)))
						{
							return true;
						}

						value_new << userid_new;
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

	// 转换userid为新的userid，生成新的表
	ExportCsvUserIdExchange(srcpath, "uidrole", "uidrole");
	// 查看原表和新生成的表的行数是否相同
	CheckUserIdExchange("uidrole");
	

	ExportCsvUserIdExchange(srcpath, "logicuid", "logicuid");
	CheckUserIdExchange("logicuid");

	
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
					int userid;
					//printf("dest database (%s) data count %d\n", destdbname.c_str(), pstorage->count());

					key_os >> userid;
					if(pstorage->find( key, value1, txn ))
					{
						if(value != value1)
						{							
							printf("value_old != value1i,userid=%d\n",userid);
						}
					}
					else
					{
						printf("not find key,userid=%d\n",userid);
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

	CmpUserIdExchange(srcpath, "uidrole", "uidrole");
	CmpUserIdExchange(srcpath, "logicuid", "logicuid");
	
	StorageEnv::checkpoint();
	StorageEnv::Close();

	StorageEnv::Open();
	StorageEnv::checkpoint( );
	StorageEnv::removeoldlogs( );
}

class ExportCsvLogicuidQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;
		int userid = 0;
		int logicuid = 0;

		try
		{
			key_os >> logicuid;
			value_os >> userid;

			printf("%d,%d\n",userid, logicuid);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportCsvLogicuidQuery, error unmarshal, userid=%d.", userid );
			return true;
		}
		return true;
	}
};

void ExportCsvLogicuid( )
{
	printf("#userid");
	printf(",logicuid");
	printf("\n");

	ExportCsvLogicuidQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "logicuid" );
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
		Log::log( LOG_ERR, "ExportCsvLogicuid, error when walk, what=%s\n", e.what() );
	}
}

class ExportCsvRoleIdQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;
		int userid = 0;
		unsigned int roles = 0;
		int logicuid = 0;

		try
		{
			key_os >> userid;
			value_os >> roles;

			try {
				value_os >> logicuid;
			} catch ( Marshal::Exception & ) {	logicuid = 0;	}

			printf("%d,%x,%d\n",userid, roles, logicuid);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportCsvRoleIdQuery, error unmarshal, userid=%d.", userid );
			return true;
		}
		return true;
	}
};

void ExportCsvRoleId( )
{
	printf("#userid");
	printf(",rolelist");
	printf(",logicuid");
	printf("\n");

	ExportCsvRoleIdQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "uidrole" );
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
		Log::log( LOG_ERR, "ExportCsvRoleId, error when walk, what=%s\n", e.what() );
	}
}

class ExportCsvCheckUserIdExchangeQuery : public StorageEnv::IQuery
{
public:
	std::string	destdbname;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{		
		Marshal::OctetsStream key_os, value_os;
		key_os = key;
		value_os = value;
		int userid = 0;
		unsigned int roles = 0;
		int logicuid = 0;

		try
		{
			value_os >> roles;
			key_os >> userid;

			if(value_os.size()==8)
			{
				
				value_os >> logicuid;
				//printf("%d,%x,%d\n",userid, roles, logicuid);

			}
			else
			{	// 检查是否有老数据 ，老数据是指uidrole中，无logicuid的数据内容，通过大小可以区分
				printf("%d,%x,%d\n",userid, roles, userid);

				logicuid = userid;
			}

			if(userid == logicuid)
			{
				printf("userid:%d == logicuid:%d\n",userid,logicuid);
			}
			//printf("%d,%x,%d\n",userid, roles, logicuid);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportCsvCheckUserIdExchangeQuery, error unmarshal, userid=%d.", userid );
			return true;
		}
		return true;

	}	
};

class ExportCsvRoleNameQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os;
		Octets key_temp = key;
		value_os = value;

		Octets name, name2;
		CharsetConverter::conv_charset_u2l( key_temp, name );
		EscapeCSVString( name, name2 );

		try
		{
			int zoneid, roleid, status, time;
			value_os >> zoneid >> roleid >> status >> time;

			printf("%.*s,%d,%d,%d,%d\n",name2.size(),(char*)name2.begin(),zoneid, roleid, status, time);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportCsvRoleNameQuery, error unmarshal, name=%.*s.", name2.size(),(char*)name2.begin() );
			return true;
		}
		return true;
	}
};

void ExportCsvRoleName( )
{
	printf("#name");
	printf(",zoneid");
	printf(",roleid");
	printf(",status");
	printf(",time");
	printf("\n");

	ExportCsvRoleNameQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "unamerole" );
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
		Log::log( LOG_ERR, "ExportCsvRoleName, error when walk, what=%s\n", e.what() );
	}
}

class ExportCsvFactionQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os;
		Octets key_temp = key;
		value_os = value;

		Octets name, name2;
		CharsetConverter::conv_charset_u2l( key_temp, name );
		EscapeCSVString( name, name2 );

		try
		{
			int zoneid, fid, status, time;
			value_os >> zoneid >> fid >> status >> time;

			printf("%.*s,%d,%d,%d,%d\n",name2.size(),(char*)name2.begin(),zoneid, fid, status, time);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportCsvFactionQuery, error unmarshal, name=%.*s.", name2.size(),(char*)name2.begin() );
			return true;
		}
		return true;
	}
};

void ExportCsvFaction( )
{
	printf("#name");
	printf(",zoneid");
	printf(",factionid");
	printf(",status");
	printf(",time");
	printf("\n");

	ExportCsvFactionQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "unamefaction" );
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
		Log::log( LOG_ERR, "ExportCsvFaction, error when walk, what=%s\n", e.what() );
	}
}

class ExportCsvFamilyQuery : public StorageEnv::IQuery
{
public:
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		Marshal::OctetsStream value_os;
		Octets key_temp = key;
		value_os = value;

		Octets name, name2;
		CharsetConverter::conv_charset_u2l( key_temp, name );
		EscapeCSVString( name, name2 );

		try
		{
			int zoneid, fid, status, time;
			value_os >> zoneid >> fid >> status >> time;

			printf("%.*s,%d,%d,%d,%d\n",name2.size(),(char*)name2.begin(),zoneid, fid, status, time);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ExportCsvFamilyQuery, error unmarshal, name=%.*s.", name2.size(),(char*)name2.begin() );
			return true;
		}
		return true;
	}
};

void ExportCsvFamily( )
{
	printf("#name");
	printf(",zoneid");
	printf(",familyid");
	printf(",status");
	printf(",time");
	printf("\n");

	ExportCsvFamilyQuery q;
	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "unamefamily" );
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
		Log::log( LOG_ERR, "ExportCsvFamily, error when walk, what=%s\n", e.what() );
	}
}

bool ImportCsvLogicuid( const char * filename )
{
	FILE* fp = fopen(filename, "r");
	if(fp)
	{
		char buf[4096];
		while(fgets(buf, sizeof(buf), fp))
		{
			if(buf[0]=='#')
				continue;
			int userid, logicuid;
			char name[2048];
			memset(name,0,sizeof(name));

			std::string src = buf;
			std::vector<std::string> r;
			if( GNET::ParseStrings( src, r ) && r.size() >= 2 )
			{
				int len = r.size();
				userid = atoi( r[len-2].c_str() );
				logicuid = atoi( r[len-1].c_str() );
				printf( "AddLogicuid userid=%d, logicuid=%d\n", userid, logicuid );
				if(logicuid>0)
					AddLogicuid( userid, logicuid );
			}
		}
		return true;
	}

	fclose(fp);
	return false;
}

bool ImportCsvRole( const char * filename )
{
	FILE* fp = fopen(filename, "r");
	if(fp)
	{
		char buf[4096];
		while(fgets(buf, sizeof(buf), fp))
		{
			if(buf[0]=='#')
				continue;
			int zoneid, roleid, status, time;
			char name[2048];
			memset(name,0,sizeof(name));

			std::string src = buf;
			std::vector<std::string> r;
			if( GNET::ParseStrings( src, r ) && r.size() >= 5 )
			{
				int len = r.size();
				zoneid = atoi( r[len-4].c_str() );
				roleid = atoi( r[len-3].c_str() );
				status = atoi( r[len-2].c_str() );
				time   = atoi( r[len-1].c_str() );
				strcat( name, r[0].c_str() );
				for( int i=1; i<len-4; i++ )
				{
					strcat( name, "," );
					strcat( name, r[i].c_str() );
				}

				Octets name2( name, strlen(name) );
				Octets name3;
				UnescapeCSVString( name2, name3 );
				memset(name,0,sizeof(name));
				memcpy(name,name3.begin(),name3.size());
				printf( "AddRole name=%s, zoneid=%d, roleid=%d, status=%d, time=%d\n", name, zoneid, roleid, status, time );
				AddRole( name, zoneid, roleid, status, time );
			}
		}
		return true;
	}

	fclose(fp);
	return false;
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
		return true;
	}
	else
	{
		return false;
	}

	fclose(fp);
	return false;
}

bool ImportCsvFaction( const char * filename )
{
	FILE* fp = fopen(filename, "r");
	if(fp)
	{
		char buf[4096];
		while(fgets(buf, sizeof(buf), fp))
		{
			if(buf[0]=='#')
				continue;
			int zoneid, fid, status, time;
			char name[2048];
			memset(name,0,sizeof(name));

			std::string src = buf;
			std::vector<std::string> r;
			if( GNET::ParseStrings( src, r ) && r.size() >= 5 )
			{
				int len = r.size();
				zoneid = atoi( r[len-4].c_str() );
				fid = atoi( r[len-3].c_str() );
				status = atoi( r[len-2].c_str() );
				time   = atoi( r[len-1].c_str() );
				strcat( name, r[0].c_str() );
				for( int i=1; i<len-4; i++ )
				{
					strcat( name, "," );
					strcat( name, r[i].c_str() );
				}

				Octets name2( name, strlen(name) );
				Octets name3;
				UnescapeCSVString( name2, name3 );
				memset(name,0,sizeof(name));
				memcpy(name,name3.begin(),name3.size());
				printf( "AddFaction name=%s, zoneid=%d, fid=%d, status=%d, time=%d\n", name, zoneid, fid, status, time );
				AddFaction( name, zoneid, fid, status, time );
			}
		}
		return true;
	}

	fclose(fp);
	return false;
}

bool ImportCsvFamily( const char * filename )
{
	FILE* fp = fopen(filename, "r");
	if(fp)
	{
		char buf[4096];
		while(fgets(buf, sizeof(buf), fp))
		{
			if(buf[0]=='#')
				continue;
			int zoneid, fid, status, time;
			char name[2048];
			memset(name,0,sizeof(name));

			std::string src = buf;
			std::vector<std::string> r;
			if( GNET::ParseStrings( src, r ) && r.size() >= 5 )
			{
				int len = r.size();
				zoneid = atoi( r[len-4].c_str() );
				fid = atoi( r[len-3].c_str() );
				status = atoi( r[len-2].c_str() );
				time   = atoi( r[len-1].c_str() );
				strcat( name, r[0].c_str() );
				for( int i=1; i<len-4; i++ )
				{
					strcat( name, "," );
					strcat( name, r[i].c_str() );
				}

				Octets name2( name, strlen(name) );
				Octets name3;
				UnescapeCSVString( name2, name3 );
				memset(name,0,sizeof(name));
				memcpy(name,name3.begin(),name3.size());
				printf( "AddFamily name=%s, zoneid=%d, fid=%d, status=%d, time=%d\n", name, zoneid, fid, status, time );
				AddFamily( name, zoneid, fid, status, time );
			}
		}
		return true;
	}

	fclose(fp);
	return false;
}

class MergeDBQuery : public StorageEnv::IQuery
{
public:
	std::string	destdbname;
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage(destdbname.c_str());
			try
			{
				if( 0 == strcmp("uidrole",destdbname.c_str()) )
				{
					Marshal::OctetsStream	key_os(key), value_new, value1, value2(value);
					if( pstorage->find( key, value1, txn ) )
					{
						int userid;
						key_os >> userid;
						unsigned int roles1, roles2;

						int logicuid1, logicuid2;
						try {
							value1 >> roles1 >> logicuid1;
						} catch ( Marshal::Exception & )	{	logicuid1 = -1;	}

						try {
							value2 >> roles2 >> logicuid2;
						} catch ( Marshal::Exception & )	{	logicuid2 = -1;	}


						if( userid != 16 && ROLELIST_DEFAULT != (roles1 & roles2) )
							printf( "user duplicates, userid=%d, old=%x, new=%x.\n", userid, roles1, roles2 );
						if( userid != 16 && logicuid1 != logicuid2 )
							printf( "user conficts, userid=%d, logicuid1=%d, logicuid2=%d.\n", userid, logicuid1, logicuid2 );

						roles1 |= roles2;
						value_new << roles1;
						if( logicuid1 != -1 )
							value_new << logicuid1;
						pstorage->insert( key, value_new, txn );
					}
					else
						pstorage->insert( key, value, txn );
				}
				else
				{
					Marshal::OctetsStream value1, value2(value);
					if( pstorage->find( key, value1, txn ) )
					{
						if( 0 == strcmp("logicuid",destdbname.c_str()) )
						{
							int logicuid, userid1, userid2;
							Marshal::OctetsStream key_os(key);
							key_os >> logicuid;
							value1 >> userid1;
							value2 >> userid2;
							if( userid1 != userid2 )
								printf( "logicuid(logicuid=%d,userid1=%d,userid2=%d) not equal.\n",
										logicuid, userid1, userid2 );
						}
						else if( 0 == strcmp("unamerole",destdbname.c_str()) )
						{
							Octets name;
							CharsetConverter::conv_charset_u2l( key, name );
							int temp, roleid1, roleid2;
							value1 >> temp >> roleid1 >> temp >> temp;
							value2 >> temp >> roleid2 >> temp >> temp;
							printf( "rolename(content=%.*s,id1=%d,id2=%d) already exists.\n",
									name.size(),(char*)name.begin(), roleid1, roleid2 );
						}
						else if( 0 == strcmp("unamefaction",destdbname.c_str()) )
						{
							Octets name;
							CharsetConverter::conv_charset_u2l( key, name );
							int temp, fid1, fid2;
							value1 >> temp >> fid1 >> temp >> temp;
							value2 >> temp >> fid2 >> temp >> temp;
							printf( "factionname(content=%.*s,id1=%d,id2=%d) already exists.\n",
									name.size(),(char*)name.begin(), fid1, fid2 );
						}
						else if( 0 == strcmp("unamefamily",destdbname.c_str()) )
						{
							Octets name;
							CharsetConverter::conv_charset_u2l( key, name );
							int temp, fid1, fid2;
							value1 >> temp >> fid1 >> temp >> temp;
							value2 >> temp >> fid2 >> temp >> temp;
							printf( "familyname(content=%.*s,id1=%d,id2=%d) already exists.\n",
									name.size(),(char*)name.begin(), fid1, fid2 );
						}
						else
							printf( "fatal duplicate error.\n" );
					}
					else
						pstorage->insert( key, value, txn );
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
			Log::log( LOG_ERR, "MergeDBQuery, what=%s\n", e.what() );
		}
		return true;
	}
};

void MergeDB( const char * srcpath, const char * srcdbname, const char * destdbname )
{
	printf( "\nmerge database %s/%s to %s:\n", srcpath, srcdbname, destdbname );

	std::string src_dir = srcpath;

	MergeDBQuery q;
	q.destdbname = destdbname;
	try
	{
		DBStandalone * pstandalone = new DBStandalone( (src_dir+"/"+srcdbname).c_str() );
		pstandalone->init();
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

void MergeDBAll( const char * srcpath )
{
	MergeDB( srcpath, "logicuid", "logicuid" );
	MergeDB( srcpath, "uidrole", "uidrole" );
	MergeDB( srcpath, "unamerole", "unamerole" );
	MergeDB( srcpath, "unamefaction", "unamefaction" );
	MergeDB( srcpath, "unamefamily", "unamefamily" );

	StorageEnv::checkpoint();
	StorageEnv::Close();

	StorageEnv::Open();
	StorageEnv::checkpoint( );
	StorageEnv::removeoldlogs( );

	printf( "\nMERGE FINISHED. \n" );
}

class PreAllocateQuery : public StorageEnv::IQuery
{
public:
	int dupname_count;
	int preallocate_count;
	time_t now;
	std::string	dbname;
	PreAllocateQuery() : dupname_count(0), preallocate_count(0) { now = time(NULL); }
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			StorageEnv::Storage * pstorage = StorageEnv::GetStorage(dbname.c_str());
			try
			{
				Marshal::OctetsStream value1, value2(value), valuename;
				if( pstorage->find( key, value1, txn ) )
					dupname_count++; //表中已经有 无需再allocate
				else
				{
					int zoneid, id, status, time;
					//表的value格式必须如此
					value2 >> zoneid >> id >> status >> time; 
					if (status == UNIQUENAME_USED) //只处理正在被使用的名字
					{
						status = UNIQUENAME_PREALLOCATE;
						valuename << zoneid << id << status << (int)now;
						pstorage->insert( key, valuename, txn );
						preallocate_count++;
					}
					else
					{
						Octets name;
						CharsetConverter::conv_charset_u2l( key, name );
						LOG_TRACE( "name(%.*s) id=%d zoneid=%d status=%d",
								name.size(),(char*)name.begin(), id, zoneid, status);
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
			Log::log( LOG_ERR, "PreAllocateQuery, what=%s\n", e.what() );
		}
		return true;
	}
};

void _PreAllocateName( const char * srcpath, const char * dbname)
{
	LOG_TRACE("preallocate database %s/%s to %s:", srcpath, dbname, dbname);
	std::string src_dir = srcpath;
	PreLoadTable((src_dir+"/"+dbname).c_str());

	PreAllocateQuery q;
	q.dbname = dbname;
	try
	{
		DBStandalone * pstandalone = new DBStandalone( (src_dir+"/"+dbname).c_str() );
		pstandalone->init();
		StorageEnv::AtomTransaction	txn;
		try
		{
			StorageEnv::Storage::Cursor cursor(&txn,(src_dir+"/"+dbname).c_str(),pstandalone,new StorageEnv::Uncompressor());
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
		Log::log( LOG_ERR, "_PreAllocateName, error when walk, what=%s\n", e.what() );
	}
	LOG_TRACE("dupnamecount %d preallocatecount %d", q.dupname_count, q.preallocate_count);
	StorageEnv::checkpoint( );
}

void PreAllocateName( const char * srcpath )
{
	_PreAllocateName( srcpath, "unamerole");
	_PreAllocateName( srcpath, "unamefaction");
	_PreAllocateName( srcpath, "unamefamily");

	StorageEnv::checkpoint();
	StorageEnv::Close();

	StorageEnv::Open();
	StorageEnv::checkpoint( );
	StorageEnv::removeoldlogs( );
}

};

