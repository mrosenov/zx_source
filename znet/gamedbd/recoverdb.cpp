#include "conf.h"
#include "log.h"
#include "storage.h"
#include "storagetool.h"
#include "xmlversion.h"
#include "conv_charset.h"
#include "localmacro.h"
#include "user"
#include "grolestatus"

#include <set>
#include <iostream>
using std::set;
using namespace GNET;
//对付私服 采用对user.flag和status.version写入标记的方式 来使私服定期宕机
//此程序可用于将 user.flag 和 status.version 标记清除
void printhelp( const char * cmd )
{
	std::cerr << "recoverdb version " << XMLVERSION << std::endl << std::endl;
	std::cerr << "Usage: " << cmd << " conf-file" << std::endl;
}

class RecoverUserQuery : public StorageEnv::IQuery
{
private:
	set<int> &userset;
public:
	RecoverUserQuery(set<int> &set) : userset(set) { } 
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int userid;
		User user;
		try
		{
			Marshal::OctetsStream(key) >> userid;
			Marshal::OctetsStream(value) >> user;
			if (user.flag & USER_TABLE_CRUSHED)
				userset.insert(userid);
		}
		catch ( ... )
		{
			Log::log(LOG_ERR, "recover user, marshal exception, userid=%d", userid);
		}
		return true;
	}
};

class RecoverStatusQuery : public StorageEnv::IQuery
{
private:
	set<int> &roleset;
public:
	RecoverStatusQuery(set<int> &set) : roleset(set) { } 
	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		int roleid;
		GRoleStatus status;
		try
		{
			Marshal::OctetsStream(key) >> roleid;
			Marshal::OctetsStream(value) >> status;
			if (status.version & STATUS_TABLE_CRUSHED)
				roleset.insert(roleid);
		}
		catch ( ... )
		{
			Log::log(LOG_ERR, "recover status, marshal exception, userid=%d", roleid);
		}
		return true;
	}
};

void RecoverUser()
{
	LOG_TRACE( "recover table user" );

	set<int> userset;
	RecoverUserQuery q(userset);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "user" );
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
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
		LOG_TRACE("find crushed users %d", userset.size());
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				set<int>::iterator it, ite = userset.end();
				for (it = userset.begin(); it != ite; ++it)
				{
					User user;
					Marshal::OctetsStream(pstorage->find(Marshal::OctetsStream()<<*it, txn)) >> user;
					user.flag &= ~USER_TABLE_CRUSHED;
					pstorage->insert(Marshal::OctetsStream()<<*it, Marshal::OctetsStream()<<user, txn);
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
		Log::log(LOG_ERR, "recover user, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

void RecoverStatus()
{
	LOG_TRACE( "recover table status" );

	set<int> roleset;
	RecoverStatusQuery q(roleset);

	try
	{
		StorageEnv::Storage * pstorage = StorageEnv::GetStorage( "status" );
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				StorageEnv::Storage::Cursor cursor = pstorage->cursor(txn);
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
		LOG_TRACE("find crushed roles %d", roleset.size());
		{
			StorageEnv::CommonTransaction	txn;
			try
			{
				set<int>::iterator it, ite = roleset.end();
				for (it = roleset.begin(); it != ite; ++it)
				{
					GRoleStatus status;
					Marshal::OctetsStream(pstorage->find(Marshal::OctetsStream()<<*it, txn)) >> status;
					status.version &= ~STATUS_TABLE_CRUSHED;
					pstorage->insert(Marshal::OctetsStream()<<*it, Marshal::OctetsStream()<<status, txn);
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
		Log::log(LOG_ERR, "recover status, error what=%s\n", e.what() );
	}
	StorageEnv::checkpoint();
}

int main(int argc, char *argv[])
{
	if (argc < 2 || access(argv[1], R_OK) == -1 )
	{
		printhelp( argv[0] );
		exit(-1);
	}
	Conf::GetInstance(argv[1]);
	Log::setprogname("recoverdb");

	if(!StorageEnv::Open())
	{
		fprintf( stderr,"Initialize storage environment failed.\n");
		exit(-1);
	}
	RecoverUser();
	RecoverStatus();
	StorageEnv::checkpoint();
	StorageEnv::Close();
	return 0;
}
